from enum import IntEnum
import inspect

from .enums import *

class Field:
    class Mode(IntEnum):
        DISABLED  = 0
        OPTIONAL  = 1
        REQUIRED  = 2
        MANDATORY = 3

    __slots__ = ('_constant', '_enable')
    def __init__(self, enable=Mode.DISABLED):
        self._enable = Field.Mode(enable)
        self._constant = False

    @property
    def is_set(self):
        if self.has_value:
            return True
        return self.is_mandatory

    @property
    def has_value(self):
        return False

    @property
    def is_enabled(self):
        return self._enable != Field.Mode.DISABLED

    @property
    def enable(self):
        return self._enable

    @enable.setter
    def enable(self, enabled):
        enabled = Field.Mode(enabled)
        if self.is_mandatory:
            # Mandatory state cannot be overwritten
            if enabled in (Field.Mode.DISABLED, Field.Mode.OPTIONAL):
                raise ValueError('field is mandatory')
            return
        self._enable = enabled

    @property
    def is_required(self):
        return self._enable in (Field.Mode.REQUIRED, Field.Mode.MANDATORY)

    @property
    def is_optional(self):
        return self._enable == Field.Mode.OPTIONAL

    @property
    def is_mandatory(self):
        return self._enable == Field.Mode.MANDATORY

    @property
    def is_disabled(self):
        return self._enable == Field.Mode.DISABLED

    @property
    def is_constant(self):
        return self._constant

    def set_constant(self):
        if not self.is_required:
            self.enable = Field.Mode.REQUIRED
        self._constant = True

    @property
    def enable_flag(self):
        return 1 << self.enable_bit

    @classmethod
    def match(cls, name):
        # Relying on a little bit of class trickery here: Field does not have
        # a "name" attribute, but it's never directly used. field_descriptor()
        # creates derived classes with the name set as a class attribute (this
        # is also a minor space optimization, since the name is the same for
        # all instances).
        return cls.name.casefold() == name.casefold()

# Placeholder for field types that have not been implemented yet
class UnimplementedField(Field):
    pass

class FieldContainer:
    def __init__(self):
        self.__fields = {}
        for name, field in self.get_field_descriptors():
            instance = field()
            setattr(self, name, instance)
            self.__fields[field.name.casefold()] = instance

    def get_field(self, name):
        return self.__fields.get(name.casefold(), None)

    @classmethod
    def get_field_descriptors(cls):
        def is_field(obj):
            return isinstance(obj, type) and issubclass(obj, Field)
        return inspect.getmembers(cls, is_field)

    @property
    def fields(self):
        return self.__fields.values()

class SimpleField(Field):
    def __init__(self):
        Field.__init__(self)
        self.value = None

    @property
    def has_value(self):
        return self.value is not None

class StructField(Field, FieldContainer):
    def __init__(self):
        Field.__init__(self)
        FieldContainer.__init__(self)
        # By default, all struct fields are required. They can marked as
        # optional or unused later.
        for field in self.fields:
            field.enable = Field.Mode.REQUIRED

    @property
    def has_value(self):
        return any(f.has_value for f in self.fields)

def field_descriptor(name, field, enable_bit=None):
    bases = (field,)
    namespace = {
        'name': name,
        'enable_bit': enable_bit
    }
    return type(name, bases, namespace)

# Basic field types

class BitField(SimpleField):
    pass

class IntegerField(SimpleField):
    @classmethod
    def create(cls, bits):
        name = 'Int{:d}Field'.format(bits)
        return type(name, (cls,), {'bits':bits})

Int64Field = IntegerField.create(64)
Int32Field = IntegerField.create(32)
Int24Field = IntegerField.create(24)
Int16Field = IntegerField.create(16)

Identifier32 = Int32Field
Identifier16 = Int16Field

class FixedPointField(SimpleField):
    @classmethod
    def create(cls, bits, radix):
        name = 'Fixed{:d}_{:d}Field'.format(bits, radix)
        return type(name, (cls,), {'bits':bits, 'radix':radix})

# Common field types

StreamID = Identifier32

class OUIField(Int24Field):
    pass

class ClassIDField(StructField):
    oui = field_descriptor('OUI', OUIField)
    information_class = field_descriptor('Information Class Code', Int16Field)
    packet_class = field_descriptor('Packet Class Code', Int16Field)

class TSIField(SimpleField):
    def __init__(self):
        super().__init__()
        self.value = TSI.NONE

class TSFField(SimpleField):
    def __init__(self):
        super().__init__()
        self.value = TSF.NONE

class DeviceIDField(StructField):
    manufacturer_oui = field_descriptor('Manufacturer OUI', OUIField)
    device_code = field_descriptor('Device Code', Int16Field)

class UserDefinedField(Field):
    def __init__(self):
        Field.__init__(self)
        self.value = None

    @classmethod
    def create(cls, bits):
        name = 'UserDefined{:d}Field'.format(bits)
        return type(name, (cls,), {'bits':bits})

    def add_field(self, name, bits, word=None, position=None):
        # TODO: validation of fields (overlap, size, etc.)
        pass

# CIF0 Types
class GainField(StructField):
    stage1 = field_descriptor('Stage 1', FixedPointField.create(16, 7))
    stage2 = field_descriptor('Stage 2', FixedPointField.create(16, 7))

