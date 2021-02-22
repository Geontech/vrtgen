# Copyright (C) 2019 Geon Technologies, LLC
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

from vrtgen.types import enums
from vrtgen.model import config
from vrtgen import parser

def parse_document(document):
    packets = list(parser.parse_stream(document))
    assert len(packets) == 1
    return packets[0]

def test_data_defaults():
    document = """
TestData:
  data
"""
    packet = parse_document(document)
    assert packet.name == 'TestData'
    assert packet.config_type == config.PacketType.DATA
    assert packet.packet_type_code == enums.PacketType.SIGNAL_DATA
    assert packet.tsi == enums.TSI.NONE
    assert packet.tsf == enums.TSF.NONE
    assert packet.stream_id.is_disabled
    assert packet.class_id.is_disabled
    assert not packet.not_v49d0
    assert not packet.spectrum

def test_data_stream_id():
    document = """
TestDataStreamID:
  data:
    stream-id: required
"""
    packet = parse_document(document)
    assert packet.name == 'TestDataStreamID'
    assert packet.stream_id.is_required
    assert packet.config_type == config.PacketType.DATA
    assert packet.packet_type_code == enums.PacketType.SIGNAL_DATA_STREAM_ID

def test_data_header():
    document = """
DataHeaderFields:
  data:
    not-v49d0: true
    spectrum: true
"""
    packet = parse_document(document)
    assert packet.config_type == config.PacketType.DATA
    assert packet.not_v49d0
    assert packet.spectrum

def test_context_defaults():
    document = """
TestContext:
  context
"""
    packet = parse_document(document)
    assert packet.name == 'TestContext'
    assert packet.config_type == config.PacketType.CONTEXT
    assert packet.packet_type_code == enums.PacketType.CONTEXT
    assert packet.tsi == enums.TSI.NONE
    assert packet.tsf == enums.TSF.NONE
    assert packet.stream_id.is_mandatory
    assert packet.class_id.is_disabled
    assert packet.tsm == enums.TSM.FINE
    assert not packet.not_v49d0

def test_context_header():
    document = """
ContextHeaderFields:
  context:
    tsm: coarse
    not-v49d0: true
"""
    packet = parse_document(document)
    assert packet.tsm == enums.TSM.COARSE
    assert packet.not_v49d0

def test_context_all_cif0():
    document = """
ContextAllCIF0:
  context:
    cif0:
      reference-point-id: optional
      bandwidth: optional
      if-reference-frequency: optional
      rf-reference-frequency: optional
      rf-reference-frequency-offset: optional
      if-band-offset: optional
      reference-level: optional
      gain: optional
      over-range-count: optional
      sample-rate: optional
      timestamp-adjustment: optional
      timestamp-calibration-time: optional
      temperature: optional
      device-id: optional
      state-event-indicators: optional
      signal-data-format: optional
      formatted-gps: optional
      formatted-ins: optional
      ecef-ephemeris: optional
      relative-ephemeris: optional
      ephemeris-reference-id: optional
      gps-ascii: optional
      context-assocation-list: optional
      cif7-enable: optional
      cif3-enable: optional
      cif1-enable: optional
"""
    packet = parse_document(document)
    assert packet.name == 'ContextAllCIF0'
    assert packet.config_type == config.PacketType.CONTEXT
    assert packet.packet_type_code == enums.PacketType.CONTEXT

def test_control_defaults():
    document = """
TestControl:
  control
"""
    packet = parse_document(document)
    assert packet.name == 'TestControl'
    assert packet.config_type == config.PacketType.CONTROL
    assert packet.packet_type_code == enums.PacketType.COMMAND
    assert packet.tsi == enums.TSI.NONE
    assert packet.tsf == enums.TSF.NONE
    assert packet.stream_id.is_mandatory
    assert packet.class_id.is_disabled
    assert not packet.ackv.value
    assert not packet.ackx.value
    assert not packet.acks.value

def test_ackv_defaults():
    document = """
TestAckV:
  ackv
"""
    packet = parse_document(document)
    assert packet.name == 'TestAckV'
    assert packet.config_type == config.PacketType.ACKV
    assert packet.packet_type_code == enums.PacketType.COMMAND
    assert packet.tsi == enums.TSI.NONE
    assert packet.tsf == enums.TSF.NONE
    assert packet.stream_id.is_mandatory
    assert packet.class_id.is_disabled
    assert packet.ackv.value
    assert not packet.ackx.value
    assert not packet.acks.value

