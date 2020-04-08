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

from .field import FieldParser
from . import value as value_parser

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
        namespace = {}
        for index, item in enumerate(value):
            try:
                self.parse_user_defined_field(log, namespace, field, item)
            except (ValueError, TypeError) as exc:
                log.error('Invalid user-defined field %d: %s', index, exc)

        field.usertype = user.create_struct(field.type, field.type.__name__, namespace)
        for content in field.usertype.get_contents():
            log.debug("'%s' %s bits=%d", content.name, content.position, content.bits)

    @classmethod
    def _parse_indicators(cls, log, namespace, value):
        if not isinstance(value, list):
            raise TypeError('indicators must be a sequence')
        fields = []
        for item in value:
            # Parse all indicator fields and add their enable flags, but defer
            # adding the field itself until all fields have been processed.
            # This ensures that all indicators are placed after the last
            # enable.
            field = cls._parse_single(log, item)
            if isinstance(field, Reserved):
                # Reserve equal number of bits in enable flags
                cls._add_field(namespace, Reserved(field.bits))
            else:
                # Create enable bit(s) linked to the field
                field.enable = Enable(bits=field.bits)
                cls._add_field(namespace, field.enable)
            fields.append(field)

        for field in fields:
            cls._add_field(namespace, field)

    @staticmethod
    def _parse_bits(value):
        try:
            return int(value)
        except:
            raise TypeError('bits must a number')

    @classmethod
    def parse_user_defined_field(cls, log, namespace, field, value):
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
                cls._parse_indicators(log, namespace, field_value)
                return
        else:
            name = str(value)
            field_value = None
        options = cls._parse_single_field(log, name, namespace, field_value)
        field = cls._add_field_with_options(name, namespace, **options)
        if options.get('optional', False):
            if isinstance(field, Reserved):
                raise ValueError('can only specify size of reserved bits')
            else:
                field.enable = Enable(name + ' Enable')
                cls._add_field(namespace, field.enable)
        cls._add_field(namespace, field)

    @classmethod
    def _parse_field_dict(cls, log, name, value):
        # Reserved field only supports bit count
        if name.casefold() == 'reserved':
            return Reserved(cls._parse_bits(value))

        bits = 1
        if isinstance(value, list):
            raise TypeError('must be mapping or scalar')
        if isinstance(value, dict):
            for key, item in value.items():
                optname = key.casefold()
                if optname == 'bits':
                    bits = cls._parse_bits(item)
        else:
            bits = int(value)
        if bits == 1:
            dtype = basic.Boolean
        else:
            dtype = basic.IntegerType.create(bits)
        return Field(name, dtype)

    @classmethod
    def _parse_single(cls, log, value):
        if isinstance(value, dict):
            if len(value) != 1:
                raise TypeError('must be single-key mapping')
            name, field_value = next(iter(value.items()))
            return cls._parse_field_dict(log, name, field_value)
        value = str(value)
        if value.casefold() == 'reserved':
            return Reserved(1)
        return Field(value, basic.Boolean)

    @classmethod
    def _parse_single_field(cls, log, name, namespace, value):
        if value is None:
            return {}

        if name == 'reserved':
            try:
                return {'bits': int(value)}
            except:
                raise TypeError('reserved field can only specify number of bits')

        return UserFieldParser(name).parse(log, value)

    @classmethod
    def _add_field_with_options(cls, name, namespace, bits=1, optional=None):
        if name == 'reserved':
            return Reserved(bits)

        if bits == 1:
            dtype = basic.Boolean
        else:
            dtype = basic.IntegerType.create(bits)
        return Field(name, dtype)

    @staticmethod
    def _add_field(namespace, field):
        if isinstance(field, Reserved):
            count = sum((k.startswith('reserved_') for k in namespace.keys()))
            attr = 'reserved_{}'.format(count + 1)
            # Guard against pathological breakage...
            assert attr not in namespace
        else:
            attr = '_'.join(field.name.lower().split())
            if isinstance(field, Enable):
                attr += '_enable'
            if attr in namespace:
                raise ValueError("duplicate name '{}'".format(field.name))
        namespace[attr] = field
