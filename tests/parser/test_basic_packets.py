# Copyright (C) 2022 Geon Technologies, LLC
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
from helpers.utils import parse_document
from vrtgen.parser.loader import get_loader
from vrtgen.parser.model.types.enums import *

def test_data_defaults():
    document = """BasicDataPacket: !Data"""
    name,packet = parse_document(document)
    assert name == 'BasicDataPacket'
    assert packet.header.packet_type.value == PacketType.SIGNAL_DATA
    assert packet.header.packet_type.value.is_data
    assert packet.header.tsi.value == TSI.NONE
    assert packet.header.tsf.value == TSF.NONE
    assert not packet.header.trailer_included.value
    assert not packet.header.not_v49d0.value
    assert not packet.header.spectrum_or_time.value
    assert not packet.stream_id.enabled
    assert not packet.class_id.enabled
    assert not packet.timestamp.enabled
    assert not packet.trailer.enabled

def test_data_stream_id_defaults():
    document = """
BasicDataStreamIDPacket: !Data
  stream_id: !StreamID
"""
    name,packet = parse_document(document)
    assert name == 'BasicDataStreamIDPacket'
    assert packet.header.packet_type.value == PacketType.SIGNAL_DATA_STREAM_ID
    assert packet.header.tsi.value == TSI.NONE
    assert packet.header.tsf.value == TSF.NONE
    assert not packet.header.trailer_included.value
    assert not packet.header.not_v49d0.value
    assert not packet.header.spectrum_or_time.value
    assert packet.stream_id.enabled
    assert not packet.class_id.enabled
    assert not packet.timestamp.enabled
    assert not packet.trailer.enabled
    
def test_context_defaults():
    document = """BasicContextPacket: !Context"""
    name, packet = parse_document(document)
    assert name == 'BasicContextPacket'
    assert packet.header.packet_type.value == PacketType.CONTEXT
    assert packet.header.packet_type.value.is_context
    assert packet.header.tsi.value == TSI.NONE
    assert packet.header.tsf.value == TSF.NONE
    assert not packet.header.not_v49d0.value
    assert not packet.header.tsm.value
    assert packet.stream_id.enabled
    assert not packet.class_id.enabled
    assert not packet.timestamp.enabled
    assert packet.cif_0.enabled
    assert not packet.cif_1.enabled
    assert not packet.cif_2.enabled
    assert not packet.cif_7.enabled

def test_control_defaults():
    document = """BasicControlPacket: !Control"""
    name, packet = parse_document(document)
    assert name == 'BasicControlPacket'
    assert packet.header.packet_type.value == PacketType.COMMAND
    assert packet.header.packet_type.value.is_command
    assert packet.header.tsi.value == TSI.NONE
    assert packet.header.tsf.value == TSF.NONE
    assert not packet.header.acknowledge_packet.value
    assert not packet.header.cancellation_packet.value
    assert packet.stream_id.enabled
    assert not packet.class_id.enabled
    assert not packet.timestamp.enabled
    assert packet.cam.enabled
    assert packet.message_id.enabled
    assert not packet.controllee_id.enabled
    assert not packet.controller_id.enabled
    assert packet.cif_0.enabled
    assert not packet.cif_1.enabled
    assert not packet.cif_2.enabled
    assert not packet.cif_7.enabled