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
    Parses a TimeStamp-Integer (TSI) literal.

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
    Parses a TimeStamp-Fractional (TSF) literal.

    Valid values are 'none', 'samples', 'picoseconds' and 'free running'. Case
    is ignored.
    """
    try:
        return _TSF_VALUES[value.casefold()]
    except KeyError:
        raise ValueError(value)

def parse_ssi(value):
    """
    Parses a Start/Stop of Sample Frame Indication (SSI) literal.

    Valid values are 'single', 'first', 'middle' and 'final'. Case is ignored.
    """
    try:
        return getattr(enums.SSI, value.upper())
    except AttributeError:
        raise ValueError(value)

def parse_tsm(value):
    """
    Parses a Timestamp Mode (TSM) literal.

    Valid values are 'fine' and 'coarse'. Case is ignored.
    """
    try:
        return getattr(enums.TSM, value.upper())
    except AttributeError:
        raise ValueError(value)

_PACKINGMETHOD_VALUES = {
    'processing': enums.PackingMethod.PROCESSING_EFFICIENT,
    'link': enums.PackingMethod.LINK_EFFICIENT,
}

def parse_packing_method(value):
    """
    Parses a Packing Method literal.

    Valid values are 'processing' and 'link'. Case is ignored.
    """
    try:
        return _PACKINGMETHOD_VALUES[value.casefold()]
    except KeyError:
        raise ValueError(value)

_DATASAMPLETYPE_VALUES = {
    'real': enums.DataSampleType.REAL,
    'complex cartesian': enums.DataSampleType.COMPLEX_CARTESIAN,
    'real/imag': enums.DataSampleType.COMPLEX_CARTESIAN,
    'complex polar': enums.DataSampleType.COMPLEX_POLAR,
    'mag/phase': enums.DataSampleType.COMPLEX_POLAR,
}
def parse_data_sample_type(value):
    """
    Parses a Data Sample Type literal.

    Valid values are 'real', 'complex cartesian', 'real/imag' (alias for
    'complex cartesian'), 'complex polar' and 'mag/phase' (alias for 'complex
    polar'). Case is ignored.
    """
    try:
        return _DATASAMPLETYPE_VALUES[value.casefold()]
    except KeyError:
        raise ValueError(value)

_DATAITEMFORMAT_VALUES = {
    'signed fixed': enums.DataItemFormat.SIGNED_FIXED,
    'signed vrt 1-bit exponent': enums.DataItemFormat.SIGNED_VRT_1,
    'signed vrt 2-bit exponent': enums.DataItemFormat.SIGNED_VRT_2,
    'signed vrt 3-bit exponent': enums.DataItemFormat.SIGNED_VRT_3,
    'signed vrt 4-bit exponent': enums.DataItemFormat.SIGNED_VRT_4,
    'signed vrt 5-bit exponent': enums.DataItemFormat.SIGNED_VRT_5,
    'signed vrt 6-bit exponent': enums.DataItemFormat.SIGNED_VRT_6,
    'signed fixed non-normalized': enums.DataItemFormat.SIGNED_FIXED_NON_NORMALIZED,
    'unsigned fixed': enums.DataItemFormat.UNSIGNED_FIXED,
    'unsigned vrt 1-bit exponent': enums.DataItemFormat.UNSIGNED_VRT_1,
    'unsigned vrt 2-bit exponent': enums.DataItemFormat.UNSIGNED_VRT_2,
    'unsigned vrt 3-bit exponent': enums.DataItemFormat.UNSIGNED_VRT_3,
    'unsigned vrt 4-bit exponent': enums.DataItemFormat.UNSIGNED_VRT_4,
    'unsigned vrt 5-bit exponent': enums.DataItemFormat.UNSIGNED_VRT_5,
    'unsigned vrt 6-bit exponent': enums.DataItemFormat.UNSIGNED_VRT_6,
    'unsigned fixed non-normalized': enums.DataItemFormat.UNSIGNED_FIXED_NON_NORMALIZED,
    'ieee half-precision': enums.DataItemFormat.IEEE754_HALF_PRECISION,
    'ieee single-precision': enums.DataItemFormat.IEEE754_SINGLE_PRECISION,
    'ieee double-precision': enums.DataItemFormat.IEEE754_DOUBLE_PRECISION,
}

def parse_data_item_format(value):
    """
    Parses a Data Item Format literal.

    Data item format values vary depending on the encoding, which may be one
    of 'fixed', 'VRT' or 'IEEE'. Case is always ignored.

    Fixed-point encodings must be either 'signed' or 'unsigned', and can be
    'non-normalized'. Normalized is the default and does not need to be
    specified.  For example, 'signed fixed' specifies a signed, normalized
    fixed-point format, while 'unsigned fixed non-normalized' specified an
    unsigned, non-normalized fixed-point format.

    VRT floating-point encodings must be either 'signed' or 'unsigned' and
    specify the number of exponent bits; for example, 'signed VRT 1-bit
    exponent'. The exponent must be between 1 and 6 bits, inclusive.

    IEEE 754 floating-point encodings must specify the precision as one of
    'half-precision' (16-bit), 'single-precision' (32-bit) or
    'double-precision' (64-bit). For example, 'IEEE single' specifies a 32-bit
    floating point format.
    """
    try:
        return _DATAITEMFORMAT_VALUES[value.casefold()]
    except KeyError:
        raise ValueError(value)
