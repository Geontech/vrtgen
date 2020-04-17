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
Helper utilities for YAML parsing.
"""

EMPTY = object()

def to_kvpair(value):
    """
    Converts a YAML value to a key-value pair.

    If value is a dictionary, it must have a single entry. The key and value
    are returned.

    If value is a list, a TypeError is raised.

    Otherwise, value is assumed to be a scalar. It it converted to a string and
    returned with the special object EMPTY as its value to indicate that there
    is none.
    """
    if isinstance(value, list):
        raise TypeError('must be name or single-key mapping')
    if isinstance(value, dict):
        if len(value) != 1:
            raise TypeError('must be single-key mapping')
        return next(iter(value.items()))
    return str(value), EMPTY
