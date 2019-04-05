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

def int_type(bits):
    if bits > 16:
        return 'uint32_t'
    elif bits > 8:
        return 'uint16_t'
    else:
        return 'uint8_t'

def enum_type(name):
    return 'vrtgen::{}::Code'.format(name)

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

def format_enable_methods(field):
    return {
        'name': field.name,
        'doc': 'enable state of ' + field.name,
        'identifier': name_to_identifier(field.name + 'Enable'),
        'position': field.enable_bit,
        'type': 'bool',
    }

def format_value_methods(field):
    identifier = name_to_identifier(field.name)
    field_data = {
        'name': field.name,
        'doc': field.name,
        'identifier': identifier,
        'position': field.position,
    }
    if issubclass(field, EnumField):
        field_data['type'] = enum_type(identifier)
    elif issubclass(field, IntegerField):
        field_data['type'] = int_type(field.bits)
    return field_data

def format_header():
    fields = []
    for attr, field in VRTHeader.get_field_descriptors():
        identifier = name_to_identifier(field.name)
        field_data = {
            'name': field.name,
            'identifier': identifier,
        }
        if hasattr(field, 'enable_bit'):
            # Header word contains the bit flag to enable the field
            field_data = format_enable_methods(field)
        else:
            # Header word contains the field's value
            field_data = format_value_methods(field)
        fields.append(field_data)
    return fields

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

    template = env.get_template('header.hpp')
    with open(os.path.join(includedir, 'header.hpp'), 'w') as fp:
        fields = format_header()
        fp.write(template.render({'fields': fields}))

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
