# Copyright (C) 2021 Geon Technologies, LLC
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

import os, shutil, jinja2
from vrtgen.backend.generator import Generator, GeneratorOption
from vrtgen.parser.model.information import InformationClass
from vrtgen.parser.model.types import *
from vrtgen.parser.model.base import Packet
from vrtgen.parser.model.command import AcknowledgePacket
from vrtgen.parser.loader import get_loader
from .type_helpers import TypeHelper, format_enum
from .jinja import *
import copy

def name_to_identifier(name):
    """
    Returns a valid C++ identifier for a string name.
    """
    def map_char(value):
        if value.isalnum():
            return value
        if value in '.' or value in '-':
            return '_'
        return ''
    return ''.join(map_char(ch) for ch in name)

def name_to_snake(name):
    """
    Returns a snake case string name.
    """
    def map_char(value):
        if value.isalnum():
            if value.isupper():
                return '_' + value.lower()
            return value
        if value in '.':
            return '_'
        return ''
    return str(name[0].lower()) + ''.join(map_char(ch) for ch in name[1:])

class CppPacket:
    def __init__(self, name, packet):
        self.name = name
        self.helper = name + 'Helper'
        self.namespace = ''
        self.config = packet

    @property
    def is_data(self):
        return self.header.packet_type.value.is_data

    @property
    def is_context(self):
        return self.header.packet_type.value.is_context

    @property
    def is_command(self):
        return self.header.packet_type.value.is_command

    @property
    def is_control(self):
        try:
            return self.config.is_control
        except:
            return False

    @property
    def is_ack(self):
        try:
            return self.config.is_ack
        except:
            return False

    @property
    def requires_cif_functions(self):
        try:
            if self.is_context:
                return True
            elif self.is_control and (self.cam.action_mode.value == enums.ActionMode.EXECUTE or
                                      self.cam.action_mode.value == enums.ActionMode.DRY_RUN):
                return True
            elif (self.is_ack and self.config.is_ack_s):
                return True
        except:
            pass
        return False

    @property
    def requires_cif_enable_functions(self):
        try:
            if self.is_control and (self.cam.action_mode.value == enums.ActionMode.NO_ACTION):
                return True
        except:
            pass
        return False

    @property
    def header(self):
        return self.config.header

    @property
    def stream_id(self):
        return self.config.stream_id

    @property
    def class_id(self):
        return self.config.class_id
    
    @property
    def timestamp(self):
        return self.config.timestamp

    @property
    def cam(self):
        try:
            return self.config.cam
        except:
            return None
    
    @property
    def message_id(self):
        try:
            return self.config.message_id
        except:
            return None

    @property
    def controllee_id(self):
        try:
            return self.config.controllee_id
        except:
            return None

    @property
    def controller_id(self):
        try:
            return self.config.controller_id
        except:
            return None

    @property
    def cif0(self):
        try:
            return self.config.cif_0
        except:
            return None

    @property
    def cif1(self):
        try:
            return self.config.cif_1
        except:
            return None

    @property
    def cif2(self):
        try:
            return self.config.cif_2
        except:
            return None

    @property
    def cif7(self):
        try:
            return self.config.cif_7
        except:
            return None

    @property
    def trailer(self):
        try:
            return self.config.trailer
        except:
            return None

    @property
    def last_prologue_field(self):
        try:
            if self.controller_id and self.controller_id.enabled:
                return self.controller_id
            elif self.controllee_id and self.controllee_id.enabled:
                return self.controllee_id
            elif self.message_id is not None:
                return self.message_id
            elif self.cam is not None:
                return self.cam
            elif self.timestamp.fractional.enabled:
                return self.timestamp.fractional
            elif self.timestamp.integer.enabled:
                return self.timestamp.integer
            elif self.class_id.enabled:
                return self.class_id
            elif self.stream_id.enabled:
                return self.stream_id
            else:
                return self.header
        except:
            return self.header

    @property
    def warnings_enabled(self):
        try:
            return (self.config.is_ack_v or self.config.is_ack_x) and self.config.warnings_enabled
        except:
            return False

    @property
    def wif0(self):
        try:
            return self.config.wif_0
        except:
            return None

    @property
    def wif1(self):
        try:
            return self.config.wif_1
        except:
            return None

    @property
    def wif2(self):
        try:
            return self.config.wif_2
        except:
            return None

    @property
    def errors_enabled(self):
        try:
            return (self.config.is_ack_v or self.config.is_ack_x) and self.config.errors_enabled
        except:
            return False

    @property
    def eif0(self):
        try:
            return self.config.eif_0
        except:
            return None

    @property
    def eif1(self):
        try:
            return self.config.eif_1
        except:
            return None

    @property
    def eif2(self):
        try:
            return self.config.eif_2
        except:
            return None

    @property
    def structs(self):
        return_value = []
        if self.requires_cif_functions:
            if self.cif1 and self.cif1.enabled:#(self.is_context or self.is_control or (self.is_ack and self.config.is_ack_s)):
                if self.cif1.sector_step_scan.enabled:
                    sector_step_scan = self.cif1.sector_step_scan.type_
                    sector_step_scan.packet_name = name_to_snake(self.name)
                    record = sector_step_scan.records.type_
                    record.packet_name = name_to_snake(self.name)
                    for field in sector_step_scan.subfield_cif.fields:
                        if field.enabled:
                            record.__dict__[field.name].enabled = True
                            record.__dict__[field.name].required = True
                    return_value.append(record)
                if self.cif1.discrete_io_32.enabled and len(self.cif1.discrete_io_32.type_.subfields) > 0:
                    self.cif1.discrete_io_32.type_.packet_name = name_to_snake(self.name)
                    return_value.append(self.cif1.discrete_io_32.type_)
                if self.cif1.discrete_io_64.enabled and len(self.cif1.discrete_io_64.type_.subfields) > 0:
                    self.cif1.discrete_io_64.type_.packet_name = name_to_snake(self.name)
                    return_value.append(self.cif1.discrete_io_64.type_)
            if self.cif7 and self.cif7.enabled:
                self.cif7.attributes.packet_name = name_to_snake(self.name)
                return_value.append(self.cif7.attributes)
        if self.trailer and self.trailer.enabled and self.trailer.is_user_defined:
            self.trailer.packet_name = name_to_snake(self.name)
            return_value.append(self.trailer)
        if self.stream_id and self.stream_id.user_defined:
            self.stream_id.packet_name = name_to_snake(self.name)
            return_value.append(self.stream_id)
        return return_value

    @property
    def enums(self):
        retval = []
        if self.cif1 and self.cif1.enabled and (self.is_context or self.is_control or self.is_query):
            if self.cif1.discrete_io_32.enabled:
                for field in self.cif1.discrete_io_32.type_.fields:
                    if isinstance(field, EnumType):
                        field.packet_name = name_to_snake(self.name)
                        retval.append(format_enum(field.type_))
            if self.cif1.discrete_io_64.enabled:
                for field in self.cif1.discrete_io_64.type_.fields:
                    if isinstance(field, EnumType):
                        field.packet_name = name_to_snake(self.name)
                        retval.append(format_enum(field.type_))
        if self.trailer and len(self.trailer.enums) > 0:
            for enum in self.trailer.enums:
                enum.packet_name = name_to_snake(self.name)
                retval.append(format_enum(enum.type_))
        return retval

