#!/usr/bin/python3

from enum import IntEnum
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
class TSI(IntEnum):
    NONE  = 0
    UTC   = 1
    GPS   = 2
    OTHER = 3

# TSF Code
# 00 - no fractional timcode
# 01 - sample-count
# 10 - real-time (picoseconds)
# 11 - free-running count
class TSF(IntEnum):
    NONE         = 0
    SAMPLE_COUNT = 1
    REAL_TIME    = 2
    FREE_RUNNING = 3

class PacketType(IntEnum):
    """
    Constants for the 4-bit Packet Type field in the VRT Packet Header.
    Refer to VITA 49.2 Table 5.1.1-1.
    """
    SIGNAL_DATA              = 0 # 0000
    SIGNAL_DATA_STREAM_ID    = 1 # 0001
    EXTENSION_DATA           = 2 # 0010
    EXTENSION_DATA_STREAM_ID = 3 # 0011
    CONTEXT                  = 4 # 0100
    EXTENSION_CONTEXT        = 5 # 0101
    COMMAND                  = 6 # 0110
    EXTENSION_COMMAND        = 7 # 0111
    # 1000-1111 reserved for future VRT Packet types


class FieldDescriptor(object):
    DISABLED = 0
    OPTIONAL = 1
    REQUIRED = 2

    __slots__ = ('name', '_enable_state', 'default_value')
    def __init__(self, name, default_value=None):
        self.name = name
        self._enable_state = FieldDescriptor.DISABLED
        self.default_value = default_value

    def match(self, name):
        return name.lower() == self.name.lower()

    @property
    def is_enabled(self):
        if self.default_value is not None:
            return True
        else:
            return self.is_required

    @property
    def is_required(self):
        return self._enable_state == FieldDescriptor.REQUIRED

    def set_required(self):
        self._enable_state = FieldDescriptor.REQUIRED

    @property
    def is_optional(self):
        return self._enable_state == FieldDescriptor.OPTIONAL

    def set_optional(self):
        self._enable_state = FieldDescriptor.OPTIONAL

    @property
    def is_disabled(self):
        return self._enable_state == FieldDescriptor.DISABLED

    def set_disabled(self):
        self._enable_state = FieldDescriptor.DISABLED

class FieldContainer:
    def __init__(self):
        self.__fields = []

    def add_field(self, field):
        self.__fields.append(field)
        return field

    def get_field(self, name):
        for field in self.__fields:
            if field.match(name):
                return field
        return None

    @property
    def fields(self):
        return self.__fields

class VRTHeader(FieldContainer):
    def __init__(self, stream_id=True):
        super().__init__()
        self.stream_id = self.add_field(FieldDescriptor('Stream ID'))
        if stream_id:
            self.stream_id.set_required()
        self.class_id = self.add_field(FieldDescriptor('Class ID'))

class VRTPacket(object):
    def __init__(self, name, stream_id=True):
        self.name = name
        self.header = VRTHeader(stream_id)
        self.integer_time = TSI.NONE
        self.fractional_time = TSF.NONE

    def get_field(self, name):
        return self.header.get_field(name)

    def get_header_bytes(self):
        header = bytearray(b'\x00' * 4)

        header[0] = self.packet_type() << 4
        if self.header.class_id.is_enabled:
            header[0] |= 0x08

        return header


class TrailerField(FieldDescriptor):
    __slots__ = ('enable_bit', 'value_bit')

    def __init__(self, name, enable_bit, value_bit):
        FieldDescriptor.__init__(self, name)
        self.enable_bit = enable_bit
        self.value_bit = value_bit


class VRTDataTrailer(FieldContainer):
    def __init__(self):
        super().__init__()
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
        self.context_packet_count = self._add_field('Associated Context Packet Count', 7, 6)

    def _add_field(self, name, enable_pos, value_pos):
        field = TrailerField(name, enable_pos, value_pos)
        self.add_field(field)
        return field

    def get_field(self, name):
        field = super().get_field(name)
        if field is None:
            raise KeyError(name)
        return field

    def get_value(self):
        flag = 0
        for field in self.fields:
            if field.is_enabled:
                flag |= 1 << field.enable_bit
            if field.default_value:
                flag |= 1 << field.value_bit
        if self.context_packet_count.is_enabled:
            flag |= 1 << 7
        return flag

