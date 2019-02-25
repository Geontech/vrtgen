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

    __slots__ = ('_enable')
    def __init__(self):
        self._enable = Field.DISABLED

    @property
    def is_set(self):
        if self.has_value:
            return True
        return self._enable in (Field.REQUIRED, Field.CONSTANT)

    @property
    def has_value(self):
        return False

    @property
    def is_enabled(self):
        return self._enable != Field.DISABLED

    @property
    def is_required(self):
        return self._enable == Field.REQUIRED

    def set_required(self):
        self._enable = Field.REQUIRED

    @property
    def is_optional(self):
        return self._enable == Field.OPTIONAL

    def set_optional(self):
        self._enable = Field.OPTIONAL

    @property
    def is_constant(self):
        return self._enable == Field.CONSTANT

    def set_constant(self):
        self._enable = Field.CONSTANT

    def match(self, name):
        # Relying on a little bit of class trickery here: Field does not have
        # a "name" attribute, but it's never directly used. field_descriptor()
        # creates derived classes with the name set as a class attribute (this
        # is also a minor space optimization, since the name is the same for
        # all instances).
        return self.name.casefold() == name.casefold()

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

class TSIField(Int32Field):
    __slots__ = ('mode',)
    def __init__(self):
        super().__init__()
        self.mode = TSI.NONE

class TSFField(Int64Field):
    __slots__ = ('mode',)
    def __init__(self):
        super().__init__()
        self.mode = TSF.NONE

class GainField(StructField):
    stage1 = field_descriptor('Stage 1', FixedPointField.create(16, 7))
    stage2 = field_descriptor('Stage 2', FixedPointField.create(16, 7))

class DeviceIDField(StructField):
    manufacturer_oui = field_descriptor('Manufacturer OUI', OUIField)
    device_code = field_descriptor('Device Code', Int16Field)

