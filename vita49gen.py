#!/usr/bin/python

import logging
import struct
import sys

import yaml

logging.basicConfig(format='%(levelname)s: %(message)s')
logging.getLogger().setLevel(logging.DEBUG)

# TSI Code
# 00 - no integer timecode
# 01 - UTC
# 10 - GPS
# 11 - Other
class TSI(object):
    NONE  = 0
    UTC   = 1
    GPS   = 2
    OTHER = 3

# TSF Code
# 00 - no fractional timcode
# 01 - sample-count
# 10 - real-time (picoseconds)
# 11 - free-running count
class TSF(object):
    NONE         = 0
    SAMPLE_COUNT = 1
    REAL_TIME    = 2
    FREE_RUNNING = 3

DISABLED = 0
OPTIONAL = 1
REQUIRED = 2

class VRTMessage(object):
    def __init__(self, name, stream_id=True):
        self.name = name
        self.has_stream_id = stream_id
        self.has_class_id = False
        self.integer_time = TSI.NONE
        self.fractional_time = TSF.NONE

    def header(self):
        header = bytearray('\x00' * 4)

        header[0] = self.packet_type() << 4
        if self.has_class_id:
            header[0] |= 0x08

        return header

class FieldDescriptor(object):
    __slots__ = ('name', '_enabled', 'default_value')
    def __init__(self, name, is_enabled=DISABLED, default_value=None):
        self.name = name
        self._enabled = is_enabled
        self.default_value = default_value

    @property
    def is_enabled(self):
        if self.is_optional:
            return self.default_value is not None
        else:
            return self.is_required

    @property
    def is_required(self):
        return self._enabled == REQUIRED

    def set_required(self):
        self._enabled = REQUIRED

    @property
    def is_optional(self):
        return self._enabled == OPTIONAL

    def set_optional(self):
        self._enabled = OPTIONAL

    @property
    def is_disabled(self):
        return self._enabled == DISABLED

    def set_disabled(self):
        self._enabled = DISABLED
        

class TrailerField(FieldDescriptor):
    __slots__ = ('enable_bit', 'value_bit')

    def __init__(self, name, enable_bit, value_bit):
        FieldDescriptor.__init__(self, name, default_value=False)
        self.enable_bit = enable_bit
        self.value_bit = value_bit

class VRTDataTrailer(object):
    def __init__(self):
        self.__fields = []
        self.calibrated_time = self._add_field('Calibrated Time', 31, 19)
        self.valid_data = self._add_field('Valid Data', 30, 18)
        self.reference_lock = self._add_field('Reference Lock', 29, 17)
        self.agc_mgc = self._add_field('AGC/MGC', 28, 16)
        self.detected_signal = self._add_field('Detected Signal', 27, 15)
        self.spectral_inversion = self._add_field('Spectral Inversion', 26, 14)
        self.over_range = self._add_field('Over-range', 25, 13)
        self.sample_loss = self._add_field('Sample Loss', 24, 12)
        # [23,22], [11,10] Sample Frame, User-Defined
        # [21..20], [9..8] User-Defined
        self.context_packet_count = False

    def _add_field(self, name, enable_pos, value_pos):
        field = TrailerField(name, enable_pos, value_pos)
        self.__fields.append(field)
        return field

    def get_field(self, name):
        for field in self.__fields:
            if not isinstance(field, TrailerField):
                continue
            if name.lower() == field.name.lower():
                return field
        raise KeyError('Invalid trailer field "'+name+'"')

    def get_value(self):
        flag = 0
        for field in self.__fields:
            if field.is_enabled:
                flag |= 1 << field.enable_bit
            if field.default_value:
                flag |= 1 << field.value_bit
        if self.context_packet_count:
            flag |= 1 << 7
        return flag

class VRTDataMessage(VRTMessage):
    def __init__(self, name):
        VRTMessage.__init__(self, name, stream_id=False)
        self.is_spectrum = False
        self.trailer = None

    def packet_type(self):
        if self.has_stream_id:
            return 0x1
        else:
            return 0x0

    def packet_specific_bits(self):
        indicator = 0
        if self.has_trailer:
            indicator |= 0x4
        if not self.is_v49_0():
            indicator |= 0x2
        if self.is_spectral:
            indicator |= 0x1

    def is_v49_0(self):
        # TBD: How to check?
        return True

    def has_trailer(self):
        return self.trailer is not None

