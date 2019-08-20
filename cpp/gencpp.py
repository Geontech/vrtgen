import argparse
import inspect
import os
import sys

import jinja2

from vrtgen.types import basic
from vrtgen.types import enums
from vrtgen.types import prologue
from vrtgen.types import trailer
from vrtgen.types import cif0
from vrtgen.types import cif1
from vrtgen.types import struct

from vrtgen.backend.cpp import types as cpptypes

JINJA_OPTIONS = {
    'trim_blocks': True,
    'lstrip_blocks': True,
    'keep_trailing_newline': True,
    'line_statement_prefix': '//%',
    'block_start_string':    '/*%',
    'block_end_string':      '%*/',
    'comment_start_string':  '/*#',
    'comment_end_string':    '#*/'
}

def is_enum(obj):
    # Ignore anything that isn't a BinaryEnum class
    if not inspect.isclass(obj) or not issubclass(obj, enums.BinaryEnum):
        return False
    # Only return concrete enums (i.e., those that have values defined) to
    # filter out abstract base classes (just BinaryEnum at present)
    return bool(obj.__members__)

def get_structs(module):
    def is_struct(obj):
        if not inspect.isclass(obj) or not issubclass(obj, struct.Struct):
            return False
        return obj.__module__ == module.__name__
    return [cls for _, cls in inspect.getmembers(module, is_struct)]

def member_type(datatype):
    if datatype == basic.OUI:
        return 'OUI'
    if issubclass(datatype, enums.BinaryEnum):
        return cpptypes.enum_type(datatype)
    if issubclass(datatype, basic.IntegerType):
        base_type = cpptypes.int_type(datatype.bits, datatype.signed)
        return 'big_endian<{}>'.format(base_type)
    if issubclass(datatype, basic.FixedPointType):
        return cpptypes.fixed_type(datatype.bits, datatype.radix)
    if issubclass(datatype, basic.NonZeroSize):
        base_type = cpptypes.int_type(datatype.bits, False)
        return 'nonzero_size<{}>'.format(base_type)
    raise NotImplementedError(datatype.__name__)

def format_docstring(doc):
    if not doc:
        return
    if doc.startswith('\n'):
        doc = doc[1:]
    indent = 0
    while doc[indent].isspace():
        indent += 1
    for line in doc.rstrip().split('\n'):
        yield line[indent:]

def tag_name(field):
    return field.attr + '_tag'

def format_enum(enum):
    # Create a format string that returns a binary constant zero-padded to the
    # number of bits, e.g.: "0b0001"
    format_string = '0b{{:0{}b}}'.format(enum.bits)
    return {
        'name': enum.__name__,
        'doc': format_docstring(enum.__doc__),
        'format': format_string.format,
        'values': list(enum)
    }

def format_enable_methods(field, member, name=None):
    if name is None:
        name = field.name
    identifier = cpptypes.name_to_identifier(name + 'Enabled')
    return {
        'name': name,
        'getter': {
            'doc': 'Get enabled state of ' + name,
            'name' : 'is'+identifier,
        },
        'setter': {
            'doc': 'Set enabled state of ' + name,
            'name' : 'set'+identifier,
        },
        'type': 'bool',
        'member': member,
        'tag': tag_name(field)
    }

def format_value_methods(field, member):
    identifier = cpptypes.name_to_identifier(field.name)
    field_data = {
        'name': field.name,
        'getter': {
            'doc': 'Get current value of ' + field.name,
            'name' : 'get'+identifier,
        },
        'setter': {
            'doc': 'Set current value of ' + field.name,
            'name' : 'set'+identifier,
        },
        'member': member,
        'type': cpptypes.value_type(field.type),
        'bits': field.type.bits,
    }

    if isinstance(member, Packed):
        field_data['tag'] = tag_name(field)

    return field_data

class Member:
    def __init__(self, name, datatype):
        self.name = 'm_' + name
        self.type = datatype
        self.doc = []

    @property
    def decl(self):
        assert self.type is not None
        return '{} {}'.format(self.type, self.name)

    def _add_field_doc(self, field):
        self.doc.append('{} {}/{}'.format(field.name, field.word, field.offset))

class BasicMember(Member):
    def __init__(self, name, field):
        super().__init__(name, member_type(field.type))
        self.field = field
        self._add_field_doc(field)

