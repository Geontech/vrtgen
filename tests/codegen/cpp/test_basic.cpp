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
#include "bytes.hpp"
#include "basic.hpp"

TEST_CASE("Basic Data Packet")
{
    using packet_type = BasicDataPacket;
    using helper = packing::BasicDataPacketHelper;
    packet_type packet_in;

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD.data(), PAYLOAD.size());

    // Check bytes required
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == 8);

    // Get buffer from pack
    auto data = helper::pack(packet_in);
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const size_t HEADER_BYTES = 4;
    const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::SIGNAL_DATA) << 4;
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    const bytes HEADER_BE{ PACKET_TYPE, 0, 0, PACKET_SIZE };
    const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;
    CHECK(packed_header == HEADER_BE);

    // Examine and check packed payload
    const size_t PAYLOAD_BYTES = PAYLOAD.size();
    const decltype(data) packed_payload(check_ptr, check_ptr + PAYLOAD_BYTES);
    check_ptr += PAYLOAD_BYTES;
    CHECK(packed_payload == PAYLOAD);

    // Check match
    CHECK_FALSE(helper::match(data.data(), data.size()));

    // Unpack verifed packed data
    packet_type packet_out;
    helper::unpack(packet_out, data.data(), data.size());

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA);
    CHECK_FALSE(header.class_id_enable());
    CHECK_FALSE(header.trailer_included());
    // NB: As configured, BasicContextPacket is V49.0-compatible
    CHECK_FALSE(header.not_v49d0());
    CHECK_FALSE(header.spectrum_or_time());
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked payload
    decltype(data) payload(packet_out.payload_size());
    std::memcpy(payload.data(), packet_out.payload(), packet_out.payload_size());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Basic Data Packet")


TEST_CASE("Basic Context Packet")
{
    using packet_type = BasicContextPacket;
    using helper = BasicContextPacket::helper;
    packet_type packet_in;

    // Stream ID is required field. Set value to check
    const uint32_t STREAM_ID = 0x12345678;
    packet_in.stream_id(STREAM_ID);

    // Check bytes required
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == 12);

    // Get buffer from pack
    auto data = helper::pack(packet_in);
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const size_t HEADER_BYTES = 4;
    const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    const bytes HEADER_BE{ PACKET_TYPE, 0, 0, PACKET_SIZE };
    const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;
    CHECK(packed_header == HEADER_BE);

    // Examine and check packed Stream ID. Value shall be in big-endian format.
    const size_t STREAM_ID_BYTES = 4;
    const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
    const decltype(data) packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
    check_ptr += STREAM_ID_BYTES;
    CHECK(packed_stream_id == STREAM_ID_BE);

    // Examine and check packed CIF0
    const size_t CIF0_BYTES = 4;
    const bytes CIF0_BE{ 0, 0, 0, 0 };
    const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
    check_ptr += CIF0_BYTES;
    CHECK(packed_cif0 == CIF0_BE);

    // Check match
    CHECK_FALSE(helper::match(data.data(), data.size()));

    // Unpack verifed packed data
    packet_type packet_out;
    helper::unpack(packet_out, data.data(), data.size());

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
    CHECK_FALSE(header.class_id_enable());
    // NB: As configured, BasicContextPacket is V49.0-compatible
    CHECK_FALSE(header.not_v49d0());
    CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked Stream ID
    CHECK(packet_out.stream_id() == STREAM_ID);

} // end TEST_CASE("Basic Context Packet")

