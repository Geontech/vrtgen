from dataclasses import dataclass, field

from vrtgen.parser import value
from vrtgen.parser.model.base import *
from vrtgen.parser.model.types import *
import math 
@dataclass
class DataHeader(Header):
    """
    VRT Packet Header with Data Packet-Specific Indicator Bits (5.1.1.1)
    """
    trailer_included : BooleanType = field(default_factory=lambda: BooleanType('trailer_included', packed_tag=PackedTag(26,1,0,0)))
    not_v49d0 : BooleanType = field(default_factory=lambda: BooleanType('not_v49d0', packed_tag=PackedTag(25,1,0,0)))
    spectrum_or_time : BooleanType = field(default_factory=lambda: BooleanType('spectrum_or_time', packed_tag=PackedTag(24,1,0,0)))
    
    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.packet_type.value = PacketType.SIGNAL_DATA

@dataclass
class Trailer(PackedStruct):
    """
    Signal Data Packet trailer (5.1.6).
    """
    name : str = 'trailer'
    state_event_indicators : StateEventIndicators = field(default_factory=lambda: StateEventIndicators('state_event_indicators', packed_tag=PackedTag(19,12,0,0)))
    # Can't be on at the same time as user_defined1 or user_defined2
    sample_frame : EnumType = field(default_factory=lambda: EnumType('sample_frame', type_=SSI, packed_tag=PackedTag(11,2,0,0)))
    # Associated Context Packet Count
    associated_context_packet_count : IntegerType = field(default_factory=lambda: IntegerType('associated_context_packet_count', bits=7, packed_tag=PackedTag(6,7,0,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

    def _validate(self, mapping):
        for _,val in mapping.items():
            if val == 'required' or val == 'optional':
                continue
            elif val == 'enable_indicator':
                continue
            elif isinstance(val, list):
                continue
            else:
                raise ValueError('invalid discrete_io type specified: ', val)

    @property
    def fields(self):
        return [f for f in self.state_event_indicators.fields] + [self.__dict__[key] for key,_ in asdict(self).items() if  not isinstance(self.__dict__[key], StateEventIndicators) and is_field_type(self.__dict__[key])]

    def _parse_mapping(self, mapping):
        packed_tag_pos = 8
        greater_than_two = False
        try:
            for key,val in mapping.items():
                if not key in [f.name for f in self.fields]:
                    self.state_event_indicators.type_ = 'UserDefinedTrailer'
                    self.type_ = 'UserDefinedTrailer'
                    if val == 'enable_indicator':
                        self.state_event_indicators.subfields.append(EnableIndicatorType(key, bits=1, user_defined=True, enabled=True, required=True, packed_tag=PackedTag(packed_tag_pos,1,0,0)))
                        self.state_event_indicators.subfields.append(EnableIndicatorType(key+'_enable', user_defined=True, bits=1, enabled=True, required=True, is_enable=True, packed_tag=PackedTag(packed_tag_pos+12,1,0,0)))
                        packed_tag_pos += 1
                    elif isinstance(val, list):
                        bits = math.ceil(len(val)/2)
                        enum_members = {}
                        member_val = 0
                        for item in val:
                            enum_members[item] = member_val
                            member_val += 1
                        user_defined_enum = BinaryEnum(key, enum_members)
                        user_defined_enum.bits = bits
                        if bits > 4:
                            raise ValueError("Enum can only be 4 bits maximum")
                        elif bits > 2:
                            greater_than_two = True
                        pos = packed_tag_pos + bits - 1
                        self.state_event_indicators.subfields.append(EnumType(key, enabled=True, required=True, user_defined=True, type_=user_defined_enum, packed_tag=PackedTag(pos,bits,0,0)))
                        self.state_event_indicators.subfields.append(EnableIndicatorType(key+'_enable', bits=bits, enabled=True, required=True, user_defined=True, is_enum=True, is_enable=True, packed_tag=PackedTag(pos+12,bits,0,0)))
                        packed_tag_pos += bits
                elif key in [f.name for f in self.state_event_indicators.fields]:
                    mode = value.parse_enable(val)
                    self.state_event_indicators.__dict__[key].enabled = True if (mode == Mode.REQUIRED or mode == Mode.OPTIONAL) else False
                    self.state_event_indicators.__dict__[key].required = True if (mode == Mode.REQUIRED) else False
                else:
                    mode = value.parse_enable(val)
                    self.__dict__[key].enabled = True if (mode == Mode.REQUIRED or mode == Mode.OPTIONAL) else False
                    self.__dict__[key].required = True if (mode == Mode.REQUIRED) else False

            if len(self.state_event_indicators.subfields) > 4 and self.sample_frame.enabled:
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

    # def validate_and_parse_mapping(self, **mapping):
    #     self._validate(mapping)
    #     self._parse_mapping(mapping)

    def _update_header(self):
        if self.stream_id.enabled:
            self.header.packet_type.value = PacketType.SIGNAL_DATA_STREAM_ID
        if self.trailer.enabled:
            self.header.trailer_included.enabled = True
            self.header.trailer_included.value = True
        super()._update_header()