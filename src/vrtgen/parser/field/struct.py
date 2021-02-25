# Copyright (C) 2020 Geon Technologies, LLC
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

"""
Parsers for handling struct fields.
"""

from vrtgen.types.prologue import ClassIdentifier

from vrtgen.parser.field.base import FieldParser
from vrtgen.parser.field.simple import SimpleFieldParser

class StructFieldParser(FieldParser):
    """
    Parser for handling struct field configuration.

    Binds struct value parsing with base field configuration.
    """
    def __init__(self, parser):
        self.parser = parser

    def parse_mapping(self, log, field, mapping):
        self.parser(log, field, mapping)

    @classmethod
    def factory(cls, field):
        """
        Creates a struct parser for a given VITA 49 field.
        """
        parser = StructValueParser(field.type)
        return cls(parser)

class StructValueParser:
    """
    Parser for struct values.

    Struct values in YAML must be a mapping, where each key corresponds to a
    subfield.
    """
    def __init__(self, struct):
        super().__init__()
        self._parsers = {}
        self._aliases = {}
        for field in struct.get_fields():
            self._parsers[field.name.casefold()] = SimpleFieldParser.factory(field)

    def __call__(self, log, context, value):
        if not isinstance(value, dict):
            raise TypeError('Struct values must be a dictionary')

        for field_name, field_value in value.items():
            name = self.get_field_name(field_name)
            try:
                parser = self.get_field_parser(name)
            except KeyError:
                log.error("Invalid field '%s'", field_name)
                continue

            # If parser lookup succeeded, field lookup must succeed
            field = context.get_field(name)
            assert field is not None

            parser(log.getChild(field.name), field, field_value)

    def add_alias(self, name, alias):
        """
        Registers an alias for a field name.
        """
        self._aliases[alias.casefold()] = name

    def get_field_name(self, name):
        """
        Resolves an alias for a field name.
        """
        return self._aliases.get(name.casefold(), name)

    def get_field_parser(self, name):
        """
        Returns the parser for a field.
        """
        parser = self._parsers.get(name.casefold(), None)
        if parser is None:
            raise KeyError(name)
        return parser

class ClassIDParser(StructValueParser):
    """
    Parser for ClassIdentifier structs.
    """
    def __init__(self):
        super().__init__(ClassIdentifier)
        self.add_alias(ClassIdentifier.oui.name, 'OUI')
