from dataclasses import dataclass, field

from vrtgen.parser.model.types import *
from vrtgen.parser.model.base import Header, Packet, CIF
from vrtgen.parser.model.prologue import ClassIdentifier, Timestamp
from vrtgen.parser.model.cif0 import CIF0
from vrtgen.parser.model.cif1 import CIF1
from vrtgen.parser.model.cif2 import CIF2
from vrtgen.parser.model.cif7 import CIF7
from vrtgen.parser.value import parse_enable, parse_identifier_format, parse_action_mode, parse_timing_control

@dataclass
class CommandHeader(Header):
    """
    VRT Packet Header with Command Packet-Specific Indicator Bits (5.1.1.1)
    """
    acknowledge_packet : BooleanType = field(default_factory=lambda: BooleanType('acknowledge_packet', enabled=True, required=True, packed_tag=PackedTag(26,1,0,0)))
    cancellation_packet : BooleanType = field(default_factory=lambda: BooleanType('cancellation_packet', enabled=True, required=True, packed_tag=PackedTag(24,1,0,0)))
    
    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.packet_type.value = PacketType.COMMAND

@dataclass
class ExtensionCommandHeader(CommandHeader):
    """
    VRT Packet Header with Extension Command Packet-Specific Indicator Bits (5.1.1.1)
    """
    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.packet_type.value = PacketType.EXTENSION_COMMAND

@dataclass
class ControlAcknowledgeMode(PackedStruct):
    """
    Control/Acknowledge Mode Field (8.2.1).
    """
    name : str = 'cam'
    controllee_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('controllee_enable', is_enable=True, packed_tag=PackedTag(31,1,0,0)))
    controllee_format : EnumType = field(default_factory=lambda: EnumType('controllee_format', type_=IdentifierFormat, packed_tag=PackedTag(30,1,0,0)))
    controller_enable : EnableIndicatorType = field(default_factory=lambda: EnableIndicatorType('controller_enable', is_enable=True, packed_tag=PackedTag(29,1,0,0)))
    controller_format : EnumType = field(default_factory=lambda: EnumType('controller_format', type_=IdentifierFormat, packed_tag=PackedTag(28,1,0,0)))
    permit_partial : BooleanType = field(default_factory=lambda: BooleanType('permit_partial', packed_tag=PackedTag(27,1,0,0)))
    permit_warnings : BooleanType = field(default_factory=lambda: BooleanType('permit_warnings', packed_tag=PackedTag(26,1,0,0)))
    permit_errors : BooleanType = field(default_factory=lambda: BooleanType('permit_errors', packed_tag=PackedTag(25,1,0,0)))
    # action_mode : EnumType = field(default_factory=lambda: EnumType('action_mode', type_=ActionMode, packed_tag=PackedTag(24,2,0,0)))
    # nack_only : BooleanType = field(default_factory=lambda: BooleanType('nack_only', packed_tag=PackedTag(22,1,0,0)))
    # timing_control : EnumType = field(default_factory=lambda: EnumType('timing_control', type_=TimestampControlMode, packed_tag=PackedTag(14,3,0,0)))
    packed_0 : PackedType = PackedType('packed_0', bits=32, packed_tag=PackedTag(0,32,0))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

    def _validate(self, mapping):
        for field in mapping:
            error_msg = 'invalid cam field provided: ' + field  
            if not field in [f.name for f in self.fields]:
                raise ValueError(error_msg)
            else:
                for cls_field in self.fields:
                    if cls_field.name == field and isinstance(cls_field, EnableIndicatorType) and cls_field.is_enable:
                        raise ValueError(error_msg)                

    def _parse_mapping(self, mapping):
        for key,val in mapping.items():
            if isinstance(self.__dict__[key], BooleanType):
                if isinstance(val, str):
                    mode = parse_enable(val)
                    self.__dict__[key].enabled = True if (mode == Mode.REQUIRED or mode == Mode.OPTIONAL) else False
                    self.__dict__[key].required = True if (mode == Mode.REQUIRED) else False
                    self.__dict__[key].value = (mode == Mode.REQUIRED)
                elif isinstance(val, bool):
                    self.__dict__[key].enabled = val
                    self.__dict__[key].required = val
                    self.__dict__[key].value = val
                else:
                    raise TypeError('invalid option {} for {}'.format(val,key))
            elif key == 'controllee_format' or key == 'controller_format':
                self.__dict__[key].value = parse_identifier_format(val)
                self.__dict__[key].enabled = True
                self.__dict__[key].required = True
            # elif key == 'action_mode':
            #     if val == 'required' or val == 'optional':
            #         mode = parse_enable(val)
            #         self.__dict__[key].enabled = True if (mode == Mode.REQUIRED or mode == Mode.OPTIONAL) else False
            #         self.__dict__[key].required = True if (mode == Mode.REQUIRED) else False
            #         if mode == Mode.REQUIRED:
            #             self.__dict__[key].value = True
            #     else:
            #         self.__dict__[key].value = parse_action_mode(val)
            #         self.__dict__[key].enabled = True
            #         self.__dict__[key].required = True
            # elif key == 'timing_control':
            #     self.__dict__[key].value = parse_timing_control(val)
            #     self.__dict__[key].enabled = True
            #     self.__dict__[key].required = True

    def validate_and_parse_mapping(self, **mapping):
        self._validate(mapping)
        self._parse_mapping(mapping)

