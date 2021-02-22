# Copyright (C) 2019 Geon Technologies, LLC
#
# This file is part of vrtgen.
#
# vrtgen is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# vrtgen is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
# more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.

"""
Types and defintions for CIF0 fields.
"""
from . import basic
from . import enums
from .struct import Struct, Field, Reserved
from .cifmeta import CIFFields

class GeolocationAngle(basic.FixedPointType, bits=32, radix=22):
    """
    Geolocation Angle Format [Definition 9.4.5-1].
    """

class CartesianCoordinate(basic.FixedPointType, bits=32, radix=5):
    """
    Position coordinate format for ECEF Ephermeris [Rule 9.4.3-5].
    """

class VelocityCoordinate(basic.FixedPointType, bits=32, radix=16):
    """
    Velocity coordinate format for ECEF Ephemeris [Rule 9.4.3-7].
    """

class Gain(Struct):
    """
    Gain/Attenuation [9.5.3].
    """
    stage_1 = Field('Stage 1', basic.FixedPoint16r7, alias='stage-1')
    stage_2 = Field('Stage 2', basic.FixedPoint16r7, alias='stage-2')

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

class PayloadFormat(Struct):
    """
    Data Packet Payload Format Field [9.13.3]
    """
    packing_method = Field('Packing Method', enums.PackingMethod)
    real_complex_type = Field('Real/Complex Type', enums.DataSampleType)
    data_item_format = Field('Data Item Format', enums.DataItemFormat)
    repeat_indicator = Field('Sample-Component Repeat Indicator', basic.Boolean)
    event_tag_size = Field('Event Tag Size', basic.IntegerType.create(3, signed=False))
    channel_tag_size = Field('Channel Tag Size', basic.IntegerType.create(4, signed=False))
    data_item_fraction_size = Field(
        'Data Item Fraction Size', basic.IntegerType.create(4, signed=False)
    )
    item_packing_field_size = Field('Item Packing Field Size', basic.NonZeroSize.create(6))
    data_item_size = Field('Data Item Size', basic.NonZeroSize.create(6))
    repeat_count = Field('Repeat Count', basic.NonZeroSize.create(16))
    vector_size = Field('Vector Size', basic.NonZeroSize.create(16))

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

