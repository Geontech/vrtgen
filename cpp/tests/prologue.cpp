
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

TEST_CASE("Data Header getter methods")
{
    using vrtgen::packing::DataHeader;

    bytes data = {0, 0, 0, 0};
    DataHeader& header = *reinterpret_cast<DataHeader*>(data.data());

    SECTION("Trailer Included") {
        data[0] = 0x04;
        REQUIRE(header.getTrailerIncluded());
    }
    SECTION("Not a V49.0 Packet") {
        data[0] = 0x02;
        REQUIRE(header.getNotaV49_0Packet());
    }
    SECTION("Signal Spectrum or Signal Time Data Packet") {
        data[0] = 0x01;
        REQUIRE(header.getSignalSpectrumorSignalTimeDataPacket());
    }
}

TEST_CASE("Data Header setter methods")
{
    using vrtgen::packing::DataHeader;

    bytes data = {0, 0, 0, 0};
    DataHeader& header = *reinterpret_cast<DataHeader*>(data.data());

    SECTION("Trailer Included") {
        header.setTrailerIncluded(true);
        REQUIRE(data == bytes({0x04, 0x00, 0x00, 0x00}));
    }
    SECTION("Not a V49.0 Packet") {
        header.setNotaV49_0Packet(true);
        REQUIRE(data == bytes({0x02, 0x00, 0x00, 0x00}));
    }
    SECTION("Signal Spectrum or Signal Time Data Packet") {
        header.setSignalSpectrumorSignalTimeDataPacket(true);
        REQUIRE(data == bytes({0x01, 0x00, 0x00, 0x00}));
    }
}

TEST_CASE("Context Header getter methods")
{
    using vrtgen::packing::ContextHeader;

    bytes data = {0, 0, 0, 0};
    ContextHeader& header = *reinterpret_cast<ContextHeader*>(data.data());

    SECTION("Not a V49.0 Packet") {
        CHECK_FALSE(header.getNotaV49_0Packet());
        data[0] = 0x02;
        REQUIRE(header.getNotaV49_0Packet());
    }
    SECTION("Timestamp Mode") {
        CHECK(header.getTimestampMode() == vrtgen::TSM::FINE);
        data[0] = 0x01;
        CHECK(header.getTimestampMode() == vrtgen::TSM::COARSE);
    }
}

TEST_CASE("Context Header setter methods")
{
    using vrtgen::packing::ContextHeader;

    bytes data = {0, 0, 0, 0};
    ContextHeader& header = *reinterpret_cast<ContextHeader*>(data.data());

    SECTION("Not a V49.0 Packet") {
        header.setNotaV49_0Packet(true);
        REQUIRE(data == bytes({0x02, 0x00, 0x00, 0x00}));
    }
    SECTION("Timestamp Mode") {
        header.setTimestampMode(vrtgen::TSM::COARSE);
        CHECK(data == bytes({0x01, 0x00, 0x00, 0x00}));
        header.setTimestampMode(vrtgen::TSM::FINE);
        CHECK(data == bytes({0x00, 0x00, 0x00, 0x00}));
    }
}

TEST_CASE("Command Header getter methods")
{
    using vrtgen::packing::CommandHeader;

    bytes data = {0, 0, 0, 0};
    CommandHeader& header = *reinterpret_cast<CommandHeader*>(data.data());

    SECTION("Acknowledge") {
        CHECK_FALSE(header.getAcknowledgePacket());
        data[0] = 0x04;
        CHECK(header.getAcknowledgePacket());
    }
    SECTION("Cancellation") {
        CHECK_FALSE(header.getCancellationPacket());
        data[0] = 0x01;
        CHECK(header.getCancellationPacket());
    }
}

TEST_CASE("Command Header setter methods")
{
    using vrtgen::packing::CommandHeader;

    bytes data = {0, 0, 0, 0};
    CommandHeader& header = *reinterpret_cast<CommandHeader*>(data.data());

    SECTION("Acknowledge") {
        header.setAcknowledgePacket(true);
        REQUIRE(data == bytes({0x04, 0x00, 0x00, 0x00}));
    }
    SECTION("Cancellation") {
        header.setCancellationPacket(true);
        REQUIRE(data == bytes({0x01, 0x00, 0x00, 0x00}));
    }
}
