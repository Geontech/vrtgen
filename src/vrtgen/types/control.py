# Copyright (C) 2021 Geon Technologies, LLC
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
Data types for Control Packet prologue.
"""

from .struct import Struct, Enable, Field, Reserved
from .basic import Boolean, IntegerType, Identifier32
from .prologue import Prologue
from . import enums

__all__ = (
    'ControlAcknowledgeMode',
    'MessageIdentifier',
    'UUIDIdentifier',
    'CommandPrologue'
)

class ControlAcknowledgeMode(Struct):
    """
    Control/Acknowledge Mode Field (8.3.1).
    """
    # 32..21 Control Packet Fields
    controllee_enable = Enable('Controllee')
    controllee_format = Field(
        'Controllee Identifier Format', enums.IdentifierFormat, enable=controllee_enable
    )
    controller_enable = Enable('Controller')
    controller_format = Field(
        'Controller Identifier Format', enums.IdentifierFormat, enable=controller_enable
    )
    permit_partial = Field('Partial Packet Implementation Permitted', Boolean)
    permit_warnings = Field('Warnings Permitted', Boolean)
    permit_errors = Field('Errors Permitted', Boolean)
    action_mode = Field('Action Mode', enums.ActionMode)
    nack_only = Field('Not-Ack Only', Boolean)
    reserved_21 = Reserved(1)
    # Common bits
    request_validation = Field('Request Validation Acknowledge Packet', Boolean)
    request_execution = Field('Request Execution Acknowledge Packet', Boolean)
    request_query = Field('Request Query-State Acknowledge Packet', Boolean)
    request_warning = Field('Request/Return Warnings', Boolean)
    request_error = Field('Request/Return Errors', Boolean)
    reserved_15 = Reserved(1)
    timing_control = Field('Timing Control/Acknowledgement', enums.TimestampControlMode)
    # 11..8 Acknowledge Packet Fields
    partial = Field('Partial Action', Boolean)
    scheduled_or_executed = Field('Action Scheduled Or Executed', Boolean)
    reserved_9 = Reserved(2)
    # 7..0 Reserved
    reserved_7 = Reserved(8)

class MessageIdentifier(IntegerType, bits=32, signed=False):
    """
    A Message Identifier (Message ID) is a 32-bit number that allows
    association of a particular Control packet with any resulting Acknowledge
    packets (8.4).
    """

class UUIDIdentifier(IntegerType, bits=128, signed=False):
    """
    UUID Control Identifier type
    """

class CommandPrologue(Prologue):
    """
    Command Packet prologue.
    """
    cam = Field('ControlAcknowledgeMode', ControlAcknowledgeMode)
    message_id = Field('Message ID', MessageIdentifier)
    controllee_id = Field('Controllee ID', Identifier32)
    controller_id = Field('Controller ID', Identifier32)
    controllee_id_uuid = Field('Controllee ID', UUIDIdentifier)
    controller_id_uuid = Field('Controller ID', UUIDIdentifier)
