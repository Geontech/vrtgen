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

#include "yamls/basic.hpp"

#include <vector>
#include <bytes.hpp>

TEST_CASE("Basic context message") {
    BasicContext packet_in;
    packet_in.setStreamIdentifier(0x12345678);
    packet_in.setReferencePointIdentifier(0x10AB7DE9);

    size_t packed_size = packing::BasicContextHelper::bytes_required(packet_in);
    CHECK(packed_size == 16);

    std::vector<unsigned char> data;
    data.resize(16);
    packing::BasicContextHelper::pack(packet_in, data.data(), data.size());

    const vrtgen::packing::ContextHeader* header = reinterpret_cast<const vrtgen::packing::ContextHeader*>(data.data());
    CHECK(header->getPacketType() == vrtgen::PacketType::CONTEXT);
    CHECK(header->getTSI() == vrtgen::TSI::NONE);
    CHECK(header->getTSF() == vrtgen::TSF::NONE);
    CHECK_FALSE(header->isClassIdentifierEnabled());
    CHECK(header->getPacketSize() == 4);
    CHECK(header->getTimestampMode() == vrtgen::TSM::FINE);
    // NB: As configured, BasicContextPacket is V49.0-compatible
    CHECK_FALSE(header->getNotaV49d0Packet());

    CHECK(packing::BasicContextHelper::match(data.data(), data.size()));

    BasicContext packet_out;
    packing::BasicContextHelper::unpack(packet_out, data.data(), data.size());
    CHECK(packet_out.getStreamIdentifier() == 0x12345678);
    CHECK(packet_out.getReferencePointIdentifier() == 0x10AB7DE9);
}

TEST_CASE("Basic control message") {
    const vrtgen::StreamIdentifier STREAM_ID = 0x12345678;
    const vrtgen::MessageIdentifier MESSAGE_ID = 0xFEDCBA98;
    const double RF_FREQ = 101.1e6;

    BasicControl packet_in;
    packet_in.setStreamIdentifier(STREAM_ID);
    packet_in.setMessageID(MESSAGE_ID);
    packet_in.setRFReferenceFrequency(RF_FREQ);

    const size_t PACKED_SIZE = 28;
    REQUIRE(packing::BasicControlHelper::bytes_required(packet_in) == PACKED_SIZE);

    bytes data;
    data.resize(PACKED_SIZE);
    packing::BasicControlHelper::pack(packet_in, data.data(), data.size());

    const vrtgen::packing::CommandHeader* header = reinterpret_cast<const vrtgen::packing::CommandHeader*>(data.data());
    CHECK(header->getPacketType() == vrtgen::PacketType::COMMAND);
    CHECK(header->getTSI() == vrtgen::TSI::NONE);
    CHECK(header->getTSF() == vrtgen::TSF::NONE);
    CHECK_FALSE(header->isClassIdentifierEnabled());
    CHECK(header->getPacketSize() == (PACKED_SIZE / 4));
    CHECK_FALSE(header->getAcknowledgePacket());
    CHECK_FALSE(header->getCancellationPacket());
    // Stream ID should be at offset 4
    CHECK(reinterpret_cast<const vrtgen::packing::StreamIdentifier*>(&data[4])->get() == STREAM_ID);
    // Message ID should be at offset 12
    CHECK(reinterpret_cast<const vrtgen::packing::MessageID*>(&data[12])->get() == MESSAGE_ID);

    CHECK(packing::BasicControlHelper::match(data.data(), data.size()));

    BasicControl packet_out;
    packing::BasicControlHelper::unpack(packet_out, data.data(), data.size());
    CHECK(packet_out.getStreamIdentifier() == STREAM_ID);
    CHECK(packet_out.getMessageID() == MESSAGE_ID);
    CHECK(packet_out.getRFReferenceFrequency() == RF_FREQ);
}

