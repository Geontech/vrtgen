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

    def __get__(self, instance, owner):
        if instance is None:
            return self
        return self.type(0)

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
    __slots__ = ('_unused', '_enable', '_alias')
    def __init__(self, name, datatype, position=None, unused=None, enable=None, alias=None):
        super().__init__(name, datatype, True, position=position)
        self._unused = unused
        self._enable = enable
        if enable is not None:
            enable.link(self)
        self._alias = alias

    @property
    def enable(self):
        """
        The linked enable bit(s) for this field.
        """
        return self._enable

    @enable.setter
    def enable(self, enable):
        assert self._enable is None
        assert enable is not None
        self._enable = enable
        enable.link(self)

    @property
    def alias(self):
        """
        The alias for the field.
        """
        return self._alias

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

def is_reserved(field):
    """
    Returns true if the argument is a reserved field.
    """
    return isinstance(field, Reserved)

def is_enable(field):
    """
    Returns true if the argument is an enable flag.
    """
    return isinstance(field, Enable)

def is_field(field):
    """
    Returns true if the argument is an editable field.
    """
    return isinstance(field, Field)

def is_struct(obj):
    """
    Returns true if the argument is a struct class type.
    """
    return obj is not None and issubclass(obj, Struct)

def _last_position(fields):
    if not fields:
        return BitPosition()
    last = fields[-1]
    return last.position + last.bits

def _check_position(self, position, bits):
    new_bits = position.offset + bits
    if new_bits > self.bits:
        raise ValueError('not enough bits')

def _merge_fields(base_fields, fields):
    # Start with the base fields and merge in the new ones
    struct_fields = base_fields[:]
    for field in fields:
        if field.position is not None:
            existing = _find_field_at(base_fields, field.position)
            _insert_field(struct_fields, field, bool(existing))
        else:
            field.position = _last_position(struct_fields)
            struct_fields.append(field)
    return struct_fields

def _find_field_at(fields, position):
    for field in fields:
        if field.position == position:
            return field
    return None

def _layout_fields(base_fields, new_fields, bits):
    pos = BitPosition()
    for field in _merge_fields(base_fields, new_fields):
        gap = field.position - pos
        if gap > 0:
            yield Reserved(gap, position=pos)
            pos += gap
        yield field
        pos = pos + field.bits

    if bits is None:
        return
    remain = bits - pos.offset
    if remain < 0:
        raise ValueError('struct too large')
    if remain > 0:
        yield Reserved(bits - pos.offset, position=pos)

def _validate_struct(cls):
    # Checks that the struct is an exact multiple of the VITA 49 word size
    # (32 bits).
    if cls.bits % 32:
        msg = '{}.{} does not end on a word boundary'.format(cls.__module__, cls.__qualname__)
        warnings.warn(msg)

    pos = BitPosition()
    for field in cls.get_contents():
        field_name = field.attr
        if not field_name:
            field_name = field.name
        if field.position != pos:
            msg = '{}.{} at {}, expected {}'.format(
                cls.__name__, field_name, field.position, pos
            )
            raise TypeError(msg)
        if not _check_alignment(field.position.bit, field.bits):
            msg = '{}.{} at {} is not naturally aligned'.format(
                cls.__name__, field_name, field.position
            )
            warnings.warn(msg)
        pos += field.bits

def _check_overlap(first, last):
    end = first.position + first.bits
    if end > last.position:
        msg = '{} overlaps {}'.format(first.name, last.name)
        raise ValueError(msg)

def _insert_field(field_list, field, replace=False):
    for index, existing in enumerate(field_list):
        if field.position == existing.position:
            if not replace:
                msg = '{} conflicts with {}'.format(field.name, existing.name)
                raise ValueError(msg)
            if field.bits != existing.bits:
                msg = '{} has different size than {}'.format(field.name, existing.name)
                raise ValueError(msg)
            assert field.bits == existing.bits
            field_list[index] = field
            return

        if field.position < existing.position:
            # New field is before existing, check that it doesn't overlap
            # and insert before
            _check_overlap(field, existing)
            field_list.insert(index, field)
            return

        # Check that current field ends before the new field starts
        _check_overlap(existing, field)

    field_list.append(field)

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

