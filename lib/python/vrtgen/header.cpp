#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "vrtgen/packing/header.hpp"

void init_header(py::module_& mod)
{
    using namespace vrtgen::packing;
    py::class_<Header>(mod, "Header")
        .def(py::init<>())
        .def_property(
            "packet_type", 
            [](const Header& self) { return self.packet_type(); },
            [](Header& self, PacketType value) { return self.packet_type(value); }
        )
        .def_property(
            "class_id_enable", 
            [](const Header& self) { return self.class_id_enable(); },
            [](Header& self, bool value) { return self.class_id_enable(value); }
        )
        .def_property(
            "tsi", 
            [](const Header& self) { return self.tsi(); },
            [](Header& self, TSI value) { return self.tsi(value); }
        )
        .def_property(
            "tsf", 
            [](const Header& self) { return self.tsf(); },
            [](Header& self, TSF value) { return self.tsf(value); }
        )
        .def_property(
            "packet_count", 
            [](const Header& self) { return self.packet_count(); },
            [](Header& self, uint8_t value) { return self.packet_count(value); }
        )
        .def_property(
            "packet_size", 
            [](const Header& self) { return self.packet_size(); },
            [](Header& self, uint16_t value) { return self.packet_size(value); }
        );

    py::class_<DataHeader, Header>(mod, "DataHeader")
        .def(py::init<>())
        .def_property(
            "trailer_included", 
            [](const DataHeader& self) { return self.trailer_included(); },
            [](DataHeader& self, bool value) { return self.trailer_included(value); }
        )
        .def_property(
            "not_v49d0", 
            [](const DataHeader& self) { return self.not_v49d0(); },
            [](DataHeader& self, bool value) { return self.not_v49d0(value); }
        )
        .def_property(
            "spectrum_or_time", 
            [](const DataHeader& self) { return self.spectrum_or_time(); },
            [](DataHeader& self, SpectrumOrTime value) { return self.spectrum_or_time(value); }
        );

    py::class_<ContextHeader, Header>(mod, "ContextHeader")
        .def(py::init<>())
        .def_property(
            "not_v49d0", 
            [](const ContextHeader& self) { return self.not_v49d0(); },
            [](ContextHeader& self, bool value) { return self.not_v49d0(value); }
        )
        .def_property(
            "tsm", 
            [](const ContextHeader& self) { return self.tsm(); },
            [](ContextHeader& self, TSM value) { return self.tsm(value); }
        );

    py::class_<CommandHeader, Header>(mod, "CommandHeader")
        .def(py::init<>())
        .def_property(
            "acknowledge_packet", 
            [](const CommandHeader& self) { return self.acknowledge_packet(); },
            [](CommandHeader& self, bool value) { return self.acknowledge_packet(value); }
        )
        .def_property(
            "cancellation_packet", 
            [](const CommandHeader& self) { return self.cancellation_packet(); },
            [](CommandHeader& self, bool value) { return self.cancellation_packet(value); }
        );
}
