from .packets import *

import collections
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
            if field.name in ('TSI', 'TSF', 'Class ID'):
                continue
            self.add_field_parser(field.name, field.format)
        self.add_field_parser('Class ID', func=self.parse_class_id)
        self.add_field_parser('TSI', func=self.parse_tsi)
        self.add_field_parser('TSF', func=self.parse_tsf)

        for field in VRTDataTrailer().fields:
            self.add_field_parser(field.name, field.format)

        for name, _, format in CIF0.FIELDS:
            self.add_field_parser(name, format)

    def parse_class_id(self, node):
        if not isinstance(node, yaml.MappingNode):
            raise TypeError('structured data required')
        value = {}
        for key_node, value_node in node.value:
            field_name = key_node.value.lower()
            if field_name == 'oui':
                value['OUI'] = self.parse_oui(value_node)
            elif field_name == 'information class code':
                value['Information Class Code'] = self.parse_int(value_node)
            elif field_name == 'packet class code':
                value['Packet Class Code'] = self.parse_int(value_node)
            else:
                self.warning("Invalid field '%s' for Class ID", key_node.value)
        return value

    def parse_oui(self, node):
        return self.constructor.construct_yaml_str(node)

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
                continue
            if field_value.value is not None:
                field.set_value(field_value.value)
        return packet

class FileParser:
    field_parsers = {}

    def parse(self, filename):
        with open(filename, 'r') as fp:
            document = yaml.load(fp)

        for name, value in document.items():
            if name.startswith('.'):
                logging.debug("Skipping hidden entry %s", name)
            else:
                try:
                    yield self.parse_packet(name, value)
                except Exception as exc:
                    logging.error("%s", str(exc))

    def split_value(self, value):
        if isinstance(value, dict):
            if len(value) != 1:
                raise ValueError('Multiple values in list')
            return value.copy().popitem()
        elif isinstance(value, list):
            raise TypeError('Fields cannot be specified as a list')
        else:
            return value, None

    def parse_packet(self, name, value):
        logging.info('Processing packet %s', name)
        if not isinstance(value, dict):
            raise RuntimeError('Invalid definition for packet ' + name)

        packet_type = value.pop('type', None)
        if packet_type is None:
            raise RuntimeError('No packet type specified for ' + name)
        if packet_type == 'data':
            packet = VRTDataPacket(name)
        elif packet_type == 'context':
            packet = VRTContextPacket(name)
        elif packet_type == 'control':
            packet = VRTControlPacket(name)
        else:
            raise RuntimeError("Invalid type '{0}' for packet '{1}'".format(packet_type, name))

        for field_name, field_value in value.items():
            if field_name == 'required':
                if not isinstance(field_value, list):
                    raise TypeError('Invalid value for required fields')
                for item in field_value:
                    key, val = self.split_value(item)
                    field = self.parse_field(packet, key, val)
                    # field.required
            elif field_name == 'optional':
                if not isinstance(field_value, list):
                    raise TypeError('Invalid value for optional fields')
                for item in field_value:
                    key, val = self.split_value(item)
                    field = self.parse_field(packet, key, val)
                    # field.optional
            else:
                field = self.parse_field(packet, field_name, field_value)
                # field.constant

        return packet

    def parse_field_value(self, name, value):
        key = name.casefold()
        parser = self.field_parsers.get(key)
        if parser:
            return parser(self, value)
        else:
            return value

    def parse_tsi(self, value):
        return {'none':  TSI.NONE,
                'utc':   TSI.UTC,
                'gps':   TSI.GPS,
                'other': TSI.OTHER}[value.lower()]
    field_parsers['tsi'] = parse_tsi

    def parse_tsf(self, value):
        return {'none':         TSF.NONE,
                'samples':      TSF.SAMPLE_COUNT,
                'picoseconds':  TSF.REAL_TIME,
                'free running': TSF.FREE_RUNNING}[value.lower()]
    field_parsers['tsf'] = parse_tsf

    def parse_field(self, packet, name, value):
        try:
            field = packet.add_field(name)
        except KeyError:
            logging.error("Invalid field '%s' for packet %s", name, packet.name)
            return
        value = self.parse_field_value(name, value)
        logging.debug("Field '%s' = %s", name, value)
        field.set_value(value)