@dataclass
class ControlCAM(ControlAcknowledgeMode):
    """
    Control Packet Control/Acknowledge Mode Field (8.3.1).
    """
    req_v : BooleanType = field(default_factory=lambda: BooleanType('req_v', packed_tag=PackedTag(20,1,0,0)))
    req_x : BooleanType = field(default_factory=lambda: BooleanType('req_x', packed_tag=PackedTag(19,1,0,0)))
    req_s : BooleanType = field(default_factory=lambda: BooleanType('req_s', packed_tag=PackedTag(18,1,0,0)))
    req_w : BooleanType = field(default_factory=lambda: BooleanType('req_w', packed_tag=PackedTag(17,1,0,0)))
    req_er : BooleanType = field(default_factory=lambda: BooleanType('req_er', packed_tag=PackedTag(16,1,0,0)))

    @property
    def is_control(self):
        return True

@dataclass
class AcknowledgeCAM(ControlAcknowledgeMode):
    """
    Acknowledge Packet Control/Acknowledge Mode Field (8.4.1).
    """
    ack_v : BooleanType = field(default_factory=lambda: BooleanType('ack_v', packed_tag=PackedTag(20,1,0,0)))
    ack_x : BooleanType = field(default_factory=lambda: BooleanType('ack_x', packed_tag=PackedTag(19,1,0,0)))
    ack_s : BooleanType = field(default_factory=lambda: BooleanType('ack_s', packed_tag=PackedTag(18,1,0,0)))
    ack_w : BooleanType = field(default_factory=lambda: BooleanType('ack_w', packed_tag=PackedTag(17,1,0,0)))
    ack_er : BooleanType = field(default_factory=lambda: BooleanType('ack_er', packed_tag=PackedTag(16,1,0,0)))
    partial_action : BooleanType = field(default_factory=lambda: BooleanType('partial_action', packed_tag=PackedTag(11,1,0,0)))
    scheduled_or_executed : BooleanType = field(default_factory=lambda: BooleanType('scheduled_or_executed', packed_tag=PackedTag(10,1,0,0)))

    @property
    def is_ack(self):
        return True

@dataclass
class ControlIdentifier(Field):
    format : IdentifierFormat = field(default_factory=IdentifierFormat)

    def __post_init__(self):
        self.type_ = type(self).__name__
        self.bits = 32

    @property
    def is_integer_type(self):
        return self.format == IdentifierFormat.WORD

    def _parse_mapping(self, mapping):
        try:
            self.format = parse_identifier_format(mapping)
            if self.format == IdentifierFormat.UUID:
                self.bits = 128

        except:
            raise ValueError('invalid identifier format: ' + mapping)
        self.enabled = True
        self.required = True

@dataclass
class CommandPacket(Packet):
    """
    Command base class
    """
    header : CommandHeader = field(default_factory=lambda: CommandHeader(enabled=True, required=True))
    message_id : IntegerType = field(default_factory=lambda: IntegerType('message_id', bits=32, enabled=True, required=True))
    controllee_id : ControlIdentifier = field(default_factory=lambda: ControlIdentifier('controllee_id'))
    controller_id : ControlIdentifier = field(default_factory=lambda: ControlIdentifier('controller_id'))
    cif_0 : CIF0 = field(default_factory=CIF0)
    cif_1 : CIF1 = field(default_factory=CIF1)
    cif_2 : CIF1 = field(default_factory=CIF2)
    cif_7 : CIF7 = field(default_factory=CIF7)

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.stream_id.enabled = True
        self.stream_id.required = True

    def _parse_mapping(self, mapping):
        try:
            for key,val in mapping.items():
                if key == 'controllee_id':
                    self.controllee_id._parse_mapping(val)
                elif key == 'controller_id':
                    self.controller_id._parse_mapping(val)
                else:
                    self.__dict__[key] = val
        except:
            raise

