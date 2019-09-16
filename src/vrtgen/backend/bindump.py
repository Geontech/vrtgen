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
Binary dump packet generator.
"""

import sys
import struct

from vrtgen.types.enums import TSI, TSF
from vrtgen.types.prologue import DataHeader, ContextHeader, CommandHeader
from vrtgen.types.trailer import Trailer
from vrtgen.types.cif0 import CIF0
from vrtgen.types.cif1 import CIF1

from vrtgen.model.field import Scope

from .generator import Generator

__all__ = ('BinaryDumper')

def group_bytes(data, size=8):
    for pos in range(0, len(data), size):
        yield data[pos:pos+size]

def dump_bytes(data, stream):
    offset = 0
    for chunk in group_bytes(data):
        data = ''.join('{:02x}'.format(ch) for ch in chunk)
        stream.write('0x{0:04x} {1} {2}\n'.format(offset, data[:8], data[8:]))
        offset += 0x10

def get_default(value, defval):
    if value is None:
        return defval
    return value

class BinaryDumper(Generator):
    """
    Writes binary dumps of default packet configurations to the console.
    """
    def _get_prologue(self, packet):
        packet_type = packet.packet_type()
        if packet_type.is_data:
            header = DataHeader()
        elif packet_type.is_context:
            header = ContextHeader()
            header.timestamp_mode = packet.timestamp_mode
        elif packet_type.is_command:
            header = CommandHeader()
        else:
            raise TypeError(str(packet_type))
        header.packet_type = packet_type
        header.tsi = packet.tsi
        header.tsf = packet.tsf
        prologue = bytes()
        if packet.stream_id.is_set:
            prologue += struct.pack('>I', packet.stream_id.value)
        if packet.class_id.is_set:
            header.class_id_enable = True
            prologue += packet.class_id.value.pack()
        if packet.tsi != TSI.NONE:
            prologue += struct.pack('>I', 0)
        if packet.tsf != TSF.NONE:
            prologue += struct.pack('>Q', 0)
        return header.pack() + prologue

    def _get_cif_prologue(self, packet):
        cif0 = CIF0.Enables()
        for field in packet.get_fields(Scope.CIF0):
            if field.is_required or field.is_set:
                cif0.set_value(field.name, True)

        cif1 = CIF1.Enables()
        for field in packet.get_fields(Scope.CIF1):
            if field.is_required or field.is_set:
                cif0.cif1_enable = True
                cif1.set_value(field.name, True)

        prologue = cif0.pack()
        if cif0.cif1_enable:
            prologue += cif1.pack()

        return prologue

    def _get_trailer_bytes(self, packet):
        trailer = Trailer()
        for config in packet.get_fields(Scope.TRAILER):
            if not config.is_required and not config.is_set:
                continue
            trailer.set_value(config.name, config.value)
        return trailer.pack()

    def generate(self, packet):
        packet_type = packet.packet_type()
        print('Packet ' + packet.name)
        print('Prologue:')
        prologue = self._get_prologue(packet)
        if not packet_type.is_data:
            prologue += self._get_cif_prologue(packet)
        dump_bytes(prologue, sys.stdout)

        if packet_type.is_data:
            print('Trailer:')
            trailer = self._get_trailer_bytes(packet)
            dump_bytes(trailer, sys.stdout)