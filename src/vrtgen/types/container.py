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
Base container class.
"""

__all__ = (
    'ContainerItem',
    'Container',
)

class ContainerItem:
    """
    Base class for named fields in a container.
    """
    __slots__ = ('name', 'type', 'editable', 'owner', 'attr')
    def __init__(self, name, datatype, editable):
        self.name = name
        self.type = datatype
        self.editable = editable
        self.owner = None
        self.attr = str()

    def __set_name__(self, owner, name):
        self.owner = owner
        self.attr = name

    def _initialize(self, instance):
        if not self.attr:
            return
        setattr(instance, self._varname, self.type())

    def __get__(self, instance, owner):
        if instance is None:
            return self
        if not hasattr(instance, self._varname):
            self._initialize(instance)
        return getattr(instance, self._varname)

    def __set__(self, instance, value):
        if not isinstance(value, self.type):
            value = self.type(value)
        setattr(instance, self._varname, value)

    @property
    def _varname(self):
        return '_' + self.attr

class Container:
    """
    Base class for container types that support dynamic field lookup by name.
    """
    # Initialize contents to empty, subclasses will extend
    _contents = []

    def __init_subclass__(cls, *args, **kwds):
        super().__init_subclass__(*args)
        # Copy the contents list so that subclasses do not accidentally modify
        # base class contents
        cls._contents = cls._contents[:]

    def get_value(self, name):
        """
        Returns a field value by its VITA 49 name.
        """
        field = self.get_field(name)
        return field.__get__(self, type(self))

    def set_value(self, name, value):
        """
        Sets a field value by its VITA 49 name.
        """
        field = self.get_field(name)
        field.__set__(self, value)

    @classmethod
    def get_contents(cls):
        """
        Returns the complete contents of this container.
        """
        return cls._contents

    @classmethod
    def get_fields(cls):
        """
        Returns all user-editable fields in this container.

        Reserved bits and enable flags are excluded.
        """
        return [field for field in cls.get_contents() if field.editable]

    @classmethod
    def get_field(cls, name):
        """
        Finds a field by its VITA 49 name.

        Raises a KeyError if no such field exists.
        """
        field_name = name.casefold()
        for field in cls.get_fields():
            if field.name.casefold() == field_name:
                return field
        raise KeyError(name)
