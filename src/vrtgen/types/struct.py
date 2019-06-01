"""
Classes for defining struct data types.
"""
import warnings

from . import basic
from .container import Container, ContainerItem

class StructItem(ContainerItem):
    """
    Base class for objects that require space in a binary structure.
    """
    __slots__ = ('word', 'offset')
    def __init__(self, name, datatype, editable):
        super().__init__(name, datatype, editable)
        self.word = None
        self.offset = None

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
    def __init__(self, name, bits=1):
        # In some cases, such as Sample Frame in the data trailer, an enable
        # may be more than one bit. All bits must be set or clear to indicate
        # the state of the enable.
        super().__init__(name, basic.IntegerType.create(bits), False)

    def __get__(self, instance, owner):
        value = super().__get__(instance, owner)
        if value is None:
            return None
        return bool(value)

    def __set__(self, instance, value):
        # -1 is a shortcut to "all bits set" for signed integers
        intval = -1 if value else 0
        super().__set__(instance, intval)

    def link(self, *args, **kwds):
        """
        Creates a field associated with this enable.
        """
        return Field(self.name, *args, enable=self, **kwds)

class Reserved(StructItem):
    """
    Reserved bits in a struct. Always 0.
    """
    def __init__(self, bits):
        super().__init__('<reserved>', basic.IntegerType.create(bits), False)

    def __set__(self, instance, value):
        raise AttributeError('reserved fields cannot be set')

class Field(StructItem):
    """
    Data field in a struct.
    """
    __slots__ = ('_unused', 'enable')
    def __init__(self, name, datatype, unused=None, enable=None):
        super().__init__(name, datatype, True)
        self._unused = unused
        self.enable = enable

class Struct(Container):
    """
    Base class for structures that have a well-defined binary layout.
    """
    # Initialize size to 0, subclasses will update their own value
    bits = 0

    @classmethod
    def _add_field(cls, attr, field):
        """
        Adds a field to a struct class definition.
        """
        assert isinstance(field, StructItem)
        word = cls.bits // 32
        offset = 31 - (cls.bits % 32)
        if not cls.check_alignment(offset, field.bits):
            msg = '{}.{} is not naturally aligned'.format(cls.__name__, attr)
            warnings.warn(msg)
        field.word = word
        field.offset = offset
        cls.bits += field.bits
        super()._add_field(attr, field)

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
