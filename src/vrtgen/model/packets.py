from vrtgen.types.struct import Struct, Field
from vrtgen.types import basic
from vrtgen.types import enums

class DataTrailer(Struct):
    calibrated_time = Field('Calibrated Time', basic.Boolean)#, 31, 19)
    valid_data = Field('Valid Data', basic.Boolean)#, 30, 18)
    reference_lock = Field('Reference Lock', basic.Boolean)#, 29, 17)
    agc_mgc = Field('AGC/MGC', basic.Boolean)#, 28, 16)
    detected_signal = Field('Detected Signal', basic.Boolean)#, 27, 15)
    spectral_inversion = Field('Spectral Inversion', basic.Boolean)#, 26, 14)
    over_range = Field('Over-range', basic.Boolean)#, 25, 13)
    sample_loss = Field('Sample Loss', basic.Boolean)#, 24, 12)
    # The Sample Frame field was added in V49.2, replacing 2 user-defined
    # bits. While the bits can still be user-defined for compatibility with
    # V49.0 implementations, the spec strongly discourages it, and it is
    # not supported here.
    sample_frame = Field('Sample Frame', enums.SSI)#, 23, 11)
    # [21..20], [9..8] User-Defined
    context_packet_count = Field('Associated Context Packet Count', basic.IntegerType.create(7))
