#include "catch/catch.hpp"

#include "vrtgen/enums.hpp"
#include "vrtgen/packing/cif0.hpp"

#include "bytes.hpp"

TEST_CASE("PayloadFormat getter methods", "[cif0]")
{
    using vrtgen::packing::PayloadFormat;
    bytes data = {0, 0, 0, 0, 0, 0, 0, 0};
    PayloadFormat& payload_format = *reinterpret_cast<PayloadFormat*>(data.data());

    SECTION("Packing Method") {
        CHECK(payload_format.getPackingMethod() == vrtgen::PackingMethod::PROCESSING_EFFICIENT);
        data[0] = 0x80;
        CHECK(payload_format.getPackingMethod() == vrtgen::PackingMethod::LINK_EFFICIENT);
    }

    SECTION("Real/Complex Type") {
        CHECK(payload_format.getRealComplexType() == vrtgen::DataSampleType::REAL);
        data[0] = 0x20;
        CHECK(payload_format.getRealComplexType() == vrtgen::DataSampleType::COMPLEX_CARTESIAN);
    }

    SECTION("Data Item Format") {
        CHECK(payload_format.getDataItemFormat() == vrtgen::DataItemFormat::SIGNED_FIXED);
        data[0] = 0x11;
        CHECK(payload_format.getDataItemFormat() == vrtgen::DataItemFormat::UNSIGNED_VRT_1);
    }

    SECTION("Data Item Size") {
        CHECK(payload_format.getDataItemSize() == 0);
        data[3] = 0x10;
        CHECK(payload_format.getDataItemSize() == 16);
    }
}

TEST_CASE("PayloadFormat setter methods", "[cif0]")
{
    using vrtgen::packing::PayloadFormat;
    bytes data = {0, 0, 0, 0, 0, 0, 0, 0};
    PayloadFormat& payload_format = *reinterpret_cast<PayloadFormat*>(data.data());

    SECTION("Packing Method") {
        payload_format.setPackingMethod(vrtgen::PackingMethod::LINK_EFFICIENT);
        CHECK(data == bytes({0x80, 0, 0, 0, 0, 0, 0, 0}));
    }

    SECTION("Real/Complex Type") {
        payload_format.setRealComplexType(vrtgen::DataSampleType::COMPLEX_POLAR);
        CHECK(data == bytes({0x40, 0, 0, 0, 0, 0, 0, 0}));
    }

    SECTION("Data Item Format") {
        payload_format.setDataItemFormat(vrtgen::DataItemFormat::UNSIGNED_FIXED_NON_NORMALIZED);
        CHECK(data == bytes({0x17, 0, 0, 0, 0, 0, 0, 0}));
    }

    SECTION("Data Item Size") {
        payload_format.setDataItemSize(32);
        CHECK(data == bytes({0, 0, 0, 0x20, 0, 0, 0, 0}));
    }
}
