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
from vrtgen.parser.loader import get_loader
from vrtgen.parser.model.types.enums import *

def check_basics(name, cif7):
    assert name == 'cif_7'
    assert cif7.name == 'cif_7'
    assert cif7.enabled
    assert cif7.required

def test_cif7_default():
    document = """
    cif_7: !CIF7
    """
    name,cif7 = parse_document(document)
    check_basics(name, cif7)

    assert not cif7.has_enabled_fields
    assert not cif7.all_optional_fields

    assert not cif7.attributes.enabled
    assert not cif7.attributes.current_value.enabled
    assert not cif7.attributes.current_value.required
    assert cif7.attributes.current_value.indicator_only
    assert not cif7.attributes.mean_value.enabled
    assert not cif7.attributes.mean_value.required
    assert not cif7.attributes.median_value.enabled
    assert not cif7.attributes.median_value.required
    assert not cif7.attributes.standard_deviation.enabled
    assert not cif7.attributes.standard_deviation.required
    assert not cif7.attributes.max_value.enabled
    assert not cif7.attributes.max_value.required
    assert not cif7.attributes.min_value.enabled
    assert not cif7.attributes.min_value.required
    assert not cif7.attributes.precision.enabled
    assert not cif7.attributes.precision.required
    assert not cif7.attributes.accuracy.enabled
    assert not cif7.attributes.accuracy.required
    assert not cif7.attributes.first_derivative.enabled
    assert not cif7.attributes.first_derivative.required
    assert not cif7.attributes.second_derivative.enabled
    assert not cif7.attributes.second_derivative.required
    assert not cif7.attributes.third_derivative.enabled
    assert not cif7.attributes.third_derivative.required

    assert not cif7.attributes.probability.enabled
    assert not cif7.attributes.probability.required
    assert not cif7.attributes.belief.enabled
    assert not cif7.attributes.belief.required

field_data = [
    ('current_value', 'required'),
    ('current_value', 'optional'),
    ('mean_value', 'required'),
    ('mean_value', 'optional'),
    ('median_value', 'required'),
    ('median_value', 'optional'),
    ('standard_deviation', 'required'),
    ('standard_deviation', 'optional'),
    ('max_value', 'required'),
    ('max_value', 'optional'),
    ('min_value', 'required'),
    ('min_value', 'optional'),
    ('precision', 'required'),
    ('precision', 'optional'),
    ('accuracy', 'required'),
    ('accuracy', 'optional'),
    ('first_derivative', 'required'),
    ('first_derivative', 'optional'),
    ('second_derivative', 'required'),
    ('second_derivative', 'optional'),
    ('third_derivative', 'required'),
    ('third_derivative', 'optional'),
    ('probability', 'required'),
    ('probability', 'optional'),
    ('belief', 'required'),
    ('belief', 'optional'),
]

@pytest.mark.parametrize('parameter,mode', field_data)
def test_cif7_fields(parameter, mode):
    document = """
    cif_7: !CIF7
        {}: {}
    """.format(parameter, mode)
    name,cif7 = parse_document(document)
    check_basics(name, cif7)

    assert hasattr(cif7.attributes, parameter)
    assert getattr(cif7.attributes, parameter).enabled
    assert cif7.has_enabled_fields
    assert not cif7.all_optional_fields
    assert getattr(cif7.attributes, parameter).required
        
    for field in [f for f in cif7.attributes.fields if f.name != parameter and not isinstance(f, PackedType) and not f.indicator_only]:
        assert not field.enabled
        assert not field.required

def test_cif7_fields():
    document = """
    cif_7: !CIF7
        probability: required
        belief: required
    """
    name,cif7 = parse_document(document)
    check_basics(name, cif7)

    probability = cif7.attributes.probability.type_
    belief = cif7.attributes.belief.type_

    assert probability.is_probability_type
    assert probability.bits == 32
    assert probability.probability_function.enabled
    assert probability.probability_function.required
    assert probability.probability_percent.enabled
    assert probability.probability_percent.required
    assert probability.name == 'probability'
    assert probability.type_ == 'Probability'
    assert belief.is_belief_type
    assert belief.bits == 32
    assert belief.belief_percent.enabled
    assert belief.belief_percent.required
    assert belief.name == 'belief'
    assert belief.type_ == 'Belief'

def test_cif7_invalid_field():
    document = """
    cif_7: !CIF7
        invalid_field: test
    """
    with pytest.raises(ValueError):
        parse_document(document)