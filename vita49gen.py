#!/usr/bin/python3

__version__ = '0.1'

import logging
import argparse
import sys

from vrtgen.parser import FileParser
from vrtgen.backend.cpp import CppGenerator

logging.basicConfig()

def group_bytes(data, size=8):
    for pos in range(0, len(data), size):
        yield data[pos:pos+size]

def dump_bytes(data, stream):
    offset = 0
    for chunk in group_bytes(data):
        data = ''.join('{:02x}'.format(ch) for ch in chunk)
        stream.write('0x{0:04x} {1} {2}\n'.format(offset, data[:8], data[8:]))
        offset += 0x10

if __name__ == '__main__':
    arg_parser = argparse.ArgumentParser(description='Generate VITA 49.2 packet classes.')
    arg_parser.add_argument('filename', nargs='+', help='VRT YAML definition file')
    arg_parser.add_argument('-v', '--verbose', action='store_true', default=False,
                            help='display debug messages')
    arg_parser.add_argument('--version', action='version', version='%(prog)s '+__version__)

    args = arg_parser.parse_args()

    if args.verbose:
        logging.getLogger().setLevel(logging.DEBUG)

    for filename in args.filename:
        logging.debug('Parsing %s', filename)
        generator = CppGenerator()
        for packet in FileParser().parse(filename):
            if args.verbose:
                print('Packet ' + packet.name)
                prologue = packet.get_prologue_bytes()
                dump_bytes(prologue, sys.stdout)
                if packet.has_trailer:
                    trailer = packet.trailer.get_bytes()
                    print('Trailer:')
                    dump_bytes(trailer, sys.stdout)
            generator.generate(packet)
