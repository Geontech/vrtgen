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
from vrtgen.types.struct import Struct

JINJA_OPTIONS = {
    'trim_blocks':           True,
    'line_statement_prefix': '//%',
    'variable_start_string': '${',
    'variable_end_string':   '}',
    'block_start_string':    '/*{%',
    'block_end_string':      '%}*/',
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
        if not inspect.isclass(obj) or not issubclass(obj, Struct):
            return False
        return obj.__module__ == module.__name__
    return [cls for _, cls in inspect.getmembers(module, is_struct)]

def int_type(bits, signed):
    if bits > 32:
        ctype = 'int64_t'
    elif bits > 16:
        ctype = 'int32_t'
    elif bits > 8:
        ctype = 'int16_t'
    else:
        ctype = 'int8_t'
    if not signed:
        ctype = 'u' + ctype
    return ctype

def fixed_type(bits, radix):
    return 'fixed<{},{}>'.format(bits, radix)

def enum_type(datatype):
    name = name_to_identifier(datatype.__name__)
    return 'vrtgen::{}::Code'.format(name)

def cpp_type(datatype):
    if issubclass(datatype, enums.BinaryEnum):
        return enum_type(datatype)
    if issubclass(datatype, basic.IntegerType):
        return int_type(datatype.bits, datatype.signed)
    if issubclass(datatype, basic.FixedPointType):
        return fixed_type(datatype.bits, datatype.radix) + '::int_type'
    if datatype == basic.Boolean:
        return 'bool'
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

def name_to_identifier(name):
    identifier = ''
    for ch in name:
        if ch.isalnum():
            identifier += ch
        elif ch in '.':
            identifier += '_'
    return identifier

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
    identifier = name_to_identifier(name + 'Enabled')
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
        'word': field.word,
        'offset': field.offset,
        'type': 'bool',
        'member': member.name,
    }

def format_value_methods(field, member):
    identifier = name_to_identifier(field.name)
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
        'word': field.word,
        'offset': field.offset,
        'member': member.name,
    }
    datatype = field.type
    if issubclass(datatype, enums.BinaryEnum):
        field_data['type'] = enum_type(datatype)
        field_data['bits'] = datatype.bits
    elif issubclass(datatype, basic.IntegerType):
        field_data['type'] = int_type(datatype.bits, datatype.signed)
        field_data['bits'] = datatype.bits
    elif issubclass(datatype, basic.FixedPointType):
        field_data['type'] = fixed_type(datatype.bits, datatype.radix) + '::float_type'
        field_data['bits'] = datatype.bits
        field_data['radix'] = datatype.radix
    elif datatype == basic.Boolean:
        field_data['type'] = 'bool'
        field_data['bits'] = 1
    return field_data

class Member:
    def __init__(self, name):
        self.name = 'm_' + name
        self.doc = []

    @property
    def decl(self):
        return '{} {}'.format(self.type, self.name)

    def _add_field_doc(self, field):
        self.doc.append('{} {}/{}'.format(field.name, field.word, field.offset))

class BasicMember(Member):
    def __init__(self, name, field):
        super().__init__(name)
        self.field = field
        # TODO: Fix hack to use a C-style packed field for 24-bit ints
        if field.bits == 24:
            if field.type.signed:
                self.type = 'int'
            else:
                self.type = 'unsigned'
        else:
            self.type = cpp_type(field.type)
        self._add_field_doc(field)

    @property
    def decl(self):
        decl = super().decl
        # TODO: See above
        if self.field.bits == 24:
            decl += ':24'
        return decl

class Reserved:
    def __init__(self, field):
        self.name = ''
        self.type = 'int'
        self.decl = '{} {}:{}'.format(self.type, self.name, field.bits)
        self.doc = ['Reserved {}/{}'.format(field.word, field.offset)]

class Packed(Member):
    def __init__(self, name, offset):
        super().__init__(name)
        self.offset = offset
        self.bits = 0

    @property
    def type(self):
        return int_type(self.bits, False)

    def full(self):
        assert self.offset >= 0
        return (1 + self.offset - self.bits) == 0

    def link_field(self, field):
        # Check for wraparound into the next word
        assert self.offset - self.bits == field.offset
        self.bits += field.bits
        self._add_field_doc(field)

