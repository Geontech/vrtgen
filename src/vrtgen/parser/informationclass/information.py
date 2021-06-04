# Copyright (C) 2021 Geon Technologies, LLC
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
Parser classes for generating VITA 49 information class configurations from
Python dictionaries.
"""

from vrtgen.parser.section import SectionParser

class PacketClassesParser(SectionParser):
    """
    Parser for packet classes list.
    """
    def parse_mapping(self, log, context, mapping):
        """
        Configures a context object from the values in th extnds list.
        """
        for packet_class in mapping:
            context.add_packet_class(packet_class)

    def __call__(self, log, context, value):
        if not isinstance(value, list):
            raise TypeError('packet-classes must be a list')
        self.parse_mapping(log, context, value)

class InformationClassParser(SectionParser):
    """
    Parser for information class configuration.
    """

InformationClassParser.add_parser('packet-classes', PacketClassesParser())
