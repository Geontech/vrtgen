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

def check_enable_indicators(trailer):
    assert not trailer.calibrated_time_enable.required
    assert not trailer.calibrated_time_enable.enabled
    assert not trailer.valid_data_enable.enabled
    assert not trailer.valid_data_enable.required
    assert not trailer.reference_lock_enable.enabled
    assert not trailer.reference_lock_enable.required
    assert not trailer.agc_mgc_enable.enabled
    assert not trailer.agc_mgc_enable.required
    assert not trailer.detected_signal_enable.enabled
    assert not trailer.detected_signal_enable.required
    assert not trailer.spectral_inversion_enable.enabled
    assert not trailer.spectral_inversion_enable.required
    assert not trailer.over_range_enable.enabled
    assert not trailer.over_range_enable.required
    assert not trailer.sample_loss_enable.enabled
    assert not trailer.sample_loss_enable.required
    assert not trailer.associated_context_packets_enable.enabled
    assert not trailer.associated_context_packets_enable.required

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
    assert not trailer.calibrated_time.enabled
    assert not trailer.calibrated_time.required
    assert not trailer.valid_data.enabled
    assert not trailer.valid_data.required
    assert not trailer.reference_lock.enabled
    assert not trailer.reference_lock.required
    assert not trailer.agc_mgc.enabled
    assert not trailer.agc_mgc.required
    assert not trailer.detected_signal.enabled
    assert not trailer.detected_signal.required
    assert not trailer.spectral_inversion.enabled
    assert not trailer.spectral_inversion.required
    assert not trailer.over_range.enabled
    assert not trailer.over_range.required
    assert not trailer.sample_loss.enabled
    assert not trailer.sample_loss.required
    assert not trailer.associated_context_packet_count.enabled
    assert not trailer.associated_context_packet_count.required
    # Enable Indicators
    check_enable_indicators(trailer)

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
    ('associated_context_packet_count', 'required'),
    ('associated_context_packet_count', 'optional')
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
    # Enable Indicators
    check_enable_indicators(trailer)

def test_trailer_invalid_field():
    document = """
    trailer: !Trailer
        invalid_field: 5
    """
    with pytest.raises(ValueError) as e_info:
        parse_document(document)

invalid_field_data = [
    ('calibrated_time_enable', 'required'),
    ('valid_data_enable', 'required'),
    ('reference_lock_enable', 'required'),
    ('agc_mgc_enable', 'required'),
    ('detected_signal_enable', 'required'),
    ('spectral_inversion_enable', 'required'),
    ('over_range_enable', 'required'),
    ('sample_loss_enable', 'required'),
    ('associated_context_packets_enable', 'required'),
]

@pytest.mark.parametrize('parameter,mode', invalid_field_data)
def test_trailer_invalid_field(parameter, mode):
    document = """
    trailer: !Trailer
        {}: {}
    """.format(parameter, mode)
    with pytest.raises(ValueError) as e_info:
        parse_document(document)