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

"""
Parsers for handling user-defined fields.
"""

from vrtgen.parser import value as value_parser
from vrtgen.parser.utils import EMPTY, to_kvpair
from vrtgen.types import basic
from vrtgen.types.struct import Enable, Field, Reserved, is_field

from vrtgen.parser.field.base import FieldParser

__all__ = ('UserDefinedFieldParser',)

def _parse_bits(value):
    if not isinstance(value, int):
        raise TypeError('bits must be an integer')
    bits = int(value)
    if bits <= 0:
        raise ValueError('bits must be 1 or more')
    return bits

class FactoryParser:
    """
    Abstract base class to parse a user field definition and construct a field
    instance.
    """
    def parse(self, log, value):
        """
        Parse a YAML configuration value.
        """
        raise NotImplementedError('parse')

    def create(self):
        """
        Create an instance of the configured field.
        """
        raise NotImplementedError('create')

class ReservedParser(FactoryParser):
    """
    Parses and creates reserved field configuration.

    If a value is provided, it must be a number of bits.
    """
    def __init__(self):
        self.bits = 1

    def parse(self, log, value):
        self.bits = _parse_bits(value)

    def create(self):
        return Reserved(self.bits)

class UserFieldParser(FactoryParser):
    """
    Parses a definition for a user-defined field.
    """
    def __init__(self, name, indicator=False):
        self.name = name
        self.optional = False
        self.bits = 1
        self.indicator = indicator

    def parse(self, log, value):
        if isinstance(value, dict):
            self._parse_mapping(log, value)
        elif value == 'optional':
            self.optional = True
        else:
            self.bits = _parse_bits(value)

    def create(self):
        if self.bits == 1:
            dtype = basic.Boolean
        else:
            dtype = basic.IntegerType.create(self.bits)
        field = Field(self.name, dtype)
        if self.optional and not self.indicator:
            field.enable = Enable(self.name)
        return field

    def _parse_mapping(self, log, value):
        for key, val in value.items():
            if key == 'optional':
                try:
                    self.optional = value_parser.parse_boolean(val)
                except TypeError as exc:
                    raise TypeError('optional {}'.format(exc))
            elif key == 'bits':
                self.bits = _parse_bits(val)
            else:
                log.warning("Invalid option '%s'", key)

def _get_user_parser(name, indicator=False):
    if name.casefold() == 'reserved':
        return ReservedParser()
    return UserFieldParser(name, indicator)

class UserDefinedFieldParser(FieldParser):
    """
    Parser for configuring user-defined fields.
    """
    def parse_scalar(self, log, field, value):
        raise TypeError('user-defined fields must be a sequence or mapping')

    def parse_option(self, log, field, name, value):
        if name.casefold() == 'fields':
            if not isinstance(value, list):
                raise TypeError('user-defined fields must be a sequence')
            self.parse_sequence(log, field, value)
        else:
            super().parse_option(log, field, name, value)

    def parse_sequence(self, log, field, value):
        for index, item in enumerate(value):
            try:
                self.parse_user_defined_field(log, field, item)
            except (ValueError, TypeError) as exc:
                log.error('Invalid user-defined field %d: %s', index, exc)

        for content in field.type.get_contents():
            log.debug("'%s' %s bits=%d", content.name, content.position, content.bits)

    @classmethod
    def _parse_indicators(cls, log, context, value):
        if not isinstance(value, list):
            raise TypeError('indicators must be a sequence')
        fields = []
        for item in value:
            # Parse all indicator fields and add their enable flags, but defer
            # adding the field itself until all fields have been processed.
            # This ensures that all indicators are placed after the last
            # enable.
            field = cls._parse_single_indicator(log, item)
            if isinstance(field, Reserved):
                # Reserve equal number of bits in enable flags
                reserved = Reserved(field.bits)
                context.add_field(reserved)
            else:
                # Create enable bit(s) linked to the field
                field.enable = Enable(field.name, bits=field.bits)
                context.add_field(field.enable)
            fields.append(field)

        for field in fields:
            context.add_field(field)

    @classmethod
    def parse_user_defined_field(cls, log, context, value):
        """
        Parses a definition for a user-defined field.
        """
        name, options = to_kvpair(value)
        if name.casefold() == 'indicators':
            if options is EMPTY:
                raise ValueError('no indicators defined in block')
            cls._parse_indicators(log, context, options)
        else:
            parser = _get_user_parser(name)
            if options is not EMPTY:
                parser.parse(log, options)
            field = parser.create()
            if is_field(field) and field.enable:
                context.add_field(field.enable)
            context.add_field(field)

    @classmethod
    def _parse_single_indicator(cls, log, value):
        name, options = to_kvpair(value)
        parser = _get_user_parser(name, indicator=True)
        if options is not EMPTY:
            parser.parse(log, options)
        return parser.create()
