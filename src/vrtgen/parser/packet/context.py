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
Context packet parser classes.
"""

from vrtgen.parser.value import parse_tsm
from vrtgen.types.prologue import ContextHeader

from .cif import CIFPacketParser

class ContextPacketParser(CIFPacketParser):
    """
    Parser for Context Packet configuration.
    """
    @staticmethod
    def parse_tsm(log, context, value):
        """
        Parses context packet timestamp mode.
        """
        context.timestamp_mode = parse_tsm(value)
        log.debug('TSM = %s', context.timestamp_mode)

ContextPacketParser.add_parser(
    ContextHeader.timestamp_mode.name,
    ContextPacketParser.parse_tsm,
    alias='TSM'
)
