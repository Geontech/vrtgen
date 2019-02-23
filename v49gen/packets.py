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
    DISABLED = 0
    OPTIONAL = 1
    REQUIRED = 2
    CONSTANT = 3

class FieldDescriptor:
    __slots__ = ('name', 'enable_bit', '_enable_state', 'format', 'value')
    def __init__(self, name, enable_bit=None, format=None):
        self.name = name
        self._enable_state = Field.DISABLED
        self.enable_bit = enable_bit
        self.format = format
        self.value = None

    def match(self, name):
        return name.lower() == self.name.lower()

    def set_value(self, value):
        self.value = value

    @property
    def is_enabled(self):
        return self._enable_state != Field.DISABLED

    @property
    def is_set(self):
        if self.is_optional:
            return self.value is not None
        else:
            return self.is_enabled

    @property
    def is_required(self):
        return self._enable_state == Field.REQUIRED

    def set_required(self):
        self._enable_state = Field.REQUIRED

    @property
    def is_optional(self):
        return self._enable_state == Field.OPTIONAL

    def set_optional(self):
        self._enable_state = Field.OPTIONAL

    @property
    def is_constant(self):
        return self._enable_state == Field.CONSTANT

    def set_constant(self):
        self._enable_state = Field.CONSTANT

class FieldContainer:
    def __init__(self):
        self.__fields = []
        for name, field in inspect.getmembers(self, lambda x: isinstance(x, FieldDescriptor)):
            self.__fields.append(field)

    def get_field(self, name):
        for field in self.__fields:
            if field.match(name):
                return field
        return None

    @property
    def fields(self):
        return self.__fields

class StructFieldDescriptor(FieldDescriptor, FieldContainer):
    def __init__(self, *args, **kwargs):
        FieldDescriptor.__init__(self, *args, **kwargs)
        FieldContainer.__init__(self)

class ClassID(StructFieldDescriptor):
    oui = FieldDescriptor('OUI', format=IntFormat(24))
    information_class = FieldDescriptor('Information Class Code', format=INT16)
    packet_class = FieldDescriptor('Packet Class Code', format=INT16)

    def __init__(self):
        super().__init__('Class ID')

class TSIField(FieldDescriptor):
    __slots__ = ('mode',)
    def __init__(self):
        super().__init__('TSI', format=INT32)
        self.mode = TSI.NONE

class TSFField(FieldDescriptor):
    __slots__ = ('mode',)
    def __init__(self):
        super().__init__('TSF', format=INT64)
        self.mode = TSF.NONE

class VRTPrologue(FieldContainer):
    stream_id = FieldDescriptor('Stream ID', format=INT32)
    class_id = ClassID()
    integer_timestamp = TSIField()
    fractional_timestamp = TSFField()

    def __init__(self):
        super().__init__()

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
        return self.prologue.integer_timestamp

    @property
    def tsf(self):
        return self.prologue.fractional_timestamp

    def get_header_bytes(self):
        header = bytearray(4)

        header[0] = self.packet_type() << 4
        if self.class_id.is_set:
            header[0] |= 0x08
        header[0] |= self.packet_specific_bits()
        header[1] = self.tsi.mode << 6
        header[1] |= self.tsf.mode << 4

        return header

class VRTDataTrailer(FieldContainer):
    calibrated_time = FieldDescriptor('Calibrated Time', 31, BIT)
    valid_data = FieldDescriptor('Valid Data', 30, BIT)
    reference_lock = FieldDescriptor('Reference Lock', 29, BIT)
    agc_mgc = FieldDescriptor('AGC/MGC', 28, BIT)
    detected_signal = FieldDescriptor('Detected Signal', 27, BIT)
    spectral_inversion = FieldDescriptor('Spectral Inversion', 26, BIT)
    over_range = FieldDescriptor('Over-range', 25, BIT)
    sample_loss = FieldDescriptor('Sample Loss', 24, BIT)
    # The Sample Frame field was added in V49.2, replacing 2 user-defined
    # bits. While the bits can still be user-defined for compatibility with
    # V49.0 implementations, the spec strongly discourages it, and it is
    # not supported here.
    sample_frame = FieldDescriptor('Sample Frame', 23, IntFormat(2))
    # [21..20], [9..8] User-Defined
    context_packet_count = FieldDescriptor('Associated Context Packet Count', 7, IntFormat(7))

    def get_bytes(self):
        word = 0
        for field in self.fields:
            if not field.is_set:
                continue
            word |= 1 << field.enable_bit
            # Sample Frame has two enable bits because it was taken from
            # user-defined bits; set both here
            if field == self.sample_frame:
                word |= 1 << field.enable_bit - 1
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
        return True

    @property
    def has_trailer(self):
        return self.trailer.is_enabled

