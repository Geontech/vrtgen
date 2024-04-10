#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include "vrtgen/types/uuid.hpp"

void init_uuid(py::module_& mod)
{
    using namespace vrtgen;
    py::class_<UUID>(mod, "UUID")
        .def(py::init<>())
        .def(py::init<const std::string&>())
        .def("get", &UUID::get)
        .def("set", &UUID::set)
        .def("size", &UUID::size);
}