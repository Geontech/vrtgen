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

"""
Parsers for handling packet fields.
"""

from vrtgen.model.field import Mode
from vrtgen.types import enums
from vrtgen.types import basic
from vrtgen.types.prologue import ClassIdentifier

from . import value as value_parser

class FieldParser:
    """
    Base class for parsers that configure a VITA 49 field from YAML.
    """
    def __call__(self, log, field, value):
        # Check for a simple enable setting first ("required" or "optional")
        enable = self.parse_enable(value)
        if enable is not None:
            field.mode = enable
        else:
            # Default to required, may be overridden in extract_keywords()
            field.mode = Mode.REQUIRED
            if isinstance(value, dict):
                value = self.extract_keywords(field, value)

            self.parse_value(log, field, value)

        log.debug("Field '%s' is %s", field.name, field.mode)

    @staticmethod
    def extract_keywords(field, mapping):
        """
        Processes field configuration keywords from a mapping.

        Returns an updated mapping with the configuration keywords removed.
        """
        result = dict()
        for key, value in mapping.items():
            if key.casefold() == 'required':
                if not value:
                    field.mode = Mode.OPTIONAL
            else:
                result[key] = value
        return result

    def parse_value(self, log, field, value):
        """
        Parses a field value.

        The value may be a mapping, sequence or scalar. Derived classes may
        implement their own processing for each type.
        """
        if isinstance(value, list):
            self.parse_sequence(log, field, value)
        elif isinstance(value, dict):
            self.parse_mapping(log, field, value)
        else:
            self.parse_scalar(log, field, value)

    def parse_mapping(self, log, field, mapping):
        """
        Parses a mapping as a field value.
        """
        # Default implementation handles the key-value pairs as options.
        # Subclasses that accept mappings as values (i.e., struct parsers)
        # should override.
        for name, value in mapping.items():
            try:
                self.parse_option(log, field, name, value)
            except (ValueError, TypeError) as exc:
                log.warning("Invalid value for option '%s': %s", name, exc)
            except KeyError as exc:
                log.warning('Invalid option %s', exc)

    @staticmethod
    def parse_option(log, field, name, value):
        """
        Parses an option.
        """
        # Default implementation raises a KeyError. Subclasses may override to
        # provide their own option processing.
        raise KeyError(name)

    @staticmethod
    def parse_sequence(log, field, value):
        """
        Parses a sequence as a field value.
        """
        # Default implementation raises a TypeError. Subclasses that support
        # configuration from sequences should override.
        raise TypeError("Field '"+field.name+"' cannot be defined with a sequence")

    @staticmethod
    def parse_scalar(log, field, value):
        """
        Parses a scalar as a field value.
        """
        # Default implementation raises a TypeError. Subclasses that support
        # configuration from scalars should override.
        raise TypeError("Field '"+field.name+"' cannot be defined with a scalar value")

    @staticmethod
    def parse_enable(value):
        """
        Parses a value as a field enablement mode.

        If value cannot be interpreted as an enablement mode, returns None.
        """
        if not isinstance(value, str):
            return None

        return {
            'required': Mode.REQUIRED,
            'optional': Mode.OPTIONAL,
            'disabled': Mode.DISABLED
        }.get(value.casefold(), None)

class SimpleFieldParser(FieldParser):
    """
    Parser for handling simple field configuration.
    """
    __TYPES__ = {}

    def __init__(self, parser):
        self.value_parser = parser

    def parse_scalar(self, log, field, value):
        field.value = self.value_parser(value)
        log.debug("Field '%s' = %s", field.name, field.value)
        # If a value is given with no other qualifiers, consider the
        # field value to be constant
        log.debug("Field '%s' is CONSTANT", field.name)
        field.set_constant()

    def parse_option(self, log, field, name, value):
        if name.casefold() == 'default':
            field.value = self.value_parser(value)
            log.debug("Field '%s' = %s", field.name, field.value)
        else:
            super().parse_option(log, field, name, value)

    @classmethod
    def register_type(cls, datatype, parser):
        """
        Registers a default value parser for a Python type.
        """
        cls.__TYPES__[datatype] = parser

    @classmethod
    def factory(cls, field):
        """
        Creates a simple parser for a given VITA 49 field.
        """
        parser = cls.__TYPES__.get(field.type, field.type)
        return cls(parser)

SimpleFieldParser.register_type(basic.Boolean, value_parser.parse_boolean)
SimpleFieldParser.register_type(basic.OUI, value_parser.parse_oui)
SimpleFieldParser.register_type(enums.TSI, value_parser.parse_tsi)
SimpleFieldParser.register_type(enums.TSF, value_parser.parse_tsf)
SimpleFieldParser.register_type(enums.SSI, value_parser.parse_ssi)
SimpleFieldParser.register_type(enums.TSM, value_parser.parse_tsm)
SimpleFieldParser.register_type(enums.PackingMethod, value_parser.parse_packing_method)
SimpleFieldParser.register_type(enums.DataSampleType, value_parser.parse_data_sample_type)
SimpleFieldParser.register_type(enums.DataItemFormat, value_parser.parse_data_item_format)
SimpleFieldParser.register_type(enums.ActionMode, value_parser.parse_action)

class StructFieldParser(FieldParser):
    """
    Parser for handling struct field configuration.

    Binds struct value parsing with base field configuration.
    """
    def __init__(self, parser):
        self.parser = parser

    def parse_mapping(self, log, field, mapping):
        self.parser(log, field, mapping)

    @classmethod
    def factory(cls, field):
        """
        Creates a struct parser for a given VITA 49 field.
        """
        parser = StructValueParser(field.type)
        return cls(parser)

class StructValueParser:
    """
    Parser for struct values.

    Struct values in YAML must be a mapping, where each key corresponds to a
    subfield.
    """
    def __init__(self, struct):
        super().__init__()
        self._parsers = {}
        self._aliases = {}
        for field in struct.get_fields():
            self._parsers[field.name.casefold()] = SimpleFieldParser.factory(field)

    def __call__(self, log, context, value):
        if not isinstance(value, dict):
            raise TypeError('Struct values must be a dictionary')

        for field_name, field_value in value.items():
            name = self.get_field_name(field_name)
            try:
                parser = self.get_field_parser(name)
            except KeyError:
                log.error("Invalid field '%s'", field_name)
                continue

            # If parser lookup succeeded, field lookup must succeed
            field = context.get_field(name)
            assert field is not None

            parser(log.getChild(field.name), field, field_value)

    def add_alias(self, name, alias):
        """
        Registers an alias for a field name.
        """
        self._aliases[alias.casefold()] = name

    def get_field_name(self, name):
        """
        Resolves an alias for a field name.
        """
        return self._aliases.get(name.casefold(), name)

    def get_field_parser(self, name):
        """
        Returns the parser for a field.
        """
        parser = self._parsers.get(name.casefold(), None)
        if parser is None:
            raise KeyError(name)
        return parser

class ClassIDParser(StructValueParser):
    """
    Parser for ClassIdentifier structs.
    """
    def __init__(self):
        super().__init__(ClassIdentifier)
        self.add_alias(ClassIdentifier.oui.name, 'OUI')

class IndexListParser(FieldParser):
    """
    Parser for index lists.
    """
    def parse_option(self, log, field, name, value):
        if name.casefold() == 'entry size':
            field.entry_size = int(value)
            log.debug("Index List entry size is %d", value)
        else:
            super().parse_option(log, field, name, value)
