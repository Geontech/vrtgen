import sys

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

class BinaryDumper(Generator):
    def generate(self, packet):
        print('Packet ' + packet.name)
        prologue = packet.get_prologue_bytes()
        dump_bytes(prologue, sys.stdout)
        if packet.has_trailer:
            trailer = packet.trailer.get_bytes()
            print('Trailer:')
            dump_bytes(trailer, sys.stdout)
