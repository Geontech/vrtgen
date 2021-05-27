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

import itertools

import pytest

from vrtgen.types import enums
from vrtgen.parser import value

def parametrize_literals(*values):
    """
    Parametrizes a test function that returns a value for a case-insensitive
    string literal.

    Creates pytest params for the input literal (assumed to be lowercase) plus
    the uppercase and mixed-case permutations.
    """
    def to_params(literal, expected):
        for lval in (literal, literal.upper(), literal.title()):
            # Use just the literal for the ID (excludes the expected value)
            yield pytest.param(lval, expected, id=lval)
    params = list(itertools.chain(*(to_params(l,e) for l,e in values)))
    return pytest.mark.parametrize("literal,expected", params)

@pytest.mark.parametrize(
    "input,expected",
    [
        pytest.param(0, False, id='0'),
        pytest.param(1, True, id='1'),
    ],
)
def test_parse_boolean(input, expected):
    assert value.parse_boolean(input) == expected

@pytest.mark.parametrize(
    "literal,expected",
    [
        pytest.param('12-34-56', 0x123456, id="numeric"),
        pytest.param('ab-cd-ef', 0xABCDEF, id="alpha"),
        pytest.param('0D-9C-A2', 0x0D9CA2, id="mixedcaps"),
    ],
)
def test_parse_oui(literal, expected):
    assert value.parse_oui(literal) == expected

def test_parse_oui_error():
    with pytest.raises(ValueError):
        value.parse_oui('123456')

@parametrize_literals(
    ('none', enums.TSI.NONE),
    ('utc', enums.TSI.UTC),
    ('gps', enums.TSI.GPS),
    ('other', enums.TSI.OTHER),
)
def test_parse_tsi(literal, expected):
    assert value.parse_tsi(literal) == expected

def test_parse_tsi_error():
    with pytest.raises(ValueError):
        value.parse_tsi('invalid')

@parametrize_literals(
    ('none', enums.TSF.NONE),
    ('samples', enums.TSF.SAMPLE_COUNT),
    ('picoseconds', enums.TSF.REAL_TIME),
    ('free-running', enums.TSF.FREE_RUNNING),
)
def test_parse_tsf(literal, expected):
    assert value.parse_tsf(literal) == expected

def test_parse_tsf_error():
    with pytest.raises(ValueError):
        value.parse_tsf('invalid')

@parametrize_literals(
    ('single', enums.SSI.SINGLE),
    ('first', enums.SSI.FIRST),
    ('middle', enums.SSI.MIDDLE),
    ('final', enums.SSI.FINAL),
)
def test_parse_ssi(literal, expected):
    assert value.parse_ssi(literal) == expected

def test_parse_ssi_error():
    with pytest.raises(ValueError):
        value.parse_ssi('invalid')

@parametrize_literals(
    ('fine', enums.TSM.FINE),
    ('coarse', enums.TSM.COARSE),
)
def test_parse_tsm(literal, expected):
    assert value.parse_tsm(literal) == expected

def test_parse_tsm_error():
    with pytest.raises(ValueError):
        value.parse_tsm('invalid')

@parametrize_literals(
    ('processing', enums.PackingMethod.PROCESSING_EFFICIENT),
    ('link', enums.PackingMethod.LINK_EFFICIENT),
)
def test_parse_packing_method(literal, expected):
    assert value.parse_packing_method(literal) == expected

def test_parse_packing_method_error():
    with pytest.raises(ValueError):
        value.parse_packing_method('invalid')

@parametrize_literals(
    ('real', enums.DataSampleType.REAL),
    ('real/imag', enums.DataSampleType.COMPLEX_CARTESIAN),
    ('mag/phase', enums.DataSampleType.COMPLEX_POLAR),
)
def test_parse_data_sample_type(literal, expected):
    assert value.parse_data_sample_type(literal) == expected

def test_parse_data_sample_type_error():
    with pytest.raises(ValueError):
        value.parse_data_sample_type('invalid')

@pytest.mark.parametrize(
    "literal,expected",
    (pytest.param(l,v,id='"{}"'.format(l)) for l, v in (
        ('signed fixed', enums.DataItemFormat.SIGNED_FIXED),
        ('unsigned fixed non-normalized', enums.DataItemFormat.UNSIGNED_FIXED_NON_NORMALIZED),
        ('signed vrt 2', enums.DataItemFormat.SIGNED_VRT_2),
        ('unsigned VRT, 5-bit exponent', enums.DataItemFormat.UNSIGNED_VRT_5),
        ('ieee half', enums.DataItemFormat.IEEE754_HALF_PRECISION),
        ('IEEE single-precision', enums.DataItemFormat.IEEE754_SINGLE_PRECISION),
    )),
)
def test_parse_data_item_format(literal, expected):
    assert value.parse_data_item_format(literal) == expected

def test_parse_data_item_format_error():
    with pytest.raises(ValueError):
        value.parse_data_item_format('invalid')

@parametrize_literals(
    ('none', enums.ActionMode.NO_ACTION),
    ('dry-run', enums.ActionMode.DRY_RUN),
    ('execute', enums.ActionMode.EXECUTE),
)
def test_parse_action(literal, expected):
    assert value.parse_action(literal) == expected

def test_parse_action_error():
    with pytest.raises(ValueError):
        value.parse_action('invalid')
