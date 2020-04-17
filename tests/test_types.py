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

import pytest

from vrtgen.types import cif0, cif1
from vrtgen.types import basic

@pytest.mark.parametrize(
    'name,bit',
    [
        ('Context Field Change Indicator', 31),
        ('Reference Point Identifier', 30),
        ('Bandwidth', 29),
        ('IF Reference Frequency', 28),
        ('RF Reference Frequency', 27),
        ('RF Reference Frequency Offset', 26),
        ('IF Band Offset', 25),
        ('Reference Level', 24),
        ('Gain', 23),
        ('Over-range Count', 22),
        ('Sample Rate', 21),
        ('Timestamp Adjustment', 20),
        ('Timestamp Calibration Time', 19),
        ('Temperature', 18),
        ('Device Identifier', 17),
        ('State/Event Indicators', 16),
        ('Signal Data Packet Payload Format', 15),
        ('Formatted GPS', 14),
        ('Formatted INS', 13),
        ('ECEF Ephemeris', 12),
        ('Relative Ephemeris', 11),
        ('Ephemeris Reference ID', 10),
        ('GPS ASCII', 9),
        ('Context Association Lists', 8),
        ('Field Attributes Enable', 7),
        ('CIF 3 Enable', 3),
        ('CIF 2 Enable', 2),
        ('CIF 1 Enable', 1),
    ]
)
def test_cif0_enables(name, bit):
    field = cif0.CIF0.Enables.get_field(name)
    assert field.position.bit == bit

@pytest.mark.parametrize(
    'name,bit',
    [
        ('Phase Offset', 31),
        ('Polarization', 30),
        ('3-D Pointing Vector', 29),
        ('3-D Pointing Vector Structure', 28),
        ('Spatial Scan Type', 27),
        ('Spatial Reference Type', 26),
        ('Beam Widths', 25),
        ('Range', 24),
        ('Eb/No BER', 20),
        ('Threshold', 19),
        ('Compression Point', 18),
        ('Intercept Points', 17),
        ('SNR/Noise Figure', 16),
        ('Aux Frequency', 15),
        ('Aux Gain', 14),
        ('Aux Bandwidth', 13),
        ('Array of CIFS', 11),
        ('Spectrum', 10),
        ('Sector Scan/Step', 9),
        ('Index List', 7),
        ('Discrete I/O 32', 6),
        ('Discrete I/O 64', 5),
        ('Health Status', 4),
        ('V49 Spec Compliance', 3),
        ('Version and Build Code', 2),
        ('Buffer Size', 1),
    ]
)
def test_cif1_enables(name, bit):
    field = cif1.CIF1.Enables.get_field(name)
    assert field.position.bit == bit


NonZero6 = basic.NonZeroSize.create(6)

@pytest.mark.parametrize(
    "value",
    [0, 65]
)
def test_size_range(value):
    with pytest.raises(ValueError):
        NonZero6(value)

Bool2 = basic.BooleanType.create(2)
Bool5 = basic.BooleanType.create(5)

Int7 = basic.IntegerType.create(7)

@pytest.mark.parametrize(
    "datatype,value,expected",
    [
        # Check that converting boolean types to their binary representation
        # occupies the correct number of bits.
        (basic.Boolean, False, 0),
        (basic.Boolean, True, 1),
        (Bool2, False, 0),
        (Bool2, True, 3),
        (Bool5, True, 31),

        # Check that binary representations of non-zero sizes are 1 less than
        # actual value.
        (NonZero6, 1, 0),
        (NonZero6, 12, 11),
        (NonZero6, 64, 63),

        # Unsigned integer conversion is a no-op.
        (basic.UInteger24, 301, 301),
        (basic.UInteger24, 16777215, 16777215),

        # Check signed integer conversion, both byte- and non-byte-aligned.
        (basic.Integer16, -1, 65535),
        (basic.Integer16, 20000, 20000),
        (basic.Integer16, -20000, 45536),
        (Int7, 5, 5),
        (Int7, -7, 121),

        # Fixed-point conversions involve both range shift and sign conversion.
        (basic.FixedPoint16r7, 2.5, 320), # 00000001 0.1000000
        (basic.FixedPoint16r7, -7.0625, 64632), # 11111100 0.1111000
    ]
)
def test_to_binary(datatype, value, expected):
    value = datatype(value)
    assert datatype.to_binary(value) == expected

NonZero5 = basic.NonZeroSize.create(5)
Int5 = basic.IntegerType.create(5)

@pytest.mark.parametrize(
    "datatype,value,expected",
    [
        # Boolean types are pretty forgiving, this mostly just checks API.
        (basic.Boolean, 0, False),
        (basic.Boolean, 1, True),
        (Bool2, 0, False),
        (Bool2, 3, True),

        # Check that effective values of non-zeros sizes are one more than the
        # binary representations.
        (NonZero5, 0, 1),
        (NonZero5, 15, 16),
        (NonZero5, 31, 32),

        # Unsigned integer conversion is a no-op.
        (basic.UInteger16, 4500, 4500),
        (basic.UInteger16, 65535, 65535),

        # Check signed integer conversion, both byte- and non-byte-aligned.
        (basic.Integer32, 4294967295, -1),
        (basic.Integer32, 5892979, 5892979),
        (basic.Integer32, 3315221642, -979745654),
        (Int5, 14, 14),
        (Int5, 31, -1),
    ]
)
def test_from_binary(datatype, value, expected):
    expected = datatype(expected)
    assert datatype.from_binary(value) == expected
