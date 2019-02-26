from enum import IntEnum
import inspect
import struct

from .formats import *

class PacketType(IntEnum):
    """
    Constants for the 4-bit Packet Type field in the VRT Packet Header.
    Refer to VITA 49.2 Table 5.1.1-1.
    """
    SIGNAL_DATA              = 0 # 0000
    SIGNAL_DATA_STREAM_ID    = 1 # 0001
    EXTENSION_DATA           = 2 # 0010
    EXTENSION_DATA_STREAM_ID = 3 # 0011
    CONTEXT                  = 4 # 0100
    EXTENSION_CONTEXT        = 5 # 0101
    COMMAND                  = 6 # 0110
    EXTENSION_COMMAND        = 7 # 0111
    # 1000-1111 reserved for future VRT Packet types

class Field:
    class Mode(IntEnum):
        DISABLED  = 0
        OPTIONAL  = 1
        REQUIRED  = 2
        MANDATORY = 3

    __slots__ = ('_constant', '_enable')
    def __init__(self, enable=Mode.DISABLED):
        self._enable = Field.Mode(enable)
        self._constant = False

    @property
    def is_set(self):
        if self.has_value:
            return True
        return self.is_mandatory

    @property
    def has_value(self):
        return False

    @property
    def is_enabled(self):
        return self._enable != Field.Mode.DISABLED

    @property
    def enable(self):
        return self._enable

    @enable.setter
    def enable(self, enabled):
        enabled = Field.Mode(enabled)
        if self.is_mandatory:
            # Mandatory state cannot be overwritten
            if enabled in (Field.Mode.DISABLED, Field.Mode.OPTIONAL):
                raise ValueError('field is mandatory')
            return
        self._enable = enabled

    @property
    def is_required(self):
        return self._enable in (Field.Mode.REQUIRED, Field.Mode.MANDATORY)

    @property
    def is_optional(self):
        return self._enable == Field.Mode.OPTIONAL

    @property
    def is_mandatory(self):
        return self._enable == Field.Mode.MANDATORY

    @property
    def is_disabled(self):
        return self._enable == Field.Mode.DISABLED

    @property
    def is_constant(self):
        return self._constant

    def set_constant(self):
        if not self.is_required:
            self.enable = Field.Mode.REQUIRED
        self._constant = True

    @property
    def enable_flag(self):
        return 1 << self.enable_bit

    @classmethod
    def match(cls, name):
        # Relying on a little bit of class trickery here: Field does not have
        # a "name" attribute, but it's never directly used. field_descriptor()
        # creates derived classes with the name set as a class attribute (this
        # is also a minor space optimization, since the name is the same for
        # all instances).
        return cls.name.casefold() == name.casefold()

class FieldContainer:
    def __init__(self):
        self.__fields = {}
        for name, field in self.get_field_descriptors():
            instance = field()
            setattr(self, name, instance)
            self.__fields[field.name.casefold()] = instance

    def get_field(self, name):
        return self.__fields.get(name.casefold(), None)

    @classmethod
    def get_field_descriptors(cls):
        def is_field(obj):
            return isinstance(obj, type) and issubclass(obj, Field)
        return inspect.getmembers(cls, is_field)

    @property
    def fields(self):
        return self.__fields.values()

# Placeholder for field types that have not been implemented yet
class UnimplementedField(Field):
    pass

class UserDefinedField(Field):
    def __init__(self):
        Field.__init__(self)
        self.value = None

    @classmethod
    def create(cls, bits):
        name = 'UserDefined{:d}Field'.format(bits)
        return type(name, (cls,), {'bits':bits})

    def add_field(self, name, bits, word=None, position=None):
        # TODO: validation of fields (overlap, size, etc.)
        pass

class SimpleField(Field):
    def __init__(self):
        Field.__init__(self)
        self.value = None

    @property
    def has_value(self):
        return self.value is not None

class BitField(SimpleField):
    pass

class IntegerField(SimpleField):
    @classmethod
    def create(cls, bits):
        name = 'Int{:d}Field'.format(bits)
        return type(name, (cls,), {'bits':bits})

Int64Field = IntegerField.create(64)
Int32Field = IntegerField.create(32)
Int24Field = IntegerField.create(24)
Int16Field = IntegerField.create(16)

Identifier32 = Int32Field
Identifier16 = Int16Field
StreamID = Identifier32

class FixedPointField(SimpleField):
    @classmethod
    def create(cls, bits, radix):
        name = 'Fixed{:d}_{:d}Field'.format(bits, radix)
        return type(name, (cls,), {'bits':bits, 'radix':radix})

