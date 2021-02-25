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
CIF field metadata classes.
"""
from vrtgen.types.container import Container
from vrtgen.types.struct import Struct, StructItem, Field, Reserved
from vrtgen.types import basic

__all__ = (
    'CIFFields',
)

def _create_enables(cif):
    """
    Dynamically creates a struct class for a CIF prologue structure.
    """
    namespace = {}
    for field in cif.get_contents():
        # Turn all non-reserved CIF bits into enable flags
        if isinstance(field, Reserved):
            entry = Reserved(1)
        else:
            entry = Field(field.name, basic.Boolean)
        namespace[field.attr] = entry

    # Pass the qualified name (which includes the containing class) to
    # Struct type object constructor and then remove it later. This helps
    # solve two problems:
    #   * If the struct is incorrectly sized, the warning message will
    #     indicate which CIF struct it's complaining about.
    #   * The qualified name does not appear to be set when dynamically
    #     creating a class
    qualname = cif.__name__ + '.Enables'
    enables = Struct.create_struct(qualname, namespace)
    enables.__name__ = 'Enables'
    enables.__module__ = cif.__module__
    return enables

class CIFFields(Container):
    """
    Base class for CIF fields.
    """
    def __init__(self):
        if not self.get_contents():
            raise TypeError("Can't create abstract CIFFields class")
        super().__init__()

    def __init_subclass__(cls, **kwds):
        super().__init_subclass__(**kwds)
        cls.Enables = _create_enables(cls)

    @classmethod
    def get_contents(cls):
        """
        Get the contents of this CIFFields class
        """
        # In Python 3.6+, dict preserves insertion order, so this will always
        # return the fields in the order they are defined
        return [v for v in cls.__dict__.values() if isinstance(v, StructItem)]