class CIF0(FieldContainer):
    # Context Field Change Indicator (0/31) is a binary flag that can be
    # set at run-time. No configuration is possible.

    # Reference Point Identifier (0/30): integer 32 (stream ID)
    reference_point_id = FieldDescriptor('Reference Point Identifier', 30, INT32)

    # Bandwidth (0/29) fixed-point 64/20, Hz
    bandwidth = FieldDescriptor('Bandwidth', 29, FixedFormat(64, 20))

    # fixed-point 64/20, Hz
    if_frequency = FieldDescriptor('IF Reference Frequency', 28, FixedFormat(64, 20))

    # fixed-point 64/20, Hz
    rf_frequency = FieldDescriptor('RF Reference Frequency', 27, FixedFormat(64, 20))

    # fixed-point 64/20, Hz
    rf_frequency_offset = FieldDescriptor('RF Reference Frequency Offset', 26, FixedFormat(64, 20))

    # fixed-point 64/20, Hz
    if_band_offset = FieldDescriptor('IF Band Offset', 25, FixedFormat(64, 20))

    # fixed-point 16/7 dBm (upper 16 reserved)
    reference_level = FieldDescriptor('Reference Level', 24, FixedFormat(16, 7))

    # [stage2 (optional), stage1]: fixed-point 16/7, dB
    gain = FieldDescriptor('Gain', 23, None)

    # integer 32
    over_range_count = FieldDescriptor('Over-range Count', 22, INT32)

    # fixed-point 64/20, Hz
    sample_rate = FieldDescriptor('Sample Rate', 21, FixedFormat(64, 20))

    # fractional time (integer 64)
    timestamp_adjustment = FieldDescriptor('Timestamp Adjustment', 20, INT64)

    # 1 word, depends on prologue TSI
    timestamp_calibration_time = FieldDescriptor('Timestamp Calibration Time', 19, INT32)

    # fixed-point 16/6, degrees C (upper 16 reserved)
    temperature = FieldDescriptor('Temperature', 18, FixedFormat(16, 6))

    # 64 bits total, specific format
    device_id = FieldDescriptor('Device Identifier', 17, None)

    # 32 bits, bit flags
    state_event_indicators = FieldDescriptor('State/Event Indicators', 16, None)

    # structured
    data_format = FieldDescriptor('Signal Data Packet Payload Format', 15, None)

    # structured
    formatted_gps = FieldDescriptor('Formatted GPS', 14, None)

    # structured
    formatted_ins = FieldDescriptor('Formatted INS', 13, None)

    # structured
    ecef_ephemeris = FieldDescriptor('ECEF Ephemeris', 12, None)

    # structured
    relative_ephemeris = FieldDescriptor('Relative Ephemeris', 11, None)

    # integer 32
    ephemeris_ref_id = FieldDescriptor('Ephemeris Ref ID', 10, INT32)

    # 2 word header plus arbitrary binary data
    gps_ascii = FieldDescriptor('GPS ASCII', 9, None)

    context_association_lists = ('Context Association Lists', 8, None)

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

