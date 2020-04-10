# Copyright (C) 2020 Geon Technologies, LLC
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

"""
Classes and helper methods for generating C++ structures.
"""

from vrtgen.types import enums, basic, struct

from . import types as cpptypes
from . import utils

def member_type(datatype):
    if datatype == basic.OUI:
        return 'OUI'
    if issubclass(datatype, enums.BinaryEnum):
        return cpptypes.enum_type(datatype)
    if issubclass(datatype, basic.IntegerType):
        base_type = cpptypes.int_type(datatype.bits, datatype.signed)
        return 'big_endian<{}>'.format(base_type)
    if issubclass(datatype, basic.FixedPointType):
        return cpptypes.fixed_type(datatype.bits, datatype.radix)
    if issubclass(datatype, basic.NonZeroSize):
        base_type = cpptypes.int_type(datatype.bits, False)
        return 'nonzero_size<{}>'.format(base_type)
    raise NotImplementedError(datatype.__name__)

def _tag_name(field):
    return field.attr + '_tag'

def format_enable_methods(field, member, name=None):
    if name is None:
        name = field.name
    identifier = cpptypes.name_to_identifier(name + 'Enabled')
    return {
        'name': name,
        'getter': {
            'doc': 'Get enabled state of ' + name,
            'name' : 'is'+identifier,
        },
        'setter': {
            'doc': 'Set enabled state of ' + name,
            'name' : 'set'+identifier,
        },
        'type': 'bool',
        'member': member,
        'tag': _tag_name(field)
    }

def format_value_methods(field, member):
    identifier = cpptypes.name_to_identifier(field.name)
    field_data = {
        'name': field.name,
        'getter': {
            'doc': 'Get current value of ' + field.name,
            'name' : 'get'+identifier,
        },
        'setter': {
            'doc': 'Set current value of ' + field.name,
            'name' : 'set'+identifier,
        },
        'member': member,
        'type': cpptypes.value_type(field.type),
        'bits': field.type.bits,
    }

    if isinstance(member, Packed):
        field_data['tag'] = _tag_name(field)

    return field_data

class Member:
    def __init__(self, name, datatype):
        self.name = 'm_' + name
        self.type = datatype
        self.doc = []

    @property
    def decl(self):
        assert self.type is not None
        return '{} {}'.format(self.type, self.name)

    def _add_field_doc(self, field):
        self.doc.append('{} {}'.format(field.name, field.position))

class BasicMember(Member):
    def __init__(self, name, field):
        super().__init__(name, member_type(field.type))
        self.field = field
        self._add_field_doc(field)

class Reserved(Member):
    def __init__(self, name, field):
        super().__init__(name, cpptypes.int_type(field.bits, False))
        self.doc = ['Reserved {}'.format(field.position)]

class Tag:
    def __init__(self, field):
        self.name = _tag_name(field)
        self.offset = field.position.bit
        self.basetype = Tag._base_type(field.type)
        self.bits = field.bits
        self.converter = Tag._converter_type(field.type)

    @property
    def type(self):
        template_args = [self.basetype, self.offset, self.bits]
        if self.converter:
            template_args.append(self.converter)
        return 'packed_tag<{}>'.format(','.join(str(arg) for arg in template_args))

    @staticmethod
    def _base_type(datatype):
        if issubclass(datatype, basic.IntegerType):
            # Use unsized types that only signify signed/unsigned since the size
            # is included in the template arguments. This simplifies the
            # implementation of sign extension.
            if datatype.signed:
                return 'signed'
            else:
                return 'unsigned'
        return cpptypes.value_type(datatype)

    @staticmethod
    def _converter_type(datatype):
        if issubclass(datatype, basic.NonZeroSize):
            return 'detail::size_converter'
        return None

class Packed(Member):
    def __init__(self, name, offset):
        super().__init__(name, None)
        self.offset = offset
        self.bits = 0
        self.tags = []

    def full(self):
        assert self.offset >= 0
        return (1 + self.offset - self.bits) == 0

    def close(self):
        # Limit the offset of tags to fall within the bit size of the packed
        # container.
        assert self.bits in (8, 16, 32)
        self.type = self.packed_type(self.bits)
        for tag in self.tags:
            tag.offset = tag.offset % self.bits

    def link_field(self, field):
        # Check for wraparound into the next word
        assert self.offset - self.bits == field.position.bit
        self.bits += field.bits
        self._add_field_doc(field)
        if not struct.is_reserved(field):
            tag = Tag(field)
            self.tags.append(tag)

    @staticmethod
    def packed_type(bits):
        return 'packed<{}>'.format(cpptypes.int_type(bits, False))

class CppStruct:
    def __init__(self, structdef):
        self.name = structdef.__name__
        self.doc = utils.format_docstring(structdef.__doc__)
        self.fields = []
        self.members = []
        self._current_packed = None
        for field in structdef.get_contents():
            self._process_field(field)

    @property
    def packed(self):
        return [f for f in self.members if isinstance(f, Packed)]

    @property
    def reserved(self):
        return [f for f in self.members if isinstance(f, Reserved)]

    def _should_pack(self, field):
        if field.bits % 8:
            # Field does not necessarily need to be byte-aligned, pack it into
            # a larger data member
            return True
        if not field.editable and self._current_packed is not None:
            # Currently adding fields to a packed member, tack on reserved
            # fields instead of giving them their own member. This avoids a
            # problem in the CAM prologue field, where there are 24 bits worth
            # of packed fields followed by 8 reserved bits, but there is no
            # native 24-bit type to hold the packed field.
            return True
        return False

    def _process_field(self, field):
        align = field.position.offset % 32
        if self._should_pack(field):
            # Pack the field into a larger data member; the field itself does
            # not need to be byte-aligned
            if self._current_packed is None:
                # The data member does have to be byte-aligned
                assert align % 8 == 0
                self._add_packed(field.position.bit)
            member = self._current_packed
            member.link_field(field)
        else:
            # Everything else should be byte-aligned
            assert align % 8 == 0
            # There is a packed field being collected, "close" it and add the
            # member variable
            if self._current_packed:
                self._close_packed()

            if field.bits % 32 == 0:
                assert align == 0
            elif field.bits != 24:
                assert align % field.bits == 0

            # Non-editable fields large enough to require their own member must
            # be reserved bits. These are handled differently so the compiler
            # doesn't issue warnings about unused private fields.
            if not field.editable:
                name = 'reserved_{}'.format(len(self.reserved))
                self.members.append(Reserved(name, field))
                return

            member = self._add_member(field)

        if field.editable:
            self._map_field(field, member)

        if self._current_packed and self._current_packed.full():
            self._close_packed()

    def _add_packed(self, offset):
        name = 'packed_' + str(len(self.packed))
        self._current_packed = Packed(name, offset)
        self.members.append(self._current_packed)

    def _close_packed(self):
        self._current_packed.close()
        self._current_packed = None

    def _add_member(self, field):
        name = cpptypes.name_to_identifier(field.name)
        member = BasicMember(name, field)
        self.members.append(member)
        return member

    def _map_field(self, field, member):
        if field.enable is not None:
            methods = format_enable_methods(field.enable, member)
            self.fields.append(methods)
        methods = format_value_methods(field, member)
        self.fields.append(methods)
