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
#include "data.hpp"
#include <bytes.hpp>
#include <vrtgen/packing/enums.hpp>

TEST_CASE("Data Packet Stream ID")
{
    using packet_type = TestData1;
    using helper = packet_type::helper;
    packet_type packet_in;

    // Stream ID is required field. Set value to check
    const uint32_t STREAM_ID = 0x12345678;
    packet_in.stream_id(STREAM_ID);

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD.data(), PAYLOAD.size());

    // Check bytes required
    const size_t EXPECTED_SIZE = 4 + // header
                                 4 + // stream_id
                                 4;  // payload
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == EXPECTED_SIZE);

    // Get buffer from pack
    auto data = helper::pack(packet_in);
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const size_t HEADER_BYTES = 4;
    const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID) << 4;
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
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID);
    CHECK_FALSE(header.class_id_enable());
    CHECK_FALSE(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK_FALSE(header.spectrum_or_time());
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked Stream ID
    CHECK(packet_out.stream_id() == STREAM_ID);

    // Examine and check unpacked payload
    decltype(data) payload(packet_out.payload_size());
    std::memcpy(payload.data(), packet_out.payload(), packet_out.payload_size());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Data Packet Stream ID")

TEST_CASE("Data Packet Class ID")
{
    using packet_type = TestData2;
    using helper = packet_type::helper;
    packet_type packet_in;

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD.data(), PAYLOAD.size());

    // Check bytes required
    const size_t EXPECTED_SIZE = 4 + // header
                                 8 + // class_id
                                 4;  // payload
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == EXPECTED_SIZE);

    // Get buffer from pack
    auto data = helper::pack(packet_in);
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const size_t HEADER_BYTES = 4;
    const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::SIGNAL_DATA) << 4;
    const uint8_t CLASS_ID_ENABLE = 0x1 << 3; // C bit 27
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    const bytes HEADER_BE{ PACKET_TYPE | CLASS_ID_ENABLE, 0, 0, PACKET_SIZE };
    const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;
    CHECK(packed_header == HEADER_BE);

    // Examine and check packed Class ID. Value shall be in big-endian format.
    const size_t CLASS_ID_BYTES = 8;
    const bytes CLASS_ID_BE{ 0, 0xFF, 0xEE, 0xDD, 0, 0, 0x12, 0x34 };
    const decltype(data) packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
    check_ptr += CLASS_ID_BYTES;
    CHECK(packed_class_id == CLASS_ID_BE);

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
    CHECK(header.class_id_enable());
    CHECK_FALSE(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK_FALSE(header.spectrum_or_time());
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked Class ID
    CHECK(packet_out.class_id().oui() == 0xFFEEDD);
    CHECK(packet_out.class_id().packet_code() == 0x1234);

    // Examine and check unpacked payload
    decltype(data) payload(packet_out.payload_size());
    std::memcpy(payload.data(), packet_out.payload(), packet_out.payload_size());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Data Packet Class ID")

TEST_CASE("Data Packet Timestamp Integer")
{
    using packet_type = TestData3;
    using helper = packet_type::helper;
    packet_type packet_in;

    // Timestamp is required field. Set value to check
    const uint32_t INTEGER_TS = 0x12345678;
    packet_in.integer_timestamp(INTEGER_TS);

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD.data(), PAYLOAD.size());

    // Check bytes required
    const size_t EXPECTED_SIZE = 4 + // header
                                 4 + // integer timestamp
                                 4;  // payload
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == EXPECTED_SIZE);

    // Get buffer from pack
    auto data = helper::pack(packet_in);
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const size_t HEADER_BYTES = 4;
    const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::SIGNAL_DATA) << 4;
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    const uint8_t TSI_TSF = (static_cast<uint8_t>(vrtgen::packing::TSI::UTC) << 6) | (static_cast<uint8_t>(vrtgen::packing::TSF::NONE) << 4);
    const bytes HEADER_BE{ PACKET_TYPE, TSI_TSF, 0, PACKET_SIZE };
    const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;
    CHECK(packed_header == HEADER_BE);

    // Examine and check packed Fractional Timestamp. Value shall be in big-endian format.
    const size_t INTEGER_TS_BYTES = 4;
    const bytes INTEGER_TS_BE{ 0x12, 0x34, 0x56, 0x78 };
    const decltype(data) packed_integer_ts(check_ptr, check_ptr + INTEGER_TS_BYTES);
    check_ptr += INTEGER_TS_BYTES;
    CHECK(packed_integer_ts == INTEGER_TS_BE);

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
    CHECK_FALSE(header.not_v49d0());
    CHECK_FALSE(header.spectrum_or_time());
    CHECK(header.tsi() == vrtgen::packing::TSI::UTC);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked timestamps
    CHECK(packet_out.integer_timestamp() == INTEGER_TS);

    // Examine and check unpacked payload
    decltype(data) payload(packet_out.payload_size());
    std::memcpy(payload.data(), packet_out.payload(), packet_out.payload_size());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Data Packet Timestamp")

