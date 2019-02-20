from .packets import *

import collections
import logging
import re

import yaml

class FieldParser:
    def parse_field_attribute(self, value):
        if isinstance(value, str):
            return {
                'required': FieldDescriptor.REQUIRED,
                'optional': FieldDescriptor.OPTIONAL
            }.get(value.casefold(), None)
        else:
            return None

    def set_field_attribute(self, field, attribute):
        if attribute == FieldDescriptor.REQUIRED:
            if not field.is_required:
                self.log.debug("Field '%s' is required", field.name)
                field.set_required()
        elif attribute == FieldDescriptor.OPTIONAL:
            if not field.is_optional:
                self.log.debug("Field '%s' is optional", field.name)
                field.set_optional()

    def parse_field_value(self, field, value):
        if field.format == BIT:
            return self.to_bool(value)
        elif isinstance(field.format, FixedFormat):
            return float(value)
        elif isinstance(field.format, IntFormat):
            return int(value)
        else:
            raise NotImplementedError('format')

    def to_bool(self, value):
        return bool(value)

class TrailerParser(FieldParser):
    def __init__(self, log, trailer):
        self.log = log
        self.trailer = trailer

    def parse_field(self, name, value):
        field = self.trailer.get_field(name)
        self.log.debug("Parsing trailer field '%s'", field.name)
        attribute = self.parse_field_attribute(value)
        if attribute is not None:
            self.set_field_attribute(field, attribute)
        else:
            value = self.parse_field_value(field, value)
            self.log.debug("Trailer field '%s' = %s", field.name, value)
            field.set_required()

    def parse(self, value):
        for field_name, field_value in value.items():
            try:
                self.parse_field(field_name, field_value)
            except KeyError:
                self.log.error("Invalid trailer field '%s'", field_name)

class PacketParser(FieldParser):
    field_parsers = {}

    def __init__(self, name):
        self.name = name
        self.log = logging.getLogger(name)

    @classmethod
    def add_field_parser(cls, name, parser):
        cls.field_parsers[name.casefold()] = parser

    def split_value(self, value):
        if isinstance(value, dict):
            if len(value) != 1:
                raise ValueError('Multiple values in list')
            return value.copy().popitem()
        elif isinstance(value, list):
            raise TypeError('Fields cannot be specified as a list')
        else:
            return value, None

    def parse(self, value):
        if not isinstance(value, dict):
            raise RuntimeError('Invalid definition for packet ' + self.name)

        packet_type = value.pop('type', None)
        if packet_type is None:
            raise RuntimeError('No packet type specified for ' + self.name)
        if packet_type == 'data':
            packet = VRTDataPacket(self.name)
        elif packet_type == 'context':
            packet = VRTContextPacket(self.name)
        elif packet_type == 'control':
            packet = VRTControlPacket(self.name)
        else:
            raise RuntimeError("Invalid type '{0}' for packet '{1}'".format(packet_type, self.name))

        for field_name, field_value in value.items():
            if field_name == 'trailer':
                try:
                    trailer = packet.trailer
                except AttributeError:
                    self.log.error('Packet type %s cannot have a trailer', packet_type)
                    continue
                TrailerParser(self.log, trailer).parse(field_value)
            elif field_name == 'required':
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
        mode = {
            'none':  TSI.NONE,
            'utc':   TSI.UTC,
            'gps':   TSI.GPS,
            'other': TSI.OTHER
        }[value.lower()]
        packet.tsi.mode = mode
        self.log.debug('TSI mode is %s', mode)
        self.set_field_attribute(packet.tsi, FieldDescriptor.REQUIRED)

    def parse_tsf(self, packet, value):
        mode = {
            'none':         TSF.NONE,
            'samples':      TSF.SAMPLE_COUNT,
            'picoseconds':  TSF.REAL_TIME,
            'free running': TSF.FREE_RUNNING
        }[value.lower()]
        packet.tsf.mode = mode
        self.log.debug('TSF mode is %s', mode)
        self.set_field_attribute(packet.tsf, FieldDescriptor.REQUIRED)

    def parse_class_id(self, packet, value):
        attribute = self.parse_field_attribute(value)
        if attribute is None:
            raise ValueError(value)
        self.set_field_attribute(packet.class_id, attribute)

    HEX_DIGIT = r'[0-9a-zA-Z]'
    OUI_RE = re.compile(r'({0})-({0})-({0})$'.format(HEX_DIGIT*2))
    def parse_oui(self, packet, value):
        match = self.OUI_RE.match(value)
        if not match:
            raise ValueError('OUI format must be XX-XX-XX')
        packet.class_id.oui = int(''.join(match.groups()), 16)
        self.set_field_attribute(packet.class_id, FieldDescriptor.REQUIRED)

    def parse_information_class(self, packet, value):
        packet.class_id.information_class = int(value)
        self.set_field_attribute(packet.class_id, FieldDescriptor.REQUIRED)

    def parse_packet_class(self, packet, value):
        packet.class_id.packet_class = int(value)
        self.set_field_attribute(packet.class_id, FieldDescriptor.REQUIRED)

    def parse_stream_id(self, packet, value):
        attribute = self.parse_field_attribute(value)
        if attribute is not None:
            self.set_field_attribute(packet.stream_id, attribute)
        else:
            packet.stream_id = int(value)
            self.set_field_attribute(packet.stream_id, FieldDescriptor.REQUIRED)

    def parse_field(self, packet, name, value):
        parser = self.field_parsers.get(name.casefold(), None)
        if parser is not None:
            self.log.debug("Parsing field '%s'", name)
            return parser(self, packet, value)

        try:
            field = packet.add_field(name)
        except KeyError:
            self.log.error("Invalid field '%s'", name)
            return
        self.log.debug("Field '%s' = %s", name, value)
        field.set_value(value)

PacketParser.add_field_parser('Stream ID', PacketParser.parse_stream_id)
PacketParser.add_field_parser('TSI', PacketParser.parse_tsi)
PacketParser.add_field_parser('TSF', PacketParser.parse_tsf)
PacketParser.add_field_parser('Class ID', PacketParser.parse_class_id)
PacketParser.add_field_parser('OUI', PacketParser.parse_oui)
PacketParser.add_field_parser('Packet Class Code', PacketParser.parse_packet_class)
PacketParser.add_field_parser('Information Class Code', PacketParser.parse_information_class)

class FileParser:
    def parse(self, filename):
        with open(filename, 'r') as fp:
            document = yaml.load(fp)

        for name, value in document.items():
            if name.startswith('.'):
                logging.debug("Skipping hidden entry %s", name)
            else:
                logging.info('Processing packet %s', name)
                try:
                    yield PacketParser(name).parse(value)
                except Exception as exc:
                    logging.exception("%s %s", name, str(exc))
