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

class CppGenerator:
    def __init__(self, standard='c++03'):
        loader = jinja2.FileSystemLoader('v49gen/backend/templates')
        self.env = jinja2.Environment(loader=loader, **JINJA_OPTIONS)
        self.template = self.env.get_template('packet.hpp')
        self.standard = 'c++03'

    def map_payload(self, packet):
        if isinstance(packet, VRTDataPacket):
            return []

        payload = []
        for field in packet.cif[0].fields:
            if field.is_disabled:
                continue
            field_type = 'int32_t'
            model_field = {
                'name' : field.name,
                'optional': field.is_optional,
                'const': field.is_constant,
            }
            model_field['type'] = field_type
            model_field['identifier'] = self.name_to_identifier(field.name)
            member_type = field_type
            if field.is_optional:
                member_type = 'optional<{}>'.format(member_type)
            model_field['member'] = {
                'type': member_type,
                'identifier': 'm_' + model_field['identifier']
            }
            payload.append(model_field)
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
        model['prologue'] = {}
        model['payload'] = self.map_payload(packet)

        print(self.template.render({'packet':model}))
