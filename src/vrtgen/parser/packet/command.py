# Copyright (C) 2020 Geon Technologies, LLC
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
Command Packet parser classes.
"""

from vrtgen.parser import value as value_parser
from vrtgen.parser.section import SectionParser
from vrtgen.types.control import ControlAcknowledgeMode

from .cif import CIFPacketParser

class PermitParser(SectionParser):
    """
    Parser for 'permit' settings on Command Packets.
    """

PermitParser.add_field_parser(ControlAcknowledgeMode.permit_partial, alias='Partial')
PermitParser.add_field_parser(ControlAcknowledgeMode.permit_warnings, alias='Warnings')
PermitParser.add_field_parser(ControlAcknowledgeMode.permit_errors, alias='Errors')

class CommandPacketParser(CIFPacketParser):
    """
    Base parser for Command Packet configuration.
    """
    @staticmethod
    def _parse_identification(value):
        if not isinstance(value, str):
            raise TypeError('identification type must be a string')
        if value.casefold() == 'none':
            return None
        return value_parser.parse_identifier_format(value)

    @classmethod
    def parse_controllee(cls, log, context, value):
        """
        Parses Controllee ID/UUID setting.
        """
        context.controllee = cls._parse_identification(value)
        log.debug('Controllee ID = %s', context.controllee)

    @classmethod
    def parse_controller(cls, log, context, value):
        """
        Parses Controller ID/UUID setting.
        """
        context.controller = cls._parse_identification(value)
        log.debug('Controller ID = %s', context.controller)

CommandPacketParser.add_parser('Permit', PermitParser())
CommandPacketParser.add_field_parser(ControlAcknowledgeMode.action_mode, alias='Action')
CommandPacketParser.add_field_parser(ControlAcknowledgeMode.nack_only, alias='NACK')
CommandPacketParser.add_parser('Controllee', CommandPacketParser.parse_controllee)
CommandPacketParser.add_parser('Controller', CommandPacketParser.parse_controller)

class AcknowledgeRequestParser(SectionParser):
    """
    Parser for acknowledgement packet requests in Control Packets.
    """

AcknowledgeRequestParser.add_field_parser(
    ControlAcknowledgeMode.request_validation, alias='Validation'
)
AcknowledgeRequestParser.add_field_parser(
    ControlAcknowledgeMode.request_execution, alias='Execution'
)
AcknowledgeRequestParser.add_field_parser(
    ControlAcknowledgeMode.request_query, alias='Query-State'
)

class ControlPacketParser(CommandPacketParser):
    """
    Parser for Control Packet configuration.
    """

ControlPacketParser.add_parser('Acknowledge', AcknowledgeRequestParser())

class AckSPacketParser(CommandPacketParser):
    """
    Parser for Acknowledge Query-Status Packet configuration.
    """

class AckVXPacketParser(CommandPacketParser):
    """
    Parser for Acknowledge Validation and Acknowledge Execution Packet
    configuration.
    """

AckVXPacketParser.add_field_parser(ControlAcknowledgeMode.request_warning, alias='Warnings')
AckVXPacketParser.add_field_parser(ControlAcknowledgeMode.request_error, alias='Errors')
AckVXPacketParser.add_field_parser(ControlAcknowledgeMode.partial, alias='Partial')
