"""
Parser classes for CIF fields.
"""

from vrtgen.types.cif0 import CIF0
from vrtgen.types.cif1 import CIF1

from .field import IndexListParser, UserDefinedFieldParser
from .section import SectionParser

class CIFPayloadParser(SectionParser):
    """
    Parses YAML configuration for CIF payload fields.
    """

CIFPayloadParser.add_field_parser(CIF0.reference_point_id)
CIFPayloadParser.add_field_parser(CIF0.bandwidth)
CIFPayloadParser.add_field_parser(CIF0.if_frequency)
CIFPayloadParser.add_field_parser(CIF0.rf_frequency)
CIFPayloadParser.add_field_parser(CIF0.rf_frequency_offset)
CIFPayloadParser.add_field_parser(CIF0.if_band_offset)
CIFPayloadParser.add_field_parser(CIF0.reference_level)
CIFPayloadParser.add_field_parser(CIF0.gain)
CIFPayloadParser.add_field_parser(CIF0.over_range_count)
CIFPayloadParser.add_field_parser(CIF0.sample_rate)
CIFPayloadParser.add_field_parser(CIF0.timestamp_calibration_time)
CIFPayloadParser.add_field_parser(CIF0.temperature)
CIFPayloadParser.add_field_parser(CIF0.device_id)
# Not implemented: CIF0.state_event_indicators
# Not implemented: CIF0.data_format
CIFPayloadParser.add_field_parser(CIF0.formatted_gps)
CIFPayloadParser.add_field_parser(CIF0.formatted_ins)
CIFPayloadParser.add_field_parser(CIF0.ecef_ephemeris)
CIFPayloadParser.add_field_parser(CIF0.relative_ephemeris)
CIFPayloadParser.add_field_parser(CIF0.ephemeris_ref_id)
# Not implemented: CIF0.gps_ascii
# Not implemented: CIF0.context_association_lists

CIFPayloadParser.add_field_parser(CIF1.phase_offset)
CIFPayloadParser.add_field_parser(CIF1.polarization)
CIFPayloadParser.add_field_parser(CIF1.pointing_vector)
CIFPayloadParser.add_field_parser(CIF1.spatial_scan_type)
CIFPayloadParser.add_field_parser(CIF1.beam_widths)
CIFPayloadParser.add_field_parser(CIF1.range)
CIFPayloadParser.add_field_parser(CIF1.ebno_ber)
CIFPayloadParser.add_field_parser(CIF1.threshold)
CIFPayloadParser.add_field_parser(CIF1.compression_point)
CIFPayloadParser.add_field_parser(CIF1.snr_noise_figure)
CIFPayloadParser.add_field_parser(CIF1.aux_frequency)
CIFPayloadParser.add_field_parser(CIF1.aux_gain)
CIFPayloadParser.add_field_parser(CIF1.aux_bandwidth)
CIFPayloadParser.add_parser(CIF1.index_list.name, IndexListParser())
CIFPayloadParser.add_parser(CIF1.discrete_io_32.name, UserDefinedFieldParser())
CIFPayloadParser.add_parser(CIF1.discrete_io_64.name, UserDefinedFieldParser())
CIFPayloadParser.add_field_parser(CIF1.health_status)
CIFPayloadParser.add_field_parser(CIF1.version_build_code)
CIFPayloadParser.add_field_parser(CIF1.buffer_size)
