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
from .basic import IntegerType

@dataclass
class StreamIdentifier(IntegerType):
    """
    A Stream Identifier (Stream ID) is a 32-bit number assigned to a VRT
    Packet Stream [5.1.2].
    """
    name : str = 'stream_id'

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

@dataclass
class IntegerTimestamp(IntegerType):
    """
    Integer Timestamp [5.1.4.1]
    """
    name : str = 'integer_timestamp'

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

@dataclass
class FractionalTimestamp(IntegerType):
    """
    Fractional Timestamp [5.1.4.2]
    """
    name : str = 'fractional_timestamp'

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 64

@dataclass
class OUI(IntegerType):
    """
    Organizationally Unique Identifier.
    """

    def __post_init__(self):
        super().__post_init__()
        self.bits = 24
    
    def __str__(self):
        octets = ((self >> n) & 0xFF for n in (16, 8, 0))
        return '-'.join('{:02X}'.format(x) for x in octets)