class StructField(Field, FieldContainer):
    def __init__(self):
        Field.__init__(self)
        FieldContainer.__init__(self)

    @property
    def has_value(self):
        return any(f.value is not None for f in self.fields)

def field_descriptor(name, field, enable_bit=None):
        bases = (field,)
        namespace = {
            'name': name,
            'enable_bit': enable_bit
        }
        return type(name, bases, namespace)

class OUIField(Int24Field):
    pass

class ClassIDField(StructField):
    oui = field_descriptor('OUI', OUIField)
    information_class = field_descriptor('Information Class Code', Int16Field)
    packet_class = field_descriptor('Packet Class Code', Int16Field)

class TSIField(SimpleField):
    def __init__(self):
        super().__init__()
        self.value = TSI.NONE

class TSFField(SimpleField):
    def __init__(self):
        super().__init__()
        self.value = TSF.NONE

class DeviceIDField(StructField):
    manufacturer_oui = field_descriptor('Manufacturer OUI', OUIField)
    device_code = field_descriptor('Device Code', Int16Field)

class VRTPrologue(FieldContainer):
    stream_id = field_descriptor('Stream ID', StreamID)
    class_id = field_descriptor('Class ID', ClassIDField)
    tsi = field_descriptor('TSI', TSIField)
    tsf = field_descriptor('TSF', TSFField)
    integer_timestamp = field_descriptor('Integer Timestamp', Int32Field)
    fractional_timestamp = field_descriptor('Fractional Timestamp', Int64Field)

    def __init__(self):
        super().__init__()
        self.tsi.enable = Field.Mode.MANDATORY
        self.tsf.enable = Field.Mode.MANDATORY

class VRTPacket(object):
    def __init__(self, name):
        self.name = name
        self.prologue = VRTPrologue()

    @property
    def has_trailer(self):
        return False

    @property
    def stream_id(self):
        return self.prologue.stream_id

    @property
    def class_id(self):
        return self.prologue.class_id

    @property
    def tsi(self):
        return self.prologue.tsi.value

    @property
    def tsf(self):
        return self.prologue.tsf.value

    def get_header_bytes(self):
        header = bytearray(4)

        header[0] = self.packet_type() << 4
        if self.class_id.is_set:
            header[0] |= 0x08
        header[0] |= self.packet_specific_bits()
        header[1] = self.tsi << 6
        header[1] |= self.tsf << 4

        return header

class VRTDataTrailer(FieldContainer):
    calibrated_time = field_descriptor('Calibrated Time', BitField, 31)
    valid_data = field_descriptor('Valid Data', BitField, 30)
    reference_lock = field_descriptor('Reference Lock', BitField, 29)
    agc_mgc = field_descriptor('AGC/MGC', BitField, 28)
    detected_signal = field_descriptor('Detected Signal', BitField, 27)
    spectral_inversion = field_descriptor('Spectral Inversion', BitField, 26)
    over_range = field_descriptor('Over-range', BitField, 25)
    sample_loss = field_descriptor('Sample Loss', BitField, 24)
    # The Sample Frame field was added in V49.2, replacing 2 user-defined
    # bits. While the bits can still be user-defined for compatibility with
    # V49.0 implementations, the spec strongly discourages it, and it is
    # not supported here.
    sample_frame = field_descriptor('Sample Frame', IntegerField.create(2), 23)
    # [21..20], [9..8] User-Defined
    context_packet_count = field_descriptor('Associated Context Packet Count', IntegerField.create(7), 7)

    def get_bytes(self):
        word = 0
        for field in self.fields:
            if not field.is_set:
                continue
            word |= field.enable_flag
            if field.value:
                # The enable and value bits are offset by 12
                position = field.enable_bit - 12
                value = int(field.value)
                word |= value << position
        return struct.pack('>I', word)

    @property
    def is_enabled(self):
        return any(field.is_enabled for field in self.fields)

class VRTDataPacket(VRTPacket):
    def __init__(self, name):
        super().__init__(name)
        self.is_spectrum = False
        self.trailer = VRTDataTrailer()

    def packet_type(self):
        if self.prologue.stream_id.is_set:
            return PacketType.SIGNAL_DATA_STREAM_ID
        else:
            return PacketType.SIGNAL_DATA

    def packet_specific_bits(self):
        # Bits:
        # (T)railer invluded
        # (N)ot a V49.0 packet
        # Signal (S)pectrum or signal time data
        indicator = 0
        if self.has_trailer:
            indicator |= 0x4
        if not self.is_v49_0:
            indicator |= 0x2
        if self.is_spectrum:
            indicator |= 0x1
        return indicator

    @property
    def is_v49_0(self):
        # TBD: How to check?
        return False

    @property
    def has_trailer(self):
        return self.trailer.is_enabled

