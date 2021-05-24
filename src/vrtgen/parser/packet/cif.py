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

from vrtgen.parser.field import IndexListParser, UserDefinedFieldParser
from vrtgen.types.cif0 import CIF0
from vrtgen.types.cif1 import CIF1

from vrtgen.parser.section import SectionParser

class CIFParser(SectionParser):
    """
    Base parser for configuring packets that contain CIF fields.
    """

CIFParser.add_field_parser(CIF0.change_indicator, alias='change-indicator')
CIFParser.add_field_parser(CIF0.reference_point_id, alias='reference-point-id')
CIFParser.add_field_parser(CIF0.bandwidth, alias='bandwidth')
CIFParser.add_field_parser(CIF0.if_frequency, alias='if-reference-frequency')
CIFParser.add_field_parser(CIF0.rf_frequency, alias='rf-reference-frequency')
CIFParser.add_field_parser(CIF0.rf_frequency_offset, alias='rf-reference-frequency-offset')
CIFParser.add_field_parser(CIF0.if_band_offset, alias='if-band-offset')
CIFParser.add_field_parser(CIF0.reference_level, alias='reference-level')
CIFParser.add_field_parser(CIF0.gain, alias='gain')
CIFParser.add_field_parser(CIF0.over_range_count, alias='over-range-count')
CIFParser.add_field_parser(CIF0.sample_rate, alias='sample-rate')
CIFParser.add_field_parser(CIF0.timestamp_adjustment, alias='timestamp-adjustment')
CIFParser.add_field_parser(CIF0.timestamp_calibration_time, alias='timestamp-calibration-time')
CIFParser.add_field_parser(CIF0.temperature, alias='temperature')
CIFParser.add_field_parser(CIF0.device_id, alias='device-id')
# Not implemented: CIF0.state_event_indicators
CIFParser.add_field_parser(CIF0.data_format, alias='data-payload-format')
CIFParser.add_field_parser(CIF0.formatted_gps, alias='formatted-gps')
CIFParser.add_field_parser(CIF0.formatted_ins, alias='formatted-ins')
CIFParser.add_field_parser(CIF0.ecef_ephemeris, alias='ecef-ephemeris')
CIFParser.add_field_parser(CIF0.relative_ephemeris, alias='relative-ephemeris')
CIFParser.add_field_parser(CIF0.ephemeris_ref_id, alias='ephemeris-reference-id')
# Not implemented: CIF0.gps_ascii
# Not implemented: CIF0.context_association_lists

CIFParser.add_field_parser(CIF1.phase_offset, alias='phase-offset')
CIFParser.add_field_parser(CIF1.polarization, alias='polarization')
CIFParser.add_field_parser(CIF1.pointing_vector)
CIFParser.add_field_parser(CIF1.spatial_scan_type)
CIFParser.add_field_parser(CIF1.beam_widths)
CIFParser.add_field_parser(CIF1.range)
CIFParser.add_field_parser(CIF1.ebno_ber)
CIFParser.add_field_parser(CIF1.threshold)
CIFParser.add_field_parser(CIF1.compression_point)
CIFParser.add_field_parser(CIF1.snr_noise_figure)
CIFParser.add_field_parser(CIF1.aux_frequency)
CIFParser.add_field_parser(CIF1.aux_gain)
CIFParser.add_field_parser(CIF1.aux_bandwidth)
CIFParser.add_parser(CIF1.index_list.name, IndexListParser())
CIFParser.add_field_parser(CIF1.discrete_io_32, UserDefinedFieldParser(), alias='discrete-io-32')
CIFParser.add_field_parser(CIF1.discrete_io_64, UserDefinedFieldParser(), alias='discrete-io-64')
CIFParser.add_field_parser(CIF1.health_status, alias='health-status')
CIFParser.add_field_parser(CIF1.version_build_code)
CIFParser.add_field_parser(CIF1.buffer_size)