class Struct(Container):
    """
    Base class for structures that have a well-defined binary layout.
    """
    def __init_subclass__(cls, bits=None, **kwds):
        super().__init_subclass__(**kwds)
        # Find all of the new struct fields defined on this class by searching
        # through the class dict; inherited fields are in the base class dict.
        # Note that in Python 3.6+, dict preserves insertion order, so this
        # will always return the fields in the order they are defined.
        new_fields = [v for v in cls.__dict__.values() if isinstance(v, StructItem)]
        # Start with existing fields (if the new class is derived from another
        # struct), or an empty list
        base_fields = getattr(cls, '_contents', [])
        cls._contents = list(_layout_fields(base_fields, new_fields, bits))

        if not cls._contents:
            # Empty (abstract) structs do not have a size or require validation
            return

        # The total number of bits in the struct is the next bit offset
        # following the last field
        cls.bits = _last_position(cls._contents).offset

        _validate_struct(cls)

    def __init__(self):
        if not getattr(self, '_contents', False):
            name = type(self).__name__
            msg = "Can't instantiate abstract struct {} with no fields".format(name)
            raise TypeError(msg)
        super().__init__()

    @classmethod
    def get_contents(cls):
        return cls._contents

    def pack(self):
        """
        Packs field values into binary format.
        """
        # First convert field values to a sequence of 32-bit words, then use
        # struct.pack() to turn that into a binary string. This lets us easily
        # zero any unused or reserved bits.
        words = [0] * (self.bits//32)
        for field in self.get_contents():
            value = field.__get__(self, type(self))
            # Disabled indicator fields report a value of None, but will be
            # packed as 0 (implicitly, because the bits are already 0).
            if value is None:
                continue
            if hasattr(field.type, 'to_binary'):
                value = field.type.to_binary(value)
            if field.bits == 64:
                words[field.position.word] = value >> 32
                words[field.position.word+1] = value & 0xFFFFFFFF
            else:
                # Mask off any sign bits (also defensively protect against
                # broken fields that exceed their allotted bits)
                mask = (1 << field.bits) - 1
                shift = field.position.bit - field.bits + 1
                words[field.position.word] |= (value & mask) << shift
        return struct.pack('>{}L'.format(len(words)), *words)

    @classmethod
    def unpack(cls, data):
        """
        Creates an instance of this Struct deserialized from a binary string.
        """
        result = cls()
        # Unpack raw binary data as a sequence of big-endian 32-bits words,
        # then extract the values from there. Other than 64-bit values (which
        # can be formed manually from two adjacent words), all types can be
        # taken from a single word (i.e., no split values).
        words = struct.unpack('>{}L'.format(cls.bits//32), data)
        for field in result.get_contents():
            # Setting an optional field after reading the enable flag clears
            # the enable flag, so in that case, skip unpacking the value
            if is_field(field) and field.enable is not None:
                if not field.enable.__get__(result, cls):
                    continue
            if field.bits == 64:
                value = (words[field.position.word] << 32) + words[field.position.word+1]
            else:
                value = words[field.position.word]
                if field.bits < 32:
                    # Extract only the relevant bits
                    mask = (1 << field.bits) - 1
                    shift = field.position.bit - field.bits + 1
                    value = (value >> shift) & mask
            if is_reserved(field):
                assert value == 0
                continue
            if hasattr(field.type, 'from_binary'):
                value = field.type.from_binary(value)
            field.__set__(result, value)
        return result

    @classmethod
    def create_struct(cls, name, namespace, **kwds):
        """
        Dynamically creates a new Struct class object as a subclass of this
        one.
        """
        return type(cls)(name, (cls,), namespace, **kwds)
