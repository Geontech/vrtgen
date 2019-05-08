"""
Types and defintions for CIF0 fields.
"""
from . import basic
from . import enums
from .struct import Struct, Field, Reserved, Enable
from .cifmeta import ContextIndicatorFields

class GeolocationAngle(basic.FixedPointType, bits=32, radix=22):
    """
    Geolocation Angle Format [Definition 9.4.5-1].
    """
    pass

class CartesianCoordinate(basic.FixedPointType, bits=32, radix=5):
    """
    Position coordinate format for ECEF Ephermeris [Rule 9.4.3-5].
    """
    pass

class VelocityCoordinate(basic.FixedPointType, bits=32, radix=16):
    """
    Velocity coordinate format for ECEF Ephemeris [Rule 9.4.3-7].
    """
    pass

class Gain(Struct):
    """
    Gain/Attenuation [9.5.3].
    """
    stage_1 = Field('Stage 1', basic.FixedPoint16_7)
    stage_2 = Field('Stage 2', basic.FixedPoint16_7)

class DeviceIdentifier(Struct):
    """
    Device Identifier [9.10.1].
    """
    reserved_1 = Reserved(8)
    manufacturer_oui = Field('Manufacturer OUI', basic.OUI)
    reserved_2 = Reserved(16)
    device_code = Field('Device Code', basic.Integer16)

class Geolocation(Struct):
    """
    Formatted GPS Geolocation [9.4.5) and INS Geolocation (9.4.6].
    """
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
    ground_speed = Field('Speed Over Ground', basic.FixedPointType.create(32, 16))
    heading_angle = Field('Heading Angle', GeolocationAngle)
    track_angle = Field('Track Angle', GeolocationAngle)
    magnetic_variation = Field('Magnetic Variation', GeolocationAngle)

class Ephemeris(Struct):
    """
    ECEF Ephemeris [9.4.3] and Relative Ephemeris [9.4.9].
    """
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
    velocity_dx = Field('Velocity dX', VelocityCoordinate)
    velocity_dy = Field('Velocity dY', VelocityCoordinate)
    velocity_dz = Field('Velocity dZ', VelocityCoordinate)

CIF0Fields = ContextIndicatorFields('CIF0')

# Context Field Change Indicator (0/31) is a binary flag that can be set at
# run-time. No configuration is possible.
CIF0Fields.change_indicator = Enable('Context Field Change Indicator')

# Reference Point Identifier (0/30): Stream ID
CIF0Fields.reference_point_id = Field('Reference Point Identifier', basic.StreamIdentifier)

# Bandwidth (0/29) fixed-point 64/20, Hz
CIF0Fields.bandwidth = Field('Bandwidth', basic.FixedPoint64_20)

# IF Reference Frequency (0/28): fixed-point 64/20, Hz
CIF0Fields.if_frequency = Field('IF Reference Frequency', basic.FixedPoint64_20)

# RF Reference Frequency (0/27): fixed-point 64/20, Hz
CIF0Fields.rf_frequency = Field('RF Reference Frequency', basic.FixedPoint64_20)

# RF Reference Frequency Offset (0/26): fixed-point 64/20, Hz
CIF0Fields.rf_frequency_offset = Field('RF Reference Frequency Offset', basic.FixedPoint64_20)

# IF Band Offset (0/25): fixed-point 64/20, Hz
CIF0Fields.if_band_offset = Field('IF Band Offset', basic.FixedPoint64_20)

# Reference Level (0/24): fixed-point 16/7 dBm (upper 16 reserved)
CIF0Fields.reference_level = Field('Reference Level', basic.FixedPoint16_7)

# Gain (0/23): [stage2 (optional), stage1], fixed-point 16/7, dB
CIF0Fields.gain = Field('Gain', Gain)

# Over-Range Count (0/22): integer 32
CIF0Fields.over_range_count = Field('Over-Range Count', basic.Integer32)

# Sample Rate (0/21): fixed-point 64/20, Hz
CIF0Fields.sample_rate = Field('Sample Rate', basic.FixedPoint64_20)

# Timestamp Adjustment (0/20): fractional time (integer 64)
CIF0Fields.timestamp_adjustment = Field('Timestamp Adjustment', basic.Integer64)

# Timestamp Calibration Time (0/19): 1 word, depends on prologue TSI
CIF0Fields.timestamp_calibration_time = Field('Timestamp Calibration Time', basic.Integer32)

# Temperature (0/18): fixed-point 16/6, degrees C (upper 16 reserved)
CIF0Fields.temperature = Field('Temperature', basic.FixedPointType.create(16, 6))

# Device Identifier (0/17): 64 bits total, structure
CIF0Fields.device_id = Field('Device Identifier', DeviceIdentifier)

# State/Event Indicators (0/16): 32 bits, bit flags
CIF0Fields.state_event_indicators = Field('State/Event Indicators', basic.Integer32)

# Data Payload Format (0/15): structured (TODO: not implemented)
CIF0Fields.data_format = Field('Signal Data Packet Payload Format', None)

# Formatted GPS (0/14): structured
CIF0Fields.formatted_gps = Field('Formatted GPS', Geolocation)

# Formatted INS (0/13): same format as GPS
CIF0Fields.formatted_ins = Field('Formatted INS', Geolocation)

# ECEF Ephemeris (0/12): structured
CIF0Fields.ecef_ephemeris = Field('ECEF Ephemeris', Ephemeris)

# Relative Ephemeris (0/11): same format as ECEF Ephemeris
# See also Ephemeris Reference Identifier
CIF0Fields.relative_ephemeris = Field('Relative Ephemeris', Ephemeris)

# Ephemeric Reference Identifier (0/10): Stream ID of context packet stream
# whose ECEF Ephemeris is necessary to translate Relative Ephemeris
CIF0Fields.ephemeris_ref_id = Field('Ephemeris Reference ID', basic.StreamIdentifier)

# GPS ASCII (0/9): Formatted GPS "sentences" (such as NMEA). 2 word header
# (manufacturer OUI, number of words) plus arbitrary binary data, null-
# padded to a full word.
CIF0Fields.gps_ascii = Field('GPS ASCII', None) # not implemented

# Context Assocation Lists (0/8): Structure to associate multiple context
# streams with another stream. There is a 2-word header followed by four
# lists of Stream IDs (Source, System, Vector-Component, Asynchronous-
# Channel) and an option Asychronous-Channel tag list. Most, if not all,
# of this should be handled at run-time, with the only code generation
# support being to enable the field.
CIF0Fields.context_association_lists = Field('Context Association Lists', None)

# Field Attributes Enable (CIF7)
CIF0Fields.cif7_enable = Enable('CIF 7 Enable')

# Reserved (0/6 to 0/4)
CIF0Fields.reserved_6 = Reserved(1)
CIF0Fields.reserved_5 = Reserved(1)
CIF0Fields.reserved_4 = Reserved(1)

# CIF 3 Enable
CIF0Fields.cif3_enable = Enable('CIF 3 Enable')

# CIF 2 Enable
CIF0Fields.cif2_enable = Enable('CIF 2 Enable')

# CIF 1 Enable
CIF0Fields.cif1_enable = Enable('CIF 1 Enable')

# Reserved
CIF0Fields.reserved_0 = Reserved(1)

CIF0Enables = CIF0Fields.create_enables()