TEST_CASE("All CIF0 Context") {
    SECTION("Field Change Indicator") {
        // TODO
        CHECK(true);
    }
    
    SECTION("Reference Point Identifier") {
        const vrtgen::StreamIdentifier REF_POINT_ID = 0x12345678;
        const vrtgen::StreamIdentifier REF_POINT_ID_BE = 0x78563412;
        const size_t PACKED_SIZE = 16;

        AllCIF0Context packet_in;
        packet_in.setReferencePointIdentifier(REF_POINT_ID);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const vrtgen::StreamIdentifier*>(&data[12])) == REF_POINT_ID_BE);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        AllCIF0Context packet_out;
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasReferencePointIdentifier());
        CHECK(packet_out.getReferencePointIdentifier() == REF_POINT_ID);
    }

    SECTION("Bandwidth") {
        // Observation 9.5.1-4
        const double BANDWIDTH_1 = 1.0; // represents 0x0000 0000 0010 0000
        const double BANDWIDTH_2 = 0.95e-6; // represents 0x0000 0000 0000 0001
        const int64_t BANDWIDTH_INT_BE_1 = 0x0000100000000000;
        const int64_t BANDWIDTH_INT_BE_2 = 0x0100000000000000;
        const size_t PACKED_SIZE = 20;

        // Bandwidth 1
        AllCIF0Context packet_in;
        packet_in.setBandwidth(BANDWIDTH_1);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == BANDWIDTH_INT_BE_1);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        AllCIF0Context packet_out;
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasBandwidth());
        CHECK(packet_out.getBandwidth() == BANDWIDTH_1);

        // Bandwidth 2
        packet_in = AllCIF0Context();
        packet_in.setBandwidth(BANDWIDTH_2);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == BANDWIDTH_INT_BE_2);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasBandwidth());
        CHECK(packet_out.getBandwidth() == BANDWIDTH_2);
    }

    SECTION("IF Reference Frequency") {
        // Observation 9.5.5-3
        const double IF_REF_FREQ_1 = 1.0; // represents 0x0000 0000 0010 0000
        const double IF_REF_FREQ_2 = 0.95e-6; // represents 0x0000 0000 0000 0001
        const double IF_REF_FREQ_3 = -1.0; // represents 0xFFFF FFFF FFF0 0000
        const double IF_REF_FREQ_4 = -0.95e-6; // represents 0xFFFF FFFF FFFF FFFF
        const int64_t IF_REF_FREQ_INT_BE_1 = 0x0000100000000000;
        const int64_t IF_REF_FREQ_INT_BE_2 = 0x0100000000000000;
        const int64_t IF_REF_FREQ_INT_BE_3 = 0x0000F0FFFFFFFFFF;
        const int64_t IF_REF_FREQ_INT_BE_4 = 0xFFFFFFFFFFFFFFFF;
        const size_t PACKED_SIZE = 20;

        // IF Reference Frequency 1
        AllCIF0Context packet_in;
        packet_in.setIFReferenceFrequency(IF_REF_FREQ_1);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_REF_FREQ_INT_BE_1);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        AllCIF0Context packet_out;
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasIFReferenceFrequency());
        CHECK(packet_out.getIFReferenceFrequency() == IF_REF_FREQ_1);

        // IF Reference Frequency 2
        packet_in = AllCIF0Context();
        packet_in.setIFReferenceFrequency(IF_REF_FREQ_2);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_REF_FREQ_INT_BE_2);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasIFReferenceFrequency());
        CHECK(packet_out.getIFReferenceFrequency() == IF_REF_FREQ_2);

        // IF Reference Frequency 3
        packet_in = AllCIF0Context();
        packet_in.setIFReferenceFrequency(IF_REF_FREQ_3);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_REF_FREQ_INT_BE_3);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasIFReferenceFrequency());
        CHECK(packet_out.getIFReferenceFrequency() == IF_REF_FREQ_3);

        // IF Reference Frequency 4
        packet_in = AllCIF0Context();
        packet_in.setIFReferenceFrequency(IF_REF_FREQ_4);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_REF_FREQ_INT_BE_4);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasIFReferenceFrequency());
        CHECK(packet_out.getIFReferenceFrequency() == IF_REF_FREQ_4);
    }

    SECTION("RF Reference Frequency") {
        // Observation 9.5.10-2
        const double RF_REF_FREQ_1 = 1.0; // represents 0x0000 0000 0010 0000
        const double RF_REF_FREQ_2 = 0.95e-6; // represents 0x0000 0000 0000 0001
        const double RF_REF_FREQ_3 = -1.0; // represents 0xFFFF FFFF FFF0 0000
        const double RF_REF_FREQ_4 = -0.95e-6; // represents 0xFFFF FFFF FFFF FFFF
        const int64_t RF_REF_FREQ_INT_BE_1 = 0x0000100000000000;
        const int64_t RF_REF_FREQ_INT_BE_2 = 0x0100000000000000;
        const int64_t RF_REF_FREQ_INT_BE_3 = 0x0000F0FFFFFFFFFF;
        const int64_t RF_REF_FREQ_INT_BE_4 = 0xFFFFFFFFFFFFFFFF;
        const size_t PACKED_SIZE = 20;

        // RF Reference Frequency 1
        AllCIF0Context packet_in;
        packet_in.setRFReferenceFrequency(RF_REF_FREQ_1);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_INT_BE_1);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        AllCIF0Context packet_out;
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasRFReferenceFrequency());
        CHECK(packet_out.getRFReferenceFrequency() == RF_REF_FREQ_1);

        // IF Reference Frequency 2
        packet_in = AllCIF0Context();
        packet_in.setRFReferenceFrequency(RF_REF_FREQ_2);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_INT_BE_2);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasRFReferenceFrequency());
        CHECK(packet_out.getRFReferenceFrequency() == RF_REF_FREQ_2);

        // IF Reference Frequency 3
        packet_in = AllCIF0Context();
        packet_in.setRFReferenceFrequency(RF_REF_FREQ_3);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_INT_BE_3);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasRFReferenceFrequency());
        CHECK(packet_out.getRFReferenceFrequency() == RF_REF_FREQ_3);

        // IF Reference Frequency 4
        packet_in = AllCIF0Context();
        packet_in.setRFReferenceFrequency(RF_REF_FREQ_4);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_INT_BE_4);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasRFReferenceFrequency());
        CHECK(packet_out.getRFReferenceFrequency() == RF_REF_FREQ_4);
    }

    SECTION("RF Reference Frequency Offset") {
        // Observation 9.5.11-4
        const double RF_REF_FREQ_OFFSET_1 = 1.0; // represents 0x0000 0000 0010 0000
        const double RF_REF_FREQ_OFFSET_2 = 0.95e-6; // represents 0x0000 0000 0000 0001
        const double RF_REF_FREQ_OFFSET_3 = -1.0; // represents 0xFFFF FFFF FFF0 0000
        const double RF_REF_FREQ_OFFSET_4 = -0.95e-6; // represents 0xFFFF FFFF FFFF FFFF
        const int64_t RF_REF_FREQ_OFFSET_INT_BE_1 = 0x0000100000000000;
        const int64_t RF_REF_FREQ_OFFSET_INT_BE_2 = 0x0100000000000000;
        const int64_t RF_REF_FREQ_OFFSET_INT_BE_3 = 0x0000F0FFFFFFFFFF;
        const int64_t RF_REF_FREQ_OFFSET_INT_BE_4 = 0xFFFFFFFFFFFFFFFF;
        const size_t PACKED_SIZE = 20;

        // RF Reference Frequency Offset 1
        AllCIF0Context packet_in;
        packet_in.setRFReferenceFrequencyOffset(RF_REF_FREQ_OFFSET_1);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_OFFSET_INT_BE_1);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        AllCIF0Context packet_out;
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasRFReferenceFrequencyOffset());
        CHECK(packet_out.getRFReferenceFrequencyOffset() == RF_REF_FREQ_OFFSET_1);

        // RF Reference Frequency Offset 2
        packet_in = AllCIF0Context();
        packet_in.setRFReferenceFrequencyOffset(RF_REF_FREQ_OFFSET_2);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_OFFSET_INT_BE_2);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasRFReferenceFrequencyOffset());
        CHECK(packet_out.getRFReferenceFrequencyOffset() == RF_REF_FREQ_OFFSET_2);

        // RF Reference Frequency Offset 3
        packet_in = AllCIF0Context();
        packet_in.setRFReferenceFrequencyOffset(RF_REF_FREQ_OFFSET_3);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_OFFSET_INT_BE_3);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasRFReferenceFrequencyOffset());
        CHECK(packet_out.getRFReferenceFrequencyOffset() == RF_REF_FREQ_OFFSET_3);

        // RF Reference Frequency Offset 4
        packet_in = AllCIF0Context();
        packet_in.setRFReferenceFrequencyOffset(RF_REF_FREQ_OFFSET_4);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_OFFSET_INT_BE_4);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasRFReferenceFrequencyOffset());
        CHECK(packet_out.getRFReferenceFrequencyOffset() == RF_REF_FREQ_OFFSET_4);
    }

    SECTION("IF Band Offset") {
        // Observation 9.5.4-3
        const double IF_BAND_OFFSET_1 = 1.0; // represents 0x0000 0000 0010 0000
        const double IF_BAND_OFFSET_2 = 0.95e-6; // represents 0x0000 0000 0000 0001
        const double IF_BAND_OFFSET_3 = -1.0; // represents 0xFFFF FFFF FFF0 0000
        const double IF_BAND_OFFSET_4 = -0.95e-6; // represents 0xFFFF FFFF FFFF FFFF
        const int64_t IF_BAND_OFFSET_INT_BE_1 = 0x0000100000000000;
        const int64_t IF_BAND_OFFSET_INT_BE_2 = 0x0100000000000000;
        const int64_t IF_BAND_OFFSET_INT_BE_3 = 0x0000F0FFFFFFFFFF;
        const int64_t IF_BAND_OFFSET_INT_BE_4 = 0xFFFFFFFFFFFFFFFF;
        const size_t PACKED_SIZE = 20;

        // IF Band Offset 1
        AllCIF0Context packet_in;
        packet_in.setIFBandOffset(IF_BAND_OFFSET_1);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_BAND_OFFSET_INT_BE_1);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        AllCIF0Context packet_out;
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasIFBandOffset());
        CHECK(packet_out.getIFBandOffset() == IF_BAND_OFFSET_1);

        // IF Band Offset 2
        packet_in = AllCIF0Context();
        packet_in.setIFBandOffset(IF_BAND_OFFSET_2);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_BAND_OFFSET_INT_BE_2);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasIFBandOffset());
        CHECK(packet_out.getIFBandOffset() == IF_BAND_OFFSET_2);

        // IF band Offset 3
        packet_in = AllCIF0Context();
        packet_in.setIFBandOffset(IF_BAND_OFFSET_3);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_BAND_OFFSET_INT_BE_3);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasIFBandOffset());
        CHECK(packet_out.getIFBandOffset() == IF_BAND_OFFSET_3);

        // IF Band Offset 4
        packet_in = AllCIF0Context();
        packet_in.setIFBandOffset(IF_BAND_OFFSET_4);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_BAND_OFFSET_INT_BE_4);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasIFBandOffset());
        CHECK(packet_out.getIFBandOffset() == IF_BAND_OFFSET_4);
    }

    SECTION("Reference Level") {
        // Observation 9.5.9-3
        const float REF_LEVEL_1 = 1.0; // represents 0x0000 0080
        const float REF_LEVEL_2 = 0.0078125; // represents 0x0000 0001
        const float REF_LEVEL_3 = -1.0; // represents 0x0000 FF80
        const float REF_LEVEL_4 = -0.0078125; // represents 0x0000 FFFF
        const int32_t REF_LEVEL_INT_BE_1 = 0x80000000;
        const int32_t REF_LEVEL_INT_BE_2 = 0x01000000;
        const int32_t REF_LEVEL_INT_BE_3 = 0x80FF0000;
        const int32_t REF_LEVEL_INT_BE_4 = 0xFFFF0000;
        const size_t PACKED_SIZE = 16;

        // Reference Level 1
        AllCIF0Context packet_in;
        packet_in.setReferenceLevel(REF_LEVEL_1);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == REF_LEVEL_INT_BE_1);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        AllCIF0Context packet_out;
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasReferenceLevel());
        CHECK(packet_out.getReferenceLevel() == REF_LEVEL_1);

        // Reference Level 2
        packet_in = AllCIF0Context();
        packet_in.setReferenceLevel(REF_LEVEL_2);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == REF_LEVEL_INT_BE_2);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasReferenceLevel());
        CHECK(packet_out.getReferenceLevel() == REF_LEVEL_2);

        // Reference Level 3
        packet_in = AllCIF0Context();
        packet_in.setReferenceLevel(REF_LEVEL_3);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == REF_LEVEL_INT_BE_3);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasReferenceLevel());
        CHECK(packet_out.getReferenceLevel() == REF_LEVEL_3);

        // Reference Level 4
        packet_in = AllCIF0Context();
        packet_in.setReferenceLevel(REF_LEVEL_4);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == REF_LEVEL_INT_BE_4);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasReferenceLevel());
        CHECK(packet_out.getReferenceLevel() == REF_LEVEL_4);
    }

    SECTION("Gain") {
        // Observation 9.5.3-5
        const float GAIN_1 = 1.0; // represents 0x0080 0080
        const float GAIN_2 = -1; // represents 0xFF80 FF80
        const float GAIN_3 = 0.0078125; // represents 0x0001 0001
        const float GAIN_4 = -0.0078125; // represents 0xFFFF FFFF
        const int32_t GAIN_INT_BE_1 = 0x80008000;
        const int32_t GAIN_INT_BE_2 = 0x80FF80FF;
        const int32_t GAIN_INT_BE_3 = 0x01000100;
        const int32_t GAIN_INT_BE_4 = 0xFFFFFFFF;
        const size_t PACKED_SIZE = 16;

        // Gain 1
        AllCIF0Context packet_in;
        vrtgen::packing::Gain gain;
        gain.setStage1(GAIN_1);
        gain.setStage2(GAIN_1);
        packet_in.setGain(gain);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == GAIN_INT_BE_1);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        AllCIF0Context packet_out;
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasGain());
        CHECK(packet_out.getGain().getStage1() == GAIN_1);
        CHECK(packet_out.getGain().getStage2() == GAIN_1);

        // Gain 2
        packet_in = AllCIF0Context();
        gain.setStage1(GAIN_2);
        gain.setStage2(GAIN_2);
        packet_in.setGain(gain);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == GAIN_INT_BE_2);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasGain());
        CHECK(packet_out.getGain().getStage1() == GAIN_2);
        CHECK(packet_out.getGain().getStage2() == GAIN_2);

        // Gain 3
        packet_in = AllCIF0Context();
        gain.setStage1(GAIN_3);
        gain.setStage2(GAIN_3);
        packet_in.setGain(gain);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == GAIN_INT_BE_3);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasGain());
        CHECK(packet_out.getGain().getStage1() == GAIN_3);
        CHECK(packet_out.getGain().getStage2() == GAIN_3);

        // Gain 4
        packet_in = AllCIF0Context();
        gain.setStage1(GAIN_4);
        gain.setStage2(GAIN_4);
        packet_in.setGain(gain);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == GAIN_INT_BE_4);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasGain());
        CHECK(packet_out.getGain().getStage1() == GAIN_4);
        CHECK(packet_out.getGain().getStage2() == GAIN_4);
    }

    SECTION("Over-Range Count") {
        // Rule 9.10.6-3
        const uint32_t COUNT_1 = 0x12345678;
        const uint32_t COUNT_BE_1 = 0x78563412;
        const size_t PACKED_SIZE = 16;

        // Gain 1
        AllCIF0Context packet_in;
        packet_in.setOverRangeCount(COUNT_1);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == COUNT_BE_1);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        AllCIF0Context packet_out;
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasOverRangeCount());
        CHECK(packet_out.getOverRangeCount() == COUNT_1);
    }

    SECTION("Sample Rate") {
        // Observation 9.5.12-2
        const double SAMPLE_RATE_1 = 1.0; // represents 0x0000 0000 0010 0000
        const double SAMPLE_RATE_2 = 0.95e-6; // represents 0x0000 0000 0000 0001
        //const double SAMPLE_RATE_3 = -1.0; // invalid TODO
        const int64_t SAMPLE_RATE_INT_BE_1 = 0x0000100000000000;
        const int64_t SAMPLE_RATE_INT_BE_2 = 0x0100000000000000;
        const size_t PACKED_SIZE = 20;

        // Sample Rate 1
        AllCIF0Context packet_in;
        packet_in.setSampleRate(SAMPLE_RATE_1);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == SAMPLE_RATE_INT_BE_1);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        AllCIF0Context packet_out;
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasSampleRate());
        CHECK(packet_out.getSampleRate() == SAMPLE_RATE_1);

        // Sample Rate 2
        packet_in = AllCIF0Context();
        packet_in.setSampleRate(SAMPLE_RATE_2);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == SAMPLE_RATE_INT_BE_2);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasSampleRate());
        CHECK(packet_out.getSampleRate() == SAMPLE_RATE_2);

        // Sample Rate 3 (invalid)
        // TODO
    }

    SECTION("Timestamp Adjustment") {
        // Rule 9.7-1, 9.7-2
        // Observation 9.7-4, 9.7-5
        const int64_t TS_ADJUSTMENT_1 = 0x1234567812345678;
        const int64_t TS_ADJUSTMENT_BE_1 = 0x7856341278563412;
        const int64_t TS_ADJUSTMENT_2 = 0x0000000000000001; // 1 femtosecond
        const int64_t TS_ADJUSTMENT_BE_2 = 0x0100000000000000;
        const size_t PACKED_SIZE = 20;

        // Timestamp Adjustment 1
        AllCIF0Context packet_in;
        packet_in.setTimestampAdjustment(TS_ADJUSTMENT_1);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == TS_ADJUSTMENT_BE_1);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        AllCIF0Context packet_out;
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasTimestampAdjustment());
        CHECK(packet_out.getTimestampAdjustment() == TS_ADJUSTMENT_1);

        // Timestamp Adjustment 2
        packet_in = AllCIF0Context();
        packet_in.setTimestampAdjustment(TS_ADJUSTMENT_2);

        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == TS_ADJUSTMENT_BE_2);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasTimestampAdjustment());
        CHECK(packet_out.getTimestampAdjustment() == TS_ADJUSTMENT_2);
    }

    SECTION("Timestamp Calibration Time") {
        // Rule 9.7.3.3-2
        const uint32_t TS_CALIBRATION = 0x12345678;
        const uint32_t TS_CALIBRATION_BE = 0x78563412;
        const size_t PACKED_SIZE = 16;

        AllCIF0Context packet_in;
        packet_in.setTimestampCalibrationTime(TS_CALIBRATION);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TS_CALIBRATION_BE);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        AllCIF0Context packet_out;
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasTimestampCalibrationTime());
        CHECK(packet_out.getTimestampCalibrationTime() == TS_CALIBRATION);
    }

    SECTION("Temperature") {
        // Observation 9.10.5-2
        const float TEMP_1 = 1.0; // represents 0x0000 0040
        const float TEMP_2 = -1.0; // represents 0x0000 FFC0
        const float TEMP_3 = 0.015625; // represents 0x0000 0001
        const float TEMP_4 = -0.015625 ; // represents 0x0000 FFFF
        const uint32_t TEMP_BE_1 = 0x40000000;
        const uint32_t TEMP_BE_2 = 0xC0FF0000;
        const uint32_t TEMP_BE_3 = 0x01000000;
        const uint32_t TEMP_BE_4 = 0xFFFF0000;
        const size_t PACKED_SIZE = 16;

        // Temperature 1
        AllCIF0Context packet_in;
        packet_in.setTemperature(TEMP_1);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TEMP_BE_1);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        AllCIF0Context packet_out;
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasTemperature());
        CHECK(packet_out.getTemperature() == TEMP_1);

        // Temperature 2
        packet_in = AllCIF0Context();
        packet_in.setTemperature(TEMP_2);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TEMP_BE_2);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasTemperature());
        CHECK(packet_out.getTemperature() == TEMP_2);

        // Temperature 3
        packet_in = AllCIF0Context();
        packet_in.setTemperature(TEMP_3);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TEMP_BE_3);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasTemperature());
        CHECK(packet_out.getTemperature() == TEMP_3);

        // Temperature 4
        packet_in = AllCIF0Context();
        packet_in.setTemperature(TEMP_4);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TEMP_BE_4);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        packet_out = AllCIF0Context();
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasTemperature());
        CHECK(packet_out.getTemperature() == TEMP_4);
    }

    SECTION("Device Identifier") {
        const uint32_t OUI = 0x00123456;
        const int16_t DEV_CODE = 0x0000789A;
        const int64_t DEVICE_ID_BE = 0x9A78000056341200;
        const size_t PACKED_SIZE = 20;

        AllCIF0Context packet_in;
        vrtgen::packing::DeviceIdentifier devid;
        devid.setManufacturerOUI(OUI);
        devid.setDeviceCode(DEV_CODE);
        packet_in.setDeviceID(devid);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == DEVICE_ID_BE);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        AllCIF0Context packet_out;
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasDeviceID());
        CHECK(packet_out.getDeviceID().getManufacturerOUI() == OUI);
        CHECK(packet_out.getDeviceID().getDeviceCode() == DEV_CODE);
    }

    SECTION("State/Event Indicators") {
        // TODO - not currently supported
        CHECK(true);
    }

    SECTION("Signal Data Packet Payload Format") {
        // Observation 9.13.3-1
        // Creating 0x1001 1100 0010 0100 0000 0001 1000 0111
        const vrtgen::PackingMethod::Code METHOD = vrtgen::PackingMethod::LINK_EFFICIENT; // 1
        const vrtgen::DataSampleType::Code SAMPLE_TYPE = vrtgen::DataSampleType::REAL; // 00
        const vrtgen::DataItemFormat::Code ITEM_FORMAT = vrtgen::DataItemFormat::IEEE754_SINGLE_PRECISION; // 01110
        const bool REPEAT = false; // 0
        const uint8_t EVENT_TAG_SIZE = 1; // 001
        const uint8_t CHANNEL_TAG_SIZE = 2; // 0010
        const uint8_t FRACTION_SIZE = 0; // 0000
        const size_t PACKING_FIELD_SIZE = 4; // 000100 (-1 = 000011)
        const size_t DATA_ITEM_SIZE = 8; // 001000 (-1 = 000111)
        // Together = 1000 1110 0001 0010 0000 0000 1100 0111
        //          = 8    E    1    2    0    0    C    7
        const uint16_t REPEAT_COUNT = 0x1234; // (-1 = 0x1233)
        const uint16_t VECTOR_SIZE = 0x5678; // (-1 = 0x5677)
        const int64_t FORMAT_INT_BE = 0x77563312C700128E;
        const size_t PACKED_SIZE = 20;

        AllCIF0Context packet_in;
        vrtgen::packing::PayloadFormat format;
        format.setPackingMethod(METHOD);
        format.setRealComplexType(SAMPLE_TYPE);
        format.setDataItemFormat(ITEM_FORMAT);
        format.setSampleComponentRepeatIndicator(REPEAT);
        format.setEventTagSize(EVENT_TAG_SIZE);
        format.setChannelTagSize(CHANNEL_TAG_SIZE);
        format.setDataItemFractionSize(FRACTION_SIZE);
        format.setItemPackingFieldSize(PACKING_FIELD_SIZE);
        format.setDataItemSize(DATA_ITEM_SIZE);
        format.setRepeatCount(REPEAT_COUNT);
        format.setVectorSize(VECTOR_SIZE);
        packet_in.setSignalDataPacketPayloadFormat(format);
        REQUIRE(packing::AllCIF0ContextHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::AllCIF0ContextHelper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == FORMAT_INT_BE);

        CHECK(packing::AllCIF0ContextHelper::match(data.data(), data.size()));

        AllCIF0Context packet_out;
        packing::AllCIF0ContextHelper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasSignalDataPacketPayloadFormat());
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getPackingMethod() == METHOD);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getRealComplexType() == SAMPLE_TYPE);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getDataItemFormat() == ITEM_FORMAT);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getSampleComponentRepeatIndicator() == REPEAT);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getEventTagSize() == EVENT_TAG_SIZE);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getChannelTagSize() == CHANNEL_TAG_SIZE);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getDataItemFractionSize() == FRACTION_SIZE);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getItemPackingFieldSize() == PACKING_FIELD_SIZE);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getDataItemSize() == DATA_ITEM_SIZE);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getRepeatCount() == REPEAT_COUNT);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getVectorSize() == VECTOR_SIZE);
    }

    SECTION("Formatted GPS") {
        CHECK(true);
    }

    SECTION("Formatted INS") {
        CHECK(true);
    }

    SECTION("ECEF Ephemeris") {
        CHECK(true);
    }

    SECTION("Relative Ephemeris") {
        CHECK(true);
    }

    SECTION("Ephemeris Ref ID") {
        CHECK(true);
    }

    SECTION("GPS ASCII") {
        // TODO - not currently supported
        CHECK(true);
    }

    SECTION("Context Association Lists") {
        // TODO  - not currently supported
        CHECK(true);
    }
}
