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

"""
Main package for VITA 49 packet configuration parsing.
"""
import logging

import yaml

from vrtgen.model.config import create_configuration
from vrtgen.model.config import InformationClassConfiguration
from vrtgen.model.config import InformationType, PacketType

from vrtgen.parser import packet
from vrtgen.parser import informationclass
from vrtgen.parser.utils import to_kvpair, EMPTY

__all__ = (
    'parse_configuration',
    'parse_file',
    'parse_stream'
)

def _create_parser(config_type):
    if isinstance(config_type, PacketType):
        return packet.create_parser(config_type)
    if isinstance(config_type, InformationType):
        return informationclass.create_parser()
    raise ValueError('invalid class type {}'.format(config_type))

def parse_configuration(name, value, include_files=None):
    """
    Parses a VITA 49 class definition.
    """
    log = logging.getLogger(name)
    config_type, config_value = to_kvpair(value)
    try:
        config_type = PacketType(config_type.casefold())
    except ValueError:
        try:
            config_type = InformationType(config_type.casefold())
        except ValueError as err:
            raise err
    config = create_configuration(config_type, name)
    parser = _create_parser(config_type)

    if config_value is not EMPTY:
        parser(log, config, config_value)

    if isinstance(config, InformationClassConfiguration) and include_files:
        for include_file in include_files:
            info_class_packets = config.get_packet_classes()
            packets = parse_file(include_file)
            for pkt in packets:
                if pkt.name in info_class_packets:
                    config.add_packet(pkt)

    return config

def parse_file(filename):
    """
    Parses VITA 49 class definitions from a YAML file.
    """
    with open(filename, 'r') as file:
        yield from parse_stream(file)

def parse_stream(stream):
    """
    Parses VITA 49 class definitions from a file-like object.
    """
    document = yaml.safe_load(stream)

    include_files = []
    for name, value in document.items():
        if name.startswith('.'):
            logging.debug("Skipping hidden entry %s", name)
        elif name == 'include':
            if not isinstance(value, list):
                raise TypeError('includes must be a list')
            include_files = value
        else:
            logging.info('Processing YAML class definition: %s', name)
            try:
                configuration = parse_configuration(name, value, include_files)
                configuration.validate()
                yield configuration
            except (ValueError, TypeError) as exc:
                logging.error("Invalid packet definition '%s': %s", name, str(exc))
            except RuntimeError as exc:
                logging.exception("%s %s", name, str(exc))
