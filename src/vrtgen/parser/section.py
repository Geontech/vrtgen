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

from .field import StructFieldParser, SimpleFieldParser
from .mapping import MappingParser

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
        Registers a parser for a specific VITA 49 field.
        """
        assert field.type is not None
        if issubclass(field.type, Struct):
            if parser is None:
                parser = StructFieldParser.factory(field)
            else:
                parser = StructFieldParser(parser)
        else:
            if parser is None:
                parser = SimpleFieldParser.factory(field)
            else:
                parser = SimpleFieldParser(parser)
        name = field.name
        cls.add_parser(name, bind_parser(name, parser), alias)
