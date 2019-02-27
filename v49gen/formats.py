from enum import IntEnum

class TSI(IntEnum):
    """
    TimeStamp-Integer (TSI) codes:
        NONE  (00) - No Integer-seconds Timestamp field included
        UTC   (01) - Coordinated Universal Time
        GPS   (10) - GPS time
        OTHER (11) - Other, must be documented
    """
    NONE  = 0b00
    UTC   = 0b01
    GPS   = 0b10
    OTHER = 0b11

class TSF(IntEnum):

    """
    TimeStamp-Fractional (TSF) codes:
        NONE         (00) - No Fractional-seconds Timestamp field included
        SAMPLE_COUNT (01) - Sample Count Timestamp
        REAL_TIME    (10) - Real-Time (Picoseconds) Timestamp
        FREE_RUNNING (11) - Free Running Count Timestamp
    """
    NONE         = 0b00
    SAMPLE_COUNT = 0b01
    REAL_TIME    = 0b10
    FREE_RUNNING = 0b11

class TSM(IntEnum):
    """
    Timestamp Mode (TSM) of context packets:
        FINE   (0) - Timestamp conveys precise timing of events related to the
                     Described Signal
        COARSE (1) - Timestamp conveys general timing of events related to the
                     Described Signal
    """
    FINE   = 0
    COARSE = 1

class SSI(IntEnum):
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
    FIRST  = 0b01
    MIDDLE = 0b10
    FINAL  = 0b11

class PackingMethod(IntEnum):
    """
    Data Format Packing Method flag:
        PROCESSING_EFFICIENT (0) - Items are padded as necessary such that they
                                   do not span 32-bit word boundaries
        LINK_EFFICIENT       (1) - No padding of items
    """
    PROCESSING_EFFICIENT = 0
    LINK_EFFICIENT       = 1

class DataSampleType(IntEnum):
    """
    Data Sample real/complex type:
        REAL              (00) - Real
        COMPLEX_CARTESIAN (01) - Complex, Cartesian
        COMPLEX_POLAR     (10) - Complex, Polar
                          (11) - Reserved
    """
    REAL              = 0b00
    COMPLEX_CARTESIAN = 0b01
    COMPLEX_POLAR     = 0b10

class DataItemFormat(IntEnum):
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
    SIGNED_FIXED                  = 0b00000
    SIGNED_VRT_1                  = 0b00001
    SIGNED_VRT_2                  = 0b00010
    SIGNED_VRT_3                  = 0b00011
    SIGNED_VRT_4                  = 0b00100
    SIGNED_VRT_5                  = 0b00101
    SIGNED_VRT_6                  = 0b00110
    SIGNED_FIXED_NON_NORMALIZED   = 0b00111
    IEEE754_HALF_PRECISION        = 0b01101
    IEEE754_SINGLE_PRECISION      = 0b01110
    IEEE754_DOUBLE_PRECISION      = 0b01111
    UNSIGNED_FIXED                = 0b10000
    UNSIGNED_VRT_1                = 0b10001
    UNSIGNED_VRT_2                = 0b10010
    UNSIGNED_VRT_3                = 0b10011
    UNSIGNED_VRT_4                = 0b10100
    UNSIGNED_VRT_5                = 0b10101
    UNSIGNED_VRT_6                = 0b10110
    UNSIGNED_FIXED_NON_NORMALIZED = 0b10111

    @property
    def is_signed(self):
        return not bool(self.value & 0b10000)
