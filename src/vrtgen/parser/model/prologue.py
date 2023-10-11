from dataclasses import dataclass, field

from vrtgen.parser import value
from vrtgen.parser.model.types import *

@dataclass
class ClassIdentifier(PackedStruct):
    """
    Class Identifier (5.1.3).
    """
    name : str = 'class_id'
    pad_bits : IntegerType = field(default_factory=lambda: IntegerType('pad_bits', bits=5, enabled=True, required=True, packed_tag=PackedTag(31,5,0,0)))
    oui : OUI = field(default_factory=lambda: OUI('oui', bits=24, enabled=True, required=True, packed_tag=PackedTag(23,24,0)))
    information_code : IntegerType = field(default_factory=lambda: IntegerType('information_code', bits=16, enabled=True, required=True, packed_tag=PackedTag(31,16,1,1)))
    packet_code : IntegerType = field(default_factory=lambda: IntegerType('packet_code', bits=16, enabled=True, required=True, packed_tag=PackedTag(15,16,1,1)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 64

    def parse_mapping(self, **mapping):
        for key,val in mapping.items():
            try:
                if key == 'oui':
                    self.oui.value = value.parse_oui(val)
                elif key == 'packet_code':
                    self.packet_code.value = val
                elif key == 'information_code':
                    self.packet_code.value = val
                else:
                    raise KeyError(val)
            except:
                raise

@dataclass
class Timestamp(Field):
    name       : str = 'timestamp'
    tsi        : EnumType = field(default_factory=lambda: EnumType('tsi', type_=TSI))
    tsf        : EnumType = field(default_factory=lambda: EnumType('tsf', type_=TSF))
    integer    : IntegerTimestamp = field(default_factory=IntegerTimestamp)
    fractional : FractionalTimestamp = field(default_factory=FractionalTimestamp)

    def __post_init__(self):
        self.type_ = type(self).__name__
        self.bits = 0

    def parse_mapping(self, **mapping):
        for key,val in mapping.items():
            if key == 'integer':
                self.tsi.required = True
                self.tsi.enabled = True
                self.tsi.value = value.parse_tsi(val)
                self.integer.enabled = True
                self.integer.required = True
                self.bits += self.integer.bits
            elif key == 'fractional':
                self.tsf.required = True
                self.tsf.enabled = True
                self.tsf.value = value.parse_tsf(val)
                self.fractional.enabled = True
                self.fractional.required = True
                self.bits += self.fractional.bits
            else:
                raise KeyError(val)