def test_ackx_defaults():
    document = """
TestAckX:
  ackx
"""
    packet = parse_document(document)
    assert packet.name == 'TestAckX'
    assert packet.config_type == config.PacketType.ACKX
    assert packet.packet_type_code == enums.PacketType.COMMAND
    assert packet.tsi == enums.TSI.NONE
    assert packet.tsf == enums.TSF.NONE
    assert packet.stream_id.is_mandatory
    assert packet.class_id.is_disabled
    assert not packet.ackv.value
    assert packet.ackx.value
    assert not packet.acks.value

def test_acks_defaults():
    document = """
TestAckS:
  acks
"""
    packet = parse_document(document)
    assert packet.name == 'TestAckS'
    assert packet.config_type == config.PacketType.ACKS
    assert packet.packet_type_code == enums.PacketType.COMMAND
    assert packet.tsi == enums.TSI.NONE
    assert packet.tsf == enums.TSF.NONE
    assert packet.stream_id.is_mandatory
    assert packet.class_id.is_disabled
    assert not packet.ackv.value
    assert not packet.ackx.value
    assert packet.acks.value

def test_timestamp():
    document = """
TimestampTest:
  data:
    timestamp:
      integer: utc
      fractional: picoseconds
"""
    packet = parse_document(document)
    assert packet.tsi == enums.TSI.UTC
    assert packet.tsf == enums.TSF.REAL_TIME

def test_class_identifier():
    document = """
TestDataClassID:
  data:
    stream-id: required
    class-id: required
"""
    packet = parse_document(document)
    assert packet.name == 'TestDataClassID'
    assert packet.config_type == config.PacketType.DATA
    assert packet.packet_type_code == enums.PacketType.SIGNAL_DATA_STREAM_ID
    assert packet.stream_id.is_required
    assert packet.class_id.is_required

def test_extends():
    document = """
include:
  - 'tests/yamls/test-extends.yaml'

TestContextExtends:
  context:
    extends:
      - TestContext
    cif0:
      formatted-gps: required
"""
    packet = parse_document(document)
    assert packet.name == 'TestContextExtends'
    assert packet.config_type == config.PacketType.CONTEXT
    assert packet.packet_type_code == enums.PacketType.CONTEXT
    assert len(packet.extends) == 1
    field_name = enums.CIF0Fields.FORMATTED_GPS.value
    assert packet.get_field(field_name).is_required

def test_information_class():
    document = """
include:
  - 'tests/yamls/test-include.yaml'

TestInformation:
  information:
    packet-classes:
      - TestData
      - TestContext
      - TestControl
"""
    info_class = parse_document(document)
    packets = info_class.get_packets()
    assert len(packets) == 3
    packet_0 = packets[0]
    assert packet_0.name == 'TestData'
    assert packet_0.config_type == config.PacketType.DATA
    assert packet_0.packet_type_code == enums.PacketType.SIGNAL_DATA
    assert packet_0.tsi == enums.TSI.NONE
    assert packet_0.tsf == enums.TSF.NONE
    assert packet_0.stream_id.is_disabled
    assert packet_0.class_id.is_disabled
    assert not packet_0.not_v49d0
    assert not packet_0.spectrum

    packet_1 = packets[1]
    assert packet_1.name == 'TestContext'
    assert packet_1.config_type == config.PacketType.CONTEXT
    assert packet_1.packet_type_code == enums.PacketType.CONTEXT
    assert packet_1.tsi == enums.TSI.NONE
    assert packet_1.tsf == enums.TSF.NONE
    assert packet_1.stream_id.is_mandatory
    assert packet_1.class_id.is_disabled
    assert packet_1.tsm == enums.TSM.FINE
    assert not packet_1.not_v49d0

    packet_2 = packets[2]
    assert packet_2.name == 'TestControl'
    assert packet_2.config_type == config.PacketType.CONTROL
    assert packet_2.packet_type_code == enums.PacketType.COMMAND
    assert packet_2.tsi == enums.TSI.NONE
    assert packet_2.tsf == enums.TSF.NONE
    assert packet_2.stream_id.is_mandatory
    assert packet_2.class_id.is_disabled
    assert not packet_2.ackv.value
    assert not packet_2.ackx.value
    assert not packet_2.acks.value
