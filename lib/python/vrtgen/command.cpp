#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "vrtgen/packing/command.hpp"

void init_command(py::module_& mod)
{
    using namespace vrtgen::packing;
    py::class_<ControlAcknowledgeMode>(mod, "ControlAcknowledgeMode")
        .def(py::init<>())
        .def_property(
            "controllee_enable",
            [](const ControlAcknowledgeMode& self) { return self.controllee_enable(); },
            [](ControlAcknowledgeMode& self, bool value) { return self.controllee_enable(value); }
        )
        .def_property(
            "controllee_format",
            [](const ControlAcknowledgeMode& self) { return self.controllee_format(); },
            [](ControlAcknowledgeMode& self, IdentifierFormat value) { return self.controllee_format(value); }
        )
        .def_property(
            "controller_enable",
            [](const ControlAcknowledgeMode& self) { return self.controller_enable(); },
            [](ControlAcknowledgeMode& self, bool value) { return self.controller_enable(value); }
        )
        .def_property(
            "controller_format",
            [](const ControlAcknowledgeMode& self) { return self.controller_format(); },
            [](ControlAcknowledgeMode& self, IdentifierFormat value) { return self.controller_format(value); }
        )
        .def_property(
            "permit_partial",
            [](const ControlAcknowledgeMode& self) { return self.permit_partial(); },
            [](ControlAcknowledgeMode& self, bool value) { return self.permit_partial(value); }
        )
        .def_property(
            "permit_warnings",
            [](const ControlAcknowledgeMode& self) { return self.permit_warnings(); },
            [](ControlAcknowledgeMode& self, bool value) { return self.permit_warnings(value); }
        )
        .def_property(
            "permit_errors",
            [](const ControlAcknowledgeMode& self) { return self.permit_errors(); },
            [](ControlAcknowledgeMode& self, bool value) { return self.permit_errors(value); }
        )
        .def_property(
            "action_mode",
            [](const ControlAcknowledgeMode& self) { return self.action_mode(); },
            [](ControlAcknowledgeMode& self, ActionMode value) { return self.action_mode(value); }
        )
        .def_property(
            "nack_only",
            [](const ControlAcknowledgeMode& self) { return self.nack_only(); },
            [](ControlAcknowledgeMode& self, bool value) { return self.nack_only(value); }
        )
        .def_property(
            "timing_control",
            [](const ControlAcknowledgeMode& self) { return self.timing_control(); },
            [](ControlAcknowledgeMode& self, TimestampControlMode value) { return self.timing_control(value); }
        );

    py::class_<ControlCAM, ControlAcknowledgeMode>(mod, "ControlCAM")
        .def(py::init<>())
        .def_property(
            "req_v",
            [](const ControlCAM& self) { return self.req_v(); },
            [](ControlCAM& self, bool value) { return self.req_v(value); }
        )
        .def_property(
            "req_x",
            [](const ControlCAM& self) { return self.req_x(); },
            [](ControlCAM& self, bool value) { return self.req_x(value); }
        )
        .def_property(
            "req_s",
            [](const ControlCAM& self) { return self.req_s(); },
            [](ControlCAM& self, bool value) { return self.req_s(value); }
        )
        .def_property(
            "req_w",
            [](const ControlCAM& self) { return self.req_w(); },
            [](ControlCAM& self, bool value) { return self.req_w(value); }
        )
        .def_property(
            "req_er",
            [](const ControlCAM& self) { return self.req_er(); },
            [](ControlCAM& self, bool value) { return self.req_er(value); }
        );

    py::class_<AcknowledgeCAM, ControlAcknowledgeMode>(mod, "AcknowledgeCAM")
        .def(py::init<>())
        .def_property(
            "ack_v",
            [](const AcknowledgeCAM& self) { return self.ack_v(); },
            [](AcknowledgeCAM& self, bool value) { return self.ack_v(value); }
        )
        .def_property(
            "ack_x",
            [](const AcknowledgeCAM& self) { return self.ack_x(); },
            [](AcknowledgeCAM& self, bool value) { return self.ack_x(value); }
        )
        .def_property(
            "ack_s",
            [](const AcknowledgeCAM& self) { return self.ack_s(); },
            [](AcknowledgeCAM& self, bool value) { return self.ack_s(value); }
        )
        .def_property(
            "ack_w",
            [](const AcknowledgeCAM& self) { return self.ack_w(); },
            [](AcknowledgeCAM& self, bool value) { return self.ack_w(value); }
        )
        .def_property(
            "ack_er",
            [](const AcknowledgeCAM& self) { return self.ack_er(); },
            [](AcknowledgeCAM& self, bool value) { return self.ack_er(value); }
        )
        .def_property(
            "partial_action",
            [](const AcknowledgeCAM& self) { return self.partial_action(); },
            [](AcknowledgeCAM& self, bool value) { return self.partial_action(value); }
        )
        .def_property(
            "scheduled_or_executed",
            [](const AcknowledgeCAM& self) { return self.scheduled_or_executed(); },
            [](AcknowledgeCAM& self, bool value) { return self.scheduled_or_executed(value); }
        );

    py::class_<WIF0, IndicatorField0>(mod, "WIF0")
        .def(py::init<>())
        .def_property(
            "wif7_enable",
            [](const WIF0& self) { return self.wif7_enable(); },
            [](WIF0& self, bool value) { return self.wif7_enable(value); }
        )
        .def_property(
            "wif3_enable",
            [](const WIF0& self) { return self.wif3_enable(); },
            [](WIF0& self, bool value) { return self.wif3_enable(value); }
        )
        .def_property(
            "wif2_enable",
            [](const WIF0& self) { return self.wif2_enable(); },
            [](WIF0& self, bool value) { return self.wif2_enable(value); }
        )
        .def_property(
            "wif1_enable",
            [](const WIF0& self) { return self.wif1_enable(); },
            [](WIF0& self, bool value) { return self.wif1_enable(value); }
        );

    py::class_<EIF0, IndicatorField0>(mod, "EIF0")
        .def(py::init<>())
        .def_property(
            "eif7_enable",
            [](const EIF0& self) { return self.eif7_enable(); },
            [](EIF0& self, bool value) { return self.eif7_enable(value); }
        )
        .def_property(
            "eif3_enable",
            [](const EIF0& self) { return self.eif3_enable(); },
            [](EIF0& self, bool value) { return self.eif3_enable(value); }
        )
        .def_property(
            "eif2_enable",
            [](const EIF0& self) { return self.eif2_enable(); },
            [](EIF0& self, bool value) { return self.eif2_enable(value); }
        )
        .def_property(
            "eif1_enable",
            [](const EIF0& self) { return self.eif1_enable(); },
            [](EIF0& self, bool value) { return self.eif1_enable(value); }
        );

    py::class_<WEIF1, IndicatorField1>(mod, "WEIF1")
        .def(py::init<>());

    py::class_<WEIF2, IndicatorField2>(mod, "WEIF2")
        .def(py::init<>());

    py::class_<WEIF3, IndicatorField3>(mod, "WEIF3")
        .def(py::init<>());

    py::class_<WEIF7, IndicatorField7>(mod, "WEIF7")
        .def(py::init<>());

    py::class_<WarningErrorFields>(mod, "WarningErrorFields")
        .def(py::init<>())
        .def_property(
            "field_not_executed",
            [](const WarningErrorFields& self) { return self.field_not_executed(); },
            [](WarningErrorFields& self, bool value) { return self.field_not_executed(value); }
        )
        .def_property(
            "device_failure",
            [](const WarningErrorFields& self) { return self.device_failure(); },
            [](WarningErrorFields& self, bool value) { return self.device_failure(value); }
        )
        .def_property(
            "erroneous_field",
            [](const WarningErrorFields& self) { return self.erroneous_field(); },
            [](WarningErrorFields& self, bool value) { return self.erroneous_field(value); }
        )
        .def_property(
            "parameter_out_of_range",
            [](const WarningErrorFields& self) { return self.parameter_out_of_range(); },
            [](WarningErrorFields& self, bool value) { return self.parameter_out_of_range(value); }
        )
        .def_property(
            "parameter_unsupported_precision",
            [](const WarningErrorFields& self) { return self.parameter_unsupported_precision(); },
            [](WarningErrorFields& self, bool value) { return self.parameter_unsupported_precision(value); }
        )
        .def_property(
            "field_value_invalid",
            [](const WarningErrorFields& self) { return self.field_value_invalid(); },
            [](WarningErrorFields& self, bool value) { return self.field_value_invalid(value); }
        )
        .def_property(
            "timestamp_problem",
            [](const WarningErrorFields& self) { return self.timestamp_problem(); },
            [](WarningErrorFields& self, bool value) { return self.timestamp_problem(value); }
        )
        .def_property(
            "hazardous_power_levels",
            [](const WarningErrorFields& self) { return self.hazardous_power_levels(); },
            [](WarningErrorFields& self, bool value) { return self.hazardous_power_levels(value); }
        )
        .def_property(
            "distortion",
            [](const WarningErrorFields& self) { return self.distortion(); },
            [](WarningErrorFields& self, bool value) { return self.distortion(value); }
        )
        .def_property(
            "in_band_power_compliance",
            [](const WarningErrorFields& self) { return self.in_band_power_compliance(); },
            [](WarningErrorFields& self, bool value) { return self.in_band_power_compliance(value); }
        )
        .def_property(
            "out_of_band_power_compliance",
            [](const WarningErrorFields& self) { return self.out_of_band_power_compliance(); },
            [](WarningErrorFields& self, bool value) { return self.out_of_band_power_compliance(value); }
        )
        .def_property(
            "cosite_interference",
            [](const WarningErrorFields& self) { return self.cosite_interference(); },
            [](WarningErrorFields& self, bool value) { return self.cosite_interference(value); }
        )
        .def_property(
            "regional_interference",
            [](const WarningErrorFields& self) { return self.regional_interference(); },
            [](WarningErrorFields& self, bool value) { return self.regional_interference(value); }
        );
}
