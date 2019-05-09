from enum import IntEnum
import inspect

from vrtgen.types.struct import Field, Struct

class Mode(IntEnum):
    DISABLED = 0
    OPTIONAL = 1
    REQUIRED = 2
    MANDATORY = 3

class FieldConfiguration:
    def __init__(self, field, mode=Mode.DISABLED):
        self.field = field
        self._mode = mode
        self._constant = False

    @property
    def name(self):
        return self.field.name

    @property
    def type(self):
        return self.field.type

    @property
    def mode(self):
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
        return self._mode != Mode.DISABLED

    @property
    def is_set(self):
        if self.has_value:
            return True
        return self.is_mandatory

    @property
    def is_required(self):
        return self._mode in (Mode.REQUIRED, Mode.MANDATORY)

    @property
    def is_optional(self):
        return self._mode == Mode.OPTIONAL

    @property
    def is_mandatory(self):
        return self._mode == Mode.MANDATORY

    @property
    def is_disabled(self):
        return self._mode == Mode.DISABLED

    @property
    def is_constant(self):
        return self._constant

    def set_constant(self):
        if not self.is_required:
            self._mode = Mode.REQUIRED
        self._constant = True

    @property
    def has_value(self):
        raise NotImplementedError('has_value')

    @staticmethod
    def create(field, mode=Mode.DISABLED):
        # The type may be none for unimplemented CIF fields
        if field.type is not None and issubclass(field.type, Struct):
            return StructFieldConfiguration(field, mode=mode)
        else:
            return SimpleFieldConfiguration(field, mode=mode)

class SimpleFieldConfiguration(FieldConfiguration):
    def __init__(self, field, mode=Mode.DISABLED):
        super().__init__(field, mode)
        self.value = None

    @property
    def has_value(self):
        return self.value is not None

class StructFieldConfiguration(FieldConfiguration):
    def __init__(self, field, mode=Mode.DISABLED):
        super().__init__(field, mode)
        self._fields = {}
        for name, value in inspect.getmembers(field.type, lambda x: isinstance(x, Field)):
            # By default, all struct fields are required. They can marked as
            # optional or unused later.
            subfield = FieldConfiguration.create(value, Mode.REQUIRED)
            setattr(self, name, subfield)
            self._fields[subfield.name] = subfield

    def get_field(self, name):
        return self._fields.get(name, None)

    @property
    def has_value(self):
        return any(f.has_value for f in self._fields.values())
