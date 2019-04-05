from enum import IntEnum
import struct

from .enums import *
from .field import *

def indicator_field(name, position):
    return field_descriptor(name, UnimplementedField, enable_bit=position)

class VRTHeader(FieldContainer):
    packet_type = field_descriptor('Packet Type', PacketTypeField, position=31)
    class_id = field_descriptor('Class Identifier', ClassIDField, enable_bit=27)
    tsi = field_descriptor('TSI', TSIField, position=23)
    tsf = field_descriptor('TSF', TSFField, position=21)
    packet_count = field_descriptor('Packet Count', IntegerField.create(4), position=19)
    packet_size = field_descriptor('Packet Size', Int16Field, position=15)

class VRTDataHeader(VRTHeader):
    trailer_included = indicator_field('Trailer Included', 26)
    not_v49_0 = indicator_field('Not a V49.0 Packet Indicator', 25)
    spectrum = indicator_field('Signal Spectrum', 24)

class VRTContextHeader(VRTHeader):
    # reserved
    not_v49_0 = indicator_field('Not a V49.0 Packet Indicator', 25)
    timestamp_mode = field_descriptor('Timestamp Mode', TSMField, position=24)

class VRTCommandHeader(VRTHeader):
    acknowledge = indicator_field('Acknowledge Packet', 26)
    # reserved
    cancellation = indicator_field('Cancellation Packet', 24)

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

    def validate(self):
        pass

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

    def get_prologue_bytes(self):
        header = bytearray(4)

        header[0] = self.packet_type() << 4
        if self.class_id.is_set:
            header[0] |= 0x08
        header[0] |= self.packet_specific_bits()
        header[1] = self.tsi << 6
        header[1] |= self.tsf << 4

        if self.tsi != TSI.NONE:
            ts = self.prologue.integer_timestamp.value
            if ts is None:
                ts = 0
            header += struct.pack('>I', ts)
        if self.tsf != TSF.NONE:
            ts = self.prologue.fractional_timestamp.value
            if ts is None:
                ts = 0
            header += struct.pack('>Q', ts)
        return header + self._get_prologue_bytes()

    def _get_prologue_bytes(self):
        return b''

    def validate(self):
        self.prologue.validate()

class VRTDataTrailer(FieldContainer):
    calibrated_time = field_descriptor('Calibrated Time', BitField, 31, 19)
    valid_data = field_descriptor('Valid Data', BitField, 30, 18)
    reference_lock = field_descriptor('Reference Lock', BitField, 29, 17)
    agc_mgc = field_descriptor('AGC/MGC', BitField, 28, 16)
    detected_signal = field_descriptor('Detected Signal', BitField, 27, 15)
    spectral_inversion = field_descriptor('Spectral Inversion', BitField, 26, 14)
    over_range = field_descriptor('Over-range', BitField, 25, 13)
    sample_loss = field_descriptor('Sample Loss', BitField, 24, 12)
    # The Sample Frame field was added in V49.2, replacing 2 user-defined
    # bits. While the bits can still be user-defined for compatibility with
    # V49.0 implementations, the spec strongly discourages it, and it is
    # not supported here.
    sample_frame = field_descriptor('Sample Frame', IntegerField.create(2), 23, 11)
    # [21..20], [9..8] User-Defined
    context_packet_count = field_descriptor('Associated Context Packet Count', IntegerField.create(7), 7, 6)

    def get_bytes(self):
        word = 0
        for field in self.fields:
            if not field.is_set:
                continue
            word |= field.enable_flag
            if field.value:
                # The enable and value bits are offset by 12
                value = int(field.value)
                word |= value << field.position
        return struct.pack('>I', word)

    @property
    def is_enabled(self):
        return any(field.is_enabled for field in self.fields)

    def validate(self):
        pass

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

    def validate(self):
        super().validate()
        self.trailer.validate()

class ContextIndicatorFields(FieldContainer):
    def get_prologue_bytes(self):
        prologue = 0
        for field in self.fields:
            if field.is_set:
                prologue |= 1 << field.enable_bit
        return struct.pack('>I', prologue)

    def validate(self):
        pass

class CIF0(ContextIndicatorFields):
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

class CIF1(ContextIndicatorFields):
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

    def _get_prologue_bytes(self):
        return b''.join(cif.get_prologue_bytes() for cif in self.cif)

    def get_field(self, name):
        for cif in self.cif:
            field = cif.get_field(name)
            if field is not None:
                return field
        return None

    def validate(self):
        for cif in self.cif:
            cif.validate()

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
