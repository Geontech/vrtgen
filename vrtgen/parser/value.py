"""
Parsers for field value types.
"""
import re

from vrtgen.types import enums

def parse_boolean(value):
    """
    Evalutes a boolean value.

    The YAML parser already converts boolean literals to Python True and False.
    This function allows the integer values 0 and 1 to be used in a boolean
    context.
    """
    if value in (0, False):
        return False
    if value in (1, True):
        return True
    raise TypeError('must be boolean, 0 or 1')

_TSI_VALUES = {
    'none':  enums.TSI.NONE,
    'utc':   enums.TSI.UTC,
    'gps':   enums.TSI.GPS,
    'other': enums.TSI.OTHER
}

_HEX_DIGIT = r'[0-9a-zA-Z]'
_OUI_RE = re.compile(r'({0})-({0})-({0})$'.format(_HEX_DIGIT*2))
def parse_oui(value):
    """
    Parsers an Organizationally Unique Identifier literal.

    OUIs are specified as hex literals in the form 'XX-XX-XX', where each
    octet is separated by a dash. Case is ignored.
    """
    match = _OUI_RE.match(str(value))
    if not match:
        raise ValueError('OUI format must be XX-XX-XX')
    return int(''.join(match.groups()), 16)

def parse_tsi(value):
    """
    Parses a TSI literal.

    Valid values are 'none', 'utc', 'gps' and 'other'. Case is ignored.
    """
    try:
        return _TSI_VALUES[value.casefold()]
    except KeyError:
        raise ValueError(value)

_TSF_VALUES = {
    'none':         enums.TSF.NONE,
    'samples':      enums.TSF.SAMPLE_COUNT,
    'picoseconds':  enums.TSF.REAL_TIME,
    'free running': enums.TSF.FREE_RUNNING
}

def parse_tsf(value):
    """
    Parses a TSF literal.

    Valid values are 'none', 'samples', 'picoseconds' and 'free running'. Case
    is ignored.
    """
    try:
        return _TSF_VALUES[value.casefold()]
    except KeyError:
        raise ValueError(value)

def parse_ssi(value):
    """
    Parses an SSI literal.

    Valid values are 'single', 'first', 'middle' and 'final'. Case is ignored.
    """
    try:
        return getattr(enums.SSI, value.upper())
    except AttributeError:
        raise ValueError(value)
