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
Packet parser classes and helper methods.
"""

from vrtgen.model.config import PacketType

from .command import ControlPacketParser, AckSPacketParser, AckVXPacketParser
from .context import ContextPacketParser
from .data import DataPacketParser

def create_parser(packet_type):
    """
    Returns a parser for the given packet type.
    """
    if packet_type == PacketType.DATA:
        return DataPacketParser()
    if packet_type == PacketType.CONTEXT:
        return ContextPacketParser()
    if packet_type == PacketType.CONTROL:
        return ControlPacketParser()
    if packet_type == PacketType.ACKV:
        return AckSPacketParser()
    if packet_type in (PacketType.ACKX, PacketType.ACKS):
        return AckVXPacketParser()

    raise ValueError("Invalid packet type '{}'".format(packet_type))
