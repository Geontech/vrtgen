from .packets import *

import logging

import yaml

class ParserError(RuntimeError):
    def __init__(self, message, node):
        super().__init__(message)
        self.node = node

    @property
    def line(self):
        return self.node.start_mark.line

    @property
    def column(self):
        return self.node.start_mark.column

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
            logging.debug("Setting value for field '%s': %s", field.name, value)
            try:
                field.set_value(value)
            except (ValueError, TypeError) as exc:
                logging.error("Could not set value for field '%s': %s", field.name, exc)
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

    def parse_field_entry(self, node):
        if isinstance(node, yaml.ScalarNode):
            return node.value, None
        elif isinstance(node, yaml.MappingNode):
            if len(node.value) != 1:
                raise ValueError('Multiple field keys')
            key_node, value_node = node.value[0]
            return key_node.value, self._constructor.construct_object(value_node)
        else:
            raise ValueError('Invalid field description')

    def parse_fields(self, packet, node):
        if not isinstance(node, yaml.SequenceNode):
            raise ParserError('Packet fields must be sequence', node)

        for field in node.value:
            try:
                name, value = self.parse_field_entry(field)
            except ValueError as exc:
                logging.error("Invalid field entry (line %d column %d)", node.start_mark.line, node.start_mark.column)
                continue
            logging.debug("Parsing field '%s'", name)
            try:
                field = packet.get_field(name)
            except KeyError as exc:
                logging.error("Invalid field %s", exc)
                continue
            if value is not None:
                field.set_value(value)
            yield field

    def get_packet_name(self, name):
        if self.namespace:
            return '::'.join((self.namespace, name))
        else:
            return name

    def parse_packet(self, name, node):
        logging.info('Processing packet %s', self.get_packet_name(name))
        if not isinstance(node, yaml.MappingNode):
            raise ParserError('Invalid packet definition', node)
        key, value = node.value[0]
        if key.value.lower() != 'type':
            raise ParserError('Packet type must follow packet declaration', node)
        packet_type = value.value.lower()
        if packet_type == 'data':
            packet = VRTDataPacket(name)
        elif packet_type == 'context':
            packet = VRTContextPacket(name)
        elif packet_type == 'control':
            packet = VRTControlPacket(name)
        else:
            raise ParserError("Invalid packet type '"+value.value+"'", node)

        for key_node, value_node in node.value[1:]:
            field_name = key_node.value
            if field_name.lower() in ('required', 'optional', 'disabled'):
                # Within an attribute scope, set the attribute on all fields
                attribute = field_name.lower()
                for field in self.parse_fields(packet, value_node):
                    self._set_field_attribute(field, attribute)
            else:
                try:
                    field = packet.get_field(field_name)
                except KeyError as exc:
                    logging.warning('Invalid field %s', exc)
                    continue
                logging.debug("Parsing field '%s'", field.name)
                self.parse_field(field, value_node)
                # If a field is only given a value, assume it's required
                # TODO: Should be constant as well?
                field.set_required()

        return packet

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
            else:
                try:
                    yield self.parse_packet(name, value_node)
                except ParserError as exc:
                    logging.error("%s (line %d column %d)", str(exc), exc.line, exc.column)
