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
    reserved_bits: int = 0
    packed_tag   : PackedTag = None
    packet_name  : str = ''
    accessors    : dict = field(default_factory=dict)
    desc         : dict = field(default_factory=dict)

    def __post_init__(self):
        self.accessors['getter'] = self.name
        self.accessors['setter'] = self.name
        self.desc['brief'] = str()
        self.desc['detailed'] = str()

    @property
    def is_optional(self):
        return (self.enabled and not self.required)

    @property
    def getter(self):
        return self.accessors['getter']

    @property
    def setter(self):
        return self.accessors['setter']

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
        super().__post_init__()
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