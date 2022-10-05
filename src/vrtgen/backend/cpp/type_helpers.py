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

import enum
import inspect
from vrtgen.parser.model.types import *
from vrtgen.parser.model.command import ControlIdentifier, WarningErrorFields
from vrtgen.parser.model.data import Trailer
from vrtgen.parser.model.cif1 import DiscreteIO
from vrtgen.parser.model.cif7 import CIF7, CIF7Attributes
from vrtgen.parser.model.aor import ArrayOfRecords

"""
Mappings of VRT types to C++ types.
"""
VRTGEN_NAMESPACE = 'vrtgen::'
PACKING_NAMESPACE = 'vrtgen::packing::'

def int_type(field, forced_signed=None):
    """
    Returns the C++ sized integer type for a given bit width and sign bit.
    """
    if not field.signed and not forced_signed:
        return 'u' + int_type(field, forced_signed=True)
    if field.bits > 32:
        return 'int64_t'
    if field.bits > 16:
        return 'int32_t'
    if field.bits > 8:
        return 'int16_t'
    return 'int8_t'

def enum_type(field, namespace=PACKING_NAMESPACE):
    if issubclass(field.type_, enum.Enum):
        return namespace + field.type_.__name__
    return namespace + type(field.type_).__name__

def enum_value(field, namespace=PACKING_NAMESPACE):
    return enum_type(field, namespace) + '::' + field.value.name

def float_type(field):
    """
    Returns the C++ floating point type for a given bit width.
    """
    if field.bits >= 32:
        return 'long double'
    return 'double'

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

def is_enum(obj):
    # Ignore anything that isn't a BinaryEnum class
    if not inspect.isclass(obj) or not issubclass(obj, enums.BinaryEnum):
        return False
    # Only return concrete enums (i.e., those that have values defined) to
    # filter out abstract base classes (just BinaryEnum at present)
    return bool(obj.__members__)

def format_enum(enum):
    """
    Returns a description of a BinaryEnum type for use in code generation.
    """
    # Create a format string that returns a hex constant (binary constants are
    # a C++14 feature)
    digits = int((enum.bits + 3) / 4)
    format_string = '0x{{:0{}x}}'.format(digits)
    return {
        'name': enum.__name__,
        'doc': enum.__doc__,
        'values': [(e.name, format_string.format(e.value)) for e in enum]
    }

class TypeHelper:
    def value_type(self, field):
        if isinstance(field, CIFEnableType):
            if field.template_type:
                return 'T'
            elif field.type_ and field.type_ != type(None).__name__:
                return self.value_type(field.type_)
            else:
                return 'bool'
        elif isinstance(field, EnableIndicatorType):
            if field.type_ and field.type_ != type(None).__name__:
                return self.value_type(field.type_)
            else:
                return 'bool'
        elif isinstance(field, BooleanType):
            return 'bool'
        elif isinstance(field, OUI):
            return VRTGEN_NAMESPACE + 'OUI'
        elif isinstance(field, ControlIdentifier):
            if field.format == IdentifierFormat.UUID:
                # return 'std::string'
                return VRTGEN_NAMESPACE + 'UUID'
            else:
                return 'uint32_t'
        elif isinstance(field, FixedPointType):
            return float_type(field)
        elif isinstance(field, IntegerType):
            return int_type(field)
        elif isinstance(field, EnumType):
            if field.user_defined and field.packet_name:
                return enum_type(field, namespace=field.packet_name+'::enums::')
            elif field.user_defined:
                return enum_type(field, namespace='enums::')
            else:
                return enum_type(field)
        elif isinstance(field, ListType):
            return 'std::vector<{}>'.format(self.value_type(field.type_))
        elif isinstance(field, PackedType):
            return 'vrtgen::packed<uint{}_t>'.format(field.bits)
        elif isinstance(field, DiscreteIO):
            if len(field.subfields) > 0:
                return '{}::structs::{}'.format(field.packet_name, field.type_)
            else:
                return 'uint{}_t'.format(field.bits)
        elif isinstance(field, ArrayOfRecords):
            return PACKING_NAMESPACE + field.type_ + '<{}::structs::{}Record>'.format(field.packet_name, field.type_)
        elif isinstance(field, CIF7Attributes):
            return '{}::structs::{}'.format(field.packet_name, field.type_)
        elif isinstance(field, CIF7):
            return PACKING_NAMESPACE + field.type_
        elif isinstance(field, TemplateArrayStruct):
            return PACKING_NAMESPACE + field.type_ + '<{}>'.format(field.template_type)
        elif isinstance(field, Field):
            return PACKING_NAMESPACE + field.type_
        else:
            raise TypeError(field)

    def debug(self, var):
        import pdb; pdb.set_trace()

    def member_type(self, field):
        if isinstance(field, CIFEnableType):
            if field.template_type:
                return 'T'
            elif field.type_:
                return self.member_type(field.type_)
        elif isinstance(field, FixedPointType):
            return int_type(field)
        elif isinstance(field, ControlIdentifier) and (field.format == IdentifierFormat.UUID):
            return VRTGEN_NAMESPACE + 'UUID'
        elif isinstance(field, Trailer) and field.state_event_indicators.is_user_defined:
            return '{}::structs::{}'.format(field.packet_name, field.type_)
        else:
            return self.value_type(field)

    def fixed_template(self, field):
        if isinstance(field, CIFEnableType):
            if field.type_:
                return self.fixed_template(field.type_)
        if not isinstance(field, FixedPointType):
            raise TypeError('converter_type requires a FixedPointType field')
        return '<{},{}>'.format(field.bits, field.radix)

    def literal_value(self, field):
        if isinstance(field, CIFEnableType):
            return self.literal_value(field.type_)
        elif isinstance(field, EnableIndicatorType):
            if field.type_ and field.type_ != type(None).__name__:
                return self.literal_value(field.type_)
            else:
                return str(field.value).lower()
        elif isinstance(field, BooleanType):
            return str(field.value).lower()
        elif isinstance(field, EnumType):
            if field.user_defined and field.packet_name:
                return enum_value(field, namespace=field.packet_name+"::enums::")
            else:
                return enum_value(field)
        elif isinstance(field, IntegerType):
            if field.value:
                return '0x{}'.format(str(hex(field.value)[2:]).upper())
            return '0'
        return None

    def is_scalar(self, field):
        if isinstance(field, BooleanType):
            return True
        elif isinstance(field, EnumType):
            return True
        elif isinstance(field, IntegerType):
            return True
        elif isinstance(field, PackedType):
            return True
        elif isinstance(field, ControlIdentifier):
            if field.format == IdentifierFormat.UUID:
                return False
            else:
                return True
        elif isinstance(field, DiscreteIO):
            if len(field.subfields) > 0:
                return False
            else:
                return True
        elif isinstance(field, CIFEnableType):
            if field.type_:
                return self.is_scalar(field.type_)
        return False

    def requires_get_set(self, field):
        if isinstance(field, CIFEnableType):
            return self.requires_get_set(field.type_)
        if isinstance(field, OUI):
            return True
        # elif isinstance(field, ControlIdentifier) and (field.format == IdentifierFormat.UUID):
        #     return True
        return False
