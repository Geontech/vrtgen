"""
Types used for VITA 49 data packet trailers.
"""

from .struct import Struct, Enable, Field
from . import basic, enums

class Trailer(Struct):
    """
    Signal Data Packet trailer (5.1.6).
    """
    calibrated_time_enable = Enable('Calibrated Time')
    valid_data_enable = Enable('Valid Data')
    reference_lock_enable = Enable('Reference Lock')
    agc_mgc_enable = Enable('AGC/MGC')
    detected_signal_enable = Enable('Detected Signal')
    spectral_inversion_enable = Enable('Spectral Inversion')
    over_range_enable = Enable('Over-range')
    sample_loss_enable = Enable('Sample Loss')
    sample_frame_enable = Enable('Sample Frame', 2)
    user_defined_enable = Enable('User-Defined', 2)
    calibrated_time = calibrated_time_enable.link(basic.Boolean)
    valid_data = valid_data_enable.link(basic.Boolean)
    reference_lock = reference_lock_enable.link(basic.Boolean)
    agc_mgc = agc_mgc_enable.link(enums.AGCMode)
    detected_signal = detected_signal_enable.link(basic.Boolean)
    spectral_inversion = spectral_inversion_enable.link(basic.Boolean)
    over_range = over_range_enable.link(basic.Boolean)
    sample_loss = sample_loss_enable.link(basic.Boolean)
    sample_frame = sample_frame_enable.link(enums.SSI)
    user_defined = user_defined_enable.link(basic.IntegerType.create(2))
    associated_context_packets_enable = Enable('Associated Context Packet Count')
    associated_context_packets = associated_context_packets_enable.link(
        basic.IntegerType.create(7, signed=False)
    )
