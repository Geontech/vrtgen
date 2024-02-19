#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "vrtgen/packing/enums.hpp"

void init_enums(py::module_& mod)
{
    using namespace vrtgen::packing;
    py::enum_<PacketType>(mod, "PacketType")
        .value("SIGNAL_DATA", PacketType::SIGNAL_DATA)
        .value("SIGNAL_DATA_STREAM_ID", PacketType::SIGNAL_DATA_STREAM_ID)
        .value("EXTENSION_DATA", PacketType::EXTENSION_DATA)
        .value("EXTENSION_DATA_STREAM_ID", PacketType::EXTENSION_DATA_STREAM_ID)
        .value("CONTEXT", PacketType::CONTEXT)
        .value("EXTENSION_CONTEXT", PacketType::EXTENSION_CONTEXT)
        .value("COMMAND", PacketType::COMMAND)
        .value("EXTENSION_COMMAND", PacketType::EXTENSION_COMMAND)
        .export_values();

    py::enum_<TSI>(mod, "TSI")
        .value("NONE", TSI::NONE)
        .value("UTC", TSI::UTC)
        .value("GPS", TSI::GPS)
        .value("OTHER", TSI::OTHER)
        .export_values();

    py::enum_<TSF>(mod, "TSF")
        .value("NONE", TSF::NONE)
        .value("SAMPLE_COUNT", TSF::SAMPLE_COUNT)
        .value("REAL_TIME", TSF::REAL_TIME)
        .value("FREE_RUNNING", TSF::FREE_RUNNING)
        .export_values();

    py::enum_<TSM>(mod, "TSM")
        .value("FINE", TSM::FINE)
        .value("COARSE", TSM::COARSE)
        .export_values();

    py::enum_<SpectrumOrTime>(mod, "SpectrumOrTime")
        .value("TIME", SpectrumOrTime::TIME)
        .value("SPECTRUM", SpectrumOrTime::SPECTRUM)
        .export_values();

    py::enum_<IdentifierFormat>(mod, "IdentifierFormat")
        .value("WORD", IdentifierFormat::WORD)
        .value("UUID", IdentifierFormat::UUID)
        .export_values();

    py::enum_<ActionMode>(mod, "ActionMode")
        .value("NO_ACTION", ActionMode::NO_ACTION)
        .value("DRY_RUN", ActionMode::DRY_RUN)
        .value("EXECUTE", ActionMode::EXECUTE)
        .export_values();

    py::enum_<TimestampControlMode>(mod, "TimestampControlMode")
        .value("IGNORE", TimestampControlMode::IGNORE)
        .value("DEVICE", TimestampControlMode::DEVICE)
        .value("LATE", TimestampControlMode::LATE)
        .value("EARLY", TimestampControlMode::EARLY)
        .value("EARLY_LATE", TimestampControlMode::EARLY_LATE)
        .value("TIMING_ISSUES", TimestampControlMode::TIMING_ISSUES)
        .export_values();

    py::enum_<AGCMode>(mod, "AGCMode")
        .value("MGC", AGCMode::MGC)
        .value("AGC", AGCMode::AGC)
        .export_values();

    py::enum_<DataItemFormat>(mod, "DataItemFormat")
        .value("SIGNED_FIXED", DataItemFormat::SIGNED_FIXED)
        .value("SIGNED_VRT_1", DataItemFormat::SIGNED_VRT_1)
        .value("SIGNED_VRT_2", DataItemFormat::SIGNED_VRT_2)
        .value("SIGNED_VRT_3", DataItemFormat::SIGNED_VRT_3)
        .value("SIGNED_VRT_4", DataItemFormat::SIGNED_VRT_4)
        .value("SIGNED_VRT_5", DataItemFormat::SIGNED_VRT_5)
        .value("SIGNED_VRT_6", DataItemFormat::SIGNED_VRT_6)
        .value("SIGNED_FIXED_NON_NORMALIZED", DataItemFormat::SIGNED_FIXED_NON_NORMALIZED)
        .value("IEEE754_HALF_PRECISION", DataItemFormat::IEEE754_HALF_PRECISION)
        .value("IEEE754_SINGLE_PRECISION", DataItemFormat::IEEE754_SINGLE_PRECISION)
        .value("IEEE754_DOUBLE_PRECISION", DataItemFormat::IEEE754_DOUBLE_PRECISION)
        .value("UNSIGNED_FIXED", DataItemFormat::UNSIGNED_FIXED)
        .value("UNSIGNED_VRT_1", DataItemFormat::UNSIGNED_VRT_1)
        .value("UNSIGNED_VRT_2", DataItemFormat::UNSIGNED_VRT_2)
        .value("UNSIGNED_VRT_3", DataItemFormat::UNSIGNED_VRT_3)
        .value("UNSIGNED_VRT_4", DataItemFormat::UNSIGNED_VRT_4)
        .value("UNSIGNED_VRT_5", DataItemFormat::UNSIGNED_VRT_5)\
        .value("UNSIGNED_VRT_6", DataItemFormat::UNSIGNED_VRT_6)
        .value("UNSIGNED_FIXED_NON_NORMALIZED", DataItemFormat::UNSIGNED_FIXED_NON_NORMALIZED)
        .export_values();

    py::enum_<DataSampleType>(mod, "DataSampleType")
        .value("REAL", DataSampleType::REAL)
        .value("COMPLEX_CARTESIAN", DataSampleType::COMPLEX_CARTESIAN)
        .value("COMPLEX_POLAR", DataSampleType::COMPLEX_POLAR)
        .export_values();

    py::enum_<EntrySize>(mod, "EntrySize")
        .value("EIGHT_BIT", EntrySize::EIGHT_BIT)
        .value("SIXTEEN_BIT", EntrySize::SIXTEEN_BIT)
        .value("THIRTY_TWO_BIT", EntrySize::THIRTY_TWO_BIT)
        .export_values();

    py::enum_<PackingMethod>(mod, "PackingMethod")
        .value("EIGHT_BIT", PackingMethod::PROCESSING_EFFICIENT)
        .value("SIXTEEN_BIT", PackingMethod::LINK_EFFICIENT)
        .export_values();

    py::enum_<SSI>(mod, "SSI")
        .value("SINGLE", SSI::SINGLE)
        .value("FIRST", SSI::FIRST)
        .value("MIDDLE", SSI::MIDDLE)
        .value("FINAL", SSI::FINAL)
        .export_values();

    py::enum_<V49StandardCompliance>(mod, "V49StandardCompliance")
        .value("V49_0", V49StandardCompliance::V49_0)
        .value("V49_1", V49StandardCompliance::V49_1)
        .value("V49_A", V49StandardCompliance::V49_A)
        .value("V49_2", V49StandardCompliance::V49_2)
        .export_values();
}
