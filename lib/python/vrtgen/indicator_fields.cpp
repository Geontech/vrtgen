#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "vrtgen/packing/indicator_fields.hpp"

void init_indicator_fields(py::module_& mod)
{
    using namespace vrtgen::packing;
    py::class_<IndicatorField>(mod, "IndicatorField")
        .def(py::init<>())
        .def("any", &IndicatorField::any)
        .def("none", &IndicatorField::none);

    py::class_<IndicatorField0, IndicatorField>(mod, "IndicatorField0")
        .def(py::init<>())
        .def_property(
            "change_indicator", 
            [](const IndicatorField0& self) { return self.change_indicator(); },
            [](IndicatorField0& self, bool value) { return self.change_indicator(value); }
        )
        .def_property(
            "reference_point_id", 
            [](const IndicatorField0& self) { return self.reference_point_id(); },
            [](IndicatorField0& self, bool value) { return self.reference_point_id(value); }
        )
        .def_property(
            "bandwidth", 
            [](const IndicatorField0& self) { return self.bandwidth(); },
            [](IndicatorField0& self, bool value) { return self.bandwidth(value); }
        )
        .def_property(
            "if_ref_frequency", 
            [](const IndicatorField0& self) { return self.if_ref_frequency(); },
            [](IndicatorField0& self, bool value) { return self.if_ref_frequency(value); }
        )
        .def_property(
            "rf_ref_frequency", 
            [](const IndicatorField0& self) { return self.rf_ref_frequency(); },
            [](IndicatorField0& self, bool value) { return self.rf_ref_frequency(value); }
        )
        .def_property(
            "rf_ref_frequency_offset", 
            [](const IndicatorField0& self) { return self.rf_ref_frequency_offset(); },
            [](IndicatorField0& self, bool value) { return self.rf_ref_frequency_offset(value); }
        )
        .def_property(
            "if_band_offset", 
            [](const IndicatorField0& self) { return self.if_band_offset(); },
            [](IndicatorField0& self, bool value) { return self.if_band_offset(value); }
        )
        .def_property(
            "reference_level", 
            [](const IndicatorField0& self) { return self.reference_level(); },
            [](IndicatorField0& self, bool value) { return self.reference_level(value); }
        )
        .def_property(
            "gain", 
            [](const IndicatorField0& self) { return self.gain(); },
            [](IndicatorField0& self, bool value) { return self.gain(value); }
        )
        .def_property(
            "over_range_count", 
            [](const IndicatorField0& self) { return self.over_range_count(); },
            [](IndicatorField0& self, bool value) { return self.over_range_count(value); }
        )
        .def_property(
            "sample_rate", 
            [](const IndicatorField0& self) { return self.sample_rate(); },
            [](IndicatorField0& self, bool value) { return self.sample_rate(value); }
        )
        .def_property(
            "timestamp_adjustment", 
            [](const IndicatorField0& self) { return self.timestamp_adjustment(); },
            [](IndicatorField0& self, bool value) { return self.timestamp_adjustment(value); }
        )
        .def_property(
            "timestamp_calibration_time", 
            [](const IndicatorField0& self) { return self.timestamp_calibration_time(); },
            [](IndicatorField0& self, bool value) { return self.timestamp_calibration_time(value); }
        )
        .def_property(
            "temperature", 
            [](const IndicatorField0& self) { return self.temperature(); },
            [](IndicatorField0& self, bool value) { return self.temperature(value); }
        )
        .def_property(
            "device_id", 
            [](const IndicatorField0& self) { return self.device_id(); },
            [](IndicatorField0& self, bool value) { return self.device_id(value); }
        )
        .def_property(
            "state_event_indicators", 
            [](const IndicatorField0& self) { return self.device_id(); },
            [](IndicatorField0& self, bool value) { return self.device_id(value); }
        )
        .def_property(
            "signal_data_format", 
            [](const IndicatorField0& self) { return self.device_id(); },
            [](IndicatorField0& self, bool value) { return self.device_id(value); }
        )
        .def_property(
            "formatted_gps", 
            [](const IndicatorField0& self) { return self.device_id(); },
            [](IndicatorField0& self, bool value) { return self.device_id(value); }
        )
        .def_property(
            "formatted_ins", 
            [](const IndicatorField0& self) { return self.device_id(); },
            [](IndicatorField0& self, bool value) { return self.device_id(value); }
        )
        .def_property(
            "ecef_ephemeris", 
            [](const IndicatorField0& self) { return self.device_id(); },
            [](IndicatorField0& self, bool value) { return self.device_id(value); }
        )
        .def_property(
            "relative_ephemeris", 
            [](const IndicatorField0& self) { return self.device_id(); },
            [](IndicatorField0& self, bool value) { return self.device_id(value); }
        )
        .def_property(
            "ephemeris_ref_id", 
            [](const IndicatorField0& self) { return self.device_id(); },
            [](IndicatorField0& self, bool value) { return self.device_id(value); }
        )
        .def_property(
            "gps_ascii", 
            [](const IndicatorField0& self) { return self.device_id(); },
            [](IndicatorField0& self, bool value) { return self.device_id(value); }
        )
        .def_property(
            "context_association_lists", 
            [](const IndicatorField0& self) { return self.device_id(); },
            [](IndicatorField0& self, bool value) { return self.device_id(value); }
        )
        .def_property(
            "relative_ephemeris", 
            [](const IndicatorField0& self) { return self.device_id(); },
            [](IndicatorField0& self, bool value) { return self.device_id(value); }
        )
        .def_property(
            "relative_ephemeris", 
            [](const IndicatorField0& self) { return self.device_id(); },
            [](IndicatorField0& self, bool value) { return self.device_id(value); }
        )
        .def_property(
            "relative_ephemeris", 
            [](const IndicatorField0& self) { return self.device_id(); },
            [](IndicatorField0& self, bool value) { return self.device_id(value); }
        )
        .def_property(
            "relative_ephemeris", 
            [](const IndicatorField0& self) { return self.device_id(); },
            [](IndicatorField0& self, bool value) { return self.device_id(value); }
        );

    py::class_<IndicatorField1, IndicatorField>(mod, "IndicatorField1")
        .def(py::init<>())
        .def_property(
            "phase_offset", 
            [](const IndicatorField1& self) { return self.phase_offset(); },
            [](IndicatorField1& self, bool value) { return self.phase_offset(value); }
        )
        .def_property(
            "polarization", 
            [](const IndicatorField1& self) { return self.polarization(); },
            [](IndicatorField1& self, bool value) { return self.polarization(value); }
        )
        .def_property(
            "pointing_vector", 
            [](const IndicatorField1& self) { return self.pointing_vector(); },
            [](IndicatorField1& self, bool value) { return self.pointing_vector(value); }
        )
        .def_property(
            "pointing_vector_structure", 
            [](const IndicatorField1& self) { return self.pointing_vector_structure(); },
            [](IndicatorField1& self, bool value) { return self.pointing_vector_structure(value); }
        )
        .def_property(
            "spatial_scan_type", 
            [](const IndicatorField1& self) { return self.spatial_scan_type(); },
            [](IndicatorField1& self, bool value) { return self.spatial_scan_type(value); }
        )
        .def_property(
            "spatial_reference_type", 
            [](const IndicatorField1& self) { return self.spatial_reference_type(); },
            [](IndicatorField1& self, bool value) { return self.spatial_reference_type(value); }
        )
        .def_property(
            "beam_width", 
            [](const IndicatorField1& self) { return self.beam_width(); },
            [](IndicatorField1& self, bool value) { return self.beam_width(value); }
        )
        .def_property(
            "range", 
            [](const IndicatorField1& self) { return self.range(); },
            [](IndicatorField1& self, bool value) { return self.range(value); }
        )
        .def_property(
            "ebno_ber", 
            [](const IndicatorField1& self) { return self.ebno_ber(); },
            [](IndicatorField1& self, bool value) { return self.ebno_ber(value); }
        )
        .def_property(
            "threshold", 
            [](const IndicatorField1& self) { return self.threshold(); },
            [](IndicatorField1& self, bool value) { return self.threshold(value); }
        )
        .def_property(
            "compression_point", 
            [](const IndicatorField1& self) { return self.compression_point(); },
            [](IndicatorField1& self, bool value) { return self.compression_point(value); }
        )
        .def_property(
            "intercept_points", 
            [](const IndicatorField1& self) { return self.intercept_points(); },
            [](IndicatorField1& self, bool value) { return self.intercept_points(value); }
        )
        .def_property(
            "snr_noise_figure", 
            [](const IndicatorField1& self) { return self.snr_noise_figure(); },
            [](IndicatorField1& self, bool value) { return self.snr_noise_figure(value); }
        )
        .def_property(
            "aux_frequency", 
            [](const IndicatorField1& self) { return self.aux_frequency(); },
            [](IndicatorField1& self, bool value) { return self.aux_frequency(value); }
        )
        .def_property(
            "aux_gain", 
            [](const IndicatorField1& self) { return self.aux_gain(); },
            [](IndicatorField1& self, bool value) { return self.aux_gain(value); }
        )
        .def_property(
            "aux_bandwidth", 
            [](const IndicatorField1& self) { return self.aux_bandwidth(); },
            [](IndicatorField1& self, bool value) { return self.aux_bandwidth(value); }
        )
        .def_property(
            "array_of_cifs", 
            [](const IndicatorField1& self) { return self.array_of_cifs(); },
            [](IndicatorField1& self, bool value) { return self.array_of_cifs(value); }
        )
        .def_property(
            "spectrum", 
            [](const IndicatorField1& self) { return self.spectrum(); },
            [](IndicatorField1& self, bool value) { return self.spectrum(value); }
        )
        .def_property(
            "sector_step_scan", 
            [](const IndicatorField1& self) { return self.sector_step_scan(); },
            [](IndicatorField1& self, bool value) { return self.sector_step_scan(value); }
        )
        .def_property(
            "index_list", 
            [](const IndicatorField1& self) { return self.index_list(); },
            [](IndicatorField1& self, bool value) { return self.index_list(value); }
        )
        .def_property(
            "discrete_io_32", 
            [](const IndicatorField1& self) { return self.discrete_io_32(); },
            [](IndicatorField1& self, bool value) { return self.discrete_io_32(value); }
        )
        .def_property(
            "discrete_io_64", 
            [](const IndicatorField1& self) { return self.discrete_io_64(); },
            [](IndicatorField1& self, bool value) { return self.discrete_io_64(value); }
        )
        .def_property(
            "health_status", 
            [](const IndicatorField1& self) { return self.health_status(); },
            [](IndicatorField1& self, bool value) { return self.health_status(value); }
        )
        .def_property(
            "v49_spec_compliance", 
            [](const IndicatorField1& self) { return self.v49_spec_compliance(); },
            [](IndicatorField1& self, bool value) { return self.v49_spec_compliance(value); }
        )
        .def_property(
            "version_information", 
            [](const IndicatorField1& self) { return self.version_information(); },
            [](IndicatorField1& self, bool value) { return self.version_information(value); }
        )
        .def_property(
            "buffer_size", 
            [](const IndicatorField1& self) { return self.buffer_size(); },
            [](IndicatorField1& self, bool value) { return self.buffer_size(value); }
        );

    py::class_<IndicatorField2, IndicatorField>(mod, "IndicatorField2")
        .def(py::init<>())
        .def_property(
            "bind", 
            [](const IndicatorField2& self) { return self.bind(); },
            [](IndicatorField2& self, bool value) { return self.bind(value); }
        )
        .def_property(
            "cited_sid", 
            [](const IndicatorField2& self) { return self.cited_sid(); },
            [](IndicatorField2& self, bool value) { return self.cited_sid(value); }
        )
        .def_property(
            "sibling_sid", 
            [](const IndicatorField2& self) { return self.sibling_sid(); },
            [](IndicatorField2& self, bool value) { return self.sibling_sid(value); }
        )
        .def_property(
            "parent_sid", 
            [](const IndicatorField2& self) { return self.parent_sid(); },
            [](IndicatorField2& self, bool value) { return self.parent_sid(value); }
        )
        .def_property(
            "child_sid", 
            [](const IndicatorField2& self) { return self.child_sid(); },
            [](IndicatorField2& self, bool value) { return self.child_sid(value); }
        )
        .def_property(
            "cited_message_id", 
            [](const IndicatorField2& self) { return self.cited_message_id(); },
            [](IndicatorField2& self, bool value) { return self.cited_message_id(value); }
        )
        .def_property(
            "controllee_id", 
            [](const IndicatorField2& self) { return self.controllee_id(); },
            [](IndicatorField2& self, bool value) { return self.controllee_id(value); }
        )
        .def_property(
            "controllee_uuid", 
            [](const IndicatorField2& self) { return self.controllee_uuid(); },
            [](IndicatorField2& self, bool value) { return self.controllee_uuid(value); }
        )
        .def_property(
            "controller_id", 
            [](const IndicatorField2& self) { return self.controller_id(); },
            [](IndicatorField2& self, bool value) { return self.controller_id(value); }
        )
        .def_property(
            "controller_uuid", 
            [](const IndicatorField2& self) { return self.controller_uuid(); },
            [](IndicatorField2& self, bool value) { return self.controller_uuid(value); }
        )
        .def_property(
            "information_source", 
            [](const IndicatorField2& self) { return self.information_source(); },
            [](IndicatorField2& self, bool value) { return self.information_source(value); }
        )
        .def_property(
            "track_id", 
            [](const IndicatorField2& self) { return self.track_id(); },
            [](IndicatorField2& self, bool value) { return self.track_id(value); }
        )
        .def_property(
            "country_code", 
            [](const IndicatorField2& self) { return self.country_code(); },
            [](IndicatorField2& self, bool value) { return self.country_code(value); }
        )
        .def_property(
            "operator_id", 
            [](const IndicatorField2& self) { return self.operator_id(); },
            [](IndicatorField2& self, bool value) { return self.operator_id(value); }
        )
        .def_property(
            "platform_class", 
            [](const IndicatorField2& self) { return self.platform_class(); },
            [](IndicatorField2& self, bool value) { return self.platform_class(value); }
        )
        .def_property(
            "platform_instance", 
            [](const IndicatorField2& self) { return self.platform_instance(); },
            [](IndicatorField2& self, bool value) { return self.platform_instance(value); }
        )
        .def_property(
            "platform_display", 
            [](const IndicatorField2& self) { return self.platform_display(); },
            [](IndicatorField2& self, bool value) { return self.platform_display(value); }
        )
        .def_property(
            "ems_device_class", 
            [](const IndicatorField2& self) { return self.ems_device_class(); },
            [](IndicatorField2& self, bool value) { return self.ems_device_class(value); }
        )
        .def_property(
            "ems_device_type", 
            [](const IndicatorField2& self) { return self.ems_device_type(); },
            [](IndicatorField2& self, bool value) { return self.ems_device_type(value); }
        )
        .def_property(
            "ems_device_instance", 
            [](const IndicatorField2& self) { return self.ems_device_instance(); },
            [](IndicatorField2& self, bool value) { return self.ems_device_instance(value); }
        )
        .def_property(
            "modulation_class", 
            [](const IndicatorField2& self) { return self.modulation_class(); },
            [](IndicatorField2& self, bool value) { return self.modulation_class(value); }
        )
        .def_property(
            "modulation_type", 
            [](const IndicatorField2& self) { return self.modulation_type(); },
            [](IndicatorField2& self, bool value) { return self.modulation_type(value); }
        )
        .def_property(
            "function_id", 
            [](const IndicatorField2& self) { return self.function_id(); },
            [](IndicatorField2& self, bool value) { return self.function_id(value); }
        )
        .def_property(
            "mode_id", 
            [](const IndicatorField2& self) { return self.mode_id(); },
            [](IndicatorField2& self, bool value) { return self.mode_id(value); }
        )
        .def_property(
            "event_id", 
            [](const IndicatorField2& self) { return self.event_id(); },
            [](IndicatorField2& self, bool value) { return self.event_id(value); }
        )
        .def_property(
            "function_priority_id", 
            [](const IndicatorField2& self) { return self.function_priority_id(); },
            [](IndicatorField2& self, bool value) { return self.function_priority_id(value); }
        )
        .def_property(
            "communication_priority_id", 
            [](const IndicatorField2& self) { return self.communication_priority_id(); },
            [](IndicatorField2& self, bool value) { return self.communication_priority_id(value); }
        )
        .def_property(
            "rf_footprint", 
            [](const IndicatorField2& self) { return self.rf_footprint(); },
            [](IndicatorField2& self, bool value) { return self.rf_footprint(value); }
        )
        .def_property(
            "rf_footprint_range", 
            [](const IndicatorField2& self) { return self.rf_footprint_range(); },
            [](IndicatorField2& self, bool value) { return self.rf_footprint_range(value); }
        );

    py::class_<IndicatorField3, IndicatorField>(mod, "IndicatorField3")
        .def(py::init<>())
        .def_property(
            "timestamp_details", 
            [](const IndicatorField3& self) { return self.timestamp_details(); },
            [](IndicatorField3& self, bool value) { return self.timestamp_details(value); }
        )
        .def_property(
            "timestamp_skew", 
            [](const IndicatorField3& self) { return self.timestamp_skew(); },
            [](IndicatorField3& self, bool value) { return self.timestamp_skew(value); }
        )
        .def_property(
            "rise_time", 
            [](const IndicatorField3& self) { return self.rise_time(); },
            [](IndicatorField3& self, bool value) { return self.rise_time(value); }
        )
        .def_property(
            "fall_time", 
            [](const IndicatorField3& self) { return self.fall_time(); },
            [](IndicatorField3& self, bool value) { return self.fall_time(value); }
        )
        .def_property(
            "offset_time", 
            [](const IndicatorField3& self) { return self.offset_time(); },
            [](IndicatorField3& self, bool value) { return self.offset_time(value); }
        )
        .def_property(
            "pulse_width", 
            [](const IndicatorField3& self) { return self.pulse_width(); },
            [](IndicatorField3& self, bool value) { return self.pulse_width(value); }
        )
        .def_property(
            "period", 
            [](const IndicatorField3& self) { return self.period(); },
            [](IndicatorField3& self, bool value) { return self.period(value); }
        )
        .def_property(
            "duration", 
            [](const IndicatorField3& self) { return self.duration(); },
            [](IndicatorField3& self, bool value) { return self.duration(value); }
        )
        .def_property(
            "dwell", 
            [](const IndicatorField3& self) { return self.dwell(); },
            [](IndicatorField3& self, bool value) { return self.dwell(value); }
        )
        .def_property(
            "jitter", 
            [](const IndicatorField3& self) { return self.jitter(); },
            [](IndicatorField3& self, bool value) { return self.jitter(value); }
        )
        .def_property(
            "age", 
            [](const IndicatorField3& self) { return self.age(); },
            [](IndicatorField3& self, bool value) { return self.age(value); }
        )
        .def_property(
            "shelf_life", 
            [](const IndicatorField3& self) { return self.shelf_life(); },
            [](IndicatorField3& self, bool value) { return self.shelf_life(value); }
        )
        .def_property(
            "air_temperature", 
            [](const IndicatorField3& self) { return self.air_temperature(); },
            [](IndicatorField3& self, bool value) { return self.air_temperature(value); }
        )
        .def_property(
            "sea_ground_temperature", 
            [](const IndicatorField3& self) { return self.sea_ground_temperature(); },
            [](IndicatorField3& self, bool value) { return self.sea_ground_temperature(value); }
        )
        .def_property(
            "humidity", 
            [](const IndicatorField3& self) { return self.humidity(); },
            [](IndicatorField3& self, bool value) { return self.humidity(value); }
        )
        .def_property(
            "barometric_pressure", 
            [](const IndicatorField3& self) { return self.barometric_pressure(); },
            [](IndicatorField3& self, bool value) { return self.barometric_pressure(value); }
        )
        .def_property(
            "sea_swell_state", 
            [](const IndicatorField3& self) { return self.sea_swell_state(); },
            [](IndicatorField3& self, bool value) { return self.sea_swell_state(value); }
        )
        .def_property(
            "tropospheric_state", 
            [](const IndicatorField3& self) { return self.sea_swell_state(); },
            [](IndicatorField3& self, bool value) { return self.sea_swell_state(value); }
        )
        .def_property(
            "network_id", 
            [](const IndicatorField3& self) { return self.sea_swell_state(); },
            [](IndicatorField3& self, bool value) { return self.sea_swell_state(value); }
        );

    py::class_<IndicatorField7, IndicatorField>(mod, "IndicatorField7")
        .def(py::init<>())
        .def_property(
            "current_value", 
            [](const IndicatorField7& self) { return self.current_value(); },
            [](IndicatorField7& self, bool value) { return self.current_value(value); }
        )
        .def_property(
            "mean_value", 
            [](const IndicatorField7& self) { return self.mean_value(); },
            [](IndicatorField7& self, bool value) { return self.mean_value(value); }
        )
        .def_property(
            "median_value", 
            [](const IndicatorField7& self) { return self.median_value(); },
            [](IndicatorField7& self, bool value) { return self.median_value(value); }
        )
        .def_property(
            "standard_deviation", 
            [](const IndicatorField7& self) { return self.standard_deviation(); },
            [](IndicatorField7& self, bool value) { return self.standard_deviation(value); }
        )
        .def_property(
            "max_value", 
            [](const IndicatorField7& self) { return self.max_value(); },
            [](IndicatorField7& self, bool value) { return self.max_value(value); }
        )
        .def_property(
            "min_value", 
            [](const IndicatorField7& self) { return self.min_value(); },
            [](IndicatorField7& self, bool value) { return self.min_value(value); }
        )
        .def_property(
            "precision", 
            [](const IndicatorField7& self) { return self.precision(); },
            [](IndicatorField7& self, bool value) { return self.precision(value); }
        )
        .def_property(
            "accuracy", 
            [](const IndicatorField7& self) { return self.accuracy(); },
            [](IndicatorField7& self, bool value) { return self.accuracy(value); }
        )
        .def_property(
            "first_derivative", 
            [](const IndicatorField7& self) { return self.first_derivative(); },
            [](IndicatorField7& self, bool value) { return self.first_derivative(value); }
        )
        .def_property(
            "second_derivative", 
            [](const IndicatorField7& self) { return self.second_derivative(); },
            [](IndicatorField7& self, bool value) { return self.second_derivative(value); }
        )
        .def_property(
            "third_derivative", 
            [](const IndicatorField7& self) { return self.third_derivative(); },
            [](IndicatorField7& self, bool value) { return self.third_derivative(value); }
        )
        .def_property(
            "probability", 
            [](const IndicatorField7& self) { return self.probability(); },
            [](IndicatorField7& self, bool value) { return self.probability(value); }
        )
        .def_property(
            "belief", 
            [](const IndicatorField7& self) { return self.belief(); },
            [](IndicatorField7& self, bool value) { return self.belief(value); }
        );
}
