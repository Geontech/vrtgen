import os

import jinja2

from vrtgen.model import config
from vrtgen.model.field import Scope
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
        self.set_header_field(prologue.Header.tsi, cpptypes.enum_value(packet.tsi))
        self.set_header_field(prologue.Header.tsf, cpptypes.enum_value(packet.tsf))
        self.set_header_field(
            prologue.Header.class_id_enable,
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
        self.class_id = None

    def add_class_id(self, packet):
        self.class_id = self.create_packing_field(packet.class_id, namespaced=False)
        for field in packet.class_id.get_fields():
            if field.is_disabled or field.is_constant:
                continue
            field_type = cpptypes.value_type(field.type)
            self.add_member(field.name, field_type)
        self.prologue.append(self.class_id)

    def add_cif(self, number):
        self.cifs.append({
            'number': number,
            'header': 'vrtgen::packing::CIF{}Enables'.format(number),
            'enabled': False,
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
        field_type = cpptypes.value_type(field.type)
        self.add_member(field.name, field_type)
        identifier = cpptypes.name_to_identifier(field.name)
        self.prologue.append({
            'name': identifier,
            'type': 'vrtgen::packing::' + identifier
        })

    def create_packing_field(self, field, namespaced=True):
        identifier = cpptypes.name_to_identifier(field.name)
        packing = {
            'name': identifier,
            'title': field.name,
            'attr': field.field.attr,
            'optional': field.is_optional,
        }
        if issubclass(field.type, struct.Struct):
            packing['struct'] = True
            packing['fields'] = []
            packing['type'] = 'vrtgen::packing::' + cpptypes.name_to_identifier(field.type.__name__)
            for subfield in field.get_fields():
                if subfield.is_disabled:
                    continue
                if namespaced:
                    subfield_name = field.name + subfield.name
                else:
                    subfield_name = subfield.name
                subfield_dict = {
                    'name': cpptypes.name_to_identifier(subfield_name),
                    'srcname': cpptypes.name_to_identifier(subfield.name),
                    'title': subfield.name,
                }
                if subfield.is_constant:
                    subfield_dict['value'] = subfield.value
                packing['fields'].append(subfield_dict)
        else:
            packing['type'] = 'vrtgen::packing::' + identifier
        return packing

    def add_field(self, field):
        cif = self.cifs[CIFS.index(field.field.owner)]
        if not cif['enabled']:
            cif['enabled'] = True

        cif_field = self.create_packing_field(field)
        cif_field['cif'] = cif['number']
        self.fields.append(cif_field)

        if issubclass(field.type, struct.Struct):
            for subfield in field.get_fields():
                if subfield.is_disabled or subfield.is_constant:
                    continue
                subfield_name = field.name + subfield.name
                self.add_member_from_field(subfield, name=subfield_name)
        else:
            self.add_member_from_field(field)

    def add_member(self, name, datatype, optional=False, value=None):
        identifier = cpptypes.name_to_identifier(name)
        if optional:
            member_type = optional_type(datatype)
        else:
            member_type = datatype

        member = {
            'name': name,
            'identifier': identifier,
            'optional': optional,
            'type': datatype,
            'member': {
                'identifier': 'm_'+identifier,
                'type': member_type,
            },
        }
        if value is not None:
            member['value'] = value
        self.members.append(member)

    def add_member_from_field(self, field, name=None):
        if name is None:
            name = field.name
        self.add_member(name, cpptypes.value_type(field.type), field.is_optional, field.value)


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
        self.header = basename + self.header_ext
        self.implfile = basename + self.impl_ext

    def end_file(self):
        context = {
            'packets':self.packets,
            'namespace': self.namespace,
        }
        template = self.env.get_template('header.hpp')
        with open(os.path.join(self.output_path, self.header), 'w') as fp:
            fp.write(template.render(context))

        context['header'] = self.header
        template = self.env.get_template('packet.cpp')
        with open(os.path.join(self.output_path, self.implfile), 'w') as fp:
            fp.write(template.render(context))

    def generate_prologue(self, cppstruct, packet):
        if not packet.stream_id.is_disabled:
            cppstruct.add_prologue_field(prologue.Prologue.stream_id)

        if not packet.class_id.is_disabled:
            cppstruct.add_class_id(packet)

        if packet.tsi != enums.TSI.NONE:
            cppstruct.add_prologue_field(prologue.Prologue.integer_timestamp)

        if packet.tsf != enums.TSF.NONE:
            cppstruct.add_prologue_field(prologue.Prologue.fractional_timestamp)

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
            cppstruct.add_member_from_field(field)

    def generate_context(self, cppstruct, packet):
        cppstruct.set_header_field(prologue.ContextHeader.not_v49d0, 'true')
        cppstruct.set_header_field(prologue.ContextHeader.timestamp_mode, cpptypes.enum_value(packet.timestamp_mode))
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
