import warnings

from vrtgen.types import enums
from vrtgen.types.prologue import Prologue, Header
from vrtgen.types.trailer import Trailer
from vrtgen.types.cif0 import CIF0
from vrtgen.types.cif1 import CIF1

from .field import FieldConfiguration, Mode

class FieldContainer:
    def __init__(self):
        self._fields = []

    def get_fields(self):
        return self._fields

    def get_field(self, name):
        key = name.casefold()
        for field in self._fields:
            if field.name.casefold() == key:
                return field
        raise KeyError(name)

    def _add_field(self, field, *args, **kwds):
        config = FieldConfiguration.create(field, *args, **kwds)
        self._fields.append(config)
        return config

    def _add_fields(self, container):
        for value in container.get_fields():
            field = self._add_field(value)

            # User the same Python-friendly attribute name for easy lookup
            name = value.attr
            if hasattr(self, name):
                warnings.warn('Duplicate attribute name {}.{}'.format(container.__name__, name))
            setattr(self, name, field)

class CIFPayloadConfiguration(FieldContainer):
    def __init__(self):
        super().__init__()
        self._add_fields(CIF0)
        self._add_fields(CIF1)

class PrologueConfiguration(FieldContainer):
    def __init__(self):
        super().__init__()

        self.tsi = self._add_field(Header.tsi, Mode.MANDATORY)
        self.tsi.value = enums.TSI()
        self.tsf = self._add_field(Header.tsf, Mode.MANDATORY)
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

    def validate(self):
        pass

class DataPacketConfiguration(PacketConfiguration):
    def __init__(self, name):
        super().__init__(name)
        self.trailer = TrailerConfiguration()

class ContextPacketConfiguration(PacketConfiguration):
    def __init__(self, name):
        super().__init__(name)
        self.prologue.stream_id.mode = Mode.MANDATORY # pylint: disable=no-member
        self.payload = CIFPayloadConfiguration()

class CommandPacketConfiguration(PacketConfiguration):
    def __init__(self, name):
        super().__init__(name)
        self.prologue.stream_id.mode = Mode.MANDATORY # pylint: disable=no-member
        self.payload = CIFPayloadConfiguration()
