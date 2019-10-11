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

#include "basic.hpp"

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
    CHECK(header->getNotaV49_0Packet());

    CHECK(packing::BasicContextHelper::match(data.data(), data.size()));

    BasicContext packet_out;
    packing::BasicContextHelper::unpack(packet_out, data.data(), data.size());
    CHECK(packet_out.getStreamIdentifier() == 0x12345678);
    CHECK(packet_out.getReferencePointIdentifier() == 0x10AB7DE9);
}

TEST_CASE("Basic command message") {
    BasicCommand packet_in;
    packet_in.setStreamIdentifier(0x12345678);
    packet_in.setRFReferenceFrequency(101.1e6);

    const size_t PACKED_SIZE = 28;
    REQUIRE(packing::BasicCommandHelper::bytes_required(packet_in) == PACKED_SIZE);

    bytes data;
    data.resize(PACKED_SIZE);
    packing::BasicCommandHelper::pack(packet_in, data.data(), data.size());
    CHECK(range(data, 4, 8) == bytes{0x12, 0x34, 0x56, 0x78});

    const vrtgen::packing::CommandHeader* header = reinterpret_cast<const vrtgen::packing::CommandHeader*>(data.data());
    CHECK(header->getPacketType() == vrtgen::PacketType::COMMAND);
    CHECK(header->getTSI() == vrtgen::TSI::NONE);
    CHECK(header->getTSF() == vrtgen::TSF::NONE);
    CHECK_FALSE(header->isClassIdentifierEnabled());
    CHECK(header->getPacketSize() == (PACKED_SIZE / 4));
    CHECK_FALSE(header->getAcknowledgePacket());
    CHECK_FALSE(header->getCancellationPacket());

    CHECK(packing::BasicCommandHelper::match(data.data(), data.size()));

    BasicCommand packet_out;
    packing::BasicCommandHelper::unpack(packet_out, data.data(), data.size());
    CHECK(packet_out.getStreamIdentifier() == 0x12345678);
    CHECK(packet_out.getRFReferenceFrequency() == 101.1e6);
}