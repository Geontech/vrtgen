# Copyright (C) 2021 Geon Technologies, LLC
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
from vrtgen.types import CIF0Fields, CIF1Fields

@pytest.mark.parametrize(
    'name,bit',
    [
        (CIF0Fields.CHANGE_INDICATOR.value, 31),
        (CIF0Fields.REFERENCE_POINT_ID.value, 30),
        (CIF0Fields.BANDWIDTH.value, 29),
        (CIF0Fields.IF_REFERENCE_FREQUENCY.value, 28),
        (CIF0Fields.RF_REFERENCE_FREQUENCY.value, 27),
        (CIF0Fields.RF_REFERENCE_FREQUENCY_OFFSET.value, 26),
        (CIF0Fields.IF_BAND_OFFSET.value, 25),
        (CIF0Fields.REFERENCE_LEVEL.value, 24),
        (CIF0Fields.GAIN.value, 23),
        (CIF0Fields.OVER_RANGE_COUNT.value, 22),
        (CIF0Fields.SAMPLE_RATE.value, 21),
        (CIF0Fields.TIMESTAMP_ADJUSTMENT.value, 20),
        (CIF0Fields.TIMESTAMP_CALIBRATION_TIME.value, 19),
        (CIF0Fields.TEMPERATURE.value, 18),
        (CIF0Fields.DEVICE_IDENTIFIER.value, 17),
        (CIF0Fields.STATE_EVENT_INDICATORS.value, 16),
        (CIF0Fields.SIGNAL_DATA_PAYLOAD_FORMAT.value, 15),
        (CIF0Fields.FORMATTED_GPS.value, 14),
        (CIF0Fields.FORMATTED_INS.value, 13),
        (CIF0Fields.ECEF_EPHEMERIS.value, 12),
        (CIF0Fields.RELATIVE_EPHEMERIS.value, 11),
        (CIF0Fields.EPHEMERIS_REFERENCE_ID.value, 10),
        (CIF0Fields.GPS_ASCII.value, 9),
        (CIF0Fields.CONTEXT_ASSOCIATION_LISTS.value, 8),
        (CIF0Fields.CIF7_ENABLE.value, 7),
        (CIF0Fields.CIF3_ENABLE.value, 3),
        (CIF0Fields.CIF2_ENABLE.value, 2),
        (CIF0Fields.CIF1_ENABLE.value, 1),
    ]
)
def test_cif0_enables(name, bit):
    field = cif0.CIF0.Enables.get_field(name)
    assert field.position.bit == bit

@pytest.mark.parametrize(
    'name,bit',
    [
        (CIF1Fields.PHASE_OFFSET.value, 31),
        (CIF1Fields.POLARIZATION.value, 30),
        (CIF1Fields.POINTING_VECTOR.value, 29),
        (CIF1Fields.POINTING_VECTOR_STRUCT.value, 28),
        (CIF1Fields.SPATIAL_SCAN_TYPE.value, 27),
        (CIF1Fields.SPATIAL_REFERENCE_TYPE.value, 26),
        (CIF1Fields.BEAM_WIDTHS.value, 25),
        (CIF1Fields.RANGE.value, 24),
        (CIF1Fields.EBNO_BER.value, 20),
        (CIF1Fields.THRESHOLD.value, 19),
        (CIF1Fields.COMPRESSION_POINT.value, 18),
        (CIF1Fields.INTERCEPT_POINTS.value, 17),
        (CIF1Fields.SNR_NOISE_FIGURE.value, 16),
        (CIF1Fields.AUX_FREQUENCY.value, 15),
        (CIF1Fields.AUX_GAIN.value, 14),
        (CIF1Fields.AUX_BANDWIDTH.value, 13),
        (CIF1Fields.ARRAY_OF_CIFS.value, 11),
        (CIF1Fields.SPECTRUM.value, 10),
        (CIF1Fields.SECTOR_SCAN_STEP.value, 9),
        (CIF1Fields.INDEX_LIST.value, 7),
        (CIF1Fields.DISCRETE_IO_32.value, 6),
        (CIF1Fields.DISCRETE_IO_64.value, 5),
        (CIF1Fields.HEALTH_STATUS.value, 4),
        (CIF1Fields.V49_SPEC_COMPLIANCE.value, 3),
        (CIF1Fields.VERSION_BUILD_CODE.value, 2),
        (CIF1Fields.BUFFER_SIZE.value, 1),
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
