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
Basic data types used in VITA 49 structures and fields.
"""
class BooleanType:
    """
    Base type for boolean value types.

    In most cases, the standard Boolean class should be used instead of
    creating a custom subclass; however, it is possible to create multi-bit
    booleans (as used in some enable fields) as subclasses.
    """
    __cached__ = {}

    def __new__(cls, value=False):
        return bool(value)

    def __init_subclass__(cls, bits, **kwds):
        super().__init_subclass__(**kwds)
        cls.bits = bits

    @classmethod
    def from_binary(cls, value):
        """
        Converts an unsigned binary representation to a boolean value.
        """
        # Any non-zero value is true, although in a strict sense multi-bit
        # booleans should be all 1s.
        return bool(value)

    @classmethod
    def to_binary(cls, value):
        """
        Converts a boolean value to its binary representation of the correct
        number of bits.
        """
        # Multi-bit booleans are all 1s; multiplying the bitmask by the value
        # converts a bool to the right integer value.
        return value * ((1 << cls.bits) - 1)

    @staticmethod
    def create(bits):
        """
        Creates new boolean types dynamically.

        If a boolean type has already been created with the same number of
        bits, returns the existing class object.
        """
        if bits == 1:
            return Boolean
        key = bits
        existing = BooleanType.__cached__.get(key, None)
        if existing:
            return existing
        name = 'Boolean{:d}'.format(bits)
        newclass = type(name, (BooleanType,), {}, bits=bits)
        BooleanType.__cached__[key] = newclass
        return newclass

class Boolean(BooleanType, bits=1):
    """
    One-bit boolean value type.
    """

class BinaryNumberType:
    """
    Base class for sized binary number types.

    Binary numbers have a bit size and may be signed or not. Common operations
    include bit masking, sign extension and range checking.
    """
    def __init_subclass__(cls, bits=None, signed=False, **kwds):
        super().__init_subclass__(**kwds)
        if not bits:
            # Must be an abstract subclass.
            return
        cls.bits = bits
        cls.signed = signed
        cls.mask = (1 << bits) - 1
        cls.minval, cls.maxval = cls._get_range()

    @classmethod
    def _get_range(cls):
        if cls.signed:
            minval = -(2**(cls.bits-1))
            maxval = -minval - 1
        else:
            minval = 0
            maxval = (2**cls.bits) - 1
        return (minval, maxval)

    @classmethod
    def to_binary(cls, value):
        """
        Converts this numeric value to to corresponding unsigned binary
        representation.
        """
        return value & cls.mask

    @classmethod
    def from_binary(cls, value):
        """
        Converts an unsigned binary representation to the correct numeric type.
        """
        if value < 0:
            raise ValueError('binary value must be unsigned')
        if value >= (1 << cls.bits):
            raise ValueError('binary value {} too large for {}'.format(value, cls.__name__))
        if cls.signed:
            value = cls._sign_extend(value)
        return value

    @classmethod
    def _range_check(cls, value):
        """
        Validates numeric values against the minimum and maximum for a class.
        """
        if value > cls.maxval:
            raise ValueError('{} > {} ({} max value)'.format(value, cls.maxval, cls.__name__, ))
        if value < cls.minval:
            raise ValueError('{} < {} ({} min value)'.format(value, cls.minval, cls.__name__, ))

    @classmethod
    def _sign_extend(cls, value):
        """
        Converts an unsigned integer value of this type's number of bits to a
        signed integer value by sign extension.
        """
        # If the sign bit is set, convert to a negative number by sign
        # extension (OR-ing all high bits).
        if value & (1 << (cls.bits - 1)):
            value |= ~cls.mask
        return value

class IntegerType(BinaryNumberType):
    """
    Base class for signed and unsigned integer types with specific bit widths.

    Extends the built-in integer type with the size and sign information, and
    provides range checking at construction time.
    """
    __cached__ = {}

    def __new__(cls, value=0):
        value = int(value)
        cls._range_check(value)
        return value

    # pylint: disable=signature-differs
    def __init_subclass__(cls, bits, signed=True, **kwds):
        super().__init_subclass__(bits, signed, **kwds)

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

class UInteger64(IntegerType, bits=64, signed=False):
    """
    64-bit unsigned integer type.
    """

class UInteger32(IntegerType, bits=32, signed=False):
    """
    32-bit unsigned integer type.
    """

class UInteger24(IntegerType, bits=24, signed=False):
    """
    24-bit unsigned integer type.
    """

class UInteger16(IntegerType, bits=16, signed=False):
    """
    16-bit unsigned integer type.
    """

class UInteger8(IntegerType, bits=8, signed=False):
    """
    8-bit unsigned integer type.
    """

class NonZeroSize(BinaryNumberType):
    """
    Base class for non-zero size fields, in which the binary representation is
    one less than the actual value.
    """
    __cached__ = {}

    def __new__(cls, value=1):
        value = int(value)
        cls._range_check(value)
        return value

    # pylint: disable=arguments-differ
    def __init_subclass__(cls, bits, **kwds):
        # In this case sizes are always unsigned
        super().__init_subclass__(bits, signed=False, **kwds)

    @classmethod
    def _get_range(cls):
        return 1, 2**cls.bits

    @classmethod
    def to_binary(cls, value):
        """
        Converts this non-zero size to its unsigned binary representation.
        """
        # Always unsigned, don't need superclass conversion
        return value - 1

    @classmethod
    def from_binary(cls, value):
        """
        Converts an unsigned binary representation to a non-zero size.
        """
        # Don't need superclass conversion here.
        return cls(value + 1)

    @staticmethod
    def create(bits):
        """
        Creates new non-zero size types dynamically.

        If a non-zero size type has already been created with the same number
        of bits, returns the existing class object.
        """
        key = bits
        existing = NonZeroSize.__cached__.get(key, None)
        if existing:
            return existing
        name = 'NonZeroSize{:d}'.format(bits)
        newclass = type(name, (NonZeroSize,), {}, bits=bits)
        NonZeroSize.__cached__[key] = newclass
        return newclass

class FixedPointType(BinaryNumberType):
    """
    Base class for fixed-point types, mapping to Python float for the actual
    value.

    Extends the built-in float type with the bits and radix information.
    """
    __cached__ = {}

    def __new__(cls, value=0.0):
        value = float(value)
        cls._range_check(value)
        return value

    # pylint: disable=arguments-differ
    def __init_subclass__(cls, bits, radix, resolution, **kwds):
        # Set radix and scale before calling base class hook because they are
        # needed for _get_range().
        cls.radix = radix
        cls.resolution = resolution
        cls.scale = 1 << cls.radix
        super().__init_subclass__(bits, signed=True, **kwds)

    @classmethod
    def _get_range(cls):
        minval, maxval = super()._get_range()
        return (minval / cls.scale, maxval / cls.scale)

    @classmethod
    def to_binary(cls, value):
        """
        Converts this fixed-point value to its binary representation as an
        unsigned integer.
        """
        # Shift radix point up so the least significant fractional bit is in
        # the ones place, then round and truncate to int. Then, mask the value
        # to convert it to unsigned.
        value = int(round(value * cls.scale))
        return super().to_binary(value)

    @classmethod
    def from_binary(cls, value):
        """
        Converts an unsigned binary representation to a fixed-point value.
        """
        # Let the base class handle sign conversion and just scale the result.
        value = super().from_binary(value)
        return cls(value / cls.scale)

    @staticmethod
    def create(bits, radix, resolution):
        """
        Creates new fixed-point types dynamically.

        If a fixed-point type has already been created with the same number of
        bits and radix point, returns the existing class object.
        """
        key = (bits, radix, resolution)
        existing = FixedPointType.__cached__.get(key, None)
        if existing:
            return existing
        name = 'FixedPoint{:d}r{:d}'.format(bits, radix)
        newclass = type(name, (FixedPointType,), {}, bits=bits, radix=radix, resolution=resolution)
        FixedPointType.__cached__[key] = newclass
        return newclass

# Common fixed-point types
class FixedPoint64r20(FixedPointType, bits=64, radix=20, resolution=8):
    """
    64-bit fixed-point type with 20 fractional bits.
    """

class FixedPoint32r16(FixedPointType, bits=32, radix=16, resolution=8):
    """
    32-bit fixed-point type with 16 fractional bits.
    """

class FixedPoint16r7(FixedPointType, bits=16, radix=7, resolution=7):
    """
    16-bit fixed-point type with 7 fractional bits.
    """

class FixedPoint16r13(FixedPointType, bits=16, radix=13, resolution=8):
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
