import logging

from vrtgen.model import config
from vrtgen.types.enums import TSM
from vrtgen.types.prologue import Header, Prologue
from vrtgen.types.trailer import Trailer

from . import field
from . import value
from .cif import CIFPayloadParser
from .section import SectionParser

class TrailerParser(SectionParser):
    pass

class UnimplementedParser:
    def __init__(self, name):
        self.name = name

    def __call__(self, log, *args):
        log.warn('%s not implemented', self.name)

TrailerParser.add_field_parser(Trailer.calibrated_time)
TrailerParser.add_field_parser(Trailer.valid_data)
TrailerParser.add_field_parser(Trailer.reference_lock)
TrailerParser.add_field_parser(Trailer.agc_mgc)
TrailerParser.add_field_parser(Trailer.detected_signal)
TrailerParser.add_field_parser(Trailer.spectral_inversion)
TrailerParser.add_field_parser(Trailer.over_range)
TrailerParser.add_field_parser(Trailer.sample_loss)
TrailerParser.add_field_parser(Trailer.sample_frame, value.parse_ssi)
TrailerParser.add_parser(Trailer.user_defined.name, UnimplementedParser('User-defined bits'))
TrailerParser.add_field_parser(Trailer.associated_context_packets)

class PrologueParser(SectionParser):
    pass

PrologueParser.add_field_parser(Prologue.stream_id, alias='Stream ID')
PrologueParser.add_field_parser(Prologue.class_id, field.ClassIDParser(), alias='Class ID')
PrologueParser.add_field_parser(Header.tsi, value.parse_tsi)
PrologueParser.add_field_parser(Header.tsf, value.parse_tsf)
PrologueParser.add_field_parser(Prologue.integer_timestamp)
PrologueParser.add_field_parser(Prologue.fractional_timestamp)

class PacketParser:
    def __init__(self, name):
        self.name = name
        self.log = logging.getLogger(name)

    def parse_prologue(self, packet, value):
        PrologueParser().parse(self.log.getChild('Prologue'), packet.prologue, value)

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

    def create_packet(self, name):
        raise NotImplementedError()

class DataPacketParser(PacketParser):
    def create_packet(self, name):
        return config.DataPacketConfiguration(name)

    def parse_trailer(self, packet, value):
        TrailerParser().parse(self.log.getChild('Trailer'), packet.trailer, value)

class ContextPacketParser(PacketParser):
    def create_packet(self, name):
        return config.ContextPacketConfiguration(name)

    def parse_payload(self, packet, value):
        CIFPayloadParser().parse(self.log.getChild('Payload'), packet, value)

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
        CIFPayloadParser().parse(self.log.getChild('Payload'), packet, value)
