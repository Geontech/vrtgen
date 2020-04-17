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

import pytest

from vrtgen.types import basic
from vrtgen.types.struct import Struct, Enable, Field, Reserved, BitPosition
from vrtgen.types import user
from vrtgen.types.cif1 import DiscreteIO32

class UnitTestStruct(Struct):
    """
    Simple struct for unit testing.
    """
    first = Field('First', basic.Integer16)
    second = Field('Second', basic.FixedPoint16r7)
    third = Field('Third', basic.Boolean)
    reserved = Reserved(15)
    count_enable = Enable()
    count = Field('Count', basic.IntegerType.create(15, signed=False), enable=count_enable)

def test_struct_classmethods():
    """
    Verifies the class methods for inspecting struct metadata.
    """
    # get_contents() should return all of the fields, reserved bits and enables
    # defined in the class
    assert UnitTestStruct.get_contents() == [
        UnitTestStruct.first, UnitTestStruct.second, UnitTestStruct.third,
        UnitTestStruct.reserved, UnitTestStruct.count_enable, UnitTestStruct.count
    ]

    # get_fields() should only return the contents of type Field
    assert UnitTestStruct.get_fields() == [
        UnitTestStruct.first, UnitTestStruct.second, UnitTestStruct.third, UnitTestStruct.count
    ]

    # Test by-name lookup, which is case-insensitive
    assert UnitTestStruct.get_field('Third') == UnitTestStruct.third
    assert UnitTestStruct.get_field('third') == UnitTestStruct.third
    with pytest.raises(KeyError):
        UnitTestStruct.get_field('Not Present')

def test_struct_layout():
    """
    Test that absent any explicit field positions, fields are laid out in order
    from MSB to LSB.
    """
    assert UnitTestStruct.bits == 64
    assert UnitTestStruct.first.position == BitPosition(31, 0)
    assert UnitTestStruct.second.position == BitPosition(15, 0)
    assert UnitTestStruct.third.position == BitPosition(31, 1)
    assert UnitTestStruct.reserved.position == BitPosition(30, 1)
    assert UnitTestStruct.count_enable.position == BitPosition(15, 1)
    assert UnitTestStruct.count.position == BitPosition(14, 1)

def test_struct_field_attrs():
    """
    Test attribute access for field values.
    """
    value = UnitTestStruct()
    assert value.first == 0
    assert value.second == 0
    assert value.third is False
    assert value.reserved == 0
    assert not value.count_enable
    assert value.count is None

    value.first = 1
    value.second = 2.25
    value.third = True
    value.count = 767

    assert value.first == 1
    assert value.second == 2.25
    assert value.third is True
    assert value.reserved == 0
    assert value.count_enable
    assert value.count == 767

def test_struct_get():
    """
    Verify getting field values via name lookup.
    """
    value = UnitTestStruct()
    value.second = -1.125
    value.third = True
    assert value.get_value('Second') == -1.125
    assert value.get_value('third') is True

def test_struct_set():
    """
    Verify setting field values via name lookup.
    """
    value = UnitTestStruct()
    value.set_value('First', -129)
    value.set_value('count', 14178)
    assert value.first == -129
    assert value.count_enable is True
    assert value.count == 14178

def test_struct_pack():
    """
    Verify serializing fields to a byte string.
    """
    value = UnitTestStruct()
    value.first = -30000
    value.second = -17.625
    value.third = True
    value.count = 12345

    # Data (grouped by 16 bits)
    #  - 2 bytes for first: 0x8AD0 (-30000)
    #  - 2 bytes for second: 0x00C0 (-17.625)
    #  - 1 bit for third:  0x8000 (true)
    #    15 bits reserved: 0x0000
    #    =                 0x8000
    #  - 1 bit for count enable: 0x8000 (true)
    #    15 bits for count:      0x3039 (12345)
    #    =                       0xB039
    data = value.pack()
    assert data == b'\x8A\xD0\xF7\x30\x80\x00\xB0\x39'

def test_struct_unpack():
    """
    Verify deserializing fields from a byte string.
    """
    # Data (grouped by 16 bits)
    #  - 2 bytes for first: 0x1234 (-32204)
    #  - 2 bytes for second: 0x00C0 (1.5)
    #  - 1 bit for third: 0x8000 (true)
    #    15 bits reserved: 0x0000 = 0x8000
    #  - 1 bit for count enable: 0x8000 (true)
    #    15 bits for count:      0x0765 (4660)
    #    =                       0x8765
    data = b'\x82\x34\x00\xC0\x80\x00\x87\x65'
    value = UnitTestStruct.unpack(data)
    assert value.first == -32204
    assert value.second == 1.5
    assert value.third is True
    assert value.count_enable is True
    assert value.count == 1893

def test_abstract_struct():
    """
    Verify that struct classes with no defined fields cannot be constructed.
    """
    with pytest.raises(TypeError):
        value = Struct()

    class AbstractStruct(Struct):
        pass

    with pytest.raises(TypeError):
        value = AbstractStruct()

class BaseStruct(Struct):
    """
    Base struct defintion for subclassing tests.
    """
    number = Field('Number', basic.Integer16)
    reserved = Reserved(8)
    pad = Field('Pad', basic.Integer8)

def test_struct_subclass():
    """
    Verify that struct subclasses append their fields to the base class.
    """
    class DerivedStruct(BaseStruct):
        left = Field('Left', basic.FixedPoint16r13)
        right = Field('Right', basic.FixedPoint16r13)

    assert BaseStruct.bits == 32
    assert DerivedStruct.bits == 64
    assert DerivedStruct.get_contents() == BaseStruct.get_contents() + [
        DerivedStruct.left, DerivedStruct.right
    ]
    assert DerivedStruct.get_fields() == BaseStruct.get_fields() + [
        DerivedStruct.left, DerivedStruct.right
    ]
    assert DerivedStruct.left.position == BitPosition(31, 1)
    assert DerivedStruct.right.position == BitPosition(15, 1)

def test_struct_subclass_rebind():
    """
    Verify that base class reserved bits can be rebound to fields in a
    subclass.
    """
    class RebindStruct(BaseStruct):
        rebound = BaseStruct.reserved.rebind(Field('Rebound', basic.Integer8))

    assert BaseStruct.bits == RebindStruct.bits
    assert BaseStruct.reserved.position == RebindStruct.rebound.position

def test_user_defined():
    """
    Verify the dynamic creation of user-defined structs.
    """
    ns = {}
    ns['reserved_1'] = Reserved(2)
    ns['a_bool_enable'] = Enable('A Bool Enable')
    ns['some_int_enable'] = Enable('Some Int Enable')
    ns['a_bool'] = Field('A Bool', basic.Boolean, enable=ns['a_bool_enable'])
    ns['some_int'] = Field('Some Int', basic.IntegerType.create(3), enable=ns['some_int_enable'], position=BitPosition(4))

    # Create the struct class and check the basics. It should only report 2
    # fields; the rest are enables and reserved.
    TestIO32 = user.create_struct(DiscreteIO32, 'TestIO32', ns)
    assert TestIO32.bits == DiscreteIO32.bits
    assert len(TestIO32.get_fields()) == 2

    # Verify the layout. There should have been a reserved field inserted
    # between 'a_bool' and 'some_int', and another at the end to pad out to the
    # full 32 bits. The positions should be in order with no gaps.
    contents = TestIO32.get_contents()
    assert len(contents) == 7
    assert isinstance(contents[4], Reserved)
    assert TestIO32.get_field('Some Int').position == BitPosition(4)
    assert isinstance(contents[-1], Reserved)
    assert contents[0].position.offset == 0
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
