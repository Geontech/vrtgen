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

from .struct import Field, Reserved, Enable, Struct, BitPosition

class UserDefinedType:
    """
    Mix-in type for user-defined bits.
    """
    @classmethod
    def builder(cls):
        """
        Gets a factory class to build a struct for these user defined bits.
        """
        if not hasattr(cls, 'bits'):
            raise TypeError("Can't create user-defined struct from class with no bit size")
        #pylint: disable=no-member
        return UserDefinedBuilder(cls.bits)

class UserDefinedBuilder:
    """
    Factory class to define fields for a user-defined structure.
    """
    def __init__(self, bits):
        self.bits = bits
        self._fields = []

    def add_reserved(self, *args, **kwds):
        """
        Add reserved bits.
        """
        reserved = Reserved(*args, **kwds)
        self._add(reserved)
        return reserved

    def add_enable(self, *args, **kwds):
        """
        Add an enable flag.
        """
        enable = Enable(*args, **kwds)
        self._add(enable)
        return enable

    def add_field(self, *args, **kwds):
        """
        Add a value field.
        """
        field = Field(*args, **kwds)
        self._add(field)
        return field

    def _next_position(self, bits):
        if not self._fields:
            position = BitPosition()
        else:
            last = self._fields[-1]
            position = last.position + last.bits
        self._check_position(position, bits)
        return position

    def _check_position(self, position, bits):
        new_bits = position.offset + bits
        if new_bits > self.bits:
            raise ValueError('not enough bits')

    def _add(self, field):
        if field.position is not None:
            self._insert(field)
            return

        field.position = self._next_position(field.bits)
        self._fields.append(field)

    def _insert(self, field):
        for index, existing in enumerate(self._fields):
            if field.position == existing.position:
                raise ValueError('{} already exists at {}'.format(existing.name, existing.position))

            if field.position < existing.position:
                # New field is before existing, check that it doesn't overlap
                # and insert before
                end = field.position + field.bits
                assert end <= existing.position
                self._fields.insert(index, field)
                return

            # Check that current field ends before the new field starts
            end = existing.position + existing.bits
            assert end <= field.position

        self._fields.append(field)

    def _layout(self):
        pos = BitPosition()
        for field in self._fields:
            gap = field.position - pos
            if gap:
                reserved = Reserved(gap)
                reserved.position = pos
                yield reserved
            yield field
            pos = pos + gap + field.bits

        if pos.offset < self.bits:
            reserved = Reserved(self.bits - pos.offset)
            reserved.position = pos
            yield reserved

    def create(self, name):
        """
        Creates a struct class with the defined fields.

        User-defined fields may be sparse; any unused space will be turned into
        reserved bits.
        """
        namespace = {}
        for index, field in enumerate(self._layout()):
            if isinstance(field, Reserved):
                attr = 'reserved_{}'.format(index)
            else:
                attr = '_'.join(field.name.lower().split())
                if isinstance(field, Enable):
                    attr += '_enable'
            namespace[attr] = field
        # Create the new struct class, skipping the field layout since we've
        # already done it here
        return Struct.create_struct(name, namespace, layout=False)