class CppGenerator(Generator):
    """
    Generates C++ packet classes.
    """

    def __init__(self):
        loader = jinja2.PackageLoader(__package__, 'templates')
        self.env = jinja2.Environment(loader=loader, **JINJA_OPTIONS,
                                      extensions=['jinja2.ext.loopcontrols', 'jinja2.ext.do'])
        self.env.filters.update({
            'sort_packed_tags' : sort_packed_tags,
            'format_docstring' : format_docstring,
            'to_snake' : name_to_snake
        })
        self.filename = None
        self.basename = None
        self.output_dir = os.getcwd()
        self.header = None
        self.implfile = None
        self.packets = []
        self.loader = get_loader()
        self.type_helper = TypeHelper()
        self.information_class = None
        self.controller_file = None
        self.controllee_base_file = None
        self.controllee_file = None
        self.project = False
        self.yamls = []

    output_dir = GeneratorOption(
        '--dir',
        doc="output directory for generated files",
        dtype=str
    )

    source_ext = GeneratorOption(
        '--source-ext',
        doc="file extension for source [.cpp]",
        dtype=str,
        defval='cpp'
    )

    header_ext = GeneratorOption(
        '--header-ext',
        doc="file extension for headers [.hpp]",
        dtype=str,
        defval='hpp'
    )

    namespace_ = GeneratorOption(
        '--namespace',
        doc='namespace for generated classes',
        dtype=str,
        defval=''
    )

    cmd_socket = GeneratorOption(
        '--cmd-socket',
        doc='socket type for V49.2 control [tcp]',
        dtype=str,
        defval='tcp',
        choices=['tcp', 'udp', 'nats']
    )

    def get_loader(self):
        return self.loader

    def start_file(self, filename):
        self.filename = filename
        self.basename, _ = os.path.splitext(os.path.basename(filename))
        self.header_name = name_to_identifier(self.basename)
        self.header_file = self.header_name + '.' + self.header_ext
        self.implfile = name_to_identifier(self.basename) + '.' + self.source_ext

    def end_file(self):
        os.makedirs(self.output_dir, exist_ok=True)
        main_context = {
            'packets': self.packets,
            'type_helper': self.type_helper,
            'namespace_': self.namespace_,
            'project_name': self.output_dir,
            'project_version': '0.1.0',
            'cmd_socket': self.cmd_socket
        }

        self.include_dir = self.output_dir
        self.src_dir = self.output_dir
        self.yaml_dir = f'{self.output_dir}/yaml'
        if self.project:
            self.include_dir = f'{self.output_dir}/include'
            self.src_dir = f'{self.output_dir}/src'
            os.makedirs(self.include_dir, exist_ok=True)
            os.makedirs(f'{self.include_dir}/packets', exist_ok=True)
            os.makedirs(self.src_dir, exist_ok=True)
            os.makedirs(f'{self.src_dir}/bin', exist_ok=True)
            os.makedirs(f'{self.src_dir}/packets', exist_ok=True)
            os.makedirs(self.yaml_dir, exist_ok=True)

            self.yamls.append(self.filename)
            for yaml in self.yamls:
                shutil.copy2(yaml, self.yaml_dir)

            template = self.env.get_template('main.cpp.jinja2')
            with open(os.path.join(f'{self.src_dir}/bin', 'main.cpp'), 'w') as fp:
                fp.write(template.render(main_context))

            template = self.env.get_template('CMakeLists.txt.jinja2')
            with open(os.path.join(self.output_dir, 'CMakeLists.txt'), 'w') as fp:
                fp.write(template.render(main_context))

            template = self.env.get_template('CMakeLists.src.jinja2')
            with open(os.path.join(self.src_dir, 'CMakeLists.txt'), 'w') as fp:
                fp.write(template.render(main_context))

            template = self.env.get_template('CMakeLists.src.bin.jinja2')
            with open(os.path.join(f'{self.src_dir}/bin', 'CMakeLists.txt'), 'w') as fp:
                fp.write(template.render(main_context))

            template = self.env.get_template('CMakeLists.packets.jinja2')
            with open(os.path.join(f'{self.src_dir}/packets', 'CMakeLists.txt'), 'w') as fp:
                fp.write(template.render(main_context))

        for packet in self.packets:
            context = {
                'packets': [packet],
                'type_helper': self.type_helper,
                'namespace_' : self.namespace_
            }
            header_file = name_to_snake(packet.name) + '.' + self.header_ext
            implfile = name_to_snake(packet.name) + '.' + self.source_ext
            context['header'] = header_file
            context['header_name'] = self.header_name + '_' + name_to_snake(packet.name).upper()

            template = self.env.get_template('packet.hpp.jinja2')
            file_path = self.include_dir
            if self.project:
                file_path += '/packets'
            with open(os.path.join(file_path, header_file), 'w') as fp:
                fp.write(template.render(context))

            template = self.env.get_template('packet.cpp.jinja2')
            file_path = self.src_dir
            if self.project:
                file_path += '/packets'
            with open(os.path.join(file_path, implfile), 'w') as fp:
                fp.write(template.render(context))

        if self.information_class:
            context['packets'] = self.packets
            context['namespace_'] = self.namespace_
            context['information_class'] = self.information_class[1]
            context['controller_name'] = 'Controller' # self.information_class[0] + 'Controller'
            context['cmd_socket'] = self.cmd_socket
            template = self.env.get_template('controller.hpp.jinja2')
            self.controller_file = '{}.{}'.format(context['controller_name'], self.header_ext)
            with open(os.path.join(self.include_dir, self.controller_file), 'w') as fp:
                fp.write(template.render(context))

            context['controllee_base_name'] = 'Controllee_base' # self.information_class[0] + 'Controllee_base'
            template = self.env.get_template('controllee_base.hpp.jinja2')
            self.controllee_base_file = '{}.{}'.format(context['controllee_base_name'], self.header_ext)
            with open(os.path.join(self.include_dir, self.controllee_base_file), 'w') as fp:
                fp.write(template.render(context))

            context['controllee_name'] = 'Controllee' # self.information_class[0] + 'Controllee'
            template = self.env.get_template('controllee.hpp.jinja2')
            self.controllee_file = '{}.{}'.format(context['controllee_name'], self.header_ext)
            with open(os.path.join(self.include_dir, self.controllee_file), 'w') as fp:
                fp.write(template.render(context))


    def generate_packet(self, name, packet):
        model = CppPacket(name, packet)
        self.packets.append(model)

    def generate(self, name, config):
        name = name_to_identifier(name)
        if isinstance(config, Packet):
            self.generate_packet(name, config)
        elif isinstance(config, InformationClass):
            self.information_class = (name, config)
        else:
            raise NotImplementedError(type(config))