@dataclass
class ControlPacket(CommandPacket):
    """
    Control Command Packet subtype
    """
    cam : ControlCAM = field(default_factory=lambda: ControlCAM(enabled=True, required=True))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.cif_0.enabled = True
        self.cif_0.required = True

    def _update_cam(self):
        if self.controllee_id.enabled:
            self.cam.controllee_enable.enabled = True
            self.cam.controllee_enable.required = True
            self.cam.controllee_enable.value = True
            self.cam.controllee_format.enabled = True
            self.cam.controllee_format.required = True
            self.cam.controllee_format.value = self.controllee_id.format
        if self.controller_id.enabled:
            self.cam.controller_enable.enabled = True
            self.cam.controller_enable.required = True
            self.cam.controller_enable.value = True
            self.cam.controller_format.enabled = True
            self.cam.controller_format.required = True
            self.cam.controller_format.value = self.controller_id.format

    @property
    def is_control(self):
        return True

    @property
    def is_control_v(self):
        return self.cam.req_v.enabled

    @property
    def is_control_x(self):
        return self.cam.req_x.enabled

    @property
    def is_control_s(self):
        return self.cam.req_s.enabled

    @property
    def warnings_enabled(self):
        return self.cam.req_w.value

    @property
    def errors_enabled(self):
        return self.cam.req_er.value

@dataclass
class ExtensionControlPacket(ControlPacket):
    """
    Extension Control Command Packet subtype
    """
    def __post_init__(self):
        super().__post_init__()
        self.header.packet_type.value = PacketType.EXTENSION_COMMAND
        self.cif_0.enabled = False
        self.cif_0.required = False

@dataclass
class WarningErrorFields(PackedStruct):
    """
    Warning and Error Response Fields [8.4.1.2.1]
    """
    name : str = 'warning_error_fields'
    field_not_executed : BooleanType = field(default_factory=lambda: BooleanType('field_not_executed', packed_tag=PackedTag(31,1,0,0)))
    device_failure : BooleanType = field(default_factory=lambda: BooleanType('device_failure', packed_tag=PackedTag(30,1,0,0)))
    erroneuous_field : BooleanType = field(default_factory=lambda: BooleanType('erroneuous_field', packed_tag=PackedTag(29,1,0,0)))
    parameter_out_of_range : BooleanType = field(default_factory=lambda: BooleanType('parameter_out_of_range', packed_tag=PackedTag(28,1,0,0)))
    parameter_unsupported_precision : BooleanType = field(default_factory=lambda:BooleanType('parameter_unsupported_precision', packed_tag=PackedTag(27,1,0,0)))
    field_value_invalid : BooleanType = field(default_factory=lambda: BooleanType('field_value_invalid', packed_tag=PackedTag(26,1,0,0)))
    timestamp_problem : BooleanType = field(default_factory=lambda: BooleanType('timestamp_problem', packed_tag=PackedTag(25,1,0,0)))
    hazardous_power_levels : BooleanType = field(default_factory=lambda: BooleanType('hazardous_power_levels', packed_tag=PackedTag(24,1,0,0)))
    distortion : BooleanType = field(default_factory=lambda: BooleanType('distortion', packed_tag=PackedTag(23,1,0,0)))
    in_band_power_compliance : BooleanType = field(default_factory=lambda: BooleanType('in_band_power_compliance', packed_tag=PackedTag(22,1,0,0)))
    out_of_band_power_compliance : BooleanType = field(default_factory=lambda: BooleanType('out_of_band_power_compliance', packed_tag=PackedTag(21,1,0,0)))
    cosite_interference : BooleanType = field(default_factory=lambda: BooleanType('cosite_interference', packed_tag=PackedTag(20,1,0,0)))
    regional_interference : BooleanType = field(default_factory=lambda: BooleanType('regional_interference', packed_tag=PackedTag(19,1,0,0)))
    packed_0 : PackedType = PackedType('packed_0', bits=32, packed_tag=PackedTag(0,32,0))

    def __post_init__(self):
        super().__post_init__()
        self.bits = 32

