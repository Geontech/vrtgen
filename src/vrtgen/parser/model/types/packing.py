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

from dataclasses import dataclass, field, asdict
from typing import List
from enum import IntEnum

VRTGEN_NAMESPACE = 'vrtgen::'
PACKING_NAMESPACE = 'vrtgen::packing::'

@dataclass
class PackedTag:
    position : int
    bits : int
    field_word : int
    packed_int : int = None

@dataclass
class Field:
    name         : str
    type_        : str = type(None).__name__
    bits         : int = 1
    enabled      : bool = False
    required     : bool = False
    user_defined : bool = False
    template_type: bool = False
    reserved_bits: int = 0
    packed_tag   : PackedTag = None
    packet_name  : str = ''

    @property
    def is_optional(self):
        return (self.enabled and not self.required)

    @property
    def is_boolean(self):
        return isinstance(self, BooleanType)

    @property
    def is_enum(self):
        return isinstance(self, EnumType)

    @property
    def is_integer(self):
        return isinstance(self, IntegerType)

    @property
    def is_fixed_point(self):
        return isinstance(self, FixedPointType)

    @property
    def is_packed(self):
        return isinstance(self, PackedType)

    @property
    def is_packed_struct(self):
        return isinstance(self, PackedStruct)

@dataclass
class BooleanType(Field):
    name : str = 'basic_boolean_type'
    value : bool = False

@dataclass
class IntegerType(Field):
    """
    Integer Type
    """
    name : str = 'basic_integer_type'
    signed : bool = False
    value : int = None

    @property
    def range(self):
        if self.signed:
            minval = -(2**(self.bits-1))
            maxval = -minval - 1
        else:
            minval = 0
            maxval = (2**self.bits) - 1
        return (minval, maxval)

    def parse_mapping(self, **mapping):
        for key,val in mapping.items():
            if key == 'bits':
                self.bits = int(val)
            if key == 'value':
                self.value = val

@dataclass
class SignedIntegerType(IntegerType):
    name : str = 'signed_int_t'

    def __post_init__(self):
        self.signed = True

@dataclass
class Unsigned8(IntegerType):
    name : str = 'unsigned_8_t'

    def __post_init__(self):
        self.bits = 8

@dataclass
class Unsigned16(IntegerType):
    name : str = 'unsigned_16_t'

    def __post_init__(self):
        self.bits = 16

@dataclass
class Signed16(SignedIntegerType):
    name : str = 'signed_16_t'

    def __post_init__(self):
        self.bits = 16

@dataclass
class Unsigned32(IntegerType):
    name : str = 'unsigned_32_t'

    def __post_init__(self):
        self.bits = 32

@dataclass
class Signed32(SignedIntegerType):
    name : str = 'signed_32_t'

    def __post_init__(self):
        self.bits = 32

@dataclass
class Unsigned64(IntegerType):
    name : str = 'unsigned_64_t'

    def __post_init__(self):
        self.bits = 64

@dataclass
class Signed64(SignedIntegerType):
    name : str = 'signed_64_t'

    def __post_init__(self):
        self.bits = 64

@dataclass
class Identifier32(Unsigned32):
    name : str = 'identifier_32_t'

    def __post_init__(self):
        self.bits = 32

@dataclass
class Identifier16(IntegerType):
    name : str = 'identifier_16_t'

    def __post_init__(self):
        self.bits = 16
        self.reserved_bits = 16

@dataclass
class FixedPointType(IntegerType):
    """
    Fixed-Point Integer Type
    """
    radix : int = 0

    @property
    def range(self):
        scale = 1 << self.radix
        minval, maxval = super().range
        return (minval / scale, maxval / scale)

@dataclass
class EnumType(Field):
    type_ : IntEnum = field(default_factory=IntEnum)
    value : int = None

    def __post_init__(self):
        self.bits = self.type_.bits
        if not self.value:
            self.value = self.type_()
        else:
            self.value = self.type_(self.value)

@dataclass
class CIFEnableType(Field):
    type_ : Field = None
    indicator_only : bool = False

    @property
    def is_enum(self):
        if self.type_:
            return self.type_.is_enum
        return False

    @property
    def is_integer(self):
        if self.type_:
            return self.type_.is_integer
        return False

    @property
    def is_fixed_point(self):
        if self.type_:
            return self.type_.is_fixed_point
        return False

@dataclass
class EnableIndicatorType(BooleanType):
    bits : int = 2
    is_enable : bool = False
    is_enum: bool = False

@dataclass
class ListType(Field):
    type_ : Field = None
    linked_size : Field = None

    def __post_init__(self):
        self.values = []

    @property
    def is_list(self):
        return True

@dataclass
class PackedType(Field):
    """
    Special type of field for libvrtgen packing
    """
    @property
    def is_packed_type(self):
        return True

@dataclass
class PackedStruct(Field):
    """
    Structure for packing fields
    """
    def __post_init__(self):
        self.type_ = type(self).__name__
        self._dynamic_size = False

    @property
    def fields(self):
        return [self.__dict__[key] for key,_ in asdict(self).items() if is_field_type(self.__dict__[key])]

    @property
    def packed_fields(self):
        return [self.__dict__[key] for key,_ in asdict(self).items() if is_packed_type(self.__dict__[key])]

    @property
    def dynamic_size(self):
        return self._dynamic_size

@dataclass
class TemplateArrayStruct(PackedStruct):
    """
    Structure for packing fields with an array subfield
    """

    def __post_init__(self):
        super().__post_init__()
        self.template_name = "T"
        self.template_type = str()
        self._dynamic_size = True
        self.total_size_field = None

    @property
    def is_template(self):
        return True
    
    @property
    def is_array_struct(self):
        return True

def is_field_type(obj):
    try:
        return isinstance(obj, Field)
    except:
        pass
    return False

def is_packed_type(obj):
    try:
        return isinstance(obj, PackedType)
    except:
        pass
    return False