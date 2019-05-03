"""
Types used for CIF1 fields.
"""
from . import basic
from .struct import Struct, Field, Reserved
from .cifmeta import ContextIndicatorFields
from .cif0 import Gain

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

CIF1Fields = ContextIndicatorFields('CIF1')

# Phase Offset (1/31): fixed-point 16/7, radians (upper 16 reserved)
CIF1Fields.phase_offset = Field('Phase Offset', basic.FixedPoint16_7)

# Polarization (1/30): [tilt, ellipticity], fixed-point 16/13, radians
CIF1Fields.polarization = Field('Polarization', Polarization)

# 3-D Pointing Vector (1/29): [elevation, azimuthal], fixed-point 16/7, degrees
CIF1Fields.pointing_vector = Field('3-D Pointing Vector', PointingVector)

# 3-D Pointing Vector Structure (1/28): structured data
CIF1Fields.pointing_vector_struct = Field('3-D Pointing Vector Structure', None) # not implemented

# Spatial Scan Type (1/27): Generic 16-bit identifier
CIF1Fields.spatial_scan_type = Field('Spatial Scan Type', basic.Identifier16)

# Spatial Reference Type (1/26): struct
CIF1Fields.spatial_reference_type = Field('Spatial Reference Type', None) # not implemented

# Beam Widths (1/25): [horizonal, vertical]: fixed-point 16/7. degrees
# NOTE: This field is referred to inconsistently in both singular and
# plural forms.
CIF1Fields.beam_widths = Field('Beam Widths', BeamWidths)

# Range (1/24): fixed-point 32/6, meters
CIF1Fields.range = Field('Range', basic.FixedPointType.create(32, 6))

# Reserved (1/23-21)
CIF1Fields.reserved_23 = Reserved(1)
CIF1Fields.reserved_22 = Reserved(1)
CIF1Fields.reserved_21 = Reserved(1)

# Eb/No BER (1/20): [Eb/No, BER], fixed-point 16/7, dB
CIF1Fields.ebno_ber = Field('Eb/No BER', EbNoBER)

# Threshold (1/18): [stage2 (optional), stage1], fixed-point 16/7, dB
CIF1Fields.threshold = Field('Threshold', Threshold)

# Compression Point (1/18): fixed-point 16/7, dBm
# NOTE: Also called "1 dB Compression Point" and "1-dB Compression Point"
CIF1Fields.compression_point = Field('Compression Point', basic.FixedPoint16_7)

# Intercept Points (1/17): [2IIP, 3IIP], fixed-point 16/7, dBm
CIF1Fields.intercept_points = Field('Intercept Points', InterceptPoints)

# SNR/Noise Figure (1/16): [SNR, Noise], fixed-point 16/7, dB
CIF1Fields.snr_noise_figure = Field('SNR/Noise Figure', SNRNoise)

# Aux Frequency (1/15): fixed-point 64/20, Hz
CIF1Fields.aux_frequency = Field('Aux Frequency', basic.FixedPoint64_20)

# Aux Gain (1/14): [stage2 (optional), stage1], fixed-point 16/7, dB
CIF1Fields.aux_gain = Field('Aux Gain', Gain)

# Aux Bandidth (1/13): fixed-point 64/20, Hz
CIF1Fields.aux_bandwidth = Field('Aux Bandwidth', basic.FixedPointType.create(64, 20))

# Reserved (1/12)
CIF1Fields.reserved_12 = Reserved(1)

# Array of CIFS (1/11): This allows multiple CIF blocks, wnich makes for
# some complex support code.
CIF1Fields.array_of_cifs = Field('Array of CIFS', None) # not implemented

# Spectrum (1/10): multi-word structured data
CIF1Fields.spectrum = Field('Spectrum', None) # not implemented

# Sector Scan/Step (1/9): Array of records, with the exact format of the
# records configurable. There is an overall header, much of which is run-
# time (e.g., number of records), with a bitfield to enable specific
# subfields.
CIF1Fields.sector_scan_step = Field('Sector Scan/Step', None) # not implemented

# Reserved (1/8)
CIF1Fields.reserved_8 = Reserved(1)

# Index List (1/7): Struct header with an array of identifiers (8, 16 or 32
# bits). Only the entry size is configurable for code generation.
CIF1Fields.index_list = Field('Index List', None)#IndexListOptions)

# Discrete I/O 32-bit (1/6): 32 additional bits of user-defined fields
CIF1Fields.discrete_io_32 = Field('Discrete I/O 32', basic.Integer32)

# Discrete I/O 64-bit (1/7): 64 additional bits of user-defined fields
CIF1Fields.discrete_io_64 = Field('Discrete I/O 64', basic.Integer64)

# Health Status (1/4): 16-bit identifier
CIF1Fields.health_status = Field('Health Status', HealthStatus)

# V49 Spec Compliance (1/3): 32 bits for V49 compliance level, only four
# values currently defined.
CIF1Fields.v49_spec_compliance = Field('V49 Spec Compliance', None) # not implmented

# Version and Build Code (1/2): struct, 32 bits
CIF1Fields.version_build_code = Field('Version and Build Code', VersionInformation)

# Buffer Size (1/1): struct, 64 bits
CIF1Fields.buffer_size = Field('Buffer Size', BufferSize)

# Reserved (1/0)
CIF1Fields.reserved_0 = Reserved(1)

CIF1Enables = CIF1Fields.create_enables()
