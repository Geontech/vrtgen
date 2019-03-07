from v49gen.model.enums import *
from v49gen.model.packets import *

import jinja2
import sys

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
    enums = [
        format_enum(PacketType, '0b{:04b}'),
        format_enum(TSI, '0b{:02b}'),
        format_enum(TSF, '0b{:02b}'),
        format_enum(TSM),
        format_enum(SSI, '0b{:02b}'),
        format_enum(PackingMethod),
        format_enum(DataSampleType, '0b{:02b}'),
        format_enum(DataItemFormat, '0b{:05b}'),
    ]
    with open('enums.hpp', 'w') as fp:
        fp.write(template.render({'enums': enums}))

    template = env.get_template('cif.hpp')
    for cif in [CIF0, CIF1]:
        filename = cif.__name__.lower() + '.hpp'
        with open(filename, 'w') as fp:
            fp.write(template.render(format_cif(cif)))

if __name__ == '__main__':
    main()