@dataclass
class WEIF0(CIF):
    """
    Warning/Error Indicator 0 Enables
    """
    name : str = 'weif_0'
    reference_point_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('reference_point_id', type_=WarningErrorFields(), packed_tag=PackedTag(30,1,0,0)))
    bandwidth : CIFEnableType = field(default_factory=lambda: CIFEnableType('bandwidth', type_=WarningErrorFields(), packed_tag=PackedTag(29,1,0,0)))
    if_ref_frequency : CIFEnableType = field(default_factory=lambda: CIFEnableType('if_ref_frequency', type_=WarningErrorFields(), packed_tag=PackedTag(28,1,0,0)))
    rf_ref_frequency : CIFEnableType = field(default_factory=lambda: CIFEnableType('rf_ref_frequency', type_=WarningErrorFields(), packed_tag=PackedTag(27,1,0,0)))
    rf_frequency_offset : CIFEnableType = field(default_factory=lambda: CIFEnableType('rf_frequency_offset', type_=WarningErrorFields(), packed_tag=PackedTag(26,1,0,0)))
    if_band_offset : CIFEnableType = field(default_factory=lambda: CIFEnableType('if_band_offset', type_=WarningErrorFields(), packed_tag=PackedTag(25,1,0,0)))
    reference_level : CIFEnableType = field(default_factory=lambda: CIFEnableType('reference_level', type_=WarningErrorFields(), packed_tag=PackedTag(24,1,0,0)))
    gain : CIFEnableType = field(default_factory=lambda: CIFEnableType('gain', type_=WarningErrorFields(), packed_tag=PackedTag(23,1,0,0)))
    over_range_count : CIFEnableType = field(default_factory=lambda: CIFEnableType('over_range_count', type_=WarningErrorFields(), packed_tag=PackedTag(22,1,0,0)))
    sample_rate : CIFEnableType = field(default_factory=lambda: CIFEnableType('sample_rate', type_=WarningErrorFields(), packed_tag=PackedTag(21,1,0,0)))
    timestamp_adjustment : CIFEnableType = field(default_factory=lambda: CIFEnableType('timestamp_adjustment', type_=WarningErrorFields(), packed_tag=PackedTag(20,1,0,0)))
    timestamp_calibration_time : CIFEnableType = field(default_factory=lambda: CIFEnableType('timestamp_calibration_time', type_=WarningErrorFields(), packed_tag=PackedTag(19,1,0,0)))
    temperature : CIFEnableType = field(default_factory=lambda: CIFEnableType('temperature', type_=WarningErrorFields(), packed_tag=PackedTag(18,1,0,0)))
    device_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('device_id', type_=WarningErrorFields(), packed_tag=PackedTag(17,1,0,0)))
    state_event_indicators : CIFEnableType = field(default_factory=lambda: CIFEnableType('state_event_indicators', type_=WarningErrorFields(), packed_tag=PackedTag(16,1,0,0)))
    signal_data_format : CIFEnableType = field(default_factory=lambda: CIFEnableType('signal_data_format', type_=WarningErrorFields(), packed_tag=PackedTag(15,1,0,0)))
    formatted_gps : CIFEnableType = field(default_factory=lambda: CIFEnableType('formatted_gps', type_=WarningErrorFields(), packed_tag=PackedTag(14,1,0,0)))
    formatted_ins : CIFEnableType = field(default_factory=lambda: CIFEnableType('formatted_ins', type_=WarningErrorFields(), packed_tag=PackedTag(13,1,0,0)))
    ecef_ephemeris : CIFEnableType = field(default_factory=lambda: CIFEnableType('ecef_ephemeris', type_=WarningErrorFields(), packed_tag=PackedTag(12,1,0,0)))
    relative_ephemeris : CIFEnableType = field(default_factory=lambda: CIFEnableType('relative_ephemeris', type_=WarningErrorFields(), packed_tag=PackedTag(11,1,0,0)))
    ephermis_ref_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('ephermis_ref_id', type_=WarningErrorFields(), packed_tag=PackedTag(10,1,0,0)))
    gps_ascii : CIFEnableType = field(default_factory=lambda: CIFEnableType('gps_ascii', type_=WarningErrorFields(), packed_tag=PackedTag(9,1,0,0)))
    context_association_lists : CIFEnableType = field(default_factory=lambda: CIFEnableType('context_association_lists', type_=WarningErrorFields(), packed_tag=PackedTag(8,1,0,0)))
    packed_0 : PackedType = PackedType('packed_0', bits=32, packed_tag=PackedTag(0,32,0))

