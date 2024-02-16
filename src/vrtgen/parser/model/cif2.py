from dataclasses import dataclass

from vrtgen.parser.model.types import *
from vrtgen.parser.model.base import CIF
from vrtgen.parser.model.command import ControlIdentifier
from vrtgen.parser.value import parse_enable

@dataclass
class CIF2(CIF):
    """
    CIF2 Enables
    """
    name : str = 'cif_2'
    bind : CIFEnableType = field(default_factory=lambda: CIFEnableType('bind', type_=Identifier32(), packed_tag=PackedTag(31,1,0,0)))
    cited_sid : CIFEnableType = field(default_factory=lambda: CIFEnableType('cited_sid', type_=Identifier32(), packed_tag=PackedTag(30,1,0,0)))
    sibling_sid : CIFEnableType = field(default_factory=lambda: CIFEnableType('sibling_sid', type_=Identifier32(), packed_tag=PackedTag(29,1,0,0)))
    parent_sid : CIFEnableType = field(default_factory=lambda: CIFEnableType('parent_sid', type_=Identifier32(), packed_tag=PackedTag(28,1,0,0)))
    child_sid : CIFEnableType = field(default_factory=lambda: CIFEnableType('child_sid', type_=Identifier32(), packed_tag=PackedTag(27,1,0,0)))
    cited_message_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('cited_message_id', type_=Identifier32(), packed_tag=PackedTag(26,1,0,0)))
    controllee_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('controllee_id', type_=Identifier32(), packed_tag=PackedTag(25,1,0,0)))
    controllee_uuid : CIFEnableType = field(default_factory=lambda: CIFEnableType('controllee_uuid', type_=ControlIdentifier('controllee_uuid'), packed_tag=PackedTag(24,1,0,0)))
    controller_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('controller_id', type_=Identifier32(), packed_tag=PackedTag(23,1,0,0)))
    controller_uuid : CIFEnableType = field(default_factory=lambda: CIFEnableType('controller_uuid', type_=ControlIdentifier('controller_uuid'), packed_tag=PackedTag(22,1,0,0)))
    information_source : CIFEnableType = field(default_factory=lambda: CIFEnableType('information_source', type_=Identifier32(), packed_tag=PackedTag(21,1,0,0)))
    track_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('track_id', type_=Identifier32(), packed_tag=PackedTag(20,1,0,0)))
    country_code : CIFEnableType = field(default_factory=lambda: CIFEnableType('country_code', type_=Identifier32(), packed_tag=PackedTag(19,1,0,0)))
    operator_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('operator_id', type_=Identifier32(), packed_tag=PackedTag(18,1,0,0)))
    platform_class : CIFEnableType = field(default_factory=lambda: CIFEnableType('platform_class', type_=Identifier32(), packed_tag=PackedTag(17,1,0,0)))
    platform_instance : CIFEnableType = field(default_factory=lambda: CIFEnableType('platform_instance', type_=Identifier32(), packed_tag=PackedTag(16,1,0,0)))
    platform_display : CIFEnableType = field(default_factory=lambda: CIFEnableType('platform_display', type_=Identifier32(), packed_tag=PackedTag(15,1,0,0)))
    ems_device_class : CIFEnableType = field(default_factory=lambda: CIFEnableType('ems_device_class', type_=Identifier32(), packed_tag=PackedTag(14,1,0,0)))
    ems_device_type : CIFEnableType = field(default_factory=lambda: CIFEnableType('ems_device_type', type_=Identifier32(), packed_tag=PackedTag(13,1,0,0)))
    ems_device_instance : CIFEnableType = field(default_factory=lambda: CIFEnableType('ems_device_instance', type_=Identifier32(), packed_tag=PackedTag(12,1,0,0)))
    modulation_class : CIFEnableType = field(default_factory=lambda: CIFEnableType('modulation_class', type_=Identifier32(), packed_tag=PackedTag(11,1,0,0)))
    modulation_type : CIFEnableType = field(default_factory=lambda: CIFEnableType('modulation_type', type_=Identifier32(), packed_tag=PackedTag(10,1,0,0)))
    function_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('function_id', type_=Identifier32(), packed_tag=PackedTag(9,1,0,0)))
    mode_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('mode_id', type_=Identifier32(), packed_tag=PackedTag(8,1,0,0)))
    event_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('event_id', type_=Identifier32(), packed_tag=PackedTag(7,1,0,0)))
    function_priority_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('function_priority_id', type_=Identifier32(), packed_tag=PackedTag(6,1,0,0)))
    communication_priority_id : CIFEnableType = field(default_factory=lambda: CIFEnableType('communication_priority_id', type_=Identifier32(), packed_tag=PackedTag(5,1,0,0)))
    rf_footprint : CIFEnableType = field(default_factory=lambda: CIFEnableType('rf_footprint', type_=Identifier32(), packed_tag=PackedTag(4,1,0,0)))
    rf_footprint_range : CIFEnableType = field(default_factory=lambda: CIFEnableType('rf_footprint_range', type_=Identifier32(), packed_tag=PackedTag(3,1,0,0)))
    packed_0 : PackedType = PackedType('packed_0', bits=32, packed_tag=PackedTag(0,32,0))

    def _parse_mapping(self, mapping):
        for key,val in mapping.items():
            mode = parse_enable(val)
            if key == 'controllee_uuid' or key == 'controller_uuid':
                self.__dict__[key].type_.format = IdentifierFormat.UUID
                self.__dict__[key].type_.bits = 128
            self.__dict__[key].enabled = True if (mode == Mode.REQUIRED or mode == Mode.OPTIONAL) else False
            self.__dict__[key].required = True if (mode == Mode.REQUIRED) else False
