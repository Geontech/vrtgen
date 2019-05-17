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
    ('free running', enums.TSF.FREE_RUNNING),
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