TEST_CASE("Data Packet Timestamp Fractional")
{
    using packet_type = TestData4;
    using helper = packet_type::helper;
    packet_type packet_in;

    // Fractional Timestamp is required field. Set value to check
    const uint64_t FRACTIONAL_TS = 0x00ABCDEF12345678;
    packet_in.fractional_timestamp(FRACTIONAL_TS);

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD.data(), PAYLOAD.size());

    // Check bytes required
    const size_t EXPECTED_SIZE = 4 + // header
                                 8 + // fractional timestamp
                                 4;  // payload
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == EXPECTED_SIZE);

    // Get buffer from pack
    auto data = helper::pack(packet_in);
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const size_t HEADER_BYTES = 4;
    const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::SIGNAL_DATA) << 4;
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    const uint8_t TSI_TSF = (static_cast<uint8_t>(vrtgen::packing::TSI::NONE) << 6) | (static_cast<uint8_t>(vrtgen::packing::TSF::REAL_TIME) << 4);
    const bytes HEADER_BE{ PACKET_TYPE , TSI_TSF, 0, PACKET_SIZE };
    const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;
    CHECK(packed_header == HEADER_BE);

    // Examine and check packed Fractional Timestamp. Value shall be in big-endian format.
    const size_t FRACTIONAL_TS_BYTES = 8;
    const bytes FRACTIONAL_TS_BE{ 0x00, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78 };
    const decltype(data) packed_fractional_ts(check_ptr, check_ptr + FRACTIONAL_TS_BYTES);
    check_ptr += FRACTIONAL_TS_BYTES;
    CHECK(packed_fractional_ts == FRACTIONAL_TS_BE);

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
    CHECK_FALSE(header.not_v49d0());
    CHECK_FALSE(header.spectrum_or_time());
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::REAL_TIME);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked timestamps
    CHECK(packet_out.fractional_timestamp() == FRACTIONAL_TS);

    // Examine and check unpacked payload
    decltype(data) payload(packet_out.payload_size());
    std::memcpy(payload.data(), packet_out.payload(), packet_out.payload_size());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Data Packet Timestamp Fractional")