class StateEventIndicators(StructField):
    calibrated_time = field_descriptor('Calibrated Time', BitField, 31)
    valid_data = field_descriptor('Valid Data', BitField, 30)
    reference_lock = field_descriptor('Reference Lock', BitField, 29)
    agc_mgc = field_descriptor('AGC/MGC', BitField, 28)
    detected_signal = field_descriptor('Detected Signal', BitField, 27)
    spectral_inversion = field_descriptor('Spectral Inversion', BitField, 26)
    over_range = field_descriptor('Over-range', BitField, 25)
    sample_loss = field_descriptor('Sample Loss', BitField, 24)
    # [23..20] reserved
    # [7..0] User-Defined
    user_defined = field_descriptor('User-Defined', UserDefinedField.create(8))

class GeolocationAngleField(FixedPointField):
    bits = 32
    radix = 22
    UNSPECIFIED = 0x7FFFFFFF

class GeolocationField(StructField):
    tsi = field_descriptor('TSI', TSIField)
    tsf = field_descriptor('TSF', TSFField)
    manufacturer_oui = field_descriptor('Manufacturer OUI', OUIField)
    # Integer timestamp should be 0xFFFFFFFF if TSI is 0
    integer_timestamp = field_descriptor('Integer-second Timestamp', Int32Field)
    # Fractional timestamp should be 0xFFFFFFFFFFFFFFFF if TSF is 0
    fractional_timestamp = field_descriptor('Fractional-second Timestamp', Int64Field)
    latitude = field_descriptor('Latitude', GeolocationAngleField)
    longitude = field_descriptor('Longitude', GeolocationAngleField)
    altitude = field_descriptor('Altitude', FixedPointField.create(32, 5))
    ground_speed = field_descriptor('Speed Over Ground', FixedPointField.create(32,16))
    heading_angle = field_descriptor('Heading Angle', GeolocationAngleField)
    track_angle = field_descriptor('Track Angle', GeolocationAngleField)
    magnetic_variation = field_descriptor('Magnetic Variation', GeolocationAngleField)

CartesianCoordinateField = FixedPointField.create(32, 5)

class EphemerisField(StructField):
    tsi = field_descriptor('TSI', TSIField)
    tsf = field_descriptor('TSF', TSFField)
    manufacturer_oui = field_descriptor('Manufacturer OUI', OUIField)
    # Integer timestamp should be 0xFFFFFFFF if TSI is 0
    integer_timestamp = field_descriptor('Integer-second Timestamp', Int32Field)
    # Fractional timestamp should be 0xFFFFFFFFFFFFFFFF if TSF is 0
    fractional_timestamp = field_descriptor('Fractional-second Timestamp', Int64Field)
    position_x = field_descriptor('Position X', CartesianCoordinateField)
    position_y = field_descriptor('Position Y', CartesianCoordinateField)
    position_z = field_descriptor('Position Z', CartesianCoordinateField)
    attitude_alpha = field_descriptor('Attitude Alpha', GeolocationAngleField)
    attitude_beta = field_descriptor('Attitude Beta', GeolocationAngleField)
    attitude_phi = field_descriptor('Attitude Phi', GeolocationAngleField)
    velocity_dx = field_descriptor('Velocity dX', FixedPointField.create(32, 16))
    velocity_dy = field_descriptor('Velocity dY', FixedPointField.create(32, 16))
    velocity_dz = field_descriptor('Velocity dZ', FixedPointField.create(32, 16))


# CIF1 Types
class PolarizationField(StructField):
    tilt = field_descriptor('Tilt Angle', FixedPointField.create(16, 13))
    ellipticity = field_descriptor('Ellipticity Angle', FixedPointField.create(16, 13))

class PointingVectorField(StructField):
    elevation = field_descriptor('Elevation Angle', FixedPointField.create(16, 7))
    azimuthal = field_descriptor('Azimuthal Angle', FixedPointField.create(16, 7))

class BeamWidthsField(StructField):
    horizontal = field_descriptor('Horizontal', FixedPointField.create(16, 7))
    vertical = field_descriptor('Vertical', FixedPointField.create(16, 7))

class EbNoBERField(StructField):
    # Energy per Bit to Noise Density ratio (Eb/No)
    # Maximum positive value (0x7FFF) signifies "unused"
    ebno = field_descriptor('Eb/No', FixedPointField.create(16, 7))

    # Bit Error Rate (BER)
    # Maximum positive value (0x7FFF) signifies "unused"
    ber = field_descriptor('BER', FixedPointField.create(16, 7))

class ThresholdField(StructField):
    stage1 = field_descriptor('Stage 1', FixedPointField.create(16, 7))
    stage2 = field_descriptor('Stage 2', FixedPointField.create(16, 7))

class SNRNoiseField(StructField):
    # Signal-to-Noise Ratio (dB)
    # Maximum positive value (0x7FFF) signifies "unused"
    snr = field_descriptor('SNR', FixedPointField.create(16, 7))

    # Noise Figure (db)
    # O signifies "unused"
    noise = field_descriptor('Noise Figure', FixedPointField.create(16, 7))

class IndexListOptions(Field):
    def __init__(self):
        super().__init__()
        self.__size = None

    @property
    def entry_size(self):
        return self.__size

    @entry_size.setter
    def entry_size(self, size):
        if size not in (None, 8, 16, 32):
            raise ValueError('Index list entries must be 8, 16 or 32 bits')
        self.__size = size

class DiscreteIO32Field(UserDefinedField):
    bits = 32

class DiscreteIO64Field(UserDefinedField):
    bits = 64
