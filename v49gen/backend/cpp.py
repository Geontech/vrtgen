import jinja2

from v49gen.model.field import *
from v49gen.model.packets import *

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

def next_pow2(value):
    return 1<<(value-1).bit_length()

def get_default(value, defval):
    if value is None:
        return defval
    else:
        return value

class CppPacket:
    def __init__(self, name):
        self.name = name
        self.fields = []
        self.structs = []

    def add_field(self, field):
        self.fields.append(field)

class CppGenerator:
    def __init__(self, standard='c++03'):
        loader = jinja2.FileSystemLoader('v49gen/backend/templates')
        self.env = jinja2.Environment(loader=loader, **JINJA_OPTIONS)
        self.template = self.env.get_template('packet.hpp')
        self.standard = 'c++03'

    def map_prologue(self, model, packet):
        for field in packet.prologue.fields:
            if field.is_disabled:
                continue
            if isinstance(field, StructField):
                for subfield in field.fields:
                    if not subfield.is_enabled:
                        continue
                    model.add_field(self.map_simple_field(subfield))
            else:
                model.add_field(self.map_simple_field(field))

    def map_trailer(self, model, packet):
        if not packet.has_trailer:
            return

        for field in packet.trailer.fields:
            if field.is_disabled:
                continue
            model.add_field(self.map_simple_field(field))

    def map_simple_field(self, field, prefix=None):
        field_value = None
        if VRTDataTrailer.sample_frame.match(field.name):
            field_type = 'SSI::Flag'
            if field.value is not None:
                field_value = 'SSI::' + field.value.name
        elif isinstance(field, BitField):
            field_type = 'bool'
            field_value = str(get_default(field.value, False)).lower()
        elif isinstance(field, IntegerField):
            bits = max(8, next_pow2(field.bits))
            field_type = 'int{:d}_t'.format(bits)
            field_value = get_default(field.value, 0)
        elif isinstance(field, FixedPointField):
            if field.bits > 32:
                field_type = 'double'
            else:
                field_type = 'float'
            field_value = get_default(field.value, 0)
        elif isinstance(field, TSIField):
            field_type = 'TSI::Mode'
            field_value = 'TSI::' + field.value.name
        elif isinstance(field, TSFField):
            field_type = 'TSF::Mode'
            field_value = 'TSF::' + field.value.name
        else:
            field_type = 'void'
        field_name = field.name
        if prefix:
            field_name = prefix + field_name
        model_field = {
            'name' : field_name,
            'optional': field.is_optional,
            'const': field.is_constant,
        }
        model_field['type'] = field_type
        if field_value is not None:
            model_field['value'] = field_value
        model_field['identifier'] = self.name_to_identifier(field_name)
        member_type = field_type
        if field.is_optional:
            member_type = 'optional<{}>'.format(member_type)
        model_field['member'] = {
            'type': member_type,
            'identifier': 'm_' + model_field['identifier']
        }
        return model_field

    def map_payload(self, model, packet):
        if isinstance(packet, VRTDataPacket):
            return

        for field in packet.cif[0].fields:
            if field.is_disabled:
                continue
            if isinstance(field, StructField):
                for subfield in field.fields:
                    if not subfield.is_enabled:
                        continue
                    model.add_field(self.map_simple_field(subfield, prefix=field.name))
            else:
                model.add_field(self.map_simple_field(field))

    def name_to_identifier(self, name):
        identifier = ''
        for ch in name:
            if not ch.isalnum():
                continue
            identifier += ch
        return identifier

    def generate(self, packet):
        model = CppPacket(self.name_to_identifier(packet.name))
        self.map_prologue(model, packet)
        self.map_payload(model, packet)
        self.map_trailer(model, packet)

        print(self.template.render({'packet':model}))