TEST_CASE("Data Packet Timestamp Full")
{
    using packet_type = TestData5;
    using helper = packet_type::helper;
    packet_type packet_in;

    // Timestamp is required field. Set value to check
    const uint32_t INTEGER_TS = 0x12345678;
    const uint64_t FRACTIONAL_TS = 0xABCDEF12345678;
    packet_in.integer_timestamp(INTEGER_TS);
    packet_in.fractional_timestamp(FRACTIONAL_TS);

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD.data(), PAYLOAD.size());

    // Check bytes required
    const size_t EXPECTED_SIZE = 4  + // header
                                 12 + // timestamp
                                 4;   // payload
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == EXPECTED_SIZE);

    // Get buffer from pack
    auto data = helper::pack(packet_in);
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const size_t HEADER_BYTES = 4;
    const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::SIGNAL_DATA) << 4;
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    const uint8_t TSI_TSF = (static_cast<uint8_t>(vrtgen::packing::TSI::UTC) << 6) | (static_cast<uint8_t>(vrtgen::packing::TSF::REAL_TIME) << 4);
    const bytes HEADER_BE{ PACKET_TYPE , TSI_TSF, 0, PACKET_SIZE };
    const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;
    CHECK(packed_header == HEADER_BE);

    // Examine and check packed Integer Timestamp. Value shall be in big-endian format.
    const size_t INTEGER_TS_BYTES = 4;
    const bytes INTEGER_TS_BE{ 0x12, 0x34, 0x56, 0x78 };
    const decltype(data) packed_integer_ts(check_ptr, check_ptr + INTEGER_TS_BYTES);
    check_ptr += INTEGER_TS_BYTES;
    CHECK(packed_integer_ts == INTEGER_TS_BE);

    // Examine and check packed Fractional Timestamp. Value shall be in big-endian format.
    const size_t FRACTIONAL_TS_BYTES = 8;
    const bytes FRACTIONAL_TS_BE{ 0x00, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78 };
    const decltype(data) packed_fractional_ts(check_ptr, check_ptr + FRACTIONAL_TS_BYTES);
    check_ptr += FRACTIONAL_TS_BYTES;
    CHECK(packed_fractional_ts == FRACTIONAL_TS_BE);

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
    CHECK_FALSE(header.not_v49d0());
    CHECK_FALSE(header.spectrum_or_time());
    CHECK(header.tsi() == vrtgen::packing::TSI::UTC);
    CHECK(header.tsf() == vrtgen::packing::TSF::REAL_TIME);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked timestamps
    CHECK(packet_out.integer_timestamp() == INTEGER_TS);
    CHECK(packet_out.fractional_timestamp() == FRACTIONAL_TS);

    // Examine and check unpacked payload
    decltype(data) payload(packet_out.payload_size());
    std::memcpy(payload.data(), packet_out.payload(), packet_out.payload_size());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Data Packet Timestamp Full")

TEST_CASE("Data Packet Trailer")
{
    using packet_type = TestData6;
    using helper = packet_type::helper;
    packet_type packet_in;

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD.data(), PAYLOAD.size());

    // Check bytes required
    const size_t EXPECTED_SIZE = 4 + // header
                                 4 + // payload
                                 4;  // trailer
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == EXPECTED_SIZE);

    // Get buffer from pack
    auto data = helper::pack(packet_in);
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const size_t HEADER_BYTES = 4;
    const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::SIGNAL_DATA) << 4;
    const uint8_t TRAILER_INCLUDED = 0x1 << 2; // T bit 26
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    const bytes HEADER_BE{ PACKET_TYPE | TRAILER_INCLUDED, 0, 0, PACKET_SIZE };
    const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;
    CHECK(packed_header == HEADER_BE);

    // Examine and check packed payload
    const size_t PAYLOAD_BYTES = PAYLOAD.size();
    const decltype(data) packed_payload(check_ptr, check_ptr + PAYLOAD_BYTES);
    check_ptr += PAYLOAD_BYTES;
    CHECK(packed_payload == PAYLOAD);

    // Examine and check packed trailer
    const size_t TRAILER_BYTES = 4;
    const uint8_t ENABLES = 0;
    const uint8_t INDICATORS = 0;
    const bytes TRAILER_BE{ ENABLES, INDICATORS, 0, 0 };
    const decltype(data) packed_trailer(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += TRAILER_BYTES;
    CHECK(packed_trailer == TRAILER_BE);

    // Check match
    CHECK_FALSE(helper::match(data.data(), data.size()));

    // Unpack verifed packed data
    packet_type packet_out;
    helper::unpack(packet_out, data.data(), data.size());

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA);
    CHECK_FALSE(header.class_id_enable());
    CHECK(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK_FALSE(header.spectrum_or_time());
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked payload
    decltype(data) payload(packet_out.payload_size());
    std::memcpy(payload.data(), packet_out.payload(), packet_out.payload_size());
    CHECK(payload == PAYLOAD);

    // Examine and check unpacked packet trailer
    const auto& trailer = packet_out.trailer();
    const auto* trailer_ptr = reinterpret_cast<const uint8_t*>(&trailer);
    const decltype(data) unpacked_trailer(trailer_ptr, trailer_ptr + TRAILER_BYTES);
    CHECK(unpacked_trailer == TRAILER_BE);

} // end TEST_CASE("Data Packet Trailer")

