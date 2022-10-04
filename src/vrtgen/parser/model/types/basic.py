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

from dataclasses import dataclass, field
from typing import List
from enum import IntEnum
from .packing import Field

@dataclass
class BooleanType(Field):
    name : str = 'basic_boolean_type'
    value : bool = False

    @property
    def is_boolean(self):
        return True

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

    @property
    def is_integer_type(self):
        return True

    def parse_mapping(self, **mapping):
        for key,val in mapping.items():
            if key == "bits":
                self.bits = int(val)
            if key == "value":
                self.value = val


    @property
    def is_integer_type(self):
        return True

@dataclass
class NonZeroIntegerType(IntegerType):
    """
    Base class for non-zero size fields, in which the binary representation is
    one less than the actual value.
    """
    converter : str = 'size_converter'

    @property
    def is_nonzero_integer_type(self):
        return True

@dataclass
class SignedIntegerType(IntegerType):
    name : str = 'signed_int_t'

    def __post_init__(self):
        super().__post_init__()
        self.signed = True

@dataclass
class Unsigned8(IntegerType):
    name : str = 'unsigned_8_t'

    def __post_init__(self):
        super().__post_init__()
        self.bits = 8

@dataclass
class Unsigned16(IntegerType):
    name : str = 'unsigned_16_t'

    def __post_init__(self):
        super().__post_init__()
        self.bits = 16

@dataclass
class Signed16(SignedIntegerType):
    name : str = 'signed_16_t'

    def __post_init__(self):
        super().__post_init__()
        self.bits = 16

@dataclass
class Unsigned32(IntegerType):
    name : str = 'unsigned_32_t'

    def __post_init__(self):
        super().__post_init__()
        self.bits = 32

@dataclass
class Signed32(SignedIntegerType):
    name : str = 'signed_32_t'

    def __post_init__(self):
        super().__post_init__()
        self.bits = 32

@dataclass
class Unsigned64(IntegerType):
    name : str = 'unsigned_64_t'

    def __post_init__(self):
        super().__post_init__()
        self.bits = 64

@dataclass
class Signed64(SignedIntegerType):
    name : str = 'signed_64_t'

    def __post_init__(self):
        super().__post_init__()
        self.bits = 64

@dataclass
class Identifier32(Unsigned32):
    name : str = 'identifier_32_t'

    def __post_init__(self):
        super().__post_init__()
        self.bits = 32

@dataclass
class Identifier16(IntegerType):
    name : str = 'identifier_16_t'

    def __post_init__(self):
        super().__post_init__()
        self.bits = 16
        self.reserved_bits = 16

@dataclass
class FixedPointType(IntegerType):
    """
    Fixed-Point Integer Type
    """
    radix : int = 0
    resolution : int = 0

    @property
    def is_fixed_point(self):
        return True

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
        super().__post_init__()
        self.bits = self.type_.bits
        if not self.value:
            self.value = self.type_()
        else:
            self.value = self.type_(self.value)

    @property
    def is_enum(self):
        return True

@dataclass
class CIFEnableType(Field):
    type_ : Field = None
    indicator_only : bool = False

@dataclass
class EnableIndicatorType(BooleanType):
    bits : int = 2
    is_enable : bool = False

@dataclass
class ListType(Field):
    type_ : Field = None
    linked_size : Field = None

    def __post_init__(self):
        super().__post_init__()
        self.values = []

    @property
    def is_list(self):
        return True