class Reserved(Member):
    def __init__(self, name, field):
        super().__init__(name, cpptypes.int_type(field.bits, False))
        self.doc = ['Reserved {}/{}'.format(field.word, field.offset)]

class Tag:
    def __init__(self, field):
        self.name = tag_name(field)
        self.offset = field.offset
        self.basetype = Tag._base_type(field.type)
        self.bits = field.bits

    @property
    def type(self):
        return 'packed_tag<{},{},{}>'.format(self.basetype, self.offset, self.bits)

    @staticmethod
    def _base_type(datatype):
        if issubclass(datatype, basic.IntegerType):
            # Use unsized types that only signify signed/unsigned since the size
            # is included in the template arguments. This simplifies the
            # implementation of sign extension.
            if datatype.signed:
                return 'signed'
            else:
                return 'unsigned'
        return cpptypes.value_type(datatype)

class Packed(Member):
    def __init__(self, name, offset):
        super().__init__(name, None)
        self.offset = offset
        self.bits = 0
        self.tags = []

    def full(self):
        assert self.offset >= 0
        return (1 + self.offset - self.bits) == 0

    def close(self):
        # Limit the offset of tags to fall within the bit size of the packed
        # container.
        assert self.bits in (8, 16, 32)
        self.type = self.packed_type(self.bits)
        for tag in self.tags:
            tag.offset = tag.offset % self.bits

    def link_field(self, field):
        # Check for wraparound into the next word
        assert self.offset - self.bits == field.offset
        self.bits += field.bits
        self._add_field_doc(field)
        if not isinstance(field, struct.Reserved):
            tag = Tag(field)
            self.tags.append(tag)

    @staticmethod
    def packed_type(bits):
        return 'packed<{}>'.format(cpptypes.int_type(bits, False))

class CppStruct:
    def __init__(self, structdef):
        self.name = structdef.__name__
        self.doc = format_docstring(structdef.__doc__)
        self.fields = []
        self.members = []
        self._current_packed = None
        for field in structdef.get_contents():
            self._process_field(field)

    @property
    def packed(self):
        return [f for f in self.members if isinstance(f, Packed)]

    @property
    def reserved(self):
        return [f for f in self.members if isinstance(f, Reserved)]

    def _process_field(self, field):
        align = 31 - field.offset
        if field.bits % 8:
            # Field does not necessarily need to be byte-aligned, pack it into
            # a larger data member
            if self._current_packed is None:
                # The data member does have to be byte-aligned
                assert align % 8 == 0
                self._add_packed(field.offset)
            member = self._current_packed
            member.link_field(field)
        else:
            # Everything else should be byte-aligned
            assert align % 8 == 0
            # There is a packed field being collected, "close" it and add the
            # member variable
            if self._current_packed:
                self._close_packed()

            if field.bits % 32 == 0:
                assert align == 0
            elif field.bits != 24:
                assert align % field.bits == 0

            # Non-editable fields large enough to require their own member must
            # be reserved bits. These are handled differently so the compiler
            # doesn't issue warnings about unused private fields.
            if not field.editable:
                name = 'reserved_{}'.format(len(self.reserved))
                self.members.append(Reserved(name, field))
                return

            member = self._add_member(field)

        if field.editable:
            self._map_field(field, member)

        if self._current_packed and self._current_packed.full():
            self._close_packed()

    def _add_packed(self, offset):
        name = 'packed_' + str(len(self.packed))
        self._current_packed = Packed(name, offset)
        self.members.append(self._current_packed)

    def _close_packed(self):
        self._current_packed.close()
        self._current_packed = None

    def _add_member(self, field):
        name = cpptypes.name_to_identifier(field.name)
        member = BasicMember(name, field)
        self.members.append(member)
        return member

    def _map_field(self, field, member):
        if field.enable is not None:
            methods = format_enable_methods(field.enable, member, name=field.name)
            self.fields.append(methods)
        methods = format_value_methods(field, member)
        self.fields.append(methods)

class CppHeaderStruct(CppStruct):
    def _map_field(self, field, member):
        if field == prologue.Header.class_id_enable:
            methods = format_enable_methods(field, member)
            self.fields.append(methods)
        else:
            super()._map_field(field, member)

