
#include <catch/catch.hpp>

#include <vrtgen/enums.hpp>
#include <vrtgen/packing/cif0.hpp>
#include <vrtgen/packing/cif1.hpp>
#include <vrtgen/packing/header.hpp>
#include <vrtgen/packing/trailer.hpp>

TEST_CASE("Header getter methods")
{
    using vrtgen::packing::Header;

    uint8_t data[] = {
        0x10, 0x5c, 0x12, 0x34,
    };
    Header& header = *reinterpret_cast<Header*>(data);
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

    uint8_t data[] = { 0, 0, 0, 0 };
    Header& header = *reinterpret_cast<Header*>(data);

    SECTION("Packet Type") {
        header.setPacketType(vrtgen::PacketType::CONTEXT);
        REQUIRE(data[0] == 0x40);
    }
    SECTION("Class Identifier Enable") {
        header.setClassIdentifierEnabled(true);
        REQUIRE(data[0] == 0x08);
    }
    SECTION("TSI") {
        header.setTSI(vrtgen::TSI::GPS);
        REQUIRE(data[1] == 0x80);
    }
    SECTION("TSF") {
        header.setTSF(vrtgen::TSF::REAL_TIME);
        REQUIRE(data[1] == 0x20);
    }
    SECTION("Packet Count") {
        header.setPacketCount(12);
        REQUIRE(data[1] == 0x0C);
    }
    SECTION("Packet Size") {
        header.setPacketSize(0xCDEF);
        REQUIRE(data[2] == 0xCD);
        REQUIRE(data[3] == 0xEF);
    }
}

TEST_CASE("swap16", "[swap]")
{
    int16_t swapped = vrtgen::swap16(0xBA5E);
    REQUIRE(swapped == 0x5EBA);
    REQUIRE(vrtgen::swap16(swapped) == 0xBA5E);
}

TEST_CASE("swap32", "[swap]")
{
    int32_t swapped = vrtgen::swap32(0xDEADBEEF);
    REQUIRE(swapped == 0xEFBEADDE);
    REQUIRE(vrtgen::swap32(swapped) == 0xDEADBEEF);
}

TEST_CASE("swap64", "[swap]")
{
    int64_t swapped = vrtgen::swap64(0x0123456789ABCDEF);
    REQUIRE(swapped == 0xEFCDAB8967452301);
    REQUIRE(vrtgen::swap64(swapped) == 0x0123456789ABCDEF);
}
