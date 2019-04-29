"""
Types used for CIF1 fields.
"""
from . import basic
from .struct import Struct, Field, Reserved

class Polarization(Struct):
    tilt = Field('Tilt Angle', basic.FixedPoint16_13)
    ellipticity = Field('Ellipticity Angle', basic.FixedPoint16_13)

class PointingVector(Struct):
    elevation = Field('Elevation Angle', basic.FixedPoint16_7)
    azimuthal = Field('Azimuthal Angle', basic.FixedPoint16_7)

class BeamWidths(Struct):
    horizontal = Field('Horizontal', basic.FixedPoint16_7)
    vertical = Field('Vertical', basic.FixedPoint16_7)

class EbNoBER(Struct):
    # Energy per Bit to Noise Density ratio (Eb/No)
    # Maximum positive value (0x7FFF) signifies "unused"
    ebno = Field('Eb/No', basic.FixedPoint16_7)

    # Bit Error Rate (BER)
    # Maximum positive value (0x7FFF) signifies "unused"
    ber = Field('BER', basic.FixedPoint16_7)

class Threshold(Struct):
    stage1 = Field('Stage 1', basic.FixedPoint16_7)
    stage2 = Field('Stage 2', basic.FixedPoint16_7)

class InterceptPoints(Struct):
    second_order = Field('Input 2nd Order Intercept', basic.FixedPoint16_7)
    third_order = Field('Input 3rd Order Intercept', basic.FixedPoint16_7)

class SNRNoise(Struct):
    # Signal-to-Noise Ratio (dB)
    # Maximum positive value (0x7FFF) signifies "unused"
    snr = Field('SNR', basic.FixedPoint16_7)

    # Noise Figure (db)
    # O signifies "unused"
    noise = Field('Noise Figure', basic.FixedPoint16_7)

class HealthStatus(Struct):
    reserved = Reserved(16)
    identifier = Field('Identifier', basic.Identifier16)

class VersionInformation(Struct):
    year = Field('Year', basic.IntegerType.create(7))
    day = Field('Day', basic.IntegerType.create(9))
    revision = Field('Revision', basic.IntegerType.create(6))
    # Remaining 10 bits are user-defined, marking as reserved for now
    user = Reserved(10)

class BufferSize(Struct):
    buffer_size = Field('Buffer Size', basic.Integer32)
    reserved = Reserved(16)
    level = Field('Level', basic.Integer8)
    status = Field('Status', basic.Integer8)
