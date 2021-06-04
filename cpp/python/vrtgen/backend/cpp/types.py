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
Mappings of VRT types to C++ types.
"""

from vrtgen.types import basic, control, enums, struct, prologue

def name_to_identifier(name):
    """
    Returns a valid C++ identifier for a string name.
    """
    def map_char(value):
        if value.isalnum():
            return value
        if value in '.':
            return '_'
        return ''
    return ''.join(map_char(ch) for ch in name)

def int_type(bits, signed):
    """
    Returns the C++ sized integer type for a given bit width and sign bit.
    """
    if not signed:
        return 'u' + int_type(bits, True)
    if bits > 32:
        return 'int64_t'
    if bits > 16:
        return 'int32_t'
    if bits > 8:
        return 'int16_t'
    return 'int8_t'

def enum_namespace(datatype):
    """
    Returns the C++ namespace that contains a VRT enumeration.
    """
    return 'vrtgen::' + name_to_identifier(datatype.__name__)

def enum_type(datatype):
    """
    Returns the C++ enum type for a VRT enumeration.
    """
    return enum_namespace(datatype) + '::Code'

def enum_value(value):
    """
    Returns the C++ enum label for a VRT enumeration value.
    """
    return '{}::{}'.format(enum_namespace(type(value)), value.name)

def float_type(bits):
    """
    Returns the C++ floating point type for a given bit width.
    """
    if bits >= 32:
        return 'double'
    return 'float'

def fixed_type(bits, radix, resolution, signed=True):
    """
    Returns the C++ fixed point packing class for a VRT fixed point type.
    """
    return 'fixed<{},{},{},{}>'.format(int_type(bits, signed), radix, float_type(bits), resolution)

def value_type(datatype):
    """
    Returns the C++ API data type for a given VRT type.

    The C++ API data type is used in the external interfaces to packing
    classes, and is not necessarily the same as the packed type.
    """
    if datatype == basic.StreamIdentifier:
        return 'vrtgen::StreamIdentifier'
    if datatype == prologue.ClassIdentifier:
        return 'vrtgen::packing::ClassIdentifier'
    if datatype == control.UUIDIdentifier:
        return 'vrtgen::UUID'
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

def bool_literal(value):
    """
    Returns a boolean value as a C++ literal.
    """
    return str(value).lower()

def literal(value, datatype):
    """
    Returns a value as a C++ literal for a given data type.
    """
    if issubclass(datatype, enums.BinaryEnum):
        return enum_value(value)
    if issubclass(datatype, basic.BooleanType):
        return bool_literal(value)
    return str(value)
