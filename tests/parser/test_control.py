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
import yaml

def test_control_default():
    document = """control: !Control"""
    name,control = parse_document(document)
    assert name == 'control'
    assert control.stream_id.enabled
    assert control.stream_id.required

def test_control_update_header_class_id():
    document = """
    control: !Control
        class_id: !ClassID
            oui: FF-EE-DD
    """
    _,control = parse_document(document)
    assert control.header.class_id_enable.enabled
    assert control.header.class_id_enable.value

def test_control_update_header_timestamp():
    document = """
    control: !Control
        timestamp: !Timestamp
            integer: utc
            fractional: picoseconds
    """
    _,control = parse_document(document)
    assert control.header.tsi.enabled
    assert control.header.tsi.value == 0b01
    assert control.header.tsf.enabled
    assert control.header.tsf.value == 0b10
