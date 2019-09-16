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
from .container import Container, ContainerMeta
from .struct import Struct, Field, Reserved
from . import basic

__all__ = (
    'CIFMeta',
    'CIFFields',
)

class CIFMeta(ContainerMeta):
    """
    Metaclass for CIF fields.
    """
    def __init__(cls, name, bases, namespace):
        super().__init__(name, bases, namespace)
        # The enables struct needs to be created in __init__, not __new__, and
        # after calling the superclass to ensure that the CIF fields have been
        # populated (i.e., get_contents() returns a non-empty list).
        cls.Enables = CIFMeta.create_enables(cls)

    @staticmethod
    def create_enables(cif):
        """
        Dynamically creates a struct class for the CIF prologue structure.
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
        # StructBuilder and then remove it later. This helps solve two
        # problems:
        #   * If the struct is incorrectly sized, the warning message will
        #     indicate which CIF struct it's complaining about.
        #   * The qualified name does not appear to be set when dynamically
        #     creating a class
        qualname = cif.__name__ + '.Enables'
        cls = ContainerMeta(qualname, (Struct,), namespace)
        cls.__name__ = 'Enables'
        return cls

class CIFFields(Container, metaclass=CIFMeta):
    """
    Base class for CIF fields.
    """
