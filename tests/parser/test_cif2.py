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

def check_basics(name, cif2):
    assert name == 'cif_2'
    assert cif2.name == 'cif_2'
    assert cif2.enabled
    assert cif2.required

def test_cif2_default():
    document = """
    cif_2: !CIF2
    """
    name,cif2 = parse_document(document)
    check_basics(name, cif2)

    assert not cif2.has_enabled_fields
    assert not cif2.all_optional_fields

    assert not cif2.bind.enabled
    assert not cif2.bind.required
    assert not cif2.cited_sid.enabled
    assert not cif2.cited_sid.required
    assert not cif2.sibling_sid.enabled
    assert not cif2.sibling_sid.required
    assert not cif2.parent_sid.enabled
    assert not cif2.parent_sid.required
    assert not cif2.child_sid.enabled
    assert not cif2.child_sid.required
    assert not cif2.cited_message_id.enabled
    assert not cif2.cited_message_id.required
    assert not cif2.controllee_id.enabled
    assert not cif2.controllee_id.required
    assert not cif2.controllee_uuid.enabled
    assert not cif2.controllee_uuid.required
    assert not cif2.controller_id.enabled
    assert not cif2.controller_id.required
    assert not cif2.controller_uuid.enabled
    assert not cif2.controller_uuid.required
    assert not cif2.information_source.enabled
    assert not cif2.information_source.required
    assert not cif2.track_id.enabled
    assert not cif2.track_id.required
    assert not cif2.country_code.enabled
    assert not cif2.country_code.required
    assert not cif2.operator_id.enabled
    assert not cif2.operator_id.required
    assert not cif2.platform_class.enabled
    assert not cif2.platform_class.required
    assert not cif2.platform_instance.enabled
    assert not cif2.platform_instance.required
    assert not cif2.platform_display.enabled
    assert not cif2.platform_display.required
    assert not cif2.ems_device_class.enabled
    assert not cif2.ems_device_class.required
    assert not cif2.ems_device_type.enabled
    assert not cif2.ems_device_type.required
    assert not cif2.ems_device_instance.enabled
    assert not cif2.ems_device_instance.required
    assert not cif2.modulation_class.enabled
    assert not cif2.modulation_class.required
    assert not cif2.modulation_type.enabled
    assert not cif2.modulation_type.required
    assert not cif2.function_id.enabled
    assert not cif2.function_id.required
    assert not cif2.mode_id.enabled
    assert not cif2.mode_id.required
    assert not cif2.event_id.enabled
    assert not cif2.event_id.required
    assert not cif2.function_priority_id.enabled
    assert not cif2.function_priority_id.required
    assert not cif2.communication_priority_id.enabled
    assert not cif2.communication_priority_id.required
    assert not cif2.rf_footprint.enabled
    assert not cif2.rf_footprint.required
    assert not cif2.rf_footprint_range.enabled
    assert not cif2.rf_footprint_range.required

field_data = [
    ('bind', 'required'),
    ('bind', 'optional'),
    ('cited_sid', 'required'),
    ('cited_sid', 'optional'),
    ('sibling_sid', 'required'),
    ('sibling_sid', 'optional'),
    ('parent_sid', 'required'),
    ('parent_sid', 'optional'),
    ('child_sid', 'required'),
    ('child_sid', 'optional'),
    ('cited_message_id', 'required'),
    ('cited_message_id', 'optional'),
    ('controllee_id', 'required'),
    ('controllee_id', 'optional'),
    ('controllee_uuid', 'required'),
    ('controllee_uuid', 'optional'),
    ('controller_id', 'required'),
    ('controller_id', 'optional'),
    ('controller_uuid', 'required'),
    ('controller_uuid', 'optional'),
    ('information_source', 'required'),
    ('information_source', 'optional'),
    ('track_id', 'required'),
    ('track_id', 'optional'),
    ('country_code', 'required'),
    ('country_code', 'optional'),
    ('operator_id', 'required'),
    ('operator_id', 'optional'),
    ('platform_class', 'required'),
    ('platform_class', 'optional'),
    ('platform_instance', 'required'),
    ('platform_instance', 'optional'),
    ('platform_display', 'required'),
    ('platform_display', 'optional'),
    ('ems_device_class', 'required'),
    ('ems_device_class', 'optional'),
    ('ems_device_type', 'required'),
    ('ems_device_type', 'optional'),
    ('ems_device_instance', 'required'),
    ('ems_device_instance', 'optional'),
    ('modulation_class', 'required'),
    ('modulation_class', 'optional'),
    ('modulation_type', 'required'),
    ('modulation_type', 'optional'),
    ('function_id', 'required'),
    ('function_id', 'optional'),
    ('mode_id', 'required'),
    ('mode_id', 'optional'),
    ('event_id', 'required'),
    ('event_id', 'optional'),
    ('function_priority_id', 'required'),
    ('function_priority_id', 'optional'),
    ('communication_priority_id', 'required'),
    ('communication_priority_id', 'optional'),
    ('rf_footprint', 'required'),
    ('rf_footprint', 'optional'),
    ('rf_footprint_range', 'required'),
    ('rf_footprint_range', 'optional')
]

@pytest.mark.parametrize('parameter,mode', field_data)
def test_cif2_fields(parameter, mode):
    document = """
    cif_2: !CIF2
        {}: {}
    """.format(parameter, mode)
    name,cif2 = parse_document(document)
    check_basics(name, cif2)

    assert hasattr(cif2, parameter)
    assert getattr(cif2, parameter).enabled
    assert cif2.has_enabled_fields
    if mode == 'required':
        assert getattr(cif2, parameter).required
    elif mode == 'optional':
        assert not getattr(cif2, parameter).required
        assert cif2.all_optional_fields
    
    for field in [f for f in cif2.fields if f.name != parameter]:
        assert not field.enabled
        assert not field.required

def test_cif2_invalid_field():
    document = """
    cif_2: !CIF2
        invalid_field: test
    """
    with pytest.raises(ValueError):
        parse_document(document)
