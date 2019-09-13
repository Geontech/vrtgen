"""
Common packet generator backend support.
"""

import abc

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