@dataclass
class WIF0(WEIF0):
    """
    Warning Indicator 0 Enables
    """
    wif_7_enable : CIFEnableType = field(default_factory=lambda: CIFEnableType('wif_7_enable', indicator_only=True, packed_tag=PackedTag(7,1,0,0)))
    wif_3_enable : CIFEnableType = field(default_factory=lambda: CIFEnableType('wif_3_enable', indicator_only=True, packed_tag=PackedTag(3,1,0,0)))
    wif_2_enable : CIFEnableType = field(default_factory=lambda: CIFEnableType('wif_2_enable', indicator_only=True, packed_tag=PackedTag(2,1,0,0)))
    wif_1_enable : CIFEnableType = field(default_factory=lambda: CIFEnableType('wif_1_enable', indicator_only=True, packed_tag=PackedTag(1,1,0,0)))

@dataclass
class EIF0(WEIF0):
    """
    Error Indicator 0 Enables
    """
    eif_7_enable : CIFEnableType = field(default_factory=lambda: CIFEnableType('eif_7_enable', indicator_only=True, packed_tag=PackedTag(7,1,0,0)))
    eif_3_enable : CIFEnableType = field(default_factory=lambda: CIFEnableType('eif_3_enable', indicator_only=True, packed_tag=PackedTag(3,1,0,0)))
    eif_2_enable : CIFEnableType = field(default_factory=lambda: CIFEnableType('eif_2_enable', indicator_only=True, packed_tag=PackedTag(2,1,0,0)))
    eif_1_enable : CIFEnableType = field(default_factory=lambda: CIFEnableType('eif_1_enable', indicator_only=True, packed_tag=PackedTag(1,1,0,0)))

