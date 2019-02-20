from .packets import *

import collections
import logging
import re

import yaml

class FileParser:
    field_parsers = {}

    @classmethod
    def add_field_parser(cls, name, parser):
        cls.field_parsers[name.casefold()] = parser

    def parse(self, filename):
        with open(filename, 'r') as fp:
            document = yaml.load(fp)

        for name, value in document.items():
            if name.startswith('.'):
                logging.debug("Skipping hidden entry %s", name)
            else:
                try:
                    yield self.parse_packet(name, value)
                except Exception as exc:
                    logging.exception("%s %s", name, str(exc))

    def split_value(self, value):
        if isinstance(value, dict):
            if len(value) != 1:
                raise ValueError('Multiple values in list')
            return value.copy().popitem()
        elif isinstance(value, list):
            raise TypeError('Fields cannot be specified as a list')
        else:
            return value, None

    def parse_packet(self, name, value):
        logging.info('Processing packet %s', name)
        if not isinstance(value, dict):
            raise RuntimeError('Invalid definition for packet ' + name)

        packet_type = value.pop('type', None)
        if packet_type is None:
            raise RuntimeError('No packet type specified for ' + name)
        if packet_type == 'data':
            packet = VRTDataPacket(name)
        elif packet_type == 'context':
            packet = VRTContextPacket(name)
        elif packet_type == 'control':
            packet = VRTControlPacket(name)
        else:
            raise RuntimeError("Invalid type '{0}' for packet '{1}'".format(packet_type, name))

        for field_name, field_value in value.items():
            if field_name == 'required':
                if not isinstance(field_value, list):
                    raise TypeError('Invalid value for required fields')
                for item in field_value:
                    key, val = self.split_value(item)
                    field = self.parse_field(packet, key, val)
                    # field.required
            elif field_name == 'optional':
                if not isinstance(field_value, list):
                    raise TypeError('Invalid value for optional fields')
                for item in field_value:
                    key, val = self.split_value(item)
                    field = self.parse_field(packet, key, val)
                    # field.optional
            else:
                field = self.parse_field(packet, field_name, field_value)
                # field.constant

        return packet

    def parse_tsi(self, packet, value):
        value = {'none':  TSI.NONE,
                 'utc':   TSI.UTC,
                 'gps':   TSI.GPS,
                 'other': TSI.OTHER}[value.lower()]
        packet.tsi.set_value(value)
        packet.tsf.set_required()

    def parse_tsf(self, packet, value):
        value = {'none':         TSF.NONE,
                 'samples':      TSF.SAMPLE_COUNT,
                 'picoseconds':  TSF.REAL_TIME,
                 'free running': TSF.FREE_RUNNING}[value.lower()]
        packet.tsf.set_value(value)
        packet.tsf.set_required()

    def parse_class_id(self, packet, value):
        if value is None:
            return
        elif str(value).casefold() == 'required':
            packet.class_id.set_required()
        elif str(value).casefold() == 'optional':
            packet.class_id.set_optional()
        else:
            raise ValueError(value)

    HEX_DIGIT = r'[0-9a-zA-Z]'
    OUI_RE = re.compile(r'({0})-({0})-({0})$'.format(HEX_DIGIT*2))
    def parse_oui(self, packet, value):
        match = self.OUI_RE.match(value)
        if not match:
            raise ValueError('OUI format must be XX-XX-XX')
        packet.class_id.oui = int(''.join(match.groups()), 16)
        packet.class_id.set_required()

    def parse_information_class(self, packet, value):
        packet.class_id.information_class = int(value)
        packet.class_id.set_required()

    def parse_packet_class(self, packet, value):
        packet.class_id.packet_class = int(value)
        packet.class_id.set_required()

    def parse_field(self, packet, name, value):
        parser = self.field_parsers.get(name.casefold(), None)
        if parser is not None:
            return parser(self, packet, value)

        try:
            field = packet.add_field(name)
        except KeyError:
            logging.error("Invalid field '%s' for packet %s", name, packet.name)
            return
        logging.debug("Field '%s' = %s", name, value)
        field.set_value(value)

FileParser.add_field_parser('TSI', FileParser.parse_tsi)
FileParser.add_field_parser('TSF', FileParser.parse_tsf)
FileParser.add_field_parser('Class ID', FileParser.parse_class_id)
FileParser.add_field_parser('OUI', FileParser.parse_oui)
FileParser.add_field_parser('Packet Class Code', FileParser.parse_packet_class)
FileParser.add_field_parser('Information Class Code', FileParser.parse_information_class)
