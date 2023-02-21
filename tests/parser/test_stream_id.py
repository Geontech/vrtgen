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

def check_basics(name, stream_id):
    assert name == 'stream_id'
    assert stream_id.name == 'stream_id'
    assert stream_id.type_ == 'StreamIdentifier'
    assert stream_id.bits == 32
    assert stream_id.enabled
    assert stream_id.required

def test_stream_id_default():
    document = """stream_id: !StreamID"""
    name,stream_id = parse_document(document)
    check_basics(name, stream_id)
    assert not stream_id.value

def test_stream_id_value():
    document = """
stream_id: !StreamID 0x1234
"""
    name,stream_id = parse_document(document)
    check_basics(name, stream_id)
    assert stream_id.value == 0x1234

def test_stream_id_custom_packed():
    document = """
stream_id: !StreamID
  thing1: !IntegerType
    bits: 10
"""
    name,stream_id = parse_document(document)
    check_basics(name, stream_id)
    assert stream_id.thing1.name == 'thing1'
    assert stream_id.thing1.bits == 10
    assert stream_id.thing1.enabled
    assert stream_id.thing1.required
