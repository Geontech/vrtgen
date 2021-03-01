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

#include "yamls/structs.hpp"

#include "bytes.hpp"

TEST_CASE("Required default struct")
{
    const vrtgen::MessageIdentifier STREAM_ID = 0x4D4D4D4D;
    const size_t PACKED_SIZE = 16;
    const float GAIN_1 = 2.125;
    const float GAIN_2 = -3.0;

    bytes expected;
    expected.resize(PACKED_SIZE);
    vrtgen::OutputBuffer buffer(expected.data(), expected.size());
    vrtgen::packing::ContextHeader* header = buffer.insert<vrtgen::packing::ContextHeader>();
    header->setPacketType(vrtgen::PacketType::CONTEXT);
    header->setPacketSize(PACKED_SIZE / 4);
    // Packet is not configured with any 49.2 fields
    header->setNotaV49d0Packet(false);
    buffer.insert<vrtgen::packing::StreamIdentifier>(STREAM_ID);
    vrtgen::packing::CIF0Enables* cif_0 = buffer.insert<vrtgen::packing::CIF0Enables>();
    cif_0->setGainEnabled(true);
    vrtgen::packing::Gain* gain = buffer.insert<vrtgen::packing::Gain>();
    gain->setStage1(GAIN_1);
    gain->setStage2(GAIN_2);

    SECTION("Pack") {
        RequiredStruct packet_in;
        packet_in.setStreamIdentifier(STREAM_ID);
        vrtgen::packing::Gain gain;
        gain.setStage1(GAIN_1);
        gain.setStage2(GAIN_2);
        packet_in.setGain(gain);

        REQUIRE(packing::RequiredStructHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::RequiredStructHelper::pack(packet_in, data.data(), data.size());

        REQUIRE(data == expected);
    }

    SECTION("Unpack") {
        CHECK(packing::RequiredStructHelper::match(expected.data(), expected.size()));

        RequiredStruct packet_out;
        packing::RequiredStructHelper::unpack(packet_out, expected.data(), expected.size());
        CHECK(packet_out.getStreamIdentifier() == STREAM_ID);
        CHECK(packet_out.getGain().getStage1() == GAIN_1);
        CHECK(packet_out.getGain().getStage2() == GAIN_2);
    }
}