class VRTPrologue(FieldContainer):
    stream_id = field_descriptor('Stream ID', Int32Field)
    class_id = field_descriptor('Class ID', ClassIDField)
    integer_timestamp = field_descriptor('TSI', TSIField)
    fractional_timestamp = field_descriptor('TSF', TSFField)

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
    reference_point_id = field_descriptor('Reference Point Identifier', Int32Field, 30)

    # Bandwidth (0/29) fixed-point 64/20, Hz
    bandwidth = field_descriptor('Bandwidth', FixedPointField.create(64, 20), 29)

    # fixed-point 64/20, Hz
    if_frequency = field_descriptor('IF Reference Frequency', FixedPointField.create(64, 20), 28)

    # fixed-point 64/20, Hz
    rf_frequency = field_descriptor('RF Reference Frequency', FixedPointField.create(64, 20), 27)

    # fixed-point 64/20, Hz
    rf_frequency_offset = field_descriptor('RF Reference Frequency Offset', FixedPointField.create(64, 20), 26)

    # fixed-point 64/20, Hz
    if_band_offset = field_descriptor('IF Band Offset', FixedPointField.create(64, 20), 25)

    # fixed-point 16/7 dBm (upper 16 reserved)
    reference_level = field_descriptor('Reference Level', FixedPointField.create(16, 7), 24)

    # [stage2 (optional), stage1]: fixed-point 16/7, dB
    gain = field_descriptor('Gain', GainField, 23)

    # integer 32
    over_range_count = field_descriptor('Over-range Count', Int32Field, 22)

    # fixed-point 64/20, Hz
    sample_rate = field_descriptor('Sample Rate', FixedPointField.create(64, 20), 21)

    # fractional time (integer 64)
    timestamp_adjustment = field_descriptor('Timestamp Adjustment', Int64Field, 20)

    # 1 word, depends on prologue TSI
    timestamp_calibration_time = field_descriptor('Timestamp Calibration Time', Int32Field, 19)

    # fixed-point 16/6, degrees C (upper 16 reserved)
    temperature = field_descriptor('Temperature', FixedPointField.create(16, 6), 18)

    # 64 bits total, specific format
    device_id = field_descriptor('Device Identifier', DeviceIDField, 17)

    # 32 bits, bit flags
    state_event_indicators = field_descriptor('State/Event Indicators', UnimplementedField, 16)

    # structured
    data_format = field_descriptor('Signal Data Packet Payload Format', UnimplementedField, 15)

    # structured
    formatted_gps = field_descriptor('Formatted GPS', UnimplementedField, 14)

    # structured
    formatted_ins = field_descriptor('Formatted INS', UnimplementedField, 13)

    # structured
    ecef_ephemeris = field_descriptor('ECEF Ephemeris', UnimplementedField, 12)

    # structured
    relative_ephemeris = field_descriptor('Relative Ephemeris', UnimplementedField, 11)

    # integer 32
    ephemeris_ref_id = field_descriptor('Ephemeris Ref ID', Int32Field, 10)

    # 2 word header plus arbitrary binary data
    gps_ascii = field_descriptor('GPS ASCII', UnimplementedField, 9)

    context_association_lists = ('Context Association Lists', 8)

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
    phase_offset = field_descriptor('Phase Offset', FixedPointField.create(16, 7), 31)

    # Polarization (1/30): [tilt, ellipticity], fixed-point 16/13, radians
    polarization = field_descriptor('Polarization', UnimplementedField, 30)

    # 3-D Pointing Vector (1/29): [elevation, azimuthal], fixed-point 16/7, degrees
    pointing_vector = field_descriptor('3-D Pointing Vector', UnimplementedField, 29)

    # 3-D Pointing Vector Structure (1/28): structured data
    pointing_vector_struct = field_descriptor('3-D Pointing Vector Structure', UnimplementedField, 28)

    # Spatial Scan Type (1/27): Generic 16-bit identifier
    spatial_scan_type = field_descriptor('Spatial Scan Type', Int16Field, 27)

    # Spatial Reference Type (1/26): struct
    spatial_reference_type = field_descriptor('Spatial Reference Type', UnimplementedField, 26)

    # Beam Widths (1/25): [horizonal, vertical]: fixed-point 16/7. degrees
    beam_widths = field_descriptor('Beam Widths', UnimplementedField, 25)

    # Range (1/24): fixed-point 32/6, meters
    range = field_descriptor('Range', FixedPointField.create(32, 6), 24)

    # Reserved (1/23)
    # Reserved (1/22)
    # Reserved (1/21)

    # Eb/No BER (1/20): [Eb/No, BER], fixed-point 16/7, dB
    ebno_ber = field_descriptor('Eb/No BER', UnimplementedField, 20)

    # Threshold (1/18): [stage2 (optional), stage1], fixed-point 16/7, dB
    threshold = field_descriptor('Threshold', UnimplementedField, 19)

    # Compression Point (1/18)
    compression_point = field_descriptor('Compression Point', FixedPointField.create(16, 7), 18)

    # Intercept Points (1/17): [2IIP, 3IIP], fixed-point 16/7, dBm
    intercept_points = field_descriptor('Intercept Points', UnimplementedField, 17)

    # SNR/Noise Figure (1/16): [SNR, Noise], fixed-point 16/7, dB
    snr_noise_figure = field_descriptor('SNR/Noise Figure', UnimplementedField, 16)

    # Aux Frequency (1/15)
    aux_frequency = field_descriptor('Aux Frequency', FixedPointField.create(64, 20), 15)

    # Aux Gain (1/14): [stage2 (optional), stage1]: fixed-point 16/7, dB
    aux_gain = field_descriptor('Aux Gain', GainField, 14)

    # Aux Bandidth (1/13)
    aux_bandwidth = field_descriptor('Aux Bandwidth', FixedPointField.create(64, 20), 13)

    # Reserved (1/12)

    # Array of CIFS (1/11): This allows multiple CIF blocks, wnich makes for
    # some complex support code.
    array_of_cifs = field_descriptor('Array of CIFS', UnimplementedField, 11)

    # Spectrum (1/10)
    spectrum = field_descriptor('Spectrum', UnimplementedField, 10)

    # Sector Scan/Step (1/9)
    sector_scan_step = field_descriptor('Sector Scan/Step', UnimplementedField, 9),

    # Reserved (1/8)

    # Index List (1/7): array of structs
    field_descriptor('Index List', UnimplementedField, 7)

    # Discrete I/O 32-bit (1/6): 32 additional bits of user-defined fields
    field_descriptor('Discrete I/O 32', UnimplementedField, 6)

    # Discrete I/O 64-bit (1/7): 64 additional bits of user-defined fields
    field_descriptor('Discrete I/O 64', UnimplementedField, 5), # 64 user-defined bits

    # Health Status (1/4): 16-bit identifier
    field_descriptor('Health Status', Int16Field, 4)

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
