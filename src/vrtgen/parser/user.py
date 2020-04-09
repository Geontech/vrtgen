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
Parsers for handling user-defined fields.
"""

from vrtgen.types import basic
from vrtgen.types.struct import Enable, Field, Reserved
from vrtgen.types import user

from vrtgen.model.field import SimpleFieldConfiguration

from .field import FieldParser
from . import value as value_parser
from .mapping import MappingParser

class IndicatorParser:
    def parse(self, value):
        if isinstance(value, list):
            raise TypeError('must be mapping or scalar')
        if isinstance(value, dict):
            if len(value) != 1:
                raise TypeError('must be single-key mapping')
            name, field_value = next(iter(value.items()))
            return self.parse_options(name, field_value)
        name = str(value)
        if name.casefold() == 'reserved':
            return Reserved(1)
        return Field(name, basic.Boolean)

    def parse_options(self, name, value):
        # Reserved field only supports bit count
        if name.casefold() == 'reserved':
            return Reserved(int(value))

        bits = 1
        if isinstance(value, list):
            raise TypeError('must be mapping or scalar')
        if isinstance(value, dict):
            for key, item in value.items():
                optname = key.casefold()
                if optname == 'bits':
                    bits = int(item)
                else:
                    raise ValueError("invalid option '{}'".format(key))
        else:
            bits = int(value)
        if bits == 1:
            dtype = basic.Boolean
        else:
            dtype = basic.IntegerType.create(bits)
        return Field(name, dtype)

class UserFieldParser:
    def __init__(self, name):
        self.name = name

    def parse(self, log, value):
        if isinstance(value, dict):
            return dict(self.parse_mapping(log, value))
        elif value == 'optional':
            return {'optional': True}
        else:
            return {'bits': int(value)}

    def parse_mapping(self, log, value):
        for key, val in value.items():
            if key == 'optional':
                try:
                    optional = value_parser.parse_boolean(val)
                except TypeError as exc:
                    raise TypeError('optional {}'.format(exc))
                yield('optional', optional)
            elif key == 'bits':
                try:
                    bits = int(val)
                except:
                    raise TypeError('bits must be a number')
                yield('bits', int(val))
            else:
                log.warning("Invalid option '%s'", key)

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

    @staticmethod
    def _parse_indicators(log, context, value):
        if not isinstance(value, list):
            raise TypeError('indicators must be a sequence')
        fields = []
        for item in value:
            # Parse all indicator fields and add their enable flags, but defer
            # adding the field itself until all fields have been processed.
            # This ensures that all indicators are placed after the last
            # enable.
            field = IndicatorParser().parse(item)
            if isinstance(field, Reserved):
                # Reserve equal number of bits in enable flags
                reserved = Reserved(field.bits)
                context.add_field(reserved)
            else:
                # Create enable bit(s) linked to the field
                field.enable = Enable(bits=field.bits)
                context.add_field(field.enable)
            fields.append(field)

        for field in fields:
            context.add_field(field)

    @classmethod
    def parse_user_defined_field(cls, log, context, value):
        """
        Parses a definition for a user-defined field.
        """
        if isinstance(value, list):
            raise TypeError('must be name or single-key mapping')
        if isinstance(value, dict):
            if len(value) != 1:
                raise TypeError('must be single-key mapping')
            name, field_value = next(iter(value.items()))
            if name.casefold() == 'indicators':
                cls._parse_indicators(log, context, field_value)
                return
        else:
            name = str(value)
            field_value = None
        options = cls._parse_single_field(log, name, field_value)
        field = cls._add_field_with_options(name, **options)
        if options.get('optional', False):
            if isinstance(field, Reserved):
                raise ValueError('can only specify size of reserved bits')
            else:
                field.enable = Enable(name + ' Enable')
                context.add_field(field.enable)
        context.add_field(field)

    @classmethod
    def _parse_single_field(cls, log, name, value):
        if value is None:
            return {}

        if name == 'reserved':
            try:
                return {'bits': int(value)}
            except:
                raise TypeError('reserved field can only specify number of bits')

        return UserFieldParser(name).parse(log, value)

    @classmethod
    def _add_field_with_options(cls, name, bits=1, optional=None):
        if name == 'reserved':
            return Reserved(bits)

        if bits == 1:
            dtype = basic.Boolean
        else:
            dtype = basic.IntegerType.create(bits)
        return Field(name, dtype)
