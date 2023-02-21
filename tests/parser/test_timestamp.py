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
import pytest
from helpers.utils import parse_document
from vrtgen.parser.model.types import enums
from vrtgen.parser.loader import get_loader
from vrtgen.parser.model.types.enums import *

def check_tsi(timestamp, value):
    if value == 'none':
        assert timestamp.tsi.value == 0
    elif value == 'utc':
        assert timestamp.tsi.value == 1
    elif value == 'gps':
        assert timestamp.tsi.value == 2
    elif value == 'other':
        assert timestamp.tsi.value == 3

def check_tsf(timestamp, value):
    if value == 'none':
        assert timestamp.tsf.value == 0
    elif value == 'sample_count':
        assert timestamp.tsf.value == 1
    elif value == 'picoseconds':
        assert timestamp.tsf.value == 2
    elif value == 'free-running':
        assert timestamp.tsf.value == 3

def check_basics(name, timestamp):
    assert name == 'timestamp'
    assert timestamp.name == 'timestamp'
    assert timestamp.type_ == 'Timestamp'
    assert timestamp.enabled
    assert timestamp.required

def test_timestamp_default():
    document = """
    timestamp: !Timestamp
    """
    name,timestamp = parse_document(document)
    check_basics(name, timestamp)
    assert timestamp.bits == 0
    # Integer
    assert not timestamp.integer.enabled
    assert not timestamp.integer.required
    assert not timestamp.integer.value
    # TSI
    assert not timestamp.tsi.enabled
    assert not timestamp.tsi.required
    assert not timestamp.tsi.value
    # Fractional
    assert not timestamp.fractional.enabled
    assert not timestamp.fractional.required
    assert not timestamp.fractional.value
    # TSF
    assert not timestamp.tsf.enabled
    assert not timestamp.tsf.required
    assert not timestamp.tsf.value

testdata = [
    ('integer', 'none'),
    ('integer', 'utc'),
    ('integer', 'gps'),
    ('integer', 'other')
]

@pytest.mark.parametrize('parameter,value', testdata)
def test_timestamp_integer(parameter, value):
    document = """
    timestamp: !Timestamp
        {}: {}
    """.format(parameter, value)
    name,timestamp = parse_document(document)
    check_basics(name, timestamp)
    assert timestamp.bits == 32 # Integer part with no Fractional
    # Integer
    assert timestamp.integer.enabled
    assert timestamp.integer.required
    assert not timestamp.integer.value
    # TSI
    assert not timestamp.tsi.enabled
    assert not timestamp.tsi.required
    check_tsi(timestamp, value)
    # Fractional
    assert not timestamp.fractional.enabled
    assert not timestamp.fractional.required
    assert not timestamp.fractional.value
    # TSF
    assert not timestamp.tsf.enabled
    assert not timestamp.tsf.required
    assert timestamp.tsf.value == enums.TSF.NONE

testdata = [
    ('fractional', 'none'),
    ('fractional', 'samples'),
    ('fractional', 'picoseconds'),
    ('fractional', 'free-running')
]

@pytest.mark.parametrize('parameter,value', testdata)
def test_timestamp_fractional(parameter, value):
    document = """
    timestamp: !Timestamp
        {}: {}
    """.format(parameter, value)
    name,timestamp = parse_document(document)
    check_basics(name, timestamp)
    assert timestamp.bits == 64 # Fractional part with no Integer
    # Integer
    assert not timestamp.integer.enabled
    assert not timestamp.integer.required
    assert not timestamp.integer.value
    # TSI
    assert not timestamp.tsi.enabled
    assert not timestamp.tsi.required
    assert timestamp.tsi.value == 0
    # Fractional
    assert timestamp.fractional.enabled
    assert timestamp.fractional.required
    assert not timestamp.fractional.value
    # TSF
    assert not timestamp.tsf.enabled
    assert not timestamp.tsf.required
    check_tsf(timestamp, value)

def test_timestamp_both():
    integer_value = 'utc'
    fractional_value = 'picoseconds'
    document = """
    timestamp: !Timestamp
        integer: {}
        fractional: {}
    """.format(integer_value, fractional_value)
    name,timestamp = parse_document(document)
    check_basics(name, timestamp)
    assert timestamp.bits == 32 + 64 # Integer and Fractional
    # Integer
    assert timestamp.integer.enabled
    assert timestamp.integer.required
    assert not timestamp.integer.value
    # TSI
    assert not timestamp.tsi.enabled
    assert not timestamp.tsi.required
    check_tsi(timestamp, integer_value)
    # Fractional
    assert timestamp.fractional.enabled
    assert timestamp.fractional.required
    assert not timestamp.fractional.value
    # TSF
    assert not timestamp.tsf.enabled
    assert not timestamp.tsf.required
    check_tsf(timestamp, fractional_value)

def test_timestamp_invalid_key():
    document = """
    timestamp: !Timestamp
        invalid_key: test
    """
    with pytest.raises(KeyError) as e_info:
        parse_document(document)

def test_timestamp_invalid_integer():
    document = """
    timestamp: !Timestamp
        integer: invalid
    """
    with pytest.raises(ValueError) as e_info:
        parse_document(document)

def test_timestamp_invalid_fractional():
    document = """
    timestamp: !Timestamp
        fractional: invalid
    """
    with pytest.raises(ValueError) as e_info:
        parse_document(document)