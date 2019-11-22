# Copyright (C) 2019 Geon Technologies, LLC
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
Types and definitions for user-defined fields.
"""

from .struct import Struct

def create_struct(cls, name, namespace):
    """
    Creates a Struct class object for a user-defined type.

    Fields are laid out in declaration order unless they have an explicit
    position. Any unused bits are treated as reserved.
    """
    classobj = Struct.create_struct(name, namespace, bits=cls.bits)
    classobj.__module__ = cls.__module__
    return classobj

def userdefined(cls):
    """
    Decorator for classes that are set aside for user-defined bits.
    """
    cls.user_defined = True
    cls.create_struct = classmethod(create_struct)
    return cls
