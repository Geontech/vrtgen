from .packet import *

import logging
import re

import yaml


TSI_VALUES = {
    'none':  TSI.NONE,
    'utc':   TSI.UTC,
    'gps':   TSI.GPS,
    'other': TSI.OTHER
}

def value_to_tsi(value):
    try:
        return TSI_VALUES[value.casefold()]
    except KeyError:
        raise ValueError(value)

TSF_VALUES = {
    'none':         TSF.NONE,
    'samples':      TSF.SAMPLE_COUNT,
    'picoseconds':  TSF.REAL_TIME,
    'free running': TSF.FREE_RUNNING
}

def value_to_tsf(value):
    try:
        return TSF_VALUES[value.casefold()]
    except KeyError:
        raise ValueError(value)

class FieldParser:
    def __call__(self, log, field, value):
        if isinstance(value, dict):
            self.parse_mapping(log, field, value)
        elif isinstance(value, list):
            self.parse_sequence(log, field, value)
        else:
            enable = self.parse_enable(value)
            if enable is not None:
                self.set_enable(log, field, enable)
            else:
                self.parse_scalar(log, field, value)

    def parse_mapping(self, log, field, mapping):
        enable = Field.Mode.REQUIRED
        for key, value in mapping.items():
            if key == 'required':
                if not value:
                    enable = Field.Mode.OPTIONAL
            elif not self.parse_mapping_entry(log, field, key, value):
                log.warn("Invalid option '%s' for field '%s'", key, field.name)

        self.set_enable(log, field, enable)

    def parse_mapping_entry(self, log, field, key, value):
        return False

    def parse_sequence(self, log, field, value):
        raise TypeError("Field '"+field.name+"' cannot be defined with a sequence")

    def parse_scalar(self, log, field, value):
        value = self.parse_scalar_value(field, value)
        self.set_value(log, field, value)
        # If a value is given with no other qualifiers, consider the
        # field value to be constant
        log.debug("Field '%s' is CONSTANT", field.name)
        field.set_constant()

    def parse_scalar_value(self, field, value):
        raise TypeError("{0} is not a valid value for field '{1}'".format(value, field.name))

    def parse_enable(self, value):
        if isinstance(value, str):
            return {
                'required': Field.Mode.REQUIRED,
                'optional': Field.Mode.OPTIONAL,
                'disabled': Field.Mode.DISABLED
            }.get(value.casefold(), None)
        else:
            return None

    def set_enable(self, log, field, enable):
        field.enable = enable
        log.debug("Field '%s' is %s", field.name, field.enable)

    def set_value(self, log, field, value):
        field.value = value
        log.debug("Field '%s' = %s", field.name, value)

class GenericFieldParser(FieldParser):
    HEX_DIGIT = r'[0-9a-zA-Z]'
    OUI_RE = re.compile(r'({0})-({0})-({0})$'.format(HEX_DIGIT*2))
    def parse_oui(self, value):
        match = self.OUI_RE.match(str(value))
        if not match:
            raise ValueError('OUI format must be XX-XX-XX')
        return int(''.join(match.groups()), 16)

    def parse_scalar_value(self, field, value):
        if isinstance(field, OUIField):
            return self.parse_oui(value)
        elif isinstance(field, TSIField):
            return value_to_tsi(value)
        elif isinstance(field, TSFField):
            return value_to_tsf(value)
        elif isinstance(field, BitField):
            return self.to_bool(value)
        elif isinstance(field, FixedPointField):
            return float(value)
        elif isinstance(field, IntegerField):
            return int(value)
        elif isinstance(field, UserDefinedField):
            raise ValueError('user-defined fields are not implemented')
        else:
            raise NotImplementedError("unsupported field '{}'".format(field.name))

    def parse_mapping_entry(self, log, field, name, value):
        if name == 'default':
            value = self.parse_scalar_value(field, value)
            self.set_value(log, field, value)
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

