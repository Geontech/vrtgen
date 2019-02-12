from enum import IntEnum
import struct

# TSI Code
# 00 - no integer timecode
# 01 - UTC
# 10 - GPS
# 11 - Other
class TSI(IntEnum):
    NONE  = 0
    UTC   = 1
    GPS   = 2
    OTHER = 3

# TSF Code
# 00 - no fractional timcode
# 01 - sample-count
# 10 - real-time (picoseconds)
# 11 - free-running count
class TSF(IntEnum):
    NONE         = 0
    SAMPLE_COUNT = 1
    REAL_TIME    = 2
    FREE_RUNNING = 3

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

class BitFormat:
    def __init__(self, bit):
        self.bit = bit

    def convert(self, value):
        return bool(value)

class IntFormat:
    def __init__(self, bits=32):
        self.bits = bits

    def convert(self, value):
        value = int(value)
        # TODO: range check
        return value

INT32 = IntFormat()

class FixedFormat(IntFormat):
    def __init__(self, bits, radix):
        super().__init__(bits)
        self.radix = radix

    def convert(self, value):
        # TODO: float-to-fixed conversion
        return float(value)

class EnumFormat:
    def __init__(self, values):
        self.values = values

    def convert(self, value):
        try:
            return self.values[value.lower()]
        except KeyError:
            raise ValueError("Invalid value '"+value+"'")

TSIFormat = EnumFormat({
    'none':  TSI.NONE,
    'utc':   TSI.UTC,
    'gps':   TSI.GPS,
    'other': TSI.OTHER
})

TSFFormat = EnumFormat({
    'none':         TSF.NONE,
    'samples':      TSF.SAMPLE_COUNT,
    'picoseconds':  TSF.REAL_TIME,
    'free running': TSF.FREE_RUNNING
})

class FieldDescriptor:
    DISABLED = 0
    OPTIONAL = 1
    REQUIRED = 2
    CONSTANT = 3

    __slots__ = ('name', 'enable_bit', '_enable_state', 'format', 'default_value')
    def __init__(self, name, enable_bit=None, format=None):
        self.name = name
        self._enable_state = FieldDescriptor.DISABLED
        self.enable_bit = enable_bit
        self.format = format
        self.default_value = None

    def match(self, name):
        return name.lower() == self.name.lower()

    def set_value(self, value):
        self.default_value = self.format.convert(value)

    @property
    def is_enabled(self):
        return self._enable_state != FieldDescriptor.DISABLED

    @property
    def is_set(self):
        if self.is_optional:
            return self.default_value is not None
        else:
            return self.is_enabled

    @property
    def is_required(self):
        return self._enable_state == FieldDescriptor.REQUIRED

    def set_required(self):
        self._enable_state = FieldDescriptor.REQUIRED

    @property
    def is_optional(self):
        return self._enable_state == FieldDescriptor.OPTIONAL

    def set_optional(self):
        self._enable_state = FieldDescriptor.OPTIONAL

    @property
    def is_constant(self):
        return self._enable_state == FieldDescriptor.CONSTANT

    def set_constant(self):
        self._enable_state = FieldDescriptor.CONSTANT

class FieldContainer:
    def __init__(self):
        self.__fields = []

    def add_field(self, *args, **kwargs):
        field = FieldDescriptor(*args, **kwargs)
        self.__fields.append(field)
        return field

    def get_field(self, name):
        for field in self.__fields:
            if field.match(name):
                return field
        return None

    @property
    def fields(self):
        return self.__fields

class VRTPrologue(FieldContainer):
    def __init__(self):
        super().__init__()
        self.stream_id = self.add_field('Stream ID', format=INT32)
        self.class_id = self.add_field('Class ID')
        self.integer_timestamp = self.add_field('TSI', format=TSIFormat)
        self.fractional_timestamp = self.add_field('TSF', format=TSFFormat)

class VRTPacket(object):
    def __init__(self, name):
        self.name = name
        self.prologue = VRTPrologue()

    @property
    def has_trailer(self):
        return False

    def get_field(self, name):
        field = self.prologue.get_field(name)
        if field is None:
            raise KeyError(name)
        return field

    def get_header_bytes(self):
        header = bytearray(4)

        header[0] = self.packet_type() << 4
        if self.prologue.class_id.is_set:
            header[0] |= 0x08
        header[0] |= self.packet_specific_bits()

        return header

    def add_field(self, name, optional=False):
        field = self.get_field(name)
        if field.is_enabled:
            raise ValueError('duplicate field')
        if optional:
            field.set_optional()
        else:
            field.set_required()
        return field

