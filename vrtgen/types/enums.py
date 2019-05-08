"""
Enumeration types used in VITA 49.
"""
from enum import IntEnum, EnumMeta

class BinaryEnumMeta(EnumMeta):
    """
    Extended metaclass for binary enumerations, adding a default value for
    instance creation.
    """
    def __new__(cls, name, bases, namespace, bits=None, **kwds):
        # Discard keywords when calling EnumMeta's new method. Unlike type, it
        # does not accept keywords, which also means that the __init_subclass__
        # method will not receive keyword values (i.e., "bits").
        enum_class = super().__new__(cls, name, bases, namespace)
        if bits:
            enum_class.bits = bits
        return enum_class

    def __call__(cls, value=0, *args, **kwds):
        # Override the creation of enum instances to default to a value of 0
        return super().__call__(value, *args, **kwds)

    def __prepare__(name, bases, **kwds):
        # Discard keywords (i.e., "bits") for EnumMeta
        return EnumMeta.__prepare__(name, bases)

class BinaryEnum(IntEnum, metaclass=BinaryEnumMeta):
    """
    Base class for integer-valued enumerations with a known bit size.
    """
    # This class ties together IntEnum and our custom metaclass, but does not
    # add any functionality (see above re: __init_subclass__)
    pass

class PacketType(BinaryEnum, bits=4):
    """
    Constants for the 4-bit Packet Type field in the VRT Packet Header.
    Refer to VITA 49.2 Table 5.1.1-1.
    """
    SIGNAL_DATA = 0b0000
    SIGNAL_DATA_STREAM_ID = 0b0001
    EXTENSION_DATA = 0b0010
    EXTENSION_DATA_STREAM_ID = 0b0011
    CONTEXT = 0b0100
    EXTENSION_CONTEXT = 0b0101
    COMMAND = 0b0110
    EXTENSION_COMMAND = 0b0111
    # 1000-1111 reserved for future VRT Packet types
    RESERVED_8 = 0b1000
    RESERVED_9 = 0b1001
    RESERVED_10 = 0b1010
    RESERVED_11 = 0b1011
    RESERVED_12 = 0b1100
    RESERVED_13 = 0b1101
    RESERVED_14 = 0b1110
    RESERVED_15 = 0b1111

class TSI(BinaryEnum, bits=2):
    """
    TimeStamp-Integer (TSI) codes:
        NONE  (00) - No Integer-seconds Timestamp field included
        UTC   (01) - Coordinated Universal Time
        GPS   (10) - GPS time
        OTHER (11) - Other, must be documented
    """
    NONE = 0b00
    UTC = 0b01
    GPS = 0b10
    OTHER = 0b11

class TSF(BinaryEnum, bits=2):

    """
    TimeStamp-Fractional (TSF) codes:
        NONE         (00) - No Fractional-seconds Timestamp field included
        SAMPLE_COUNT (01) - Sample Count Timestamp
        REAL_TIME    (10) - Real-Time (Picoseconds) Timestamp
        FREE_RUNNING (11) - Free Running Count Timestamp
    """
    NONE = 0b00
    SAMPLE_COUNT = 0b01
    REAL_TIME = 0b10
    FREE_RUNNING = 0b11

class TSM(BinaryEnum, bits=1):
    """
    Timestamp Mode (TSM) of context packets:
        FINE   (0) - Timestamp conveys precise timing of events related to the
                     Described Signal
        COARSE (1) - Timestamp conveys general timing of events related to the
                     Described Signal
    """
    FINE = 0
    COARSE = 1

class SSI(BinaryEnum, bits=2):
    """
    Start/Stop of Sample Frame Indication (SSI) Bits:
        SINGLE (00) - Sample Frames are not applicable to data packets, or the
                      entire Sample Frame is contained in a single data packet
        FIRST  (01) - First data packet of current Sample Frame
        MIDDLE (10) - Middle packet or packets of Sample Frame: "continuation"
                      indicator
        FINAL  (11) - Final data packet of current Sample Frame
    """
    SINGLE = 0b00
    FIRST = 0b01
    MIDDLE = 0b10
    FINAL = 0b11

class PackingMethod(BinaryEnum, bits=1):
    """
    Data Format Packing Method flag:
        PROCESSING_EFFICIENT (0) - Items are padded as necessary such that they
                                   do not span 32-bit word boundaries
        LINK_EFFICIENT       (1) - No padding of items
    """
    PROCESSING_EFFICIENT = 0
    LINK_EFFICIENT = 1

