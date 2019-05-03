import logging

import yaml

from .packet import DataPacketParser, ContextPacketParser, CommandPacketParser

class FileParser:
    def parse_packet(self, name, value):
        if not isinstance(value, dict):
            raise RuntimeError('Invalid definition for packet ' + name)

        packet_type = value.pop('type', None)
        if packet_type is None:
            raise RuntimeError('No packet type specified for ' + name)
        if packet_type == 'data':
            parser = DataPacketParser(name)
        elif packet_type == 'context':
            parser = ContextPacketParser(name)
        elif packet_type == 'command':
            parser = CommandPacketParser(name)
        else:
            raise RuntimeError("Invalid type '{0}' for packet '{1}'".format(packet_type, name))

        return parser.parse(value)

    def parse(self, filename):
        with open(filename, 'r') as fp:
            document = yaml.load(fp)

        for name, value in document.items():
            if name.startswith('.'):
                logging.debug("Skipping hidden entry %s", name)
            else:
                logging.info('Processing packet %s', name)
                try:
                    packet = self.parse_packet(name, value)
                    packet.validate()
                    yield packet
                except Exception as exc:
                    logging.exception("%s %s", name, str(exc))
