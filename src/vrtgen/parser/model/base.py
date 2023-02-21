from dataclasses import dataclass, field, asdict

from vrtgen.parser.model.types import *
from vrtgen.parser.model.prologue import ClassIdentifier, Timestamp
from vrtgen.parser.value import parse_enable

@dataclass
class Header(PackedStruct):
    """
    VRT Packet Header (5.1.1).
    """
    name : str = 'header'
    packet_type : EnumType = field(default_factory=lambda: EnumType('packet_type', type_=PacketType, enabled=True, required=True, packed_tag=PackedTag(31,4,0,0)))
    class_id_enable : BooleanType = field(default_factory=lambda: BooleanType('class_id_enable', packed_tag=PackedTag(27,1,0,0)))
    tsi : EnumType = field(default_factory=lambda: EnumType('tsi', type_=TSI, packed_tag=PackedTag(23,2,0,0)))
    tsf : EnumType = field(default_factory=lambda: EnumType('tsf', type_=TSF, packed_tag=PackedTag(21,2,0,0)))
    packet_count : IntegerType = field(default_factory=lambda: IntegerType('packet_count', bits=4, enabled=True, required=True, packed_tag=PackedTag(19,4,0,0)))
    packet_size : Unsigned16 = field(default_factory=lambda: Unsigned16('packet_size', enabled=True, required=True, packed_tag=PackedTag(15,16,0,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

@dataclass
class CIF(PackedStruct):
    """
    VRT Context/Command Indiciator Field (CIF) Maxtrix Base class
    """
    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

    @property
    def is_optional(self):
        return super().is_optional or self.all_optional_fields

    @property
    def has_enabled_fields(self):
        for field in self.fields:
            if isinstance(field, PackedType):
                continue
            if field.indicator_only:
                continue
            if field.enabled:
                return True
        return False

    @property
    def all_optional_fields(self):
        if not self.has_enabled_fields:
            return False
        for field in self.fields:
            if field.enabled and field.required:
                return False
        return True

    def _validate(self, mapping):
        for field in mapping:
            if not field in [f.name for f in self.fields]:
                raise ValueError('invalid field provided: ' + field)

    def _parse_mapping(self, mapping):
        for key,val in mapping.items():
            mode = parse_enable(val)
            self.__dict__[key].enabled = True if (mode == Mode.REQUIRED or mode == Mode.OPTIONAL) else False
            self.__dict__[key].required = True if (mode == Mode.REQUIRED) else False

    def validate_and_parse_mapping(self, **mapping):
        self._validate(mapping)
        self._parse_mapping(mapping)

@dataclass
class Packet:
    """
    Base VRT Packet
    """
    header : Header = field(default_factory=lambda: Header(enabled=True, required=True))
    stream_id : StreamIdentifier = field(default_factory=StreamIdentifier)
    class_id  : ClassIdentifier = field(default_factory=ClassIdentifier)
    timestamp : Timestamp = field(default_factory=Timestamp)

    def __post_init__(self):
        self.type_ = type(self).__name__

    @property
    def fields(self):
        return [self.__dict__[key] for key,_ in asdict(self).items() if is_field_type(self.__dict__[key])]

    def validate_and_parse_mapping(self, **mapping):
        self._validate(mapping)
        self._parse_mapping(mapping)
        self._update_header()

    def _validate(self, mapping):
        for field in mapping:
            if not field in [f.name for f in self.fields]:
                raise ValueError('invalid field provided: ' + field)

    def _parse_mapping(self, mapping):
        for key,value in mapping.items():
            self.__dict__[key] = value

    def _update_header(self):
        if self.class_id.enabled:
            self.header.class_id_enable.enabled = True
            self.header.class_id_enable.value = True
        if self.timestamp.integer.enabled:
            self.header.tsi.enabled = True
            self.header.tsi.value = self.timestamp.tsi.value
        if self.timestamp.fractional.enabled:
            self.header.tsf.enabled = True
            self.header.tsf.value = self.timestamp.tsf.value

@dataclass
class StateEventIndicators(PackedStruct):
    """
    State and Event Indicator Field [9.10.8].
    """
    name : str = 'state_event_indicators'
    # Indicators
    # calibrated_time : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('calibrated_time', packed_tag=PackedTag(19,1,0,0)))
    # valid_data : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('valid_data', packed_tag=PackedTag(18,1,0,0)))
    # reference_lock : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('reference_lock', packed_tag=PackedTag(17,1,0,0)))
    # agc_mgc : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('agc_mgc', packed_tag=PackedTag(16,1,0,0)))
    # detected_signal : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('detected_signal', packed_tag=PackedTag(15,1,0,0)))
    # spectral_inversion : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('spectral_inversion', packed_tag=PackedTag(14,1,0,0)))
    # over_range : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('over_range', packed_tag=PackedTag(13,1,0,0)))
    # sample_loss : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('sample_loss', packed_tag=PackedTag(12,1,0,0)))
    subfields : List[Field] = field(default_factory=list)
    packed_0 : PackedType = field(default_factory=lambda: PackedType('packed_0', bits=32, packed_tag=PackedTag(0,32,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

    @property
    def child_of(self):
        return 'Trailer'

    @property
    def fields(self):
        return [field for field in self.subfields] + [self.__dict__[key] for key,_ in asdict(self).items() if is_field_type(self.__dict__[key])]

    @property
    def is_user_defined(self):
        return len(self.subfields) > 0

    @property
    def enums(self):
        return [f for f in self.subfields if isinstance(f, EnumType)]
