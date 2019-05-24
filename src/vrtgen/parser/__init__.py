"""
Main package for VITA 49 packet configuration parsing.
"""
import logging

import yaml

from .packet import DataPacketParser, ContextPacketParser, CommandPacketParser

__all__ = (
    'parse_packet',
    'parse_file',
    'parse_stream'
)

def parse_packet(name, value):
    """
    Parses a VITA 49 packet definition.
    """
    if not isinstance(value, dict):
        raise RuntimeError('Invalid definition for packet ' + name)

    packet_type = value.pop('type', None)
    if packet_type is None:
        raise RuntimeError('No packet type specified for ' + name)
    if packet_type == 'data':
        parser = DataPacketParser(name)
    elif packet_type == 'context':
        parser = ContextPacketParser(name)
    elif packet_type == 'command':
        parser = CommandPacketParser(name)
    else:
        raise RuntimeError("Invalid type '{0}' for packet '{1}'".format(packet_type, name))

    return parser.parse(value)

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
            except RuntimeError as exc:
                logging.exception("%s %s", name, str(exc))
