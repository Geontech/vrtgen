import logging
import re

from .field import *
from vrtgen.model import config

from vrtgen.types.enums import TSI, TSF, SSI
from vrtgen.types.struct import Struct
from vrtgen.types.cif1 import CIF1

class SectionParser:
    FIELD_PARSERS = {}
    FIELD_ALIASES = {}

    def __init__(self, log, context):
        self.log = log
        self.context = context

    def __init_subclass__(cls, **kwargs):
        super().__init_subclass__(**kwargs)
        # Copy the class-wide parser tables on subclass creation so that they
        # can add or otherwise alter the parsers without affecting any other
        # classes in the hierarchy
        cls.FIELD_PARSERS = cls.FIELD_PARSERS.copy()
        cls.FIELD_ALIASES = cls.FIELD_ALIASES.copy()

    @classmethod
    def add_field_parser(cls, field, parser):
        if not isinstance(field, str):
            field = field.name
        cls.FIELD_PARSERS[field.casefold()] = parser

    @classmethod
    def add_field_alias(cls, field, alias):
        if not isinstance(field, str):
            field = field.name
        cls.FIELD_ALIASES[alias.casefold()] = field

    def get_field_parser(self, field):
        parser = self.FIELD_PARSERS.get(field.name.casefold(), None)
        if parser is not None:
            return parser
        elif issubclass(field.type, Struct):
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

            field_name = self.FIELD_ALIASES.get(field_name.casefold(), field_name)
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

class TrailerParser(SectionParser):
    def __init__(self, log, trailer):
        super().__init__(log.getChild('Trailer'), trailer)

    def parse_option(self, name, value):
        if name.casefold() == 'user-defined':
            self.log.warn('User-defined bits not implemented')
            return True
        else:
            return super().parse_option(name, value)

TrailerParser.add_field_parser(config.SAMPLE_FRAME, SSIParser())

class CIFPayloadParser(SectionParser):
    def __init__(self, log, packet):
        super().__init__(log.getChild('Payload'), packet)

CIFPayloadParser.add_field_parser(CIF1.index_list, IndexListParser())
CIFPayloadParser.add_field_parser(CIF1.discrete_io_32, UserDefinedFieldParser())
CIFPayloadParser.add_field_parser(CIF1.discrete_io_64, UserDefinedFieldParser())

class PrologueParser(SectionParser):
    def __init__(self, log, prologue):
        super().__init__(log.getChild('Prologue'), prologue)

PrologueParser.add_field_alias(config.STREAM_ID, 'Stream ID')
PrologueParser.add_field_parser(config.CLASS_ID, ClassIDParser())
PrologueParser.add_field_alias(config.CLASS_ID, 'Class ID')

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
        return config.DataPacketConfiguration(name)

    def parse_trailer(self, packet, value):
        #TrailerParser(self.log, packet.trailer).parse(value)
        pass

class ContextPacketParser(PacketParser):
    def create_packet(self, name):
        return config.ContextPacketConfiguration(name)

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
        return config.CommandPacketConfiguration(name)

    def parse_payload(self, packet, value):
        CIFPayloadParser(self.log, packet).parse(value)
