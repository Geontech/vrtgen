"""
Classes for parsing sections of a VITA 49 packet definition.
"""
from vrtgen.types.struct import Struct

from .field import StructFieldParser, SimpleFieldParser
from .mapping import MappingParser

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

class SectionParser(MappingParser):
    """
    Base class for parsers that manage a section of a VITA 49 packet
    definition.

    To define a section parser, create a subclass and register child parsers
    statically with that class.
    """
    @classmethod
    def add_field_parser(cls, field, parser=None, alias=None):
        """
        Registers a parser for a specific VITA 49 field.
        """
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
