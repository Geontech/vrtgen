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
Parser classes for CIF fields.
"""

from vrtgen.parser.field import IndexListParser
from vrtgen.parser.user import UserDefinedFieldParser
from vrtgen.types.cif0 import CIF0
from vrtgen.types.cif1 import CIF1

from .base import PacketParser

class CIFPacketParser(PacketParser):
    """
    Base parser for configuring packets that contain CIF fields.
    """

CIFPacketParser.add_field_parser(CIF0.reference_point_id)
CIFPacketParser.add_field_parser(CIF0.bandwidth)
CIFPacketParser.add_field_parser(CIF0.if_frequency)
CIFPacketParser.add_field_parser(CIF0.rf_frequency)
CIFPacketParser.add_field_parser(CIF0.rf_frequency_offset)
CIFPacketParser.add_field_parser(CIF0.if_band_offset)
CIFPacketParser.add_field_parser(CIF0.reference_level)
CIFPacketParser.add_field_parser(CIF0.gain)
CIFPacketParser.add_field_parser(CIF0.over_range_count)
CIFPacketParser.add_field_parser(CIF0.sample_rate)
CIFPacketParser.add_field_parser(CIF0.timestamp_calibration_time)
CIFPacketParser.add_field_parser(CIF0.temperature)
CIFPacketParser.add_field_parser(CIF0.device_id)
# Not implemented: CIF0.state_event_indicators
CIFPacketParser.add_field_parser(CIF0.data_format, alias='Data Payload Format')
CIFPacketParser.add_field_parser(CIF0.formatted_gps)
CIFPacketParser.add_field_parser(CIF0.formatted_ins)
CIFPacketParser.add_field_parser(CIF0.ecef_ephemeris)
CIFPacketParser.add_field_parser(CIF0.relative_ephemeris)
CIFPacketParser.add_field_parser(CIF0.ephemeris_ref_id)
# Not implemented: CIF0.gps_ascii
# Not implemented: CIF0.context_association_lists

CIFPacketParser.add_field_parser(CIF1.phase_offset)
CIFPacketParser.add_field_parser(CIF1.polarization)
CIFPacketParser.add_field_parser(CIF1.pointing_vector)
CIFPacketParser.add_field_parser(CIF1.spatial_scan_type)
CIFPacketParser.add_field_parser(CIF1.beam_widths)
CIFPacketParser.add_field_parser(CIF1.range)
CIFPacketParser.add_field_parser(CIF1.ebno_ber)
CIFPacketParser.add_field_parser(CIF1.threshold)
CIFPacketParser.add_field_parser(CIF1.compression_point)
CIFPacketParser.add_field_parser(CIF1.snr_noise_figure)
CIFPacketParser.add_field_parser(CIF1.aux_frequency)
CIFPacketParser.add_field_parser(CIF1.aux_gain)
CIFPacketParser.add_field_parser(CIF1.aux_bandwidth)
CIFPacketParser.add_parser(CIF1.index_list.name, IndexListParser())
CIFPacketParser.add_field_parser(CIF1.discrete_io_32, UserDefinedFieldParser())
CIFPacketParser.add_field_parser(CIF1.discrete_io_64, UserDefinedFieldParser())
CIFPacketParser.add_field_parser(CIF1.health_status)
CIFPacketParser.add_field_parser(CIF1.version_build_code)
CIFPacketParser.add_field_parser(CIF1.buffer_size)
