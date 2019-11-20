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

#import pytest

from vrtgen.types import basic
from vrtgen.types.struct import BitPosition
from vrtgen.types.cif1 import DiscreteIO32

def test_discreteio32():
    builder = DiscreteIO32.builder()
    builder.add_reserved(2)
    en_bool = builder.add_enable()
    en_int = builder.add_enable()
    builder.add_field('A Bool', basic.Boolean, enable=en_bool)
    builder.add_field('Some Int', basic.IntegerType.create(3), enable=en_int, position=BitPosition(2))
    TestIO32 = builder.create('TestIO32')
    test = TestIO32()
    test.a_bool = True
    test.some_int = -1
    print(' '.join(hex(x) for x in test.pack()))