class CppStruct:
    def __init__(self, structdef):
        self.name = structdef.__name__
        self.doc = format_docstring(structdef.__doc__)
        self.fields = []
        self.members = []
        self._packed = None
        self._packed_fields = 0
        for field in structdef.get_contents():
            self._process_field(field)

    def _process_field(self, field):
        align = 31 - field.offset
        if field.bits % 8:
            # Field does not necessarily need to be byte-aligned, pack it into
            # a larger data member
            if self._packed is None:
                # The data member does have to be byte-aligned
                assert align % 8 == 0
                self._add_packed(field.offset)
            member = self._packed
            remain = self._packed.offset - self._packed.bits
            member.link_field(field)
        else:
            # Everything else should be byte-aligned
            assert align % 8 == 0
            # There is a packed field being collected, "close" it and add the
            # member variable
            if self._packed:
                self._close_packed()

            if field.bits % 32 == 0:
                assert align == 0
            elif field.bits != 24:
                assert align % field.bits == 0

            # Non-editable fields large enough to require their own member must
            # be reserved bits. These are handled differently so the compiler
            # doesn't issue warnings about unused private fields.
            if not field.editable:
                self.members.append(Reserved(field))
                return

            member = self._add_member(field)

        if field.editable:
            self._map_field(field, member)

        if self._packed and self._packed.full():
            self._close_packed()

    def _add_packed(self, offset):
        name = 'packed_' + str(self._packed_fields)
        self._packed_fields += 1
        self._packed = Packed(name, offset)
        self.members.append(self._packed)

    def _close_packed(self):
        assert self._packed.bits % 8 == 0
        self._packed = None

    def _add_member(self, field):
        name = name_to_identifier(field.name)
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

def generate_enums(env, filename):
    template = env.get_template('enums.hpp')
    enum_types = [format_enum(en) for _, en in inspect.getmembers(enums, is_enum)]

    with open(filename, 'w') as fp:
        fp.write(template.render({'enums': enum_types}))

def generate_header(env, filename):
    template = env.get_template('struct.hpp')
    with open(filename, 'w') as fp:
        structs = [
            CppHeaderStruct(prologue.Header),
            CppHeaderStruct(prologue.DataHeader),
            CppHeaderStruct(prologue.ContextHeader),
            CppHeaderStruct(prologue.CommandHeader),
            CppStruct(prologue.ClassIdentifier)
        ]
        fp.write(template.render({
            'name': 'header',
            'structs': structs,
        }))

def generate_trailer(env, filename):
    template = env.get_template('struct.hpp')
    with open(filename, 'w') as fp:
        fp.write(template.render({
            'name': 'trailer',
            'structs': [CppStruct(trailer.Trailer)],
        }))

def generate_cif(env, module, cif, filename):
    template = env.get_template('struct.hpp')
    with open(filename, 'w') as fp:
        structs = []
        enable = CppEnableStruct(cif.Enables)
        # Override name and docstring from parent CIFFields class
        enable.name = cif.__name__ + 'Enables'
        enable.doc = format_docstring(cif.__doc__)
        structs.append(enable)
        for structdef in get_structs(module):
            structs.append(CppStruct(structdef))
        fp.write(template.render({
            'name': cif.__name__,
            'structs': structs,
        }))

def main():
    loader = jinja2.FileSystemLoader(os.path.join(os.path.dirname(__file__), 'templates'))
    env = jinja2.Environment(loader=loader, **JINJA_OPTIONS)

    includedir = 'include/vrtgen'
    os.makedirs(includedir, exist_ok=True)
    generate_enums(env, 'include/vrtgen/enums.hpp')

    includedir = 'include/vrtgen/packing'
    os.makedirs(includedir, exist_ok=True)
    generate_header(env, os.path.join(includedir, 'header.hpp'))
    generate_trailer(env, os.path.join(includedir, 'trailer.hpp'))
    generate_cif(env, cif0, cif0.CIF0, os.path.join(includedir, 'cif0.hpp'))
    generate_cif(env, cif1, cif1.CIF1, os.path.join(includedir, 'cif1.hpp'))

if __name__ == '__main__':
    main()
