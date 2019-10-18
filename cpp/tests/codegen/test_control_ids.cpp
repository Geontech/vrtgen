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
    const vrtgen::MessageIdentifier MESSAGE_ID = 0x3C3C3C3C;
    const vrtgen::GenericIdentifier32 CONTROLLEE_ID = 0x1A1A1A1A;
    const vrtgen::GenericIdentifier32 CONTROLLER_ID = 0x2B2B2B2B;
    ControlIDs packet_in;
    packet_in.setMessageID(MESSAGE_ID);
    packet_in.setControlleeID(CONTROLLEE_ID);
    packet_in.setControllerID(CONTROLLER_ID);

    const size_t PACKED_SIZE = 28;
    REQUIRE(packing::ControlIDsHelper::bytes_required(packet_in) == PACKED_SIZE);

    bytes data;
    data.resize(PACKED_SIZE);
    packing::ControlIDsHelper::pack(packet_in, data.data(), data.size());

    const vrtgen::packing::CommandHeader* header = reinterpret_cast<const vrtgen::packing::CommandHeader*>(data.data());
    CHECK(header->getPacketType() == vrtgen::PacketType::COMMAND);
    // CAM field should be at offset 8
    const vrtgen::packing::ControlAcknowledgeMode* cam = reinterpret_cast<const vrtgen::packing::ControlAcknowledgeMode*>(&data[8]);
    CHECK(cam->isControlleeEnabled());
    CHECK(cam->getControlleeIdentifierFormat() == vrtgen::IdentifierFormat::WORD);
    CHECK(cam->isControllerEnabled());
    CHECK(cam->getControllerIdentifierFormat() == vrtgen::IdentifierFormat::WORD);
    // Controllee ID should be at offset 16
    CHECK(reinterpret_cast<const vrtgen::packing::ControlleeID*>(&data[16])->get() == CONTROLLEE_ID);
    // Controller ID should be at offset 20
    CHECK(reinterpret_cast<const vrtgen::packing::ControllerID*>(&data[20])->get() == CONTROLLER_ID);

    CHECK(packing::ControlIDsHelper::match(data.data(), data.size()));

    ControlIDs packet_out;
    packing::ControlIDsHelper::unpack(packet_out, data.data(), data.size());
    CHECK(packet_out.getMessageID() == MESSAGE_ID);
    CHECK(packet_out.getControlleeID() == CONTROLLEE_ID);
    CHECK(packet_out.getControllerID() == CONTROLLER_ID);
}