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
#include "constants.hpp"

TEST_CASE("Basic Data Packet")
{
    using packet_type = BasicDataPacket;
    packet_type packet_in;

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD);

    // Check packet size
    const size_t EXPECTED_SIZE = 4 + // header
                                 4;  // payload
    const size_t PACKED_SIZE = packet_in.size();
    CHECK(PACKED_SIZE == EXPECTED_SIZE);

    // Get underlying data
    auto data = packet_in.data();
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    check_ptr += HEADER_BYTES;

    // Examine and check packed payload
    const size_t PAYLOAD_BYTES = PAYLOAD.size();
    const bytes packed_payload(check_ptr, check_ptr + PAYLOAD_BYTES);
    check_ptr += PAYLOAD_BYTES;
    CHECK(packed_payload == PAYLOAD);

    // Check match
    CHECK_FALSE(packet_type::match(data));

    // Unpack verifed packed data
    bytes packed_bytes(packet_in.size());
    memcpy(packed_bytes.data(), data.data(), data.size());
    packet_type packet_out({ packed_bytes.data(), packed_bytes.size() });

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA);
    CHECK_FALSE(header.class_id_enable());
    CHECK_FALSE(header.trailer_included());
    // NB: As configured, BasicDatatPacket is V49.0-compatible
    CHECK_FALSE(header.not_v49d0());
    CHECK_FALSE(header.spectrum_or_time());
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked payload
    auto unpack_payload = packet_out.payload();
    bytes payload(unpack_payload.begin(), unpack_payload.end());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Basic Data Packet")

TEST_CASE("Basic Context Packet")
{
    using packet_type = BasicContextPacket;
    packet_type packet_in;

    // Stream ID is required field. Set value to check
    const uint32_t STREAM_ID = 0x12345678;
    packet_in.stream_id(STREAM_ID);

    // Check packet size
    const size_t EXPECTED_SIZE = 4 + // header
                                 4 + // stream_id
                                 4;  // cif 0
    const size_t PACKED_SIZE = packet_in.size();
    CHECK(PACKED_SIZE == EXPECTED_SIZE);

    // Get underlying data
    auto data = packet_in.data();
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    
    const bytes packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;

    // Examine and check packed Stream ID. Value shall be in big-endian format.
    const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
    const bytes packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
    check_ptr += STREAM_ID_BYTES;
    CHECK(packed_stream_id == STREAM_ID_BE);

    // Examine and check packed CIF0
    const bytes CIF0_BE{ 0, 0, 0, 0 };
    const bytes packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
    check_ptr += CIF0_BYTES;
    CHECK(packed_cif0 == CIF0_BE);

    // Check match
    CHECK_FALSE(packet_type::match(data));

    // Unpack verifed packed data
    bytes packed_bytes(packet_in.size());
    memcpy(packed_bytes.data(), data.data(), data.size());
    packet_type packet_out({ packed_bytes.data(), packed_bytes.size() });

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
    packet_type packet_in;

    // Stream ID is required field. Set value to check
    const uint32_t STREAM_ID = 0x12345678;
    packet_in.stream_id(STREAM_ID);

    // Message ID is required field. Set value to check
    const uint32_t MESSAGE_ID = 0xABCDEF;
    packet_in.message_id(MESSAGE_ID);

    // Check packet size
    const size_t EXPECTED_SIZE = 4 + // header
                                 4 + // stream_id
                                 4 + // cam
                                 4 + // message_id
                                 4;  // cif 0
    const size_t PACKED_SIZE = packet_in.size();
    CHECK(PACKED_SIZE == EXPECTED_SIZE);

    // Get underlying data
    auto data = packet_in.data();
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    
    const bytes packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;

    // Examine and check packed Stream ID. Value shall be in big-endian format.
    const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
    const bytes packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
    check_ptr += STREAM_ID_BYTES;
    CHECK(packed_stream_id == STREAM_ID_BE);

    // Examine and check packed CAM
    const bytes CAM_BE{ 0, 0, 0, 0 };
    const bytes packed_cam(check_ptr, check_ptr + CAM_BYTES);
    check_ptr += CAM_BYTES;
    CHECK(packed_cam == CAM_BE);

    // Examine and check packed Message ID. Value shall be in big-endian format.
    const bytes MESSAGE_ID_BE{ 0x00, 0xAB, 0xCD, 0xEF };
    const bytes packed_message_id(check_ptr, check_ptr + MESSAGE_ID_BYTES);
    check_ptr += MESSAGE_ID_BYTES;
    CHECK(packed_message_id == MESSAGE_ID_BE);

    // Examine and check packed CIF0
    const bytes CIF0_BE{ 0, 0, 0, 0 };
    const bytes packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
    check_ptr += CIF0_BYTES;
    CHECK(packed_cif0 == CIF0_BE);

    // Check match
    CHECK_FALSE(packet_type::match(data));

    // Unpack verifed packed data
    bytes packed_bytes(packet_in.size());
    memcpy(packed_bytes.data(), data.data(), data.size());
    packet_type packet_out({ packed_bytes.data(), packed_bytes.size() });

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
/*
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
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    
    const bytes packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;

    // Examine and check packed Stream ID. Value shall be in big-endian format.
    const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
    const bytes packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
    check_ptr += STREAM_ID_BYTES;
    CHECK(packed_stream_id == STREAM_ID_BE);

    // Examine and check packed CAM
    const bytes CAM_BE{ 0, 0, 0, 0 };
    const bytes packed_cam(check_ptr, check_ptr + CAM_BYTES);
    check_ptr += CAM_BYTES;
    CHECK(packed_cam == CAM_BE);

    // Examine and check packed Message ID. Value shall be in big-endian format.
    const bytes MESSAGE_ID_BE{ 0x00, 0xAB, 0xCD, 0xEF };
    const bytes packed_message_id(check_ptr, check_ptr + MESSAGE_ID_BYTES);
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
*/
