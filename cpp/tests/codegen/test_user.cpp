/*
 * Copyright (C) 2020 Geon Technologies, LLC
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

#include <bytes.hpp>

#include "user.hpp"

TEST_CASE("User-defined Discrete I/O 32") {
    const vrtgen::StreamIdentifier STREAM_ID = 0x39016501;
    const bool FLAG = true;
    const int NUMBER = -2;
    const int SOMETIMES = 1;
    const int ALWAYS = 5;
    // Upper 8 bits are indicators (0xEE):
    //   Flag enable (1)
    //   Number enable (2)
    //   reserved (1)
    //   Flag (1)
    //   Number (2)
    //   reserved (1)
    // Next 4 bits are (0xA):
    //   Sometimes enable (1)
    //   Sometimes (2)
    //   reserved (1)
    // Next 4 bits are (0x5):
    //   Always (4)
    // Remainder are reserved
    const uint32_t DISCRETE_IO_32 = 0xECA50000;
    // Context packet requires 4 words (header, stream ID, CIF0, CIF1), plus 1
    // for Discrete I/O 32
    const size_t PACKED_SIZE = 20;

    bytes expected;
    expected.resize(PACKED_SIZE);
    vrtgen::OutputBuffer buffer(expected.data(), expected.size());
    auto header = buffer.insert<vrtgen::packing::ContextHeader>();
    header->setPacketType(vrtgen::PacketType::CONTEXT);
    header->setPacketSize(PACKED_SIZE / 4);
    header->setNotaV49d0Packet(true);
    buffer.insert<vrtgen::packing::StreamIdentifier>(STREAM_ID);
    auto cif_0 = buffer.insert<vrtgen::packing::CIF0Enables>();
    cif_0->setCIF1Enable(true);
    auto cif_1 = buffer.insert<vrtgen::packing::CIF1Enables>();
    cif_1->setDiscreteIO32Enabled(true);
    buffer.insert<vrtgen::packing::DiscreteIO32>(DISCRETE_IO_32);

    SECTION("Pack") {
        UserDefined packet_in;
        packet_in.setStreamIdentifier(STREAM_ID);
        ::structs::userdefined::DiscreteIO32 dio32;
        dio32.setFlagEnabled(true);
        dio32.setFlag(FLAG);
        dio32.setNumberEnabled(true);
        dio32.setNumber(NUMBER);
        dio32.setSometimesEnabled(true);
        dio32.setSometimes(SOMETIMES);
        dio32.setAlways(ALWAYS);
        packet_in.setDiscreteIO32(dio32);

        REQUIRE(packing::UserDefinedHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::UserDefinedHelper::pack(packet_in, data.data(), data.size());

        REQUIRE(data == expected);
    }

    SECTION("Unpack") {
        CHECK(packing::UserDefinedHelper::match(expected.data(), expected.size()));

        UserDefined packet_out;
        packing::UserDefinedHelper::unpack(packet_out, expected.data(), expected.size());
        CHECK(packet_out.getStreamIdentifier() == STREAM_ID);
        CHECK(packet_out.getDiscreteIO32().getAlways() == ALWAYS);
        REQUIRE(packet_out.getDiscreteIO32().isFlagEnabled());
        REQUIRE(packet_out.getDiscreteIO32().isNumberEnabled());
        REQUIRE(packet_out.getDiscreteIO32().isSometimesEnabled());
        CHECK(packet_out.getDiscreteIO32().getFlag() == FLAG);
        CHECK(packet_out.getDiscreteIO32().getNumber() == NUMBER);
        CHECK(packet_out.getDiscreteIO32().getSometimes() == SOMETIMES);
    }
}
