# Copyright (C) 2023 Geon Technologies, LLC
#
# This file is part of vrtgen.
#
# vrtgen is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# vrtgen is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
# more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/. 

import yaml
from dataclasses import make_dataclass, field

from vrtgen.parser.model import data, context, command
from vrtgen.parser.model.types import packing
from vrtgen.parser.model import prologue, cif0, cif1, cif2, cif7
from vrtgen.parser.model import information

def data_packet_constructor(loader, node) -> data.DataPacket:
    """
    Construct a DataPacket.
    """
    packet = data.DataPacket()
    if not isinstance(node, yaml.ScalarNode):
        packet.validate_and_parse_mapping(**loader.construct_mapping(node))
    return packet

def context_packet_constructor(loader, node) -> context.ContextPacket:
    """
    Construct a ContextPacket.
    """
    packet = context.ContextPacket()
    if not isinstance(node, yaml.ScalarNode):
        packet.validate_and_parse_mapping(**loader.construct_mapping(node))
    return packet

def ext_context_packet_constructor(loader, node) -> context.ExtensionContextPacket:
    """
    Construct a ContextPacket.
    """
    packet = context.ExtensionContextPacket()
    if not isinstance(node, yaml.ScalarNode):
        packet.validate_and_parse_mapping(**loader.construct_mapping(node))
    return packet

def control_packet_constructor(loader, node) -> command.ControlPacket:
    """
    Construct a CommandPacket.
    """
    packet = command.ControlPacket()
    if not isinstance(node, yaml.ScalarNode):
        packet.validate_and_parse_mapping(**loader.construct_mapping(node))
        packet._update_cam()
    return packet

def ack_packet_constructor(loader, node) -> command.AcknowledgePacket:
    """
    Construct an AcknowledgePacket.
    """
    packet = command.AcknowledgePacket()
    if not isinstance(node, yaml.ScalarNode):
        packet.validate_and_parse_mapping(**loader.construct_mapping(node))
    return packet

def ext_control_packet_constructor(loader, node) -> command.ExtensionControlPacket:
    """
    Construct an ExtensionCommandPacket.
    """
    packet = command.ExtensionControlPacket()
    if not isinstance(node, yaml.ScalarNode):
        packet.validate_and_parse_mapping(**loader.construct_mapping(node))
        packet._update_cam()
    return packet

def ext_ack_packet_constructor(loader, node) -> command.ExtensionAcknowledgePacket:
    """
    Construct an ExtensionAcknowledgePacket.
    """
    packet = command.ExtensionAcknowledgePacket()
    if not isinstance(node, yaml.ScalarNode):
        packet.validate_and_parse_mapping(**loader.construct_mapping(node))
    return packet

def stream_id_constructor(loader, node) -> prologue.StreamIdentifier:
    stream_id = prologue.StreamIdentifier(enabled=True, required=True)
    if isinstance(node, yaml.ScalarNode):
        node_scalar_value = loader.construct_scalar(node)
        if node_scalar_value:
            try:
                if node_scalar_value.startswith('0x'):
                    node_scalar_value = int(node_scalar_value, base=16)
                else:
                    node_scalar_value = int(node_scalar_value)
            except:
                raise
            if not isinstance(node_scalar_value, int):
                raise ValueError('stream_id value must be an int, not',type(node_scalar_value))
            stream_id.value = node_scalar_value
    else:
        pos = 0
        fields = [('name',str,'stream_id')]
        for key,val in loader.construct_mapping(node, deep=True).items():
            if type(val) != packing.IntegerType:
                raise TypeError('stream_id constituents must be of type !IntegerType')
            val.name = key
            pos += val.bits
            if pos > 32:
                raise ValueError('stream_id constituent bits cannot be greater than 32')
            val.packed_tag = packing.PackedTag(position=pos-1, bits=val.bits, field_word=0, packed_int=0)
            val.is_extension_type =  True
            val.user_defined = True
            fields.append((val.name, type(val), field(default_factory=lambda: val)))
        fields.append(('packed', packing.PackedType, field(default_factory=lambda: packing.PackedType('packed', bits=32, enabled=True, required=True, packed_tag=packing.PackedTag(0,32,0)))))
        packed_stream_id_type = make_dataclass(
            cls_name='StreamIdentifier',
            fields=fields,
            bases=(packing.PackedStruct,)
        )
        packed_stream_id_type.__doc__ = '''Custom Packed Stream Identifier'''
        stream_id = packed_stream_id_type(enabled=True, required=True, user_defined=True)
        stream_id.bits = 32
    return stream_id

def class_id_constructor(loader, node) -> prologue.ClassIdentifier:
    class_id = prologue.ClassIdentifier(enabled=True, required=True)
    if isinstance(node, yaml.ScalarNode):
        raise ValueError('Class ID must be a mapping')
    class_id.parse_mapping(**loader.construct_mapping(node))
    return class_id

def timestamp_constructor(loader, node) -> prologue.Timestamp:
    timestamp = prologue.Timestamp(enabled=True, required=True)
    if not isinstance(node, yaml.ScalarNode):
        timestamp.parse_mapping(**loader.construct_mapping(node))
    return timestamp

def cam_constructor(loader, node) -> command.ControlCAM:
    cam = command.ControlCAM(enabled=True, required=True)
    if not isinstance(node, yaml.ScalarNode):
        cam.validate_and_parse_mapping(**loader.construct_mapping(node))
    return cam

def cif0_constructor(loader, node) -> cif0.CIF0:
    cif_0 = cif0.CIF0(enabled=True, required=True)
    if not isinstance(node, yaml.ScalarNode):
        cif_0.validate_and_parse_mapping(**loader.construct_mapping(node, deep=True))
    return cif_0

