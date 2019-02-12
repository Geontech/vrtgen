#!/usr/bin/python3

import logging
import sys

from v49gen.parser import Parser

logging.basicConfig(format='%(levelname)s: %(message)s')
logging.getLogger().setLevel(logging.DEBUG)

def hex_bytes(data):
    return ''.join('{:02x}'.format(ch) for ch in data)

if __name__ == '__main__':
    source = sys.argv[1]
    p = Parser()
    for packet in p.parse(source):
        header = packet.get_header_bytes()
        logging.debug('Packet header  0x%s', hex_bytes(header))
        if packet.has_trailer:
            logging.debug('Packet trailer 0x%s', hex_bytes(packet.trailer.get_bytes()))
