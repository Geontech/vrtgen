"""
Types for configuration of packet fields and subfields.
"""

from enum import IntEnum

from vrtgen.types.struct import Struct

class Mode(IntEnum):
    """
    Defines the usage of a field or subfield:
        DISABLED: Field is never present.
        OPTIONAL: Field can be enabled or disabled at runtime.
        REQUIRED: Field is always present.
        MANDATORY: Field is present and cannot and be disabled or optional.
    """
    DISABLED = 0
    OPTIONAL = 1
    REQUIRED = 2
    MANDATORY = 3

class FieldConfiguration:
    """
    Base class to represent the packet-specific configuration of a particular
    field or subfield.
    """
    def __init__(self, field, mode=Mode.DISABLED):
        self.field = field
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
        return self.is_mandatory

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

    @staticmethod
    def create(field, mode=Mode.DISABLED):
        """
        Creates the correct subclass of FieldConfiguration for a given field
        instance.
        """
        # The type may be none for unimplemented CIF fields
        if field.type is not None and issubclass(field.type, Struct):
            return StructFieldConfiguration(field, mode=mode)
        return SimpleFieldConfiguration(field, mode=mode)

class SimpleFieldConfiguration(FieldConfiguration):
    """
    Configuration for a simple field type (integer, fixed-point, etc.).
    """
    def __init__(self, field, mode=Mode.DISABLED):
        super().__init__(field, mode)
        self.value = None

    @property
    def has_value(self):
        return self.value is not None

class StructFieldConfiguration(FieldConfiguration):
    """
    Configuration for a structured field type.
    """
    def __init__(self, field, mode=Mode.DISABLED):
        super().__init__(field, mode)
        self._fields = {}
        for subfield in field.type.get_fields():
            # By default, all struct fields are required. They can marked as
            # optional or unused later.
            config = FieldConfiguration.create(subfield, Mode.REQUIRED)
            setattr(self, subfield.attr, config)
            self._fields[subfield.name.casefold()] = config

    def get_field(self, name):
        """
        Returns the configuration for a subfield by VITA 49.2 name.
        """
        return self._fields.get(name.casefold(), None)

    @property
    def has_value(self):
        return any(f.has_value for f in self._fields.values())
