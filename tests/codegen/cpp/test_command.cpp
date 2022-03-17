/*
 * Copyright (C) 2021 Geon Technologies, LLC
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

#include "catch.hpp"

#include "command.hpp"

#include <vector>
#include <bytes.hpp>

TEST_CASE("Command Packet")
{
    using packet_type = SampleControl;
    using helper = packet_type::helper;
    packet_type packet_in;

    // Examine and check packed header
    // const size_t HEADER_BYTES = 4;
    // const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::COMMAND) << 4;
    // const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    // const bytes HEADER_BE{ PACKET_TYPE, 0, 0, PACKET_SIZE };
    // const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
    // check_ptr += HEADER_BYTES;
    // CHECK(packed_header == HEADER_BE);

    // // Check match
    // CHECK(helper::match(data.data(), data.size()));

    // // Unpack verifed packed data
    // packet_type packet_out;
    // helper::unpack(packet_out, data.data(), data.size());

    // // Examine and check unpacked packet header
    // const auto& header = packet_out.header();
    // CHECK(header.packet_type() == vrtgen::packing::PacketType::COMMAND);
    // CHECK_FALSE(header.class_id_enable());
    // CHECK_FALSE(header.trailer_included());
    // CHECK_FALSE(header.acknowledge_packet());
    // CHECK_FALSE(header.cancellation_packet());
    // CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    // CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    // CHECK(header.packet_size() == PACKET_SIZE);

} // end TEST_CASE("Basic Data Packet")
