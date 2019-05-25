"""
Basic data types used in VITA 49 structures and fields.
"""

class Boolean:
    """
    One-bit boolean value type.
    """
    bits = 1
    def __new__(cls, value=False):
        return bool(value)

class IntegerType(int):
    """
    Base class for signed and unsigned integer types with specific bit widths.

    Extends the built-in integer type with the size and sign information, and
    provides range checking at construction time.
    """
    __cached__ = {}

    def __new__(cls, value=0):
        value = int.__new__(cls, value)
        if value > cls.maxval:
            raise ValueError('{} cannot exceed {:d}'.format(cls.__name__, cls.maxval))
        if value < cls.minval:
            raise ValueError('{} cannot be less than {:d}'.format(cls.__name__, cls.minval))
        return value

    def __init_subclass__(cls, bits, signed=True, **kwds):
        super().__init_subclass__(**kwds)
        cls.bits = bits
        cls.signed = signed
        cls.minval, cls.maxval = IntegerType.range(bits, signed)

    @staticmethod
    def range(bits, signed):
        """
        Returns the minimum and maximum values for an integer type.
        """
        if signed:
            minval = -(2**(bits-1))
            maxval = -minval - 1
        else:
            minval = 0
            maxval = (2**bits) - 1
        return (minval, maxval)

    @staticmethod
    def create(bits, signed=True):
        """
        Creates new integer types dynamically.

        If an integer type has already been created with the same number of
        bits and sign, returns the existing class object.
        """
        key = (bits, signed)
        existing = IntegerType.__cached__.get(key, None)
        if existing:
            return existing
        name = 'Int{:d}'.format(bits)
        if not signed:
            name = 'U'+name
        newclass = type(name, (IntegerType,), {}, bits=bits, signed=signed)
        IntegerType.__cached__[key] = newclass
        return newclass

class Integer64(IntegerType, bits=64):
    """
    64-bit signed integer type.
    """

class Integer32(IntegerType, bits=32):
    """
    32-bit signed integer type.
    """

class Integer24(IntegerType, bits=24):
    """
    24-bit signed integer type.
    """

class Integer16(IntegerType, bits=16):
    """
    16-bit signed integer type.
    """

class Integer8(IntegerType, bits=8):
    """
    8-bit signed integer type.
    """

class FixedPointType(float):
    """
    Base class for fixed-point types, mapping to Python float for the actual
    value.

    Extends the built-in float type with the bits and radix information.
    """
    __cached__ = {}

    def __new__(cls, value=0.0):
        value = float.__new__(cls, value)
        if value > cls.maxval:
            raise ValueError('{} cannot exceed {:d}'.format(cls.__name__, cls.maxval))
        if value < cls.minval:
            raise ValueError('{} cannot be less than {:d}'.format(cls.__name__, cls.minval))
        return value

    def __init_subclass__(cls, bits, radix, **kwds):
        super().__init_subclass__(**kwds)
        cls.bits = bits
        cls.radix = radix
        cls.resolution = 1 / (2**radix)
        minval, maxval = IntegerType.range(bits, signed=True)
        cls.minval = minval * cls.resolution
        cls.maxval = maxval * cls.resolution

    @staticmethod
    def create(bits, radix):
        """
        Creates new fixed-point types dynamically.

        If a fixed-point type has already been created with the same number of
        bits and radix point, returns the existing class object.
        """
        key = (bits, radix)
        existing = FixedPointType.__cached__.get(key, None)
        if existing:
            return existing
        name = 'FixedPoint{:d}r{:d}'.format(bits, radix)
        newclass = type(name, (FixedPointType,), {}, bits=bits, radix=radix)
        FixedPointType.__cached__[key] = newclass
        return newclass

# Common fixed-point types
class FixedPoint64r20(FixedPointType, bits=64, radix=20):
    """
    64-bit fixed-point type with 20 fractional bits.
    """

class FixedPoint32r16(FixedPointType, bits=32, radix=16):
    """
    32-bit fixed-point type with 16 fractional bits.
    """

class FixedPoint16r7(FixedPointType, bits=16, radix=7):
    """
    16-bit fixed-point type with 7 fractional bits.
    """

class FixedPoint16r13(FixedPointType, bits=16, radix=13):
    """
    16-bit fixed-point type with 13 fractional bits.
    """

class Identifier32(IntegerType, bits=32, signed=False):
    """
    32-bit generic identifier.
    """

class Identifier16(IntegerType, bits=16, signed=False):
    """
    16-bit generic identifier.
    """

class StreamIdentifier(IntegerType, bits=32, signed=False):
    """
    A Stream Identifier (Stream ID) is a 32-bit number assigned to a VRT
    Packet Stream [5.1.2].
    """

class OUI(IntegerType, bits=24, signed=False):
    """
    Organizationally Unique Identifier.
    """
    def __str__(self):
        octets = ((self >> n) & 0xFF for n in (16, 8, 0))
        return '-'.join('{:02X}'.format(x) for x in octets)
