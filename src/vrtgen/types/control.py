"""
Data types for Control Packet prologue.
"""

from .struct import Struct, Field, Reserved
from .basic import Boolean
from . import enums

__all__ = (
    'ControlAcknowledgeMode',
)

class ControlAcknowledgeMode(Struct):
    """
    Control/Acknowledge Mode Field (8.3.1).
    """
    # 32..21 Control Packet Fields
    controllee_enable = Field('Controllee enable', Boolean)
    controllee_format = Field('Controllee identifier format', enums.IdentifierFormat)
    controller_enable = Field('Controller enable', Boolean)
    controller_format = Field('Controller identifier format', enums.IdentifierFormat)
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
