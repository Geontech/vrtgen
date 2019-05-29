"""
Base container class.
"""

class ContainerItem:
    """
    Base class for objects that require space in a binary structure.
    """
    __slots__ = ('name', 'type', 'editable')
    def __init__(self, name, datatype, editable):
        self.name = name
        self.type = datatype
        self.editable = editable

class ContainerMeta(type):
    def __init__(cls, name, bases, namespace):
        type.__init__(cls, name, bases, namespace)
        for attr, value in namespace.items():
            if not isinstance(value, ContainerItem):
                continue
            cls._add_field(attr, value)
        cls._validate()

    @staticmethod
    def _validate():
        return

class Container(metaclass=ContainerMeta):
    # Initialize contents to empty, subclasses will extend
    _contents = []

    def __init_subclass__(cls, *args, **kwds):
        super().__init_subclass__(*args, **kwds)
        # Copy the contents list so that subclasses do not accidentally modify
        # base class contents
        cls._contents = cls._contents[:]

    @classmethod
    def _add_field(cls, attr, field):
        cls._contents.append(field)

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
