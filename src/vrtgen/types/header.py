"""
Types used for VITA 49 packet headers.
"""

from .struct import Struct, Field, Reserved
from . import basic, enums

class ClassIdentifier(Struct):
    """
    Class Identifier (5.1.3).
    """
    pad_bits = Field('Pad Bit Count', basic.IntegerType.create(5))
    reserved_1 = Reserved(3)
    oui = Field('Organizationally Unique Identifier', basic.OUI)
    information_code = Field('Information Class Code', basic.Integer16)
    packet_code = Field('Packet Class Code', basic.Integer16)

class Header(Struct):
    """
    VRT Packet Header (5.1.1).
    """
    packet_type = Field('Packet Type', enums.PacketType)
    class_id_enable = Field('Class Identifier', basic.Boolean)
    packet_specific_26 = Reserved(1)
    packet_specific_25 = Reserved(1)
    packet_specific_24 = Reserved(1)
    tsi = Field('TSI', enums.TSI)
    tsf = Field('TSF', enums.TSF)
    packet_count = Field('Packet Count', basic.IntegerType.create(4))
    packet_size = Field('Packet Size', basic.Integer16)
