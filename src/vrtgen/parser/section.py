"""
Classes for parsing sections of a VITA 49 packet definition.
"""
from vrtgen.types.struct import Struct

from .field import StructFieldParser, GenericFieldParser

class SectionParser:
    """
    Base class for parsers that manage a section of a VITA 49 packet
    definition.

    To define a section parser, create a subclass and register child parsers
    statically with that class.
    """
    __PARSERS__ = {}
    __ALIASES__ = {}

    def __init__(self, log, context):
        self.log = log
        self.context = context

    def __init_subclass__(cls, **kwargs):
        super().__init_subclass__(**kwargs)
        # Copy the class-wide parser tables on subclass creation so that they
        # can add or otherwise alter the parsers without affecting any other
        # classes in the hierarchy
        cls.__PARSERS__ = cls.__PARSERS__.copy()
        cls.__ALIASES__ = cls.__ALIASES__.copy()

    @classmethod
    def add_parser(cls, name, parser, alias=None):
        """
        Registers a generic parser for a given name.
        """
        assert parser is not None
        cls.__PARSERS__[name.casefold()] = parser
        if alias:
            cls.__ALIASES__[alias.casefold()] = name

    @classmethod
    def add_field_parser(cls, field, parser=None, alias=None):
        """
        Registers a parser for a specific VITA 49 field.
        """
        if parser is None:
            assert field.type is not None
            if issubclass(field.type, Struct):
                parser = StructFieldParser()
            else:
                parser = GenericFieldParser()
        cls.add_parser(field.name, parser, alias)

    def get_field_parser(self, field):
        parser = self.__PARSERS__.get(field.name.casefold(), None)
        if parser is None:
            raise ValueError("Unsupported field '{}'".format(field.name))
        return parser

    def parse_field(self, field, value):
        self.log.debug("Parsing field '%s'", field.name)
        parser = self.get_field_parser(field)
        try:
            parser(self.log, field, value)
        except (TypeError, ValueError) as exc:
            self.log.error("Invalid definition for '%s': %s", field.name, exc)

    def parse_option(self, name, value):
        return False

    def parse(self, value):
        for field_name, field_value in value.items():
            if self.parse_option(field_name, field_value):
                continue

            field_name = self.__ALIASES__.get(field_name.casefold(), field_name)
            field = self.context.get_field(field_name)
            if field is None:
                self.log.error("Invalid field '%s'", field_name)
                continue

            try:
                self.parse_field(field, field_value)
            except (TypeError, ValueError) as exc:
                self.log.error("Invalid value for field '%s': %s", field_name, exc)
            except Exception as exc:
                self.log.exception("Field '%s': %s", field_name, exc)
