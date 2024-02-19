#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include "vrtgen/packing/state_event_indicators.hpp"

void init_state_event_indicators(py::module_& mod)
{
    using namespace vrtgen::packing;
    py::class_<StateEventIndicators>(mod, "StateEventIndicators")
        .def(py::init<>())
        .def_property(
            "calibrated_time", 
            [](const StateEventIndicators& self) { return self.calibrated_time(); },
            [](StateEventIndicators& self, bool value) { return self.calibrated_time(value); }
        )
        .def("reset_calibrated_time", &StateEventIndicators::reset_calibrated_time)
        .def_property(
            "valid_data", 
            [](const StateEventIndicators& self) { return self.valid_data(); },
            [](StateEventIndicators& self, bool value) { return self.valid_data(value); }
        )
        .def("reset_valid_data", &StateEventIndicators::reset_valid_data)
        .def_property(
            "reference_lock", 
            [](const StateEventIndicators& self) { return self.reference_lock(); },
            [](StateEventIndicators& self, bool value) { return self.reference_lock(value); }
        )
        .def("reset_reference_lock", &StateEventIndicators::reset_reference_lock)
        .def_property(
            "agc_mgc", 
            [](const StateEventIndicators& self) { return self.agc_mgc(); },
            [](StateEventIndicators& self, bool value) { return self.agc_mgc(value); }
        )
        .def("reset_agc_mgc", &StateEventIndicators::reset_agc_mgc)
        .def_property(
            "detected_signal", 
            [](const StateEventIndicators& self) { return self.detected_signal(); },
            [](StateEventIndicators& self, bool value) { return self.detected_signal(value); }
        )
        .def("reset_detected_signal", &StateEventIndicators::reset_detected_signal)
        .def_property(
            "spectral_inversion", 
            [](const StateEventIndicators& self) { return self.spectral_inversion(); },
            [](StateEventIndicators& self, bool value) { return self.spectral_inversion(value); }
        )
        .def("reset_spectral_inversion", &StateEventIndicators::reset_spectral_inversion)
        .def_property(
            "over_range", 
            [](const StateEventIndicators& self) { return self.over_range(); },
            [](StateEventIndicators& self, bool value) { return self.over_range(value); }
        )
        .def("reset_over_range", &StateEventIndicators::reset_over_range)
        .def_property(
            "sample_loss", 
            [](const StateEventIndicators& self) { return self.sample_loss(); },
            [](StateEventIndicators& self, bool value) { return self.sample_loss(value); }
        )
        .def("reset_sample_loss", &StateEventIndicators::reset_sample_loss);
}