TEST_CASE("Data Packet Trailer Fields")
{
    using packet_type = TestData7;
    using helper = packet_type::helper;
    packet_type packet_in;

    // Trailer required. Set values to check
    packet_in.valid_data(true);
    packet_in.agc_mgc(true);

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD.data(), PAYLOAD.size());

    // Check bytes required
    const size_t EXPECTED_SIZE = 4 + // header
                                 4 + // payload
                                 4;  // trailer
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == EXPECTED_SIZE);

    // Get buffer from pack
    auto data = helper::pack(packet_in);
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const size_t HEADER_BYTES = 4;
    const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::SIGNAL_DATA) << 4;
    const uint8_t TRAILER_INCLUDED = 0x1 << 2; // T bit 26
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    const bytes HEADER_BE{ PACKET_TYPE | TRAILER_INCLUDED, 0, 0, PACKET_SIZE };
    const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;
    CHECK(packed_header == HEADER_BE);

    // Examine and check packed payload
    const size_t PAYLOAD_BYTES = PAYLOAD.size();
    const decltype(data) packed_payload(check_ptr, check_ptr + PAYLOAD_BYTES);
    check_ptr += PAYLOAD_BYTES;
    CHECK(packed_payload == PAYLOAD);

    // Examine and check packed trailer
    const size_t TRAILER_BYTES = 4;
    const uint8_t ENABLES = (0x1 << 6) | (0x1 << 4); // valid_data_enable @ 30; agc_mgc_enable @ 28
    const uint8_t INDICATORS = (0x1 << 2) | 0x1; // valid_data @ 18; agc_mgc @ 16
    const bytes TRAILER_BE{ ENABLES, INDICATORS, 0, 0 };
    const decltype(data) packed_trailer(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += TRAILER_BYTES;
    CHECK(packed_trailer == TRAILER_BE);

    // Check match
    CHECK_FALSE(helper::match(data.data(), data.size()));

    // Unpack verifed packed data
    packet_type packet_out;
    helper::unpack(packet_out, data.data(), data.size());

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA);
    CHECK_FALSE(header.class_id_enable());
    CHECK(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK_FALSE(header.spectrum_or_time());
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked payload
    decltype(data) payload(packet_out.payload_size());
    std::memcpy(payload.data(), packet_out.payload(), packet_out.payload_size());
    CHECK(payload == PAYLOAD);

    // Examine and check unpacked packet trailer
    const auto& trailer = packet_out.trailer();
    CHECK(packet_out.has_valid_data());
    CHECK(packet_out.valid_data());
    CHECK(trailer.valid_data_enable());
    CHECK(trailer.valid_data());
    CHECK(packet_out.agc_mgc());
    CHECK(trailer.agc_mgc_enable());
    CHECK(trailer.agc_mgc());

} // end TEST_CASE("Data Packet Trailer Fields")

