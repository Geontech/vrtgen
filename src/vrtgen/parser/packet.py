import logging
import re

from .field import *
from vrtgen.model import config

from vrtgen.types.enums import TSI, TSF, SSI
from vrtgen.types.struct import Struct, Field
from vrtgen.types.cif0 import CIF0
from vrtgen.types.cif1 import CIF1
from vrtgen.types.header import Header

class SectionParser:
    __PARSERS__ = {}
    __ALIASES__ = {}

    def __init__(self, log, context):
        self.log = log
        self.context = context

    def __init_subclass__(cls, **kwargs):
        super().__init_subclass__(**kwargs)
        # Copy the class-wide parser tables on subclass creation so that they
        # can add or otherwise alter the parsers without affecting any other
        # classes in the hierarchy
        cls.__PARSERS__ = cls.__PARSERS__.copy()
        cls.__ALIASES__ = cls.__ALIASES__.copy()

    @classmethod
    def add_parser(cls, name, parser, alias=None):
        assert parser is not None
        cls.__PARSERS__[name.casefold()] = parser
        if alias:
            cls.__ALIASES__[alias.casefold()] = name

    @classmethod
    def add_field_parser(cls, field, parser=None, alias=None):
        if parser is None:
            assert field.type is not None
            if issubclass(field.type, Struct):
                parser = StructFieldParser()
            else:
                parser = GenericFieldParser()
        cls.add_parser(field.name, parser, alias)

    def get_field_parser(self, field):
        parser = self.__PARSERS__.get(field.name.casefold(), None)
        if parser is None:
            raise ValueError("Unsupported field '{}'".format(field.name))
        return parser

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

            field_name = self.__ALIASES__.get(field_name.casefold(), field_name)
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

TrailerParser.add_parser(config.SAMPLE_FRAME, SSIParser())

class CIFPayloadParser(SectionParser):
    def __init__(self, log, packet):
        super().__init__(log.getChild('Payload'), packet)

CIFPayloadParser.add_field_parser(CIF0.reference_point_id)
CIFPayloadParser.add_field_parser(CIF0.bandwidth)
CIFPayloadParser.add_field_parser(CIF0.if_frequency)
CIFPayloadParser.add_field_parser(CIF0.rf_frequency)
CIFPayloadParser.add_field_parser(CIF0.rf_frequency_offset)
CIFPayloadParser.add_field_parser(CIF0.if_band_offset)
CIFPayloadParser.add_field_parser(CIF0.reference_level)
CIFPayloadParser.add_field_parser(CIF0.gain)
CIFPayloadParser.add_field_parser(CIF0.over_range_count)
CIFPayloadParser.add_field_parser(CIF0.sample_rate)
CIFPayloadParser.add_field_parser(CIF0.timestamp_calibration_time)
CIFPayloadParser.add_field_parser(CIF0.temperature)
CIFPayloadParser.add_field_parser(CIF0.device_id)
# Not implemented: CIF0.state_event_indicators
# Not implemented: CIF0.data_format
CIFPayloadParser.add_field_parser(CIF0.formatted_gps)
CIFPayloadParser.add_field_parser(CIF0.formatted_ins)
CIFPayloadParser.add_field_parser(CIF0.ecef_ephemeris)
CIFPayloadParser.add_field_parser(CIF0.relative_ephemeris)
CIFPayloadParser.add_field_parser(CIF0.ephemeris_ref_id)
# Not implemented: CIF0.gps_ascii
# Not implemented: CIF0.context_association_lists

CIFPayloadParser.add_field_parser(CIF1.phase_offset)
CIFPayloadParser.add_field_parser(CIF1.polarization)
CIFPayloadParser.add_field_parser(CIF1.pointing_vector)
CIFPayloadParser.add_field_parser(CIF1.spatial_scan_type)
CIFPayloadParser.add_field_parser(CIF1.beam_widths)
CIFPayloadParser.add_field_parser(CIF1.range)
CIFPayloadParser.add_field_parser(CIF1.ebno_ber)
CIFPayloadParser.add_field_parser(CIF1.threshold)
CIFPayloadParser.add_field_parser(CIF1.compression_point)
CIFPayloadParser.add_field_parser(CIF1.snr_noise_figure)
CIFPayloadParser.add_field_parser(CIF1.aux_frequency)
CIFPayloadParser.add_field_parser(CIF1.aux_gain)
CIFPayloadParser.add_field_parser(CIF1.aux_bandwidth)
CIFPayloadParser.add_parser(CIF1.index_list.name, IndexListParser())
CIFPayloadParser.add_parser(CIF1.discrete_io_32.name, UserDefinedFieldParser())
CIFPayloadParser.add_parser(CIF1.discrete_io_64.name, UserDefinedFieldParser())
CIFPayloadParser.add_field_parser(CIF1.health_status)
CIFPayloadParser.add_field_parser(CIF1.version_build_code)
CIFPayloadParser.add_field_parser(CIF1.buffer_size)

class PrologueParser(SectionParser):
    def __init__(self, log, prologue):
        super().__init__(log.getChild('Prologue'), prologue)

PrologueParser.add_parser(config.STREAM_ID, GenericFieldParser(), alias='Stream ID')
PrologueParser.add_parser(config.CLASS_ID, ClassIDParser(), alias='Class ID')
PrologueParser.add_field_parser(Header.tsi)
PrologueParser.add_field_parser(Header.tsf)
PrologueParser.add_parser(config.INTEGER_TIMESTAMP, GenericFieldParser())
PrologueParser.add_parser(config.FRACTIONAL_TIMESTAMP, GenericFieldParser())

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
