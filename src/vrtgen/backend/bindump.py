"""
Binary dump packet generator.
"""

import sys
import struct

from vrtgen.types.enums import TSI, TSF, PacketType
from vrtgen.types.prologue import Header
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
    def _get_header(self, packet):
        header = Header()

        for field in header.get_fields():
            try:
                config = packet.get_field(field.name)
            except KeyError:
                continue
            header.set_value(field.name, config.value)

        return header

    def _get_prologue(self, packet):
        header = self._get_header(packet)
        prologue = header.pack()
        if packet.stream_id.is_set:
            prologue += struct.pack('>I', packet.stream_id.value)
        if packet.class_id.is_set:
            prologue += packet.class_id.value.pack()
        if packet.tsi != TSI.NONE:
            prologue += struct.pack('>I', 0)
        if packet.tsf != TSF.NONE:
            prologue += struct.pack('>Q', 0)
        return prologue

    def _get_cif_prologue(self, packet):
        cif0 = CIF0.Enables()
        for field in packet.get_fields(scope=Scope.CIF0):
            if field.is_required or field.is_set:
                cif0.set_value(field.name, True)

        cif1 = CIF1.Enables()
        for field in packet.get_fields(scope=Scope.CIF1):
            if field.is_required or field.is_set:
                cif0.cif1_enable = True
                cif1.set_value(field.name, True)

        prologue = cif0.pack()
        if cif0.cif1_enable:
            prologue += cif1.pack()

        return prologue

    def _get_trailer_bytes(self, packet):
        trailer = Trailer()
        for config in packet.get_fields(scope=Scope.TRAILER):
            if not config.is_required and not config.is_set:
                continue
            trailer.set_value(config.name, config.value)
        return trailer.pack()

    def generate(self, packet):
        is_data = packet.packet_type() in (PacketType.SIGNAL_DATA, PacketType.SIGNAL_DATA_STREAM_ID)
        print('Packet ' + packet.name)
        print('Prologue:')
        prologue = self._get_prologue(packet)
        if not is_data:
            prologue += self._get_cif_prologue(packet)
        dump_bytes(prologue, sys.stdout)

        if is_data:
            print('Trailer:')
            trailer = self._get_trailer_bytes(packet)
            dump_bytes(trailer, sys.stdout)
