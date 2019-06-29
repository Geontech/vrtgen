import os

import jinja2

from vrtgen.model import config
from vrtgen.model.field import Scope
from vrtgen.types import basic
from vrtgen.types import enums
from vrtgen.types import struct

from vrtgen.backend.generator import Generator

from . import types as cpptypes

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
    
JINJA_OPTIONS = {
    'trim_blocks': True,
    'lstrip_blocks': True,
    'keep_trailing_newline': True,
    'line_statement_prefix': '//%',
    'block_start_string':    '/*%',
    'block_end_string':      '%*/',
    'comment_start_string':  '/*#',
    'comment_end_string':    '#*/'
}

def next_pow2(value):
    return 1<<(value-1).bit_length()

def value_type(datatype):
    if datatype == basic.StreamIdentifier:
        return 'vrtgen::StreamIdentifier'
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
    raise NotImplementedError(datatype.__name__)

def optional_type(typename):
    return 'vrtgen::optional<{}>'.format(typename)

class CppPacket:
    def __init__(self, name, packet):
        self.name = name
        self.namespace = ''
        self.type = cpptypes.enum_value(packet.packet_type())
        self.has_stream_id = packet.stream_id.is_enabled
        self.has_class_id = packet.class_id.is_enabled
        self.has_integer_timestamp = packet.tsi.value != enums.TSI.NONE
        self.has_fractional_timestamp = packet.tsf.value != enums.TSF.NONE
        self.header = {
            'fields': [],
        }
        self.set_header_field('PacketType', cpptypes.enum_value(packet.packet_type()))
        self.set_header_field('TSI', cpptypes.enum_value(packet.tsi.value))
        self.set_header_field('TSF', cpptypes.enum_value(packet.tsf.value))
        self.set_header_field('ClassIdentifierEnabled', str(self.has_class_id).lower(), getter='isClassIdentifierEnabled')

        if packet.packet_type() in (enums.PacketType.SIGNAL_DATA, enums.PacketType.SIGNAL_DATA_STREAM_ID):
            self.header['type'] = 'vrtgen::packing::DataHeader'
        elif packet.packet_type() == enums.PacketType.CONTEXT:
            self.header['type'] = 'vrtgen::packing::ContextHeader'
        elif packet.packet_type() == enums.PacketType.COMMAND:
            self.header['type'] = 'vrtgen::packing::CommandHeader'

        self.fields = []
        self.structs = []
        self.members = []

    def set_header_field(self, name, value, setter=None, getter=None):
        if setter is None:
            setter = 'set' + name
        if getter is None:
            getter = 'get' + name
        self.header['fields'].append({
            'name': name,
            'getter': getter,
            'setter': setter,
            'value': value,
        })

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
        self.env.filters['namespace'] = do_namespace
        self.template = self.env.get_template('header.hpp')
        self.output_path = os.getcwd()
        self.header_ext = '.hpp'
        self.header = None
        self.packets = []
        self.namespace = ''

    def start_file(self, filename):
        basename, _ = os.path.splitext(filename)
        header_file = os.path.join(self.output_path, basename + self.header_ext)
        self.header = open(header_file, 'w')

    def end_file(self):
        context = {
            'packets':self.packets,
            'namespace': self.namespace,
        }
        self.header.write(self.template.render(context))
        self.header.close()

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
        model = CppPacket(name, packet)
        if packet.packet_type() in (enums.PacketType.SIGNAL_DATA, enums.PacketType.SIGNAL_DATA_STREAM_ID):
            self.generate_data(model, packet)
        elif isinstance(packet, config.ContextPacketConfiguration):
            self.generate_context(model, packet)
        elif isinstance(packet, config.CommandPacketConfiguration):
            self.generate_command(model, packet)

        self.packets.append(model)
