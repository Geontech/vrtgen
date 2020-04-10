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
from vrtgen.backend.cpp.struct import CppStruct, member_type, format_enable_methods, format_value_methods

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
BACKEND_TEMPLATE_PATH = os.path.join(os.path.dirname(__file__), 'python/vrtgen/backend/cpp/templates')

class LibraryGenerator:
    def __init__(self, includedir):
        self.loader = jinja2.loaders.ChoiceLoader([
            jinja2.FileSystemLoader(TEMPLATE_PATH),
            jinja2.FileSystemLoader(BACKEND_TEMPLATE_PATH)
        ])
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
