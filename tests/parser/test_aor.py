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

import pytest
from helpers.utils import parse_document
from vrtgen.parser.loader import get_loader
from vrtgen.parser.model.types.enums import *

def test_sector_step_scan_cif_default():
    document = """
    subfield_cif: !SectorStepScanCIF
    """
    name,sector_step_cif = parse_document(document)
    assert name == 'subfield_cif'
    assert sector_step_cif.type_ == 'SectorStepScanCIF'
    assert sector_step_cif.sector_number.enabled
    assert sector_step_cif.sector_number.required
    assert not sector_step_cif.sector_number.type_.value
    assert sector_step_cif.f1_start_frequency.enabled
    assert sector_step_cif.f1_start_frequency.required
    assert not sector_step_cif.f1_start_frequency.type_.value
    assert not sector_step_cif.f2_stop_frequency.enabled
    assert not sector_step_cif.f2_stop_frequency.required
    assert not sector_step_cif.f2_stop_frequency.type_.value
    assert not sector_step_cif.resolution_bandwidth.enabled
    assert not sector_step_cif.resolution_bandwidth.required
    assert not sector_step_cif.resolution_bandwidth.type_.value
    assert not sector_step_cif.tune_step_size.enabled
    assert not sector_step_cif.tune_step_size.required
    assert not sector_step_cif.tune_step_size.type_.value
    assert not sector_step_cif.number_points.enabled
    assert not sector_step_cif.number_points.required
    assert not sector_step_cif.number_points.type_.value
    assert not sector_step_cif.default_gain.enabled
    assert not sector_step_cif.default_gain.required
    assert not sector_step_cif.threshold.enabled
    assert not sector_step_cif.threshold.required
    assert not sector_step_cif.dwell_time.enabled
    assert not sector_step_cif.dwell_time.required
    assert not sector_step_cif.dwell_time.type_.value
    assert not sector_step_cif.start_time.enabled
    assert not sector_step_cif.start_time.required
    assert not sector_step_cif.time_3.enabled
    assert not sector_step_cif.time_3.required
    assert not sector_step_cif.time_3.type_.value
    assert not sector_step_cif.time_4.enabled
    assert not sector_step_cif.time_4.required
    assert not sector_step_cif.time_4.type_.value

testdata = [
    ('f2_stop_frequency', True),
    ('f2_stop_frequency', False),
    ('resolution_bandwidth', True),
    ('resolution_bandwidth', False),
    ('tune_step_size', True),
    ('tune_step_size', False),
    ('number_points', True),
    ('number_points', False),
    ('default_gain', True),
    ('default_gain', False),
    ('threshold', True),
    ('threshold', False),
    ('dwell_time', True),
    ('dwell_time', False),
    ('start_time', True),
    ('start_time', False),
    ('time_3', True),
    ('time_3', False),
    ('time_4', True),
    ('time_4', False),
]

@pytest.mark.parametrize('parameter,mode', testdata)
def test_sector_step_scan_cif_fields(parameter, mode):
    document = """
    subfield_cif: !SectorStepScanCIF
        {}: {}
    """.format(parameter, 'required' if mode else 'optional')
    name,sector_step_cif = parse_document(document)
    assert name == 'subfield_cif'
    assert hasattr(sector_step_cif, parameter)
    assert getattr(sector_step_cif, parameter).enabled
    assert getattr(sector_step_cif, parameter).required == mode
    
    assert not sector_step_cif.all_optional_fields
    assert sector_step_cif.has_enabled_fields
    
    assert sector_step_cif.sector_number.enabled
    assert sector_step_cif.sector_number.required
    assert sector_step_cif.f1_start_frequency.enabled
    assert sector_step_cif.f1_start_frequency.required
    for field in [f for f in sector_step_cif.fields if f.name != parameter and f.name != 'sector_number' and f.name != 'f1_start_frequency']:
        assert not field.enabled
        assert not field.required



