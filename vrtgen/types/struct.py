import warnings

class StructEntry:
    """
    Base class for objects that require space in a binary structure.
    """
    pass

class Enable(StructEntry):
    bits = 1
    def __init__(self, name):
        self.name = name

class Reserved(StructEntry):
    __slots__ = ('bits',)
    name = '<reserved>'
    def __init__(self, bits):
        self.bits = bits

class Field(StructEntry):
    __slots__ = ('name', 'type', '_attr')
    def __init__(self, name, type, unused=None):
        self.name = name
        self.type = type

    def __set_name__(self, cls, name):
        self._attr = '_' + name

    def _initialize(self, instance):
        setattr(instance, self._attr, self.type())

    def __get__(self, instance, owner):
        if instance is None:
            return self
        if not hasattr(instance, self._attr):
            self._initialize(instance)
        return getattr(instance, self._attr)

    @property
    def bits(self):
        return self.type.bits

class StructBuilder(type):
    def __init__(cls, name, bases, namespace):
        type.__init__(cls, name, bases, namespace)
        cls._contents = []
        word = 0
        offset = 31
        size = 0
        for attr, value in namespace.items():
            if not isinstance(value, StructEntry):
                continue
            if not StructBuilder._check_alignment(offset, value):
                msg = '{}.{} is not naturally aligned'.format(name, attr)
                warnings.warn(msg)
            cls._contents.append(value)
            value.word = word
            value.offset = offset
            size += value.bits
            offset -= value.bits
            if offset < 0:
                word += 1
                offset += 32
        cls.bits = size

    @classmethod
    def _check_alignment(cls, offset, field):
        """
        Returns true if a field  is naturally aligned within a struct based on
        its starting offset.
        """
        # Adjust from 0-based to 1-based indexing to determine alignment
        pos = offset + 1
        if field.bits < 8:
            # No alignment concerns for sub-byte fields
            return True
        elif field.bits in (8, 16, 32):
            # Common power-of-two integer sizes should be naturally aligned
            return (pos % field.bits) == 0
        elif field.bits == 24:
            # 24-bit integers just need to be 8-bit aligned, presumably since
            # they are not a "natural" register size
            return (pos % 8) == 0
        elif field.bits == 64:
            # 64-bit ints must be at least aligned to a 32-bit word boundary,
            # but full 64-bit alignment does not appear to be a concern
            return pos == 32
        else:
            # Other cases TBD
            return True

class Struct(metaclass=StructBuilder):
    @classmethod
    def get_contents(cls):
        return cls._contents

    @classmethod
    def get_fields(cls):
        return [field for field in cls.get_contents() if isinstance(field, Field)]
