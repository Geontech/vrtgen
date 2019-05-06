from enum import IntEnum
import inspect
import warnings

from vrtgen.types.struct import Field, Struct
from vrtgen.types import basic, enums, header
from vrtgen.types.cif0 import CIF0Fields
from vrtgen.types.cif1 import CIF1Fields

class Mode(IntEnum):
    DISABLED  = 0
    OPTIONAL  = 1
    REQUIRED  = 2
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
            self.enable = Mode.REQUIRED
        self._constant = True

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

class FieldContainer:
    def get_field(self, name):
        key = name.casefold()
        for attr, field in inspect.getmembers(self, lambda x: isinstance(x, FieldConfiguration)):
            if field.name.casefold() == key:
                return field
        return None

class CIFPayloadConfiguration(FieldContainer):
    def __init__(self):
        super().__init__()
        self._add_cif_fields(CIF0Fields)
        self._add_cif_fields(CIF1Fields)

    def _add_cif_fields(self, cif):
        for name, value in inspect.getmembers(cif, lambda x: isinstance(x, Field)):
            field = FieldConfiguration.create(value)

            # User the same Python-friendly attribute name for easy lookup
            if hasattr(self, name):
                warnings.warn('Duplicate attribute name {}.{}'.format(cif.__name__, name))
            setattr(self, name, field)

class PrologueConfiguration(FieldContainer):
    def __init__(self):
        super().__init__()

        self.tsi = FieldConfiguration.create(header.Header.tsi, Mode.MANDATORY)
        self.tsi.value = enums.TSI()
        self.tsf = FieldConfiguration.create(header.Header.tsf, Mode.MANDATORY)
        self.tsf.value = enums.TSF()
        self.stream_id = self._create_field('Stream Identifier', basic.Identifier32)
        self.class_id = self._create_field('Class Identifier', header.ClassIdentifier)
        self.integer_timestamp = self._create_field('Integer Timestamp', basic.Integer32)
        self.fractional_timestamp = self._create_field('Fractional Timestamp', basic.Integer64)

    def _create_field(self, name, type, **kwds):
        field = Field(name, type, **kwds)
        return FieldConfiguration.create(field)

class PacketConfiguration:
    def __init__(self, name):
        self.name = name
        self.prologue = PrologueConfiguration()

    def get_field(self, name):
        return self.payload.get_field(name)

    def validate(self):
        pass

class DataPacketConfiguration(PacketConfiguration):
    def __init__(self, name):
        super().__init__(name)

class ContextPacketConfiguration(PacketConfiguration):
    def __init__(self, name):
        super().__init__(name)
        self.prologue.stream_id.mode = Mode.MANDATORY
        self.payload = CIFPayloadConfiguration()

class CommandPacketConfiguration(PacketConfiguration):
    def __init__(self, name):
        super().__init__(name)
        self.prologue.stream_id.mode = Mode.MANDATORY
        self.payload = CIFPayloadConfiguration()
