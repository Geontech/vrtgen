#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_enums(py::module_& mod);
void init_header(py::module_& mod);
void init_prologue(py::module_& mod);
void init_trailer(py::module_& mod);
void init_state_event_indicators(py::module_& mod);
void init_indicator_fields(py::module_& mod);
void init_cif0(py::module_& mod);
void init_cif1(py::module_& mod);
void init_cif2(py::module_& mod);
void init_cif7(py::module_& mod);
void init_command(py::module_& mod);

PYBIND11_MODULE(pyvrtgen, vrtgen_mod) {
    auto enum_submod = vrtgen_mod.def_submodule("enums");
    init_enums(enum_submod);
    auto packing_submod = vrtgen_mod.def_submodule("packing");
    init_header(packing_submod);
    init_prologue(packing_submod);
    init_state_event_indicators(packing_submod);
    init_trailer(packing_submod);
    init_indicator_fields(packing_submod);
    init_cif0(packing_submod);
    init_cif1(packing_submod);
    init_cif2(packing_submod);
    init_cif7(packing_submod);
    init_command(packing_submod);
}
