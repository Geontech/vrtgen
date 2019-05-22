"""
Types used for VITA 49 data packet trailers.
"""

from .struct import Struct, Field, Enable
from . import basic, enums

class Trailer(Struct):
    calibrated_time_enable = Enable('Calibrated Time Enable')
    valid_data_enable = Enable('Valid Data Enable')
    reference_lock_enable = Enable('Reference Lock Enable')
    agc_mgc_enable = Enable('AGC/MGC Enable')
    detected_signal_enable = Enable('Detected Signal Enable')
    spectral_inversion_enable = Enable('Spectral Inversion')
    over_range_enable = Enable('Over-range')
    sample_loss_enable = Enable('Sample Loss')
    sample_frame_enable = Enable('Sample Frame', 2)
    user_defined_enable = Enable('User-Defined', 2)
    calibrated_time = Field('Calibrated Time', basic.Boolean)
    valid_data = Field('Valid Data', basic.Boolean)
    reference_lock = Field('Reference Lock', basic.Boolean)
    agc_mgc = Field('AGC/MGC', enums.AGCMode)
    detected_signal = Field('Detected Signal', basic.Boolean)
    spectral_inversion = Field('Spectral Inversion', basic.Boolean)
    over_range = Field('Over-range', basic.Boolean)
    sample_loss = Field('Sample Loss', basic.Boolean)
    sample_frame = Field('Sample Frame', enums.SSI)
    user_defined = Field('User-Defined', basic.IntegerType.create(2))
    associated_context_packets_enable = Enable('Associated Context Packet Count Enable')
    associated_context_packets = Field('Associated Context Packet Count', basic.IntegerType.create(7, signed=False))
