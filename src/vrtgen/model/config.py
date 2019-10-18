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
Types for VITA 49 packet configurations.
"""

from enum import Enum
import warnings

from vrtgen.types import enums
from vrtgen.types.prologue import Prologue
from vrtgen.types.trailer import Trailer
from vrtgen.types.cif0 import CIF0
from vrtgen.types.cif1 import CIF1
from vrtgen.types.control import ControlAcknowledgeMode

from .field import FieldConfiguration, Mode, Scope

class PacketType(Enum):
    """
    Packet subtypes.
    """
    DATA = 'data'
    CONTEXT = 'context'
    CONTROL = 'control'
    ACKV = 'ackv'
    ACKX = 'ackx'
    ACKS = 'acks'

class PacketConfiguration:
    """
    Base class for VRT packet configuration.
    """
    def __init__(self, name, packet_type):
        self.name = name
        self.packet_type = packet_type
        self._fields = []
        self.tsi = enums.TSI()
        self.tsf = enums.TSF()
        self.stream_id = self._add_field(Prologue.stream_id, Scope.PROLOGUE)
        self.class_id = self._add_field(Prologue.class_id, Scope.PROLOGUE)

    def get_fields(self, *scopes):
        """
        Gets all of the field configurations defined for this packet type.

        Optionally, only fields within the given scope(s) are returned.
        """
        if not scopes:
            return self._fields
        return [field for field in self._fields if field.scope in scopes]

    def get_field(self, name):
        """
        Returns the configuration for a specific field by VITA 49 name.
        """
        key = name.casefold()
        for field in self._fields:
            if field.name.casefold() == key:
                return field
        raise KeyError(name)

    def _add_field(self, field, scope, *args, **kwds):
        """
        Adds a configuration for the given field.
        """
        config = FieldConfiguration.create(field, scope, *args, **kwds)
        self._fields.append(config)
        return config

    def _add_fields(self, container, scope):
        """
        Adds configurations for all the fields in a container.
        """
        for value in container.get_fields():
            field = self._add_field(value, scope)

            # User the same Python-friendly attribute name for easy lookup
            name = value.attr
            if hasattr(self, name):
                warnings.warn('Duplicate attribute name {}.{}'.format(container.__name__, name))
            setattr(self, name, field)

    def validate(self):
        """
        Validates the current configuration of this packet.
        """
        # Override or extend in subclasses to check for invalid combinations
        # of field configurations.

    @property
    def packet_type_code(self):
        """
        Returns the Packet Type Code for this packet configuration.
        """
        return self._get_packet_type_code()

    def _get_packet_type_code(self):
        raise NotImplementedError('packet_type_code')

class DataPacketConfiguration(PacketConfiguration):
    """
    Configuration for a Data Packet.
    """
    def __init__(self, name):
        super().__init__(name, PacketType.DATA)

        self._add_fields(Trailer, Scope.TRAILER)

    def _get_packet_type_code(self):
        if self.stream_id.is_enabled:
            return enums.PacketType.SIGNAL_DATA_STREAM_ID
        return enums.PacketType.SIGNAL_DATA

class CIFPacketConfiguration(PacketConfiguration):
    """
    Base class for packet types that contain Context Information Fields.
    """
    # pylint: disable=abstract-method
    def __init__(self, name, packet_type):
        super().__init__(name, packet_type)

        self.stream_id.mode = Mode.MANDATORY

        self._add_fields(CIF0, Scope.CIF0)
        self._add_fields(CIF1, Scope.CIF1)

class ContextPacketConfiguration(CIFPacketConfiguration):
    """
    Configuration for a Context Packet.
    """
    def __init__(self, name):
        super().__init__(name, PacketType.CONTEXT)
        self.timestamp_mode = enums.TSM()

    def _get_packet_type_code(self):
        return enums.PacketType.CONTEXT

class CommandPacketConfiguration(CIFPacketConfiguration):
    """
    Configuration for a Command Packet.
    """
    def __init__(self, name, packet_type):
        super().__init__(name, packet_type)
        self.controllee = None
        self.controller = None

        self.partial_permitted = self._add_field(
            ControlAcknowledgeMode.permit_partial, Scope.CAM, Mode.MANDATORY
        )
        self.warnings = self._add_field(
            ControlAcknowledgeMode.permit_warnings, Scope.CAM, Mode.MANDATORY
        )
        self.errors = self._add_field(
            ControlAcknowledgeMode.permit_errors, Scope.CAM, Mode.MANDATORY
        )
        self.action = self._add_field(
            ControlAcknowledgeMode.action_mode, Scope.CAM, Mode.MANDATORY
        )
        self.nack = self._add_field(
            ControlAcknowledgeMode.nack_only, Scope.CAM, Mode.MANDATORY
        )
        self.ackv = self._add_field(
            ControlAcknowledgeMode.request_validation, Scope.CAM, Mode.MANDATORY
        )
        self.ackx = self._add_field(
            ControlAcknowledgeMode.request_execution, Scope.CAM, Mode.MANDATORY
        )
        self.acks = self._add_field(
            ControlAcknowledgeMode.request_query, Scope.CAM, Mode.MANDATORY
        )

    def get_acknowledge(self, packet_type):
        """
        Returns the field associated with the given acknowledgement packet
        type.
        """
        if packet_type == PacketType.ACKV:
            return self.ackv
        if packet_type == PacketType.ACKX:
            return self.ackx
        if packet_type == PacketType.ACKS:
            return self.acks
        raise ValueError(packet_type)

    def _get_packet_type_code(self):
        return enums.PacketType.COMMAND

class ControlPacketConfiguration(CommandPacketConfiguration):
    """
    Configuration for a Control Packet.
    """
    def __init__(self, name):
        super().__init__(name, PacketType.CONTROL)

class AcknowledgePacketConfiguration(CommandPacketConfiguration):
    """
    Configuration for an Acknowledge Packet.
    """
    def __init__(self, name, packet_type):
        super().__init__(name, packet_type)
        for field in (self.ackv, self.ackx, self.acks):
            field.value = False
            field.set_constant()
        ack = self.get_acknowledge(packet_type)
        ack.value = True

def create_packet(packet_type, name):
    args = [name]
    if packet_type == PacketType.DATA:
        cls = DataPacketConfiguration
    elif packet_type == PacketType.CONTEXT:
        cls = ContextPacketConfiguration
    elif packet_type == PacketType.CONTROL:
        cls = ControlPacketConfiguration
    elif packet_type in (PacketType.ACKV, PacketType.ACKX, PacketType.ACKS):
        cls = AcknowledgePacketConfiguration
        args.append(packet_type)
    else:
        raise ValueError(packet_type)
    return cls(*args)