class VRTDataPacket(VRTPacket):
    def __init__(self, name):
        VRTPacket.__init__(self, name, stream_id=False)
        self.is_spectrum = False
        self.trailer = None

    def packet_type(self):
        if self.header.stream_id.is_enabled:
            return PacketType.SIGNAL_DATA_STREAM_ID
        else:
            return PacketType.SIGNAL_DATA

    def packet_specific_bits(self):
        indicator = 0
        if self.has_trailer:
            indicator |= 0x4
        if not self.is_v49_0():
            indicator |= 0x2
        if self.is_spectral:
            indicator |= 0x1
        return indicator

    def is_v49_0(self):
        # TBD: How to check?
        return True

    def has_trailer(self):
        return self.trailer is not None

class VRTContextPacket(VRTPacket):
    def __init__(self, name):
        VRTPacket.__init__(self, name, stream_id=True)

    def packet_type(self):
        return PacketType.CONTEXT

    def packet_specific_bits(self):
        indicator = 0
        if not self.is_v49_0():
            indicator |= 0x02
        if self.is_timestamp_mode:
            indicator |= 0x01
        return indicator

    def is_v49_0(self):
        # TBD: How to check?
        return True

class VRTCommandPacket(VRTPacket):
    def __init__(self, name):
        VRTPacket.__init__(self, name, stream_id=True)

    def packet_type(self):
        return PacketType.COMMAND

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

    def parse_context_packet(self, name, node):
        return VRTContextPacket(name)

    def parse_command_packet(self, name, node):
        return VRTCommandPacket(name)

    def parse_namespace(self, node):
        namespace = str(node.value)
        logging.debug('Using namespace %s', namespace)
        self.namespace = namespace

    def parse_field(self, field, node):
        if isinstance(node, yaml.ScalarNode):
            bool_value = self._constructor.construct_yaml_bool(node)
            if bool_value:
                field.set_required()

    def parse_trailer(self, node):
        if not isinstance(node, yaml.MappingNode):
            logging.warning('Invalid trailer definition in packet (line %d, column %d)' % (node.start_mark.line, node.start_mark.column))
            return None

        trailer = VRTDataTrailer()
        for field_name, field_value in node.value:
            field_name = str(field_name.value)
            try:
                field = trailer.get_field(field_name)
            except KeyError as exc:
                logging.warning('Invalid trailer field %s', exc)
                continue

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

        logging.debug('Trailer %s', hex(trailer.get_value()))

        return trailer

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
                trailer = self.parse_trailer(value_node)
            else:
                field = packet.get_field(field_name)
                if field is None:
                    logging.error("Invalid field '%s'", field_name)
                    continue
                logging.debug("Parsing field '%s'", field.name)
                self.parse_field(field, value_node)

    def get_packet_name(self, name):
        if self.namespace:
            return '::'.join((self.namespace, name))
        else:
            return name

    def parse_data_packets(self, node):
        if not isinstance(node, yaml.MappingNode):
            logging.warning('Invalid data packet section (line %d column %d)',
                            node.start_mark.line, node.start_mark.column)

        for key_node, value_node in node.value:
            packet = VRTDataPacket(key_node.value)
            self.parse_packet(packet, value_node)
            yield packet

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
            elif name == 'data':
                yield from self.parse_data_packets(value_node)
            else:
                if name.startswith('.'):
                    logging.debug('Skipping hidden packet %s', name)
                    continue
                try:
                    yield self.parse_packet(name, value_node)
                except Exception as exc:
                    logging.exception(exc)

def hex_bytes(data):
    return ''.join('{:02x}'.format(ch) for ch in data)

if __name__ == '__main__':
    source = sys.argv[1]
    p = Parser()
    for packet in p.parse(source):
        header = packet.get_header_bytes()
        logging.debug('Packet header 0x%s', hex_bytes(header))
