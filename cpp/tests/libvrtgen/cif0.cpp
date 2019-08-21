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
        CHECK(payload_format.getDataItemSize() == 1);
        data[3] = 0x0F;
        CHECK(payload_format.getDataItemSize() == 16);
    }

    SECTION("Repeat Count") {
        CHECK(payload_format.getRepeatCount() == 1);
        data[4] = 0x12;
        data[5] = 0x34;
        CHECK(payload_format.getRepeatCount() == 0x1235);
    }

    SECTION("Vector Size") {
        CHECK(payload_format.getVectorSize() == 1);
        data[6] = 0x56;
        data[7] = 0x78;
        CHECK(payload_format.getVectorSize() == 0x5679);
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
        CHECK(data == bytes({0, 0, 0, 0x1F, 0, 0, 0, 0}));
    }

    SECTION("Repeat Count") {
        payload_format.setRepeatCount(512);
        CHECK(data == bytes({0, 0, 0, 0, 0x01, 0xFF, 0, 0}));

        payload_format.setRepeatCount(65536);
        CHECK(data == bytes({0, 0, 0, 0, 0xFF, 0xFF, 0, 0}));

        payload_format.setRepeatCount(1);
        CHECK(data == bytes({0, 0, 0, 0, 0, 0, 0, 0}));
    }

    SECTION("Vector Size") {
        payload_format.setVectorSize(1024);
        CHECK(data == bytes({0, 0, 0, 0, 0, 0, 0x03, 0xFF}));

        payload_format.setVectorSize(65536);
        CHECK(data == bytes({0, 0, 0, 0, 0, 0, 0xFF, 0xFF}));

        payload_format.setVectorSize(1);
        CHECK(data == bytes({0, 0, 0, 0, 0, 0, 0, 0}));
    }
}