TEST_CASE("Basic Control Packet")
{
    using packet_type = BasicControlPacket;
    using helper = BasicControlPacket::helper;
    packet_type packet_in;

    // Stream ID is required field. Set value to check
    const uint32_t STREAM_ID = 0x12345678;
    packet_in.stream_id(STREAM_ID);

    // Message ID is required field. Set value to check
    const uint32_t MESSAGE_ID = 0xABCDEF;
    packet_in.message_id(MESSAGE_ID);

    // Check bytes required
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == 20);

    // Get buffer from pack
    auto data = helper::pack(packet_in);
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const size_t HEADER_BYTES = 4;
    const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::COMMAND) << 4;
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    const bytes HEADER_BE{ PACKET_TYPE, 0, 0, PACKET_SIZE };
    const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;
    CHECK(packed_header == HEADER_BE);

    // Examine and check packed Stream ID. Value shall be in big-endian format.
    const size_t STREAM_ID_BYTES = 4;
    const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
    const decltype(data) packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
    check_ptr += STREAM_ID_BYTES;
    CHECK(packed_stream_id == STREAM_ID_BE);

    // Examine and check packed CAM
    const size_t CAM_BYTES = 4;
    const bytes CAM_BE{ 0, 0, 0, 0 };
    const decltype(data) packed_cam(check_ptr, check_ptr + CAM_BYTES);
    check_ptr += CAM_BYTES;
    CHECK(packed_cam == CAM_BE);

    // Examine and check packed Message ID. Value shall be in big-endian format.
    const size_t MESSAGE_ID_BYTES = 4;
    const bytes MESSAGE_ID_BE{ 0x00, 0xAB, 0xCD, 0xEF };
    const decltype(data) packed_message_id(check_ptr, check_ptr + MESSAGE_ID_BYTES);
    check_ptr += MESSAGE_ID_BYTES;
    CHECK(packed_message_id == MESSAGE_ID_BE);

    // Examine and check packed CIF0
    const size_t CIF0_BYTES = 4;
    const bytes CIF0_BE{ 0, 0, 0, 0 };
    const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
    check_ptr += CIF0_BYTES;
    CHECK(packed_cif0 == CIF0_BE);

    // Check match
    CHECK_FALSE(helper::match(data.data(), data.size()));

    // Unpack verifed packed data
    packet_type packet_out;
    helper::unpack(packet_out, data.data(), data.size());

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::COMMAND);
    CHECK_FALSE(header.class_id_enable());
    CHECK_FALSE(header.acknowledge_packet());
    CHECK_FALSE(header.cancellation_packet());
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked Stream ID
    CHECK(packet_out.stream_id() == STREAM_ID);

    // Examine and check unpacked Message ID
    CHECK(packet_out.message_id() == MESSAGE_ID);

} // end TEST_CASE("Basic Control Packet")

TEST_CASE("Basic Acknowledge Packet")
{
    using packet_type = BasicControlPacketAck;
    using helper = BasicControlPacketAck::helper;
    packet_type packet_in;

    // Stream ID is required field. Set value to check
    const uint32_t STREAM_ID = 0x12345678;
    packet_in.stream_id(STREAM_ID);

    // Message ID is required field. Set value to check
    const uint32_t MESSAGE_ID = 0xABCDEF;
    packet_in.message_id(MESSAGE_ID);

    // Check bytes required
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == 16);

    // Get buffer from pack
    auto data = helper::pack(packet_in);
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const size_t HEADER_BYTES = 4;
    const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::COMMAND) << 4;
    const uint8_t INDICATORS = 0x1 << 2; // acknowledge bit 26 set to true
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    const bytes HEADER_BE{ PACKET_TYPE | INDICATORS, 0, 0, PACKET_SIZE };
    const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;
    CHECK(packed_header == HEADER_BE);

    // Examine and check packed Stream ID. Value shall be in big-endian format.
    const size_t STREAM_ID_BYTES = 4;
    const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
    const decltype(data) packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
    check_ptr += STREAM_ID_BYTES;
    CHECK(packed_stream_id == STREAM_ID_BE);

    // Examine and check packed CAM
    const size_t CAM_BYTES = 4;
    const bytes CAM_BE{ 0, 0, 0, 0 };
    const decltype(data) packed_cam(check_ptr, check_ptr + CAM_BYTES);
    check_ptr += CAM_BYTES;
    CHECK(packed_cam == CAM_BE);

    // Examine and check packed Message ID. Value shall be in big-endian format.
    const size_t MESSAGE_ID_BYTES = 4;
    const bytes MESSAGE_ID_BE{ 0x00, 0xAB, 0xCD, 0xEF };
    const decltype(data) packed_message_id(check_ptr, check_ptr + MESSAGE_ID_BYTES);
    check_ptr += MESSAGE_ID_BYTES;
    CHECK(packed_message_id == MESSAGE_ID_BE);

    // Check match
    CHECK_FALSE(helper::match(data.data(), data.size()));

    // Unpack verifed packed data
    packet_type packet_out;
    helper::unpack(packet_out, data.data(), data.size());

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::COMMAND);
    CHECK_FALSE(header.class_id_enable());
    CHECK(header.acknowledge_packet());
    CHECK_FALSE(header.cancellation_packet());
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked Stream ID
    CHECK(packet_out.stream_id() == STREAM_ID);

    // Examine and check unpacked Message ID
    CHECK(packet_out.message_id() == MESSAGE_ID);

} // end TEST_CASE("Basic Acknowledge Packet")
