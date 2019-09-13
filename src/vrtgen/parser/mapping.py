"""
Classes for parsing mappings.
"""

class MappingParser:
    """
    Base class for parsers that handle a YAML mapping.

    To define a mapping parser, create a subclass and register child parsers
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
        Registers a parser for a given name.
        """
        assert parser is not None
        cls.__PARSERS__[name.casefold()] = parser
        if alias:
            cls.__ALIASES__[alias.casefold()] = name

    def parse_mapping(self, log, context, mapping):
        """
        Configures a context object from the values in a mapping.
        """
        for name, value in mapping.items():
            field_name = self.__ALIASES__.get(name.casefold(), name)
            parser = self.__PARSERS__.get(field_name.casefold(), None)
            if parser is None:
                log.error("Invalid name '{}'".format(name))
                continue

            try:
                parser(log, context, value)
            except (TypeError, ValueError) as exc:
                log.error("Invalid value for '%s': %s", field_name, exc)
            except RuntimeError as exc:
                log.exception("'%s': %s", field_name, exc)

    def __call__(self, log, context, value):
        if not isinstance(value, dict):
            raise TypeError('must be a mapping')
        self.parse_mapping(log, context, value)
