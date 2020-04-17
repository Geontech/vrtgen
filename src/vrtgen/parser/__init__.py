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

from vrtgen.model.config import create_packet, PacketType

from .packet import create_parser
from .utils import to_kvpair, EMPTY

__all__ = (
    'parse_packet',
    'parse_file',
    'parse_stream'
)

def parse_packet(name, value):
    """
    Parses a VITA 49 packet definition.
    """
    #if not isinstance(value, dict):
    #    raise TypeError('Invalid definition for packet ' + name)
    #if len(value) != 1:
    #    raise ValueError('Must be single-element mapping')
    log = logging.getLogger(name)

    packet_type, packet_value = to_kvpair(value)
    packet_type = PacketType(packet_type.casefold())
    packet = create_packet(packet_type, name)
    parser = create_parser(packet_type)

    if packet_value is not EMPTY:
        parser(log, packet, packet_value)

    return packet

def parse_file(filename):
    """
    Parses VITA 49 packet definitions from a YAML file.
    """
    with open(filename, 'r') as file:
        yield from parse_stream(file)

def parse_stream(stream):
    """
    Parses VITA 49 packet definitions from a file-like object.
    """
    document = yaml.safe_load(stream)

    for name, value in document.items():
        if name.startswith('.'):
            logging.debug("Skipping hidden entry %s", name)
        else:
            logging.info('Processing packet %s', name)
            try:
                packet = parse_packet(name, value)
                packet.validate()
                yield packet
            except (ValueError, TypeError) as exc:
                logging.error("Invalid packet definition '%s': %s", name, str(exc))
            except RuntimeError as exc:
                logging.exception("%s %s", name, str(exc))