@dataclass
class WEIF1(CIF):
    """
    Warning/Error Indicator 1 Enables
    """
    name : str = 'weif1'
    phase_offset : CIFEnableType = field(default_factory=lambda: CIFEnableType('phase_offset', type_=WarningErrorFields(), packed_tag=PackedTag(31,1,0,0)))
    polarization : CIFEnableType = field(default_factory=lambda: CIFEnableType('polarization', type_=WarningErrorFields(), packed_tag=PackedTag(30,1,0,0)))
    pointing_vector : CIFEnableType = field(default_factory=lambda: CIFEnableType('pointing_vector', type_=WarningErrorFields(), packed_tag=PackedTag(29,1,0,0)))
    pointing_vector_structure : CIFEnableType = field(default_factory=lambda: CIFEnableType('pointing_vector_structure', type_=WarningErrorFields(), packed_tag=PackedTag(28,1,0,0)))
    spatial_scan_type : CIFEnableType = field(default_factory=lambda: CIFEnableType('spatial_scan_type', type_=WarningErrorFields(), packed_tag=PackedTag(27,1,0,0)))
    spatial_reference_type : CIFEnableType = field(default_factory=lambda: CIFEnableType('spatial_reference_type', type_=WarningErrorFields(), packed_tag=PackedTag(26,1,0,0)))
    beam_width : CIFEnableType = field(default_factory=lambda: CIFEnableType('beam_width', type_=WarningErrorFields(), packed_tag=PackedTag(25,1,0,0)))
    range : CIFEnableType = field(default_factory=lambda: CIFEnableType('range', type_=WarningErrorFields(), packed_tag=PackedTag(24,1,0,0)))
    ebno_ber : CIFEnableType = field(default_factory=lambda: CIFEnableType('ebno_ber', type_=WarningErrorFields(), packed_tag=PackedTag(20,1,0,0)))
    threshold : CIFEnableType = field(default_factory=lambda: CIFEnableType('threshold', type_=WarningErrorFields(), packed_tag=PackedTag(19,1,0,0)))
    compression_point : CIFEnableType = field(default_factory=lambda: CIFEnableType('compression_point', type_=WarningErrorFields(), packed_tag=PackedTag(18,1,0,0)))
    intercept_points : CIFEnableType = field(default_factory=lambda: CIFEnableType('intercept_points', type_=WarningErrorFields(), packed_tag=PackedTag(17,1,0,0)))
    snr_noise_figure : CIFEnableType = field(default_factory=lambda: CIFEnableType('snr_noise_figure', type_=WarningErrorFields(), packed_tag=PackedTag(16,1,0,0)))
    aux_frequency : CIFEnableType = field(default_factory=lambda: CIFEnableType('aux_frequency', type_=WarningErrorFields(), packed_tag=PackedTag(15,1,0,0)))
    aux_gain : CIFEnableType = field(default_factory=lambda: CIFEnableType('aux_gain', type_=WarningErrorFields(), packed_tag=PackedTag(14,1,0,0)))
    aux_bandwidth : CIFEnableType = field(default_factory=lambda: CIFEnableType('aux_bandwidth', type_=WarningErrorFields(), packed_tag=PackedTag(13,1,0,0)))
    array_of_cifs : CIFEnableType = field(default_factory=lambda: CIFEnableType('array_of_cifs', type_=WarningErrorFields(), packed_tag=PackedTag(11,1,0,0)))
    spectrum : CIFEnableType = field(default_factory=lambda: CIFEnableType('spectrum', type_=WarningErrorFields(), packed_tag=PackedTag(10,1,0,0)))
    sector_step_scan : CIFEnableType = field(default_factory=lambda: CIFEnableType('sector_step_scan', type_=WarningErrorFields(), packed_tag=PackedTag(9,1,0,0)))
    index_list : CIFEnableType = field(default_factory=lambda: CIFEnableType('index_list', type_=WarningErrorFields(), packed_tag=PackedTag(7,1,0,0)))
    discrete_io_32 : CIFEnableType = field(default_factory=lambda: CIFEnableType('discrete_io_32', type_=WarningErrorFields(), packed_tag=PackedTag(6,1,0,0)))
    discrete_io_64 : CIFEnableType = field(default_factory=lambda: CIFEnableType('discrete_io_64', type_=WarningErrorFields(), packed_tag=PackedTag(5,1,0,0)))
    health_status : CIFEnableType = field(default_factory=lambda: CIFEnableType('health_status', type_=WarningErrorFields(), packed_tag=PackedTag(4,1,0,0)))
    v49_spec_compliance : CIFEnableType = field(default_factory=lambda: CIFEnableType('v49_spec_compliance', type_=WarningErrorFields(), packed_tag=PackedTag(3,1,0,0)))
    version_build_code : CIFEnableType = field(default_factory=lambda: CIFEnableType('version_build_code', type_=WarningErrorFields(), packed_tag=PackedTag(2,1,0,0)))
    buffer_size : CIFEnableType = field(default_factory=lambda: CIFEnableType('buffer_size', type_=WarningErrorFields(), packed_tag=PackedTag(1,1,0,0)))
    packed_0 : PackedType = PackedType('packed_0', bits=32, packed_tag=PackedTag(0,32,0))