class CIF0(CIFFields):
    """
    CIF 0 Fields.
    """
    # Context Field Change Indicator (0/31) is a binary flag that can be set at
    # run-time. No configuration is possible.
    change_indicator = Field(enums.CIF0Fields.CHANGE_INDICATOR.value, basic.Boolean)

    # Reference Point Identifier (0/30): Stream ID
    reference_point_id = Field(enums.CIF0Fields.REFERENCE_POINT_ID.value, basic.StreamIdentifier)

    # Bandwidth (0/29) fixed-point 64/20, Hz
    bandwidth = Field(enums.CIF0Fields.BANDWIDTH.value, basic.FixedPoint64r20)

    # IF Reference Frequency (0/28): fixed-point 64/20, Hz
    if_frequency = Field(enums.CIF0Fields.IF_REFERENCE_FREQUENCY.value, basic.FixedPoint64r20)

    # RF Reference Frequency (0/27): fixed-point 64/20, Hz
    rf_frequency = Field(enums.CIF0Fields.RF_REFERENCE_FREQUENCY.value, basic.FixedPoint64r20)

    # RF Reference Frequency Offset (0/26): fixed-point 64/20, Hz
    rf_frequency_offset = Field(enums.CIF0Fields.RF_REFERENCE_FREQUENCY_OFFSET.value, basic.FixedPoint64r20)

    # IF Band Offset (0/25): fixed-point 64/20, Hz
    if_band_offset = Field(enums.CIF0Fields.IF_BAND_OFFSET.value, basic.FixedPoint64r20)

    # Reference Level (0/24): fixed-point 16/7 dBm (upper 16 reserved)
    reference_level = Field(enums.CIF0Fields.REFERENCE_LEVEL.value, basic.FixedPoint16r7)

    # Gain (0/23): [stage2 (optional), stage1], fixed-point 16/7, dB
    gain = Field(enums.CIF0Fields.GAIN.value, Gain)

    # Over-Range Count (0/22): integer 32
    over_range_count = Field(enums.CIF0Fields.OVER_RANGE_COUNT.value, basic.Integer32)

    # Sample Rate (0/21): fixed-point 64/20, Hz
    sample_rate = Field(enums.CIF0Fields.SAMPLE_RATE.value, basic.FixedPoint64r20)

    # Timestamp Adjustment (0/20): fractional time (integer 64)
    timestamp_adjustment = Field(enums.CIF0Fields.TIMESTAMP_ADJUSTMENT.value, basic.Integer64)

    # Timestamp Calibration Time (0/19): 1 word, depends on prologue TSI
    timestamp_calibration_time = Field(enums.CIF0Fields.TIMESTAMP_CALIBRATION_TIME.value, basic.Integer32)

    # Temperature (0/18): fixed-point 16/6, degrees C (upper 16 reserved)
    temperature = Field(enums.CIF0Fields.TEMPERATURE.value, basic.FixedPointType.create(16, 6))

    # Device Identifier (0/17): 64 bits total, structure
    device_id = Field(enums.CIF0Fields.DEVICE_IDENTIFIER.value, DeviceIdentifier)

    # State/Event Indicators (0/16): 32 bits, bit flags
    state_event_indicators = Field(enums.CIF0Fields.STATE_EVENT_INDICATORS.value, basic.Integer32)

    # Data Payload Format (0/15): structured
    data_format = Field(enums.CIF0Fields.SIGNAL_DATA_PAYLOAD_FORMAT.value, PayloadFormat)

    # Formatted GPS (0/14): structured
    formatted_gps = Field(enums.CIF0Fields.FORMATTED_GPS.value, Geolocation)

    # Formatted INS (0/13): same format as GPS
    formatted_ins = Field(enums.CIF0Fields.FORMATTED_INS.value, Geolocation)

    # ECEF Ephemeris (0/12): structured
    ecef_ephemeris = Field(enums.CIF0Fields.ECEF_EPHEMERIS.value, Ephemeris)

    # Relative Ephemeris (0/11): same format as ECEF Ephemeris
    # See also Ephemeris Reference Identifier
    relative_ephemeris = Field(enums.CIF0Fields.RELATIVE_EPHEMERIS.value, Ephemeris)

    # Ephemeric Reference Identifier (0/10): Stream ID of context packet stream
    # whose ECEF Ephemeris is necessary to translate Relative Ephemeris
    ephemeris_ref_id = Field(enums.CIF0Fields.EPHEMERIS_REFERENCE_ID.value, basic.StreamIdentifier)

    # GPS ASCII (0/9): Formatted GPS "sentences" (such as NMEA). 2 word header
    # (manufacturer OUI, number of words) plus arbitrary binary data, null-
    # padded to a full word.
    gps_ascii = Field(enums.CIF0Fields.GPS_ASCII.value, None) # not implemented

    # Context Assocation Lists (0/8): Structure to associate multiple context
    # streams with another stream. There is a 2-word header followed by four
    # lists of Stream IDs (Source, System, Vector-Component, Asynchronous-
    # Channel) and an option Asychronous-Channel tag list. Most, if not all,
    # of this should be handled at run-time, with the only code generation
    # support being to enable the field.
    context_association_lists = Field(enums.CIF0Fields.CONTEXT_ASSOCIATION_LISTS.value, None)

    # Field Attributes Enable (CIF7)
    cif7_enable = Field(enums.CIF0Fields.CIF7_ENABLE.value, basic.Boolean)

    # Reserved (0/6 to 0/4)
    reserved_6 = Reserved(1)
    reserved_5 = Reserved(1)
    reserved_4 = Reserved(1)

    # CIF 3 Enable
    cif3_enable = Field(enums.CIF0Fields.CIF3_ENABLE.value, basic.Boolean)

    # CIF 2 Enable
    cif2_enable = Field(enums.CIF0Fields.CIF2_ENABLE.value, basic.Boolean)

    # CIF 1 Enable
    cif1_enable = Field(enums.CIF0Fields.CIF1_ENABLE.value, basic.Boolean)

    # Reserved
    reserved_0 = Reserved(1)
