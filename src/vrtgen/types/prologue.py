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
Types used for VITA 49 packet prologues.
"""

from .struct import Struct, Field, Reserved
from . import basic, enums

class ClassIdentifier(Struct):
    """
    Class Identifier (5.1.3).
    """
    pad_bits = Field('Pad Bit Count', basic.IntegerType.create(5, signed=False))
    reserved_1 = Reserved(3)
    oui = Field('Organizationally Unique Identifier', basic.OUI)
    information_code = Field('Information Class Code', basic.Identifier16)
    packet_code = Field('Packet Class Code', basic.Identifier16)

class Header(Struct):
    """
    VRT Packet Header (5.1.1).
    """
    packet_type = Field('Packet Type', enums.PacketType)
    class_id_enable = Field('Class Identifier', basic.Boolean)
    packet_specific_26 = Reserved(1)
    packet_specific_25 = Reserved(1)
    packet_specific_24 = Reserved(1)
    tsi = Field('TSI', enums.TSI)
    tsf = Field('TSF', enums.TSF)
    packet_count = Field('Packet Count', basic.IntegerType.create(4, signed=False))
    packet_size = Field('Packet Size', basic.UInteger16)

class DataHeader(Header):
    """
    VRT Packet Header with Data Packet-Specific Indicator Bits (5.1.1.1)
    """
    trailer_included = Header.packet_specific_26.rebind(
        Field('Trailer Included', basic.Boolean)
    )
    not_v49d0 = Header.packet_specific_25.rebind(
        Field('Not a V49d0 Packet', basic.Boolean)
    )
    spectrum = Header.packet_specific_24.rebind(
        Field('Signal Spectrum or Signal Time Data Packet', basic.Boolean)
    )

class ContextHeader(Header):
    """
    VRT Packet Header with Context Packet-Specific Indicator Bits (5.1.1.1)
    """
    not_v49d0 = Header.packet_specific_25.rebind(
        Field('Not a V49d0 Packet', basic.Boolean)
    )
    timestamp_mode = Header.packet_specific_24.rebind(
        Field('Timestamp Mode', enums.TSM)
    )

class CommandHeader(Header):
    """
    VRT Packet Header with Command Packet-Specific Indicator Bits (5.1.1.1)
    """
    acknowledge_packet = Header.packet_specific_26.rebind(
        Field('Acknowledge Packet', basic.Boolean)
    )
    cancellation_packet = Header.packet_specific_24.rebind(
        Field('Cancellation Packet', basic.Boolean)
    )

class Prologue(Struct):
    """
    VRT Packet Prologue (5).
    """
    stream_id = Field('Stream Identifier', basic.StreamIdentifier)
    class_id = Field('Class Identifier', ClassIdentifier)
    integer_timestamp = Field('Integer Timestamp', basic.UInteger32)
    fractional_timestamp = Field('Fractional Timestamp', basic.UInteger64)
