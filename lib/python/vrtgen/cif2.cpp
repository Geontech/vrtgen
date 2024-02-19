#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "vrtgen/packing/cif2.hpp"

void init_cif2(py::module_& mod)
{
    using namespace vrtgen::packing;
    py::class_<CIF2, IndicatorField2>(mod, "CIF2")
        .def(py::init<>());
}