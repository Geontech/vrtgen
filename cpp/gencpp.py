import inspect
import os
import sys

import jinja2

from vrtgen.types import basic
from vrtgen.types import enums
from vrtgen.types import prologue
from vrtgen.types import trailer
from vrtgen.types import cif0
from vrtgen.types import cif1

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

def is_enum(obj):
    # Ignore anything that isn't a BinaryEnum class
    if not inspect.isclass(obj) or not issubclass(obj, enums.BinaryEnum):
        return False
    # Only return concrete enums (i.e., those that have values defined) to
    # filter out abstract base classes (just BinaryEnum at present)
    return bool(obj.__members__)

def int_type(bits):
    if bits > 16:
        return 'uint32_t'
    elif bits > 8:
        return 'uint16_t'
    else:
        return 'uint8_t'

def enum_type(field):
    name = name_to_identifier(field.type.__name__)
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

def format_enum(enum):
    # Create a format string that returns a binary constant zero-padded to the
    # number of bits, e.g.: "0b0001"
    format_string = '0b{{:0{}b}}'.format(enum.bits)
    return {
        'name': enum.__name__,
        'doc': format_docstring(enum.__doc__),
        'format': format_string.format,
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
    identifier = name_to_identifier(field.name + 'Enabled')
    if field.enable is None:
        offset = field.offset
    else:
        offset = field.enable.offset
    return {
        'name': field.name,
        'doc': 'enable state of ' + field.name,
        'getter': {
            'doc': 'Get enabled state of ' + field.name,
            'name' : 'is'+identifier,
        },
        'setter': {
            'doc': 'Set enabled state of ' + field.name,
            'name' : 'set'+identifier,
        },
        'position': offset,
        'type': 'bool',
    }

def format_value_methods(field):
    identifier = name_to_identifier(field.name)
    field_data = {
        'name': field.name,
        'getter': {
            'doc': 'Get current value of ' + field.name,
            'name' : 'get'+identifier,
        },
        'setter': {
            'doc': 'Set current value of ' + field.name,
            'name' : 'set'+identifier,
        },
        'position': field.offset,
    }
    if issubclass(field.type, enums.BinaryEnum):
        field_data['type'] = enum_type(field)
        field_data['bits'] = field.bits
    elif issubclass(field.type, basic.IntegerType):
        field_data['type'] = int_type(field.bits)
        field_data['bits'] = field.bits
    elif field.type == basic.Boolean:
        field_data['type'] = 'bool'
        field_data['bits'] = 1
    return field_data

def format_header():
    fields = []
    for field in prologue.Header.get_fields():
        identifier = name_to_identifier(field.name)
        field_data = {
            'name': field.name,
            'identifier': identifier,
        }
        if field == prologue.Header.class_id_enable:
            field_data = format_enable_methods(field)
        else:
            field_data = format_value_methods(field)
        fields.append(field_data)
    return fields

def main():
    loader = jinja2.FileSystemLoader('templates')
    env = jinja2.Environment(loader=loader, **JINJA_OPTIONS)
    template = env.get_template('enums.hpp')
    enum_types = [format_enum(en) for _, en in inspect.getmembers(enums, is_enum)]
    includedir = 'include/vrtgen'
    os.makedirs(includedir, exist_ok=True)
    with open(os.path.join(includedir, 'enums.hpp'), 'w') as fp:
        fp.write(template.render({'enums': enum_types}))

    includedir = 'include/vrtgen/packing'
    os.makedirs(includedir, exist_ok=True)

    template = env.get_template('struct.hpp')
    with open(os.path.join(includedir, 'header.hpp'), 'w') as fp:
        fields = format_header()
        fp.write(template.render({'name': 'Header', 'fields': fields}))

    template = env.get_template('struct.hpp')
    with open(os.path.join(includedir, 'trailer.hpp'), 'w') as fp:
        fields = []
        for field in trailer.Trailer.get_fields():
            fields.append(format_enable_methods(field))
            fields.append(format_value_methods(field))
        fp.write(template.render({'name': 'Trailer', 'fields': fields}))

    template = env.get_template('struct.hpp')

    for cif in [cif0.CIF0, cif1.CIF1]:
        filename = cif.__name__.lower() + '.hpp'
        with open(os.path.join(includedir, filename), 'w') as fp:
            fields = []
            for field in cif.Enables.get_fields():
                if field.type is None:
                    continue
                fields.append(format_enable_methods(field))
            fp.write(template.render({'name': cif.__name__, 'fields': fields}))

if __name__ == '__main__':
    main()