class VRTContextMessage(VRTMessage):
    def __init__(self, name):
        VRTMessage.__init__(self, name, stream_id=True)

    def packet_type(self):
        return 0x4

    def packet_specific_bits(self):
        indicator = 0
        if not self.is_v49_0():
            indicator |= 0x02
        if self.is_timestamp_mode:
            indicator |= 0x01

    def is_v49_0(self):
        # TBD: How to check?
        return True

class VRTCommandMessage(VRTMessage):
    def __init__(self, name):
        VRTMessage.__init__(self, name, stream_id=True)

    def packet_type(self):
        return 0x6

    def is_v49_0(self):
        # Command packets were added in 49.2, so by definition cannot be 49.0
        return False

# Header fields:
# Stream ID (optional in data, mandatory everything else)
# Class ID
# Integer Timestamp
# Fractional Timestamp



class Parser(object):
    def __init__(self):
        self._constructor = yaml.constructor.SafeConstructor()

    def parse_data_message(self, name, data):
        message = VRTDataMessage(name)
        return message

    def parse_context_message(self, name, node):
        return VRTContextMessage(name)
    
    def parse_command_message(self, name, node):
        return VRTCommandMessage(name)

    def parse_namespace(self, node):
        namespace = str(node.value)
        logging.debug('Using namespace %s', namespace)
        self.namespace = namespace

    def parse_trailer(self, node):
        if not isinstance(node, yaml.SequenceNode):
            logging.warning('Invalid trailer definition in message (line %d, column %d)' % (node.start_mark.line, node.start_mark.column))
            return None
        
        trailer = VRTDataTrailer()
        for field_node in node.value:
            if isinstance(field_node, yaml.ScalarNode):
                field_name = str(field_node.value)
                required = True
                value = None
            elif isinstance(field_node, yaml.MappingNode):
                field_name, field_value = field_node.value[0]
                field_name = str(field_name.value)
                mapping = self._constructor.construct_mapping(field_value)
                required = mapping.get('required', True)
                value = mapping.get('default', None)
            else:
                logging.warning('Invalid trailer field definition in message (line %d, column %d)' % (field_node.start_mark.line, field_node.start_mark.column)) 
                continue
            try:
                field = trailer.get_field(field_name)
            except Exception as exc:
                logging.warning('%s', exc)
                continue
            if required:
                field.set_required()
            if value is not None:
                field.default_value = value

        print hex(trailer.get_value())

        return trailer

    def parse_message(self, name, node):
        logging.info('Processing message %s', self.get_message_name(name))
        if not isinstance(node, yaml.MappingNode):
            err = 'Invalid message definition %s (line %d, column %d)' % (name, node.start_mark.line, node.start_mark.column)
            raise RuntimeError(err)

        trailer = None
        for key_node, value_node in node.value:
            field_name = str(key_node.value).lower()
            if field_name == 'trailer':
                trailer = self.parse_trailer(value_node)

        mapping = self._constructor.construct_mapping(node)
        message_type = mapping.get('type', None).lower()
        if message_type == 'data':
            return self.parse_data_message(name, mapping)
        elif message_type == 'context':
            return self.parse_context_message(name, node)
        elif message_type == 'command':
            return self.parse_command_message(name, node)
        elif message_type is None:
            raise RuntimeError('No message type specified')
        else:
            raise RuntimeError('Invalid message type ' + mapping['type'])

    def get_message_name(self, name):
        if self.namespace:
            return '::'.join((self.namespace, name))
        else:
            return name

    def parse(self, filename):
        with open(filename, 'r') as fp:
            root = yaml.compose(fp)
        if not isinstance(root, yaml.MappingNode):
            raise RuntimeError('invalid YAML file')

        self.namespace = None
        for key_node, value_node in root.value:
            name = str(key_node.value)
            if name == 'namespace':
                self.parse_namespace(value_node)
            else: 
                if name.startswith('.'):
                    logging.debug('Skipping hidden message %s', name)
                    continue
                try:
                    yield self.parse_message(name, value_node)
                except Exception as exc:
                    logging.exception(exc)

if __name__ == '__main__':
    source = sys.argv[1]
    p = Parser()
    for message in p.parse(source):
        print [hex(ch) for ch in message.header()]
