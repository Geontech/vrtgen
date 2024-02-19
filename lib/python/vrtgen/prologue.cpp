#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "vrtgen/packing/prologue.hpp"

void init_prologue(py::module_& mod)
{
    using namespace vrtgen::packing;
    py::class_<ClassIdentifier>(mod, "ClassIdentifier")
        .def(py::init<>())
        .def_property(
            "pad_bits", 
            [](const ClassIdentifier& self) { return self.pad_bits(); },
            [](ClassIdentifier& self, uint8_t value) { return self.pad_bits(value); }
        )
        .def_property(
            "oui", 
            [](const ClassIdentifier& self) { return self.oui(); },
            [](ClassIdentifier& self, uint32_t value) { return self.oui(value); }
        )
        .def_property(
            "information_code", 
            [](const ClassIdentifier& self) { return self.information_code(); },
            [](ClassIdentifier& self, uint16_t value) { return self.information_code(value); }
        )
        .def_property(
            "packet_code", 
            [](const ClassIdentifier& self) { return self.packet_code(); },
            [](ClassIdentifier& self, uint16_t value) { return self.packet_code(value); }
        );
}