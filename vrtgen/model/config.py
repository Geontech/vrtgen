import inspect
import warnings

from vrtgen.types.struct import Field
from vrtgen.types import basic, enums, header
from vrtgen.types.cif0 import CIF0Fields
from vrtgen.types.cif1 import CIF1Fields

from .field import FieldConfiguration, Mode

class FieldContainer:
    def get_field(self, name):
        key = name.casefold()
        for _, field in inspect.getmembers(self, lambda x: isinstance(x, FieldConfiguration)):
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

# Header field name constants
STREAM_ID = 'Stream Identifier'
CLASS_ID = 'Class Identifier'

# Trailer field name constants
SAMPLE_FRAME = 'Sample Frame'

class PrologueConfiguration(FieldContainer):
    def __init__(self):
        super().__init__()

        self.tsi = FieldConfiguration.create(header.Header.tsi, Mode.MANDATORY)
        self.tsi.value = enums.TSI()
        self.tsf = FieldConfiguration.create(header.Header.tsf, Mode.MANDATORY)
        self.tsf.value = enums.TSF()
        self.stream_id = self._create_field(STREAM_ID, basic.Identifier32)
        self.class_id = self._create_field(CLASS_ID, header.ClassIdentifier)
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
        self.trailer = None

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
