import os

import jinja2

from vrtgen.model import config
from vrtgen.model.field import Scope
from vrtgen.types import basic
from vrtgen.types import enums
from vrtgen.types import struct

from vrtgen.backend.generator import Generator

from . import types as cpptypes

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

def value_type(datatype):
    if issubclass(datatype, basic.BooleanType):
        return 'bool'
    if issubclass(datatype, enums.BinaryEnum):
        return cpptypes.enum_type(datatype)
    if issubclass(datatype, basic.FixedPointType):
        return cpptypes.float_type(datatype.bits)
    if issubclass(datatype, basic.IntegerType):
        return cpptypes.int_type(datatype.bits, datatype.signed)
    if issubclass(datatype, struct.Struct):
        return cpptypes.name_to_identifier(datatype.__name__)
    return 'TODO'

def optional_type(typename):
    return 'vrtgen::optional<{}>'.format(typename)

class CppPacket:
    def __init__(self, name):
        self.name = name
        self.namespace = ''
        self.fields = []
        self.structs = []
        self.members = []

    def add_field(self, field):
        self.fields.append(field)

    def add_member(self, name, datatype, optional=False):
        identifier = cpptypes.name_to_identifier(name)
        if optional:
            member_type = optional_type(datatype)
        else:
            member_type = datatype

        self.members.append({
            'name': name,
            'identifier': identifier,
            'optional': optional,
            'type': datatype,
            'member': {
                'identifier': 'm_'+identifier,
                'type': member_type,
            },
        })

class CppGenerator(Generator):
    def __init__(self):
        template_path = os.path.join(os.path.dirname(__file__), 'templates')
        loader = jinja2.FileSystemLoader(template_path)
        self.env = jinja2.Environment(loader=loader, **JINJA_OPTIONS)
        self.template = self.env.get_template('packet.hpp')

    def generate_class_id(self, cppstruct, packet):
        if packet.class_id.is_disabled:
            return
        for field in packet.class_id.get_fields():
            if field.is_disabled or field.is_constant:
                continue
            field_type = value_type(field.type)
            cppstruct.add_member(field.name, field_type)

    def generate_prologue(self, cppstruct, packet):
        if packet.tsi != enums.TSI.NONE:
            field_type = value_type(packet.integer_timestamp.type)
            cppstruct.add_member(packet.integer_timestamp.name, field_type)
            
        if packet.tsf != enums.TSF.NONE:
            field_type = value_type(packet.fractional_timestamp.type)
            cppstruct.add_member(packet.fractional_timestamp.name, field_type)

        if not packet.stream_id.is_disabled:
            field_type = value_type(packet.stream_id.type)
            cppstruct.add_member(packet.stream_id.name, field_type, packet.stream_id.is_optional)

        self.generate_class_id(cppstruct, packet)

    def generate_payload(self, cppstruct, packet):
        for field in packet.get_fields(Scope.PAYLOAD):
            if field.is_disabled:
                continue
            field_type = value_type(field.type)
            cppstruct.add_member(field.name, field_type, field.is_optional)

    def generate_data(self, cppstruct, packet):
        self.generate_prologue(cppstruct, packet)

        for field in packet.get_fields(Scope.TRAILER):
            if field.is_disabled:
                continue
            field_type = value_type(field.type)
            cppstruct.add_member(field.name, field_type, field.is_optional)

    def generate_context(self, cppstruct, packet):
        self.generate_prologue(cppstruct, packet)
        self.generate_payload(cppstruct, packet)

    def generate_command(self, cppstruct, packet):
        self.generate_prologue(cppstruct, packet)
        self.generate_payload(cppstruct, packet)

    def generate(self, packet):
        name = cpptypes.name_to_identifier(packet.name)
        model = CppPacket(name)
        if packet.packet_type() in (enums.PacketType.SIGNAL_DATA, enums.PacketType.SIGNAL_DATA_STREAM_ID):
            self.generate_data(model, packet)
        elif isinstance(packet, config.ContextPacketConfiguration):
            self.generate_context(model, packet)
        elif isinstance(packet, config.CommandPacketConfiguration):
            self.generate_command(model, packet)

        print(self.template.render({'packet':model}))
        