@dataclass
class WEIF2(CIF):
    """
    Warning/Error Indicator 2 Enables
    """
    name : str = 'weif2'
    bind : CIFEnableType = field(default_factory=lambda: CIFEnableType('bind', type_=WarningErrorFields(), packed_tag=PackedTag(31,1,0,0)))
    cited_sid : CIFEnableType = field(default_factory=lambda: CIFEnableType('cited_sid', type_=WarningErrorFields(), packed_tag=PackedTag(30,1,0,0)))
    sibling_sid : CIFEnableType = field(default_factory=lambda: CIFEnableType('sibling_sid', type_=WarningErrorFields(), packed_tag=PackedTag(29,1,0,0)))
    parent_sid : CIFEnableType = field(default_factory=lambda: CIFEnableType('parent_sid', type_=WarningErrorFields(), packed_tag=PackedTag(28,1,0,0)))
    child_sid : CIFEnableType = field(default_factory=lambda: CIFEnableType('child_sid', type_=WarningErrorFields(), packed_tag=PackedTag(27,1,0,0)))
    cited_message_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('cited_message_id', type_=WarningErrorFields(), packed_tag=PackedTag(26,1,0,0)))
    controllee_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('controllee_id', type_=WarningErrorFields(), packed_tag=PackedTag(25,1,0,0)))
    controllee_uuid : CIFEnableType = field(default_factory=lambda: CIFEnableType('controllee_uuid', type_=WarningErrorFields(), packed_tag=PackedTag(24,1,0,0)))
    controller_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('controller_id', type_=WarningErrorFields(), packed_tag=PackedTag(23,1,0,0)))
    controller_uuid : CIFEnableType = field(default_factory=lambda: CIFEnableType('controller_uuid', type_=WarningErrorFields(), packed_tag=PackedTag(22,1,0,0)))
    information_source : CIFEnableType = field(default_factory=lambda: CIFEnableType('information_source', type_=WarningErrorFields(), packed_tag=PackedTag(21,1,0,0)))
    track_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('track_id', type_=WarningErrorFields(), packed_tag=PackedTag(20,1,0,0)))
    country_code : CIFEnableType = field(default_factory=lambda: CIFEnableType('country_code', type_=WarningErrorFields(), packed_tag=PackedTag(19,1,0,0)))
    operator_ : CIFEnableType = field(default_factory=lambda: CIFEnableType('operator_', type_=WarningErrorFields(), packed_tag=PackedTag(18,1,0,0)))
    platform_class : CIFEnableType = field(default_factory=lambda: CIFEnableType('platform_class', type_=WarningErrorFields(), packed_tag=PackedTag(17,1,0,0)))
    platform_instance : CIFEnableType = field(default_factory=lambda: CIFEnableType('platform_instance', type_=WarningErrorFields(), packed_tag=PackedTag(16,1,0,0)))
    platform_display : CIFEnableType = field(default_factory=lambda: CIFEnableType('platform_display', type_=WarningErrorFields(), packed_tag=PackedTag(15,1,0,0)))
    ems_device_class : CIFEnableType = field(default_factory=lambda: CIFEnableType('ems_device_class', type_=WarningErrorFields(), packed_tag=PackedTag(14,1,0,0)))
    ems_device_type : CIFEnableType = field(default_factory=lambda: CIFEnableType('ems_device_type', type_=WarningErrorFields(), packed_tag=PackedTag(13,1,0,0)))
    ems_device_instance : CIFEnableType = field(default_factory=lambda: CIFEnableType('ems_device_instance', type_=WarningErrorFields(), packed_tag=PackedTag(12,1,0,0)))
    modulation_class : CIFEnableType = field(default_factory=lambda: CIFEnableType('modulation_class', type_=WarningErrorFields(), packed_tag=PackedTag(11,1,0,0)))
    modulation_type : CIFEnableType = field(default_factory=lambda: CIFEnableType('modulation_type', type_=WarningErrorFields(), packed_tag=PackedTag(10,1,0,0)))
    function_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('function_id', type_=WarningErrorFields(), packed_tag=PackedTag(9,1,0,0)))
    mode_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('mode_id', type_=WarningErrorFields(), packed_tag=PackedTag(8,1,0,0)))
    event_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('event_id', type_=WarningErrorFields(), packed_tag=PackedTag(7,1,0,0)))
    function_priority_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('function_priority_id', type_=WarningErrorFields(), packed_tag=PackedTag(6,1,0,0)))
    communication_priority_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('communication_priority_id', type_=WarningErrorFields(), packed_tag=PackedTag(5,1,0,0)))
    rf_footprint : CIFEnableType = field(default_factory=lambda: CIFEnableType('rf_footprint', type_=WarningErrorFields(), packed_tag=PackedTag(4,1,0,0)))
    rf_footprint_range : CIFEnableType = field(default_factory=lambda: CIFEnableType('rf_footprint_range', type_=WarningErrorFields(), packed_tag=PackedTag(3,1,0,0)))
    packed_0 : PackedType = PackedType('packed_0', bits=32, packed_tag=PackedTag(0,32,0))

