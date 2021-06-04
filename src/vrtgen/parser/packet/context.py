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
Context packet parser classes.
"""

from vrtgen.types.prologue import ContextHeader

from .base import PacketParser
from .cif import CIFParser

class ContextPacketParser(PacketParser):
    """
    Parser for Context Packet configuration.
    """

ContextPacketParser.add_field_value_parser(ContextHeader.timestamp_mode, alias='tsm')
ContextPacketParser.add_field_value_parser(ContextHeader.not_v49d0, alias='not-v49d0')
ContextPacketParser.add_parser('cif0', CIFParser())
ContextPacketParser.add_parser('cif1', CIFParser())