TEST_CASE("Data Packet Both Identifiers")
{
    using packet_type = TestData8;
    using helper = packet_type::helper;
    packet_type packet_in;

    // Stream ID is required field. Set value to check
    const uint32_t STREAM_ID = 0x12345678;
    packet_in.stream_id(STREAM_ID);

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD.data(), PAYLOAD.size());

    // Check bytes required
    const size_t EXPECTED_SIZE = 4 + // header
                                 4 + // stream_id
                                 8 + // class_id
                                 4;  // payload
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == EXPECTED_SIZE);

    // Get buffer from pack
    auto data = helper::pack(packet_in);
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const size_t HEADER_BYTES = 4;
    const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID) << 4;
    const uint8_t CLASS_ID_ENABLE = 0x1 << 3; // C bit 27
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    const bytes HEADER_BE{ PACKET_TYPE | CLASS_ID_ENABLE, 0, 0, PACKET_SIZE };
    const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;
    CHECK(packed_header == HEADER_BE);

    // Examine and check packed Stream ID. Value shall be in big-endian format.
    const size_t STREAM_ID_BYTES = 4;
    const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
    const decltype(data) packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
    check_ptr += STREAM_ID_BYTES;
    CHECK(packed_stream_id == STREAM_ID_BE);

    // Examine and check packed Class ID. Value shall be in big-endian format.
    const size_t CLASS_ID_BYTES = 8;
    const bytes CLASS_ID_BE{ 0, 0xFF, 0xEE, 0xDD, 0, 0, 0x12, 0x34 };
    const decltype(data) packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
    check_ptr += CLASS_ID_BYTES;
    CHECK(packed_class_id == CLASS_ID_BE);

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
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID);
    CHECK(header.class_id_enable());
    CHECK_FALSE(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK_FALSE(header.spectrum_or_time());
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked Stream ID
    CHECK(packet_out.stream_id() == STREAM_ID);

    // Examine and check unpacked Class ID
    CHECK(packet_out.class_id().oui() == 0xFFEEDD);
    CHECK(packet_out.class_id().packet_code() == 0x1234);

    // Examine and check unpacked payload
    decltype(data) payload(packet_out.payload_size());
    std::memcpy(payload.data(), packet_out.payload(), packet_out.payload_size());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Data Packet Both Identifiers")

TEST_CASE("Data Packet Full Prologue")
{
    using packet_type = TestData9;
    using helper = packet_type::helper;
    packet_type packet_in;

    // Stream ID is required field. Set value to check
    const uint32_t STREAM_ID = 0x12345678;
    packet_in.stream_id(STREAM_ID);

    // Timestamp is required field. Set value to check
    const uint32_t INTEGER_TS = 0x12345678;
    const uint64_t FRACTIONAL_TS = 0xABCDEF12345678;
    packet_in.integer_timestamp(INTEGER_TS);
    packet_in.fractional_timestamp(FRACTIONAL_TS);

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD.data(), PAYLOAD.size());

    // Check bytes required
    const size_t EXPECTED_SIZE = 4 +  // header
                                 4 +  // stream_id
                                 8 +  // class_id
                                 12 + // timestamp
                                 4;   // payload
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == EXPECTED_SIZE);

    // Get buffer from pack
    auto data = helper::pack(packet_in);
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const size_t HEADER_BYTES = 4;
    const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID) << 4;
    const uint8_t CLASS_ID_ENABLE = 0x1 << 3; // C bit 27
    const uint8_t TSI_TSF = (static_cast<uint8_t>(vrtgen::packing::TSI::UTC) << 6) | (static_cast<uint8_t>(vrtgen::packing::TSF::REAL_TIME) << 4);
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    const bytes HEADER_BE{ PACKET_TYPE | CLASS_ID_ENABLE, TSI_TSF, 0, PACKET_SIZE };
    const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;
    CHECK(packed_header == HEADER_BE);

    // Examine and check packed Stream ID. Value shall be in big-endian format.
    const size_t STREAM_ID_BYTES = 4;
    const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
    const decltype(data) packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
    check_ptr += STREAM_ID_BYTES;
    CHECK(packed_stream_id == STREAM_ID_BE);

    // Examine and check packed Class ID. Value shall be in big-endian format.
    const size_t CLASS_ID_BYTES = 8;
    const bytes CLASS_ID_BE{ 0, 0xFF, 0xEE, 0xDD, 0, 0, 0x12, 0x34 };
    const decltype(data) packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
    check_ptr += CLASS_ID_BYTES;
    CHECK(packed_class_id == CLASS_ID_BE);

    // Examine and check packed Integer Timestamp. Value shall be in big-endian format.
    const size_t INTEGER_TS_BYTES = 4;
    const bytes INTEGER_TS_BE{ 0x12, 0x34, 0x56, 0x78 };
    const decltype(data) packed_integer_ts(check_ptr, check_ptr + INTEGER_TS_BYTES);
    check_ptr += INTEGER_TS_BYTES;
    CHECK(packed_integer_ts == INTEGER_TS_BE);

    // Examine and check packed Fractional Timestamp. Value shall be in big-endian format.
    const size_t FRACTIONAL_TS_BYTES = 8;
    const bytes FRACTIONAL_TS_BE{ 0x00, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78 };
    const decltype(data) packed_fractional_ts(check_ptr, check_ptr + FRACTIONAL_TS_BYTES);
    check_ptr += FRACTIONAL_TS_BYTES;
    CHECK(packed_fractional_ts == FRACTIONAL_TS_BE);

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
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID);
    CHECK(header.class_id_enable());
    CHECK_FALSE(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK_FALSE(header.spectrum_or_time());
    CHECK(header.tsi() == vrtgen::packing::TSI::UTC);
    CHECK(header.tsf() == vrtgen::packing::TSF::REAL_TIME);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked Stream ID
    CHECK(packet_out.stream_id() == STREAM_ID);

    // Examine and check unpacked Class ID
    CHECK(packet_out.class_id().oui() == 0xFFEEDD);
    CHECK(packet_out.class_id().packet_code() == 0x1234);

    // Examine and check unpacked timestamps
    CHECK(packet_out.integer_timestamp() == INTEGER_TS);
    CHECK(packet_out.fractional_timestamp() == FRACTIONAL_TS);

    // Examine and check unpacked payload
    decltype(data) payload(packet_out.payload_size());
    std::memcpy(payload.data(), packet_out.payload(), packet_out.payload_size());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Data Packet Full Prologue")

TEST_CASE("Data Packet All")
{
    using packet_type = TestData10;
    using helper = packet_type::helper;
    packet_type packet_in;

    // Stream ID is required field. Set value to check
    const uint32_t STREAM_ID = 0x12345678;
    packet_in.stream_id(STREAM_ID);

    // Timestamp is required field. Set value to check
    const uint32_t INTEGER_TS = 0x12345678;
    const uint64_t FRACTIONAL_TS = 0xABCDEF12345678;
    packet_in.integer_timestamp(INTEGER_TS);
    packet_in.fractional_timestamp(FRACTIONAL_TS);

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD.data(), PAYLOAD.size());

    // Trailer required. Set values to check
    packet_in.valid_data(true);
    packet_in.agc_mgc(true);

    // Check bytes required
    const size_t EXPECTED_SIZE = 4 +  // header
                                 4 +  // stream_id
                                 8 +  // class_id
                                 12 + // timestamp
                                 4 +  // payload
                                 4;   // trailer
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == EXPECTED_SIZE);

    // Get buffer from pack
    auto data = helper::pack(packet_in);
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const size_t HEADER_BYTES = 4;
    const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID) << 4;
    const uint8_t CLASS_ID_ENABLE = 0x1 << 3; // C bit 27
    const uint8_t TRAILER_INCLUDED = 0x1 << 2; // T bit 26
    const uint8_t TSI_TSF = (static_cast<uint8_t>(vrtgen::packing::TSI::UTC) << 6) | (static_cast<uint8_t>(vrtgen::packing::TSF::REAL_TIME) << 4);
    const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
    const bytes HEADER_BE{ PACKET_TYPE | CLASS_ID_ENABLE | TRAILER_INCLUDED, TSI_TSF, 0, PACKET_SIZE };
    const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += HEADER_BYTES;
    CHECK(packed_header == HEADER_BE);

    // Examine and check packed Stream ID. Value shall be in big-endian format.
    const size_t STREAM_ID_BYTES = 4;
    const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
    const decltype(data) packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
    check_ptr += STREAM_ID_BYTES;
    CHECK(packed_stream_id == STREAM_ID_BE);

    // Examine and check packed Class ID. Value shall be in big-endian format.
    const size_t CLASS_ID_BYTES = 8;
    const bytes CLASS_ID_BE{ 0, 0xFF, 0xEE, 0xDD, 0, 0, 0x12, 0x34 };
    const decltype(data) packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
    check_ptr += CLASS_ID_BYTES;
    CHECK(packed_class_id == CLASS_ID_BE);

    // Examine and check packed Integer Timestamp. Value shall be in big-endian format.
    const size_t INTEGER_TS_BYTES = 4;
    const bytes INTEGER_TS_BE{ 0x12, 0x34, 0x56, 0x78 };
    const decltype(data) packed_integer_ts(check_ptr, check_ptr + INTEGER_TS_BYTES);
    check_ptr += INTEGER_TS_BYTES;
    CHECK(packed_integer_ts == INTEGER_TS_BE);

    // Examine and check packed Fractional Timestamp. Value shall be in big-endian format.
    const size_t FRACTIONAL_TS_BYTES = 8;
    const bytes FRACTIONAL_TS_BE{ 0x00, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78 };
    const decltype(data) packed_fractional_ts(check_ptr, check_ptr + FRACTIONAL_TS_BYTES);
    check_ptr += FRACTIONAL_TS_BYTES;
    CHECK(packed_fractional_ts == FRACTIONAL_TS_BE);

    // Examine and check packed payload
    const size_t PAYLOAD_BYTES = PAYLOAD.size();
    const decltype(data) packed_payload(check_ptr, check_ptr + PAYLOAD_BYTES);
    check_ptr += PAYLOAD_BYTES;
    CHECK(packed_payload == PAYLOAD);

    // Examine and check packed trailer
    const size_t TRAILER_BYTES = 4;
    const uint8_t ENABLES = (0x1 << 6) | (0x1 << 4); // valid_data_enable @ 30; agc_mgc_enable @ 28
    const uint8_t INDICATORS = (0x1 << 2) | 0x1; // valid_data @ 18; agc_mgc @ 16
    const bytes TRAILER_BE{ ENABLES, INDICATORS, 0, 0 };
    const decltype(data) packed_trailer(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += TRAILER_BYTES;
    CHECK(packed_trailer == TRAILER_BE);

    // Check match
    CHECK_FALSE(helper::match(data.data(), data.size()));

    // Unpack verifed packed data
    packet_type packet_out;
    helper::unpack(packet_out, data.data(), data.size());

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID);
    CHECK(header.class_id_enable());
    CHECK(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK_FALSE(header.spectrum_or_time());
    CHECK(header.tsi() == vrtgen::packing::TSI::UTC);
    CHECK(header.tsf() == vrtgen::packing::TSF::REAL_TIME);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked Stream ID
    CHECK(packet_out.stream_id() == STREAM_ID);

    // Examine and check unpacked Class ID
    CHECK(packet_out.class_id().oui() == 0xFFEEDD);
    CHECK(packet_out.class_id().packet_code() == 0x1234);

    // Examine and check unpacked timestamps
    CHECK(packet_out.integer_timestamp() == INTEGER_TS);
    CHECK(packet_out.fractional_timestamp() == FRACTIONAL_TS);

    // Examine and check unpacked payload
    decltype(data) payload(packet_out.payload_size());
    std::memcpy(payload.data(), packet_out.payload(), packet_out.payload_size());
    CHECK(payload == PAYLOAD);

    // Examine and check unpacked packet trailer
    const auto& trailer = packet_out.trailer();
    CHECK(packet_out.has_valid_data());
    CHECK(packet_out.valid_data());
    CHECK(trailer.valid_data_enable());
    CHECK(trailer.valid_data());
    CHECK(packet_out.agc_mgc());
    CHECK(trailer.agc_mgc_enable());
    CHECK(trailer.agc_mgc());

} // end TEST_CASE("Data Packet Full Prologue")

