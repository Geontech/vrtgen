"""
CIF field metadata classes.
"""
from .struct import Struct, StructBuilder, StructEntry, Field, Reserved
from . import basic

__all__ = ('CIFMeta',)

class CIFMeta(type):
    """
    Metaclass for CIF fields.
    """
    def __new__(cls, name, bases, namespace):
        cif = type.__new__(cls, name, bases, namespace)
        cif.Enables = CIFMeta._create_enables(namespace)
        return cif

    @staticmethod
    def _create_enables(cifdict):
        # Dynamically create a struct class for the CIF prologue structure
        namespace = {}
        for attr, field in cifdict.items():
            if not isinstance(field, StructEntry):
                continue
            # Turn all non-reserved CIF bits into enable flags
            if isinstance(field, Reserved):
                entry = Reserved(1)
            else:
                entry = Field(field.name, basic.Boolean)
            namespace[attr] = entry
        return StructBuilder('Enables', (Struct,), namespace)