@dataclass
class AcknowledgePacket(CommandPacket):
    """
    Acknowledge Command Packet subtype
    """
    cam : AcknowledgeCAM = field(default_factory=lambda: AcknowledgeCAM(enabled=True, required=True))
    wif_0 : WIF0 = field(default_factory=lambda: WIF0('wif_0'))
    wif_1 : WEIF1 = field(default_factory=lambda: WEIF1('wif_1'))
    wif_2 : WEIF2 = field(default_factory=lambda: WEIF2('wif_2'))
    eif_0 : EIF0 = field(default_factory=lambda: EIF0('eif_0'))
    eif_1 : WEIF1 = field(default_factory=lambda: WEIF1('eif_1'))
    eif_2 : WEIF2 = field(default_factory=lambda: WEIF2('eif_2'))
    responds_to: str = field(default_factory=str)

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.header.acknowledge_packet.value = True

    def update_from_control(self, ctrl_packet):
        # Update Header
        self.header.class_id_enable = ctrl_packet.header.class_id_enable
        self.header.tsi = ctrl_packet.header.tsi
        self.header.tsf = ctrl_packet.header.tsf
        # Update Stream ID
        self.stream_id = ctrl_packet.stream_id
        # Class ID should be set in the YAML
        self.class_id = ctrl_packet.class_id
        # if ctrl_packet.class_id.enabled and not self.class_id.enabled:
        #     raise ValueError('class_id set in control packet but missing from ack YAML definition')
        # Update Timestamps
        self.timestamp = ctrl_packet.timestamp
        # Update CAM
        for field in ctrl_packet.cam.fields:
            ack_cam_field = None
            if field.name == 'req_v':
                ack_cam_field = self.cam.ack_v.name
            elif field.name == 'req_x':
                ack_cam_field = self.cam.ack_x.name
            elif field.name == 'req_s':
                ack_cam_field = self.cam.ack_s.name
            elif field.name == 'req_w':
                ack_cam_field = self.cam.ack_w.name
            elif field.name == 'req_er':
                ack_cam_field = self.cam.ack_er.name
            else:
                self.cam.__dict__[field.name] = field
            if ack_cam_field:
                self.cam.__dict__[ack_cam_field].enabled = field.enabled
                # self.cam.__dict__[ack_cam_field].required = field.required
                self.cam.__dict__[ack_cam_field].value = field.value
        # Update Control Identifiers
        self.controllee_id = ctrl_packet.controllee_id
        self.controller_id = ctrl_packet.controller_id
        # Update Warning/Error Fields
        if ctrl_packet.cif_0.enabled:
            if self.is_ack_s:
                self.cif_0 = ctrl_packet.cif_0
            if self.is_ack_v or self.is_ack_x:
                self.wif_0.enabled = True
                self.eif_0.enabled = True
                for field in ctrl_packet.cif_0.fields:
                    if field.name in [f.name for f in self.wif_0.fields] and field.enabled:
                        self.wif_0.__dict__[field.name].enabled = True
                        self.eif_0.__dict__[field.name].enabled = True
        if ctrl_packet.cif_1.enabled:
            if self.is_ack_s:
                self.cif_1 = ctrl_packet.cif_1
            if self.is_ack_v or self.is_ack_x:
                self.wif_1.enabled = True
                self.eif_1.enabled = True
                for field in ctrl_packet.cif_1.fields:
                    if field.name in [f.name for f in self.wif_1.fields] and field.enabled:
                        self.wif_1.__dict__[field.name].enabled = True
                        self.eif_1.__dict__[field.name].enabled = True
        if ctrl_packet.cif_2.enabled:
            if self.is_ack_s:
                self.cif_2 = ctrl_packet.cif_2
            if self.is_ack_v or self.is_ack_x:
                self.wif_2.enabled = True
                self.eif_2.enabled = True
                for field in ctrl_packet.cif_2.fields:
                    if field.name in [f.name for f in self.wif_2.fields] and field.enabled:
                        self.wif_2.__dict__[field.name].enabled = True
                        self.eif_2.__dict__[field.name].enabled = True

    def _validate(self, mapping):
        for field in mapping:
            if not field in [f.name for f in self.fields] and field != 'responds_to':
                raise ValueError('invalid field provided: ' + field)

    @property
    def is_ack(self):
        return True

    @property
    def is_ack_v(self):
        return self.cam.ack_v.enabled

    @property
    def is_ack_x(self):
        return self.cam.ack_x.enabled

    @property
    def is_ack_s(self):
        return self.cam.ack_s.enabled

    @property
    def warnings_enabled(self):
        return self.cam.ack_w.value

    @property
    def errors_enabled(self):
        return self.cam.ack_er.value

@dataclass
class ExtensionAcknowledgePacket(AcknowledgePacket):
    """
    Extension Acknowledge Command Packet subtype
    """
    def __post_init__(self):
        super().__post_init__()
        self.header.packet_type.value = PacketType.EXTENSION_COMMAND