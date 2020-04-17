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
Parsers for handling simple (i.e., single-valued) fields.
"""

from vrtgen.parser.value import get_value_parser

from .base import FieldParser

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
        parser = get_value_parser(field.type)
        return cls(parser)
