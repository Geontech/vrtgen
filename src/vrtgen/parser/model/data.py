from dataclasses import dataclass, field

from vrtgen.parser.model.types.enums import *
from vrtgen.parser import value
from vrtgen.parser.model.base import *
from vrtgen.parser.model.types import *
import math 
from vrtgen.parser.value import parse_spectrum_or_time

@dataclass
class DataHeader(Header):
    """
    VRT Packet Header with Data Packet-Specific Indicator Bits (5.1.1.1)
    """
    trailer_included : BooleanType = field(default_factory=lambda: BooleanType('trailer_included', packed_tag=PackedTag(26,1,0,0)))
    not_v49d0 : BooleanType = field(default_factory=lambda: BooleanType('not_v49d0', packed_tag=PackedTag(25,1,0,0)))
    spectrum_or_time : EnumType = field(default_factory=lambda: EnumType('spectrum_or_time', type_=SPECTRUM_OR_TIME, packed_tag=PackedTag(24,1,0,0)))
    
    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.packet_type.value = PacketType.SIGNAL_DATA

@dataclass
class Trailer(StateEventIndicators):
    """
    Signal Data Packet trailer (5.1.6).
    """
    name : str = 'trailer'
    # Can't be on at the same time as user_defined1 or user_defined2
    sample_frame : EnumType = field(default_factory=lambda: EnumType('sample_frame', type_=SSI, packed_tag=PackedTag(11,2,0,0)))
    # Associated Context Packet Count
    # associated_context_packets_count : IntegerType = field(default_factory=lambda: IntegerType('associated_context_packets_count', bits=7, packed_tag=PackedTag(6,7,0,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

    def _validate(self, mapping):
        for field in mapping:
            if 'user_defined' in field:
                continue
            if not field in [f.name for f in self.fields]:
                raise ValueError('invalid field provided: ' + field)

    @property
    def none_enabled(self):
        return not any(field.enabled for field in self.fields)

    @property
    def is_user_defined(self):
        return super().is_user_defined or self.user_defined

    @property
    def fields(self):
        return super().fields #[f for f in super().fields] + [self.sample_frame, self.associated_context_packets_count]

    def _parse_mapping(self, mapping):
        packed_tag_pos = 8
        greater_than_two = False
        try:
            for key,val in mapping.items():
                if not key in [f.name for f in self.fields]:
                    # self.type_ = 'UserDefinedTrailer'
                    if val == 'enable_indicator':
                        self.subfields.append(EnableIndicatorType(key, bits=1, user_defined=True, enabled=True, required=True, packed_tag=PackedTag(packed_tag_pos,1,0,0)))
                        self.subfields.append(EnableIndicatorType(key+'_enable', user_defined=True, bits=1, enabled=True, required=True, is_enable=True, packed_tag=PackedTag(packed_tag_pos+12,1,0,0)))
                        packed_tag_pos += 1
                    elif isinstance(val, list):
                        bits = math.floor(math.log(len(val)-1) / math.log(2)) + 1
                        if bits > 4:
                            raise ValueError("Enum can only be 4 bits maximum")
                        enum_members = {}
                        member_val = 0
                        for item in val:
                            enum_members[item] = member_val
                            member_val += 1
                        user_defined_enum = BinaryEnum(key, enum_members)
                        user_defined_enum.bits = bits
                        if bits > 2:
                            greater_than_two = True
                        pos = packed_tag_pos + bits - 1
                        self.subfields.append(EnumType(key, enabled=True, required=True, user_defined=True, type_=user_defined_enum, packed_tag=PackedTag(pos,bits,0,0)))
                        self.subfields.append(EnableIndicatorType(key+'_enable', bits=bits, enabled=True, required=True, user_defined=True, is_enable=True, is_enum=True, packed_tag=PackedTag(pos+12,bits,0,0)))
                        packed_tag_pos += bits
                else:
                    mode = value.parse_enable(val)
                    self.__dict__[key].enabled = True if (mode == Mode.REQUIRED or mode == Mode.OPTIONAL) else False
                    self.__dict__[key].required = True if (mode == Mode.REQUIRED) else False

            if len(self.subfields) > 4 and self.sample_frame.enabled:
                raise ValueError('Cannot have user_defined_higher and sample_frame')
            if greater_than_two and self.sample_frame.enabled:
                raise ValueError('Cannot have user_defined_higher and sample_frame')
        except:
            raise

    def validate_and_parse_mapping(self, **mapping):
        self._validate(mapping)
        self._parse_mapping(mapping)

@dataclass
class DataPacket(Packet):
    header : DataHeader = field(default_factory=lambda: DataHeader(enabled=True, required=True))
    trailer : Trailer = field(default_factory=Trailer)

    def _update_header(self):
        if self.stream_id.enabled:
            self.header.packet_type.value = PacketType.SIGNAL_DATA_STREAM_ID
        if self.trailer.enabled:
            self.header.trailer_included.enabled = True
            self.header.trailer_included.value = True
        if self.trailer.sample_frame.enabled:
            self.header.not_v49d0.enabled = True
            self.header.not_v49d0.required = True
            self.header.not_v49d0.value = True
        if self.header.spectrum_or_time.enabled and self.header.spectrum_or_time.value == SPECTRUM_OR_TIME.SPECTRUM:
            self.header.not_v49d0.enabled = True
            self.header.not_v49d0.required = True
            self.header.not_v49d0.value = True
        super()._update_header()

    def _validate(self, mapping):
        for field in mapping:
            if not field in [f.name for f in (self.fields + self.header.fields)]:
                raise ValueError('invalid field provided: ' + field)

    def _parse_mapping(self, mapping):
        for key,val in mapping.items():
            if key == 'spectrum_or_time':
                try:
                    self.header.spectrum_or_time.enabled = True
                    self.header.spectrum_or_time.required = True
                    self.header.spectrum_or_time.value = parse_spectrum_or_time(val)
                except:
                    raise
            else:
                self.__dict__[key] = val
