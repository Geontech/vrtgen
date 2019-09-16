# Copyright (C) 2019 Geon Technologies, LLC
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
