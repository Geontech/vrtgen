from dataclasses import dataclass

from vrtgen.parser.model.types import *
from vrtgen.parser.model.base import CIF
from vrtgen.parser.value import parse_enable

@dataclass
class Gain(PackedStruct):
    """
    Gain/Attenuation [9.5.3].
    """
    name : str = 'gain'
    stage_2 : FixedPoint16r7 = FixedPoint16r7('stage_2', packed_tag=PackedTag(31,16,0))
    stage_1 : FixedPoint16r7 = FixedPoint16r7('stage_1', required=True, enabled=True, packed_tag=PackedTag(15,16,0))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

@dataclass
class DeviceIdentifier(PackedStruct):
    """
    Device Identifier [9.10.1].
    """
    name : str = 'device_id'
    reserved_0 : PackedType = PackedType('reserved_0', bits=8, packed_tag=PackedTag(31,8,0))
    manufacturer_oui : OUI = OUI('manufacturer_oui', packed_tag=PackedTag(23,24,0))
    device_code : IntegerType = IntegerType('device_code', bits=16, packed_tag=PackedTag(15,16,1,1))
    packed_1 : PackedType = PackedType('packed_1', bits=32, packed_tag=PackedTag(0,32,1))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 64

@dataclass
class StateEventIndicators(PackedStruct):
    """
    State and Event Indicator Field [9.10.8].
    """
    name : str = 'state_event_indicators'
    # Enables
    calibrated_time_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('calibrated_time_enable', is_enable=True, packed_tag=PackedTag(31,1,0,0)))
    valid_data_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('valid_data_enable', is_enable=True, packed_tag=PackedTag(30,1,0,0)))
    reference_lock_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('reference_lock_enable', is_enable=True, packed_tag=PackedTag(29,1,0,0)))
    agc_mgc_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('agc_mgc_enable', is_enable=True, packed_tag=PackedTag(28,1,0,0)))
    detected_signal_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('detected_signal_enable', is_enable=True, packed_tag=PackedTag(27,1,0,0)))
    spectral_inversion_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('spectral_inversion_enable', is_enable=True, packed_tag=PackedTag(26,1,0,0)))
    over_range_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('over_range_enable', is_enable=True, packed_tag=PackedTag(25,1,0,0)))
    sample_loss_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('sample_loss_enable', is_enable=True, packed_tag=PackedTag(24,1,0,0)))
    # Indicators
    calibrated_time : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('calibrated_time', packed_tag=PackedTag(19,1,0,0)))
    valid_data : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('valid_data', packed_tag=PackedTag(18,1,0,0)))
    reference_lock : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('reference_lock', packed_tag=PackedTag(17,1,0,0)))
    agc_mgc : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('agc_mgc', packed_tag=PackedTag(16,1,0,0)))
    detected_signal : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('detected_signal', packed_tag=PackedTag(15,1,0,0)))
    spectral_inversion : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('spectral_inversion', packed_tag=PackedTag(14,1,0,0)))
    over_range : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('over_range', packed_tag=PackedTag(13,1,0,0)))
    sample_loss : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('sample_loss', packed_tag=PackedTag(12,1,0,0)))
    # TODO user_defined
    packed_0 : PackedType = field(default_factory=lambda: PackedType('packed_0', bits=32, packed_tag=PackedTag(0,32,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

@dataclass
class PayloadFormat(PackedStruct):
    """
    Data Packet Payload Format Field [9.13.3]
    """
    name : str = 'payload_format'
    packing_method : EnumType = EnumType('packing_method', type_=PackingMethod, packed_tag=PackedTag(31,1,0,0))
    real_complex_type : EnumType = EnumType('real_complex_type', type_=DataSampleType, packed_tag=PackedTag(30,2,0,0))
    data_item_format : EnumType = EnumType('data_item_format', type_=DataItemFormat, packed_tag=PackedTag(28,5,0,0))
    repeat_indicator : BooleanType = BooleanType('repeat_indicator', packed_tag=PackedTag(23,1,0,0))
    event_tag_size : IntegerType = IntegerType('event_tag_size', bits=3, packed_tag=PackedTag(22,3,0,0))
    channel_tag_size : IntegerType = IntegerType('channel_tag_size', bits=4, packed_tag=PackedTag(19,4,0,0))
    data_item_fraction_size : IntegerType = IntegerType('data_item_fraction_size', bits=4, packed_tag=PackedTag(15,4,0,0))
    item_packing_field_size : NonZeroIntegerType = NonZeroIntegerType('item_packing_field_size', bits=6, packed_tag=PackedTag(11,6,0,0))
    data_item_size : NonZeroIntegerType = NonZeroIntegerType('data_item_size', bits=6, packed_tag=PackedTag(5,6,0,0))
    packed_0 : PackedType = PackedType('packed_0', bits=32, packed_tag=PackedTag(0,32,0))
    repeat_count : NonZeroIntegerType = NonZeroIntegerType('repeat_count', bits=16, packed_tag=PackedTag(31,16,1))
    vector_size : NonZeroIntegerType = NonZeroIntegerType('vector_size', bits=16, packed_tag=PackedTag(15,16,1))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

@dataclass
class Geolocation(PackedStruct):
    """
    Formatted GPS Geolocation [9.4.5) and INS Geolocation (9.4.6].
    """
    name : str = 'geolocation'
    tsi : EnumType = EnumType('tsi', type_=TSI, packed_tag=PackedTag(27,2,0,0))
    tsf : EnumType = EnumType('tsf', type_=TSF, packed_tag=PackedTag(25,2,0,0))
    packed_0 : PackedType = PackedType('packed_0', bits=8, packed_tag=PackedTag(23,8,0))
    manufacturer_oui : OUI = OUI('manufacturer_oui', packed_tag=PackedTag(23,24,0))
    # Integer timestamp shall be 0xFFFFFFFF if TSI is 0 (rule 9.4.5-6)
    integer_timestamp : IntegerTimestamp = IntegerTimestamp(value=0xFFFFFFFF, packed_tag=PackedTag(31,32,1))
    # Fractional timestamp shall be 0xFFFFFFFFFFFFFFFF if TSF is 0 (rule 9.4.5-6)
    fractional_timestamp : FractionalTimestamp = FractionalTimestamp(value=0xFFFFFFFFFFFFFFFF, packed_tag=PackedTag(31,64,2))
    # Default values for these subfields shall be 0x7FFFFFFF when unspecified (rule 9.4.5-6)
    latitude : GeolocationAngle = GeolocationAngle('latitude', value=0x7FFFFFFF, packed_tag=PackedTag(31,32,4))
    longitude : GeolocationAngle = GeolocationAngle('longitude', value=0x7FFFFFFF, packed_tag=PackedTag(31,32,5))
    altitude : FixedPointType = FixedPointType('altitude', bits=32, signed=True, radix=5, resolution=3, value=0x7FFFFFFF, packed_tag=PackedTag(31,32,6))
    speed_over_ground : FixedPointType = FixedPointType('speed_over_ground', bits=32, signed=True, radix=16, resolution=6, value=0x7FFFFFFF, packed_tag=PackedTag(31,32,7))
    heading_angle : GeolocationAngle = GeolocationAngle('heading_angle', value=0x7FFFFFFF, packed_tag=PackedTag(31,32,8))
    track_angle : GeolocationAngle = GeolocationAngle('track_angle', value=0x7FFFFFFF, packed_tag=PackedTag(31,32,9))
    magnetic_variation : GeolocationAngle = GeolocationAngle('magnetic_variation', value=0x7FFFFFFF, packed_tag=PackedTag(31,32,10))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 11 * 32 # 11 words

@dataclass
class Ephemeris(PackedStruct):
    """
    ECEF Ephemeris [9.4.3] and Relative Ephemeris [9.4.9].
    """
    name : str = 'ephemeris'
    tsi : EnumType = EnumType('tsi', type_=TSI, packed_tag=PackedTag(27,2,0,0))
    tsf : EnumType = EnumType('tsf', type_=TSF, packed_tag=PackedTag(25,2,0,0))
    packed_0 : PackedType = PackedType('packed_0', bits=8, packed_tag=PackedTag(23,8,0))
    manufacturer_oui : OUI = OUI('manufacturer_oui', packed_tag=PackedTag(23,24,0))
    # Integer timestamp should be 0xFFFFFFFF if TSI is 0
    integer_timestamp : IntegerTimestamp = IntegerTimestamp(value=0xFFFFFFFF, packed_tag=PackedTag(31,32,1))
    # Fractional timestamp should be 0xFFFFFFFFFFFFFFFF if TSF is 0
    fractional_timestamp : FractionalTimestamp = FractionalTimestamp(value=0xFFFFFFFFFFFFFFFF, packed_tag=PackedTag(31,64,2))
    # Default values for these subfields shall be 0x7FFFFFFF when unspecified (rule 9.4.3-8)
    position_x : CartesianCoordinate = CartesianCoordinate('position_x', value=0x7FFFFFFF, packed_tag=PackedTag(31,32,4))
    position_y : CartesianCoordinate = CartesianCoordinate('position_y', value=0x7FFFFFFF, packed_tag=PackedTag(31,32,5))
    position_z : CartesianCoordinate = CartesianCoordinate('position_z', value=0x7FFFFFFF, packed_tag=PackedTag(31,32,6))
    attitude_alpha : GeolocationAngle = GeolocationAngle('attitude_alpha', value=0x7FFFFFFF, packed_tag=PackedTag(31,32,7))
    attitude_beta : GeolocationAngle = GeolocationAngle('attitude_beta', value=0x7FFFFFFF, packed_tag=PackedTag(31,32,8))
    attitude_phi : GeolocationAngle = GeolocationAngle('attitude_phi', value=0x7FFFFFFF, packed_tag=PackedTag(31,32,9))
    velocity_dx : VelocityCoordinate = VelocityCoordinate('velocity_dx', value=0x7FFFFFFF, packed_tag=PackedTag(31,32,10))
    velocity_dy : VelocityCoordinate = VelocityCoordinate('velocity_dy', value=0x7FFFFFFF, packed_tag=PackedTag(31,32,11))
    velocity_dz : VelocityCoordinate = VelocityCoordinate('velocity_dz', value=0x7FFFFFFF, packed_tag=PackedTag(31,32,12))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 13 * 32 # 13 words

@dataclass
class GPS_ASCII(PackedStruct):
    """
    GPS ASCII Field [9.4.7]
    """
    name : str = 'gps_ascii'
    manufacturer_oui : OUI = OUI('manufacturer_oui', packed_tag=PackedTag(23,24,0))
    num_words : IntegerType = IntegerType('num_words', bits=32, packed_tag=PackedTag(31,32,1))
    sentence : ListType = ListType('sentence', type_=IntegerType(bits=8), packed_tag=PackedTag(31,32,2))

    def __post_init__(self):
        super().__post_init__()
        self._dynamic_size = True

@dataclass
class ContextAssociationLists(PackedStruct):
    """
    Context Association Lists [9.13.2]
    """
    name : str = 'context_association_lists'
    source_list_size : IntegerType = field(default_factory=lambda: IntegerType('source_list_size', bits=9, enabled=True, required=True, packed_tag=PackedTag(24,9,0,0)))
    system_list_size : IntegerType = field(default_factory=lambda: IntegerType('system_list_size', bits=9, enabled=True, required=True, packed_tag=PackedTag(8,9,0,0)))
    packed_0 : PackedType = PackedType('packed_0', bits=32, packed_tag=PackedTag(0,32,0))
    vector_component_list_size : IntegerType = field(default_factory=lambda: IntegerType('vector_component_list_size', bits=16, enabled=True, required=True, packed_tag=PackedTag(31,16,1,1)))
    async_channel_tag_list_enable : BooleanType = field(default_factory=lambda: BooleanType('async_channel_tag_list_enable', enabled=True, required=True, packed_tag=PackedTag(15,1,1,1)))
    async_channel_list_size : IntegerType = field(default_factory=lambda: IntegerType('async_channel_list_size', bits=15, enabled=True, required=True, packed_tag=PackedTag(14,15,1,1)))
    packed_1 : PackedType = PackedType('packed_1', bits=32, packed_tag=PackedTag(0,32,1))
    source_list : ListType = field(default_factory=lambda: ListType('source_list', type_=Identifier32(), enabled=True, required=True, packed_tag=PackedTag(31,0,2)))
    system_list : ListType = field(default_factory=lambda: ListType('system_list', type_=Identifier32(), enabled=True, required=True, packed_tag=PackedTag(31,0,3)))
    vector_component_list : ListType = field(default_factory=lambda: ListType('vector_component_list', type_=Identifier32(), enabled=True, required=True, packed_tag=PackedTag(31,0,4)))
    async_channel_list : ListType = field(default_factory=lambda: ListType('async_channel_list', type_=Identifier32(), enabled=True, required=True, packed_tag=PackedTag(31,0,5)))
    async_channel_tag_list : ListType = field(default_factory=lambda: ListType('async_channel_tag_list', type_=Identifier32(), packed_tag=PackedTag(31,0,6)))

    def __post_init__(self):
        super().__post_init__()
        self._dynamic_size = True
        self.source_list.linked_size = self.source_list_size
        self.system_list.linked_size = self.system_list_size
        self.vector_component_list.linked_size = self.vector_component_list_size
        self.async_channel_list.linked_size = self.async_channel_list_size
        self.async_channel_tag_list.linked_size = self.async_channel_list_size

@dataclass
class CIF0(CIF):
    """
    CIF0 Enables
    """
    name : str = 'cif_0'
    change_indicator : CIFEnableType = field(default_factory=lambda: CIFEnableType('change_indicator', type_=BooleanType('change_indicator'), enabled=True, required=True, indicator_only=True, packed_tag=PackedTag(31,1,0,0)))
    reference_point_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('reference_point_id', type_=Unsigned32(), packed_tag=PackedTag(30,1,0,0)))
    bandwidth : CIFEnableType = field(default_factory=lambda: CIFEnableType('bandwidth', type_=FixedPoint64r20(), packed_tag=PackedTag(29,1,0,0)))
    if_ref_frequency : CIFEnableType = field(default_factory=lambda: CIFEnableType('if_ref_frequency', type_=FixedPoint64r20(), packed_tag=PackedTag(28,1,0,0)))
    rf_ref_frequency : CIFEnableType = field(default_factory=lambda: CIFEnableType('rf_ref_frequency', type_=FixedPoint64r20(), packed_tag=PackedTag(27,1,0,0)))
    rf_ref_frequency_offset : CIFEnableType = field(default_factory=lambda: CIFEnableType('rf_ref_frequency_offset', type_=FixedPoint64r20(), packed_tag=PackedTag(26,1,0,0)))
    if_band_offset : CIFEnableType = field(default_factory=lambda: CIFEnableType('if_band_offset', type_=FixedPoint64r20(), packed_tag=PackedTag(25,1,0,0)))
    reference_level : CIFEnableType = field(default_factory=lambda: CIFEnableType('reference_level', type_=FixedPoint16r7(reserved_bits=16), packed_tag=PackedTag(24,1,0,0)))
    gain : CIFEnableType = field(default_factory=lambda: CIFEnableType('gain', type_=Gain(), packed_tag=PackedTag(23,1,0,0)))
    over_range_count : CIFEnableType = field(default_factory=lambda: CIFEnableType('over_range_count', type_=Signed32(), packed_tag=PackedTag(22,1,0,0)))
    sample_rate : CIFEnableType = field(default_factory=lambda: CIFEnableType('sample_rate', type_=FixedPoint64r20(), packed_tag=PackedTag(21,1,0,0)))
    timestamp_adjustment : CIFEnableType = field(default_factory=lambda: CIFEnableType('timestamp_adjustment', type_=Signed64(), packed_tag=PackedTag(20,1,0,0)))
    timestamp_calibration_time : CIFEnableType = field(default_factory=lambda: CIFEnableType('timestamp_calibration_time', type_=Unsigned32(), packed_tag=PackedTag(19,1,0,0)))
    temperature : CIFEnableType = field(default_factory=lambda: CIFEnableType('temperature', type_=FixedPoint16r6(reserved_bits=16), packed_tag=PackedTag(18,1,0,0)))
    device_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('device_id', type_=DeviceIdentifier(), packed_tag=PackedTag(17,1,0,0)))
    state_event_indicators : CIFEnableType = field(default_factory=lambda: CIFEnableType('state_event_indicators', type_=StateEventIndicators(), packed_tag=PackedTag(16,1,0,0)))
    signal_data_format : CIFEnableType = field(default_factory=lambda: CIFEnableType('signal_data_format', type_=PayloadFormat(), packed_tag=PackedTag(15,1,0,0)))
    formatted_gps : CIFEnableType = field(default_factory=lambda: CIFEnableType('formatted_gps', type_=Geolocation(), packed_tag=PackedTag(14,1,0,0)))
    formatted_ins : CIFEnableType = field(default_factory=lambda: CIFEnableType('formatted_ins', type_=Geolocation(), packed_tag=PackedTag(13,1,0,0)))
    ecef_ephemeris : CIFEnableType = field(default_factory=lambda: CIFEnableType('ecef_ephemeris', type_=Ephemeris(), packed_tag=PackedTag(12,1,0,0)))
    relative_ephemeris : CIFEnableType = field(default_factory=lambda: CIFEnableType('relative_ephemeris', type_=Ephemeris(), packed_tag=PackedTag(11,1,0,0)))
    ephemeris_ref_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('ephemeris_ref_id', type_=Unsigned32(), packed_tag=PackedTag(10,1,0,0)))
    # not currently supported
    gps_ascii : CIFEnableType = field(default_factory=lambda: CIFEnableType('gps_ascii', type_=GPS_ASCII(), packed_tag=PackedTag(9,1,0,0)))
    context_association_lists : CIFEnableType = field(default_factory=lambda: CIFEnableType('context_association_lists', type_=ContextAssociationLists(), packed_tag=PackedTag(8,1,0,0)))
    # cif7_enable : CIFEnableType = field(default_factory=lambda: CIFEnableType('cif7_enable', indicator_only=True, packed_tag=PackedTag(7,1,0,0)))
    # cif3_enable : CIFEnableType = field(default_factory=lambda: CIFEnableType('cif3_enable', indicator_only=True, packed_tag=PackedTag(3,1,0,0)))
    # cif2_enable : CIFEnableType = field(default_factory=lambda: CIFEnableType('cif2_enable', indicator_only=True, packed_tag=PackedTag(2,1,0,0)))
    # cif1_enable : CIFEnableType = field(default_factory=lambda: CIFEnableType('cif1_enable', indicator_only=True, packed_tag=PackedTag(1,1,0,0)))

    @property
    def all_optional_fields(self):
        # disable this from the template because CIF0 always required if part of packet structure
        return False

    def _parse_mapping(self, mapping):
        for key,val in mapping.items():
            mode = parse_enable(val)
            if key == 'change_indicator':
                if mode != Mode.REQUIRED:
                    print('change_indicator is a required field, ignoring: ', val)
            else:
                self.__dict__[key].enabled = True if (mode == Mode.REQUIRED or mode == Mode.OPTIONAL) else False
                self.__dict__[key].required = True if (mode == Mode.REQUIRED) else False