from .packets import *

import collections
import logging
import re

import yaml

class FieldParser:
    def __call__(self, log, context, name, value):
        field = context.get_field(name)
        log.debug("Parsing field '%s'", field.name)
        if isinstance(value, dict):
            self.parse_mapping(log, field, value)
        elif isinstance(value, list):
            self.parse_sequence(log, field, value)
        else:
            attribute = self.parse_attribute(value)
            if attribute is not None:
                self.set_attribute(log, field, attribute)
            else:
                self.parse_scalar(log, field, value)
                # If a value is given with no other qualifiers, consider the
                # field value to be constant
                self.set_attribute(log, field, FieldDescriptor.CONSTANT)

    def parse_mapping(self, log, field, mapping):
        attribute = FieldDescriptor.REQUIRED
        for key, value in mapping.items():
            if key == 'required':
                if not value:
                    attribute = FieldDescriptor.OPTIONAL
            elif not self.parse_mapping_entry(log, field, key, value):
                log.warn("Invalid option '%s' for field '%s'", key, field.name)

        self.set_attribute(log, field, attribute)

    def parse_mapping_entry(self, log, field, key, value):
        return False

    def parse_sequence(self, log, field, value):
        raise TypeError("Field '"+field.name+"' cannot be defined with a sequence")

    def parse_scalar(self, log, field, value):
        raise TypeError("{0} is not a valid value for field '{1}'".format(value, field.name))

    def parse_attribute(self, value):
        if isinstance(value, str):
            return {
                'required': FieldDescriptor.REQUIRED,
                'optional': FieldDescriptor.OPTIONAL
            }.get(value.casefold(), None)
        else:
            return None

    def set_attribute(self, log, field, attribute):
        if attribute == FieldDescriptor.REQUIRED:
            if not field.is_required:
                log.debug("Field '%s' is required", field.name)
                field.set_required()
        elif attribute == FieldDescriptor.OPTIONAL:
            if not field.is_optional:
                log.debug("Field '%s' is optional", field.name)
                field.set_optional()
        elif attribute == FieldDescriptor.CONSTANT:
            if not field.is_constant:
                log.debug("Field '%s' is constant", field.name)
                field.set_constant()

    def set_value(self, log, field, value):
        field.value = value
        log.debug("Field '%s' = %s", field.name, value)

class GenericFieldParser(FieldParser):
    def parse_scalar_value(self, field, value):
        if field.format == BIT:
            return self.to_bool(value)
        elif isinstance(field.format, FixedFormat):
            return float(value)
        elif isinstance(field.format, IntFormat):
            return int(value)
        else:
            raise NotImplementedError('unsupported format {}'.format(field.format))

    def parse_scalar(self, log, field, value):
        value = self.parse_scalar_value(field, value)
        self.set_value(log, field, value)

    def parse_mapping_entry(self, log, field, name, value):
        if name == 'value':
            self.parse_scalar(log, field, value)
        else:
            return False
        return True

    def to_bool(self, value):
        if value in (0, False):
            return False
        elif value in (1, True):
            return True
        else:
            raise TypeError('must be boolean, 0 or 1')

class SectionParser:
    FIELD_PARSERS = {}

    def __init__(self, log, context):
        self.log = log
        self.context = context

    def __init_subclass__(cls, **kwargs):
        super().__init_subclass__(**kwargs)
        # Copy the class-wide parser tables on subclass creation so that they
        # can add or otherwise alter the parsers without affecting any other
        # classes in the hierarchy
        cls.FIELD_PARSERS = cls.FIELD_PARSERS.copy()

    @classmethod
    def add_field_parser(cls, name, parser):
        cls.FIELD_PARSERS[name.casefold()] = parser

    def get_field_parser(self, name):
        return self.FIELD_PARSERS.get(name.casefold(), GenericFieldParser())

    def parse_field(self, name, value):
        parser = self.get_field_parser(name)
        if parser is None:
            parser = GenericFieldParser()
        try:
            parser(self.log, self.context, name, value)
        except KeyError:
            self.log.error("Invalid field '%s'", name)
        except (TypeError, ValueError) as exc:
            self.log.error("Invalid definition for '%s': %s", name, exc)

    def parse_option(self, name, value):
        return False

    def parse(self, value):
        for field_name, field_value in value.items():
            if self.parse_option(field_name, field_value):
                continue
            try:
                self.parse_field(field_name, field_value)
            except KeyError:
                self.log.error("Invalid field '%s'", field_name)
            except ValueError as exc:
                self.log.error("Invalid value for field '%s': %s", field_name, exc)
            except Exception as exc:
                self.log.error("Field '%s': %s", field_name, exc)

class SSIParser(FieldParser):
    def parse_ssi(self, value):
        ssi = getattr(SSI, value.upper(), None)
        if ssi is None:
            raise ValueError(value)
        return ssi

    def parse_scalar(self, log, field, value):
        value = self.parse_ssi(value)
        self.set_value(log, field, value)