class GainField(StructField):
    stage1 = field_descriptor('Stage 1', FixedPointField.create(16, 7))
    stage2 = field_descriptor('Stage 2', FixedPointField.create(16, 7))

    def __init__(self):
        super().__init__()
        self.stage1.enable = Field.Mode.REQUIRED
        self.stage2.enable = Field.Mode.REQUIRED

class StateEventIndicators(StructField):
    calibrated_time = field_descriptor('Calibrated Time', BitField, 31)
    valid_data = field_descriptor('Valid Data', BitField, 30)
    reference_lock = field_descriptor('Reference Lock', BitField, 29)
    agc_mgc = field_descriptor('AGC/MGC', BitField, 28)
    detected_signal = field_descriptor('Detected Signal', BitField, 27)
    spectral_inversion = field_descriptor('Spectral Inversion', BitField, 26)
    over_range = field_descriptor('Over-range', BitField, 25)
    sample_loss = field_descriptor('Sample Loss', BitField, 24)
    # [23..20] reserved
    # [7..0] User-Defined
    user_defined = field_descriptor('User-Defined', UserDefinedField.create(8))

class GeolocationAngleField(FixedPointField):
    bits = 32
    radix = 22
    UNSPECIFIED = 0x7FFFFFFF

class GeolocationField(StructField):
    tsi = field_descriptor('TSI', TSIField)
    tsf = field_descriptor('TSF', TSFField)
    manufacturer_oui = field_descriptor('Manufacturer OUI', OUIField)
    # Integer timestamp should be 0xFFFFFFFF if TSI is 0
    integer_timestamp = field_descriptor('Integer-second Timestamp', Int32Field)
    # Fractional timestamp should be 0xFFFFFFFFFFFFFFFF if TSF is 0
    fractional_timestamp = field_descriptor('Fractional-second Timestamp', Int64Field)
    latitude = field_descriptor('Latitude', GeolocationAngleField)
    longitude = field_descriptor('Longitude', GeolocationAngleField)
    altitude = field_descriptor('Altitude', FixedPointField.create(32, 5))
    ground_speed = field_descriptor('Speed Over Ground', FixedPointField.create(32,16))
    heading_angle = field_descriptor('Heading Angle', GeolocationAngleField)
    track_angle = field_descriptor('Track Angle', GeolocationAngleField)
    magnetic_variation = field_descriptor('Magnetic Variation', GeolocationAngleField)

CartesianCoordinateField = FixedPointField.create(32, 5)

class EphemerisField(StructField):
    tsi = field_descriptor('TSI', TSIField)
    tsf = field_descriptor('TSF', TSFField)
    manufacturer_oui = field_descriptor('Manufacturer OUI', OUIField)
    # Integer timestamp should be 0xFFFFFFFF if TSI is 0
    integer_timestamp = field_descriptor('Integer-second Timestamp', Int32Field)
    # Fractional timestamp should be 0xFFFFFFFFFFFFFFFF if TSF is 0
    fractional_timestamp = field_descriptor('Fractional-second Timestamp', Int64Field)
    position_x = field_descriptor('Position X', CartesianCoordinateField)
    position_y = field_descriptor('Position Y', CartesianCoordinateField)
    position_z = field_descriptor('Position Z', CartesianCoordinateField)
    attitude_alpha = field_descriptor('Attitude Alpha', GeolocationAngleField)
    attitude_beta = field_descriptor('Attitude Beta', GeolocationAngleField)
    attitude_phi = field_descriptor('Attitude Phi', GeolocationAngleField)
    velocity_dx = field_descriptor('Velocity dX', FixedPointField.create(32, 16))
    velocity_dy = field_descriptor('Velocity dY', FixedPointField.create(32, 16))
    velocity_dz = field_descriptor('Velocity dZ', FixedPointField.create(32, 16))

