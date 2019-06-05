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
from vrtgen.types.struct import Struct

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

def get_structs(module):
    def is_struct(obj):
        if not inspect.isclass(obj) or not issubclass(obj, Struct):
            return False
        return obj.__module__ == module.__name__
    return [cls for _, cls in inspect.getmembers(module, is_struct)]

def int_type(bits, signed):
    if bits > 32:
        ctype = 'int64_t'
    elif bits > 32:
        ctype = 'int32_t'
    elif bits > 8:
        ctype = 'int16_t'
    else:
        ctype = 'int8_t'
    if not signed:
        ctype = 'u' + ctype
    return ctype

def fixed_type(bits, radix):
    #return int_type(bits, True)
    return 'fixed<{},{}>'.format(bits, radix)

def enum_type(datatype):
    name = name_to_identifier(datatype.__name__)
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

def format_enable_methods(field, enable=None):
    if enable is None:
        enable = field
    identifier = name_to_identifier(field.name + 'Enabled')
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
        'word': enable.word,
        'offset': enable.offset,
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
        'word': field.word,
        'offset': field.offset,
    }
    datatype = field.type
    if issubclass(datatype, enums.BinaryEnum):
        field_data['type'] = enum_type(datatype)
        field_data['bits'] = datatype.bits
    elif issubclass(datatype, basic.IntegerType):
        field_data['type'] = int_type(datatype.bits, datatype.signed)
        field_data['bits'] = datatype.bits
    elif issubclass(datatype, basic.FixedPointType):
        field_data['type'] = fixed_type(datatype.bits, datatype.radix)
        field_data['bits'] = datatype.bits
    elif datatype == basic.Boolean:
        field_data['type'] = 'bool'
        field_data['bits'] = 1
    return field_data

def format_header():
    fields = []
    for field in prologue.Header.get_fields():
        identifier = name_to_identifier(field.name)
        if field == prologue.Header.class_id_enable:
            field_data = format_enable_methods(field)
        else:
            field_data = format_value_methods(field)
        fields.append(field_data)
    return fields

def format_struct(structdef, fields, bits=None):
    if bits is None:
        bits = structdef.bits
    return {
        'name': structdef.__name__,
        'doc': format_docstring(structdef.__doc__),
        'words': bits // 32,
        'fields': fields,
    }

def format_cif_struct(structdef):
    fields = []
    for field in structdef.get_fields():
        identifier = name_to_identifier(field.name)
        field_data = format_value_methods(field)
        fields.append(field_data)
    return format_struct(structdef, fields)

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
        structs = [format_struct(prologue.Header, format_header())]
        structs.append(format_cif_struct(prologue.ClassIdentifier))
        fp.write(template.render({
            'name': 'header',
            'structs': structs,
        }))

    template = env.get_template('struct.hpp')
    with open(os.path.join(includedir, 'trailer.hpp'), 'w') as fp:
        fields = []
        for field in trailer.Trailer.get_fields():
            fields.append(format_enable_methods(field, field.enable))
            fields.append(format_value_methods(field))
        fp.write(template.render({
            'name': 'trailer',
            'structs': [format_struct(trailer.Trailer, fields)],
        }))

    template = env.get_template('struct.hpp')

    for module, cif in [(cif0, cif0.CIF0), (cif1, cif1.CIF1)]:
        filename = cif.__name__.lower() + '.hpp'
        with open(os.path.join(includedir, filename), 'w') as fp:
            structs = []
            fields = []
            for field in cif.Enables.get_fields():
                fields.append(format_enable_methods(field))
            structs.append(format_struct(cif, fields, bits=cif.Enables.bits))
            for structdef in get_structs(module):
                structs.append(format_cif_struct(structdef))
            fp.write(template.render({
                'name': cif.__name__,
                'structs': structs,
            }))

if __name__ == '__main__':
    main()
