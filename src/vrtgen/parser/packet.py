# Copyright (C) 2019 Geon Technologies, LLC
#
# This file is part of vrtgen.
#
# vrtgen is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# vrtgen is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
# more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.

"""
Parser classes for generating VITA 49 packet configurations from Python
dictionaries.
"""

import logging

from vrtgen.model import config
from vrtgen.types.prologue import ContextHeader, Prologue
from vrtgen.types.trailer import Trailer
from vrtgen.types.control import ControlAcknowledgeMode

from . import field
from .cif import CIFPayloadParser
from .mapping import MappingParser
from .section import SectionParser
from . import value as value_parser

def unimplemented_parser(name):
    """
    Creates a parser function that logs a 'not implemented' warning.
    """
    def parser(log, *args):
        # pylint: disable=unused-argument
        log.warn('%s not implemented', name)
    return parser

class TimestampParser(MappingParser):
    """
    Parser for timestamp configuration.

    Only the TSI and TSF modes are configurable, under the names "integer" and
    "fractional".
    """
    @staticmethod
    def parse_integer(log, context, value):
        """
        Parses integer timestamp mode.
        """
        context.tsi = value_parser.parse_tsi(value)
        log.debug('TSI = %s', context.tsi)

    @staticmethod
    def parse_fractional(log, context, value):
        """
        Parses fractional timestamp mode.
        """
        context.tsf = value_parser.parse_tsf(value)
        log.debug('TSF = %s', context.tsf)

TimestampParser.add_parser('integer', TimestampParser.parse_integer)
TimestampParser.add_parser('fractional', TimestampParser.parse_fractional)

class PrologueParser(SectionParser):
    """
    Base parser for packet prologue configuration.
    """

PrologueParser.add_field_parser(Prologue.stream_id, alias='Stream ID')
PrologueParser.add_field_parser(Prologue.class_id, field.ClassIDParser(), alias='Class ID')
PrologueParser.add_parser('timestamp', TimestampParser())

class PacketParser:
    """
    Base class for parsers that create and configure VITA 49 packets.
    """
    def __init__(self, name):
        self.name = name
        self.log = logging.getLogger(name)

    def create_packet(self, name):
        """
        Creates a default packet configuration.
        """
        raise NotImplementedError()

    def _get_parser(self):
        raise NotImplementedError()

    def parse(self, value):
        """
        Creates a packet configuration from a Python dictionary.
        """
        packet = self.create_packet(self.name)
        parser = self._get_parser()
        parser(self.log, packet, value)
        return packet

# Data Packet parser classes
class TrailerParser(SectionParser):
    """
    Parser for Data Packet Trailer configuration.
    """

TrailerParser.add_field_parser(Trailer.calibrated_time)
TrailerParser.add_field_parser(Trailer.valid_data)
TrailerParser.add_field_parser(Trailer.reference_lock)
TrailerParser.add_field_parser(Trailer.agc_mgc)
TrailerParser.add_field_parser(Trailer.detected_signal)
TrailerParser.add_field_parser(Trailer.spectral_inversion)
TrailerParser.add_field_parser(Trailer.over_range)
TrailerParser.add_field_parser(Trailer.sample_loss)
TrailerParser.add_field_parser(Trailer.sample_frame)
TrailerParser.add_parser(Trailer.user_defined.name, unimplemented_parser('User-defined bits'))
TrailerParser.add_field_parser(Trailer.associated_context_packets)

class DataSectionParser(PrologueParser):
    """
    Parser for Data Packet configuration sections.
    """

DataSectionParser.add_parser('Trailer', TrailerParser())

class DataPacketParser(PacketParser):
    """
    Parser for Data Packet configuration.
    """
    def create_packet(self, name):
        return config.DataPacketConfiguration(name)

    def _get_parser(self):
        return DataSectionParser()

# Context Packet parser classes
class ContextSectionParser(PrologueParser):
    """
    Parser for Context Packet configuration sections.
    """
    @staticmethod
    def parse_tsm(log, context, value):
        """
        Parses context packet timestamp mode.
        """
        context.timestamp_mode = value_parser.parse_tsm(value)
        log.debug('TSM = %s', context.timestamp_mode)

ContextSectionParser.add_parser(
    ContextHeader.timestamp_mode.name,
    ContextSectionParser.parse_tsm,
    alias='TSM'
)
ContextSectionParser.add_parser('Payload', CIFPayloadParser())

class ContextPacketParser(PacketParser):
    """
    Parser for Context Packet configuration.
    """
    def create_packet(self, name):
        return config.ContextPacketConfiguration(name)

    def _get_parser(self):
        return ContextSectionParser()

# Command Packet parser classes

class CommandSectionParser(PrologueParser):
    """
    Parser for Command Packet configuration sections.
    """
    _ACKNOWLEDGE_TYPES = {
        'validation': config.Acknowledgement.VALIDATION,
        'execution': config.Acknowledgement.EXECUTION,
        'query-state': config.Acknowledgement.QUERY_STATE,
    }
    @classmethod
    def _parse_acknowledge_type(cls, log, context, value):
        try:
            ack_type = cls._ACKNOWLEDGE_TYPES[value.casefold()]
        except KeyError:
            raise ValueError(value)
        log.debug('Acknowledge %s', ack_type)
        context.acknowledge.append(ack_type)

    @classmethod
    def parse_acknowledge(cls, log, context, value):
        """
        Parses acknowledgement packet types.
        """
        if isinstance(value, dict):
            raise TypeError('acknowledgement packet types must be a sequence or scalar')

        if not isinstance(value, list):
            value = [value]

        for item in value:
            try:
                cls._parse_acknowledge_type(log, context, item)
            except ValueError:
                log.warning("Invalid acknowledgement packet type '%s'", item)
            except TypeError:
                log.error('Acknowledgement packet type must be a string')

    @staticmethod
    def _parse_identification(value):
        if not isinstance(value, str):
            raise TypeError('identification type must be a string')
        if value.casefold() == 'none':
            return None
        return value_parser.parse_identifier_format(value)

    @classmethod
    def parse_controllee(cls, log, context, value):
        context.controllee = cls._parse_identification(value)
        log.debug('Controllee ID = %s', context.controllee)

    @classmethod
    def parse_controller(cls, log, context, value):
        context.controller = cls._parse_identification(value)
        log.debug('Controller ID = %s', context.controller)

CommandSectionParser.add_parser('Payload', CIFPayloadParser())
CommandSectionParser.add_parser('Acknowledge', CommandSectionParser.parse_acknowledge)
CommandSectionParser.add_field_parser(ControlAcknowledgeMode.action, alias='Action')
CommandSectionParser.add_field_parser(ControlAcknowledgeMode.nack, alias='NACK')
CommandSectionParser.add_parser('Controllee', CommandSectionParser.parse_controllee)
CommandSectionParser.add_parser('Controller', CommandSectionParser.parse_controller)

class CommandPacketParser(PacketParser):
    """
    Parser for Command Packet configuration.
    """
    def create_packet(self, name):
        return config.CommandPacketConfiguration(name)

    def _get_parser(self):
        return CommandSectionParser()