class VRTDataTrailer(FieldContainer):
    def __init__(self):
        super().__init__()
        self.calibrated_time = self.add_field('Calibrated Time', 31, BitFormat(19))
        self.valid_data = self.add_field('Valid Data', 30, BitFormat(18))
        self.reference_lock = self.add_field('Reference Lock', 29, BitFormat(17))
        self.agc_mgc = self.add_field('AGC/MGC', 28, BitFormat(16))
        self.detected_signal = self.add_field('Detected Signal', 27, BitFormat(15))
        self.spectral_inversion = self.add_field('Spectral Inversion', 26, BitFormat(14))
        self.over_range = self.add_field('Over-range', 25, BitFormat(13))
        self.sample_loss = self.add_field('Sample Loss', 24, BitFormat(12))
        # [23,22], [11,10] Sample Frame, User-Defined
        # [21..20], [9..8] User-Defined
        self.context_packet_count = self.add_field('Associated Context Packet Count', 7, IntFormat(7))

    def get_bytes(self):
        flag = 0
        for field in self.fields:
            if field.is_set:
                flag |= 1 << field.enable_bit
                if field.default_value:
                    flag |= 1 << field.format.bit
        return struct.pack('>I', flag)

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

    def get_field(self, name):
        field = self.trailer.get_field(name)
        if field is not None:
            return field
        return super().get_field(name)

class CIF0(FieldContainer):
    FIELDS = (
        ('Context Field Change Indicator', 31), # No data
        ('Reference Point Identifier', 30), # integer 32 (stream ID)
        ('Bandwidth', 29), # fixed-point 64/20, Hz
        ('IF Reference Frequency', 28), # fixed-point 64/20, Hz
        ('RF Reference Frequency', 27), # fixed-point 64/20, Hz
        ('RF Reference Frequency Offset', 26), # fixed-point 64/20, Hz
        ('IF Band Offset', 25), # fixed-point 64/20, Hz
        ('Reference Level', 24), # fixed-point 16/7 dBm (upper 16 reserved)
        ('Gain', 23), # [stage2 (optional), stage1]: fixed-point 16/7, dB
        ('Over-range Count', 22), # integer 32
        ('Sample Rate', 21), # fixed-point 64/20, Hz
        ('Timestamp Adjustment', 20), # fractional time (integer 64)
        ('Timestamp Calibration Time', 19), # 1 word, depends on prologue TSI
        ('Temperature', 18), # fixed-point 16/6, degrees C (upper 16 reserved)
        ('Device Identifier', 17), # 64 bits total, specific format
        ('State/Event Indicators', 16), # 32 bits, bit flags
        ('Signal Data Packet Payload Format', 15), # structured
        ('Formatted GPS', 14), # structured
        ('Formatted INS', 13), # structured
        ('ECEF Ephemeris', 12), # structured
        ('Relative Ephemeris', 11), # structured
        ('Ephemeris Ref ID', 10), # integer 32
        ('GPS ASCII', 9), # 2 word header plus arbitrary binary data
        ('Context Association Lists', 8)
        # Field Attributes Enable (CIF7)
        # Reserved
        # Reserved
        # Reserved
        # CIF 3 Enable
        # CIF 2 Enable
        # CIF 1 Enable
        # Reserved
    )

    def __init__(self):
        super().__init__()
        for name, bit in CIF0.FIELDS:
            self.add_field(name, bit)

    def get_prologue_bytes(self):
        prologue = 0
        for field in self.fields:
            if field.is_set:
                prologue |= 1 << field.enable_bit
        return struct.pack('>I', prologue)

class VRTContextPacket(VRTPacket):
    def __init__(self, name):
        super().__init__(name)
        self.prologue.stream_id.set_required()
        self.is_timestamp_mode = False

        # TODO: move this into a common location
        self.cif0 = CIF0()

    def get_header_bytes(self):
        base = super().get_header_bytes()
        return base + self.cif0.get_prologue_bytes()

    def packet_type(self):
        return PacketType.CONTEXT

    def get_field(self, name):
        field = self.cif0.get_field(name)
        if field is not None:
            return field
        return super().get_field(name)

    def packet_specific_bits(self):
        indicator = 0
        if not self.is_v49_0():
            indicator |= 0x02
        if self.is_timestamp_mode:
            indicator |= 0x01
        return indicator

    def is_v49_0(self):
        # TBD: How to check?
        return True

class VRTCommandPacket(VRTPacket):
    def __init__(self, name):
        super().__init__(name)
        self.prologue.stream_id.set_required()

    def packet_type(self):
        return PacketType.COMMAND

    def is_v49_0(self):
        # Command packets were added in 49.2, so by definition cannot be 49.0
        return False
