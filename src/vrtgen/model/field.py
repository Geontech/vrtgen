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
Types for configuration of packet fields and subfields.
"""

from enum import Enum, auto

from vrtgen.types import struct
from vrtgen.types import user

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
        PROLOGUE: Packet prologue (excluding CIF headers).
        CAM: ControlAcknowledgeMode field (Command packets only).
        CIF0: CIF 0 fields (Context and Command packets only).
        CIF1: CIF 1 fields (Context and Command packets only).
        TRAILER: Packet trailer (Data packet only).
    """
    HEADER = auto()
    PROLOGUE = auto()
    CAM = auto()
    CIF0 = auto()
    CIF1 = auto()
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

    @property
    def is_user_defined(self):
        """
        True if this field has user-defined subfields.
        """
        return False

    @staticmethod
    def create(field, scope, mode=Mode.DISABLED):
        """
        Creates the correct subclass of FieldConfiguration for a given field
        instance.
        """
        # The type may be none for unimplemented CIF fields
        if user.is_user_defined(field.type):
            cls = UserDefinedFieldConfiguration
        elif struct.is_struct(field.type):
            cls = StructFieldConfiguration
        else:
            cls = SimpleFieldConfiguration
        return cls(field, scope, mode=mode)

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

class ContainerFieldConfiguration(FieldConfiguration):
    """
    Base class for field types that contain subfields.
    """
    def __init__(self, *args, **kwds):
        super().__init__(*args, **kwds)
        self._fields = {}

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

    def _add_field(self, field, mode):
        config = FieldConfiguration.create(field, self.scope, mode)
        setattr(self, field.attr, config)
        self._fields[field.name.casefold()] = config
        return config

class UserDefinedFieldConfiguration(ContainerFieldConfiguration):
    """
    Configuration for user-defined structure field types.
    """
    def __init__(self, *args, **kwds):
        super().__init__(*args, **kwds)
        self._type = None
        self._namespace = {}

    @property
    def type(self):
        if self._type is None:
            cls = self.field.type
            self._type = user.create_struct(cls, cls.__name__, self._namespace)
        return self._type

    @type.setter
    def type(self, dtype):
        assert self._type is None
        self._type = dtype

    @property
    def is_user_defined(self):
        return True

    def add_field(self, field):
        """
        Adds a field defintion to the user-defined struct type.
        """
        # Disallow adding fields after the struct definition has been created
        assert self._type is None
        if struct.is_reserved(field):
            count = sum((k.startswith('reserved_') for k in self._namespace))
            attr = 'reserved_{}'.format(count + 1)
            # Guard against pathological breakage...
            assert attr not in self._namespace
        else:
            attr = '_'.join(field.name.lower().split())
            if struct.is_enable(field):
                attr += '_enable'
            if attr in self._namespace:
                raise ValueError("duplicate name '{}'".format(field.name))
        self._namespace[attr] = field

        # Create field configuration for editable fields
        if struct.is_field(field):
            # Assume optional if there is a linked enable
            mode = Mode.OPTIONAL if field.enable else Mode.REQUIRED
            self._add_field(field, mode)

class StructFieldConfiguration(ContainerFieldConfiguration):
    """
    Configuration for a structured field type.
    """
    def __init__(self, field, scope, mode=Mode.DISABLED):
        super().__init__(field, scope, mode)
        for subfield in field.type.get_fields():
            # By default, all struct fields are required. They can marked as
            # optional or unused later.
            self._add_field(subfield, Mode.REQUIRED)
