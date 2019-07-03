from vrtgen.types import enums
from vrtgen import parser

def parse_single(document):
    packets = list(parser.parse_stream(document))
    assert len(packets) == 1
    return packets[0]

def test_data_defaults():
    document = """
TestData:
    type: data
"""
    packet = parse_single(document)
    assert packet.name == 'TestData'
    assert packet.tsi == enums.TSI.NONE
    assert packet.tsf == enums.TSF.NONE
    assert packet.stream_id.is_disabled
    assert packet.class_id.is_disabled

def test_context_defaults():
    document = """
TestContext:
    type: context
"""
    packet = parse_single(document)
    assert packet.name == 'TestContext'
    assert packet.tsi == enums.TSI.NONE
    assert packet.tsf == enums.TSF.NONE
    assert packet.stream_id.is_mandatory
    assert packet.class_id.is_disabled

def test_command_defaults():
    document = """
TestCommand:
    type: command
"""
    packet = parse_single(document)
    assert packet.name == 'TestCommand'
    assert packet.tsi == enums.TSI.NONE
    assert packet.tsf == enums.TSF.NONE
    assert packet.stream_id.is_mandatory
    assert packet.class_id.is_disabled
