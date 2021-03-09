# Copyright (C) 2019 Geon Technologies, LLC
#
# This file is part of vrtgen.
#
# vrtgen is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# vrtgen is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
# more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.

import os, subprocess

import jinja2

from vrtgen.model.config import PacketType, InformationType
from vrtgen.model.field import Scope
from vrtgen.types import enums
from vrtgen.types import struct
from vrtgen.types import prologue
from vrtgen.types import cif0
from vrtgen.types import cif1
from vrtgen.types import control

from vrtgen.backend.generator import Generator, GeneratorOption

from . import types as cpptypes
from .jinja import JINJA_OPTIONS, do_namespace
from .struct import CppStruct

CIFS = (0,1)

PACKING_LIB = 'vrtgen::packing::'

def optional_type(typename):
    return 'vrtgen::optional<{}>'.format(typename)

def header_type(packet_type):
    retval = 'vrtgen::packing::'
    if packet_type == PacketType.DATA:
        retval += 'DataHeader'
    elif packet_type == PacketType.CONTEXT:
        retval += 'ContextHeader'
    elif packet_type in (PacketType.CONTROL, PacketType.ACKS, PacketType.ACKV, PacketType.ACKX):
        retval += 'CommandHeader'
    return retval

def prologue_type(packet_type_code):
    retval = 'generic'
    if packet_type_code in (enums.PacketType.COMMAND, enums.PacketType.EXTENSION_COMMAND):
        retval = 'command'
    return retval

def get_enabled_accessors(name):
    identifier = cpptypes.name_to_identifier(name)
    getter = 'is{}Enabled'.format(identifier)
    setter = 'set{}Enabled'.format(identifier)
    return (getter, setter)

def get_accessors(field, name=None):
    if name is None:
        name = field.name
    if struct.is_enable(field):
        return get_enabled_accessors(name)

    identifier = cpptypes.name_to_identifier(name)
    getter = 'get' + identifier
    setter = 'set' + identifier
    return (getter, setter)

