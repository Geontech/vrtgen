
#include <catch/catch.hpp>

#include <vrtgen/enums.hpp>
#include <vrtgen/packing/cif0.hpp>
#include <vrtgen/packing/cif1.hpp>
#include <vrtgen/packing/header.hpp>
#include <vrtgen/packing/trailer.hpp>

#include "bytes.hpp"

TEST_CASE("Header getter methods")
{
    using vrtgen::packing::Header;

    bytes data = {
        0x10, 0x5c, 0x12, 0x34,
    };
    Header& header = *reinterpret_cast<Header*>(data.data());
    SECTION("Packet Type") {
        REQUIRE(header.getPacketType() == vrtgen::PacketType::SIGNAL_DATA_STREAM_ID);
    }
    SECTION("Class Identifier Enable") {
        REQUIRE(header.isClassIdentifierEnabled() == false);
    }
    SECTION("TSI") {
        REQUIRE(header.getTSI() == vrtgen::TSI::UTC);
    }
    SECTION("TSF") {
        REQUIRE(header.getTSF() == vrtgen::TSF::SAMPLE_COUNT);
    }
    SECTION("Packet Count") {
        REQUIRE(header.getPacketCount() == 12);
    }
    SECTION("Packet Size") {
        REQUIRE(header.getPacketSize() == 0x1234);
    }
}

TEST_CASE("Header setter methods")
{
    using vrtgen::packing::Header;

    bytes data = {0, 0, 0, 0}; // zero-initialize
    Header& header = *reinterpret_cast<Header*>(data.data());

    SECTION("Packet Type") {
        header.setPacketType(vrtgen::PacketType::CONTEXT);
        REQUIRE(data == bytes({0x40, 0x00, 0x00, 0x00}));
    }
    SECTION("Class Identifier Enable") {
        header.setClassIdentifierEnabled(true);
        REQUIRE(data == bytes({0x08, 0x00, 0x00, 0x00}));
    }
    SECTION("TSI") {
        header.setTSI(vrtgen::TSI::GPS);
        REQUIRE(data == bytes({0x00, 0x80, 0x00, 0x00}));
    }
    SECTION("TSF") {
        header.setTSF(vrtgen::TSF::REAL_TIME);
        REQUIRE(data == bytes({0x00, 0x20, 0x00, 0x00}));
    }
    SECTION("Packet Count") {
        header.setPacketCount(12);
        REQUIRE(data == bytes({0x00, 0x0C, 0x00, 0x00}));
    }
    SECTION("Packet Size") {
        header.setPacketSize(0xCDEF);
        REQUIRE(data == bytes({0x00, 0x00, 0xCD, 0xEF}));
    }
}
