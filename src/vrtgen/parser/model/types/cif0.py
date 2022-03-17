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
from .basic import FixedPointType

@dataclass
class GeolocationAngle(FixedPointType):
    """
    Geolocation Angle Format [Definition 9.4.5-1].
    """
    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32
        self.signed = True
        self.radix = 22
        self.resolution = 9

@dataclass
class CartesianCoordinate(FixedPointType):
    """
    Position coordinate format for ECEF Ephermeris [Rule 9.4.3-5].
    """
    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32
        self.signed = True
        self.radix = 5
        self.resolution = 5

@dataclass
class VelocityCoordinate(FixedPointType):
    """
    Velocity coordinate format for ECEF Ephemeris [Rule 9.4.3-7].
    """
    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32
        self.signed = True
        self.radix = 16
        self.resolution = 8
