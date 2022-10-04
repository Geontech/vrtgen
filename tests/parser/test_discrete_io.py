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
from vrtgen.parser.model.types.basic import EnumType
from vrtgen.parser.loader import get_loader
from vrtgen.parser.model.types.enums import *

def check_basics(name, discrete_io, bits):
    assert name == 'discrete_io_' + str(bits)
    assert discrete_io.name == 'discrete_io_' + str(bits)
    assert discrete_io.type_ == 'DiscreteIO' + str(bits)
    assert discrete_io.enabled

testdata = [
    (32),
    (64)
]

@pytest.mark.parametrize('bits', testdata)
def test_discreteio_default(bits):
    document = """
    discrete_io_{}: !DiscreteIO{}
    """.format(bits, bits)
    name,discrete_io = parse_document(document)
    check_basics(name, discrete_io, bits)
    assert not discrete_io.required
    assert len(discrete_io.subfields) == 0
    # check number of packed structs for a DiscreteIO
    assert len(discrete_io.fields) == bits/32 

    assert discrete_io.bits == bits

testdata = [
    (32, 'bool', 'required'),
    (32, 'bool', 'optional'),
    (32, 'enable_indicator', 'required'),
    (32, 'enable_indicator', 'optional'),
    (32, 2, 'required'),
    (32, 2, 'optional'),
    (64, 'bool', 'required'),
    (64, 'bool', 'optional'),
    (64, 'enable_indicator', 'required'),
    (64, 'enable_indicator', 'optional'),
    (64, 2, 'required'),
    (64, 2, 'optional'),
]

@pytest.mark.parametrize('bits,type,mode', testdata)
def test_discreteio_fields(bits,type,mode):
    document = """
    discrete_io_{}: !DiscreteIO{}
        test: {}
        mode: {}
    """.format(bits, bits, type, mode)
    name,discrete_io = parse_document(document)
    check_basics(name, discrete_io, bits)
    if mode == "required":
        assert discrete_io.required
    elif mode == "optional":
        assert not discrete_io.required

    assert discrete_io.bits == bits

testdata = [
    (32),
    (64)
]

@pytest.mark.parametrize('bits', testdata)
def test_discreteio_lists(bits):
    document = """
    discrete_io_{}: !DiscreteIO{}
        test_list: !!seq
          - zero
          - one
          - two
    """.format(bits, bits)
    name,discrete_io = parse_document(document)
    check_basics(name, discrete_io, bits)
    assert not discrete_io.required

    assert discrete_io.bits == bits
    assert discrete_io.subfields[0].value.name == 'zero'
    assert discrete_io.subfields[0].value.value == 0
    assert isinstance(discrete_io.subfields[0], EnumType)
    enums = [f for f in discrete_io.subfields[0].type_]
    seq_dict = {"zero": 0, "one": 1, "two": 2}
    cnt = 0
    for key, value in seq_dict.items():
        enum = enums[cnt]
        assert enum.name == key
        assert enum.value == value
        cnt += 1

testdata = [
    (32, 'test_list: !!seq\n          - 2'),
    (32, 'test: test'),
    (32, 'test1: 0x12345678\n        test2: 0x56789ABC'),
    (64, 'test_list: !!seq\n          - 2'),
    (64, 'test: test'),
    (64, 'test1: 0x12345678\n        test2: 0x9ABCDEF1\n        test5: 0xBEEF'),
]

@pytest.mark.parametrize('bits,invalid_field', testdata)
def test_discreteio_lists_error(bits,invalid_field):
    document = """
    discrete_io_{}: !DiscreteIO{}
        {}
    """.format(bits, bits,invalid_field)
    with pytest.raises(Exception):
        parse_document(document)