from .packets import *

import logging

import yaml

class Parser(object):
    def __init__(self):
        self._constructor = yaml.constructor.SafeConstructor()

    def parse_namespace(self, node):
        namespace = str(node.value)
        logging.debug('Using namespace %s', namespace)
        self.namespace = namespace

    def parse_field(self, field, node):
        if isinstance(node, yaml.ScalarNode):
            value = self._constructor.construct_object(node)
            if value in ('required', 'optional', 'disabled'):
                self._set_field_attribute(field, value)
            else:
                logging.debug("Setting value for field '%s': %s", field.name, value)
                try:
                    field.set_value(value)
                except (ValueError, TypeError) as exc:
                    logging.error("Could not set value for field '%s': %s", field.name, exc)
                # If a field is only given a value, assume it's required
                # TODO: Should be constant as well?
                field.set_required()
        elif isinstance(node, yaml.MappingNode):
            for key_node, value_node in node.value:
                key = key_node.value.lower()
                if key == 'value':
                    value = self._constructor.construct_object(value_node)
                    logging.debug("Setting value '%s' for field '%s'", value, field.name)
                    try:
                        field.set_value(value)
                    except Exception as exc:
                        logging.error("Could not set value for field '%s': %s", field.name, exc)
                elif key == 'attributes':
                    for attr in self._constructor.construct_sequence(value_node):
                        self._set_field_attribute(field, attr)
                else:
                    logging.warning("Invalid key '%s'", key)
        else:
            logging.error("Invalid value for field '%s'", field.name)

    def _set_field_attribute(self, field, value):
        if value == 'optional':
            logging.debug("Field '%s' is optional", field.name)
            field.set_optional()
        elif value == 'required':
            logging.debug("Field '%s' is required", field.name)
            field.set_required()
        elif value == 'disabled':
            logging.debug("Field '%s' is disabled", field.name)
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
