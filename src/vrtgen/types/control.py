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
Data types for Control Packet prologue.
"""

from .struct import Struct, Enable, Field, Reserved
from .basic import Boolean, IntegerType
from .prologue import Prologue
from . import enums

__all__ = (
    'ControlAcknowledgeMode',
    'MessageIdentifier'
)

class ControlAcknowledgeMode(Struct):
    """
    Control/Acknowledge Mode Field (8.3.1).
    """
    # 32..21 Control Packet Fields
    controllee_enable = Enable('Controllee enable')
    controllee_format = Field(
        'Controllee identifier format', enums.IdentifierFormat, enable=controllee_enable
    )
    controller_enable = Enable('Controller enable')
    controller_format = Field(
        'Controller identifier format', enums.IdentifierFormat, enable=controller_enable
    )
    partial_permitted = Field('Partial packet implementation permitted', Boolean)
    warnings = Field('Warnings', Boolean)
    errors = Field('Errors', Boolean)
    action = Field('Action Bit Field', enums.ActionMode)
    nack = Field('Not-ack only', Boolean)
    reserved_21 = Reserved(1)
    # Common bits
    request_validation = Field('Request Validation Acknowledge packet', Boolean)
    request_execution = Field('Request Execution Acknowledge packet', Boolean)
    request_query = Field('Request Query-State Acknowledge packet', Boolean)
    request_warning = Field('Request/Return Warnings', Boolean)
    request_error = Field('Request/Return Errors', Boolean)
    reserved_15 = Reserved(1)
    timing_control = Field('Timing Control/Acknowledgement', enums.TimestampControlMode)
    # 11..8 Acknowledge Packet Fields
    partial = Field('Partial action', Boolean)
    scheduled_or_executed = Field('Action Scheduled or Executed', Boolean)
    reserved_9 = Reserved(2)
    # 7..0 Reserved
    reserved_7 = Reserved(8)

class MessageIdentifier(IntegerType, bits=32, signed=False):
    """
    A Message Identifier (Message ID) is a 32-bit number that allows
    association of a particular Control packet with any resulting Acknowledge
    packets (8.4).
    """

class CommandPrologue(Prologue):
    """
    Command Packet prologue.
    """
    cam = Field('ControlAcknowledgeMode', ControlAcknowledgeMode)
    message_id = Field('Message ID', MessageIdentifier)