testdata = [
    ('sector_number', True),
    ('sector_number', False),
    ('f1_start_frequency', True),
    ('f1_start_frequency', False),
]

@pytest.mark.parametrize('parameter,mode', testdata)
def test_sector_step_scan_cif_fields(parameter, mode):
    document = """
    subfield_cif: !SectorStepScanCIF
        {}: {}
    """.format(parameter, 'required' if mode else 'optional')
    with pytest.raises(ValueError):
        parse_document(document)

def test_sector_step_scan_default():
    document = """
    sector_step_scan: !SectorStepScan
    """
    name, sector_step = parse_document(document)
    assert name == 'sector_step_scan'
    assert sector_step.type_ == 'SectorStepScan'

    assert sector_step.subfield_cif.enabled
    assert sector_step.subfield_cif.required
    assert sector_step.records.enabled
    assert sector_step.records.enabled

    assert not sector_step.records.linked_size.value
    assert len(sector_step.records.values) == 0
    assert sector_step.records.is_list

    assert sector_step.records.type_.sector_number.enabled
    assert sector_step.records.type_.sector_number.required
    assert not sector_step.records.type_.sector_number.value
    assert sector_step.records.type_.f1_start_frequency.enabled
    assert sector_step.records.type_.f1_start_frequency.required
    assert not sector_step.records.type_.f1_start_frequency.value
    assert not sector_step.records.type_.f2_stop_frequency.enabled
    assert not sector_step.records.type_.f2_stop_frequency.required
    assert not sector_step.records.type_.f2_stop_frequency.value
    assert not sector_step.records.type_.resolution_bandwidth.enabled
    assert not sector_step.records.type_.resolution_bandwidth.required
    assert not sector_step.records.type_.resolution_bandwidth.value
    assert not sector_step.records.type_.tune_step_size.enabled
    assert not sector_step.records.type_.tune_step_size.required
    assert not sector_step.records.type_.tune_step_size.value
    assert not sector_step.records.type_.number_points.enabled
    assert not sector_step.records.type_.number_points.required
    assert not sector_step.records.type_.number_points.value
    assert not sector_step.records.type_.default_gain.enabled
    assert not sector_step.records.type_.default_gain.required
    assert not sector_step.records.type_.threshold.enabled
    assert not sector_step.records.type_.threshold.required
    assert not sector_step.records.type_.dwell_time.enabled
    assert not sector_step.records.type_.dwell_time.required
    assert not sector_step.records.type_.dwell_time.value
    assert not sector_step.records.type_.start_time.enabled
    assert not sector_step.records.type_.start_time.required
    assert not sector_step.records.type_.time_3.enabled
    assert not sector_step.records.type_.time_3.required
    assert not sector_step.records.type_.time_3.value
    assert not sector_step.records.type_.time_4.enabled
    assert not sector_step.records.type_.time_4.required
    assert not sector_step.records.type_.time_4.value

def test_index_list_default():
    document = """
    index_list: !IndexList
    """
    name, index_list = parse_document(document)
    assert name == 'index_list'
    assert index_list.name == 'index_list'
    assert index_list.type_ == 'IndexList'

    assert index_list.entry_size.enabled
    assert index_list.entry_size.required
    assert index_list.entry_size.value == 1

testdata = [
    (8, 1),
    (16, 2),
    (32, 4),
]

@pytest.mark.parametrize('bits,enum_val', testdata)
def test_index_list_entry_size(bits,enum_val):
    document = """
    index_list: !IndexList
        entry_size: {}
    """.format(bits)
    _, index_list = parse_document(document)
    assert index_list.entry_size.value == enum_val
    assert index_list.entries.type_.bits == bits
    assert index_list.template_type == 'uint{}_t'.format(bits)
    assert index_list.total_size.required
    assert index_list.num_entries.required
    assert index_list.entries.required

def test_index_list_field_error():
    document = """
    index_list: !IndexList 
        test: test
    """
    with pytest.raises(ValueError):
        parse_document(document)