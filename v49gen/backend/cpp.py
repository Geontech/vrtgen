import jinja2

from v49gen.packet import *

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

class CppGenerator:
    def __init__(self, standard='c++03'):
        loader = jinja2.FileSystemLoader('v49gen/backend/templates')
        self.env = jinja2.Environment(loader=loader, **JINJA_OPTIONS)
        self.template = self.env.get_template('packet.hpp')
        self.standard = 'c++03'

    def map_prologue(self, packet):
        prologue = []
        for field in packet.prologue.fields:
            if field.is_disabled:
                continue
            if isinstance(field, StructField):
                for subfield in field.fields:
                    if not subfield.is_enabled:
                        continue
                    prologue.append(self.map_simple_field(subfield))
            else:
                prologue.append(self.map_simple_field(field))
        return prologue

    def map_simple_field(self, field):
        field_value = None
        if isinstance(field, IntegerField):
            bits = next_pow2(field.bits)
            field_type = 'int{:d}_t'.format(bits)
            field_value = field.value
            if field_value is None:
                field_value = 0
        elif isinstance(field, FixedPointField):
            if field.bits > 32:
                field_type = 'double'
            else:
                field_type = 'float'
            field_value = field.value
            if field_value is None:
                field_value = 0.0
        elif isinstance(field, TSIField):
            field_type = 'TSI::Mode'
            field_value = 'TSI::' + field.value.name
        elif isinstance(field, TSFField):
            field_type = 'TSF::Mode'
            field_value = 'TSF::' + field.value.name
        else:
            field_type = 'void'
        model_field = {
            'name' : field.name,
            'optional': field.is_optional,
            'const': field.is_constant,
        }
        model_field['type'] = field_type
        if field_value is not None:
            model_field['value'] = field_value
        model_field['identifier'] = self.name_to_identifier(field.name)
        member_type = field_type
        if field.is_optional:
            member_type = 'optional<{}>'.format(member_type)
        model_field['member'] = {
            'type': member_type,
            'identifier': 'm_' + model_field['identifier']
        }
        return model_field

    def map_payload(self, packet):
        if isinstance(packet, VRTDataPacket):
            return []

        payload = []
        for field in packet.cif[0].fields:
            if field.is_disabled:
                continue
            if isinstance(field, StructField):
                pass
            else:
                payload.append(self.map_simple_field(field))
        return payload

    def name_to_identifier(self, name):
        identifier = ''
        for ch in name:
            if not ch.isalnum():
                continue
            identifier += ch
        return identifier

    def generate(self, packet):
        model = {}
        model['name'] = packet.name
        fields = self.map_prologue(packet)
        fields.extend(self.map_payload(packet))
        model['fields'] = fields

        print(self.template.render({'packet':model}))
