# Copyright (C) 2019 Geon Technologies, LLC
#
# This file is part of vrtgen.
#
# vrtgen is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# vrtgen is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
# more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.

import argparse
import inspect
import os
import sys

import jinja2

from vrtgen.types import basic
from vrtgen.types import enums
from vrtgen.types import prologue
from vrtgen.types import trailer
from vrtgen.types import control
from vrtgen.types import cif0
from vrtgen.types import cif1
from vrtgen.types import struct
from vrtgen.types import user

from vrtgen.backend.cpp import types as cpptypes
from vrtgen.backend.cpp.jinja import JINJA_OPTIONS
from vrtgen.backend.cpp import utils

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


def tag_name(field):
    return field.attr + '_tag'

def format_enum(enum):
    # Create a format string that returns a hex constant (binary constants are
    # a C++14 feature)
    digits = int((enum.bits + 3) / 4)
    format_string = '0x{{:0{}x}}'.format(digits)
    return {
        'name': enum.__name__,
        'doc': utils.format_docstring(enum.__doc__),
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
        self.doc.append('{} {}'.format(field.name, field.position))

class BasicMember(Member):
    def __init__(self, name, field):
        super().__init__(name, member_type(field.type))
        self.field = field
        self._add_field_doc(field)

class Reserved(Member):
    def __init__(self, name, field):
        super().__init__(name, cpptypes.int_type(field.bits, False))
        self.doc = ['Reserved {}'.format(field.position)]

class Tag:
    def __init__(self, field):
        self.name = tag_name(field)
        self.offset = field.position.bit
        self.basetype = Tag._base_type(field.type)
        self.bits = field.bits
        self.converter = Tag._converter_type(field.type)

    @property
    def type(self):
        template_args = [self.basetype, self.offset, self.bits]
        if self.converter:
            template_args.append(self.converter)
        return 'packed_tag<{}>'.format(','.join(str(arg) for arg in template_args))

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

    @staticmethod
    def _converter_type(datatype):
        if issubclass(datatype, basic.NonZeroSize):
            return 'detail::size_converter'
        return None

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
        assert self.offset - self.bits == field.position.bit
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
        self.doc = utils.format_docstring(structdef.__doc__)
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

    def _should_pack(self, field):
        if field.bits % 8:
            # Field does not necessarily need to be byte-aligned, pack it into
            # a larger data member
            return True
        if not field.editable and self._current_packed is not None:
            # Currently adding fields to a packed member, tack on reserved
            # fields instead of giving them their own member. This avoids a
            # problem in the CAM prologue field, where there are 24 bits worth
            # of packed fields followed by 8 reserved bits, but there is no
            # native 24-bit type to hold the packed field.
            return True
        return False

    def _process_field(self, field):
        align = field.position.offset % 32
        if self._should_pack(field):
            # Pack the field into a larger data member; the field itself does
            # not need to be byte-aligned
            if self._current_packed is None:
                # The data member does have to be byte-aligned
                assert align % 8 == 0
                self._add_packed(field.position.bit)
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
            methods = format_enable_methods(field.enable, member)
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

    def field_to_typedef(self, field):
        return self.create_field_typedef(field.name, field.type)

    @staticmethod
    def create_field_typedef(name, datatype):
        return {
            'name': cpptypes.name_to_identifier(name),
            'type': 'typename field<{}>::type'.format(member_type(datatype)),
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

    def generate_control(self, filename):
        # Minor misnomer: the CIF header emits typedefs, which we want for
        # MessageIdentifier
        template = self.env.get_template('cif.hpp')
        structs = [
            CppStruct(control.ControlAcknowledgeMode),
        ]

        typedefs = [
            self.create_field_typedef('MessageID', control.MessageIdentifier),
            self.create_field_typedef('ControlleeID', basic.Identifier32),
            self.create_field_typedef('ControllerID', basic.Identifier32),
        ]
        with open(filename, 'w') as fp:
            fp.write(template.render({
                'name': 'control',
                'structs': structs,
                'typedefs': typedefs,
            }))

    def generate_cif(self, filename, module, cif):
        structs = []
        enable = CppEnableStruct(cif.Enables)
        # Override name and docstring from parent CIFFields class
        enable.name = cif.__name__ + 'Enables'
        enable.doc = utils.format_docstring(cif.__doc__)
        structs.append(enable)
        for structdef in get_structs(module):
            structs.append(CppStruct(structdef))

        typedefs = []
        for field in cif.get_fields():
            if not field.type:
                continue
            if issubclass(field.type, (basic.IntegerType, basic.FixedPointType)):
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
        ('packing/control.hpp', generate_control),
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