TEST_CASE("Data Packet Trailer User Defined")
{
    using packet_type = TestData11;
    packet_type packet_in;

    SECTION("Rule 5.1.6-4") {
        test_data11::structs::UserDefinedTrailer trailer;
        test_data11::structs::UserDefinedTrailer unpack_trailer;
        bytes packed_bytes{ 
            0xFF, 0xFF, 0xFF, 0xFF, // Trailer
        };

        trailer.sample_frame_enable(true);
        trailer.user_defined_enable_indicator_enable(true);
        trailer.user_defined_enum_enable(true);
        trailer.sample_frame(vrtgen::packing::SSI(3));
        trailer.user_defined_enable_indicator(true);
        trailer.user_defined_enum(test_data11::enums::user_defined_enum(1));
        CHECK(trailer.sample_frame_enable() == true);
        CHECK(trailer.user_defined_enable_indicator_enable() == true);
        CHECK(trailer.user_defined_enum_enable() == true);
        CHECK(trailer.sample_frame() == vrtgen::packing::SSI(3));
        CHECK(trailer.user_defined_enable_indicator() == true);
        CHECK(trailer.user_defined_enum() == test_data11::enums::user_defined_enum(1));
        trailer.pack_into(packed_bytes.data());
        // enabled the first and the last to verify the range of the state and event indicators
        CHECK(packed_bytes == bytes{ 0x00, 0xF0, 0x0F, 0x00 });

        unpack_trailer.unpack_from(packed_bytes.data());
        CHECK(unpack_trailer.sample_frame_enable() == true);
        CHECK(unpack_trailer.user_defined_enable_indicator_enable() == true);
        CHECK(unpack_trailer.user_defined_enum_enable() == true);
        CHECK(unpack_trailer.sample_frame() == vrtgen::packing::SSI(3));
        CHECK(unpack_trailer.user_defined_enable_indicator() == true);
        CHECK(unpack_trailer.user_defined_enum() == test_data11::enums::user_defined_enum(1));
    }
}