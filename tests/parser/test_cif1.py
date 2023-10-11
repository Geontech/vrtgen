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

def check_basics(name, cif1):
    assert name == 'cif_1'
    assert cif1.name == 'cif_1'
    assert cif1.enabled
    assert cif1.required

def test_cif1_default():
    document = """
    cif_1: !CIF1
    """
    name,cif1 = parse_document(document)
    check_basics(name, cif1)

    assert not cif1.has_enabled_fields
    assert not cif1.all_optional_fields

    assert not cif1.phase_offset.enabled
    assert not cif1.phase_offset.required
    assert not cif1.polarization.enabled
    assert not cif1.polarization.required
    assert not cif1.pointing_vector.enabled
    assert not cif1.pointing_vector.required
    assert not cif1.pointing_vector_structure.enabled
    assert not cif1.pointing_vector_structure.required
    assert not cif1.spatial_scan_type.enabled
    assert not cif1.spatial_scan_type.required
    assert not cif1.spatial_reference_type.enabled
    assert not cif1.spatial_reference_type.required
    assert not cif1.beam_width.enabled
    assert not cif1.beam_width.required
    assert not cif1.range.enabled
    assert not cif1.range.required
    assert not cif1.ebno_ber.enabled
    assert not cif1.ebno_ber.required
    assert not cif1.threshold.enabled
    assert not cif1.threshold.required
    assert not cif1.compression_point.enabled
    assert not cif1.compression_point.required
    assert not cif1.intercept_points.enabled
    assert not cif1.intercept_points.required
    assert not cif1.snr_noise_figure.enabled
    assert not cif1.snr_noise_figure.required
    assert not cif1.aux_frequency.enabled
    assert not cif1.aux_frequency.required
    assert not cif1.aux_gain.enabled
    assert not cif1.aux_gain.required
    assert not cif1.aux_bandwidth.enabled
    assert not cif1.aux_bandwidth.required
    assert not cif1.array_of_cifs.enabled
    assert not cif1.array_of_cifs.required
    assert not cif1.spectrum.enabled
    assert not cif1.spectrum.required
    assert not cif1.sector_step_scan.enabled
    assert not cif1.sector_step_scan.required
    assert not cif1.index_list.enabled
    assert not cif1.index_list.required
    assert not cif1.discrete_io_32.enabled
    assert not cif1.discrete_io_32.required
    assert not cif1.discrete_io_64.enabled
    assert not cif1.discrete_io_64.required
    assert not cif1.health_status.enabled
    assert not cif1.health_status.required
    assert not cif1.v49_spec_compliance.enabled
    assert not cif1.v49_spec_compliance.required
    assert not cif1.version_information.enabled
    assert not cif1.version_information.required
    assert not cif1.buffer_size.enabled
    assert not cif1.buffer_size.required

field_data = [
    ('phase_offset', 'required'),
    ('phase_offset', 'optional'),
    ('polarization', 'required'),
    ('polarization', 'optional'),
    ('pointing_vector', 'required'),
    ('pointing_vector', 'optional'),
    ('pointing_vector_structure', 'required'),
    ('pointing_vector_structure', 'optional'),
    ('spatial_scan_type', 'required'),
    ('spatial_scan_type', 'optional'),
    ('spatial_reference_type', 'required'),
    ('spatial_reference_type', 'optional'),
    ('beam_width', 'required'),
    ('beam_width', 'optional'),
    ('range', 'required'),
    ('range', 'optional'),
    ('ebno_ber', 'required'),
    ('ebno_ber', 'optional'),
    ('threshold', 'required'),
    ('threshold', 'optional'),
    ('compression_point', 'required'),
    ('compression_point', 'optional'),
    ('intercept_points', 'required'),
    ('intercept_points', 'optional'),
    ('snr_noise_figure', 'required'),
    ('snr_noise_figure', 'optional'),
    ('aux_frequency', 'required'),
    ('aux_frequency', 'optional'),
    ('aux_gain', 'required'),
    ('aux_gain', 'optional'),
    ('aux_bandwidth', 'required'),
    ('aux_bandwidth', 'optional'),
    ('array_of_cifs', 'required'),
    ('array_of_cifs', 'optional'),
    ('spectrum', 'required'),
    ('spectrum', 'optional'),
    ('discrete_io_32', 'required'),
    ('discrete_io_32', 'optional'),
    ('discrete_io_64', 'required'),
    ('discrete_io_64', 'optional'),
    ('health_status', 'required'),
    ('health_status', 'optional'),
    ('v49_spec_compliance', 'required'),
    ('v49_spec_compliance', 'optional'),
    ('version_information', 'required'),
    ('version_information', 'optional'),
    ('buffer_size', 'required'),
    ('buffer_size', 'optional'),
]

@pytest.mark.parametrize('parameter,mode', field_data)
def test_cif1_fields(parameter, mode):
    document = """
    cif_1: !CIF1
        {}: {}
    """.format(parameter, mode)
    name,cif1 = parse_document(document)
    check_basics(name, cif1)

    assert hasattr(cif1, parameter)
    assert getattr(cif1, parameter).enabled
    assert cif1.has_enabled_fields
    if mode == 'required':
        assert getattr(cif1, parameter).required
    elif mode == 'optional':
        assert not getattr(cif1, parameter).required
        assert cif1.all_optional_fields
    
    for field in [f for f in cif1.fields if f.name != parameter]:
        assert not field.enabled
        assert not field.required

def test_cif1_invalid_field():
    document = """
    cif_1: !CIF1
        invalid_field: test
    """
    with pytest.raises(ValueError):
        parse_document(document)

invalid_field_data = [
    ('sector_step_scan', 'required'),
    ('sector_step_scan', 'optional'),
    ('index_list', 'required'),
    ('index_list', 'optional'),
]

@pytest.mark.parametrize('parameter,mode', invalid_field_data)
def test_cif1_invalid_values(parameter, mode):
    document = """
    cif_1: !CIF1
        {}: {}
    """.format(parameter, mode)

    with pytest.raises(ValueError):
        parse_document(document)