class CIF0(FieldContainer):
    # Context Field Change Indicator (0/31) is a binary flag that can be
    # set at run-time. No configuration is possible.

    # Reference Point Identifier (0/30): Stream ID
    reference_point_id = field_descriptor('Reference Point Identifier', StreamID, 30)

    # Bandwidth (0/29) fixed-point 64/20, Hz
    bandwidth = field_descriptor('Bandwidth', FixedPointField.create(64, 20), 29)

    # IF Reference Frequency (0/28): fixed-point 64/20, Hz
    if_frequency = field_descriptor('IF Reference Frequency', FixedPointField.create(64, 20), 28)

    # RF Reference Frequency (0/27): fixed-point 64/20, Hz
    rf_frequency = field_descriptor('RF Reference Frequency', FixedPointField.create(64, 20), 27)

    # RF Reference Frequency Offset (0/26): fixed-point 64/20, Hz
    rf_frequency_offset = field_descriptor('RF Reference Frequency Offset', FixedPointField.create(64, 20), 26)

    # IF Band Offset (0/25): fixed-point 64/20, Hz
    if_band_offset = field_descriptor('IF Band Offset', FixedPointField.create(64, 20), 25)

    # Reference Level (0/24): fixed-point 16/7 dBm (upper 16 reserved)
    reference_level = field_descriptor('Reference Level', FixedPointField.create(16, 7), 24)

    # Gain (0/23): [stage2 (optional), stage1], fixed-point 16/7, dB
    gain = field_descriptor('Gain', GainField, 23)

    # Over-Range Count (0/22): integer 32
    over_range_count = field_descriptor('Over-Range Count', Int32Field, 22)

    # Sample Rate (0/21): fixed-point 64/20, Hz
    sample_rate = field_descriptor('Sample Rate', FixedPointField.create(64, 20), 21)

    # Timestamp Adjustment (0/20): fractional time (integer 64)
    timestamp_adjustment = field_descriptor('Timestamp Adjustment', Int64Field, 20)

    # Timestamp Calibration Time (0/19): 1 word, depends on prologue TSI
    timestamp_calibration_time = field_descriptor('Timestamp Calibration Time', Int32Field, 19)

    # Temperature (0/18): fixed-point 16/6, degrees C (upper 16 reserved)
    temperature = field_descriptor('Temperature', FixedPointField.create(16, 6), 18)

    # Device Identifier (0/17): 64 bits total, specific format
    device_id = field_descriptor('Device Identifier', DeviceIDField, 17)

    # State/Event Indicators (0/16): 32 bits, bit flags
    state_event_indicators = field_descriptor('State/Event Indicators', StateEventIndicators, 16)

    # Data Payload Format (0/15): structured
    data_format = field_descriptor('Signal Data Packet Payload Format', UnimplementedField, 15)

    # Formatted GPS (0/14): structured
    formatted_gps = field_descriptor('Formatted GPS', GeolocationField, 14)

    # Formatted INS (0/13): same format as GPS
    formatted_ins = field_descriptor('Formatted INS', GeolocationField, 13)

    # ECEF Ephemeris (0/12): structured
    ecef_ephemeris = field_descriptor('ECEF Ephemeris', EphemerisField, 12)

    # Relative Ephemeris (0/11): same format as ECEF Ephemeris
    # See also Ephemeris Reference Identifier
    relative_ephemeris = field_descriptor('Relative Ephemeris', EphemerisField, 11)

    # Ephemeric Reference Identifier (0/10): Stream ID of context packet stream
    # whose ECEF Ephemeris is necessary to translate Relative Ephemeris
    ephemeris_ref_id = field_descriptor('Ephemeris Reference ID', StreamID, 10)

    # GPS ASCII (0/9): Formatted GPS "sentences" (such as NMEA). 2 word header
    # (manufacturer OUI, number of words) plus arbitrary binary data, null-
    # padded to a full word.
    gps_ascii = field_descriptor('GPS ASCII', UnimplementedField, 9)

    # Context Assocation Lists (0/8): Structure to associate multiple context
    # streams with another stream. There is a 2-word header followed by four
    # lists of Stream IDs (Source, System, Vector-Component, Asynchronous-
    # Channel) and an option Asychronous-Channel tag list. Most, if not all,
    # of this should be handled at run-time, with the only code generation
    # support being to enable the field.
    context_association_lists = field_descriptor('Context Association Lists', UnimplementedField, 8)

    # Field Attributes Enable (CIF7)
    # Reserved
    # Reserved
    # Reserved
    # CIF 3 Enable
    # CIF 2 Enable
    # CIF 1 Enable
    # Reserved

    def get_prologue_bytes(self):
        prologue = 0
        for field in self.fields:
            if field.is_set:
                prologue |= 1 << field.enable_bit
        return struct.pack('>I', prologue)

