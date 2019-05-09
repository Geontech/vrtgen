import re

from vrtgen.model.field import Mode
from vrtgen.types.enums import TSI, TSF, SSI
from vrtgen.types import basic
from vrtgen.types.header import ClassIdentifier

TSI_VALUES = {
    'none':  TSI.NONE,
    'utc':   TSI.UTC,
    'gps':   TSI.GPS,
    'other': TSI.OTHER
}

def value_to_tsi(value):
    try:
        return TSI_VALUES[value.casefold()]
    except KeyError:
        raise ValueError(value)

TSF_VALUES = {
    'none':         TSF.NONE,
    'samples':      TSF.SAMPLE_COUNT,
    'picoseconds':  TSF.REAL_TIME,
    'free running': TSF.FREE_RUNNING
}

def value_to_tsf(value):
    try:
        return TSF_VALUES[value.casefold()]
    except KeyError:
        raise ValueError(value)

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
        value = self.parse_scalar_value(field, value)
        self.set_value(log, field, value)
        # If a value is given with no other qualifiers, consider the
        # field value to be constant
        log.debug("Field '%s' is CONSTANT", field.name)
        field.set_constant()

    def parse_scalar_value(self, field, value):
        raise TypeError("{0} is not a valid value for field '{1}'".format(value, field.name))

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

    def set_value(self, log, field, value):
        field.value = value
        log.debug("Field '%s' = %s", field.name, value)

class GenericFieldParser(FieldParser):
    HEX_DIGIT = r'[0-9a-zA-Z]'
    OUI_RE = re.compile(r'({0})-({0})-({0})$'.format(HEX_DIGIT*2))
    def parse_oui(self, value):
        match = self.OUI_RE.match(str(value))
        if not match:
            raise ValueError('OUI format must be XX-XX-XX')
        return int(''.join(match.groups()), 16)

    def parse_scalar_value(self, field, value):
        if field.type == basic.OUI:
            return self.parse_oui(value)
        elif field.type == TSI:
            return value_to_tsi(value)
        elif field.type == TSF:
            return value_to_tsf(value)
        elif field.type == basic.Boolean:
            return self.to_bool(value)
        elif issubclass(field.type, basic.FixedPointType):
            return float(value)
        elif issubclass(field.type, basic.IntegerType):
            return int(value)
        else:
            raise NotImplementedError("unsupported field '{}'".format(field.name))

    def parse_mapping_entry(self, log, field, name, value):
        if name == 'default':
            value = self.parse_scalar_value(field, value)
            self.set_value(log, field, value)
        else:
            return False
        return True

    def to_bool(self, value):
        if value in (0, False):
            return False
        elif value in (1, True):
            return True
        else:
            raise TypeError('must be boolean, 0 or 1')

class UserDefinedFieldParser(FieldParser):
    def parse_scalar_value(self, log, value):
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

class SSIParser(FieldParser):
    def parse_scalar_value(self, log, field, value):
        ssi = getattr(SSI, value.upper(), None)
        if ssi is None:
            raise ValueError(value)
        return ssi

class StructFieldParser(FieldParser):
    def parse_mapping_entry(self, log, field, name, value):
        subfield = field.get_field(name)
        if subfield is None:
            return False
        parser = GenericFieldParser()
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
