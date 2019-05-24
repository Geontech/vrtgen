"""
Classes for parsing sections of a VITA 49 packet definition.
"""
from vrtgen.types.struct import Struct

from .field import StructFieldParser, SimpleFieldParser

def bind_parser(name, parser):
    """
    Wraps a field value parser to fetch the target field from the incoming
    context.
    """
    def wrapped_parse(log, context, value):
        field = context.get_field(name)
        assert field is not None
        log.debug("Parsing field '%s'", field.name)
        return parser(log.getChild(field.name), field, value)
    return wrapped_parse

class SectionParser:
    """
    Base class for parsers that manage a section of a VITA 49 packet
    definition.

    To define a section parser, create a subclass and register child parsers
    statically with that class.
    """
    __PARSERS__ = {}
    __ALIASES__ = {}

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
        if isinstance(field, str):
            assert parser is not None
            name = field
        else:
            assert field.type is not None
            if issubclass(field.type, Struct):
                if parser is None:
                    parser = StructFieldParser.factory(field)
                else:
                    parser = StructFieldParser(parser)
            else:
                if parser is None:
                    parser = SimpleFieldParser.factory(field)
                else:
                    parser = SimpleFieldParser(parser)
            name = field.name
        cls.add_parser(name, bind_parser(name, parser), alias)

    def parse(self, log, context, value):
        for name, field_value in value.items():
            field_name = self.__ALIASES__.get(name.casefold(), name)
            parser = self.__PARSERS__.get(field_name.casefold(), None)
            if parser is None:
                log.error("Invalid field '{}'".format(name))
                continue

            try:
                parser(log, context, field_value)
            except (TypeError, ValueError) as exc:
                log.error("Invalid value for field '%s': %s", field_name, exc)
            except Exception as exc:
                log.exception("Field '%s': %s", field_name, exc)
