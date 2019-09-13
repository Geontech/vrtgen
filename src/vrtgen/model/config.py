"""
Types for VITA 49 packet configurations.
"""

from enum import Enum, auto
import warnings

from vrtgen.types import enums
from vrtgen.types.prologue import Prologue
from vrtgen.types.trailer import Trailer
from vrtgen.types.cif0 import CIF0
from vrtgen.types.cif1 import CIF1
from vrtgen.types.control import ControlAcknowledgeMode

from .field import FieldConfiguration, Mode, Scope

class Acknowledgement(Enum):
    """
    Acknowledgment packet types.
    """
    VALIDATION = auto()
    EXECUTION = auto()
    QUERY_STATE = auto()

class PacketConfiguration:
    """
    Base class for VRT packet configuration.
    """
    def __init__(self, name):
        self.name = name
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

    def packet_type(self):
        """
        Returns the Packet Type Code for this packet configuration.
        """
        raise NotImplementedError('packet_type')

class DataPacketConfiguration(PacketConfiguration):
    """
    Configuration for a Data Packet.
    """
    def __init__(self, name):
        super().__init__(name)

        self._add_fields(Trailer, Scope.TRAILER)

    def packet_type(self):
        if self.stream_id.is_enabled:
            return enums.PacketType.SIGNAL_DATA_STREAM_ID
        return enums.PacketType.SIGNAL_DATA

class CIFPacketConfiguration(PacketConfiguration):
    """
    Base class for packet types that contain Context Information Fields.
    """
    # pylint: disable=abstract-method
    def __init__(self, name):
        super().__init__(name)

        self.stream_id.mode = Mode.MANDATORY

        self._add_fields(CIF0, Scope.CIF0)
        self._add_fields(CIF1, Scope.CIF1)

class ContextPacketConfiguration(CIFPacketConfiguration):
    """
    Configuration for a Context Packet.
    """
    def __init__(self, name):
        super().__init__(name)
        self.timestamp_mode = enums.TSM()

    def packet_type(self):
        return enums.PacketType.CONTEXT

class CommandPacketConfiguration(CIFPacketConfiguration):
    """
    Configuration for a Command Packet.
    """
    def __init__(self, name):
        super().__init__(name)
        self.acknowledge = []
        self.controllee = None
        self.controller = None

        self._action = self._add_field(ControlAcknowledgeMode.action, Scope.PROLOGUE, Mode.MANDATORY)
        self._nack = self._add_field(ControlAcknowledgeMode.nack, Scope.PROLOGUE, Mode.MANDATORY)

        self._action.value = enums.ActionMode()
        self._nack.value = False

    def packet_type(self):
        return enums.PacketType.COMMAND

    @property
    def action(self):
        """
        The Action Mode of this packet.
        """
        return self._action.value

    @property
    def nack(self):
        """
        True if this packet only requires acknowledgement on failures.
        """
        return self._nack.value
