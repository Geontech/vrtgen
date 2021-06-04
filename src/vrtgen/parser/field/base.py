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
Parsers for handling packet fields.
"""

from vrtgen.model.field import Mode

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
