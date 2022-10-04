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

def check_basics(name, cif0):
    assert name == 'cif_0'
    assert cif0.name == 'cif_0'
    assert cif0.enabled
    assert cif0.required

def test_cif0_default():
    document = """
    cif_0: !CIF0
    """
    name,cif0 = parse_document(document)
    check_basics(name, cif0)

    assert not cif0.has_enabled_fields
    assert not cif0.all_optional_fields

    assert cif0.change_indicator.enabled
    assert cif0.change_indicator.required
    assert cif0.change_indicator.indicator_only

    assert not cif0.reference_point_id.enabled
    assert not cif0.reference_point_id.required
    assert not cif0.bandwidth.enabled
    assert not cif0.bandwidth.required
    assert not cif0.if_ref_frequency.enabled
    assert not cif0.if_ref_frequency.required
    assert not cif0.rf_ref_frequency.enabled
    assert not cif0.rf_ref_frequency.required
    assert not cif0.rf_ref_frequency_offset.enabled
    assert not cif0.rf_ref_frequency_offset.required
    assert not cif0.if_band_offset.enabled
    assert not cif0.if_band_offset.required
    assert not cif0.reference_level.enabled
    assert not cif0.reference_level.required
    assert not cif0.gain.enabled
    assert not cif0.gain.required
    assert not cif0.over_range_count.enabled
    assert not cif0.over_range_count.required
    assert not cif0.sample_rate.enabled
    assert not cif0.sample_rate.required
    assert not cif0.timestamp_adjustment.enabled
    assert not cif0.timestamp_adjustment.required
    assert not cif0.timestamp_calibration_time.enabled
    assert not cif0.timestamp_calibration_time.required
    assert not cif0.temperature.enabled
    assert not cif0.temperature.required
    assert not cif0.device_id.enabled
    assert not cif0.device_id.required
    assert not cif0.state_event_indicators.enabled
    assert not cif0.state_event_indicators.required
    assert not cif0.signal_data_format.enabled
    assert not cif0.signal_data_format.required
    assert not cif0.formatted_gps.enabled
    assert not cif0.formatted_gps.required
    assert not cif0.formatted_ins.enabled
    assert not cif0.formatted_ins.required
    assert not cif0.ecef_ephemeris.enabled
    assert not cif0.ecef_ephemeris.required
    assert not cif0.relative_ephemeris.enabled
    assert not cif0.relative_ephemeris.required
    assert not cif0.ephemeris_ref_id.enabled
    assert not cif0.ephemeris_ref_id.required
    # assert not cif0.gps_ascii.enabled
    # assert not cif0.gps_ascii.required
    assert not cif0.context_association_lists.enabled
    assert not cif0.context_association_lists.required

field_data = [
    ('reference_point_id', 'required'),
    ('reference_point_id', 'optional'),
    ('bandwidth', 'required'),
    ('bandwidth', 'optional'),
    ('if_ref_frequency', 'required'),
    ('if_ref_frequency', 'optional'),
    ('rf_ref_frequency', 'required'),
    ('rf_ref_frequency', 'optional'),
    ('rf_ref_frequency_offset', 'required'),
    ('rf_ref_frequency_offset', 'optional'),
    ('if_band_offset', 'required'),
    ('if_band_offset', 'optional'),
    ('reference_level', 'required'),
    ('reference_level', 'optional'),
    ('gain', 'required'),
    ('gain', 'optional'),
    ('over_range_count', 'required'),
    ('over_range_count', 'optional'),
    ('sample_rate', 'required'),
    ('sample_rate', 'optional'),
    ('timestamp_adjustment', 'required'),
    ('timestamp_adjustment', 'optional'),
    ('timestamp_calibration_time', 'required'),
    ('timestamp_calibration_time', 'optional'),
    ('temperature', 'required'),
    ('temperature', 'optional'),
    ('device_id', 'required'),
    ('device_id', 'optional'),
    ('state_event_indicators', 'required'),
    ('state_event_indicators', 'optional'),
    ('signal_data_format', 'required'),
    ('signal_data_format', 'optional'),
    ('formatted_gps', 'required'),
    ('formatted_gps', 'optional'),
    ('formatted_ins', 'required'),
    ('formatted_ins', 'optional'),
    ('ecef_ephemeris', 'required'),
    ('ecef_ephemeris', 'optional'),
    ('relative_ephemeris', 'required'),
    ('relative_ephemeris', 'optional'),
    ('ephemeris_ref_id', 'required'),
    ('ephemeris_ref_id', 'optional'),
    # ('gps_ascii', 'required'),
    # ('gps_ascii', 'optional'),
    ('context_association_lists', 'required'),
    ('context_association_lists', 'optional')
]

@pytest.mark.parametrize('parameter,mode', field_data)
def test_cif0_fields(parameter, mode):
    document = """
    cif_0: !CIF0
        {}: {}
    """.format(parameter, mode)
    name,cif0 = parse_document(document)
    check_basics(name, cif0)

    assert hasattr(cif0, parameter)
    assert getattr(cif0, parameter).enabled
    assert cif0.has_enabled_fields
    assert not cif0.all_optional_fields

    if mode == 'required':
        assert getattr(cif0, parameter).required
    elif mode == 'optional':
        assert not getattr(cif0, parameter).required
        
    
    for field in [f for f in cif0.fields if f.name != parameter and not f.indicator_only]:
        assert not field.enabled
        assert not field.required

def test_cif0_invalid_field():
    document = """
    cif_0: !CIF0
        invalid_field: test
    """
    with pytest.raises(ValueError):
        parse_document(document)

change_indicator = [
    ('required'),
    ('optional'),
]

@pytest.mark.parametrize('mode', change_indicator)
def test_cif0_change_indicator(mode):
    document = """
    cif_0: !CIF0
        change_indicator: {}
    """.format(mode)
    name,cif0 = parse_document(document)
    check_basics(name, cif0)

    assert cif0.change_indicator.enabled
    assert cif0.change_indicator.required