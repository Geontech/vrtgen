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

"""
Parsers for field value types.
"""
import re

from vrtgen.types import basic
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
    'free-running': enums.TSF.FREE_RUNNING
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

# IEEE 754 format specifier regexp:
# * "ieee" keyword with optional "754" suffix, space or "-" separated
# * precision keyord with optional "-precision" suffix (group "precision")
_IEEE_REGEXP = re.compile(r'ieee(?:[ -]754)? +(?P<precision>single|double|half)(?:-precision)?')
_IEEE_FORMATS = {
    'half': enums.DataItemFormat.IEEE754_HALF_PRECISION,
    'single': enums.DataItemFormat.IEEE754_SINGLE_PRECISION,
    'double': enums.DataItemFormat.IEEE754_DOUBLE_PRECISION,
}
def _ieee_format(precision):
    return _IEEE_FORMATS[precision]

# Common regexp prefix for optional (un)signed qualifier
_SIGNED_REGEXP = r'(?P<sign>(?:un)?signed)'

# Fixed-point format specifier regexp:
# * optional sign qualifier (group "sign")
# * "fixed" keyword with optional "-point" suffix
# * optional "non-normalized" qualifier (group "non_normalized")
_FIXED_REGEXP = re.compile(
    _SIGNED_REGEXP + r' +fixed(?:-point)?(?:[ ]+(?P<non_normalized>non-normalized))?'
)
def _fixed_format(sign, non_normalized):
    value = 0
    if sign == 'unsigned':
        value += 0b10000
    if non_normalized is not None:
        value += 0b00111
    return enums.DataItemFormat(value)

# VRT floating point format specifier regexp:
# * optional sign qualifier (group "sign")
# * "vrt" keyword with optional ","
# * exponent bits with optional "-bit" and "exponent" suffixes (group "exp")
_VRTFLOAT_REGEXP = re.compile(
    _SIGNED_REGEXP + r' +vrt[, ] *(?P<exp>[1-6])(?:[ -]bit)?(?:[ ]+exponent)?'
)
def _vrtfloat_format(sign, exp):
    # VRT float formats happen to have the exponent in the lower bits of the
    # format code
    value = int(exp)
    if sign == 'unsigned':
        value += 0b10000
    return enums.DataItemFormat(value)

# Mapping of regular expressions to format extraction functions
_DATA_ITEM_FORMAT_PARSERS = (
    (_IEEE_REGEXP, _ieee_format),
    (_FIXED_REGEXP, _fixed_format),
    (_VRTFLOAT_REGEXP, _vrtfloat_format)
)
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
    # Check each possible regexp for a complete match (i.e., reject strings with
    # extra noise on the end). On a match, use the associated parse function.
    for regexp, parser in _DATA_ITEM_FORMAT_PARSERS:
        match = regexp.fullmatch(value.casefold())
        if match:
            return parser(**match.groupdict())

    raise ValueError(value)

_ACTION_VALUES = {
    'none': enums.ActionMode.NO_ACTION,
    'dry-run': enums.ActionMode.DRY_RUN,
    'execute': enums.ActionMode.EXECUTE,
}
def parse_action(value):
    """
    Parses an Action Mode literal.

    Valid values are 'none', 'dry run', and 'execute'. Case is ignored.
    """
    try:
        return _ACTION_VALUES[value.casefold()]
    except KeyError:
        raise ValueError(value)

_IDENTIFIER_FORMAT_VALUES = {
    'word': enums.IdentifierFormat.WORD,
    'uuid': enums.IdentifierFormat.UUID,
}
def parse_identifier_format(value):
    """
    Parses a CAM identifier format literal.

    Valid values are 'word' (32 bits) and 'UUID' (128 bits). Case is ignored.
    """
    try:
        return _IDENTIFIER_FORMAT_VALUES[value.casefold()]
    except KeyError:
        raise ValueError(value)

# Type object to parse function table
_PARSERS = {
    basic.Boolean: parse_boolean,
    basic.OUI: parse_oui,
    enums.TSI: parse_tsi,
    enums.TSF: parse_tsf,
    enums.SSI: parse_ssi,
    enums.TSM: parse_tsm,
    enums.PackingMethod: parse_packing_method,
    enums.DataSampleType: parse_data_sample_type,
    enums.DataItemFormat: parse_data_item_format,
    enums.ActionMode: parse_action,
}
def get_value_parser(datatype):
    """
    Returns a parser that converts a YAML value into the given data type.

    If no specialized parser is registered, the data type constructor is used.
    """
    return _PARSERS.get(datatype, datatype)
