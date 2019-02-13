#!/usr/bin/python3

__version__ = '0.1'

import logging
import argparse
import sys

from v49gen.parser import FileParser

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
        for packet in FileParser().parse(filename):
            prologue = packet.get_header_bytes()
            if args.verbose:
                print('Packet prologue:')
                dump_bytes(prologue, sys.stdout)
                if packet.has_trailer:
                    trailer = packet.trailer.get_bytes()
                    print('Packet trailer:')
                    dump_bytes(trailer, sys.stdout)
