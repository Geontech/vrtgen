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

import yaml, logging

from vrtgen.parser.model.command import AcknowledgePacket
from vrtgen.parser.model.information import InformationClass

__all__ = (
    'parse_file',
    'parse_stream'
)

def parse_file(filename, loader):
    """
    Parses VITA 49 class definitions from a YAML file.
    """
    with open(filename, 'r') as file:
        yield from parse_stream(file, loader)

def parse_stream(stream, loader):
    """
    Parses VITA 49 class definitions from a file-like object.
    """
    parsed = yaml.load(stream, Loader=loader)
    parsed_packets = {}
    parsed_files = []
    has_information_class = False
    for name, value in parsed.items():
        if isinstance(value, InformationClass):
            has_information_class = True
            break
    for name, value in parsed.items():
        if name.startswith('.'):
            logging.debug("Skipping hidden entry %s", name)
        elif name == 'include':
            if not isinstance(value, list):
                raise TypeError('include must be a list')
            for filename in value:
                parsed_files.append(filename)
                for inc_name, inc_value in parse_file(filename, loader):
                    parsed_packets[inc_name] = inc_value
        elif isinstance(value, InformationClass):
            class_names = value.packet_classes.values
            for pkt_name in class_names:
                packet = parsed_packets.get(pkt_name, None)
                if packet:
                    packet.class_id.information_code.value = value.code
                    yield pkt_name,packet
                else:
                    raise ValueError('Unknown packet class:', pkt_name)
            yield name,value
        else:
            logging.info('Processing YAML class definition: %s', name)
            parsed_packets[name] = value
            if isinstance(value, AcknowledgePacket):
                ctrl_packet = parsed_packets.get(value.responds_to, None)
                if not ctrl_packet:
                    raise ValueError('Unknown control packet provided in responds_to for', name)
                value.update_from_control(ctrl_packet)
            if not has_information_class:
                yield name,value
    yield 'files',parsed_files
