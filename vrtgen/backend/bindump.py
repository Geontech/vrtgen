import sys
import struct

from vrtgen.model.enums import TSI, TSF

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
    else:
        return value

class BinaryDumper(Generator):
    def _get_header(self, packet):
        header = bytearray(4)

        header[0] = packet.packet_type() << 4
        if packet.class_id.is_set:
            header[0] |= 0x08
        header[0] |= packet.packet_specific_bits()
        header[1] = packet.tsi << 6
        header[1] |= packet.tsf << 4

        return header

    def _get_prologue(self, packet):
        prologue = self._get_header(packet)
        if packet.has_stream_id:
            stream_id = get_default(packet.stream_id.value, 0)
            prologue += struct.pack('>I', stream_id)
        if packet.has_class_id:
            # TODO
            prologue += b'\x00' * 8
        if packet.tsi != TSI.NONE:
            ts = get_default(packet.prologue.integer_timestamp.value, 0)
            prologue += struct.pack('>I', ts)
        if packet.tsf != TSF.NONE:
            ts = get_default(packet.prologue.fractional_timestamp.value, 0)
            prologue += struct.pack('>Q', ts)

        if packet.is_data:
            return prologue

        for cif in packet.cif:
            if not cif.is_enabled:
                continue
            prologue += self._get_cif_prologue(cif)
        return prologue

    def _get_cif_prologue(self, cif):
        prologue = 0
        for field in cif.fields:
            if field.is_set:
                prologue |= 1 << field.enable_bit
        return struct.pack('>I', prologue)

    def _get_trailer_bytes(self, trailer):
        word = 0
        for field in trailer.fields:
            if not field.is_set:
                continue
            word |= field.enable_flag
            if field.value:
                # The enable and value bits are offset by 12
                value = int(field.value)
                word |= value << field.position
        return struct.pack('>I', word)

    def generate(self, packet):
        print('Packet ' + packet.name)
        prologue = self._get_prologue(packet)
        print('Prologue:')
        dump_bytes(prologue, sys.stdout)

        if packet.is_data and packet.has_trailer:
            trailer = self._get_trailer_bytes(packet.trailer)
            print('Trailer:')
            dump_bytes(trailer, sys.stdout)
