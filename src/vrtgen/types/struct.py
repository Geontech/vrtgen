"""
Classes for defining struct data types.
"""
import warnings

from . import basic

class StructEntry:
    """
    Base class for objects that require space in a binary structure.
    """
    __slots__ = ('name', 'type', 'word', 'offset', '_attr')
    def __init__(self, name, datatype):
        self.name = name
        self.type = datatype
        self.word = None
        self.offset = None
        self._attr = None

    def __set_name__(self, owner, name):
        self._attr = '_' + name

    def _initialize(self, instance):
        setattr(instance, self._attr, self.type())

    def __get__(self, instance, owner):
        if instance is None:
            return self
        if not hasattr(instance, self._attr):
            self._initialize(instance)
        return getattr(instance, self._attr)

    def __set__(self, instance, value):
        if not isinstance(value, self.type):
            value = self.type(value)
        setattr(instance, self._attr, value)

    @property
    def bits(self):
        """
        Number of bits occupied by this struct entry.
        """
        return self.type.bits

class Enable(StructEntry):
    """
    Boolean flag to enable or disable a feature.
    """
    def __init__(self, name, bits=1):
        # In some cases, such as Sample Frame in the data trailer, an enable
        # may be more than one bit. All bits must be set or clear to indicate
        # the state of the enable.
        super().__init__(name, basic.IntegerType.create(bits))

    def __get__(self, instance, owner):
        value = super().__get__(instance, owner)
        if value is None:
            return None
        return bool(value)

    def __set__(self, instance, value):
        # -1 is a shortcut to "all bits set" for signed integers
        intval = -1 if value else 0
        super().__set__(instance, intval)

class Reserved(StructEntry):
    """
    Reserved bits in a struct. Always 0.
    """
    def __init__(self, bits):
        super().__init__('<reserved>', basic.IntegerType.create(bits))

    def __set__(self, instance, value):
        raise AttributeError('reserved fields cannot be set')

class Field(StructEntry):
    """
    Data field in a struct.
    """
    __slots__ = ('_unused',)
    def __init__(self, name, datatype, unused=None):
        super().__init__(name, datatype)
        self._unused = unused

class StructBuilder(type):
    """
    Specialized metaclass for creating a struct type.

    To create a struct, create a subclass of Struct.
    """
    def __init__(cls, name, bases, namespace):
        type.__init__(cls, name, bases, namespace)
        for attr, value in namespace.items():
            if not isinstance(value, StructEntry):
                continue
            cls.add_field(attr, value)
        cls.check_size()

    def add_field(cls, attr, field):
        """
        Adds a field to a struct class definition.
        """
        # Disable the "no member" warning because in this case cls is an
        # instance of the class we are constructing, which has a bits member
        # pylint: disable=no-member
        word = cls.bits // 32
        offset = 31 - (cls.bits % 32)
        if not cls.check_alignment(offset, field.bits):
            msg = '{}.{} is not naturally aligned'.format(cls.__name__, attr)
            warnings.warn(msg)
        cls._contents.append(field)
        field.word = word
        field.offset = offset
        cls.bits += field.bits

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

    def check_size(cls):
        """
        Checks that a struct class is an exact multiple of the VITA 49 word
        size (32 bits).
        """
        # Disable the "no member" warning because in this case cls is an
        # instance of the class we are constructing, which has a bits member
        if cls.bits % 32: # pylint: disable=no-member
            msg = '{}.{} does not end on a word boundary'.format(cls.__module__, cls.__qualname__)
            warnings.warn(msg)

class Struct(metaclass=StructBuilder):
    """
    Base class for structures that have a well-defined binary layout.
    """
    # Initialize size to 0, subclasses will update their own value
    bits = 0

    # Initialize contents to empty, subclasses will extend
    _contents = []

    def __init_subclass__(cls, *args, **kwds):
        super().__init_subclass__(*args, **kwds)
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
        Returns the complete contents of this structure, including reserved
        bits and enable flags.
        """
        return cls._contents

    @classmethod
    def get_fields(cls):
        """
        Returns all user-editable fields in this structure.

        Reserved bits and enable flags are excluded.
        """
        return [field for field in cls.get_contents() if isinstance(field, Field)]

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
