"""
CIF field metadata classes.
"""
import inspect

from .struct import Struct, StructBuilder, StructEntry, Field, Reserved
from . import basic

__all__ = ('CIFMeta',)

class CIFMeta(type):
    """
    Metaclass for CIF fields.
    """
    def __new__(cls, name, bases, namespace):
        cif = type.__new__(cls, name, bases, namespace)
        cif.Enables = CIFMeta._create_enables(cif)
        return cif

    @staticmethod
    def _create_enables(cif):
        # Dynamically create a struct class for the CIF prologue structure
        namespace = {}
        for attr, field in inspect.getmembers(cif, lambda x: isinstance(x, StructEntry)):
            # Turn all non-reserved CIF bits into enable flags
            if isinstance(field, Reserved):
                entry = Reserved(1)
            else:
                entry = Field(field.name, basic.Boolean)
            namespace[attr] = entry
        return StructBuilder('Enables', (Struct,), namespace)
