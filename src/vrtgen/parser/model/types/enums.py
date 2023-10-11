# Copyright (C) 2019 Geon Technologies, LLC
#
# This file is part of vrtgen.
#
# vrtgen is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# vrtgen is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
# more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.

"""
Enumeration types used in VITA 49.
"""
from enum import Enum, IntEnum, EnumMeta, auto

class Mode(Enum):
    """
    Defines the usage of a field or subfield:
        DISABLED: Field is never present.
        OPTIONAL: Field can be enabled or disabled at runtime.
        REQUIRED: Field is always present.
        MANDATORY: Field is present and cannot and be disabled or optional.
    """
    DISABLED = auto()
    OPTIONAL = auto()
    REQUIRED = auto()
    MANDATORY = auto()

class BinaryEnumMeta(EnumMeta):
    """
    Extended metaclass for binary enumerations, adding a default value for
    instance creation.
    """
    #pylint: disable=arguments-differ
    def __new__(cls, name, bases, namespace, **kwds):
        # Discard keywords when calling EnumMeta's new method. Unlike type, it
        # does not accept keywords, which also means that the __init_subclass__
        # method will not receive keyword values (i.e., "bits").
        enum_class = super().__new__(cls, name, bases, namespace)
        # Dispatch to __init_subclass__ with the expected keywords. This is a
        # minor deviation from the way it normally works, because it looks up
        # __init_subclass__ in the class as opposed to its first base class,
        # but works normally as long as only the base BinaryEnum class defines
        # __init_subclass__.
        getattr(enum_class, '__init_subclass__')(**kwds)
        return enum_class

    def __call__(cls, *args, **kwds):
        # Override the creation of enum instances to default to a value of 0
        # when called with no arguments
        if not args and not kwds:
            args = (0,)
        return super().__call__(*args, **kwds)

    def __prepare__(name, bases, **kwds): #pylint: disable=bad-mcs-method-argument
        # Discard keywords (i.e., "bits") for EnumMeta
        del kwds
        return EnumMeta.__prepare__(name, bases)

class BinaryEnum(IntEnum, metaclass=BinaryEnumMeta):
    """
    Base class for integer-valued enumerations with a known bit size.
    """
    # This class ties together IntEnum and our custom metaclass
    def __init_subclass__(cls, bits=None, **kwds):
        # This method will be called twice--first by EnumMeta.__new__ without
        # keywords, and then by BinaryEnumMeta.__new__ with keywords. This is
        # necessary to work around the lack of keyword support in EnumMeta.
        # If bits is None, assume that it's the first call and return early.
        if bits is None:
            return
        super().__init_subclass__(**kwds)
        cls.bits = bits

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

    @property
    def is_data(self):
        """
        True if this Packet Type code represents a Data Packet.
        """
        return self in (
            PacketType.SIGNAL_DATA,
            PacketType.SIGNAL_DATA_STREAM_ID,
            PacketType.EXTENSION_DATA,
            PacketType.EXTENSION_DATA_STREAM_ID,
        )

    @property
    def is_context(self):
        """
        True if this Packet Type code represents a Context Packet.
        """
        return self in (
            PacketType.CONTEXT,
            PacketType.EXTENSION_CONTEXT,
        )


    @property
    def is_command(self):
        """
        True if this Packet Type code represents a Command Packet.
        """
        return self in (
            PacketType.COMMAND,
            PacketType.EXTENSION_COMMAND,
        )

    @property
    def is_extension(self):
        """
        True if this Packet Type code represents and Extension Packet Type
        (Data, Context or Command).
        """
        return self in (
            PacketType.EXTENSION_DATA,
            PacketType.EXTENSION_DATA_STREAM_ID,
            PacketType.EXTENSION_CONTEXT,
            PacketType.EXTENSION_COMMAND,
        )

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
    ANY = 0b111

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
    ANY = 0b111

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

class SPECTRUM_OR_TIME(BinaryEnum, bits=1):
    """
    S Bit (S_BIT) of data packets:
        SPECTRUM (0) - Conveys that the vector contains spectrum data
        TIME     (1) - Conveys that the vector contains time data
    """
    TIME = 0
    SPECTRUM = 1

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

class AGCMode(BinaryEnum, bits=1):
    """
    AGC/MGC Indicator:
        MGC (0) - Manual Gain Control
        AGC (1) - Automatic Gain Control active
    """
    MGC = 0
    AGC = 1

class IdentifierFormat(BinaryEnum, bits=1):
    """
    Control Packet Controllee/Controller Identifier Format:
        WORD (0) - 32-bit word
        UUID (1) - 128-bit UUID
    """
    WORD = 0
    UUID = 1

class ActionMode(BinaryEnum, bits=2):
    """
    Control Packet Action Mode Bits:
        NO_ACTION (0) - Controllee takes no action
        DRY_RUN   (1) - Controllee does not implement commands but sends ACK
                        packets as though it had
        EXECUTE   (2) - Controllee implements command and sends ACK packet(s)
        RESERVED  (3) - Reserved
    """
    NO_ACTION = 0b00
    DRY_RUN = 0b01
    EXECUTE = 0b10
    RESERVED = 0b11

