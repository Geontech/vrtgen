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
Classes for defining struct data types.
"""
import struct
import warnings

from . import basic
from .container import Container, ContainerItem

class BitPosition:
    """
    Position of a bit within a VRT structure by word and bit.

    Instances of this class are immutable.
    """
    __slots__ = ('_bit', '_word')
    def __init__(self, bit, word=0):
        self._bit = bit
        self._word = word

    @property
    def word(self):
        """
        VRT word number of this position.
        """
        return self._word

    @property
    def bit(self):
        """
        Bit number from within VRT word.

        Bits are numbered in descending order from MSB (31) to LSB (0).
        """
        return self._bit

    @property
    def offset(self):
        """
        Absolute bit offset from the MSB of the first word.

        Bit offsets are numbered in ascending order, starting at 0 for the MSB
        of the first word.
        """
        return self.word * 32 + (31 - self.bit)

    @classmethod
    def from_offset(cls, offset):
        """
        Creates a BitPosition from an absolute bit offset.
        """
        word = offset // 32
        bit = 31 - (offset % 32)
        return BitPosition(bit, word)

    def __eq__(self, other):
        return self.offset == other.offset

    def __lt__(self, other):
        return self.offset < other.offset

    def __le__(self, other):
        return (self == other) or (self < other)

    def __gt__(self, other):
        return not self <= other

    def __ge__(self, other):
        return not self < other

    def __str__(self):
        return '{}/{}'.format(self.word, self.bit)

    def __repr__(self):
        return '{}({},{})'.format(self.__class__.__name__, self.bit, self.word)

class StructItem(ContainerItem):
    """
    Base class for objects that require space in a binary structure.
    """
    __slots__ = ('_position',)
    def __init__(self, name, datatype, editable):
        super().__init__(name, datatype, editable)
        self._position = None

    @property
    def position(self):
        """
        VRT word and offset at which the first bit of this field resides.

        Can only be set once. Subsequent attempts to overwrite the value raise
        an AttributeError.
        """
        return self._position

    @position.setter
    def position(self, position):
        assert self._position is None
        self._position = position

    @property
    def bits(self):
        """
        Number of bits occupied by this struct entry.
        """
        return self.type.bits

class Enable(StructItem):
    """
    Boolean flag to enable or disable a feature.
    """
    __slots__ = ('indicator',)
    def __init__(self, name=None, bits=1):
        # In some cases, such as Sample Frame in the data trailer, an enable
        # may be more than one bit. All bits must be set or clear to indicate
        # the state of the enable.
        super().__init__(name, basic.BooleanType.create(bits), False)
        self.indicator = None

    def __set_name__(self, owner, name):
        # By the time the attribute name is being set (i.e., after the class
        # namespace is complete), there must be a linked indicator field.
        assert self.indicator is not None
        super().__set_name__(owner, name)

    def link(self, field):
        """
        Creates a field associated with this enable.

        If this enable field does not already have a name, sets the name to
        the linked field's name.
        """
        assert self.indicator is None
        if self.name is None:
            self.name = field.name
        self.indicator = field

class Reserved(StructItem):
    """
    Reserved bits in a struct. Always 0.
    """
    def __init__(self, bits):
        super().__init__('<reserved>', basic.IntegerType.create(bits), False)

    def __set__(self, instance, value):
        raise AttributeError('reserved fields cannot be set')

    def rebind(self, field):
        """
        Replaces these reserved bits with another field.
        """
        assert self.bits == field.bits
        field.position = self.position
        return field

class Field(StructItem):
    """
    Data field in a struct.
    """
    __slots__ = ('_unused', 'enable')
    def __init__(self, name, datatype, unused=None, enable=None):
        super().__init__(name, datatype, True)
        self._unused = unused
        self.enable = enable
        if enable is not None:
            enable.link(self)

    def __get__(self, instance, owner):
        if instance is not None and self.enable:
            if not self.enable.__get__(instance, owner):
                return None
        return super().__get__(instance, owner)

    def __set__(self, instance, value):
        if self.enable:
            if value is None:
                self.enable.__set__(instance, False)
                return
            self.enable.__set__(instance, True)
        super().__set__(instance, value)

class Struct(Container):
    """
    Base class for structures that have a well-defined binary layout.
    """
    # Initialize size to 0, subclasses will update their own value
    bits = 0

    @classmethod
    def _add_field(cls, field):
        """
        Adds a field to a struct class definition.
        """
        assert isinstance(field, StructItem)

        # Check for rebound fields, usually from a base class
        if field.position is not None:
            cls._replace_field(field)
            return

        position = BitPosition.from_offset(cls.bits)
        if not cls.check_alignment(position.bit, field.bits):
            msg = '{}.{} is not naturally aligned'.format(cls.__name__, field.attr)
            warnings.warn(msg)
        field.position = position
        cls.bits += field.bits
        super()._add_field(field)

    @classmethod
    def _replace_field(cls, field):
        index, current = cls._find_field(field.position)
        if current is None:
            cls.bits += field.bits
            super()._add_field(field)
        else:
            cls._contents[index] = field
            setattr(cls, current.attr, field)

    @classmethod
    def _find_field(cls, position):
        for index, current in enumerate(cls._contents):
            if current.position == position:
                return index, current
        return -1, None

    @staticmethod
    def check_alignment(offset, bits):
        """
        Returns true if a field  is naturally aligned within a struct based on
        its starting offset.
        """
        # No alignment concerns for sub-byte fields
        if bits < 8:
            return True

        # Adjust from 0-based to 1-based indexing to determine alignment
        pos = offset + 1
        if bits in (8, 16, 32):
            # Common power-of-two integer sizes should be naturally aligned
            return (pos % bits) == 0
        if bits == 24:
            # 24-bit integers just need to be 8-bit aligned, presumably since
            # they are not a "natural" register size
            return (pos % 8) == 0
        if bits == 64:
            # 64-bit ints must be at least aligned to a 32-bit word boundary,
            # but full 64-bit alignment does not appear to be a concern
            return pos == 32

        # Other cases TBD
        return True

    @classmethod
    def _validate(cls):
        """
        Checks that a struct class is an exact multiple of the VITA 49 word
        size (32 bits).
        """
        if cls.bits % 32:
            msg = '{}.{} does not end on a word boundary'.format(cls.__module__, cls.__qualname__)
            warnings.warn(msg)

    def pack(self):
        """
        Packs field values into binary format.
        """
        data = bytes()
        bits = 0
        word = 0
        for field in self.get_contents():
            value = field.__get__(self, type(self))
            # Disabled indicator fields report a value of None, but should be
            # packed as 0
            if value is None:
                value = 0
            if hasattr(value, 'to_binary'):
                value = value.to_binary()
            # Mask off any sign bits (also defensively protect against broken
            # fields that exceed their allotted bits)
            mask = (1 << field.bits) - 1
            word = (word << field.bits) | (value & mask)
            bits += field.bits
            if bits % 32:
                # Only pack on word boundaries
                continue
            if bits == 32:
                fmt = 'L'
            elif bits == 64:
                fmt = 'Q'
            data += struct.pack('>'+fmt, word)
            word = 0
            bits = 0
        return data
