from .packets import *

import collections
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

class FieldParser:
    __slots__ = ('name', 'func')

    def __init__(self, name, func):
        self.name = name
        self.func = func

    def parse_value(self, node):
        return self.func(node)

FieldValue = collections.namedtuple('FieldValue', ['value', 'optional'])

class PacketParser:
    def __init__(self, name):
        self.name = name
        self.log = logging.getLogger(name)
        self.type = None
        self.fields = {}
        self.field_parsers = []
        self.constructor = yaml.constructor.SafeConstructor()

        for field in VRTPrologue().fields:
            if field.name in ('TSI', 'TSF'):
                continue
            self.add_field_parser(field.name, field.format)
        self.add_field_parser('TSI', func=self.parse_tsi)
        self.add_field_parser('TSF', func=self.parse_tsf)

        for field in VRTDataTrailer().fields:
            self.add_field_parser(field.name, field.format)

        for field in CIF0().fields:
            self.add_field_parser(field.name, field.format)

    def parse_tsi(self, node):
        value = self.constructor.construct_yaml_str(node)
        if value == 'none':
            return TSI.NONE
        elif value == 'utc':
            return TSI.UTC
        elif value == 'gps':
            return TSI.GPS
        elif value == 'other':
            return TSI.OTHER
        else:
            raise KeyError(value)

    def parse_tsf(self, node):
        value = self.constructor.construct_yaml_str(node)
        if value == 'none':
            return TSF.NONE
        elif value == 'samples':
            return TSF.SAMPLE_COUNT
        elif value == 'picoseconds':
            return TSF.REAL_TIME
        elif value == 'free running':
            return TSF.FREE_RUNNING
        else:
            raise KeyError(value)

    def parse_int(self, node):
        return self.constructor.construct_yaml_int(node)

    def parse_float(self, node):
        return self.constructor.construct_yaml_float(node)

    def parse_bit(self, node):
        try:
            self.constructor.construct_yaml_bool(node)
        except:
            return {'0':False, '1':True}[node.value]

    def parse_empty(self, node):
        raise ValueError('cannot set value')

    def add_field_parser(self, name, format=None, func=None):
        if not func:
            if format == BIT:
                func = self.parse_bit
            elif isinstance(format, FixedFormat):
                # TODO: bits/range
                func = self.parse_float
            elif isinstance(format, IntFormat):
                # TODO: bit widths
                func = self.parse_int
            else:
                func = self.parse_empty
        self.field_parsers.append(FieldParser(name, func))

    def get_field_parser(self, name):
        for parser in self.field_parsers:
            if name.lower() == parser.name.lower():
                return parser
        return None

    def warning(self, *args, **kwargs):
        self.log.warning(*args, **kwargs)

    def error(self, *args, **kwargs):
        self.log.error(*args, **kwargs)

    def parse_field(self, name, value_node=None, optional=False):
        if name in self.fields:
            self.warning("Duplicate field '%s'", name)
            return
        field = self.get_field_parser(name)
        if field is None:
            self.error("Invalid field '%s'", name)
            return
        if value_node is not None:
            try:
                value = field.parse_value(value_node)
            except (KeyError, ValueError) as exc:
                self.error("Invalid value for field '%s': '%s'", field.name, value_node.value)
                return
            self.log.debug('Field %s has value %s', field.name, value)
        else:
            value = None
        self.log.debug('Field %s is %s', field.name, 'optional' if optional else 'required')
        self.fields[field.name] = FieldValue(value, optional)

    def parse_packet_type(self, node):
        packet_type = self.constructor.construct_yaml_str(node)
        if packet_type.lower() == 'data':
            self.type = VRTDataPacket
        elif packet_type.lower() == 'context':
            self.type = VRTContextPacket
        elif packet_type.lower() == 'control':
            self.type = VRTControlPacket
        else:
            raise ParserError("Invalid packet type '"+packet_type+"'")

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

    def parse_fields(self, node, optional=False):
        if not isinstance(node, yaml.SequenceNode):
            self.error('Packet fields must be sequence', node)
            return

        for field in node.value:
            try:
                field_name, value_node = self.parse_field_list_entry(field)
            except ParserError as exc:
                self.error('%s', exc)
                continue
            self.parse_field(field_name, value_node, optional)

    def parse(self, node):
        for key_node, value_node in MergingIterator(node.value):
            field_name = key_node.value
            if field_name.lower() in ('required', 'optional'):
                # Within an attribute scope, set the attribute on all fields
                optional = (field_name.lower() == 'optional')
                self.parse_fields(value_node, optional)
            elif field_name.lower() == 'type':
                try:
                    self.parse_packet_type(value_node)
                except ParserError as exc:
                    self.error('%s', exc)
            else:
                field = self.parse_field(field_name, value_node)
                if field is None:
                    continue

        if self.type is None:
            raise ParserError('No packet type provided', node)
        packet = self.type(self.name)
        for field_name, field_value in self.fields.items():
            try:
                field = packet.add_field(field_name, field_value.optional)
            except KeyError as exc:
                self.error("'%s' is not a valid field for packet type", field_name)
            if field_value.value is not None:
                field.set_value(field_value.value)
        return packet

class FileParser(object):
    def __init__(self):
        self._constructor = yaml.constructor.SafeConstructor()

    def parse_namespace(self, node):
        namespace = str(node.value)
        logging.debug('Using namespace %s', namespace)
        self.namespace = namespace

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

        return PacketParser(name).parse(node)

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
