"""
Types used for VITA 49 data packet trailers.
"""

from .struct import Struct, Enable, Field
from . import basic, enums

class Trailer(Struct):
    """
    Signal Data Packet trailer (5.1.6).
    """
    calibrated_time_enable = Enable()
    valid_data_enable = Enable()
    reference_lock_enable = Enable()
    agc_mgc_enable = Enable()
    detected_signal_enable = Enable()
    spectral_inversion_enable = Enable()
    over_range_enable = Enable()
    sample_loss_enable = Enable()
    sample_frame_enable = Enable(2)
    user_defined_enable = Enable(2)
    calibrated_time = Field('Calibrated Time', basic.Boolean, enable=calibrated_time_enable)
    valid_data = Field('Valid Data', basic.Boolean, enable=valid_data_enable)
    reference_lock = Field('Reference Lock', basic.Boolean, enable=reference_lock_enable)
    agc_mgc = Field('AGC/MGC', enums.AGCMode, enable=agc_mgc_enable)
    detected_signal = Field('Detected Signal', basic.Boolean, enable=detected_signal_enable)
    spectral_inversion = Field(
        'Spectral Inversion', basic.Boolean, enable=spectral_inversion_enable
    )
    over_range = Field('Over-range', basic.Boolean, enable=over_range_enable)
    sample_loss = Field('Sample Loss', basic.Boolean, enable=sample_loss_enable)
    sample_frame = Field('Sample Frame', enums.SSI, enable=sample_frame_enable)
    user_defined = Field('User-Defined', basic.IntegerType.create(2), enable=user_defined_enable)
    associated_context_packets_enable = Enable()
    associated_context_packets = Field(
        'Associated Context Packet Count',
        basic.IntegerType.create(7, signed=False),
        enable=associated_context_packets_enable
    )
