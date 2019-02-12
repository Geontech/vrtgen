from .packets import *

import logging

import yaml

class MergingIterator:
    def __init__(self, nodes):
        self._iter = iter(nodes)
        self._stack = []

    def __iter__(self):
        return self

    def __next__(self):
        try:
            item = next(self._iter)
        except StopIteration:
            if not self._stack:
                raise
            self._iter = self._stack.pop()
            item = next(self._iter)
        if item[0].tag == 'tag:yaml.org,2002:merge':
            self._stack.append(self._iter)
            self._iter = iter(item[1].value)
            return next(self._iter)
        return item

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

    def parse_field_value(self, field, node):
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

    def parse_field_list_entry(self, node):
        if isinstance(node, yaml.ScalarNode):
            return node.value, None
        elif isinstance(node, yaml.MappingNode):
            if len(node.value) != 1:
                raise ParserError('Multiple field keys', node)
            key_node, value_node = node.value[0]
            return key_node.value, value_node
        else:
            raise ParserError('Invalid field description', node)

    def parse_fields(self, packet, node, optional):
        if not isinstance(node, yaml.SequenceNode):
            raise ParserError('Packet fields must be sequence', node)

        for field in node.value:
            try:
                name, value_node = self.parse_field_list_entry(field)
            except ParserError as exc:
                logging.error("%s (line %d column %d)", exc, exc.line, exc.column)
                continue
            try:
                field = packet.add_field(name, optional)
            except KeyError as exc:
                logging.error("Invalid field %s", exc)
                continue
            except ValueError as exc:
                logging.error("Duplicate field '%s'", name)
                continue
            logging.debug("Parsing field '%s'", name)
            if value_node is not None:
                self.parse_field_value(field, value_node)

    def get_packet_name(self, name):
        if self.namespace:
            return '::'.join((self.namespace, name))
        else:
            return name

    def _create_packet(self, name, node):
        key, value = node
        if key.value.lower() != 'type':
            raise ParserError('Packet type must follow packet declaration', node)
        packet_type = value.value.lower()
        if packet_type == 'data':
            return VRTDataPacket(name)
        elif packet_type == 'context':
            return VRTContextPacket(name)
        elif packet_type == 'control':
            return VRTControlPacket(name)
        else:
            raise ParserError("Invalid packet type '"+value.value+"'", node)

    def parse_packet(self, name, node):
        logging.info('Processing packet %s', self.get_packet_name(name))
        if not isinstance(node, yaml.MappingNode):
            raise ParserError('Invalid packet definition', node)

        map_iter = MergingIterator(node.value)
        packet = self._create_packet(name, next(map_iter))

        for key_node, value_node in map_iter:
            field_name = key_node.value
            if field_name.lower() in ('required', 'optional'):
                # Within an attribute scope, set the attribute on all fields
                optional = field_name.lower() == 'optional'
                self.parse_fields(packet, value_node, optional)
            else:
                try:
                    field = packet.add_field(field_name)
                except KeyError as exc:
                    logging.warning('Invalid field %s', exc)
                    continue
                logging.debug("Parsing field '%s'", field.name)
                self.parse_field_value(field, value_node)
                # If a field is only given a value, its value is constant
                field.set_constant()

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
            elif name.startswith('.'):
                logging.debug("Skipping hidden entry %s", name)
            else:
                try:
                    yield self.parse_packet(name, value_node)
                except ParserError as exc:
                    logging.error("%s (line %d column %d)", str(exc), exc.line, exc.column)