def cif1_constructor(loader, node) -> cif1.CIF1:
    cif_1 = cif1.CIF1(enabled=True, required=True)
    if not isinstance(node, yaml.ScalarNode):
        cif_1.validate_and_parse_mapping(**loader.construct_mapping(node))
    return cif_1

def cif2_constructor(loader, node) -> cif2.CIF2:
    cif_2 = cif2.CIF2(enabled=True, required=True)
    if not isinstance(node, yaml.ScalarNode):
        cif_2.validate_and_parse_mapping(**loader.construct_mapping(node))
    return cif_2

def cif7_constructor(loader, node) -> cif7.CIF7:
    cif_7 = cif7.CIF7(enabled=True, required=True)
    if not isinstance(node, yaml.ScalarNode):
        cif_7.validate_and_parse_mapping(**loader.construct_mapping(node))
    return cif_7

def difi1p2_reference_level_constructor(loader, node) -> cif0.ReferenceLevel:
    reference_level = cif0.ReferenceLevel()
    if not isinstance(node, yaml.ScalarNode):
        reference_level.validate_and_parse_mapping(**loader.construct_mapping(node, deep=True))
    return reference_level

def sector_step_scan_constructor(loader, node) -> cif1.SectorStepScan:
    sector_step_scan = cif1.SectorStepScan(enabled=True, required=True)
    if not isinstance(node, yaml.ScalarNode):
        sector_step_scan.validate_and_parse_mapping(**loader.construct_mapping(node))
    return sector_step_scan

def sector_step_scan_cif_constructor(loader, node) -> cif1.SectorStepScanCIF:
    sector_step_scan_cif = cif1.SectorStepScanCIF(enabled=True, required=True)
    if not isinstance(node, yaml.ScalarNode):
        sector_step_scan_cif.validate_and_parse_mapping(**loader.construct_mapping(node))
    return sector_step_scan_cif

def index_list_constructor(loader, node) -> cif1.IndexList:
    index_list = cif1.IndexList(enabled=True, required=True)
    if not isinstance(node, yaml.ScalarNode):
        index_list.validate_and_parse_mapping(**loader.construct_mapping(node))
    return index_list

def discrete_io_32_constructor(loader, node) -> cif1.DiscreteIO:
    discrete_io = cif1.DiscreteIO32(enabled=True)
    if not isinstance(node, yaml.ScalarNode):
        discrete_io.validate_and_parse_mapping(**loader.construct_mapping(node, deep=True))
    return discrete_io

def discrete_io_64_constructor(loader, node) -> cif1.DiscreteIO:
    discrete_io = cif1.DiscreteIO64(enabled=True)
    if not isinstance(node, yaml.ScalarNode):
        discrete_io.validate_and_parse_mapping(**loader.construct_mapping(node, deep=True))
    return discrete_io

def trailer_constructor(loader, node) -> data.Trailer:
    trailer = data.Trailer(enabled=True, required=True)
    if not isinstance(node, yaml.ScalarNode):
        trailer.validate_and_parse_mapping(**loader.construct_mapping(node, deep=True))
    return trailer

def information_class_contructor(loader, node) -> information.InformationClass:
    info_class = information.InformationClass()
    if not isinstance(node, yaml.ScalarNode):
        info_class.validate_and_parse_mapping(**loader.construct_mapping(node, deep=True))
    return info_class

def integer_type_constructor(loader, node) -> packing.IntegerType:
    integer_type = packing.IntegerType(enabled=True, required=True)

    if not isinstance(node, yaml.ScalarNode):
        integer_type.parse_mapping(**loader.construct_mapping(node))
    return integer_type

def get_loader():
    loader = yaml.SafeLoader
    # Packet constructors
    loader.add_constructor('!Data', data_packet_constructor)
    loader.add_constructor('!Context', context_packet_constructor)
    loader.add_constructor('!ExtensionContext', ext_context_packet_constructor)
    loader.add_constructor('!Control', control_packet_constructor)
    loader.add_constructor('!Ack', ack_packet_constructor)
    loader.add_constructor('!ExtensionControl', ext_control_packet_constructor)
    loader.add_constructor('!ExtensionAck', ext_ack_packet_constructor)
    # Prologue constructors
    loader.add_constructor('!StreamID', stream_id_constructor)
    loader.add_constructor('!ClassID', class_id_constructor)
    loader.add_constructor('!Timestamp', timestamp_constructor)
    # CIF0 constructors
    loader.add_constructor('!CIF0', cif0_constructor)
    loader.add_constructor('!DIFI1p2ReferenceLevel', difi1p2_reference_level_constructor)
    # CIF1 constructors
    loader.add_constructor('!CIF1', cif1_constructor)
    loader.add_constructor('!SectorStepScan', sector_step_scan_constructor)
    loader.add_constructor('!SectorStepScanCIF', sector_step_scan_cif_constructor)
    loader.add_constructor('!IndexList', index_list_constructor)
    loader.add_constructor('!DiscreteIO32', discrete_io_32_constructor)
    loader.add_constructor('!DiscreteIO64', discrete_io_64_constructor)
    # CIF2 constructors
    loader.add_constructor('!CIF2', cif2_constructor)
    # CIF7 constructors
    loader.add_constructor('!CIF7', cif7_constructor)
    # Command constructors
    loader.add_constructor('!ControlAcknowledgeMode', cam_constructor)
    # Trailer constructor
    loader.add_constructor('!Trailer', trailer_constructor)
    # Information Class constructor
    loader.add_constructor('!InformationClass', information_class_contructor)
    # Types
    loader.add_constructor('!IntegerType', integer_type_constructor)

    return loader
