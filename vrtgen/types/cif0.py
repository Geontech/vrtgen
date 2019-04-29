"""
Types used for CIF0 fields.
"""
from . import basic
from . import enums
from .struct import Struct, Field, Reserved

class GeolocationAngle(basic.FixedPointType, bits=32, radix=22):
    pass

class CartesianCoordinate(basic.FixedPointType, bits=32, radix=5):
    pass

class Gain(Struct):
    stage_1 = Field('Stage 1', basic.FixedPoint16_7)
    stage_2 = Field('Stage 2', basic.FixedPoint16_7)

class DeviceIdentifier(Struct):
    reserved_1 = Reserved(8)
    manufacturer_oui = Field('Manufacturer OUI', basic.OUI)
    reserved_2 = Reserved(16)
    device_code = Field('Device Code', basic.Integer16)

class Geolocation(Struct):
    reserved = Reserved(4)
    tsi = Field('TSI', enums.TSI)
    tsf = Field('TSF', enums.TSF)
    manufacturer_oui = Field('Manufacturer OUI', basic.OUI)
    # Integer timestamp should be 0xFFFFFFFF if TSI is 0
    integer_timestamp = Field('Integer-second Timestamp', basic.Integer32)
    # Fractional timestamp should be 0xFFFFFFFFFFFFFFFF if TSF is 0
    fractional_timestamp = Field('Fractional-second Timestamp', basic.Integer64)
    latitude = Field('Latitude', GeolocationAngle)
    longitude = Field('Longitude', GeolocationAngle)
    altitude = Field('Altitude', basic.FixedPointType.create(32, 5))
    ground_speed = Field('Speed Over Ground', basic.FixedPointType.create(32,16))
    heading_angle = Field('Heading Angle', GeolocationAngle)
    track_angle = Field('Track Angle', GeolocationAngle)
    magnetic_variation = Field('Magnetic Variation', GeolocationAngle)

class Ephemeris(Struct):
    reserved = Reserved(4)
    tsi = Field('TSI', enums.TSI)
    tsf = Field('TSF', enums.TSF)
    manufacturer_oui = Field('Manufacturer OUI', basic.OUI)
    # Integer timestamp should be 0xFFFFFFFF if TSI is 0
    integer_timestamp = Field('Integer-second Timestamp', basic.Integer32)
    # Fractional timestamp should be 0xFFFFFFFFFFFFFFFF if TSF is 0
    fractional_timestamp = Field('Fractional-second Timestamp', basic.Integer64)
    position_x = Field('Position X', CartesianCoordinate)
    position_y = Field('Position Y', CartesianCoordinate)
    position_z = Field('Position Z', CartesianCoordinate)
    attitude_alpha = Field('Attitude Alpha', GeolocationAngle)
    attitude_beta = Field('Attitude Beta', GeolocationAngle)
    attitude_phi = Field('Attitude Phi', GeolocationAngle)
    velocity_dx = Field('Velocity dX', basic.FixedPoint32_16)
    velocity_dy = Field('Velocity dY', basic.FixedPoint32_16)
    velocity_dz = Field('Velocity dZ', basic.FixedPoint32_16)
