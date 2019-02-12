#!/usr/bin/python3

import logging
import sys
import yaml

logging.basicConfig(format='%(levelname)s: %(message)s')
logging.getLogger().setLevel(logging.DEBUG)

from v49gen.packets import *

class Parser(object):
    def __init__(self):
        self._constructor = yaml.constructor.SafeConstructor()

    def parse_namespace(self, node):
        namespace = str(node.value)
        logging.debug('Using namespace %s', namespace)
        self.namespace = namespace

    def parse_field(self, field, node):
        if isinstance(node, yaml.ScalarNode):
            if node.value == 'required':
                field.set_required()
            elif node.value == 'optional':
                field.set_optional()
            elif node.value == 'disabled':
                field.set_disabled()
        else:
            logging.warning("Invalid value for field '%s' (line %d, column %d)",
                            field.name, node.start_mark.line, node.start_mark.column)

    def parse_trailer(self, packet, node):
        if not isinstance(node, yaml.MappingNode):
            logging.warning('Invalid trailer definition in packet (line %d, column %d)' % (node.start_mark.line, node.start_mark.column))
            return None

        for field_name, field_value in node.value:
            field_name = str(field_name.value)
            try:
                field = packet.get_field(field_name)
            except KeyError as exc:
                logging.warning('Invalid trailer field %s', exc)
                continue

            logging.debug("Parsing field '%s'", field.name)
            if isinstance(field_value, yaml.ScalarNode):
                try:
                    field.default_value = self._constructor.construct_yaml_bool(field_value)
                except:
                    self._set_field_attribute(field, str(field_value.value))
            elif isinstance(field_value, yaml.MappingNode):
                mapping = self._constructor.construct_mapping(field_value)
                for key, value in mapping.items():
                    if key == 'default':
                        field.default_value = value
                    elif key == 'attributes':
                        for attribute in value:
                            self._set_field_attribute(field, attribute)
            else:
                logging.warning('Invalid value for trailer field "%s" (line %d, column %d)',
                                field_name, field_value.start_mark.line, field_value.start_mark.column)
                continue

    def _set_field_attribute(self, field, value):
        if value == 'optional':
            field.set_optional()
        elif value == 'required':
            field.set_required()
        elif value == 'disabled':
            field.set_disabled()
        else:
            logger.warning("Invalid field attribute '%s'", value)

    def parse_packet(self, packet, node):
        logging.info('Processing packet %s', self.get_packet_name(packet.name))
        if not isinstance(node, yaml.MappingNode):
            err = 'Invalid packet definition %s (line %d, column %d)' % (name, node.start_mark.line, node.start_mark.column)
            raise RuntimeError(err)

        for key_node, value_node in node.value:
            field_name = key_node.value
            if field_name.lower() == 'trailer':
                if not isinstance(packet, VRTDataPacket):
                    logging.warning('Trailer only valid for data packets')
                    continue
                self.parse_trailer(packet, value_node)
            else:
                try:
                    field = packet.get_field(field_name)
                except KeyError as exc:
                    logging.warning('Invalid field %s', exc)
                    continue
                logging.debug("Parsing field '%s'", field.name)
                self.parse_field(field, value_node)

    def get_packet_name(self, name):
        if self.namespace:
            return '::'.join((self.namespace, name))
        else:
            return name

    def parse_packets(self, PacketClass, node):
        if not isinstance(node, yaml.MappingNode):
            logging.warning('Invalid packet section (line %d column %d)',
                            node.start_mark.line, node.start_mark.column)

        for key_node, value_node in node.value:
            packet = PacketClass(key_node.value)
            self.parse_packet(packet, value_node)
            yield packet

    def parse(self, filename):
        with open(filename, 'r') as fp:
            root = yaml.compose(fp)
        if not isinstance(root, yaml.MappingNode):
            raise RuntimeError('invalid YAML file')

        self.namespace = None
        for key_node, value_node in root.value:
            name = key_node.value
            if name == 'namespace':
                self.parse_namespace(value_node)
            elif name == 'data':
                yield from self.parse_packets(VRTDataPacket, value_node)
            elif name == 'context':
                yield from self.parse_packets(VRTContextPacket, value_node)
            elif name == 'control':
                yield from self.parse_packets(VRTControlPacket, value_node)
            else:
                logging.warning("Invalid section '%s'", key_node.value)

def hex_bytes(data):
    return ''.join('{:02x}'.format(ch) for ch in data)

if __name__ == '__main__':
    source = sys.argv[1]
    p = Parser()
    for packet in p.parse(source):
        header = packet.get_header_bytes()
        logging.debug('Packet header  0x%s', hex_bytes(header))
        if packet.has_trailer:
            logging.debug('Packet trailer 0x%s', hex_bytes(packet.trailer.get_bytes()))
