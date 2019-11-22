# Copyright (C) 2019 Geon Technologies, LLC
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

from vrtgen.types import basic
from vrtgen.types.struct import Struct, Enable, Field, Reserved, BitPosition
from vrtgen.types.cif1 import DiscreteIO32

def test_discreteio32():
    ns = {}
    ns['reserved_1'] = Reserved(2)
    ns['a_bool_enable'] = Enable('A Bool Enable')
    ns['some_int_enable'] = Enable('Some Int Enable')
    ns['a_bool'] = Field('A Bool', basic.Boolean, enable=ns['a_bool_enable'])
    ns['some_int'] = Field('Some Int', basic.IntegerType.create(3), enable=ns['some_int_enable'], position=BitPosition(4))

    # Create the struct class and check the basics. It should only report 2
    # fields; the rest are enables and reserved.
    TestIO32 = DiscreteIO32.create_struct('TestIO32', ns)
    assert TestIO32.bits == DiscreteIO32.bits
    assert len(TestIO32.get_fields()) == 2

    # Verify the layout. There should have been a reserved field inserted
    # between 'a_bool' and 'some_int', and another at the end to pad out to the
    # full 32 bits. The positions should be in order with no gaps.
    contents = TestIO32.get_contents()
    assert len(contents) == 7
    assert contents[0].position.offset == 0
    assert isinstance(contents[4], Reserved)
    assert TestIO32.get_field('Some Int').position == BitPosition(4)
    assert isinstance(contents[-1], Reserved)
    for index in range(1, len(contents)):
        last = contents[index - 1]
        assert contents[index].position == (last.position + last.bits)

    # Check that standard struct access works, both attribute-based and
    # by-name.
    test = TestIO32()
    test.some_int = -1
    assert test.get_value('Some Int') == -1
    assert test.pack() == b'\x10\x00\x00\x1c'

    test = TestIO32()
    test.set_value('A Bool', True)
    assert test.a_bool == True
    assert test.pack() == b'\x28\x00\x00\x00'
