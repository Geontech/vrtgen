"""
Parsers for handling packet fields.
"""

from vrtgen.model.field import Mode
from vrtgen.types import enums
from vrtgen.types import basic
from vrtgen.types.header import ClassIdentifier

from . import value as value_parser

class FieldParser:
    def __call__(self, log, field, value):
        if isinstance(value, dict):
            self.parse_mapping(log, field, value)
        elif isinstance(value, list):
            self.parse_sequence(log, field, value)
        else:
            enable = self.parse_enable(value)
            if enable is not None:
                self.set_enable(log, field, enable)
            else:
                self.parse_scalar(log, field, value)

    def parse_mapping(self, log, field, mapping):
        enable = Mode.REQUIRED
        for key, value in mapping.items():
            if key == 'required':
                if not value:
                    enable = Mode.OPTIONAL
            elif not self.parse_mapping_entry(log, field, key, value):
                log.warn("Invalid option '%s' for field '%s'", key, field.name)

        self.set_enable(log, field, enable)

    def parse_mapping_entry(self, log, field, key, value):
        return False

    def parse_sequence(self, log, field, value):
        raise TypeError("Field '"+field.name+"' cannot be defined with a sequence")

    def parse_scalar(self, log, field, value):
        raise TypeError("Field '"+field.name+"' cannot be defined with a scalar value")

    def parse_enable(self, value):
        if isinstance(value, str):
            return {
                'required': Mode.REQUIRED,
                'optional': Mode.OPTIONAL,
                'disabled': Mode.DISABLED
            }.get(value.casefold(), None)
        else:
            return None

    def set_enable(self, log, field, enable):
        field.enable = enable
        log.debug("Field '%s' is %s", field.name, field.enable)

class SimpleFieldParser(FieldParser):
    def __init__(self, parser):
        self.parser = parser

    def parse_scalar(self, log, field, value):
        value = self.parser(value)
        self.set_value(log, field, value)
        # If a value is given with no other qualifiers, consider the
        # field value to be constant
        log.debug("Field '%s' is CONSTANT", field.name)
        field.set_constant()

    def parse_mapping_entry(self, log, field, name, value):
        if name == 'default':
            value = self.parser(value)
            self.set_value(log, field, value)
        else:
            return False
        return True

    def set_value(self, log, field, value):
        field.value = value
        log.debug("Field '%s' = %s", field.name, value)

    @staticmethod
    def factory(field):
        if field.type == basic.OUI:
            parser = value_parser.parse_oui
        else:
            parser = field.type
        return SimpleFieldParser(parser)

class UserDefinedFieldParser(FieldParser):
    def parse_scalar(self, log, value):
        raise TypeError('user-defined fields must be a sequence or mapping')

    def parse_mapping_entry(self, log, field, name, value):
        if name == 'fields':
            if not isinstance(value, list):
                raise TypeError('user-defined fields must be a sequence')
            self.parse_sequence(log, field, value)
            return True
        else:
            return False

    def parse_sequence(self, log, field, value):
        log = log.getChild(field.name)
        for index, item in enumerate(value):
            try:
                self.parse_user_defined_field(log, field, item)
            except (ValueError, TypeError) as exc:
                log.error('Invalid user-defined field %d: %s', index, exc)

    def parse_user_defined_field(self, log, field, value):
        if not isinstance(value, dict):
            raise TypeError('must be a mapping')
        name = None
        bits = 1
        word = None
        position = None
        for attr_name, attr_value in value.items():
            if attr_name == 'name':
                name = attr_value
            elif attr_name == 'bits':
                bits = int(attr_value)
            elif attr_name == 'position':
                position = int(attr_value)
            elif attr_name == 'word':
                word = int(attr_value)
            else:
                raise ValueError('invalid attribute {}'.format(attr_name))
        if name is None:
            raise ValueError('no name given')
        field.add_field(name, bits, word, position)
        log.debug("'%s' bits=%d position=%s/%s", name, bits, word, position)

class StructFieldParser(FieldParser):
    def parse_mapping_entry(self, log, field, name, value):
        subfield = field.get_field(name)
        if subfield is None:
            return False
        parser = SimpleFieldParser.factory(subfield)
        log = log.getChild(field.name)
        try:
            parser(log, subfield, value)
        except (ValueError, TypeError) as exc:
            log.error("Invalid definition for '%s': %s", subfield.name, exc)
        return True

class ClassIDParser(StructFieldParser):
    def parse_mapping_entry(self, log, field, name, value):
        if name.casefold() == 'oui':
            name = ClassIdentifier.oui.name
        return super().parse_mapping_entry(log, field, name, value)

class IndexListParser(FieldParser):
    def parse_mapping_entry(self, log, field, name, value):
        if name.casefold() == 'entry size':
            field.entry_size = int(value)
            log.debug("Index List entry size is %d", value)
        else:
            return False
        return True
