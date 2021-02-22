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
Parser classes for generating VITA 49 packet configurations from Python
dictionaries.
"""

from vrtgen.parser import field
from vrtgen.parser.mapping import MappingParser
from vrtgen.parser.section import SectionParser
from vrtgen.parser import value as value_parser
from vrtgen.types.prologue import Prologue

def unimplemented_parser(name):
    """
    Creates a parser function that logs a 'not implemented' warning.
    """
    def parser(log, *args):
        # pylint: disable=unused-argument
        log.warn('%s not implemented', name)
    return parser

class TimestampParser(MappingParser):
    """
    Parser for timestamp configuration.

    Only the TSI and TSF modes are configurable, under the names "integer" and
    "fractional".
    """
    @staticmethod
    def parse_integer(log, context, value):
        """
        Parses integer timestamp mode.
        """
        context.tsi = value_parser.parse_tsi(value)
        log.debug('TSI = %s', context.tsi)

    @staticmethod
    def parse_fractional(log, context, value):
        """
        Parses fractional timestamp mode.
        """
        context.tsf = value_parser.parse_tsf(value)
        log.debug('TSF = %s', context.tsf)

TimestampParser.add_parser('integer', TimestampParser.parse_integer)
TimestampParser.add_parser('fractional', TimestampParser.parse_fractional)

class ExtendsParser(SectionParser):
    """
    Parser for packet class extension list.
    """
    def parse_mapping(self, log, context, mapping):
        """
        Configures a context object from the values in th extnds list.
        """
        for packet_class in mapping:
            context.extends.append(packet_class)

    def __call__(self, log, context, value):
        if not isinstance(value, list):
            raise TypeError('extends must be a list')
        self.parse_mapping(log, context, value)

class PacketParser(SectionParser):
    """
    Base parser for packet configuration.
    """

PacketParser.add_field_parser(Prologue.stream_id, alias='stream-id')
PacketParser.add_field_parser(Prologue.class_id, field.ClassIDParser(), alias='class-id')
PacketParser.add_parser('timestamp', TimestampParser())
PacketParser.add_parser('extends', ExtendsParser())