class PolarizationField(StructField):
    tilt = field_descriptor('Tilt Angle', FixedPointField.create(16, 13))
    ellipticity = field_descriptor('Ellipticity Angle', FixedPointField.create(16, 13))

class PointingVectorField(StructField):
    elevation = field_descriptor('Elevation Angle', FixedPointField.create(16, 7))
    azimuthal = field_descriptor('Azimuthal Angle', FixedPointField.create(16, 7))

class BeamWidthsField(StructField):
    horizontal = field_descriptor('Horizontal', FixedPointField.create(16, 7))
    vertical = field_descriptor('Vertical', FixedPointField.create(16, 7))

class EbNoBERField(StructField):
    # Energy per Bit to Noise Density ratio (Eb/No)
    # Maximum positive value (0x7FFF) signifies "unused"
    ebno = field_descriptor('Eb/No', FixedPointField.create(16, 7))

    # Bit Error Rate (BER)
    # Maximum positive value (0x7FFF) signifies "unused"
    ber = field_descriptor('BER', FixedPointField.create(16, 7))

class ThresholdField(StructField):
    stage1 = field_descriptor('Stage 1', FixedPointField.create(16, 7))
    stage2 = field_descriptor('Stage 2', FixedPointField.create(16, 7))

class SNRNoiseField(StructField):
    # Signal-to-Noise Ratio (dB)
    # Maximum positive value (0x7FFF) signifies "unused"
    snr = field_descriptor('SNR', FixedPointField.create(16, 7))

    # Noise Figure (db)
    # O signifies "unused"
    noise = field_descriptor('Noise Figure', FixedPointField.create(16, 7))

class IndexListOptions(Field):
    def __init__(self):
        super().__init__()
        self.__size = None

    @property
    def entry_size(self):
        return self.__size

    @entry_size.setter
    def entry_size(self, size):
        if size not in (None, 8, 16, 32):
            raise ValueError('Index list entries must be 8, 16 or 32 bits')
        self.__size = size

class DiscreteIO32Field(UserDefinedField):
    bits = 32

class DiscreteIO64Field(UserDefinedField):
    bits = 64

