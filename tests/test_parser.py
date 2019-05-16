from vrtgen.types import enums
from vrtgen.parser import FileParser

def parse_single(document):
    parser = FileParser()
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
    assert packet.prologue.tsi.value == enums.TSI.NONE
    assert packet.prologue.tsf.value == enums.TSF.NONE
    assert packet.prologue.stream_id.is_disabled
    assert packet.prologue.class_id.is_disabled

def test_context_defaults():
    document = """
TestContext:
    type: context
"""
    packet = parse_single(document)
    assert packet.name == 'TestContext'
    assert packet.prologue.tsi.value == enums.TSI.NONE
    assert packet.prologue.tsf.value == enums.TSF.NONE
    assert packet.prologue.stream_id.is_mandatory
    assert packet.prologue.class_id.is_disabled

def test_command_defaults():
    document = """
TestCommand:
    type: command
"""
    packet = parse_single(document)
    assert packet.name == 'TestCommand'
    assert packet.prologue.tsi.value == enums.TSI.NONE
    assert packet.prologue.tsf.value == enums.TSF.NONE
    assert packet.prologue.stream_id.is_mandatory
    assert packet.prologue.class_id.is_disabled