class CppPacket:
    def __init__(self, name, packet):
        self.name = name
        self.helper = name + 'Helper'
        self.namespace = ''
        self.packet_type = cpptypes.enum_value(packet.packet_type_code)
        self.header = {}
        self.prologue = {}
        self.cam = None
        self.cifs = []
        self.fields = []
        self.structs = []
        self.ack = None
        self.ack_type = None

        self.__init_header(packet)
        self.__init_prologue(packet)
        self.__init_payload(packet)
        self.__init_ack(packet)

    @property
    def is_variable_length(self):
        retval = False
        for cif in [c for c in self.cifs if c.get('enabled', False)]:
            retval |= any(field.get('optional', False) for field in cif.get('fields', []))
        return retval

    @property
    def is_reqs(self):
        return (self.ack_type and self.ack_type == PacketType.ACKS)

    @property
    def is_reqx(self):
        return (self.ack_type and self.ack_type == PacketType.ACKX)

    @property
    def is_data(self):
        return ('data' in self.packet_type.lower())

    @property
    def is_context(self):
        return ('context' in self.packet_type.lower())

    def __init_header(self, packet):
        self.header['type'] = header_type(packet.config_type)
        self.header['fields'] = []
        self.__set_header_field(prologue.Header.packet_type, cpptypes.enum_value(packet.packet_type_code))
        self.__set_header_field(prologue.Header.tsi, cpptypes.enum_value(packet.tsi))
        self.__set_header_field(prologue.Header.tsf, cpptypes.enum_value(packet.tsf))
        self.__set_header_field(prologue.Header.class_id_enable,
                                str(packet.class_id.is_enabled).lower(),
                                getter='isClassIdentifierEnabled',
                                setter='setClassIdentifierEnabled',
                                )
        if packet.config_type == PacketType.DATA:
            self.__set_header_field(prologue.DataHeader.trailer_included, 'false')
        for field in packet.get_fields(Scope.HEADER):
            self.__set_header_field(field)

    def __init_prologue(self, packet):
        self.prologue['type'] = prologue_type(packet.config_type)
        self.prologue['fields'] = []
        # Stream ID
        if not packet.stream_id.is_disabled:
            self.__add_prologue_field(prologue.Prologue.stream_id)
        # Class ID
        if not packet.class_id.is_disabled:
            self.__add_class_id(packet)
        # Timestamp Integer
        if packet.tsi != enums.TSI.NONE:
            self.__add_prologue_field(prologue.Prologue.integer_timestamp)
        # Timestamp Fractional
        if packet.tsf != enums.TSF.NONE:
            self.__add_prologue_field(prologue.Prologue.fractional_timestamp)
        # Additional command prologue
        type_code = cpptypes.enum_value(packet.packet_type_code)
        if packet.packet_type_code in (enums.PacketType.COMMAND, enums.PacketType.COMMAND):
            self.__set_header_field(prologue.CommandHeader.cancellation_packet, 'false')
            self.__init_command_prologue(packet)
            
    def __init_command_prologue(self, packet):
        # Extra header fields
        if packet.config_type == PacketType.CONTROL:
            self.__set_header_field(prologue.CommandHeader.acknowledge_packet, 'false')
        elif packet.config_type in (PacketType.ACKS, PacketType.ACKV, PacketType.ACKX):  
            self.__set_header_field(prologue.CommandHeader.acknowledge_packet, 'true')

        # CAM
        self.cam = {
            'name' : 'ControlAcknowledgeMode',
            'attr': 'cam',
            'type' : 'vrtgen::packing::ControlAcknowledgeMode',
            'struct': True,
            'fields': [],
        }
        for field in packet.get_fields(Scope.CAM):
            self.__add_cam_field(field)

        # Message ID
        self.__add_prologue_field(control.CommandPrologue.message_id, post_cam=True)

        # Controllee
        controllee_enable = packet.controllee is not None
        self.__set_cam_field(control.ControlAcknowledgeMode.controllee_enable, controllee_enable)
        if controllee_enable:
            self.__set_cam_field(control.ControlAcknowledgeMode.controllee_format, packet.controllee)
            if packet.controllee == enums.IdentifierFormat.WORD:
                self.__add_prologue_field(control.CommandPrologue.controllee_id, post_cam=True)
            elif packet.controllee == enums.IdentifierFormat.UUID:
                self.__add_prologue_field(control.CommandPrologue.controllee_id_uuid, post_cam=True)

        # Controller
        controller_enable = packet.controller is not None
        self.__set_cam_field(control.ControlAcknowledgeMode.controller_enable, controller_enable)
        if controller_enable:
            self.__set_cam_field(control.ControlAcknowledgeMode.controller_format, packet.controller)
            if packet.controller == enums.IdentifierFormat.WORD:
                self.__add_prologue_field(control.CommandPrologue.controller_id, post_cam=True)
            elif packet.controller == enums.IdentifierFormat.UUID:
                self.__add_prologue_field(control.CommandPrologue.controller_id_uuid, post_cam=True)

    def __init_payload(self, packet):
        for number in CIFS:
            self.cifs.append({
                'number': number,
                'header': PACKING_LIB + 'CIF{}Enables'.format(number),
                'enabled': (number == 0 and packet.config_type != PacketType.DATA),
                'fields': []
            })
        for field in packet.get_fields(Scope.CIF0, Scope.CIF1):
            if field.is_disabled:
                continue
            if field.is_user_defined:
                self.__add_struct(CppStruct(field.type))
            cif_num = 0
            if field.scope == Scope.CIF1:
                cif_num = 1
            self.__add_cif_field(field, cif_num)
        for field in packet.get_fields(Scope.TRAILER):
            if field.is_disabled:
                continue
            # TODO figure this out
            self.__create_member_from_field(field)

    def __init_ack(self, packet):
        if packet.packet_type_code is enums.PacketType.COMMAND and packet.config_type == PacketType.CONTROL:
            ack_name = self.name + 'Ack'
            if packet.acks.value:
                self.ack_type = PacketType.ACKS
            elif packet.ackx.value:
                self.ack_type = PacketType.ACKX
            if self.ack_type:
                packet.config_type = self.ack_type
                self.ack = CppPacket(ack_name, packet)

    def __add_prologue_field(self, field, post_cam=False):
        identifier = cpptypes.name_to_identifier(field.name)
        field_type = cpptypes.value_type(field.type)
        prologue_field = {
            'name': field.name,
            'identifier': identifier,
            'type': PACKING_LIB + identifier,
            'post_cam': post_cam,
            'member': self.__create_member(field.name, field_type)
        }
        if 'UUID' in field_type:
            prologue_field['type'] = PACKING_LIB + 'UUID'
            prologue_field['member']['datatype'] = 'std::string'
        self.prologue['fields'].append(prologue_field)

    def __add_cam_field(self, field):
        member = None
        if field.is_optional:
            # In the context of CAM, "optional" fields mean that the value can
            # vary (e.g., warnings might be set or not set on an AckV packet),
            # not that its presence is optional.
            member = self.__create_member_from_field(field, optional=False)
            value = None
        else:
            value = field.value
            if value is None:
                value = field.type()
        self.__set_cam_field(field.field, value, member)

    def __add_cif_field(self, field, cif_num):
        cif = self.cifs[CIFS.index(cif_num)]
        if not cif['enabled']:
            cif['enabled'] = True

        cif_field = self.__create_cif_field(field)
        cif['fields'].append(cif_field)

    def __set_header_field(self, field, value=None, getter=None, setter=None):
        name = cpptypes.name_to_identifier(field.name)
        if getter is None:
            getter = 'get' + name
        if setter is None:
            setter = 'set' + name
        if value is None:
            value = cpptypes.literal(field.value, field.type)
        self.header['fields'].append({
            'name': name,
            'title': field.name,
            'getter': getter,
            'setter': setter,
            'value': value,
        })

    def __set_cam_field(self, field, value=None, member=None):
        assert self.cam is not None
        getter, setter = get_accessors(field)
        cam_field = {
            'name': cpptypes.name_to_identifier(field.name),
            'getter': getter,
            'setter': setter,
            'src': {
                'getter': getter,
                'setter': setter,
            },
            'title': field.name,
            'member': member 
        }
        if value is not None:
            cam_field['value'] = cpptypes.literal(value, field.type)
        self.cam['fields'].append(cam_field)

    def __create_cam(self):
        cam = {
            'name' : 'ControlAcknowledgeMode',
            'attr': 'cam',
            'type' : PACKING_LIB + 'ControlAcknowledgeMode',
            'struct': True,
            'subfields': [],
        }
        return cam

    def __create_cif_field(self, field, namespaced=True):
        identifier = cpptypes.name_to_identifier(field.name)
        cif_field = {
            'name': field.name,
            'identifier': identifier,
            'attr': field.field.attr,
            'optional': field.is_optional,
            'type': PACKING_LIB + identifier,
            'member': self.__create_member_from_field(field)
        }
        if struct.is_struct(field.type):
            cif_field['struct'] = True
            cif_field['subfields'] = []
            if field.is_user_defined:
                namespace = '::structs::' + self.name.lower() + '::'
            else:
                namespace = PACKING_LIB
            cif_field['type'] = namespace + cpptypes.name_to_identifier(field.type.__name__)
            member_type = namespace + cpptypes.name_to_identifier(field.type.__name__)
            cif_field['member']['datatype'] = member_type
            if cif_field['optional']:
                member_type = optional_type(member_type)
            cif_field['member']['type'] = member_type
            for subfield in field.get_fields():
                if subfield.is_disabled:
                    continue
                if namespaced:
                    subfield_name = field.name + subfield.name
                else:
                    subfield_name = subfield.name
                sub_get, sub_set = get_accessors(subfield, name=subfield_name)
                src_get, src_set = get_accessors(subfield, name=subfield.name)
                subfield_dict = {
                    'name': subfield_name,
                    'getter': sub_get,
                    'setter': sub_set,
                    'src': {
                        'getter': src_get,
                        'setter': src_set,
                    },
                    'title': subfield.name,
                }
                if subfield.is_constant:
                    subfield_dict['value'] = cpptypes.literal(subfield.value, subfield.type)
                if subfield.is_optional:
                    subfield_dict['optional'] = True
                    subfield_dict['check'] = sub_get.replace('get', 'has')
                    getter, setter = get_enabled_accessors(subfield.name)
                    subfield_dict['src']['enable'] = {
                        'setter': setter,
                        'getter': getter,
                    }
                subfield_dict['member'] = self.__create_member_from_field(subfield, name=subfield_name)
                cif_field['subfields'].append(subfield_dict)
        return cif_field

    def __create_member(self, name, datatype, optional=False, value=None):
        identifier = cpptypes.name_to_identifier(name)
        if optional:
            member_type = optional_type(datatype)
        else:
            member_type = datatype
        member = {
            'name': name,
            'identifier': 'm_'+identifier,
            'type': member_type,
            'optional': optional,
            'datatype': datatype,
            'value': None
        }
        if value is not None:
            member['value'] = value
        return member

    def __create_member_from_field(self, field, name=None, optional=None):
        if name is None:
            name = field.name
        if field.value is not None:
            value = cpptypes.literal(field.value, field.type)
        else:
            value = None
        if optional is None:
            optional = field.is_optional
        datatype = cpptypes.value_type(field.type)
        return self.__create_member(name, datatype, optional, value)

    def __add_struct(self, structdef):
        self.structs.append(structdef)