class TrailerParser(SectionParser):
    def __init__(self, log, trailer):
        super().__init__(log.getChild('Trailer'), trailer)

    def parse_option(self, name, value):
        if name.casefold() == 'user-defined':
            self.log.warn('User-defined bits not implemented')
            return True
        else:
            return super().parse_option(name, value)

TrailerParser.add_field_parser('Sample Frame', SSIParser())

class CIFPayloadParser(SectionParser):
    def __init__(self, log, packet):
        super().__init__(log.getChild('CIF'), packet)

class PrologueParser(SectionParser):
    def __init__(self, log, prologue):
        super().__init__(log.getChild('Prologue'), prologue)


class StreamIDParser(FieldParser):
    def parse_scalar(self, log, field, value):
        self.set_value(log, field, int(value))

class TSIParser(FieldParser):
    VALUES = {
        'none':  TSI.NONE,
        'utc':   TSI.UTC,
        'gps':   TSI.GPS,
        'other': TSI.OTHER
    }

    def parse_tsi(self, log, field, value):
        mode = self.VALUES.get(value.casefold(), None)
        if mode is None:
            raise ValueError(value)
        field.mode = mode
        log.debug('TSI mode is %s', mode)

    def parse_mapping_entry(self, log, field, name, value):
        if name == 'format':
            self.parse_tsi(log, field, value)
        else:
            return False
        return True

    def parse_scalar(self, log, field, value):
        self.parse_tsi(log, field, value)

class TSFParser(FieldParser):
    VALUES = {
        'none':         TSF.NONE,
        'samples':      TSF.SAMPLE_COUNT,
        'picoseconds':  TSF.REAL_TIME,
        'free running': TSF.FREE_RUNNING
    }

    def parse_tsf(self, log, field, value):
        mode = self.VALUES.get(value.casefold(), None)
        if mode is None:
            raise ValueError(value)
        field.mode = mode
        log.debug('TSF mode is %s', mode)

    def parse_mapping_entry(self, log, field, name, value):
        if name == 'format':
            self.parse_tsf(log, field, value)
        else:
            return False
        return True

    def parse_scalar(self, log, field, value):
        self.parse_tsf(log, field, value)

class ClassIDParser(FieldParser):
    HEX_DIGIT = r'[0-9a-zA-Z]'
    OUI_RE = re.compile(r'({0})-({0})-({0})$'.format(HEX_DIGIT*2))
    def parse_oui(self, value):
        match = self.OUI_RE.match(value)
        if not match:
            raise ValueError('OUI format must be XX-XX-XX')
        return int(''.join(match.groups()), 16)

    def parse_mapping_entry(self, log, field, name, value):
        if field.oui.match(name):
            subfield = field.oui
            value = self.parse_oui(value)
        elif field.information_class.match(name):
            subfield = field.information_class
            value = int(value)
        elif field.packet_class.match(name):
            subfield = field.packet_class
            value = int(value)
        else:
            return False
        subfield.value = value
        log.debug("'%s' = %s", subfield.name, value)
        return True

PrologueParser.add_field_parser('Stream ID', StreamIDParser())
PrologueParser.add_field_parser('TSI', TSIParser())
PrologueParser.add_field_parser('TSF', TSFParser())
PrologueParser.add_field_parser('Class ID', ClassIDParser())

class PacketParser:
    def __init__(self, name):
        self.name = name
        self.log = logging.getLogger(name)

    def parse_prologue(self, packet, value):
        PrologueParser(self.log, packet.prologue).parse(value)

    def parse_trailer(self, packet, value):
        self.log.error('Only data packets can have a trailer')

    def parse_payload(self, packet, value):
        raise NotImplementedError('Payload processing not implemented')

    def parse_option(self, packet, name, value):
        return False

    def parse(self, value):
        packet = self.create_packet(self.name)

        for field_name, field_value in value.items():
            if field_name == 'prologue':
                self.parse_prologue(packet, field_value)
            elif field_name == 'trailer':
                self.parse_trailer(packet, field_value)
            elif field_name == 'payload':
                self.parse_payload(packet, field_value)
            elif not self.parse_option(packet, field_name, field_value):
                self.log.error("Invalid option '%s'", field_name)

        return packet

class DataPacketParser(PacketParser):
    def create_packet(self, name):
        return VRTDataPacket(name)

    def parse_trailer(self, packet, value):
        TrailerParser(self.log, packet.trailer).parse(value)

class ContextPacketParser(PacketParser):
    def create_packet(self, name):
        return VRTContextPacket(name)

    def parse_payload(self, packet, value):
        CIFPayloadParser(self.log, packet).parse(value)

class CommandPacketParser(PacketParser):
    def create_packet(self, name):
        return VRTCommandPacket(name)

    def parse_payload(self, packet, value):
        CIFPayloadParser(self.log, packet).parse(value)

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
                    yield self.parse_packet(name, value)
                except Exception as exc:
                    logging.exception("%s %s", name, str(exc))