class CIF1(FieldContainer):
    # Phase Offset (1/31): fixed-point 16/7, radians (upper 16 reserved)
    phase_offset = FieldDescriptor('Phase Offset', 31, FixedFormat(16, 7))

    # Polarization (1/30): [tilt, ellipticity], fixed-point 16/13, radians
    polarization = FieldDescriptor('Polarization', 30, None)

    # 3-D Pointing Vector (1/29): [elevation, azimuthal], fixed-point 16/7, degrees
    pointing_vector = FieldDescriptor('3-D Pointing Vector', 29, None)

    # 3-D Pointing Vector Structure (1/28): structured data
    pointing_vector_struct = FieldDescriptor('3-D Pointing Vector Structure', 28, None)

    # Spatial Scan Type (1/27): Generic 16-bit identifier
    spatial_scan_type = FieldDescriptor('Spatial Scan Type', 27, INT16)

    # Spatial Reference Type (1/26): struct
    spatial_reference_type = FieldDescriptor('Spatial Reference Type', 26, None)

    # Beam Widths (1/25): [horizonal, vertical]: fixed-point 16/7. degrees
    beam_widths = FieldDescriptor('Beam Widths', 25, None)

    # Range (1/24): fixed-point 32/6, meters
    range = FieldDescriptor('Range', 24, FixedFormat(32, 6))

    # Reserved (1/23)
    # Reserved (1/22)
    # Reserved (1/21)

    # Eb/No BER (1/20): [Eb/No, BER], fixed-point 16/7, dB
    ebno_ber = FieldDescriptor('Eb/No BER', 20, None)

    # Threshold (1/18): [stage2 (optional), stage1], fixed-point 16/7, dB
    threshold = FieldDescriptor('Threshold', 19, None)

    # Compression Point (1/18)
    compression_point = FieldDescriptor('Compression Point', 18, FixedFormat(16, 7))

    # Intercept Points (1/17): [2IIP, 3IIP], fixed-point 16/7, dBm
    intercept_points = FieldDescriptor('Intercept Points', 17, None)

    # SNR/Noise Figure (1/16): [SNR, Noise], fixed-point 16/7, dB
    snr_noise_figure = FieldDescriptor('SNR/Noise Figure', 16, None)

    # Aux Frequency (1/15)
    aux_frequency = FieldDescriptor('Aux Frequency', 15, FixedFormat(64, 20))

    # Aux Gain (1/14): [stage2 (optional), stage1]: fixed-point 16/7, dB
    aux_gain = FieldDescriptor('Aux Gain', 14, None)

    # Aux Bandidth (1/13)
    aux_bandwidth = FieldDescriptor('Aux Bandwidth', 13, FixedFormat(64, 20))

    # Reserved (1/12)

    # Array of CIFS (1/11): This allows multiple CIF blocks, wnich makes for
    # some complex support code.
    array_of_cifs = FieldDescriptor('Array of CIFS', 11, None)

    # Spectrum (1/10)
    spectrum = FieldDescriptor('Spectrum', 10, None)

    # Sector Scan/Step (1/9)
    sector_scan_step = FieldDescriptor('Sector Scan/Step', 9, None),

    # Reserved (1/8)

    # Index List (1/7): array of structs
    FieldDescriptor('Index List', 7, None)

    # Discrete I/O 32-bit (1/6): 32 additional bits of user-defined fields
    FieldDescriptor('Discrete I/O 32', 6, None)

    # Discrete I/O 64-bit (1/7): 64 additional bits of user-defined fields
    FieldDescriptor('Discrete I/O 64', 5, None), # 64 user-defined bits

    # Health Status (1/4): 16-bit identifier
    FieldDescriptor('Health Status', 4, INT16)

    # V49 Spec Compliance (1/3): 32 bits for V49 compliance level, only four
    # values currently defined.

    # Version and Build Code (1/2): struct, 32 bits
    version_build_code = FieldDescriptor('Version and Build Code', 2, None)

    # Buffer Size (1/1): struct, 64 bits
    buffer_size = FieldDescriptor('Buffer Size', 1, None)

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
        self.stream_id.set_required()
        self.cif = [CIF0(), CIF1()]

    def get_header_bytes(self):
        base = super().get_header_bytes()
        return base + self.cif[0].get_prologue_bytes()

    def get_field(self, name):
        for cif in self.cif:
            field = cif.get_field(name)
            if field is not None:
                return field
        return super().get_field(name)

class VRTContextPacket(VRTCIFPacket):
    def __init__(self, name):
        super().__init__(name)
        self.is_timestamp_mode = False

    def packet_type(self):
        return PacketType.CONTEXT

    def packet_specific_bits(self):
        # Bits:
        # (R)eserved
        # (N)ot a V49.0 packet
        # Timestamp Mode (TSM)
        indicator = 0
        if not self.is_v49_0():
            indicator |= 0x02
        if self.is_timestamp_mode:
            indicator |= 0x01
        return indicator

    def is_v49_0(self):
        # TBD: How to check?
        return True

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
