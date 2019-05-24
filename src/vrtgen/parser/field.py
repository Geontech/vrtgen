"""
Parsers for handling packet fields.
"""

from vrtgen.model.field import Mode
from vrtgen.types import enums
from vrtgen.types import basic
from vrtgen.types.prologue import ClassIdentifier

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
    __TYPES__ = {}

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

    @classmethod
    def register_type(cls, datatype, parser):
        cls.__TYPES__[datatype] = parser

    @classmethod
    def factory(cls, field):
        parser = cls.__TYPES__.get(field.type, field.type)
        return cls(parser)

SimpleFieldParser.register_type(basic.Boolean, value_parser.parse_boolean)
SimpleFieldParser.register_type(basic.OUI, value_parser.parse_oui)
SimpleFieldParser.register_type(enums.TSI, value_parser.parse_tsi)
SimpleFieldParser.register_type(enums.TSF, value_parser.parse_tsf)
SimpleFieldParser.register_type(enums.SSI, value_parser.parse_ssi)

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
    def __init__(self, parser):
        self.parser = parser

    def parse_mapping_entry(self, log, field, name, value):
        try:
            self.parser(log, field, {name:value})
        except KeyError:
            log.error("Invalid field '%s'", name)
        except (ValueError, TypeError) as exc:
            log.error("Invalid definition for '%s': %s", name, exc)
        return True

    @classmethod
    def factory(cls, field):
        parser = StructValueParser(field.type)
        return cls(parser)

class StructValueParser:
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
            name = self._aliases.get(field_name.casefold(), field_name)
            parser = self._parsers.get(name.casefold(), None)
            if parser is None:
                raise KeyError(field_name)
            field = context.get_field(name)
            assert field is not None
            parser(log.getChild(field.name), field, field_value)

    def add_alias(self, name, alias):
        self._aliases[alias.casefold()] = name

class ClassIDParser(StructValueParser):
    def __init__(self):
        super().__init__(ClassIdentifier)
        self.add_alias(ClassIdentifier.oui.name, 'OUI')

class IndexListParser(FieldParser):
    def parse_mapping_entry(self, log, field, name, value):
        if name.casefold() == 'entry size':
            field.entry_size = int(value)
            log.debug("Index List entry size is %d", value)
        else:
            return False
        return True