class CIF1(FieldContainer):
    # Phase Offset (1/31): fixed-point 16/7, radians (upper 16 reserved)
    phase_offset = field_descriptor('Phase Offset', FixedPointField.create(16, 7), 31)

    # Polarization (1/30): [tilt, ellipticity], fixed-point 16/13, radians
    polarization = field_descriptor('Polarization', PolarizationField, 30)

    # 3-D Pointing Vector (1/29): [elevation, azimuthal], fixed-point 16/7, degrees
    pointing_vector = field_descriptor('3-D Pointing Vector', PointingVectorField, 29)

    # 3-D Pointing Vector Structure (1/28): structured data
    pointing_vector_struct = field_descriptor('3-D Pointing Vector Structure', UnimplementedField, 28)

    # Spatial Scan Type (1/27): Generic 16-bit identifier
    spatial_scan_type = field_descriptor('Spatial Scan Type', Identifier16, 27)

    # Spatial Reference Type (1/26): struct
    spatial_reference_type = field_descriptor('Spatial Reference Type', UnimplementedField, 26)

    # Beam Widths (1/25): [horizonal, vertical]: fixed-point 16/7. degrees
    # NOTE: This field is referred to inconsistently in both singular and
    # plural forms.
    beam_widths = field_descriptor('Beam Widths', BeamWidthsField, 25)

    # Range (1/24): fixed-point 32/6, meters
    range = field_descriptor('Range', FixedPointField.create(32, 6), 24)

    # Reserved (1/23)
    # Reserved (1/22)
    # Reserved (1/21)

    # Eb/No BER (1/20): [Eb/No, BER], fixed-point 16/7, dB
    ebno_ber = field_descriptor('Eb/No BER', EbNoBERField, 20)

    # Threshold (1/18): [stage2 (optional), stage1], fixed-point 16/7, dB
    threshold = field_descriptor('Threshold', ThresholdField, 19)

    # Compression Point (1/18): fixed-point 16/7, dBm
    # NOTE: Also called "1 dB Compression Point" and "1-dB Compression Point"
    compression_point = field_descriptor('Compression Point', FixedPointField.create(16, 7), 18)

    # Intercept Points (1/17): [2IIP, 3IIP], fixed-point 16/7, dBm
    intercept_points = field_descriptor('Intercept Points', UnimplementedField, 17)

    # SNR/Noise Figure (1/16): [SNR, Noise], fixed-point 16/7, dB
    snr_noise_figure = field_descriptor('SNR/Noise Figure', SNRNoiseField, 16)

    # Aux Frequency (1/15): fixed-point 64/20, Hz
    aux_frequency = field_descriptor('Aux Frequency', FixedPointField.create(64, 20), 15)

    # Aux Gain (1/14): [stage2 (optional), stage1], fixed-point 16/7, dB
    aux_gain = field_descriptor('Aux Gain', GainField, 14)

    # Aux Bandidth (1/13): fixed-point 64/20, Hz
    aux_bandwidth = field_descriptor('Aux Bandwidth', FixedPointField.create(64, 20), 13)

    # Reserved (1/12)

    # Array of CIFS (1/11): This allows multiple CIF blocks, wnich makes for
    # some complex support code.
    array_of_cifs = field_descriptor('Array of CIFS', UnimplementedField, 11)

    # Spectrum (1/10): multi-word structured data
    spectrum = field_descriptor('Spectrum', UnimplementedField, 10)

    # Sector Scan/Step (1/9): Array of records, with the exact format of the
    # records configurable. There is an overall header, much of which is run-
    # time (e.g., number of records), with a bitfield to enable specific
    # subfields.
    sector_scan_step = field_descriptor('Sector Scan/Step', UnimplementedField, 9),

    # Reserved (1/8)

    # Index List (1/7): Struct header with an array of identifiers (8, 16 or 32
    # bits). Only the entry size is configurable for code generation.
    index_list = field_descriptor('Index List', IndexListOptions, 7)

    # Discrete I/O 32-bit (1/6): 32 additional bits of user-defined fields
    discrete_io_32 = field_descriptor('Discrete I/O 32', DiscreteIO32Field, 6)

    # Discrete I/O 64-bit (1/7): 64 additional bits of user-defined fields
    discrete_io_64 = field_descriptor('Discrete I/O 64', DiscreteIO64Field, 5)

    # Health Status (1/4): 16-bit identifier
    health_status = field_descriptor('Health Status', Int16Field, 4)

    # V49 Spec Compliance (1/3): 32 bits for V49 compliance level, only four
    # values currently defined.

    # Version and Build Code (1/2): struct, 32 bits
    version_build_code = field_descriptor('Version and Build Code', UnimplementedField, 2)

    # Buffer Size (1/1): struct, 64 bits
    buffer_size = field_descriptor('Buffer Size', UnimplementedField, 1)

    # Reserved (1/0)

    def get_prologue_bytes(self):
        prologue = 0
        for field in self.fields:
            if field.is_set:
                prologue |= 1 << field.enable_bit
        return struct.pack('>I', prologue)

class VRTCIFPacket(VRTPacket):
    """
    Common base class for VRT packets that include the CIF fields (i.e.,
    context and command).
    """
    def __init__(self, name):
        super().__init__(name)
        self.stream_id.enable = Field.Mode.MANDATORY
        self.cif = [CIF0(), CIF1()]

    def get_header_bytes(self):
        base = super().get_header_bytes()
        return base + self.cif[0].get_prologue_bytes()

    def get_field(self, name):
        for cif in self.cif:
            field = cif.get_field(name)
            if field is not None:
                return field
        return None

class VRTContextPacket(VRTCIFPacket):
    def __init__(self, name):
        super().__init__(name)
        self.timestamp_mode = TSM.FINE

    def packet_type(self):
        return PacketType.CONTEXT

    def packet_specific_bits(self):
        # Bits:
        # (R)eserved
        # (N)ot a V49.0 packet
        # Timestamp Mode (TSM)
        indicator = 0
        if not self.is_v49_0:
            indicator |= 0x02
        indicator |= self.timestamp_mode
        return indicator

    @property
    def is_v49_0(self):
        # TBD: How to check?
        return False

class VRTCommandPacket(VRTCIFPacket):
    def __init__(self, name):
        super().__init__(name)
        self.acknowledge = False
        self.cancellation = False

    def packet_type(self):
        return PacketType.COMMAND

    def packet_specific_bits(self):
        # Bits:
        # (A)cknowledge
        # (R)eserved
        # Cance(L)lation
        indicator = 0
        if self.acknowledge:
            indicator |= 0x4
        if self.cancellation:
            indicator |= 0x1
        return indicator

    def is_v49_0(self):
        # Command packets were added in 49.2, so by definition cannot be 49.0
        return False
