"""
Types for configuration of packet fields and subfields.
"""

from enum import Enum, auto

from vrtgen.types.struct import Struct

class Mode(Enum):
    """
    Defines the usage of a field or subfield:
        DISABLED: Field is never present.
        OPTIONAL: Field can be enabled or disabled at runtime.
        REQUIRED: Field is always present.
        MANDATORY: Field is present and cannot and be disabled or optional.
    """
    DISABLED = auto()
    OPTIONAL = auto()
    REQUIRED = auto()
    MANDATORY = auto()

class Scope(Enum):
    """
    Defines the section of the packet in which a field resides:
        PROLOGUE: Packet prologue.
        PAYLOAD: Packet payload (Context and Command Packets).
        TRAILER: Packet trailer (Data Packet only).
    """
    PROLOGUE = auto()
    PAYLOAD = auto()
    TRAILER = auto()

class FieldConfiguration:
    """
    Base class to represent the packet-specific configuration of a particular
    field or subfield.
    """
    def __init__(self, field, scope, mode=Mode.DISABLED):
        self.field = field
        self.scope = scope
        self._mode = mode
        self._constant = False

    @property
    def name(self):
        """
        VITA 49.2 field or subfield name.
        """
        return self.field.name

    @property
    def type(self):
        """
        Data type of the field.
        """
        return self.field.type

    @property
    def mode(self):
        """
        The configured usage of this field.

        See Mode.
        """
        return self._mode

    @mode.setter
    def mode(self, mode):
        mode = Mode(mode)
        if self.is_mandatory:
            # Mandatory state cannot be overwritten
            if mode in (Mode.DISABLED, Mode.OPTIONAL):
                raise ValueError('field is mandatory')
            return
        self._mode = mode

    @property
    def is_enabled(self):
        """
        True if this field may be present in a packet.
        """
        return self._mode != Mode.DISABLED

    @property
    def is_set(self):
        """
        True if this field is required, or is optional and has a value.
        """
        if self.has_value:
            return True
        return self.is_required

    @property
    def is_required(self):
        """
        True if this field must be present in the containing packet.
        """
        return self._mode in (Mode.REQUIRED, Mode.MANDATORY)

    @property
    def is_optional(self):
        """
        True if this field can be enabled or disabled at runtime.
        """
        return self._mode == Mode.OPTIONAL

    @property
    def is_mandatory(self):
        """
        True if this field must be present for all packets and cannot be
        disabled.
        """
        return self._mode == Mode.MANDATORY

    @property
    def is_disabled(self):
        """
        True if this field is not present in the containing packet.
        """
        return self._mode == Mode.DISABLED

    @property
    def is_constant(self):
        """
        True if this field's value is immutable.
        """
        return self._constant

    def set_constant(self):
        """
        Set or clear the constant status of this field's value.
        """
        if not self.is_required:
            self._mode = Mode.REQUIRED
        self._constant = True

    @property
    def has_value(self):
        """
        True if a value has been explicitly set for this field.
        """
        raise NotImplementedError('has_value')

    @property
    def value(self):
        """
        The current configured value of this field.
        """
        if not self.is_set:
            return None
        return self._get_value()

    @value.setter
    def value(self, newval):
        self._set_value(newval)

    @staticmethod
    def create(field, scope, mode=Mode.DISABLED):
        """
        Creates the correct subclass of FieldConfiguration for a given field
        instance.
        """
        # The type may be none for unimplemented CIF fields
        if field.type is not None and issubclass(field.type, Struct):
            return StructFieldConfiguration(field, scope, mode=mode)
        return SimpleFieldConfiguration(field, scope, mode=mode)

    def _get_value(self):
        raise NotImplementedError('_get_value')

    def _set_value(self, value):
        raise NotImplementedError('_set_value')

class SimpleFieldConfiguration(FieldConfiguration):
    """
    Configuration for a simple field type (integer, fixed-point, etc.).
    """
    def __init__(self, field, scope, mode=Mode.DISABLED):
        super().__init__(field, scope, mode)
        self._value = None

    @property
    def has_value(self):
        return self._value is not None

    def _get_value(self):
        if not self.is_set:
            return None
        if self._value is None:
            return self.field.type()
        return self._value

    def _set_value(self, value):
        self._value = value

class StructFieldConfiguration(FieldConfiguration):
    """
    Configuration for a structured field type.
    """
    def __init__(self, field, scope, mode=Mode.DISABLED):
        super().__init__(field, scope, mode)
        self._fields = {}
        for subfield in field.type.get_fields():
            # By default, all struct fields are required. They can marked as
            # optional or unused later.
            config = FieldConfiguration.create(subfield, self.scope, Mode.REQUIRED)
            setattr(self, subfield.attr, config)
            self._fields[subfield.name.casefold()] = config

    def get_field(self, name):
        """
        Returns the configuration for a subfield by VITA 49.2 name.
        """
        return self._fields.get(name.casefold(), None)

    def get_fields(self):
        """
        Returns the configurations for all subfields.
        """
        return self._fields.values()

    @property
    def has_value(self):
        return any(f.has_value for f in self._fields.values())

    def _get_value(self):
        result = self.field.type()
        for field in self._fields.values():
            if not field.has_value:
                continue
            result.set_value(field.name, field.value)
        return result

    def _set_value(self, value):
        raise AttributeError('cannot set struct value')
