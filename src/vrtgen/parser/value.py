import re
from vrtgen.parser.model.types import enums

_TSI_VALUES = {
    'none':  enums.TSI.NONE,
    'utc':   enums.TSI.UTC,
    'gps':   enums.TSI.GPS,
    'other': enums.TSI.OTHER
}

def parse_tsi(value):
    """
    Parses a TimeStamp-Integer (TSI) literal.

    Valid values are 'none', 'utc', 'gps' and 'other'. Case is ignored.
    """
    try:
        return _TSI_VALUES[value.casefold()]
    except KeyError:
        raise ValueError('invalid inteer value: ' + value)

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
        raise ValueError('invalid fractional value: ' + value)

_TSM_VALUES = {
    'fine' : enums.TSM.FINE,
    'coarse' :  enums.TSM.COARSE
}

def parse_tsm(value):
    """
    Parses a Timestamp Mode (TSM) literal.

    Valid values are 'fine' and 'coarse'. Case is ignored.
    """
    try:
        return _TSF_VALUES[value.casefold()]
    except KeyError:
        raise ValueError('invalid TSM value: ' + value)

_ENABLE_VALUES = {
    'required': enums.Mode.REQUIRED,
    'optional': enums.Mode.OPTIONAL,
    # 'disabled': enums.Mode.DISABLED
}

def parse_enable(value):
    if not isinstance(value, str):
        raise TypeError(value)
    try:
        return _ENABLE_VALUES[value.casefold()]
    except KeyError:
        raise ValueError('invalid enable value: ' + value)

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
        raise ValueError('invalid identifier format: ' + value)

_ACTION_MODE_VALUES = {
    'none': enums.ActionMode.NO_ACTION,
    'dry-run': enums.ActionMode.DRY_RUN,
    'execute': enums.ActionMode.EXECUTE,
}
def parse_action_mode(value):
    """
    Parses an Action Mode literal.

    Valid values are 'none', 'dry-run', and 'execute'. Case is ignored.
    """
    try:
        return _ACTION_MODE_VALUES[value.casefold()]
    except KeyError:
        raise ValueError(value)

_TIMESTAMP_CONTROL_MODE_VALUES = {
    'ignore': enums.TimestampControlMode.IGNORE,
    'device': enums.TimestampControlMode.DEVICE,
    'late': enums.TimestampControlMode.LATE,
    'early': enums.TimestampControlMode.EARLY,
    'early-late': enums.TimestampControlMode.EARLY_LATE,
}
def parse_timing_control(value):
    """
    Parses a Timestamp Control Mode literal.

    Valid values are 'ignore', 'device', 'late', 'early', and 'early-late'. Case is ignored.
    """
    try:
        return _TIMESTAMP_CONTROL_MODE_VALUES[value.casefold()]
    except KeyError:
        raise ValueError(value)

_ENTRY_SIZE_VALUES = {
    '8' : enums.EntrySize.EIGHT_BIT,
    '16': enums.EntrySize.SIXTEEN_BIT,
    '32': enums.EntrySize.THIRTY_TWO_BIT,
}
def parse_entry_size(value):
    """
    Parses an Index List entry size literal.

    Valid values are '32' (32 bits), '16' (16 bits), '8' (8 bits).
    """
    try:
        return _ENTRY_SIZE_VALUES[str(value)]
    except KeyError:
        raise ValueError('invalid entry size: ' + value)