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

def check_basics(name, trailer):
    assert name == 'trailer'
    assert trailer.name == 'trailer'
    assert trailer.type_ == 'Trailer'
    assert trailer.bits == 32
    assert trailer.enabled
    assert trailer.required

def test_trailer_default():
    document = """
    trailer: !Trailer
    """
    name,trailer = parse_document(document)
    check_basics(name, trailer)
    assert not trailer.state_event_indicators.calibrated_time.enabled
    assert not trailer.state_event_indicators.calibrated_time.required
    assert not trailer.state_event_indicators.valid_data.enabled
    assert not trailer.state_event_indicators.valid_data.required
    assert not trailer.state_event_indicators.reference_lock.enabled
    assert not trailer.state_event_indicators.reference_lock.required
    assert not trailer.state_event_indicators.agc_mgc.enabled
    assert not trailer.state_event_indicators.agc_mgc.required
    assert not trailer.state_event_indicators.detected_signal.enabled
    assert not trailer.state_event_indicators.detected_signal.required
    assert not trailer.state_event_indicators.spectral_inversion.enabled
    assert not trailer.state_event_indicators.spectral_inversion.required
    assert not trailer.state_event_indicators.over_range.enabled
    assert not trailer.state_event_indicators.over_range.required
    assert not trailer.state_event_indicators.sample_loss.enabled
    assert not trailer.state_event_indicators.sample_loss.required
    assert not trailer.associated_context_packet_count.enabled
    assert not trailer.associated_context_packet_count.required
    assert len(trailer.state_event_indicators.subfields) == 0

testdata = [
    ('calibrated_time', 'required'),
    ('calibrated_time', 'optional'),
    ('valid_data', 'required'),
    ('valid_data', 'optional'),
    ('reference_lock', 'required'),
    ('reference_lock', 'optional'),
    ('agc_mgc', 'required'),
    ('agc_mgc', 'optional'),
    ('detected_signal', 'required'),
    ('detected_signal', 'optional'),
    ('spectral_inversion', 'required'),
    ('spectral_inversion', 'optional'),
    ('over_range', 'required'),
    ('over_range', 'optional'),
    ('sample_loss', 'required'),
    ('sample_loss', 'optional'),
]

@pytest.mark.parametrize('parameter,mode', testdata)
def test_trailer_state_event_fields(parameter, mode):
    document = """
    trailer: !Trailer
        {}: {}
    """.format(parameter, mode)
    name,trailer = parse_document(document)
    check_basics(name, trailer)
    assert hasattr(trailer.state_event_indicators, parameter)
    assert getattr(trailer.state_event_indicators, parameter).enabled
    if mode == 'required':
        assert getattr(trailer.state_event_indicators, parameter).required
    else:
        assert not getattr(trailer.state_event_indicators, parameter).required

testdata = [
    ('associated_context_packet_count', 'required'),
    ('associated_context_packet_count', 'optional'),
    ('sample_frame', 'required'),
    ('sample_frame', 'optional'),
]

@pytest.mark.parametrize('parameter,mode', testdata)
def test_trailer_fields(parameter, mode):
    document = """
    trailer: !Trailer
        {}: {}
    """.format(parameter, mode)
    name,trailer = parse_document(document)
    check_basics(name, trailer)
    assert hasattr(trailer, parameter)
    assert getattr(trailer, parameter).enabled
    if mode == 'required':
        assert getattr(trailer, parameter).required
    else:
        assert not getattr(trailer, parameter).required

def test_trailer_invalid_field():
    document = """
    trailer: !Trailer
        invalid_field: 5
    """
    with pytest.raises(ValueError) as e_info:
        parse_document(document)

def test_trailer_user_and_sample_error():
    document = """
    trailer: !Trailer
        valid_data: required
        sample_frame: required
        user_defined1: enable_indicator
        user_defined2: enable_indicator
        user_defined3: enable_indicator
    """
    with pytest.raises(ValueError):
        parse_document(document)

def test_trailer_user_and_sample_error_enum():
    document = """
    trailer: !Trailer
        valid_data: required
        sample_frame: required
        user_defined1: !!seq
            - one
            - two
            - three
            - four
            - five
    """
    with pytest.raises(ValueError):
        parse_document(document)

def test_trailer_largest_user_defined():
    document = """
    trailer: !Trailer
        user_defined1: !!seq
            - zero
            - one
            - two
            - three
            - four
            - five
            - six
            - seven
            - eight
            - nine
            - ten
            - eleven
            - twelve
            - thirteen
            - fourteen
            - fifteen
    """
    _, trailer = parse_document(document)
    i = 0
    for f in trailer.state_event_indicators.subfields[0].type_:
        assert f.value == i
        i += 1

