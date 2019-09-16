/*
 * Copyright (C) 2019 Geon Technologies, LLC
 *
 * This file is part of vrtgen.
 *
 * vrtgen is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * vrtgen is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */


#include "catch/catch.hpp"

#include "vrtgen/enums.hpp"
#include "vrtgen/packing/cif0.hpp"
#include "vrtgen/packing/cif1.hpp"
#include "vrtgen/packing/header.hpp"
#include "vrtgen/packing/trailer.hpp"

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

TEST_CASE("ClassIdentifier getter methods")
{
    using vrtgen::packing::ClassIdentifier;

    bytes data = {0x28, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    ClassIdentifier& class_id = *reinterpret_cast<ClassIdentifier*>(data.data());
    CHECK(sizeof(class_id) == data.size());

    SECTION("Pad Bit Count") {
        REQUIRE(class_id.getPadBitCount() == 5);
    }
    SECTION("Organizationally Unique Identifier") {
        REQUIRE(class_id.getOrganizationallyUniqueIdentifier() == 0x234567);
    }
    SECTION("Information Class Code") {
        REQUIRE(class_id.getInformationClassCode() == 0x89AB);
    }
    SECTION("Packet Class Code") {
        REQUIRE(class_id.getPacketClassCode() == 0xCDEF);
    }
}

TEST_CASE("ClassIdentifier setter methods")
{
    using vrtgen::packing::ClassIdentifier;

    bytes data = {0, 0, 0, 0, 0, 0, 0, 0};
    ClassIdentifier& class_id = *reinterpret_cast<ClassIdentifier*>(data.data());

    SECTION("Pad Bit Count") {
        class_id.setPadBitCount(0x13);
        REQUIRE(data == bytes({0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
    }
    SECTION("Organizationally Unique Identifier") {
        class_id.setOrganizationallyUniqueIdentifier(0x5307C0);
        REQUIRE(data == bytes({0x00, 0x53, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x00}));
    }
    SECTION("Information Class Code") {
        class_id.setInformationClassCode(0x916F);
        REQUIRE(data == bytes({0x00, 0x00, 0x00, 0x00, 0x91, 0x6F, 0x00, 0x00}));
    }
    SECTION("Packet Class Code") {
        class_id.setPacketClassCode(0x1234);
        REQUIRE(data == bytes({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34}));
    }
}
