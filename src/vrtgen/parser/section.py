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

"""
Classes for parsing sections of a VITA 49 packet definition.
"""
from vrtgen.types.struct import Struct

from vrtgen.parser.field import FieldParser, StructFieldParser, SimpleFieldParser
from vrtgen.parser.mapping import MappingParser
from vrtgen.parser.value import get_value_parser

def bind_parser(name, parser):
    """
    Wraps a field value parser to fetch the target field from the incoming
    context.
    """
    def wrapped_parse(log, context, value):
        field = context.get_field(name)
        assert field is not None
        log.debug("Parsing field '%s'", field.name)
        return parser(log.getChild(field.name), field, value)
    return wrapped_parse

def _bind_value_parser(name, parser):
    def parse_and_set(log, field, value):
        field.value = parser(value)
        log.debug("Field '%s' = %s", field.name, field.value)
    return bind_parser(name, parse_and_set)

class SectionParser(MappingParser):
    """
    Base class for parsers that manage a section of a VITA 49 packet
    definition.

    To define a section parser, create a subclass and register child parsers
    statically with that class.
    """
    @classmethod
    def add_field_parser(cls, field, parser=None, alias=None):
        """
        Registers a configuration parser for a specific VITA 49 field.
        """
        assert field.type is not None
        parser = cls._wrap_field_parser(field, parser)
        name = field.name
        cls.add_parser(name, bind_parser(name, parser), alias)

    @classmethod
    def add_field_value_parser(cls, field, parser=None, alias=None):
        """
        Registers a value parser for a specific VITA 49.2 field.

        The parser must take a YAML scalar value and return a Python value
        compatible with the field's data type.

        Fields registered with this method will only accept a scalar value.
        """
        assert field.type is not None
        name = field.name
        if parser is None:
            parser = get_value_parser(field.type)
        cls.add_parser(name, _bind_value_parser(name, parser), alias)

    @staticmethod
    def _wrap_field_parser(field, parser):
        # Parser is already a field parser, pass through
        if isinstance(parser, FieldParser):
            return parser

        # Otherwise it's a value parser, determine whether to treat it as a
        # struct or a simple
        if issubclass(field.type, Struct):
            cls = StructFieldParser
        else:
            cls = SimpleFieldParser

        # Value parser was given, use it
        if parser is not None:
            return cls(parser)

        # All we have is a field, use the parser factory
        return cls.factory(field)
