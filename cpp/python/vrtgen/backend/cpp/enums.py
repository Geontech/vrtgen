# Copyright (C) 2020 Geon Technologies, LLC
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
Helper methods for generating C++ enumerations.
"""

from . import utils

def format_enum(enum):
    """
    Returns a description of a BinaryEnum type for use in code generation.
    """
    # Create a format string that returns a hex constant (binary constants are
    # a C++14 feature)
    digits = int((enum.bits + 3) / 4)
    format_string = '0x{{:0{}x}}'.format(digits)
    return {
        'name': enum.__name__,
        'doc': utils.format_docstring(enum.__doc__),
        'values': [(e.name, format_string.format(e.value)) for e in enum]
    }