class CppInformationClass:
    def __init__(self, name, config):
        self.name = name
        self.controller_name = name + 'Controller'
        self.controllee_name = name + 'Controllee'
        self.controllee_base_name = name + 'Controllee_base'
        self._packets = []

    def get_packets(self):
        return self._packets

class CppGenerator(Generator):
    """
    Generates C++ packet classes.
    """

    def __init__(self):
        loader = jinja2.PackageLoader(__package__, 'templates')
        self.env = jinja2.Environment(loader=loader,
                                      extensions=['jinja2.ext.do', 'jinja2.ext.loopcontrols'],
                                      **JINJA_OPTIONS)
        self.env.filters['namespace'] = do_namespace
        self.output_dir = os.getcwd()
        self.header_file = None
        self.impl_file = None
        self.packets = []
        self.namespace = ''
        self.information_class = None
        self.controller_file = None
        self.controllee_base_file = None
        self.controllee_file = None

    output_dir = GeneratorOption(
        '--dir',
        doc="output directory for generated files",
        dtype=str
    )

    source_ext = GeneratorOption(
        '--source-ext',
        doc="file extension for source [cpp]",
        dtype=str,
        defval='cpp'
    )

    header_ext = GeneratorOption(
        '--header-ext',
        doc="file extension for headers [hpp]",
        dtype=str,
        defval='hpp'
    )

    def start_file(self, filename):
        basename, _ = os.path.splitext(os.path.basename(filename))
        self.header_file = basename + '.' + self.header_ext
        self.impl_file = basename + '.' + self.source_ext

    def end_file(self):
        os.makedirs(self.output_dir, exist_ok=True)
        context = {
            'packets': self.packets,
            'namespace': self.namespace,
        }
        template = self.env.get_template('header.hpp')
        with open(os.path.join(self.output_dir, self.header_file), 'w') as fp:
            fp.write(template.render(context))

        context['header'] = self.header_file
        template = self.env.get_template('packet.cpp')
        with open(os.path.join(self.output_dir, self.impl_file), 'w') as fp:
            fp.write(template.render(context))

        if self.information_class:
            context['information_class'] = self.information_class
            template = self.env.get_template('controller.hpp')
            self.controller_file = self.information_class.controller_name + '.' + self.header_ext
            with open(os.path.join(self.output_dir, self.controller_file), 'w') as fp:
                fp.write(template.render(context))

            template = self.env.get_template('controllee_base.hpp')
            self.controllee_base_file = self.information_class.controllee_base_name + '.' + self.header_ext
            with open(os.path.join(self.output_dir, self.controllee_base_file), 'w') as fp:
                fp.write(template.render(context))

            template = self.env.get_template('controllee.hpp')
            self.controllee_file = '{}.{}'.format(self.information_class.controllee_name, self.header_ext)
            if not os.path.exists(self.controllee_file):
                with open(os.path.join(self.output_dir, self.controllee_file), 'w') as fp:
                    fp.write(template.render(context))
            else:
                controllee_orig_file = self.controllee_file + '.orig'
                os.rename(self.controllee_file, controllee_orig_file)
                with open(os.path.join(self.output_dir, self.controllee_file), 'w') as fp:
                    fp.write(template.render(context))
                # Check for diffs
                do_patch = False
                result = subprocess.run(['diff', '-u', self.controllee_file, controllee_orig_file],
                                        stdout=subprocess.PIPE)
                if len(result.stdout.decode('utf-8')):
                    msg = 'Changes detected in {}. Would you like it auto-patched? [y/N] '
                    msg = msg.format(self.controllee_file)
                    req_patch = input(msg)
                    if req_patch.lower() == 'y':
                        do_patch = True
                    else:
                        print('Skipping auto-patch. Original file is: {}'.format(controllee_orig_file))
                # Patch
                if do_patch:
                    patchfile = self.controllee_file + '.patch'
                    with open(os.path.join(self.output_dir, patchfile), 'w') as fp:
                        fp.write(result.stdout.decode('utf-8'))
                    result = subprocess.run(['patch', self.controllee_file, patchfile])
                    if result.returncode == 0:
                        keep_orig = input('Keep original file? [y/N] ')
                        if keep_orig.lower() != 'y':
                            os.rename(controllee_orig_file, self.controllee_file)
                        os.remove(patchfile)

    def generate_information_class(self, cppstruct, info_class):
        for packet in info_class.get_packets():
            name = cpptypes.name_to_identifier(packet.name)
            self.generate_packet(name, packet)

    def generate_packet(self, name, packet):
        model = CppPacket(name, packet)
        self.packets.append(model)
        if model.ack is not None:
            self.packets.append(model.ack)

    def generate(self, config):
        name = cpptypes.name_to_identifier(config.name)
        if isinstance(config.config_type, InformationType):
            model = CppInformationClass(name, config)
            self.generate_information_class(model, config)
            self.information_class = model
        elif isinstance(config.config_type, PacketType):
            self.generate_packet(name, config)
        else:
            raise NotImplementedError(config.config_type)
