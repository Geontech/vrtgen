from dataclasses import dataclass
import math

from vrtgen.parser.model.types import *
from vrtgen.parser.model.base import CIF
from vrtgen.parser.model.aor import *
from vrtgen.parser.model.cif0 import Gain
from vrtgen.parser.model.prologue import Timestamp
from vrtgen.parser.value import parse_enable, parse_entry_size

@dataclass
class Polarization(PackedStruct):
    """
    Antenna polarization [9.4.8].
    """
    name : str = 'polarization'
    tilt_angle : FixedPointType = field(default_factory=lambda: FixedPointType('tilt_angle', bits=16, signed=True, radix=13, packed_tag=PackedTag(31,16,0)))
    ellipticity_angle : FixedPointType = field(default_factory=lambda: FixedPointType('ellipticity_angle', bits=16, signed=True, radix=13, packed_tag=PackedTag(15,16,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

@dataclass
class PointingVector(PackedStruct):
    """
    3-D Pointing Vector [9.4.1.1].
    """
    name : str = 'pointing_vector'
    elevation : FixedPoint16r7 = field(default_factory=lambda: FixedPoint16r7('elevation', packed_tag=PackedTag(31,16,0)))
    azimuthal : FixedPoint16r7 = field(default_factory=lambda: FixedPoint16r7('azimuthal', packed_tag=PackedTag(15,16,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

@dataclass
class SpatialReferenceType(PackedStruct):
    """
    Spatial Reference Type [9.4.12].
    """
    name : str = 'spatial_reference_type'
    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

@dataclass
class BeamWidth(PackedStruct):
    """
    Beam Widths [9.4.2].
    """
    name : str = 'beam_width'
    horizontal : FixedPoint16r7 = field(default_factory=lambda: FixedPoint16r7('horizontal', packed_tag=PackedTag(31,16,0)))
    vertical : FixedPoint16r7 = field(default_factory=lambda: FixedPoint16r7('vertical', packed_tag=PackedTag(15,16,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

@dataclass
class EbNoBER(PackedStruct):
    """
    Energy per Bit to Noise Density Ratio (Eb/No) and Bit Error Rate [9.5.17].
    """
    name : str = 'ebno_ber'
    # Maximum positive value (0x7FFF) signifies "unused"
    # Energy per Bit to Noise Density ratio (Eb/No)
    ebno : FixedPoint16r7 = field(default_factory=lambda: FixedPoint16r7('horizontal', value=0x7FFF, packed_tag=PackedTag(31,16,0)))
    # Bit Error Rate (BER)
    ber : FixedPoint16r7 = field(default_factory=lambda: FixedPoint16r7('vertical', value=0x7FFF, packed_tag=PackedTag(15,16,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32


@dataclass
class Threshold(PackedStruct):
    """
    Signal threshold level [9.5.13].
    """
    name : str = 'threshold'
    stage_2 : FixedPoint16r7 = field(default_factory=lambda: FixedPoint16r7('stage_2', packed_tag=PackedTag(31,16,0)))
    stage_1 : FixedPoint16r7 = field(default_factory=lambda: FixedPoint16r7('stage_1', packed_tag=PackedTag(15,16,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32


@dataclass
class InterceptPoints(PackedStruct):
    """
    Second and Third Order Intercept points [9.5.6].
    """
    name : str = 'intercept_points'
    second_order : FixedPoint16r7 = field(default_factory=lambda: FixedPoint16r7('second_order', packed_tag=PackedTag(31,16,0)))
    third_order : FixedPoint16r7 = field(default_factory=lambda: FixedPoint16r7('third_order', packed_tag=PackedTag(15,16,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

@dataclass
class SNRNoise(PackedStruct):
    """
    Signal-to-Noise Ratio (SNR) and Noise Figure [9.5.7].
    """
    name : str = 'snr_noise'
    # Signal-to-Noise Ratio (dB)
    # Maximum positive value (0x7FFF) signifies "unused"
    snr : FixedPoint16r7 = field(default_factory=lambda: FixedPoint16r7('snr', value=0x7FFF, packed_tag=PackedTag(31,16,0)))
    # Noise Figure (db)
    noise : FixedPoint16r7 = field(default_factory=lambda: FixedPoint16r7('noise', packed_tag=PackedTag(15,16,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

@dataclass
class Time(FixedPointType):
    """
    Geolocation Angle Format [Definition 9.4.5-1].
    """
    name : str = 'Time'
    def __post_init__(self):
        self.bits = 64
        self.signed = True
        self.radix = 22

@dataclass
class SpectrumType(PackedStruct):
    """
    Spectrum Type Subfield [9.6.1.1]
    """
    name : str = 'SpectrumType'
    reserved: IntegerType = field(default_factory=lambda: IntegerType('reserved', bits=12, packed_tag=PackedTag(31,12,0)))
    spectrum_type: IntegerType = field(default_factory=lambda: IntegerType('spectrum_type', bits=8, packed_tag=PackedTag(19,8,0)))
    averaging_type: IntegerType = field(default_factory=lambda: IntegerType('averaging_type', bits=8, packed_tag=PackedTag(11,8,0)))
    window_time : IntegerType = field(default_factory=lambda: IntegerType('window_time', bits=4, packed_tag=PackedTag(3,4,0)))

@dataclass
class WindowType(PackedStruct):
    """
    Window Type Subfield [9.6.1.2]
    """
    name : str = 'WindowType'
    reserved: IntegerType = field(default_factory=lambda: IntegerType('reserved', bits=24, packed_tag=PackedTag(31,24,0)))
    window_type: IntegerType = field(default_factory=lambda: IntegerType('window_type', bits=8, packed_tag=PackedTag(7,8,0)))

@dataclass
class SpectrumF1F2Indicies(PackedStruct):
    """
    Spectrum F1-F2 Indicies Subfield [9.6.1.9]
    """
    name : str = 'SpectrumF1F2Indicies'
    f1_index : Unsigned32 = field(default_factory=lambda: Unsigned32('f1_index', packed_tag=PackedTag(31,32,0)))
    f2_index : Unsigned32 = field(default_factory=lambda: Unsigned32('f2_index', packed_tag=PackedTag(31,32,1)))

@dataclass
class Spectrum(PackedStruct):
    """
    Spectrum [9.6.1]
    """
    name : str = 'Spectrum'
    spectrum_type : SpectrumType = field(default_factory=lambda: SpectrumType('spectrum_type', packed_tag=PackedTag(31,32,0)))
    window_type : WindowType = field(default_factory=lambda: WindowType('window_type', packed_tag=PackedTag(31,32,1)))
    number_transform_points : Unsigned32 = field(default_factory=lambda: Unsigned32('number_transform_points', packed_tag=PackedTag(31,32,2)))
    number_window_points : Unsigned32 = field(default_factory=lambda: Unsigned32('number_window_points', packed_tag=PackedTag(31,32,3)))
    resolution : FixedPoint64r20 = field(default_factory=lambda: FixedPoint64r20('resolution', packed_tag=PackedTag(31,64,4)))
    span : FixedPoint64r20 = field(default_factory=lambda: FixedPoint64r20('span', packed_tag=PackedTag(31,64,6)))
    number_averages : Unsigned32 = field(default_factory=lambda: Unsigned32('number_averages', packed_tag=PackedTag(31,32,8)))
    # The weighting_factor field is defined to be 32-bits, but there is no mention of type.
    # As it represents a coefficient value, we assume a signed fixed-point w/ radix 16 here.
    weighting_factor : FixedPointType = field(default_factory=lambda: FixedPointType('weighting_factor', bits=32, signed=True, radix=16, packed_tag=PackedTag(31,32,9)))
    spectrum_f1_f2_indicies : SpectrumF1F2Indicies = field(default_factory=lambda: SpectrumF1F2Indicies('spectrum_f1_f2_indicies', packed_tag=PackedTag(31, 64, 10)))
    # There are options for how the window_time_delta field can be defined:
    # A) Time: 32-bit unsigned int
    # B) Samples: 32-bit unsigned int
    # C) Percent Overlap: signed fixed-point w/ radix 12
    window_time_delta : Unsigned32 = field(default_factory=lambda: Unsigned32('window_time_delta', packed_tag=PackedTag(31,32,12)))
    # window_time_delta : FixedPointType = field(default_factory=lambda: FixedPointType('window_time_delta', bits=32, signed=True, radix=12, packed_tag=PackedTag(31,32,12)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 13 * 32 # 13 words
    
@dataclass
class SectorStepScanCIF(CIF):
    """
    Sector/Step-Scan Control/Context Indicator Word [9.6.2.1]
    """
    name : str = 'subfield_cif'
    sector_number : CIFEnableType = field(default_factory=lambda: CIFEnableType('sector_number', type_=Unsigned32(), enabled=True, required=True, packed_tag=PackedTag(31,1,0,0)))
    f1_start_frequency : CIFEnableType = field(default_factory=lambda: CIFEnableType('f1_start_frequency', type_=FixedPoint64r20(), enabled=True, required=True, packed_tag=PackedTag(30,1,0,0)))
    f2_stop_frequency : CIFEnableType = field(default_factory=lambda: CIFEnableType('f2_stop_frequency', type_=FixedPoint64r20(), packed_tag=PackedTag(29,1,0,0)))
    resolution_bandwidth : CIFEnableType = field(default_factory=lambda: CIFEnableType('resolution_bandwidth', type_=FixedPoint64r20(), packed_tag=PackedTag(28,1,0,0)))
    tune_step_size : CIFEnableType = field(default_factory=lambda: CIFEnableType('tune_step_size', type_=FixedPoint64r20(), packed_tag=PackedTag(27,1,0,0)))
    number_points : CIFEnableType = field(default_factory=lambda: CIFEnableType('number_points', type_=IntegerType('NumberPoints', bits=32), packed_tag=PackedTag(26,1,0,0)))
    default_gain : CIFEnableType = field(default_factory=lambda: CIFEnableType('default_gain', type_=Gain('DefaultGain'), packed_tag=PackedTag(25,1,0,0)))
    threshold : CIFEnableType = field(default_factory=lambda: CIFEnableType('threshold', type_=Gain('Threshold'), packed_tag=PackedTag(24,1,0,0)))
    dwell_time : CIFEnableType = field(default_factory=lambda: CIFEnableType('dwell_time', type_=FractionalTimestamp('DwellTime'), packed_tag=PackedTag(23,1,0,0)))
    start_time : CIFEnableType = field(default_factory=lambda: CIFEnableType('start_time', type_=Timestamp('StartTime'), packed_tag=PackedTag(22,1,0,0)))
    time_3 : CIFEnableType = field(default_factory=lambda: CIFEnableType('time_3', type_=FractionalTimestamp('Time3', bits=32), packed_tag=PackedTag(21,1,0,0)))
    time_4 : CIFEnableType = field(default_factory=lambda: CIFEnableType('time_4', type_=FractionalTimestamp('Time4', bits=32), packed_tag=PackedTag(20,1,0,0)))
    packed_0 : PackedType = field(default_factory=lambda: PackedType('packed_0', bits=32, packed_tag=PackedTag(0,32,0)))

    def _validate(self, mapping):
        for key, val in mapping.items():
            if not key in [f.name for f in self.fields]:
                raise ValueError('invalid field provided: {}'.format(key))
            elif key == 'sector_number' or key == 'f1_start_frequency':
                raise ValueError('invalid field provided: {} already required by default'.format(key))

@dataclass
class SectorStepScanRecord(Record):
    """
    SectorStepScan Record Type
    """
    name : str = 'SectorStepScanRecord'
    sector_number : Unsigned32 = field(default_factory=lambda: Unsigned32('sector_number', enabled=True, required=True, packed_tag=PackedTag(31,32,0)))
    f1_start_frequency : FixedPoint64r20 = field(default_factory=lambda: FixedPoint64r20('f1_start_frequency', enabled=True, required=True, packed_tag=PackedTag(31,64,1)))
    f2_stop_frequency : FixedPoint64r20 = field(default_factory=lambda: FixedPoint64r20('f2_stop_frequency', packed_tag=PackedTag(31,64,3)))
    resolution_bandwidth : FixedPoint64r20 = field(default_factory=lambda: FixedPoint64r20(packed_tag=PackedTag(31,64,5)))
    tune_step_size : FixedPoint64r20 = field(default_factory=lambda: FixedPoint64r20(packed_tag=PackedTag(31,64,7)))
    number_points : Unsigned32 = field(default_factory=lambda: Unsigned32('number_points', packed_tag=PackedTag(31,32,9)))
    default_gain : Gain = field(default_factory=lambda: Gain('default_gain', packed_tag=PackedTag(31,32,10)))
    threshold : Gain = field(default_factory=lambda: Gain('threshold', packed_tag=PackedTag(31,32,11)))
    dwell_time : FractionalTimestamp = field(default_factory=lambda: FractionalTimestamp('dwell_time', type_=FractionalTimestamp('DwellTime'), packed_tag=PackedTag(23,1,0)))
    start_time : Timestamp = field(default_factory=lambda: Timestamp('start_time', type_=Timestamp('StartTime'), packed_tag=PackedTag(22,1,0)))
    time_3 : FractionalTimestamp = field(default_factory=lambda: FractionalTimestamp('time_3', type_=FractionalTimestamp('Time3', bits=32), packed_tag=PackedTag(21,1,0)))
    time_4 : FractionalTimestamp = field(default_factory=lambda: FractionalTimestamp('time_4', type_=FractionalTimestamp('Time4', bits=32), packed_tag=PackedTag(20,1,0)))

@dataclass
class SectorStepScan(ArrayOfRecords):
    """
    Sector/Step-Scan Field [9.6.2]
    """
    name : str = 'sector_step_scan'
    subfield_cif : SectorStepScanCIF = field(default_factory=lambda: SectorStepScanCIF(name='subfield_cif', enabled=True, required=True, packed_tag=PackedTag(31,32,2)))
    records : ListType = field(default_factory=lambda: ListType('records', type_=SectorStepScanRecord(), enabled=True, required=True, packed_tag=PackedTag(31,0,3)))

    def __post_init__(self):
        super().__post_init__()
        # Association needed for jinja generation
        self.records.linked_size = self.num_records

    def _parse_mapping(self, mapping):
        for key,val in mapping.items():
            if key == 'subfield_cif':
                self.__dict__[key] = val

@dataclass
class IndexList(TemplateArrayStruct):
    """
    Index List Field [9.3.2]
    """
    name : str = 'index_list'
    total_size : Unsigned32 = field(default_factory=lambda: Unsigned32('total_size', bits=32, required=True, packed_tag=PackedTag(31,32,0)))
    entry_size : EnumType = field(default_factory=lambda: EnumType('entry_size', type_=EntrySize, value=EntrySize.EIGHT_BIT.value, enabled=True, required=True, packed_tag=PackedTag(31,4,1,0)))
    num_entries : IntegerType = field(default_factory=lambda: IntegerType('num_entries', bits=20, required=True, packed_tag=PackedTag(19,20,1,0)))
    packed_0 : PackedType = field(default_factory=lambda: PackedType('packed_0', bits=32, packed_tag=PackedTag(0,32,1)))
    entries : ListType = field(default_factory=lambda: ListType('entries', type_=IntegerType(), enabled=True, required=True, packed_tag=PackedTag(31,0,2)))

    def __post_init__(self):
        super().__post_init__()
        self.entries.linked_size = self.num_entries
        self.total_size_field = self.total_size

    def _validate(self, mapping):
        for field in mapping:
            if not field in [f.name for f in self.fields]:
                raise ValueError('invalid field provided: ' + field)

    def _parse_mapping(self, mapping):
        for key,val in mapping.items():
            if key == 'entry_size':
                self.entry_size.value = parse_entry_size(val)
                if self.entry_size.value == EntrySize.EIGHT_BIT:
                    self.entries.type_.bits = 8
                elif self.entry_size.value == EntrySize.SIXTEEN_BIT:
                    self.entries.type_.bits = 16
                elif self.entry_size.value == EntrySize.THIRTY_TWO_BIT:
                    self.entries.type_.bits = 32
                self.template_type = 'uint{}_t'.format(self.entries.type_.bits)

    def validate_and_parse_mapping(self, **mapping):
        self._validate(mapping)
        self._parse_mapping(mapping)

@dataclass
class DiscreteIO(PackedStruct):
    """
    Discrete I/O Field [9.11]
    """
    name : str = 'discrete_io'
    subfields : List[Field] = field(default_factory=list)

    def __post_init__(self):
        super().__post_init__()
        self.user_defined = True

    @property
    def is_integer(self):
        return len(self.subfields) == 0

    def _validate(self, mapping):
        for key,val in mapping.items():
            if key == 'mode':
                continue
            elif val == 'bool':
                continue
            elif val == 'enable_indicator':
                continue
            elif isinstance(val, int):
                continue
            elif isinstance(val, list):
                continue
            else:
                raise ValueError('invalid discrete_io type specified: ', val)

    def _parse_mapping(self, mapping):
        packed_tag_pos = 0
        packed_tag_word = 0
        for key,val in mapping.items():
            if key == 'mode':
                mode = parse_enable(val)
                self.enabled = True if (mode == Mode.REQUIRED or mode == Mode.OPTIONAL) else False
                self.required = True if (mode == Mode.REQUIRED) else False
            elif val == 'bool':
                self.subfields.append(BooleanType(key, enabled=True, required=True, packed_tag=PackedTag(packed_tag_pos,1,packed_tag_word,packed_tag_word)))
                packed_tag_pos += 1
            elif val == 'enable_indicator':
                self.subfields.append(EnableIndicatorType(key, enabled=True, required=True, packed_tag=PackedTag(packed_tag_pos,1,packed_tag_word,packed_tag_word)))
                packed_tag_pos += 1
                self.subfields.append(EnableIndicatorType(key+'_enable', enabled=True, required=True, is_enable=True, packed_tag=PackedTag(packed_tag_pos,1,packed_tag_word,packed_tag_word)))
                packed_tag_pos += 1
            elif isinstance(val, int):
                pos = packed_tag_pos + val - 1
                self.subfields.append(IntegerType(key, bits=val, enabled=True, required=True, packed_tag=PackedTag(pos,val,packed_tag_word,packed_tag_word)))
                packed_tag_pos += val
            elif isinstance(val, list):
                bits = math.ceil(len(val)/2)
                enum_members = {}
                member_val = 0
                for item in val:
                    enum_members[item] = member_val
                    member_val += 1
                discrete_io_enum = BinaryEnum(key, enum_members)
                discrete_io_enum.bits = bits
                pos = packed_tag_pos + bits - 1
                self.subfields.append(EnumType(key, enabled=True, required=True, user_defined=True, type_=discrete_io_enum, packed_tag=PackedTag(pos,bits,packed_tag_word,packed_tag_word)))
                packed_tag_pos += bits
                
            if packed_tag_pos > self.bits:
                raise ValueError('too many subfield bits for discrete io field')
            packed_tag_word = packed_tag_pos // 32

    def validate_and_parse_mapping(self, **mapping):
        self._validate(mapping)
        self._parse_mapping(mapping)

@dataclass
class DiscreteIO32(DiscreteIO):
    """
    Discrete I/O Field 32-bit [9.11]
    """
    name : str = 'discrete_io_32'
    subfields : List[Field] = field(default_factory=list)
    packed : PackedType = field(default_factory=lambda: PackedType('packed', bits=32, enabled=True, required=True, packed_tag=PackedTag(0,32,1)))

    def __post_init__(self):
        super().__post_init__()
        self.bits = 32

    @property
    def fields(self):
        return [field for field in self.subfields] + [self.packed]

@dataclass
class DiscreteIO64(DiscreteIO):
    """
    Discrete I/O Field 64-bit [9.11]
    """
    name : str = 'discrete_io_64'
    subfields : List[Field] = field(default_factory=list)
    packed : PackedType = field(default_factory=lambda: PackedType('packed', bits=64, enabled=True, required=True, packed_tag=PackedTag(0,64,1)))

    def __post_init__(self):
        super().__post_init__()
        self.bits = 64

    @property
    def fields(self):
        return ([field for field in self.subfields] + [self.packed])

@dataclass
class VersionInformation(PackedStruct):
    """
    Version Information [9.10.4].
    """
    name : str = 'version_information'
    year : IntegerType = field(default_factory=lambda: IntegerType('year', bits=7, enabled=True, required=True, packed_tag=PackedTag(31,7,0)))
    day : IntegerType = field(default_factory=lambda: IntegerType('day', bits=9, enabled=True, required=True, packed_tag=PackedTag(24,9,0)))
    revision : IntegerType = field(default_factory=lambda: IntegerType('revision', bits=6, enabled=True, required=True, packed_tag=PackedTag(15,6,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

@dataclass
class BufferStatus(PackedStruct):
    """
    Buffer Status Subfield [DIFI 1.2 4.3]
    """
    name : str = 'BufferStatus'
    reserved: IntegerType = field(default_factory=lambda: IntegerType('reserved', bits=16, packed_tag=PackedTag(31,16,0)))
    level: IntegerType = field(default_factory=lambda: IntegerType('spectrum_type', bits=12, packed_tag=PackedTag(15,12,0)))
    overflow: BooleanType = field(default_factory=lambda: BooleanType('overflow', packed_tag=PackedTag(3,1,0,0)))
    nearly_full: BooleanType = field(default_factory=lambda: BooleanType('nearly_full', packed_tag=PackedTag(2,1,0,0)))
    nearly_empty: BooleanType = field(default_factory=lambda: BooleanType('nearly_empty', packed_tag=PackedTag(1,1,0,0)))
    underflow: BooleanType = field(default_factory=lambda: BooleanType('underflow', packed_tag=PackedTag(0,1,0,0)))

@dataclass
class BufferSize(PackedStruct):
    """
    Buffer Size [DIFI 1.2 4.3].
    """
    name : str = 'buffer_size'
    buffer_size : Unsigned64 = field(default_factory=lambda: Unsigned64('buffer_size', packed_tag=PackedTag(31,64,0)))
    buffer_status : BufferStatus = field(default_factory=lambda: BufferStatus('buffer_status', packed_tag=PackedTag(31,32,2)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 96

    def _validate(self, mapping):
        for key,val in mapping.items():
            if key == 'mode':
                continue
            else:
                raise ValueError('invalid buffer_size type specified: ', val)

    def _parse_mapping(self, mapping):
        for key,val in mapping.items():
            if key == 'mode':
                mode = parse_enable(val)
                self.enabled = True if (mode == Mode.REQUIRED or mode == Mode.OPTIONAL) else False
                self.required = True if (mode == Mode.REQUIRED) else False

    def validate_and_parse_mapping(self, **mapping):
        self._validate(mapping)
        self._parse_mapping(mapping)
        
@dataclass
class CIF1(CIF):
    """
    CIF1 Enables
    """
    name : str = 'cif_1'
    phase_offset : CIFEnableType = field(default_factory=lambda: CIFEnableType('phase_offset', type_=FixedPoint16r7(reserved_bits=16), packed_tag=PackedTag(31,1,0,0)))
    polarization : CIFEnableType = field(default_factory=lambda: CIFEnableType('polarization', type_=Polarization(), packed_tag=PackedTag(30,1,0,0)))
    pointing_vector : CIFEnableType = field(default_factory=lambda: CIFEnableType('pointing_vector', type_=PointingVector(), packed_tag=PackedTag(29,1,0,0)))
    pointing_vector_structure : CIFEnableType = field(default_factory=lambda: CIFEnableType('pointing_vector_structure', packed_tag=PackedTag(28,1,0,0)))
    spatial_scan_type : CIFEnableType = field(default_factory=lambda: CIFEnableType('spatial_scan_type', type_=Identifier16(), packed_tag=PackedTag(27,1,0,0)))
    spatial_reference_type : CIFEnableType = field(default_factory=lambda: CIFEnableType('spatial_reference_type', packed_tag=PackedTag(26,1,0,0)))
    beam_width : CIFEnableType = field(default_factory=lambda: CIFEnableType('beam_width', type_=BeamWidth(), packed_tag=PackedTag(25,1,0,0)))
    range : CIFEnableType = field(default_factory=lambda: CIFEnableType('range', type_=FixedPoint32r6(), packed_tag=PackedTag(24,1,0,0)))
    ebno_ber : CIFEnableType = field(default_factory=lambda: CIFEnableType('ebno_ber', type_=EbNoBER(), packed_tag=PackedTag(20,1,0,0)))
    threshold : CIFEnableType = field(default_factory=lambda: CIFEnableType('threshold', type_=Threshold(), packed_tag=PackedTag(19,1,0,0)))
    compression_point : CIFEnableType = field(default_factory=lambda: CIFEnableType('compression_point', type_=FixedPoint16r7(reserved_bits=16), packed_tag=PackedTag(18,1,0,0)))
    intercept_points : CIFEnableType = field(default_factory=lambda: CIFEnableType('intercept_points', type_=InterceptPoints(), packed_tag=PackedTag(17,1,0,0)))
    snr_noise_figure : CIFEnableType = field(default_factory=lambda: CIFEnableType('snr_noise_figure', type_=SNRNoise(), packed_tag=PackedTag(16,1,0,0)))
    aux_frequency : CIFEnableType = field(default_factory=lambda: CIFEnableType('aux_frequency', type_=FixedPoint64r20(), packed_tag=PackedTag(15,1,0,0)))
    aux_gain : CIFEnableType = field(default_factory=lambda: CIFEnableType('aux_gain', type_=Gain(), packed_tag=PackedTag(14,1,0,0)))
    aux_bandwidth : CIFEnableType = field(default_factory=lambda: CIFEnableType('aux_bandwidth', type_=FixedPoint64r20(), packed_tag=PackedTag(13,1,0,0)))
    array_of_cifs : CIFEnableType = field(default_factory=lambda: CIFEnableType('array_of_cifs', packed_tag=PackedTag(11,1,0,0)))
    spectrum : CIFEnableType = field(default_factory=lambda: CIFEnableType('spectrum', type_=Spectrum(), packed_tag=PackedTag(10,1,0,0)))
    sector_step_scan : CIFEnableType = field(default_factory=lambda: CIFEnableType('sector_step_scan', type_=SectorStepScan(), packed_tag=PackedTag(9,1,0,0)))
    index_list : CIFEnableType = field(default_factory=lambda: CIFEnableType('index_list', type_=IndexList(), packed_tag=PackedTag(7,1,0,0)))
    discrete_io_32 : CIFEnableType = field(default_factory=lambda: CIFEnableType('discrete_io_32', type_=DiscreteIO32(), packed_tag=PackedTag(6,1,0,0)))
    discrete_io_64 : CIFEnableType = field(default_factory=lambda: CIFEnableType('discrete_io_64', type_=DiscreteIO64(), packed_tag=PackedTag(5,1,0,0)))
    health_status : CIFEnableType = field(default_factory=lambda: CIFEnableType('health_status', type_=Identifier16(), packed_tag=PackedTag(4,1,0,0)))
    v49_spec_compliance : CIFEnableType = field(default_factory=lambda: CIFEnableType('v49_spec_compliance', type_=EnumType('v49_spec', type_=V49StandardCompliance, value=1), packed_tag=PackedTag(3,1,0,0)))
    version_information : CIFEnableType = field(default_factory=lambda: CIFEnableType('version_information', type_=VersionInformation(), packed_tag=PackedTag(2,1,0,0)))
    buffer_size : CIFEnableType = field(default_factory=lambda: CIFEnableType('buffer_size', type_=FixedPoint64r20(), packed_tag=PackedTag(1,1,0,0)))
    packed_0 : PackedType = field(default_factory=lambda: PackedType('packed_0', bits=32, packed_tag=PackedTag(0,32,0)))

    def _validate(self, mapping):
        for key, val in mapping.items():
            if not key in [f.name for f in self.fields]:
                raise ValueError('invalid field provided: ' + key)
            elif key == 'sector_step_scan' and not isinstance(val, SectorStepScan):
                raise ValueError('Sector Step Scan not of the correct type')
            elif key == 'index_list' and not isinstance(val, IndexList):
                raise ValueError('Index list not of the correct type')

    def _parse_mapping(self, mapping):
        for key,val in mapping.items():
            if key in ('sector_step_scan', 'index_list'):
                self.__dict__[key].enabled = True
                self.__dict__[key].required = True
                self.__dict__[key].type_ = val
            elif 'discrete_io' in key:
                if isinstance(val, str):
                    mode = parse_enable(val)
                    self.__dict__[key].enabled = True if (mode == Mode.REQUIRED or mode == Mode.OPTIONAL) else False
                    self.__dict__[key].required = True if (mode == Mode.REQUIRED) else False
                else:
                    self.__dict__[key].enabled = val.enabled
                    self.__dict__[key].required = val.required
                    self.__dict__[key].type_ = val
            elif 'buffer_size' in key:
                if isinstance(val, str):
                    mode = parse_enable(val)
                    self.__dict__[key].enabled = True if (mode == Mode.REQUIRED or mode == Mode.OPTIONAL) else False
                    self.__dict__[key].required = True if (mode == Mode.REQUIRED) else False
                else:
                    self.__dict__[key].enabled = val.enabled
                    self.__dict__[key].required = val.required
                    self.__dict__[key].type_ = val
            else:
                mode = parse_enable(val)
                self.__dict__[key].enabled = True if (mode == Mode.REQUIRED or mode == Mode.OPTIONAL) else False
                self.__dict__[key].required = True if (mode == Mode.REQUIRED) else False
