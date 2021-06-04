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
Common packet generator backend support.
"""

import abc
import inspect

class Generator(abc.ABC):
    """
    Abstract base class for packet generator backends.

    Subclasses must implement the generate method.
    """
    @staticmethod
    def set_option(name, value):
        """
        Sets an option for this generator.
        """
        raise ValueError("invalid option '"+name+"'")

    @staticmethod
    def start_file(filename):
        """
        Notification for the start of processing of an input YAML file.

        Override if your generator needs to perform any actions at the start
        of an input file, such as opening an output file.
        """

    @staticmethod
    def end_file():
        """
        Notification for the end of processing of an input YAML file.

        Override if your generator needs to perform any actions at the end of
        an input file, such as closing an output file.
        """

    @abc.abstractmethod
    def generate(self, packet):
        """
        Generate output for a packet configuration.

        Must be implemented by subclasses.
        """

    @classmethod
    def get_options(cls):
        """
        Gets the command line configurable options.
        """
        return inspect.getmembers(cls, lambda x: isinstance(x, GeneratorOption))

class GeneratorOption:
    """
    Command line configurable attribute for Generator classes.

    When defining a generator, options should be declared as class-level
    attributes.
    """
    def __init__(self, opt, doc=None, dtype=None, defval=None):
        self.opt = opt
        self.help = doc
        self.type = dtype
        self.default = defval
        self.attr = None

    def __set_name__(self, owner, name):
        self.attr = '_' + name

    def __get__(self, instance, owner):
        if instance is None:
            return self
        return getattr(instance, self.attr)

    def __set__(self, instance, value):
        setattr(instance, self.attr, value)
