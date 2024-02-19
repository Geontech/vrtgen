#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include "vrtgen/packing/trailer.hpp"

void init_trailer(py::module_& mod)
{
    using namespace vrtgen::packing;
    py::class_<Trailer, StateEventIndicators>(mod, "Trailer")
        .def(py::init<>())
        .def_property(
            "sample_frame", 
            [](const Trailer& self) { return self.sample_frame(); },
            [](Trailer& self, SSI value) { return self.sample_frame(value); }
        )
        .def_property(
            "associated_context_packets_count", 
            [](const Trailer& self) { return self.associated_context_packets_count(); },
            [](Trailer& self, uint8_t value) { return self.associated_context_packets_count(value); }
        );
}
