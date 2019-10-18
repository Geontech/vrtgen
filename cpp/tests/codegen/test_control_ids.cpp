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

#include "control_ids.hpp"

#include "bytes.hpp"

TEST_CASE("Control IDs") {
    const vrtgen::MessageIdentifier STREAM_ID = 0x4D4D4D4D;
    const vrtgen::MessageIdentifier MESSAGE_ID = 0x3C3C3C3C;
    const vrtgen::GenericIdentifier32 CONTROLLEE_ID = 0x1A1A1A1A;
    const vrtgen::GenericIdentifier32 CONTROLLER_ID = 0x2B2B2B2B;
    const size_t PACKED_SIZE = 28;

    bytes expected;
    expected.resize(PACKED_SIZE);
    vrtgen::OutputBuffer buffer(expected.data(), expected.size());
    vrtgen::packing::CommandHeader* header = buffer.insert<vrtgen::packing::CommandHeader>();
    header->setPacketType(vrtgen::PacketType::COMMAND);
    header->setPacketSize(PACKED_SIZE / 4);
    buffer.insert<vrtgen::packing::StreamIdentifier>(STREAM_ID);
    vrtgen::packing::ControlAcknowledgeMode* cam = buffer.insert<vrtgen::packing::ControlAcknowledgeMode>();
    cam->setControlleeEnabled(true);
    cam->setControlleeIdentifierFormat(vrtgen::IdentifierFormat::WORD);
    cam->setControllerEnabled(true);
    cam->setControllerIdentifierFormat(vrtgen::IdentifierFormat::WORD);
    buffer.insert<vrtgen::packing::MessageID>(MESSAGE_ID);
    buffer.insert<vrtgen::packing::ControlleeID>(CONTROLLEE_ID);
    buffer.insert<vrtgen::packing::ControlleeID>(CONTROLLER_ID);

    SECTION("Pack") {
        ControlIDs packet_in;
        packet_in.setStreamIdentifier(STREAM_ID);
        packet_in.setMessageID(MESSAGE_ID);
        packet_in.setControlleeID(CONTROLLEE_ID);
        packet_in.setControllerID(CONTROLLER_ID);

        REQUIRE(packing::ControlIDsHelper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packing::ControlIDsHelper::pack(packet_in, data.data(), data.size());

        REQUIRE(data == expected);
    }

    SECTION("Unpack") {
        CHECK(packing::ControlIDsHelper::match(expected.data(), expected.size()));

        ControlIDs packet_out;
        packing::ControlIDsHelper::unpack(packet_out, expected.data(), expected.size());
        CHECK(packet_out.getStreamIdentifier() == STREAM_ID);
        CHECK(packet_out.getMessageID() == MESSAGE_ID);
        CHECK(packet_out.getControlleeID() == CONTROLLEE_ID);
        CHECK(packet_out.getControllerID() == CONTROLLER_ID);
    }
}