def test_trailer_user_enum_too_big():
    document = """
    trailer: !Trailer
        user_defined1: !!seq
            - zero
            - one
            - two
            - three
            - four
            - five
            - six
            - seven
            - eight
            - nine
            - ten
            - eleven
            - twelve
            - thirteen
            - fourteen
            - fifteen
            - sixteen
    """
    with pytest.raises(Exception):
        parse_document(document)

def test_trailer_basic_user_defined():
    var = 'test_var'
    document = """
    trailer: !Trailer
        valid_data: required
        {}: enable_indicator
    """.format(var)
    name,trailer = parse_document(document)
    assert name == 'trailer'
    assert len(trailer.state_event_indicators.subfields) == 2
    assert trailer.state_event_indicators.valid_data.enabled
    assert trailer.state_event_indicators.valid_data.required
    assert trailer.state_event_indicators.subfields[0].name == var
    assert trailer.state_event_indicators.subfields[0].enabled
    assert trailer.state_event_indicators.subfields[0].required
    assert trailer.state_event_indicators.subfields[1].name == var + '_enable'
    assert trailer.state_event_indicators.subfields[1].enabled
    assert trailer.state_event_indicators.subfields[1].required

def test_trailer_all_four_user_defined():
    var = 'test_var'
    document = """
    trailer: !Trailer
        valid_data: required
        {}1: enable_indicator
        {}2: enable_indicator
        {}3: enable_indicator
        {}4: enable_indicator
    """.format(var, var, var, var)
    name,trailer = parse_document(document)
    assert name == 'trailer'
    assert trailer.state_event_indicators.valid_data.enabled
    assert trailer.state_event_indicators.valid_data.required
    assert len(trailer.state_event_indicators.subfields) == 2*4
    for i in range(0, 4):
        assert trailer.state_event_indicators.subfields[2*i].name == var + str(i+1)
        assert trailer.state_event_indicators.subfields[2*i].enabled
        assert trailer.state_event_indicators.subfields[2*i].required
        assert trailer.state_event_indicators.subfields[2*i+1].name == var + str(i+1) + '_enable'
        assert trailer.state_event_indicators.subfields[2*i+1].enabled
        assert trailer.state_event_indicators.subfields[2*i+1].required
        
def test_trailer_sample_and_user_defined():
    var = 'test_var'
    document = """
    trailer: !Trailer
        sample_frame: required
        {}1: enable_indicator
        {}2: enable_indicator
    """.format(var, var, var, var)
    name,trailer = parse_document(document)
    assert name == 'trailer'
    assert trailer.sample_frame.enabled
    assert trailer.sample_frame.required
    assert trailer.sample_frame.packed_tag.position == 11
    assert trailer.sample_frame.bits == 2
    assert len(trailer.state_event_indicators.subfields) == 2*2
    for i in range(0, 2):
        assert trailer.state_event_indicators.subfields[2*i].name == var + str(i+1)
        assert trailer.state_event_indicators.subfields[2*i].enabled
        assert trailer.state_event_indicators.subfields[2*i].required
        assert trailer.state_event_indicators.subfields[2*i].packed_tag.position == i + 8
        assert trailer.state_event_indicators.subfields[2*i].bits == 1
        assert trailer.state_event_indicators.subfields[2*i+1].name == var + str(i+1) + '_enable'
        assert trailer.state_event_indicators.subfields[2*i+1].enabled
        assert trailer.state_event_indicators.subfields[2*i+1].required
        assert trailer.state_event_indicators.subfields[2*i+1].packed_tag.position == i + 20
        assert trailer.state_event_indicators.subfields[2*i+1].bits == 1

def test_trailer_user_defined_list():
    var = 'test_var'
    document = """
    trailer: !Trailer
        valid_data: required
        {}: !!seq
            - one
            - two
            - three
    """.format(var)
    name,trailer = parse_document(document)
    assert name == 'trailer'
    assert trailer.state_event_indicators.valid_data.enabled
    assert trailer.state_event_indicators.valid_data.required
    assert len(trailer.state_event_indicators.subfields) == 1*2
    assert trailer.state_event_indicators.subfields[0].bits == 2
    assert trailer.state_event_indicators.subfields[0].name == var
    assert trailer.state_event_indicators.subfields[0].enabled
    assert trailer.state_event_indicators.subfields[0].required
    
    enums = [f for f in trailer.state_event_indicators.subfields[0].type_]
    assert enums[0].name == 'one'
    assert enums[0].value == 0
    assert enums[1].name == 'two'
    assert enums[1].value == 1
    assert enums[2].name == 'three'
    assert enums[2].value == 2

def test_trailer_sample_frame():
    document = """
    trailer: !Trailer
        valid_data: required
        sample_frame: required
    """
    name,trailer = parse_document(document)
    assert name == 'trailer'
    assert trailer.state_event_indicators.valid_data.enabled
    assert trailer.state_event_indicators.valid_data.required
    assert trailer.sample_frame.enabled
    assert trailer.sample_frame.required