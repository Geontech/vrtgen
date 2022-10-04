from dataclasses import dataclass, field

from vrtgen.parser.model.types import *
from vrtgen.parser.model.base import Header, Packet
from vrtgen.parser.model.prologue import ClassIdentifier, Timestamp
from vrtgen.parser.model.cif0 import CIF0
from vrtgen.parser.model.cif1 import CIF1
from vrtgen.parser.model.cif2 import CIF2
from vrtgen.parser.model.cif7 import CIF7
from vrtgen.parser.value import parse_tsm

@dataclass
class ContextHeader(Header):
    """
    VRT Packet Header with Context Packet-Specific Indicator Bits (5.1.1.1)
    """
    not_v49d0 : BooleanType = field(default_factory=lambda: BooleanType('not_v49d0', packed_tag=PackedTag(25,1,0,0)))
    tsm : EnumType = field(default_factory=lambda: EnumType('tsm', type_=TSM, packed_tag=PackedTag(24,1,0,0)))
    
    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.packet_type.value = PacketType.CONTEXT

@dataclass
class ContextPacket(Packet):
    header : ContextHeader = field(default_factory=lambda: ContextHeader(enabled=True, required=True))
    # stream_id : StreamIdentifier = field(default_factory=lambda: StreamIdentifier(enabled=True, required=True))
    # class_id  : ClassIdentifier = field(default_factory=ClassIdentifier)
    # timestamp : Timestamp = field(default_factory=Timestamp)
    cif_0 : CIF0 = field(default_factory=lambda: CIF0(enabled=True, required=True))
    cif_1 : CIF1 = field(default_factory=CIF1)
    cif_2 : CIF2 = field(default_factory=CIF2)
    cif_7 : CIF7 = field(default_factory=CIF7)

    def __post_init__(self):
        super().__post_init__()
        self.stream_id.enabled = True
        self.stream_id.required = True

    # def validate_and_parse_mapping(self, **mapping):
    #     self._validate(mapping)
    #     self._parse_mapping(mapping)

    def _validate(self, mapping):
        for field in mapping:
            if not field in [f.name for f in (self.fields + self.header.fields)]:
                raise ValueError('invalid field provided: ' + field)

    def _parse_mapping(self, mapping):
        for key,val in mapping.items():
            if key == 'tsm':
                try:
                    self.header.tsm.value = parse_tsm(val)
                except:
                    raise
            else:
                self.__dict__[key] = val

@dataclass
class ExtensionContextPacket(Packet):
    """
    Extension Context Packet
    """
    
    def __post_init__(self):
        super().__post_init__()
        self.header.packet_type = PacketType.EXTENSION_CONTEXT
