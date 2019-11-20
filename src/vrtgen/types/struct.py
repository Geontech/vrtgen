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
    def __init__(self, bit=31, word=0):
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

    def __add__(self, bits):
        return BitPosition.from_offset(self.offset + bits)

    def __sub__(self, other):
        if isinstance(other, BitPosition):
            return self.offset - other.offset
        return BitPosition.from_offset(self.offset - other)

class StructItem(ContainerItem):
    """
    Base class for objects that require space in a binary structure.
    """
    __slots__ = ('_position',)
    def __init__(self, name, datatype, editable, position=None):
        super().__init__(name, datatype, editable)
        self._position = position

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
    def __init__(self, name=None, bits=1, position=None):
        # In some cases, such as Sample Frame in the data trailer, an enable
        # may be more than one bit. All bits must be set or clear to indicate
        # the state of the enable.
        super().__init__(name, basic.BooleanType.create(bits), False, position=position)
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
    def __init__(self, bits, position=None):
        super().__init__('<reserved>', basic.IntegerType.create(bits), False, position=position)

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
    def __init__(self, name, datatype, position=None, unused=None, enable=None):
        super().__init__(name, datatype, True, position=position)
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

class StructMeta(Container.__class__):
    """
    Metaclass for creating binary struct classes.

    On class creation, struct fields are laid out in order from the MSB of the
    first word on. If the fields have already been laid out, such as from a
    user-defined structure, the class declaration can include 'layout=False' to
    skip layout.
    """
    # Pylint has difficulties with metaclass methods, mistakenly assuming that
    # a call from the class instance is unbound. Disable the warning here since
    # metaclasses are a special case.
    #pylint: disable=no-value-for-parameter
    def __init__(cls, name, bases, namespace, layout=True, **kwds):
        super().__init__(name, bases, namespace, **kwds)
        # Fields are collected in __init__ instead of __new__ because the
        # superclass __init__ will call __set_name__ on all of the fields,
        # which is useful for warning messages, etc.
        fields = [v for v in namespace.values() if isinstance(v, StructItem)]
        if layout:
            cls._layout_fields(fields)
        else:
            cls._contents = fields
        cls.bits = cls._total_bits()
        cls._validate()

    def __call__(cls, *args, **kwds):
        if not cls._contents:
            msg = "Can't instantiate abstract struct {} with no fields".format(cls.__name__)
            raise TypeError(msg)
        return super().__call__(*args, **kwds)

    def _total_bits(cls):
        return cls._end().offset

    def _end(cls):
        if not cls._contents:
            return BitPosition()
        last = cls._contents[-1]
        return last.position + last.bits

    def _validate(cls):
        # Checks that the struct is an exact multiple of the VITA 49 word size
        # (32 bits).
        if cls.bits % 32:
            msg = '{}.{} does not end on a word boundary'.format(cls.__module__, cls.__qualname__)
            warnings.warn(msg)

        pos = BitPosition()
        for field in cls.get_contents():
            if field.position != pos:
                msg = '{}.{} at {}, expected {}'.format(
                    cls.__name__, field.attr, field.position, pos
                )
                raise TypeError(msg)
            if not StructMeta._check_alignment(field.position.bit, field.bits):
                msg = '{}.{} is not naturally aligned'.format(cls.__name__, field.attr)
                warnings.warn(msg)
            pos += field.bits

    def _layout_fields(cls, fields):
        # Start with existing fields (if the new class is derived from another
        # struct), or an empty list
        if hasattr(cls, '_contents'):
            # Make a copy to avoid modifying base class
            cls._contents = cls._contents[:]
        else:
            cls._contents = []
        for field in fields:
            if field.position is not None:
                cls._replace(field)
            else:
                field.position = cls._end()
                cls._contents.append(field)

    def _replace(cls, field):
        for index, existing in enumerate(cls._contents):
            if field.position != existing.position:
                continue

            # Same position, replace exisiting field (assumed to be
            # rebinding a reserved field, such as packet-specific bits in
            # the header)
            assert field.bits == existing.bits
            cls._contents[index] = field
            return

        raise ValueError('no field at {}'.format(field.position))

    @staticmethod
    def _check_alignment(offset, bits):
        """
        Returns true if a field is naturally aligned within a struct based on
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

class Struct(Container, metaclass=StructMeta):
    """
    Base class for structures that have a well-defined binary layout.
    """
    def __init_subclass__(cls, layout=None, **kwds):
        # This method is overridden only to swallow the 'layout' argument,
        # which is used by the metaclass.
        #pylint: disable=arguments-differ,unused-argument
        super().__init_subclass__(**kwds)

    @classmethod
    def get_contents(cls):
        return cls._contents

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

def create_struct(name, namespace):
    """
    Dynamically creates a new Struct class object.
    """
    return type(Struct)(name, (Struct,), namespace)