class CppEnableStruct(CppStruct):
    # Special fields that are already defined enables in the CIF prologue,
    # and so can be treated as values.
    FIELDS = (
        cif0.CIF0.Enables.change_indicator,
        cif0.CIF0.Enables.cif7_enable,
        cif0.CIF0.Enables.cif3_enable,
        cif0.CIF0.Enables.cif2_enable,
        cif0.CIF0.Enables.cif1_enable,
    )
    def _map_field(self, field, member):
        if field in self.FIELDS:
            methods = format_value_methods(field, member)
        else:
            methods = format_enable_methods(field, member)
        self.fields.append(methods)

TEMPLATE_PATH = os.path.join(os.path.dirname(__file__), 'templates')

class LibraryGenerator:
    def __init__(self, includedir):
        self.loader = jinja2.FileSystemLoader(TEMPLATE_PATH)
        self.env = jinja2.Environment(loader=self.loader, **JINJA_OPTIONS)
        self.includedir = includedir

    def generate_enums(self, filename):
        template = self.env.get_template('enums.hpp')
        enum_types = [format_enum(en) for _, en in inspect.getmembers(enums, is_enum)]

        with open(filename, 'w') as fp:
            fp.write(template.render({'enums': enum_types}))

    @staticmethod
    def field_to_typedef(field):
        return {
            'name': cpptypes.name_to_identifier(field.name),
            'type': 'field<{}>'.format(member_type(field.type)),
        }

    def generate_header(self, filename):
        # Minor misnomer: the CIF header emits typedefs, which we want for
        template = self.env.get_template('cif.hpp')
        structs = [
            CppHeaderStruct(prologue.Header),
            CppHeaderStruct(prologue.DataHeader),
            CppHeaderStruct(prologue.ContextHeader),
            CppHeaderStruct(prologue.CommandHeader),
            CppStruct(prologue.ClassIdentifier)
        ]

        typedefs = [
            self.field_to_typedef(prologue.Prologue.stream_id),
            self.field_to_typedef(prologue.Prologue.integer_timestamp),
            self.field_to_typedef(prologue.Prologue.fractional_timestamp),
        ]
        with open(filename, 'w') as fp:
            fp.write(template.render({
                'name': 'header',
                'structs': structs,
                'typedefs': typedefs,
            }))

    def generate_trailer(self, filename):
        template = self.env.get_template('struct.hpp')
        with open(filename, 'w') as fp:
            fp.write(template.render({
                'name': 'trailer',
                'structs': [CppStruct(trailer.Trailer)],
            }))

    def generate_cif(self, filename, module, cif):
        structs = []
        enable = CppEnableStruct(cif.Enables)
        # Override name and docstring from parent CIFFields class
        enable.name = cif.__name__ + 'Enables'
        enable.doc = format_docstring(cif.__doc__)
        structs.append(enable)
        for structdef in get_structs(module):
            structs.append(CppStruct(structdef))

        typedefs = []
        for field in cif.get_fields():
            if not field.type or not issubclass(field.type, (basic.IntegerType, basic.FixedPointType)):
                continue
            typedefs.append(self.field_to_typedef(field))

        template = self.env.get_template('cif.hpp')
        with open(filename, 'w') as fp:
            fp.write(template.render({
                'name': cif.__name__,
                'structs': structs,
                'typedefs': typedefs,
            }))

    def headers(self):
        for target in self.TARGETS:
            name = target[0]
            yield os.path.join(self.includedir, name)

    def generate(self):
        for target in self.TARGETS:
            name, method = target[:2]
            args = target[2:]
            filename = os.path.join(self.includedir, name)
            dirname = os.path.dirname(filename)
            os.makedirs(dirname, exist_ok=True)
            method(self, filename, *args)

    TARGETS = [
        ('enums.hpp', generate_enums),
        ('packing/trailer.hpp', generate_trailer),
        ('packing/header.hpp', generate_header),
        ('packing/cif0.hpp', generate_cif, cif0, cif0.CIF0),
        ('packing/cif1.hpp', generate_cif, cif1, cif1.CIF1),
    ]


def main(args):
    parser = argparse.ArgumentParser(prog='gencpp')
    parser.add_argument('command', choices=('headers', 'generate'))

    opts = parser.parse_args(args)

    includedir = 'include/vrtgen'
    gen = LibraryGenerator(includedir)

    if opts.command == 'generate':
        gen.generate()
    elif opts.command == 'headers':
        for name in gen.headers():
            print(name)

if __name__ == '__main__':
    main(sys.argv[1:])
