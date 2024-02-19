#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include "vrtgen/packing/cif0.hpp"

void init_cif0(py::module_& mod)
{
    using namespace vrtgen::packing;
    py::class_<CIF0, IndicatorField0>(mod, "CIF0")
        .def(py::init<>())
        .def_property(
            "cif7_enable",
            [](const CIF0& self) { return self.cif7_enable(); },
            [](CIF0& self, bool value) { return self.cif7_enable(value); }
        )
        .def_property(
            "cif3_enable",
            [](const CIF0& self) { return self.cif3_enable(); },
            [](CIF0& self, bool value) { return self.cif3_enable(value); }
        )
        .def_property(
            "cif2_enable",
            [](const CIF0& self) { return self.cif2_enable(); },
            [](CIF0& self, bool value) { return self.cif2_enable(value); }
        )
        .def_property(
            "cif1_enable",
            [](const CIF0& self) { return self.cif1_enable(); },
            [](CIF0& self, bool value) { return self.cif1_enable(value); }
        );

    py::class_<Gain>(mod, "Gain")
        .def(py::init<>())
        .def_property(
            "stage_2",
            [](const Gain& self) { return self.stage_2(); },
            [](Gain& self, double value) { return self.stage_2(value); }
        )
        .def_property(
            "stage_1",
            [](const Gain& self) { return self.stage_1(); },
            [](Gain& self, double value) { return self.stage_1(value); }
        );

    py::class_<DeviceIdentifier>(mod, "DeviceIdentifier")
        .def(py::init<>())
        .def_property(
            "manufacturer_oui",
            [](const DeviceIdentifier& self) { return self.manufacturer_oui(); },
            [](DeviceIdentifier& self, uint32_t value) { return self.manufacturer_oui(value); }
        )
        .def_property(
            "device_code",
            [](const DeviceIdentifier& self) { return self.device_code(); },
            [](DeviceIdentifier& self, uint16_t value) { return self.device_code(value); }
        );

    py::class_<Geolocation>(mod, "Geolocation")
        .def(py::init<>())
        .def_property(
            "tsi",
            [](const Geolocation& self) { return self.tsi(); },
            [](Geolocation& self, TSI value) { return self.tsi(value); }
        )
        .def_property(
            "tsf",
            [](const Geolocation& self) { return self.tsf(); },
            [](Geolocation& self, TSF value) { return self.tsf(value); }
        )
        .def_property(
            "manufacturer_oui",
            [](const Geolocation& self) { return self.manufacturer_oui(); },
            [](Geolocation& self, uint32_t value) { return self.manufacturer_oui(value); }
        )
        .def_property(
            "integer_timestamp",
            [](const Geolocation& self) { return self.integer_timestamp(); },
            [](Geolocation& self, uint32_t value) { return self.integer_timestamp(value); }
        )
        .def_property(
            "fractional_timestamp",
            [](const Geolocation& self) { return self.fractional_timestamp(); },
            [](Geolocation& self, uint64_t value) { return self.fractional_timestamp(value); }
        )
        .def_property(
            "latitude",
            [](const Geolocation& self) { return self.latitude(); },
            [](Geolocation& self, double value) { return self.latitude(value); }
        )
        .def_property(
            "longitude",
            [](const Geolocation& self) { return self.longitude(); },
            [](Geolocation& self, double value) { return self.longitude(value); }
        )
        .def_property(
            "altitude",
            [](const Geolocation& self) { return self.altitude(); },
            [](Geolocation& self, double value) { return self.altitude(value); }
        )
        .def_property(
            "speed_over_ground",
            [](const Geolocation& self) { return self.speed_over_ground(); },
            [](Geolocation& self, double value) { return self.speed_over_ground(value); }
        )
        .def_property(
            "heading_angle",
            [](const Geolocation& self) { return self.heading_angle(); },
            [](Geolocation& self, double value) { return self.heading_angle(value); }
        )
        .def_property(
            "track_angle",
            [](const Geolocation& self) { return self.track_angle(); },
            [](Geolocation& self, double value) { return self.track_angle(value); }
        )
        .def_property(
            "magnetic_variation",
            [](const Geolocation& self) { return self.magnetic_variation(); },
            [](Geolocation& self, double value) { return self.magnetic_variation(value); }
        );

    py::class_<Ephemeris>(mod, "Ephemeris")
        .def(py::init<>())
        .def_property(
            "tsi",
            [](const Ephemeris& self) { return self.tsi(); },
            [](Ephemeris& self, TSI value) { return self.tsi(value); }
        )
        .def_property(
            "tsf",
            [](const Ephemeris& self) { return self.tsf(); },
            [](Ephemeris& self, TSF value) { return self.tsf(value); }
        )
        .def_property(
            "manufacturer_oui",
            [](const Ephemeris& self) { return self.manufacturer_oui(); },
            [](Ephemeris& self, uint32_t value) { return self.manufacturer_oui(value); }
        )
        .def_property(
            "integer_timestamp",
            [](const Ephemeris& self) { return self.integer_timestamp(); },
            [](Ephemeris& self, uint32_t value) { return self.integer_timestamp(value); }
        )
        .def_property(
            "fractional_timestamp",
            [](const Ephemeris& self) { return self.fractional_timestamp(); },
            [](Ephemeris& self, uint64_t value) { return self.fractional_timestamp(value); }
        )
        .def_property(
            "position_x",
            [](const Ephemeris& self) { return self.position_x(); },
            [](Ephemeris& self, double value) { return self.position_x(value); }
        )
        .def_property(
            "position_y",
            [](const Ephemeris& self) { return self.position_y(); },
            [](Ephemeris& self, double value) { return self.position_y(value); }
        )
        .def_property(
            "position_z",
            [](const Ephemeris& self) { return self.position_z(); },
            [](Ephemeris& self, double value) { return self.position_z(value); }
        )
        .def_property(
            "attitude_alpha",
            [](const Ephemeris& self) { return self.attitude_alpha(); },
            [](Ephemeris& self, double value) { return self.attitude_alpha(value); }
        )
        .def_property(
            "attitude_beta",
            [](const Ephemeris& self) { return self.attitude_beta(); },
            [](Ephemeris& self, double value) { return self.attitude_beta(value); }
        )
        .def_property(
            "attitude_phi",
            [](const Ephemeris& self) { return self.attitude_phi(); },
            [](Ephemeris& self, double value) { return self.attitude_phi(value); }
        )
        .def_property(
            "velocity_dx",
            [](const Ephemeris& self) { return self.velocity_dx(); },
            [](Ephemeris& self, double value) { return self.velocity_dx(value); }
        )
        .def_property(
            "velocity_dy",
            [](const Ephemeris& self) { return self.velocity_dy(); },
            [](Ephemeris& self, double value) { return self.velocity_dy(value); }
        )
        .def_property(
            "velocity_dz",
            [](const Ephemeris& self) { return self.velocity_dz(); },
            [](Ephemeris& self, double value) { return self.velocity_dz(value); }
        );

    py::class_<GpsAscii>(mod, "GpsAscii")
        .def(py::init<>())
        .def_property(
            "manufacturer_oui",
            [](const GpsAscii& self) { return self.manufacturer_oui(); },
            [](GpsAscii& self, uint32_t value) { return self.manufacturer_oui(value); }
        )
        .def_property_readonly(
            "number_of_words",
            [](const GpsAscii& self) { return self.number_of_words(); }
        )
        .def_property(
            "ascii_sentences",
            [](GpsAscii& self) { return self.ascii_sentences(); },
            [](GpsAscii& self, const std::vector<uint8_t>& value) { return self.ascii_sentences(value); }
        );

    py::class_<PayloadFormat>(mod, "PayloadFormat")
        .def(py::init<>())
        .def_property(
            "packing_method",
            [](const PayloadFormat& self) { return self.packing_method(); },
            [](PayloadFormat& self, PackingMethod value) { return self.packing_method(value); }
        )
        .def_property(
            "real_complex_type",
            [](const PayloadFormat& self) { return self.real_complex_type(); },
            [](PayloadFormat& self, DataSampleType value) { return self.real_complex_type(value); }
        )
        .def_property(
            "data_item_format",
            [](const PayloadFormat& self) { return self.data_item_format(); },
            [](PayloadFormat& self, DataItemFormat value) { return self.data_item_format(value); }
        )
        .def_property(
            "repeat_indicator",
            [](const PayloadFormat& self) { return self.repeat_indicator(); },
            [](PayloadFormat& self, bool value) { return self.repeat_indicator(value); }
        )
        .def_property(
            "event_tag_size",
            [](const PayloadFormat& self) { return self.event_tag_size(); },
            [](PayloadFormat& self, uint8_t value) { return self.event_tag_size(value); }
        )
        .def_property(
            "channel_tag_size",
            [](const PayloadFormat& self) { return self.channel_tag_size(); },
            [](PayloadFormat& self, uint8_t value) { return self.channel_tag_size(value); }
        )
        .def_property(
            "data_item_fraction_size",
            [](const PayloadFormat& self) { return self.data_item_fraction_size(); },
            [](PayloadFormat& self, uint8_t value) { return self.data_item_fraction_size(value); }
        )
        .def_property(
            "item_packing_field_size",
            [](const PayloadFormat& self) { return self.item_packing_field_size(); },
            [](PayloadFormat& self, uint8_t value) { return self.item_packing_field_size(value); }
        )
        .def_property(
            "data_item_size",
            [](const PayloadFormat& self) { return self.data_item_size(); },
            [](PayloadFormat& self, uint8_t value) { return self.data_item_size(value); }
        )
        .def_property(
            "repeat_count",
            [](const PayloadFormat& self) { return self.repeat_count(); },
            [](PayloadFormat& self, uint16_t value) { return self.repeat_count(value); }
        )
        .def_property(
            "vector_size",
            [](const PayloadFormat& self) { return self.vector_size(); },
            [](PayloadFormat& self, uint16_t value) { return self.vector_size(value); }
        );

    py::class_<ContextAssociationLists>(mod, "ContextAssociationLists")
        .def(py::init<>())
        .def_property(
            "source_list_size",
            [](const ContextAssociationLists& self) { return self.source_list_size(); },
            [](ContextAssociationLists& self, uint16_t value) { return self.source_list_size(value); }
        )
        .def_property(
            "system_list_size",
            [](const ContextAssociationLists& self) { return self.system_list_size(); },
            [](ContextAssociationLists& self, uint16_t value) { return self.system_list_size(value); }
        )
        .def_property(
            "vector_component_list_size",
            [](const ContextAssociationLists& self) { return self.vector_component_list_size(); },
            [](ContextAssociationLists& self, uint16_t value) { return self.vector_component_list_size(value); }
        )
        .def_property(
            "async_channel_tag_list_enable",
            [](const ContextAssociationLists& self) { return self.async_channel_tag_list_enable(); },
            [](ContextAssociationLists& self, bool value) { return self.async_channel_tag_list_enable(value); }
        )
        .def_property(
            "async_channel_list_size",
            [](const ContextAssociationLists& self) { return self.async_channel_list_size(); },
            [](ContextAssociationLists& self, uint16_t value) { return self.async_channel_list_size(value); }
        )
        .def_property(
            "source_list",
            [](ContextAssociationLists& self) { return self.source_list(); },
            [](ContextAssociationLists& self, const std::vector<uint32_t>& value) { return self.source_list(value); }
        )
        .def_property(
            "system_list",
            [](ContextAssociationLists& self) { return self.system_list(); },
            [](ContextAssociationLists& self, const std::vector<uint32_t>& value) { return self.system_list(value); }
        )
        .def_property(
            "vector_component_list",
            [](ContextAssociationLists& self) { return self.vector_component_list(); },
            [](ContextAssociationLists& self, const std::vector<uint32_t>& value) { return self.vector_component_list(value); }
        )
        .def_property(
            "async_channel_list",
            [](ContextAssociationLists& self) { return self.async_channel_list(); },
            [](ContextAssociationLists& self, const std::vector<uint32_t>& value) { return self.async_channel_list(value); }
        )
        .def_property(
            "async_channel_tag_list",
            [](ContextAssociationLists& self) { return self.async_channel_tag_list(); },
            [](ContextAssociationLists& self, const std::vector<uint32_t>& value) { return self.async_channel_tag_list(value); }
        );
}
