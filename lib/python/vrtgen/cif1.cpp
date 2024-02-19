#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include "vrtgen/packing/cif1.hpp"

void init_cif1(py::module_& mod)
{
    using namespace vrtgen::packing;
    py::class_<CIF1, IndicatorField1>(mod, "CIF1")
        .def(py::init<>());

    py::class_<Polarization>(mod, "Polarization")
        .def(py::init<>())
        .def_property(
            "tilt_angle",
            [](const Polarization& self) { return self.tilt_angle(); },
            [](Polarization& self, double value) { return self.tilt_angle(value); }
        )
        .def_property(
            "ellipticity_angle",
            [](const Polarization& self) { return self.ellipticity_angle(); },
            [](Polarization& self, double value) { return self.ellipticity_angle(value); }
        );

    py::class_<PointingVector>(mod, "PointingVector")
        .def(py::init<>())
        .def_property(
            "elevation_angle",
            [](const PointingVector& self) { return self.elevation_angle(); },
            [](PointingVector& self, double value) { return self.elevation_angle(value); }
        )
        .def_property(
            "azimuthal_angle",
            [](const PointingVector& self) { return self.azimuthal_angle(); },
            [](PointingVector& self, double value) { return self.azimuthal_angle(value); }
        );

    py::class_<BeamWidth>(mod, "BeamWidth")
        .def(py::init<>())
        .def_property(
            "horizontal",
            [](const BeamWidth& self) { return self.horizontal(); },
            [](BeamWidth& self, double value) { return self.horizontal(value); }
        )
        .def_property(
            "vertical",
            [](const BeamWidth& self) { return self.vertical(); },
            [](BeamWidth& self, double value) { return self.vertical(value); }
        );

    py::class_<EbNoBER>(mod, "EbNoBER")
        .def(py::init<>())
        .def_property(
            "ebno",
            [](const EbNoBER& self) { return self.ebno(); },
            [](EbNoBER& self, double value) { return self.ebno(value); }
        )
        .def_property(
            "ber",
            [](const EbNoBER& self) { return self.ber(); },
            [](EbNoBER& self, double value) { return self.ber(value); }
        );

    py::class_<Threshold>(mod, "Threshold")
        .def(py::init<>())
        .def_property(
            "stage_2",
            [](const Threshold& self) { return self.stage_2(); },
            [](Threshold& self, double value) { return self.stage_2(value); }
        )
        .def_property(
            "stage_1",
            [](const Threshold& self) { return self.stage_1(); },
            [](Threshold& self, double value) { return self.stage_1(value); }
        );

    py::class_<InterceptPoints>(mod, "InterceptPoints")
        .def(py::init<>())
        .def_property(
            "second_order",
            [](const InterceptPoints& self) { return self.second_order(); },
            [](InterceptPoints& self, double value) { return self.second_order(value); }
        )
        .def_property(
            "third_order",
            [](const InterceptPoints& self) { return self.third_order(); },
            [](InterceptPoints& self, double value) { return self.third_order(value); }
        );

    py::class_<SNRNoise>(mod, "SNRNoise")
        .def(py::init<>())
        .def_property(
            "snr",
            [](const SNRNoise& self) { return self.snr(); },
            [](SNRNoise& self, double value) { return self.snr(value); }
        )
        .def_property(
            "noise",
            [](const SNRNoise& self) { return self.noise(); },
            [](SNRNoise& self, double value) { return self.noise(value); }
        );

    py::class_<SectorStepScanCIF>(mod, "SectorStepScanCIF")
        .def(py::init<>())
        .def_property(
            "sector_number",
            [](const SectorStepScanCIF& self) { return self.sector_number(); },
            [](SectorStepScanCIF& self, bool value) { return self.sector_number(value); }
        )
        .def_property(
            "f1_start_frequency",
            [](const SectorStepScanCIF& self) { return self.f1_start_frequency(); },
            [](SectorStepScanCIF& self, bool value) { return self.f1_start_frequency(value); }
        )
        .def_property(
            "f2_stop_frequency",
            [](const SectorStepScanCIF& self) { return self.f2_stop_frequency(); },
            [](SectorStepScanCIF& self, bool value) { return self.f2_stop_frequency(value); }
        )
        .def_property(
            "resolution_bandwidth",
            [](const SectorStepScanCIF& self) { return self.resolution_bandwidth(); },
            [](SectorStepScanCIF& self, bool value) { return self.resolution_bandwidth(value); }
        )
        .def_property(
            "tune_step_size",
            [](const SectorStepScanCIF& self) { return self.tune_step_size(); },
            [](SectorStepScanCIF& self, bool value) { return self.tune_step_size(value); }
        )
        .def_property(
            "number_points",
            [](const SectorStepScanCIF& self) { return self.number_points(); },
            [](SectorStepScanCIF& self, bool value) { return self.number_points(value); }
        )
        .def_property(
            "default_gain",
            [](const SectorStepScanCIF& self) { return self.default_gain(); },
            [](SectorStepScanCIF& self, bool value) { return self.default_gain(value); }
        )
        .def_property(
            "threshold",
            [](const SectorStepScanCIF& self) { return self.threshold(); },
            [](SectorStepScanCIF& self, bool value) { return self.threshold(value); }
        )
        .def_property(
            "dwell_time",
            [](const SectorStepScanCIF& self) { return self.dwell_time(); },
            [](SectorStepScanCIF& self, bool value) { return self.dwell_time(value); }
        )
        .def_property(
            "start_time",
            [](const SectorStepScanCIF& self) { return self.start_time(); },
            [](SectorStepScanCIF& self, bool value) { return self.start_time(value); }
        )
        .def_property(
            "time_3",
            [](const SectorStepScanCIF& self) { return self.time_3(); },
            [](SectorStepScanCIF& self, bool value) { return self.time_3(value); }
        )
        .def_property(
            "time_4",
            [](const SectorStepScanCIF& self) { return self.time_4(); },
            [](SectorStepScanCIF& self, bool value) { return self.time_4(value); }
        );

    // TODO - SectorStepScan
    // TODO - IndexList

    py::class_<VersionInformation>(mod, "VersionInformation")
        .def(py::init<>())
        .def_property(
            "year",
            [](const VersionInformation& self) { return self.year(); },
            [](VersionInformation& self, uint8_t value) { return self.year(value); }
        )
        .def_property(
            "day",
            [](const VersionInformation& self) { return self.day(); },
            [](VersionInformation& self, uint8_t value) { return self.day(value); }
        )
        .def_property(
            "revision",
            [](const VersionInformation& self) { return self.revision(); },
            [](VersionInformation& self, uint8_t value) { return self.revision(value); }
        )
        .def_property(
            "user_defined",
            [](const VersionInformation& self) { return self.user_defined(); },
            [](VersionInformation& self, uint16_t value) { return self.user_defined(value); }
        );
}
