import os

import jinja2

from vrtgen.model import config
from vrtgen.model.field import Scope
from vrtgen.types import basic
from vrtgen.types import enums
from vrtgen.types import struct
from vrtgen.types import prologue
from vrtgen.types import cif0
from vrtgen.types import cif1

from vrtgen.backend.generator import Generator

from . import types as cpptypes

CIFS = (
    cif0.CIF0,
    cif1.CIF1,
)

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
    def __init__(self, name, packet, header_class):
        self.name = name
        self.helper = name + 'Helper'
        self.namespace = ''
        self.type = cpptypes.enum_value(packet.packet_type())
        self.header = {
            'type': 'vrtgen::packing::' + header_class,
            'fields': [],
        }
        self.set_header_field(prologue.Header.packet_type, cpptypes.enum_value(packet.packet_type()))
        self.set_header_field(prologue.Header.tsi, cpptypes.enum_value(packet.tsi.value))
        self.set_header_field(prologue.Header.tsf, cpptypes.enum_value(packet.tsf.value))
        self.set_header_field(prologue.Header.class_id_enable,
            str(packet.class_id.is_enabled).lower(),
            getter='isClassIdentifierEnabled',
            setter='setClassIdentifierEnabled',
        )

        self.prologue = []

        self.cifs = []
        self.add_cif(0)
        self.add_cif(1)
        self.fields = []
        self.members = []
        self.class_id = []
   
    def add_cif(self, number):
        self.cifs.append({
            'number': number,
            'header': 'vrtgen::packing::CIF{}Enables'.format(number),
            'enabled': False,
            'optional': False,
        })

    def set_header_field(self, field, value, getter=None, setter=None):
        name = cpptypes.name_to_identifier(field.name)
        if getter is None:
            getter = 'get' + name
        if setter is None:
            setter = 'set' + name
        self.header['fields'].append({
            'name': name,
            'title': field.name,
            'getter': getter,
            'setter': setter,
            'value': value,
        })

    def add_prologue_field(self, field):
        field_type = value_type(field.type)
        self.add_member(field.name, field_type, field.is_optional)
        identifier = cpptypes.name_to_identifier(field.name)
        self.prologue.append({
            'name': identifier,
            'type': 'vrtgen::packing::' + identifier
        })

    def add_field(self, field):
        cif = self.cifs[CIFS.index(field.field.owner)]
        field_type = value_type(field.type)
        self.add_member(field.name, field_type, field.is_optional)
        identifier = cpptypes.name_to_identifier(field.name)
        self.fields.append({
            'name': identifier,
            'title': field.name,
            'optional': field.is_optional,
            'type': 'vrtgen::packing::' + identifier,
            'cif': cif['number'],
        })
        if not cif['enabled']:
            cif['enabled'] = True
            cif['optional'] = True
        if not field.is_optional:
            cif['optional'] = False

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
        self.output_path = os.getcwd()
        self.header_ext = '.hpp'
        self.impl_ext = '.cpp'
        self.header = None
        self.implfile = None
        self.packets = []
        self.namespace = ''

    def start_file(self, filename):
        basename, _ = os.path.splitext(filename)
        header_file = os.path.join(self.output_path, basename + self.header_ext)
        self.header = open(header_file, 'w')
        impl_file = os.path.join(self.output_path, basename + self.impl_ext)
        self.implfile = open(impl_file, 'w')

    def end_file(self):
        context = {
            'packets':self.packets,
            'namespace': self.namespace,
        }
        template = self.env.get_template('header.hpp')
        self.header.write(template.render(context))
        self.header.close()

        context['header'] = os.path.basename(self.header.name)
        template = self.env.get_template('packet.cpp')
        self.implfile.write(template.render(context))
        self.implfile.close

    def generate_class_id(self, cppstruct, packet):
        if packet.class_id.is_disabled:
            return
        identifier = cpptypes.name_to_identifier(packet.class_id.name)
        class_id = {
            'name': identifier,
            'title': packet.class_id.name,
            'attr': 'class_id',
            'type': 'vrtgen::packing::' + identifier,
            'struct': True,
            'fields': [],
        }
        for field in packet.class_id.get_fields():
            if field.is_disabled:
                continue
            subfield = {
                'name': cpptypes.name_to_identifier(field.name),
                'title': field.name,
            }
            if field.is_constant:
                subfield['value'] = field.value
            else:
                field_type = value_type(field.type)
                cppstruct.add_member(field.name, field_type)
            class_id['fields'].append(subfield)
        cppstruct.prologue.append(class_id)

    def generate_prologue(self, cppstruct, packet):
        if not packet.stream_id.is_disabled:
            cppstruct.add_prologue_field(packet.stream_id)

        self.generate_class_id(cppstruct, packet)

        if packet.tsi.value != enums.TSI.NONE:
            cppstruct.add_prologue_field(packet.integer_timestamp)

        if packet.tsf.value != enums.TSF.NONE:
            cppstruct.add_prologue_field(packet.fractional_timestamp)

    def generate_payload(self, cppstruct, packet):
        for field in packet.get_fields(Scope.PAYLOAD):
            if field.is_disabled:
                continue
            cppstruct.add_field(field)

    def generate_data(self, cppstruct, packet):
        cppstruct.set_header_field(prologue.DataHeader.trailer_included, 'false')
        cppstruct.set_header_field(prologue.DataHeader.not_v49d0, 'true')
        cppstruct.set_header_field(prologue.DataHeader.spectrum, 'false')

        self.generate_prologue(cppstruct, packet)

        for field in packet.get_fields(Scope.TRAILER):
            if field.is_disabled:
                continue
            field_type = value_type(field.type)
            cppstruct.add_member(field.name, field_type, field.is_optional)

    def generate_context(self, cppstruct, packet):
        cppstruct.set_header_field(prologue.ContextHeader.not_v49d0, 'true')
        cppstruct.set_header_field(prologue.ContextHeader.timestamp_mode, cpptypes.enum_value(packet.timestamp_mode.value))
        cppstruct.cifs[0]['enabled'] = True

        self.generate_prologue(cppstruct, packet)
        self.generate_payload(cppstruct, packet)

    def generate_command(self, cppstruct, packet):
        cppstruct.set_header_field(prologue.CommandHeader.acknowledge_packet, 'false')
        cppstruct.set_header_field(prologue.CommandHeader.cancellation_packet, 'false')
        cppstruct.cifs[0]['enabled'] = True
        
        self.generate_prologue(cppstruct, packet)
        self.generate_payload(cppstruct, packet)

    def generate(self, packet):
        name = cpptypes.name_to_identifier(packet.name)
        if packet.packet_type() in (enums.PacketType.SIGNAL_DATA, enums.PacketType.SIGNAL_DATA_STREAM_ID):
            model = CppPacket(name, packet, 'DataHeader')
            self.generate_data(model, packet)
        elif isinstance(packet, config.ContextPacketConfiguration):
            model = CppPacket(name, packet, 'ContextHeader')
            self.generate_context(model, packet)
        elif isinstance(packet, config.CommandPacketConfiguration):
            model = CppPacket(name, packet, 'CommandHeader')
            self.generate_command(model, packet)

        self.packets.append(model)
