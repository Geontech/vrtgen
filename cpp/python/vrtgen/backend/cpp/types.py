from vrtgen.types import basic
from vrtgen.types import enums
from vrtgen.types import struct

def name_to_identifier(name):
    identifier = ''
    for ch in name:
        if ch.isalnum():
            identifier += ch
        elif ch in '.':
            identifier += '_'
    return identifier

def int_type(bits, signed):
    if bits > 32:
        ctype = 'int64_t'
    elif bits > 16:
        ctype = 'int32_t'
    elif bits > 8:
        ctype = 'int16_t'
    else:
        ctype = 'int8_t'
    if not signed:
        ctype = 'u' + ctype
    return ctype

def enum_namespace(datatype):
    return 'vrtgen::' + name_to_identifier(datatype.__name__)

def enum_type(datatype):
    return enum_namespace(datatype) + '::Code'

def enum_value(value):
    return '{}::{}'.format(enum_namespace(type(value)), value.name)

def float_type(bits):
    if bits >= 32:
        return 'double'
    else:
        return 'float'

def fixed_type(bits, radix, signed=True):
    return 'fixed<{},{},{}>'.format(int_type(bits, signed), radix, float_type(bits))

def value_type(datatype):
    if datatype == basic.StreamIdentifier:
        return 'vrtgen::StreamIdentifier'
    if issubclass(datatype, basic.BooleanType):
        return 'bool'
    if issubclass(datatype, enums.BinaryEnum):
        return enum_type(datatype)
    if issubclass(datatype, basic.FixedPointType):
        return float_type(datatype.bits)
    if issubclass(datatype, basic.IntegerType):
        return int_type(datatype.bits, datatype.signed)
    if issubclass(datatype, basic.NonZeroSize):
        return 'size_t'
    if issubclass(datatype, struct.Struct):
        return name_to_identifier(datatype.__name__)
    raise NotImplementedError(datatype.__name__)

def literal(value, datatype):
    if issubclass(datatype, enums.BinaryEnum):
        return enum_value(value)
    if issubclass(datatype, basic.BooleanType):
        return str(value).lower()
    return str(value)
