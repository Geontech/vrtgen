#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "vrtgen/packing/cif7.hpp"

void init_cif7(py::module_& mod)
{
    using namespace vrtgen::packing;
    py::class_<CIF7, IndicatorField7>(mod, "CIF7")
        .def(py::init<>());

    py::class_<Belief>(mod, "Belief")
        .def(py::init<>())
        .def_property(
            "belief_percent",
            [](const Belief& self) { return self.belief_percent(); },
            [](Belief& self, int value) { return self.belief_percent(value); }
        );

    py::class_<Probability>(mod, "Probability")
        .def(py::init<>())
        .def_property(
            "probability_function",
            [](const Probability& self) { return self.probability_function(); },
            [](Probability& self, uint8_t value) { return self.probability_function(value); }
        )
        .def_property(
            "probability_percent",
            [](const Probability& self) { return self.probability_percent(); },
            [](Probability& self, uint8_t value) { return self.probability_percent(value); }
        );
}