class TimestampControlMode(BinaryEnum, bits=3):
    """
    Control Packet Timestamp Control Mode (Tables 8.3.1.7-1 and 8.4.1.5-1):
        IGNORE        (0) - Execute(d) with no timestamp constraints
        DEVICE        (1) - Execute(d) within device timing precision window
        LATE          (2) - Execute(d) within device timing precision window,
                            pr after if within application timing window
        EARLY         (3) - Execute(d) within device timing precision window,
                            or before if within application timing window
        EARLY_LATE    (4) - Execute(d) within application timing execution
                            window
        RESERVED_5    (5) - Reserved
        RESERVED_6    (6) - Reserved
        TIMING_ISSUES (7) - Did not executed some controls at specified time
                            (Acknowledge only)
    """
    IGNORE = 0b000
    DEVICE = 0b001
    LATE = 0b010
    EARLY = 0b011
    EARLY_LATE = 0b100
    RESERVED_5 = 0b101
    RESERVED_6 = 0b110
    TIMING_ISSUES = 0b111

class EntrySize(BinaryEnum, bits=4):
    """
    Index List Entry Size Subfield Coding (Table 9.3.2-1):
        EIGHT_BIT       (1) - 8-bit fields packed into 32 bits
        SIXTEEN_BIT     (1) - 16-bit fields packed into 32 bits
        THIRTY_TWO_BIT  (1) - 32-bit fields
    """
    EIGHT_BIT = 0b0001
    SIXTEEN_BIT = 0b0010
    THIRTY_TWO_BIT = 0b0100

class V49StandardCompliance(BinaryEnum, bits=32):
    """
    V49 Standard and Specification Compliance Number (Table 9.10.3-1)
    """
    V49_0 = 0b001
    V49_1 = 0b010
    V49_A = 0b011
    V49_2 = 0b100

# class CIF0Fields(Enum):
#     """
#     CIF0 fields
#     """
#     CHANGE_INDICATOR = 'change_indicator',
#     REFERENCE_POINT_ID = 'reference_point_id',
#     BANDWIDTH = 'bandwidth',
#     IF_REFERENCE_FREQUENCY = 'if_reference_frequency',
#     RF_REFERENCE_FREQUENCY = 'rf_reference_frequency',
#     RF_REFERENCE_FREQUENCY_OFFSET = 'rf_reference_frequency_offset',
#     IF_BAND_OFFSET = 'if_band_offset',
#     REFERENCE_LEVEL = 'reference_level',
#     GAIN = 'gain',
#     OVER_RANGE_COUNT = 'over_range_count',
#     SAMPLE_RATE = 'sample_rate',
#     TIMESTAMP_ADJUSTMENT = 'timestamp_adjustment',
#     TIMESTAMP_CALIBRATION_TIME = 'Timestamp Calibration Time',
#     TEMPERATURE = 'Temperature',
#     DEVICE_IDENTIFIER = 'Device Identifier',
#     STATE_EVENT_INDICATORS = 'State/Event Indicators',
#     SIGNAL_DATA_PAYLOAD_FORMAT = 'Signal Data Packet Payload Format',
#     FORMATTED_GPS = 'Formatted GPS',
#     FORMATTED_INS = 'Formatted INS',
#     ECEF_EPHEMERIS = 'ECEF Ephemeris',
#     RELATIVE_EPHEMERIS = 'Relative Ephemeris',
#     EPHEMERIS_REFERENCE_ID = 'Ephemeris Reference ID',
#     GPS_ASCII = 'GPS ASCII',
#     CONTEXT_ASSOCIATION_LISTS = 'Context Association Lists',
#     CIF7_ENABLE = 'Field Attributes Enable',
#     CIF3_ENABLE = 'CIF 3 Enable',
#     CIF2_ENABLE = 'CIF 2 Enable',
#     CIF1_ENABLE = 'CIF 1 Enable'

#     @property
#     def str_value(self):
#         return '{}'.format(self.value[0])

# class CIF1Fields(Enum):
#     """
#     CIF1 Fields
#     """
#     PHASE_OFFSET = 'Phase Offset',
#     POLARIZATION = 'Polarization',
#     POINTING_VECTOR = '3-D Pointing Vector',
#     POINTING_VECTOR_STRUCT = '3-D Pointing Vector Structure',
#     SPATIAL_SCAN_TYPE = 'Spatial Scan Type',
#     SPATIAL_REFERENCE_TYPE = 'Spatial Reference Type',
#     BEAM_WIDTHS = 'Beam Widths',
#     RANGE = 'Range',
#     EBNO_BER = 'Eb/No BER',
#     THRESHOLD = 'Threshold',
#     COMPRESSION_POINT = 'Compression Point',
#     INTERCEPT_POINTS = 'Intercept Points',
#     SNR_NOISE_FIGURE = 'SNR/Noise Figure',
#     AUX_FREQUENCY = 'Aux Frequency',
#     AUX_GAIN = 'Aux Gain',
#     AUX_BANDWIDTH = 'Aux Bandwidth',
#     ARRAY_OF_CIFS = 'Array of CIFS',
#     SPECTRUM = 'Spectrum',
#     SECTOR_SCAN_STEP = 'Sector Scan/Step',
#     INDEX_LIST = 'Index List',
#     DISCRETE_IO_32 = 'Discrete I/O 32',
#     DISCRETE_IO_64 = 'Discrete I/O 64',
#     HEALTH_STATUS = 'Health Status',
#     V49_SPEC_COMPLIANCE = 'V49 Spec Compliance',
#     VERSION_BUILD_CODE = 'Version and Build Code',
#     BUFFER_SIZE = 'Buffer Size'

#     @property
#     def str_value(self):
#         return '{}'.format(self.value[0])