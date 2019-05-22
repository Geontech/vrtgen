import inspect
import warnings

from vrtgen.types.struct import Field
from vrtgen.types import basic, enums
from vrtgen.types.prologue import Prologue, Header
from vrtgen.types.trailer import Trailer
from vrtgen.types.cif0 import CIF0
from vrtgen.types.cif1 import CIF1

from .field import FieldConfiguration, Mode

class FieldContainer:
    def get_field(self, name):
        key = name.casefold()
        for _, field in inspect.getmembers(self, lambda x: isinstance(x, FieldConfiguration)):
            if field.name.casefold() == key:
                return field
        return None

    def _add_fields(self, container):
        for name, value in inspect.getmembers(container, lambda x: isinstance(x, Field)):
            field = FieldConfiguration.create(value)

            # User the same Python-friendly attribute name for easy lookup
            if hasattr(self, name):
                warnings.warn('Duplicate attribute name {}.{}'.format(cif.__name__, name))
            setattr(self, name, field)

class CIFPayloadConfiguration(FieldContainer):
    def __init__(self):
        super().__init__()
        self._add_fields(CIF0)
        self._add_fields(CIF1)

class PrologueConfiguration(FieldContainer):
    def __init__(self):
        super().__init__()

        self.tsi = FieldConfiguration.create(Header.tsi, Mode.MANDATORY)
        self.tsi.value = enums.TSI()
        self.tsf = FieldConfiguration.create(Header.tsf, Mode.MANDATORY)
        self.tsf.value = enums.TSF()

        self._add_fields(Prologue)

class TrailerConfiguration(FieldContainer):
    def __init__(self):
        super().__init__()
        self._add_fields(Trailer)


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
        self.trailer = TrailerConfiguration()

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