class UserDefinedFieldParser(FieldParser):
    def parse_scalar_value(self, log, value):
        raise TypeError('user-defined fields must be a sequence or mapping')

    def parse_mapping_entry(self, log, field, name, value):
        if name == 'fields':
            if not isinstance(value, list):
                raise TypeError('user-defined fields must be a sequence')
            self.parse_sequence(log, field, value)
            return True
        else:
            return False

    def parse_sequence(self, log, field, value):
        log = log.getChild(field.name)
        for index, item in enumerate(value):
            try:
                self.parse_user_defined_field(log, field, item)
            except (ValueError, TypeError) as exc:
                log.error('Invalid user-defined field %d: %s', index, exc)

    def parse_user_defined_field(self, log, field, value):
        if not isinstance(value, dict):
            raise TypeError('must be a mapping')
        name = None
        bits = 1
        word = None
        position = None
        for attr_name, attr_value in value.items():
            if attr_name == 'name':
                name = attr_value
            elif attr_name == 'bits':
                bits = int(attr_value)
            elif attr_name == 'position':
                position = int(attr_value)
            elif attr_name == 'word':
                word = int(attr_value)
            else:
                raise ValueError('invalid attribute {}'.format(attr_name))
        if name is None:
            raise ValueError('no name given')
        field.add_field(name, bits, word, position)
        log.debug("'%s' bits=%d position=%s/%s", name, bits, word, position)

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
    def add_field_parser(cls, field, parser):
        cls.FIELD_PARSERS[field.name.casefold()] = parser

    def get_field_parser(self, field):
        parser = self.FIELD_PARSERS.get(field.name.casefold(), None)
        if parser is not None:
            return parser
        elif isinstance(field, StructField):
            return StructFieldParser()
        else:
            return GenericFieldParser()

    def parse_field(self, field, value):
        self.log.debug("Parsing field '%s'", field.name)
        parser = self.get_field_parser(field)
        try:
            parser(self.log, field, value)
        except (TypeError, ValueError) as exc:
            self.log.error("Invalid definition for '%s': %s", field.name, exc)

    def parse_option(self, name, value):
        return False

    def parse(self, value):
        for field_name, field_value in value.items():
            if self.parse_option(field_name, field_value):
                continue

            field = self.context.get_field(field_name)
            if field is None:
                self.log.error("Invalid field '%s'", field_name)
                continue

            try:
                self.parse_field(field, field_value)
            except (TypeError, ValueError) as exc:
                self.log.error("Invalid value for field '%s': %s", field_name, exc)
            except Exception as exc:
                self.log.exception("Field '%s': %s", field_name, exc)

class SSIParser(FieldParser):
    def parse_scalar_value(self, log, field, value):
        ssi = getattr(SSI, value.upper(), None)
        if ssi is None:
            raise ValueError(value)
        return ssi

class TrailerParser(SectionParser):
    def __init__(self, log, trailer):
        super().__init__(log.getChild('Trailer'), trailer)

    def parse_option(self, name, value):
        if name.casefold() == 'user-defined':
            self.log.warn('User-defined bits not implemented')
            return True
        else:
            return super().parse_option(name, value)

TrailerParser.add_field_parser(VRTDataTrailer.sample_frame, SSIParser())

class StructFieldParser(FieldParser):
    def parse_mapping_entry(self, log, field, name, value):
        subfield = field.get_field(name)
        if subfield is None:
            return False
        parser = GenericFieldParser()
        log = log.getChild(field.name)
        try:
            parser(log, subfield, value)
        except (ValueError, TypeError) as exc:
            log.error("Invalid definition for '%s': %s", subfield.name, exc)
        return True

class CIFPayloadParser(SectionParser):
    def __init__(self, log, packet):
        super().__init__(log.getChild('Payload'), packet)

class IndexListParser(FieldParser):
    def parse_mapping_entry(self, log, field, name, value):
        if name.casefold() == 'entry size':
            field.entry_size = int(value)
            log.debug("Index List entry size is %d", value)
        else:
            return False
        return True

CIFPayloadParser.add_field_parser(CIF1.index_list, IndexListParser())
CIFPayloadParser.add_field_parser(CIF1.discrete_io_32, UserDefinedFieldParser())
CIFPayloadParser.add_field_parser(CIF1.discrete_io_64, UserDefinedFieldParser())

class PrologueParser(SectionParser):
    def __init__(self, log, prologue):
        super().__init__(log.getChild('Prologue'), prologue)

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

    def parse_option(self, packet, name, value):
        if name.casefold() == 'timestamp mode':
            try:
                packet.timestamp_mode = {'fine': TSM.FINE, 'coarse': TSM.COARSE}[value.casefold()]
            except KeyError:
                self.log.error("Invalid Timestamp Mode '%s'", value)
            else:
                self.log.debug('Timestamp Mode = %s', packet.timestamp_mode)
            return True
        else:
            return False

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
