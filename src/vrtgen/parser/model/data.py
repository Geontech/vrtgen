from dataclasses import dataclass, field

from vrtgen.parser import value
from vrtgen.parser.model.base import *
from vrtgen.parser.model.types import *

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
    # Enables
    calibrated_time_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('calibrated_time_enable', is_enable=True, packed_tag=PackedTag(31,1,0,0)))
    valid_data_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('valid_data_enable', is_enable=True, packed_tag=PackedTag(30,1,0,0)))
    reference_lock_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('reference_lock_enable', is_enable=True, packed_tag=PackedTag(29,1,0,0)))
    agc_mgc_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('agc_mgc_enable', is_enable=True, packed_tag=PackedTag(28,1,0,0)))
    detected_signal_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('detected_signal_enable', is_enable=True, packed_tag=PackedTag(27,1,0,0)))
    spectral_inversion_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('spectral_inversion_enable', is_enable=True, packed_tag=PackedTag(26,1,0,0)))
    over_range_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('over_range_enable', is_enable=True, packed_tag=PackedTag(25,1,0,0)))
    sample_loss_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('sample_loss_enable', is_enable=True, packed_tag=PackedTag(24,1,0,0)))
    # TODO sample_frame_enable
    # TODO user_defined_enable
    # Indicators
    calibrated_time : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('calibrated_time', packed_tag=PackedTag(19,1,0,0)))
    valid_data : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('valid_data', packed_tag=PackedTag(18,1,0,0)))
    reference_lock : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('reference_lock', packed_tag=PackedTag(17,1,0,0)))
    agc_mgc : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('agc_mgc', packed_tag=PackedTag(16,1,0,0)))
    detected_signal : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('detected_signal', packed_tag=PackedTag(15,1,0,0)))
    spectral_inversion : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('spectral_inversion', packed_tag=PackedTag(14,1,0,0)))
    over_range : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('over_range', packed_tag=PackedTag(13,1,0,0)))
    sample_loss : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('sample_loss', packed_tag=PackedTag(12,1,0,0)))
    # TODO sample_frame
    # TODO user_defined
    # Associated Context Packet Count
    associated_context_packets_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('associated_context_packet_count_enable', is_enable=True, packed_tag=PackedTag(7,1,0,0)))
    associated_context_packets : IntegerType = field(default_factory=lambda: IntegerType('associated_context_packet_count', bits=7, packed_tag=PackedTag(6,7,0,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

    def _validate(self, mapping):
        for field in mapping:
            error = False
            error_msg = 'invalid trailer field provided: ' + field  
            if not field in [f.name for f in self.fields]:
                error = True
            else:
                for cls_field in self.fields:
                    if cls_field.name == field and isinstance(cls_field, EnableIndicatorType) and cls_field.is_enable:
                            error = True
                            break
            if error:
                raise ValueError(error_msg)

    def _parse_mapping(self, mapping):
        try:
            for key,val in mapping.items():
                mode = value.parse_enable(val)
                self.__dict__[key].enabled = True if (mode == Mode.REQUIRED or mode == Mode.OPTIONAL) else False
                self.__dict__[key].required = True if (mode == Mode.REQUIRED) else False
        except:
            raise

    def validate_and_parse_mapping(self, **mapping):
        self._validate(mapping)
        self._parse_mapping(mapping)

@dataclass
class DataPacket(Packet):
    header : DataHeader = field(default_factory=lambda: DataHeader(enabled=True, required=True))
    trailer : Trailer = field(default_factory=Trailer)

    def validate_and_parse_mapping(self, **mapping):
        self._validate(mapping)
        self._parse_mapping(mapping)

    def _update_header(self):
        if self.stream_id.enabled:
            self.header.packet_type.value = PacketType.SIGNAL_DATA_STREAM_ID
        if self.trailer.enabled:
            self.header.trailer_included.enabled = True
            self.header.trailer_included.value = True
        super()._update_header()