class DataSampleType(BinaryEnum, bits=2):
    """
    Data Sample real/complex type:
        REAL              (00) - Real
        COMPLEX_CARTESIAN (01) - Complex, Cartesian
        COMPLEX_POLAR     (10) - Complex, Polar
                          (11) - Reserved
    """
    REAL = 0b00
    COMPLEX_CARTESIAN = 0b01
    COMPLEX_POLAR = 0b10
    RESERVED = 0b11

class DataItemFormat(BinaryEnum, bits=5):
    """
    Data Item Format codes:
        SIGNED_FIXED                  (00000) - Signed Fixed-Point
        SIGNED_VRT_1                  (00001) - Signed VRT, 1-bit exponent
        SIGNED_VRT_2                  (00001) - Signed VRT, 2-bit exponent
        SIGNED_VRT_3                  (00001) - Signed VRT, 3-bit exponent
        SIGNED_VRT_4                  (00001) - Signed VRT, 4-bit exponent
        SIGNED_VRT_5                  (00001) - Signed VRT, 5-bit exponent
        SIGNED_VRT_6                  (00001) - Signed VRT, 6-bit exponent
        SIGNED_FIXED_NON_NORMALIZED   (00111) - Signed Fixed-Point Non-Normalized
                                      (01000) - Reserved
                                      (01001) - Reserved
                                      (01010) - Reserved
                                      (01011) - Reserved
                                      (01100) - Reserved
        IEEE754_HALF_PRECISION        (01101) - IEEE-754 Half-Precision Floating-Point
        IEEE754_SINGLE_PRECISION      (01110) - IEEE-754 Single-Precision Floating-Point
        IEEE754_DOUBLE_PRECISION      (01111) - IEEE-754 Double-Precision Floating-Point
        UNSIGNED_FIXED                (10000) - Unsigned Fixed-Point
        UNSIGNED_VRT_1                (10001) - Unsigned VRT, 1-bit exponent
        UNSIGNED_VRT_2                (10001) - Unsigned VRT, 2-bit exponent
        UNSIGNED_VRT_3                (10001) - Unsigned VRT, 3-bit exponent
        UNSIGNED_VRT_4                (10001) - Unsigned VRT, 4-bit exponent
        UNSIGNED_VRT_5                (10001) - Unsigned VRT, 5-bit exponent
        UNSIGNED_VRT_6                (10001) - Unsigned VRT, 6-bit exponent
        UNSIGNED_FIXED_NON_NORMALIZED (10111) - Unsigned Fixed-Point Non-Normalized
                                      (11000) - Reserved
                                      (11001) - Reserved
                                      (11010) - Reserved
                                      (11011) - Reserved
                                      (11100) - Reserved
                                      (11101) - Reserved
                                      (11110) - Reserved
                                      (11111) - Reserved
    """
    SIGNED_FIXED = 0b00000
    SIGNED_VRT_1 = 0b00001
    SIGNED_VRT_2 = 0b00010
    SIGNED_VRT_3 = 0b00011
    SIGNED_VRT_4 = 0b00100
    SIGNED_VRT_5 = 0b00101
    SIGNED_VRT_6 = 0b00110
    SIGNED_FIXED_NON_NORMALIZED = 0b00111
    RESERVED_8 = 0b01000
    RESERVED_9 = 0b01001
    RESERVED_10 = 0b01010
    RESERVED_11 = 0b01011
    RESERVED_12 = 0b01100
    IEEE754_HALF_PRECISION = 0b01101
    IEEE754_SINGLE_PRECISION = 0b01110
    IEEE754_DOUBLE_PRECISION = 0b01111
    UNSIGNED_FIXED = 0b10000
    UNSIGNED_VRT_1 = 0b10001
    UNSIGNED_VRT_2 = 0b10010
    UNSIGNED_VRT_3 = 0b10011
    UNSIGNED_VRT_4 = 0b10100
    UNSIGNED_VRT_5 = 0b10101
    UNSIGNED_VRT_6 = 0b10110
    UNSIGNED_FIXED_NON_NORMALIZED = 0b10111
    RESERVED_24 = 0b11000
    RESERVED_25 = 0b11001
    RESERVED_26 = 0b11010
    RESERVED_27 = 0b11011
    RESERVED_28 = 0b11100
    RESERVED_29 = 0b11101
    RESERVED_30 = 0b11110
    RESERVED_31 = 0b11111

    @property
    def is_signed(self):
        """
        True if this data format includes a sign bit.
        """
        return not bool(self.value & 0b10000)
