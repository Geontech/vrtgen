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
Common helpers for C++ code generation using Jinja.
"""

JINJA_OPTIONS = {
    'trim_blocks': True,
    'lstrip_blocks': True,
    'keep_trailing_newline': True,
    'comment_start_string':  '/*#',
    'comment_end_string':    '#*/',
    'line_comment_prefix':   '//#',
}

def do_namespace(text, namespace):
    """
    Jinja filter to add a C++ namespace around a block of text.
    """
    if not namespace:
        return text
    def apply_namespace(text, namespace):
        indent = ' '*4
        prefix = ''
        for segment in namespace.split('::'):
            yield prefix + 'namespace ' + segment + '{'
            prefix += indent
        for line in text.splitlines():
            yield prefix + line
        for segment in namespace.split('::'):
            prefix = prefix[:-len(indent)]
            yield prefix + '}'
    return '\n'.join(apply_namespace(text, namespace))

def sort_packed_tags(fields):
    return sorted(
        sorted(fields, key=lambda f: f.packed_tag.position, reverse=True),
        key=lambda f: f.packed_tag.field_word
    )

def format_docstring(doc):
    """
    Formats a Python docstring as input text for a Doxygen comment.
    """
    if not doc:
        return
    if doc.startswith('\n'):
        doc = doc[1:]
    indent = 0
    while doc[indent].isspace():
        indent += 1
    for line in doc.rstrip().split('\n'):
        yield line[indent:]
