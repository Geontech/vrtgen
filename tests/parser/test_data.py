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

from helpers.utils import parse_document
from vrtgen.parser.model.types.enums import *
from vrtgen.parser.model.data import *
import pytest

from vrtgen.parser.model.types.enums import SpectrumOrTime

def test_data_default():
    document = """data: !Data"""
    name,data = parse_document(document)
    assert name == 'data'
    assert data.type_ == 'DataPacket'
    assert not data.stream_id.enabled
    assert not data.stream_id.required
    assert not data.header.spectrum_or_time.enabled
    assert not data.header.spectrum_or_time.required
    assert data.header.packet_type.value == PacketType.SIGNAL_DATA
    assert data.header.enabled
    assert data.header.required
    assert data.header.type_ == 'DataHeader'
    assert not data.stream_id.enabled
    assert not data.stream_id.required
    assert not data.class_id.enabled
    assert not data.class_id.required
    assert not data.timestamp.enabled
    assert not data.timestamp.required
    assert not data.trailer.enabled
    assert not data.trailer.required

    assert not data.header.trailer_included.enabled
    assert not data.header.trailer_included.required
    assert not data.header.not_v49d0.enabled
    assert not data.header.not_v49d0.required
    assert not data.header.spectrum_or_time.enabled
    assert not data.header.spectrum_or_time.required

def test_data_update_header_stream_id():
    document = """
    data: !Data
        stream_id: !StreamID 0x1234
    """
    _,data = parse_document(document)
    assert data.header.packet_type.value == PacketType.SIGNAL_DATA_STREAM_ID
    assert data.stream_id.enabled
    assert data.stream_id.required
    assert not data.header.spectrum_or_time.enabled
    assert not data.header.spectrum_or_time.required

testdata = [
    ('spectrum', SpectrumOrTime.SPECTRUM),
    ('time', SpectrumOrTime.TIME),
]

@pytest.mark.parametrize('mode,enum', testdata)
def test_data_header(mode, enum):
    document = """
        data: !Data
            spectrum_or_time: {}
    """.format(mode)
    name,data = parse_document(document)
    assert data.header.spectrum_or_time.enabled
    assert data.header.spectrum_or_time.required
    assert data.header.spectrum_or_time.value == enum

def test_data_update_header_trailer():
    document = """
    data: !Data
        trailer: !Trailer
    """
    _,data = parse_document(document)
    assert data.header.trailer_included.enabled
    assert data.header.trailer_included.value

def test_data_update_header_class_id():
    document = """
    data: !Data
        class_id: !ClassID
            oui: FF-EE-DD
    """
    _,data = parse_document(document)
    assert data.header.class_id_enable.enabled
    assert data.header.class_id_enable.value

def test_data_update_header_timestamp():
    document = """
    data: !Data
        timestamp: !Timestamp
            integer: utc
            fractional: picoseconds
    """
    _,data = parse_document(document)
    assert data.header.tsi.enabled
    assert data.header.tsi.value == 0b01
    assert data.header.tsf.enabled
    assert data.header.tsf.value == 0b10

def test_data_not_v49d0():
    document = """
    data: !Data
        trailer: !Trailer
            sample_frame: required
    """
    _,data = parse_document(document)
    assert data.header.not_v49d0.enabled
    assert data.header.not_v49d0.required

# Add a test here that tests the conditions when spectrum_or_time is true
# def test_data_spectrum_or_time():
#     document = """
#     data: !Data
#         trailer: !Trailer
#             sample_frame: required
#     """
#     _,data = parse_document(document)
#     assert data.header.not_v49d0.enabled
#     assert data.header.not_v49d0.required
