# Copyright (C) 2020 Geon Technologies, LLC
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
Data packet parser classes.
"""

from vrtgen.parser.section import SectionParser
from vrtgen.types.prologue import DataHeader
from vrtgen.types.trailer import Trailer

from .base import PacketParser, unimplemented_parser

class TrailerParser(SectionParser):
    """
    Parser for Data Packet Trailer configuration.
    """

TrailerParser.add_field_parser(Trailer.calibrated_time, alias='calibrated-time')
TrailerParser.add_field_parser(Trailer.valid_data, alias='valid-data')
TrailerParser.add_field_parser(Trailer.reference_lock, alias='reference-lock')
TrailerParser.add_field_parser(Trailer.agc_mgc, alias='agc-mgc')
TrailerParser.add_field_parser(Trailer.detected_signal, alias='detected-signal')
TrailerParser.add_field_parser(Trailer.spectral_inversion, alias='spectral-inversion')
TrailerParser.add_field_parser(Trailer.over_range, alias='over-range')
TrailerParser.add_field_parser(Trailer.sample_loss, alias='sample-loss')
TrailerParser.add_field_parser(Trailer.sample_frame, alias='sample-frame')
TrailerParser.add_parser(Trailer.user_defined.name, unimplemented_parser('User-defined bits'))
TrailerParser.add_field_parser(Trailer.associated_context_packets, alias='associated-context')

class DataPacketParser(PacketParser):
    """
    Parser for Data Packet configuration.
    """

DataPacketParser.add_field_value_parser(DataHeader.not_v49d0, alias='not-v49d0')
DataPacketParser.add_field_value_parser(DataHeader.spectrum, alias='spectrum')
DataPacketParser.add_parser('trailer', TrailerParser())
