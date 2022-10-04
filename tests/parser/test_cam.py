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
from vrtgen.parser.model.types.packing import PackedType
from vrtgen.parser.model.types.basic import BooleanType, EnumType
from vrtgen.parser.loader import get_loader
from vrtgen.parser.model.types.enums import *

def check_basics(name, cam):
    assert name == 'cam'
    assert cam.name == 'cam'
    assert cam.type_ == 'ControlCAM'
    assert cam.enabled
    assert cam.required
    assert cam.bits == 32

def test_cam_default():
    document="""
    cam: !ControlAcknowledgeMode
    """
    name,cam = parse_document(document)
    check_basics(name, cam)

    assert not cam.controllee_enable.enabled
    assert not cam.controllee_enable.required
    assert not cam.controllee_format.enabled
    assert not cam.controllee_format.required
    assert not cam.controller_enable.enabled
    assert not cam.controller_enable.required
    assert not cam.controller_format.enabled
    assert not cam.controller_format.required
    assert not cam.permit_partial.enabled
    assert not cam.permit_partial.required
    assert not cam.permit_warnings.enabled
    assert not cam.permit_warnings.required
    assert not cam.permit_errors.enabled
    assert not cam.permit_errors.required
    assert not cam.action_mode.enabled
    assert not cam.action_mode.required
    assert not cam.nack_only.enabled
    assert not cam.nack_only.required
    assert not cam.timing_control.enabled
    assert not cam.timing_control.required
    assert not cam.req_v.enabled
    assert not cam.req_v.required
    assert not cam.req_x.enabled
    assert not cam.req_x.required
    assert not cam.req_s.enabled
    assert not cam.req_s.required
    assert not cam.req_w.enabled
    assert not cam.req_w.required
    assert not cam.req_er.enabled
    assert not cam.req_er.required

testdata = [
    ('controllee_format', 'word', 0),
    ('controllee_format', 'uuid', 1),
    ('controller_format', 'word', 0),
    ('controller_format', 'uuid', 1),
    ('permit_partial', 'true', True),
    ('permit_partial', 'false', False),
    ('permit_partial', 'required', True),
    ('permit_partial', 'optional', False),
    ('permit_warnings', 'true', True),
    ('permit_warnings', 'false', False),
    ('permit_warnings', 'required', True),
    ('permit_warnings', 'optional', False),
    ('permit_errors', 'true', True),
    ('permit_errors', 'false', False),
    ('permit_errors', 'required', True),
    ('permit_errors', 'optional', False),
    ('action_mode', 'none', 0),
    ('action_mode', 'dry-run', 1),
    ('action_mode', 'execute', 2),
    ('action_mode', 'required', True),
    ('action_mode', 'optional', False),
    ('nack_only', 'true', True),
    ('nack_only', 'false', False),
    ('nack_only', 'required', True),
    ('nack_only', 'optional', False),
    ('timing_control', 'ignore', 0),
    ('timing_control', 'device', 1),
    ('timing_control', 'late', 2),
    ('timing_control', 'early', 3),
    ('timing_control', 'early-late', 4),
    ('req_v', 'true', True),
    ('req_v', 'false', False),
    ('req_v', 'required', True),
    ('req_v', 'optional', False),
    ('req_x', 'true', True),
    ('req_x', 'false', False),
    ('req_x', 'required', True),
    ('req_x', 'optional', False),
    ('req_s', 'true', True),
    ('req_s', 'false', False),
    ('req_s', 'required', True),
    ('req_s', 'optional', False),
    ('req_w', 'true', True),
    ('req_w', 'false', False),
    ('req_w', 'required', True),
    ('req_w', 'optional', False),
    ('req_er', 'true', True),
    ('req_er', 'false', False),
    ('req_er', 'required', True),
    ('req_er', 'optional', False),
]

@pytest.mark.parametrize('parameter,mode,value', testdata)
def test_cam_fields(parameter, mode, value):
    document = """
    cam: !ControlAcknowledgeMode
        {}: {}
    """.format(parameter, mode)
    name,cam = parse_document(document)
    check_basics(name, cam)

    assert hasattr(cam, parameter)
    if isinstance(getattr(cam, parameter), BooleanType):
        if mode == 'required' or mode == 'optional':
            assert getattr(cam, parameter).enabled
        else:
            assert getattr(cam, parameter).enabled == value
        assert getattr(cam, parameter).required == value
    else:
        assert getattr(cam, parameter).enabled
        if mode == 'optional':
            assert not getattr(cam, parameter).required
        else:
            assert getattr(cam, parameter).required
    assert getattr(cam, parameter).value == value

testdata = [
    ('permit_partial', 2),
    ('req_v', 2),
    ('req_x', 2),
    ('req_s', 2),
    ('req_w', 2),
    ('req_er', 2),
    ('controllee_format', 'invalid'),
    ('controller_format', 'invalid'),
    ('action_mode', 'invalid'),
    ('timing_control', 'invalid'),
]

@pytest.mark.parametrize('parameter,value', testdata)
def test_cam_invalid_value(parameter, value):
    document = """
    cam: !ControlAcknowledgeMode
        {}: {}
    """.format(parameter, value)
    with pytest.raises(Exception):
        parse_document(document)

def test_cam_invalid_field():
    document="""
    cam: !ControlAcknowledgeMode
        test: test
    """
    with pytest.raises(ValueError):
        parse_document(document)

testdata = [
    ('controllee_enable', 'required'),
    ('controllee_enable', 'optional'),
    ('controller_enable', 'required'),
    ('controller_enable', 'optional'),
]

@pytest.mark.parametrize('parameter,mode', testdata)
def test_cam_invalid_enable(parameter, mode):
    document = """
    cam: !ControlAcknowledgeMode
        {}: {}
    """.format(parameter, mode)
    with pytest.raises(Exception):
        parse_document(document)