# Copyright (C) 2021 Geon Technologies, LLC
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

import logging
import yaml

from vrtgen.model.field import FieldConfiguration, Scope
from vrtgen.parser.field import UserDefinedFieldParser
from vrtgen.types.cif1 import CIF1
from vrtgen.types import basic
from vrtgen.types import struct

def _parse_field_config(document):
    options = yaml.safe_load(document)
    log = logging.getLogger()

    config = FieldConfiguration.create(CIF1.discrete_io_32, Scope.CIF1)
    parser = UserDefinedFieldParser()
    parser(log, config, options)

    user_type = config.type
    assert struct.is_struct(user_type)
    assert user_type.bits == CIF1.discrete_io_32.bits

    return user_type

def test_user_field():
    document = """
fields:
    - first
    - reserved
    - second:
        bits: 2
    - reserved: 2
    - third:
        bits: 10
"""
    user_type = _parse_field_config(document)

    # There should be the 3 fields, 2 explicit reserved fields and a reserved
    # field consuming the remaining bits.
    contents = user_type.get_contents()
    assert len(contents) == 6
    assert struct.is_field(contents[0])
    assert struct.is_reserved(contents[1])
    assert contents[1].bits == 1
    assert struct.is_field(contents[2])
    assert struct.is_reserved(contents[3])
    assert contents[3].bits == 2
    assert struct.is_field(contents[4])
    assert struct.is_reserved(contents[5])
    assert contents[5].position.offset == 16
    assert contents[5].bits == 16

    first = user_type.get_field('first')
    assert first.bits == 1
    assert first.position.offset == 0
    assert issubclass(first.type, basic.BooleanType)

    second = user_type.get_field('second')
    assert second.bits == 2
    assert second.position.offset == 2
    assert issubclass(second.type, basic.IntegerType)

    third = user_type.get_field('third')
    assert third.bits == 10
    assert third.position.offset == 6
    assert issubclass(third.type, basic.IntegerType)

def test_user_indicators():
    document = """
fields:
    - indicators:
        - reserved
        - simple
        - reserved: 4
        - bigger:
            bits: 2
"""
    user_type = _parse_field_config(document)

    # Check general layout and reserved sizes. Named fields and their enables
    # will be checked further below.
    contents = user_type.get_contents()
    assert len(contents) == 9
    assert struct.is_reserved(contents[0])
    assert contents[0].bits == 1
    assert struct.is_enable(contents[1])
    assert struct.is_reserved(contents[2])
    assert contents[2].bits == 4
    assert struct.is_enable(contents[3])
    assert struct.is_reserved(contents[4])
    assert contents[4].bits == 1
    assert struct.is_field(contents[5])
    assert struct.is_reserved(contents[6])
    assert contents[6].bits == 4
    assert struct.is_field(contents[7])
    assert struct.is_reserved(contents[8])
    assert contents[8].position.offset == 16
    assert contents[8].bits == 16

    # Check the defined fields for correct types, offsets, etc. There are 8
    # bits worth of enable flags, so fields should start from bit 23 in VITA
    # 49 indexing convention, with their corresponding index flags 8 bits
    # prior.
    fields = user_type.get_fields()
    assert len(fields) == 2

    simple = user_type.get_field('simple')
    assert simple.type.bits == 1
    assert issubclass(simple.type, basic.BooleanType)
    # Offset should bit 1 in second byte
    assert simple.position.offset == 9
    assert simple.enable is not None
    assert simple.enable.bits == simple.bits
    assert simple.position == simple.enable.position + 8

    bigger = user_type.get_field('bigger')
    assert bigger.type.bits == 2
    assert issubclass(bigger.type, basic.IntegerType)
    # Offset should bit 6 in second byte
    assert bigger.position.offset == 14
    assert bigger.enable is not None
    assert bigger.enable.bits == bigger.bits
    assert bigger.position == bigger.enable.position + 8

def test_user_optional():
    document = """
fields:
    - sometimes: optional
    - rarely:
        optional: true
        bits: 5
"""
    user_type = _parse_field_config(document)

    # There should be the 2 fields, 2 enable flags and a reserved field
    # consuming the remaining bits.
    contents = user_type.get_contents()
    assert len(contents) == 5
    assert struct.is_enable(contents[0])
    assert struct.is_field(contents[1])
    assert struct.is_enable(contents[2])
    assert struct.is_field(contents[3])
    assert struct.is_reserved(contents[4])
    assert contents[4].bits == 24
    assert contents[4].position.offset == 8

    sometimes = user_type.get_field('sometimes')
    assert sometimes.bits == 1
    assert sometimes.position.offset == 1
    assert sometimes.enable is not None
    assert sometimes.enable.bits == 1
    assert sometimes.position == sometimes.enable.position + 1

    rarely = user_type.get_field('rarely')
    assert rarely.bits == 5
    assert rarely.position.offset == 3
    assert rarely.enable is not None
    assert rarely.enable.bits == 1
    assert rarely.position == rarely.enable.position + 1
