"""
Base container class.
"""

__all__ = (
    'ContainerItem',
    'ContainerMeta',
    'Container',
)

class ContainerItem:
    """
    Base class for named fields in a container.
    """
    __slots__ = ('name', 'type', 'editable', '_attr')
    def __init__(self, name, datatype, editable):
        self.name = name
        self.type = datatype
        self.editable = editable
        self._attr = None

    def __set_name__(self, owner, name):
        self._attr = '_' + name

    def _initialize(self, instance):
        if self._attr is None:
            return
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

class ContainerMeta(type):
    """
    Metaclass for constructing container classes.
    """
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
    """
    Base class for container types that support dynamic field lookup by name.
    """
    # Initialize contents to empty, subclasses will extend
    _contents = []

    def __init_subclass__(cls, *args, **kwds):
        super().__init_subclass__(*args, **kwds)
        # Copy the contents list so that subclasses do not accidentally modify
        # base class contents
        cls._contents = cls._contents[:]

    @classmethod
    def _add_field(cls, attr, field):
        # pylint: disable=unused-argument
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
