from vrtgen.model import enums
from vrtgen.model.packets import *

import jinja2
import sys
import os

JINJA_OPTIONS = {
    'trim_blocks':           True,
    'line_statement_prefix': '//%',
    'variable_start_string': '${',
    'variable_end_string':   '}',
    'block_start_string':    '/*{%',
    'block_end_string':      '%}*/',
    'comment_start_string':  '/*#',
    'comment_end_string':    '#*/'
}

def format_docstring(doc):
    if not doc:
        return
    if doc.startswith('\n'):
        doc = doc[1:]
    indent = 0
    while doc[indent].isspace():
        indent += 1
    for line in doc.rstrip().split('\n'):
        yield line[indent:]

def name_to_identifier(name):
    identifier = ''
    for ch in name:
        if ch.isalnum():
            identifier += ch
    return identifier

def format_enum(enum, format='{}'):
    return {
        'name': enum.__name__,
        'doc': format_docstring(enum.__doc__),
        'format': format.format,
        'values': list(enum)
    }

def format_cif(cif):
    fields = []
    for attr, field in cif.get_field_descriptors():
        fields.append({'name': name_to_identifier(field.name), 'bit': field.enable_bit})
    return {
        'name': cif.__name__,
        'fields': fields
    }

def main():
    loader = jinja2.FileSystemLoader('templates')
    env = jinja2.Environment(loader=loader, **JINJA_OPTIONS)
    template = env.get_template('enums.hpp')
    enum_types = [
        format_enum(enums.PacketType, '0b{:04b}'),
        format_enum(enums.TSI, '0b{:02b}'),
        format_enum(enums.TSF, '0b{:02b}'),
        format_enum(enums.TSM),
        format_enum(enums.SSI, '0b{:02b}'),
        format_enum(enums.PackingMethod),
        format_enum(enums.DataSampleType, '0b{:02b}'),
        format_enum(enums.DataItemFormat, '0b{:05b}'),
    ]
    includedir = 'include/vrtgen'
    os.makedirs(includedir, exist_ok=True)
    with open(os.path.join(includedir, 'enums.hpp'), 'w') as fp:
        fp.write(template.render({'enums': enum_types}))

    includedir = 'include/vrtgen/packing'
    os.makedirs(includedir, exist_ok=True)
    template = env.get_template('trailer.hpp')
    with open(os.path.join(includedir, 'trailer.hpp'), 'w') as fp:
        enables = []
        fields = []
        for attr, field in VRTDataTrailer.get_field_descriptors():
            identifier = name_to_identifier(field.name)
            enables.append({'name': field.name, 'identifier': identifier, 'enable_bit': field.enable_bit})
            if issubclass(field, BitField):
                fields.append({'name': field.name, 'identifier': identifier, 'position': field.position})
        fp.write(template.render({'enables': enables, 'fields': fields}))

    template = env.get_template('cif.hpp')

    for cif in [CIF0, CIF1]:
        filename = cif.__name__.lower() + '.hpp'
        with open(os.path.join(includedir, filename), 'w') as fp:
            fp.write(template.render(format_cif(cif)))

if __name__ == '__main__':
    main()
