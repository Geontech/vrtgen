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
from vrtgen.parser.model.cif1 import CIF1
from vrtgen.parser.model.cif2 import CIF2
from vrtgen.parser.model.cif7 import CIF7
import pytest
import yaml

def test_context_default():
    document = """context: !Context"""
    name,context = parse_document(document)
    assert name == 'context'
    assert context.stream_id.enabled
    assert context.stream_id.required
    assert context.header.enabled
    assert context.header.required

def test_context_update_header_class_id():
    document = """
    context: !Context
        class_id: !ClassID
            oui: FF-EE-DD
    """
    _,context = parse_document(document)
    assert context.header.class_id_enable.enabled
    assert context.header.class_id_enable.value

def test_context_update_header_timestamp():
    document = """
    context: !Context
        timestamp: !Timestamp
            integer: utc
            fractional: picoseconds
    """
    _,context = parse_document(document)
    assert context.header.tsi.enabled
    assert context.header.tsi.value == 0b01
    assert context.header.tsf.enabled
    assert context.header.tsf.value == 0b10

testdata = [
    ('fine', TSM.FINE),
    ('coarse', TSM.COARSE),
]

@pytest.mark.parametrize('input,value', testdata)
def test_context_update_header_timestamp(input,value):
    document = """
    context: !Context
        tsm: {}
    """.format(input)
    _,context = parse_document(document)
    assert context.header.tsm.enabled
    assert context.header.tsm.required
    assert context.header.tsm.value == value

testdata = [
    ('cif_1', 'CIF1'),
    ('cif_2', 'CIF2'),
    ('cif_7', 'CIF7'),
]

@pytest.mark.parametrize('variable,variable_class', testdata)
def test_context_not_v49d0(variable, variable_class):
    document = """
    context: !Context
        {}: !{}
    """.format(variable, variable_class)
    _,context = parse_document(document)
    assert context.header.not_v49d0.enabled
    assert context.header.not_v49d0.required
