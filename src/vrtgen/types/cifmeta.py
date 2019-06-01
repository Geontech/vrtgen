"""
CIF field metadata classes.
"""
from .container import Container, ContainerMeta, ContainerItem
from .struct import Struct, Field, Reserved
from . import basic

__all__ = (
    'CIFMeta',
    'CIFFields',
)

class CIFMeta(ContainerMeta):
    """
    Metaclass for CIF fields.
    """
    def __new__(cls, name, bases, namespace):
        namespace['Enables'] = CIFMeta.create_enables(name, namespace)
        return type.__new__(cls, name, bases, namespace)

    @staticmethod
    def create_enables(name, cifdict):
        """
        Dynamically creates a struct class for the CIF prologue structure.
        """
        namespace = {}
        for attr, field in cifdict.items():
            if not isinstance(field, ContainerItem):
                continue
            # Turn all non-reserved CIF bits into enable flags
            if isinstance(field, Reserved):
                entry = Reserved(1)
            else:
                entry = Field(field.name, basic.Boolean)
            namespace[attr] = entry

        # Pass the qualified name (which includes the containing class) to
        # StructBuilder and then remove it later. This helps solve two
        # problems:
        #   * If the struct is incorrectly sized, the warning message will
        #     indicate which CIF struct it's complaining about.
        #   * The qualified name does not appear to be set when dynamically
        #     creating a class
        qualname = name+'.Enables'
        cls = ContainerMeta(qualname, (Struct,), namespace)
        cls.__name__ = 'Enables'
        return cls

class CIFFields(Container, metaclass=CIFMeta):
    """
    Base class for CIF fields.
    """
