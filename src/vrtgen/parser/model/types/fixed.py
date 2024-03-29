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

from dataclasses import dataclass
from .packing import FixedPointType

@dataclass
class FixedPoint64r20(FixedPointType):
    """
    Common Fixed Point Type of 64 bits with radix point 20
    """
    name : str = 'fixed64r20_t'

    def __post_init__(self):
        self.type_ = type(self).__name__
        self.bits = 64
        self.signed = True
        self.radix = 20
        self.resolution = 8

@dataclass
class FixedPoint32r6(FixedPointType):
    """
    Common Fixed Point Type of 16 bits with radix point 7
    """
    name : str = 'fixed32r6_t'

    def __post_init__(self):
        self.type_ = type(self).__name__
        self.bits = 32
        self.signed = True
        self.radix = 6
        self.resolution = 6

@dataclass
class FixedPoint16r7(FixedPointType):
    """
    Common Fixed Point Type of 16 bits with radix point 7
    """
    name : str = 'fixed16r7_t'

    def __post_init__(self):
        self.type_ = type(self).__name__
        self.bits = 16
        self.signed = True
        self.radix = 7
        self.resolution = 7

@dataclass
class FixedPoint16r6(FixedPointType):
    """
    Common Fixed Point Type of 16 bits with radix point 6
    """
    name : str = 'fixed16r6_t'

    def __post_init__(self):
        self.type_ = type(self).__name__
        self.bits = 16
        self.signed = True
        self.radix = 6
        self.resolution = 6
