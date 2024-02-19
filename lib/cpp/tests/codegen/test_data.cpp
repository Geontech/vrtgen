/*
 * Copyright (C) 2024 Geon Technologies, LLC
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

#include <catch2/catch_test_macros.hpp>
#include <bytes.hpp>
#include <vrtgen/packing/enums.hpp>

#include "data/test_data1.hpp"
#include "data/test_data2.hpp"
#include "data/test_data3.hpp"
#include "data/test_data4.hpp"
#include "data/test_data5.hpp"
#include "data/test_data6.hpp"
#include "data/test_data7.hpp"
#include "data/test_data8.hpp"
#include "data/test_data9.hpp"
#include "data/test_data10.hpp"
#include "data/test_data12.hpp"
#include "data/test_data13.hpp"
#include "stream_id/without_stream_id_data.hpp"
#include "stream_id/with_stream_id_data.hpp"
#include "constants.hpp"

using namespace data_ns::packets;
using namespace stream_id_ns::packets;

TEST_CASE("Section 6.1", "[data_packet][6.1]")
{
    SECTION("Rule 6.1-1")
    {
        using packet_type = TestData10;
        
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
        packet_in.payload(PAYLOAD);

        // Trailer required. Set values to check
        packet_in.trailer().valid_data(true);
        packet_in.trailer().agc_mgc(true);

        // Check bytes required
        const size_t EXPECTED_SIZE = 4 +  // header
                                    4 +  // stream_id
                                    8 +  // class_id
                                    12 + // timestamp
                                    4 +  // payload
                                    4;   // trailer
        const size_t PACKED_SIZE = packet_in.size();
        CHECK(PACKED_SIZE == EXPECTED_SIZE);

        // Get buffer from pack
        auto data = packet_in.data();
        CHECK(data.size() == PACKED_SIZE);
        auto* check_ptr = data.data();

        // Examine and check packed header
        const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
        check_ptr += HEADER_BYTES;

        // Examine and check packed Stream ID. Value shall be in big-endian format.
        const size_t STREAM_ID_BYTES = 4;
        const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
        const bytes packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
        check_ptr += STREAM_ID_BYTES;
        CHECK(packed_stream_id == STREAM_ID_BE);

        // Examine and check packed Class ID. Value shall be in big-endian format.
        const size_t CLASS_ID_BYTES = 8;
        const bytes CLASS_ID_BE{ 0, 0xFF, 0xEE, 0xDD, 0, 0, 0x12, 0x34 };
        const bytes packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
        check_ptr += CLASS_ID_BYTES;
        CHECK(packed_class_id == CLASS_ID_BE);

        // Examine and check packed Integer Timestamp. Value shall be in big-endian format.
        const size_t INTEGER_TS_BYTES = 4;
        const bytes INTEGER_TS_BE{ 0x12, 0x34, 0x56, 0x78 };
        const bytes packed_integer_ts(check_ptr, check_ptr + INTEGER_TS_BYTES);
        check_ptr += INTEGER_TS_BYTES;
        CHECK(packed_integer_ts == INTEGER_TS_BE);

        // Examine and check packed Fractional Timestamp. Value shall be in big-endian format.
        const size_t FRACTIONAL_TS_BYTES = 8;
        const bytes FRACTIONAL_TS_BE{ 0x00, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78 };
        const bytes packed_fractional_ts(check_ptr, check_ptr + FRACTIONAL_TS_BYTES);
        check_ptr += FRACTIONAL_TS_BYTES;
        CHECK(packed_fractional_ts == FRACTIONAL_TS_BE);

        // Examine and check packed payload
        const size_t PAYLOAD_BYTES = PAYLOAD.size();
        const bytes packed_payload(check_ptr, check_ptr + PAYLOAD_BYTES);
        check_ptr += PAYLOAD_BYTES;
        CHECK(packed_payload == PAYLOAD);

        // Examine and check packed trailer
        const size_t TRAILER_BYTES = 4;
        const uint8_t ENABLES = (0x1 << 6) | (0x1 << 4); // valid_data_enable @ 30; agc_mgc_enable @ 28
        const uint8_t INDICATORS = (0x1 << 2) | 0x1; // valid_data @ 18; agc_mgc @ 16
        const bytes TRAILER_BE{ ENABLES, INDICATORS, 0, 0 };
        const bytes packed_trailer(check_ptr, check_ptr + HEADER_BYTES);
        check_ptr += TRAILER_BYTES;
        CHECK(packed_trailer == TRAILER_BE);

        // Check match
        CHECK_FALSE(packet_in.match(data));

        // Unpack verifed packed data
        packet_type packet_out(data);

        // Examine and check unpacked packet header
        const auto& header = packet_out.header();
        CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID);
        CHECK(header.class_id_enable());
        CHECK(header.trailer_included());
        CHECK_FALSE(header.not_v49d0());
        CHECK(header.spectrum_or_time() == vrtgen::packing::SpectrumOrTime::TIME);
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
        bytes payload(packet_out.payload_size());
        std::memcpy(payload.data(), packet_out.payload().data(), packet_out.payload_size());
        CHECK(payload == PAYLOAD);

        // Examine and check unpacked packet trailer
        const auto& trailer = packet_out.trailer();
        
        CHECK(trailer.valid_data().has_value());
        CHECK(trailer.valid_data().value());
        CHECK(trailer.agc_mgc().has_value());
        CHECK(trailer.agc_mgc().value());
    }

    SECTION("Rule 6.1-3")
    {
        // Stream ID Consistently Omitted/Included - "Consistency" is up to the user to design the yaml correctly
        uint8_t PACKET_TYPE;

        SECTION("Data Packet without Stream ID")
        {
            WithoutStreamIdData packet_in;
            PACKET_TYPE = (0b0000) << 4;
            auto data = packet_in.data();
            auto* check_ptr = data.data();

            const bytes HEADER_BE{ PACKET_TYPE };
            const bytes packet_type(check_ptr, check_ptr + 1);
            check_ptr += HEADER_BYTES;
            CHECK(packet_type == HEADER_BE);

            WithoutStreamIdData packet_out(data);
            CHECK(packet_out.header().packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA);
        }

        SECTION("Data Packet with Stream ID")
        {
            WithStreamIdData packet_in;
            PACKET_TYPE = (0b0001) << 4;
            const bytes HEADER_BE{ PACKET_TYPE };
            auto data = packet_in.data();
            auto* check_ptr = data.data();

            const bytes packet_type(check_ptr, check_ptr + 1);
            check_ptr += HEADER_BYTES;
            CHECK(packet_type == HEADER_BE);

            WithStreamIdData packet_out(data);
            CHECK(packet_out.header().packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID);
        }
    }

    // Issue #66
    // SECTION("Rule 6.1.1-1")
    // {
    //     TestData1 packet_in;
    //     bytes payload;
    //     size_t prologue_size = packet_in.header().size() + sizeof(packet_in.stream_id());
    //     size_t payload_size = ((65535*4) - prologue_size) + 15; // (2^16-1) 32-bit words minux the header and optional params
    //     payload.resize(payload_size);
    //     REQUIRE_THROWS(packet_in.payload(payload);
    // }

    SECTION("Rule 6.1.1-2")
    {
        TestData1 packet_in;
        bytes payload;
        size_t payload_size = 16;
        payload.resize(payload_size);
        packet_in.payload(payload);
        CHECK(packet_in.payload_size() == payload_size);
        payload.resize(32);
        packet_in.payload(payload);
        CHECK(packet_in.payload_size() == 32);
    }

    SECTION("Rule 6.1.1-3")
    {
        TestData1 packet_in;
        bytes payload{ 1,2,3,4,5,6,7,8,9,10 };
        bytes payload_padded{ 1,2,3,4,5,6,7,8,9,10,0,0 };
        packet_in.payload(payload);
        CHECK(packet_in.payload_size() == payload_padded.size());
        bytes packed_payload(packet_in.payload_size());
        std::memcpy(packed_payload.data(), packet_in.payload().data(), packet_in.payload_size());
        CHECK(packed_payload == payload_padded);
    }
}

TEST_CASE("Section 6.2", "[data_packet][6.2]")
{
    SECTION("Rule 6.2-1")
    {
        TestData12 packet_in;
        CHECK(packet_in.header().spectrum_or_time() == vrtgen::packing::SpectrumOrTime::TIME);

        auto data = packet_in.data();

        TestData12 packet_out(data);
        CHECK(packet_out.header().spectrum_or_time() == vrtgen::packing::SpectrumOrTime::TIME);
    }
}

TEST_CASE("Section 6.3", "[data_packet][6.3]")
{
    SECTION("Rule 6.3.1-2")
    {
        TestData13 packet_in;
        // FIXME #43
        // CHECK(packet_in.header().spectrum_or_time() == vrtgen::packing::SpectrumOrTime::SPECTRUM);

        auto data = packet_in.data();

        TestData13 packet_out(data);
        CHECK(packet_out.header().spectrum_or_time() == vrtgen::packing::SpectrumOrTime::SPECTRUM);
    }
}






/////////////////////////// LEGACY //////////////////////////////

TEST_CASE("Data Packet Stream ID")
{
    using packet_type = TestData1;
    packet_type packet_in;

    // Stream ID is required field. Set value to check
    const uint32_t STREAM_ID = 0x12345678;
    packet_in.stream_id(STREAM_ID);

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD);

    // Check packet size
    const size_t EXPECTED_SIZE = HEADER_BYTES +
                                 STREAM_ID_BYTES +
                                 PAYLOAD.size();
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
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID);
    CHECK_FALSE(header.class_id_enable());
    CHECK_FALSE(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK(header.spectrum_or_time() == vrtgen::packing::SpectrumOrTime::TIME);
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked Stream ID
    CHECK(packet_out.stream_id() == STREAM_ID);

    // Examine and check unpacked payload
    auto unpack_payload = packet_out.payload();
    bytes payload(unpack_payload.begin(), unpack_payload.end());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Data Packet Stream ID")

TEST_CASE("Data Packet Class ID")
{
    using packet_type = TestData2;
    packet_type packet_in;

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD);

    // Check packet size
    const size_t EXPECTED_SIZE = HEADER_BYTES +
                                 CLASS_ID_BYTES +
                                 PAYLOAD.size();
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

    // Examine and check packed Class ID. Value shall be in big-endian format.
    const bytes CLASS_ID_BE{ 0, 0xFF, 0xEE, 0xDD, 0, 0, 0x12, 0x34 };
    const bytes packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
    check_ptr += CLASS_ID_BYTES;
    CHECK(packed_class_id == CLASS_ID_BE);

    // Examine and check packed payload
    const size_t PAYLOAD_BYTES = PAYLOAD.size();
    const bytes packed_payload(check_ptr, check_ptr + PAYLOAD_BYTES);
    check_ptr += PAYLOAD_BYTES;
    CHECK(packed_payload == PAYLOAD);

    // Check match
    CHECK_FALSE(packet_type::match(data));

    // Unpack verifed packed data
    bytes packed_bytes(data.size());
    memcpy(packed_bytes.data(), data.data(), data.size());
    packet_type packet_out({ packed_bytes.data(), packed_bytes.size() });

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA);
    CHECK(header.class_id_enable());
    CHECK_FALSE(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK(header.spectrum_or_time() == vrtgen::packing::SpectrumOrTime::TIME);
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked Class ID
    CHECK(packet_out.class_id().oui() == 0xFFEEDD);
    CHECK(packet_out.class_id().packet_code() == 0x1234);

    // Examine and check unpacked payload
    auto unpack_payload = packet_out.payload();
    bytes payload(unpack_payload.begin(), unpack_payload.end());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Data Packet Class ID")

TEST_CASE("Data Packet Timestamp Integer")
{
    using packet_type = TestData3;
    packet_type packet_in;

    // Timestamp is required field. Set value to check
    const uint32_t INTEGER_TS = 0x12345678;
    packet_in.integer_timestamp(INTEGER_TS);

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD);

    // Check bytes required
    const size_t EXPECTED_SIZE = HEADER_BYTES +
                                 4 + // integer timestamp
                                 PAYLOAD.size();
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

    // Examine and check packed Fractional Timestamp. Value shall be in big-endian format.
    const bytes INTEGER_TS_BE{ 0x12, 0x34, 0x56, 0x78 };
    const bytes packed_integer_ts(check_ptr, check_ptr + INTEGER_TS_BYTES);
    check_ptr += INTEGER_TS_BYTES;
    CHECK(packed_integer_ts == INTEGER_TS_BE);

    // Examine and check packed payload
    const size_t PAYLOAD_BYTES = PAYLOAD.size();
    const bytes packed_payload(check_ptr, check_ptr + PAYLOAD_BYTES);
    check_ptr += PAYLOAD_BYTES;
    CHECK(packed_payload == PAYLOAD);

    // Check match
    CHECK_FALSE(packet_type::match(data));

    // Unpack verifed packed data
    bytes packed_bytes(data.size());
    memcpy(packed_bytes.data(), data.data(), data.size());
    packet_type packet_out({ packed_bytes.data(), packed_bytes.size() });

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA);
    CHECK_FALSE(header.class_id_enable());
    CHECK_FALSE(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK(header.spectrum_or_time() == vrtgen::packing::SpectrumOrTime::TIME);
    CHECK(header.tsi() == vrtgen::packing::TSI::UTC);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked timestamps
    CHECK(packet_out.integer_timestamp() == INTEGER_TS);

    // Examine and check unpacked payload
    auto unpack_payload = packet_out.payload();
    bytes payload(unpack_payload.begin(), unpack_payload.end());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Data Packet Timestamp")

TEST_CASE("Data Packet Timestamp Fractional")
{
    using packet_type = TestData4;
    packet_type packet_in;

    // Fractional Timestamp is required field. Set value to check
    const uint64_t FRACTIONAL_TS = 0x00ABCDEF12345678;
    packet_in.fractional_timestamp(FRACTIONAL_TS);

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD);

    // Check packet size
    const size_t EXPECTED_SIZE = HEADER_BYTES +
                                 8 + // fractional timestamp
                                 PAYLOAD.size();
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

    // Examine and check packed Fractional Timestamp. Value shall be in big-endian format.
    const bytes FRACTIONAL_TS_BE{ 0x00, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78 };
    const bytes packed_fractional_ts(check_ptr, check_ptr + FRACTIONAL_TS_BYTES);
    check_ptr += FRACTIONAL_TS_BYTES;
    CHECK(packed_fractional_ts == FRACTIONAL_TS_BE);

    // Examine and check packed payload
    const size_t PAYLOAD_BYTES = PAYLOAD.size();
    const bytes packed_payload(check_ptr, check_ptr + PAYLOAD_BYTES);
    check_ptr += PAYLOAD_BYTES;
    CHECK(packed_payload == PAYLOAD);

    // Check match
    CHECK_FALSE(packet_type::match(data));

    // Unpack verifed packed data
    bytes packed_bytes(data.size());
    memcpy(packed_bytes.data(), data.data(), data.size());
    packet_type packet_out({ packed_bytes.data(), packed_bytes.size() });

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA);
    CHECK_FALSE(header.class_id_enable());
    CHECK_FALSE(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK(header.spectrum_or_time() == vrtgen::packing::SpectrumOrTime::TIME);
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::REAL_TIME);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked timestamps
    CHECK(packet_out.fractional_timestamp() == FRACTIONAL_TS);

    // Examine and check unpacked payload
    auto unpack_payload = packet_out.payload();
    bytes payload(unpack_payload.begin(), unpack_payload.end());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Data Packet Timestamp Fractional")

TEST_CASE("Data Packet Timestamp Full")
{
    using packet_type = TestData5;
    packet_type packet_in;

    // Timestamp is required field. Set value to check
    const uint32_t INTEGER_TS = 0x12345678;
    const uint64_t FRACTIONAL_TS = 0xABCDEF12345678;
    packet_in.integer_timestamp(INTEGER_TS);
    packet_in.fractional_timestamp(FRACTIONAL_TS);

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD);

    // Check packet size
    const size_t EXPECTED_SIZE = 4  + // header
                                 12 + // timestamp
                                 PAYLOAD.size();
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

    // Examine and check packed Integer Timestamp. Value shall be in big-endian format.
    const bytes INTEGER_TS_BE{ 0x12, 0x34, 0x56, 0x78 };
    const bytes packed_integer_ts(check_ptr, check_ptr + INTEGER_TS_BYTES);
    check_ptr += INTEGER_TS_BYTES;
    CHECK(packed_integer_ts == INTEGER_TS_BE);

    // Examine and check packed Fractional Timestamp. Value shall be in big-endian format.
    const bytes FRACTIONAL_TS_BE{ 0x00, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78 };
    const bytes packed_fractional_ts(check_ptr, check_ptr + FRACTIONAL_TS_BYTES);
    check_ptr += FRACTIONAL_TS_BYTES;
    CHECK(packed_fractional_ts == FRACTIONAL_TS_BE);

    // Examine and check packed payload
    const size_t PAYLOAD_BYTES = PAYLOAD.size();
    const bytes packed_payload(check_ptr, check_ptr + PAYLOAD_BYTES);
    check_ptr += PAYLOAD_BYTES;
    CHECK(packed_payload == PAYLOAD);

    // Check match
    CHECK_FALSE(packet_type::match(data));

    // Unpack verifed packed data
    bytes packed_bytes(data.size());
    memcpy(packed_bytes.data(), data.data(), data.size());
    packet_type packet_out({ packed_bytes.data(), packed_bytes.size() });

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA);
    CHECK_FALSE(header.class_id_enable());
    CHECK_FALSE(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK(header.spectrum_or_time() == vrtgen::packing::SpectrumOrTime::TIME);
    CHECK(header.tsi() == vrtgen::packing::TSI::UTC);
    CHECK(header.tsf() == vrtgen::packing::TSF::REAL_TIME);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked timestamps
    CHECK(packet_out.integer_timestamp() == INTEGER_TS);
    CHECK(packet_out.fractional_timestamp() == FRACTIONAL_TS);

    // Examine and check unpacked payload
    auto unpack_payload = packet_out.payload();
    bytes payload(unpack_payload.begin(), unpack_payload.end());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Data Packet Timestamp Full")

TEST_CASE("Data Packet Trailer")
{
    using packet_type = TestData6;
    packet_type packet_in;

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD);

    // Check bytes required
    const size_t EXPECTED_SIZE = HEADER_BYTES +
                                 4 + // payload
                                 4;  // trailer
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

    // Examine and check packed payload
    const size_t PAYLOAD_BYTES = PAYLOAD.size();
    const bytes packed_payload(check_ptr, check_ptr + PAYLOAD_BYTES);
    check_ptr += PAYLOAD_BYTES;
    CHECK(packed_payload == PAYLOAD);

    // Examine and check packed trailer
    const uint8_t ENABLES = 0;
    const uint8_t INDICATORS = 0;
    const bytes TRAILER_BE{ ENABLES, INDICATORS, 0, 0 };
    const bytes packed_trailer(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += TRAILER_BYTES;
    CHECK(packed_trailer == TRAILER_BE);

    // Check match
    CHECK_FALSE(packet_type::match(data));

    // Unpack verifed packed data
    bytes packed_bytes(data.size());
    memcpy(packed_bytes.data(), data.data(), data.size());
    packet_type packet_out({ packed_bytes.data(), packed_bytes.size() });

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA);
    CHECK_FALSE(header.class_id_enable());
    CHECK(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK(header.spectrum_or_time() == vrtgen::packing::SpectrumOrTime::TIME);
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked payload
    auto unpack_payload = packet_out.payload();
    bytes payload(unpack_payload.begin(), unpack_payload.end());
    CHECK(payload == PAYLOAD);

    // Examine and check unpacked packet trailer
    const auto& trailer = packet_out.trailer();
    const auto* trailer_ptr = reinterpret_cast<const uint8_t*>(&trailer);
    const bytes unpacked_trailer(trailer_ptr, trailer_ptr + TRAILER_BYTES);
    CHECK(unpacked_trailer == TRAILER_BE);

} // end TEST_CASE("Data Packet Trailer")

TEST_CASE("Data Packet Trailer Fields")
{
    using packet_type = TestData7;
    packet_type packet_in;

    // Trailer required. Set values to check
    packet_in.trailer().valid_data(true);
    packet_in.trailer().agc_mgc(true);

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD);

    // Check bytes required
    const size_t EXPECTED_SIZE = HEADER_BYTES +
                                 4 + // payload
                                 4;  // trailer
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

    // Examine and check packed payload
    const size_t PAYLOAD_BYTES = PAYLOAD.size();
    const bytes packed_payload(check_ptr, check_ptr + PAYLOAD_BYTES);
    check_ptr += PAYLOAD_BYTES;
    CHECK(packed_payload == PAYLOAD);

    // Examine and check packed trailer
    const uint8_t ENABLES = (0x1 << 6) | (0x1 << 4); // valid_data_enable @ 30; agc_mgc_enable @ 28
    const uint8_t INDICATORS = (0x1 << 2) | 0x1; // valid_data @ 18; agc_mgc @ 16
    const bytes TRAILER_BE{ ENABLES, INDICATORS, 0, 0 };
    const bytes packed_trailer(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += TRAILER_BYTES;
    CHECK(packed_trailer == TRAILER_BE);

    // Check match
    CHECK_FALSE(packet_type::match(data));

    // Unpack verifed packed data
    bytes packed_bytes(data.size());
    memcpy(packed_bytes.data(), data.data(), data.size());
    packet_type packet_out({ packed_bytes.data(), packed_bytes.size() });

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA);
    CHECK_FALSE(header.class_id_enable());
    CHECK(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK(header.spectrum_or_time() == vrtgen::packing::SpectrumOrTime::TIME);
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked payload
    auto unpack_payload = packet_out.payload();
    bytes payload(unpack_payload.begin(), unpack_payload.end());
    CHECK(payload == PAYLOAD);

    // Examine and check unpacked packet trailer
    const auto& trailer = packet_out.trailer();
    CHECK(trailer.valid_data().has_value());
    CHECK(trailer.valid_data().value());
    CHECK(trailer.agc_mgc().has_value());
    CHECK(trailer.agc_mgc().value());

} // end TEST_CASE("Data Packet Trailer Fields")

TEST_CASE("Data Packet Both Identifiers")
{
    using packet_type = TestData8;
    packet_type packet_in;

    // Stream ID is required field. Set value to check
    const uint32_t STREAM_ID = 0x12345678;
    packet_in.stream_id(STREAM_ID);

    // Set small payload to verify
    const bytes PAYLOAD{ 0x12, 0x34, 0x56, 0x78 };
    packet_in.payload(PAYLOAD);

    // Check packet size
    const size_t EXPECTED_SIZE = HEADER_BYTES +
                                 STREAM_ID_BYTES +
                                 CLASS_ID_BYTES +
                                 PAYLOAD.size();
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

    // Examine and check packed Class ID. Value shall be in big-endian format.
    const bytes CLASS_ID_BE{ 0, 0xFF, 0xEE, 0xDD, 0, 0, 0x12, 0x34 };
    const bytes packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
    check_ptr += CLASS_ID_BYTES;
    CHECK(packed_class_id == CLASS_ID_BE);

    // Examine and check packed payload
    const size_t PAYLOAD_BYTES = PAYLOAD.size();
    const bytes packed_payload(check_ptr, check_ptr + PAYLOAD_BYTES);
    check_ptr += PAYLOAD_BYTES;
    CHECK(packed_payload == PAYLOAD);

    // Check match
    CHECK_FALSE(packet_type::match(data));

    // Unpack verifed packed data
    bytes packed_bytes(data.size());
    memcpy(packed_bytes.data(), data.data(), data.size());
    packet_type packet_out({ packed_bytes.data(), packed_bytes.size() });

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID);
    CHECK(header.class_id_enable());
    CHECK_FALSE(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK(header.spectrum_or_time() == vrtgen::packing::SpectrumOrTime::TIME);
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked Stream ID
    CHECK(packet_out.stream_id() == STREAM_ID);

    // Examine and check unpacked Class ID
    CHECK(packet_out.class_id().oui() == 0xFFEEDD);
    CHECK(packet_out.class_id().packet_code() == 0x1234);

    // Examine and check unpacked payload
    auto unpack_payload = packet_out.payload();
    bytes payload(unpack_payload.begin(), unpack_payload.end());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Data Packet Both Identifiers")

TEST_CASE("Data Packet Full Prologue")
{
    using packet_type = TestData9;
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
    packet_in.payload(PAYLOAD);

    // Check packet size
    const size_t EXPECTED_SIZE = 4 +  // header
                                 4 +  // stream_id
                                 8 +  // class_id
                                 12 + // timestamp
                                 PAYLOAD.size();
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

    // Examine and check packed Class ID. Value shall be in big-endian format.
    const bytes CLASS_ID_BE{ 0, 0xFF, 0xEE, 0xDD, 0, 0, 0x12, 0x34 };
    const bytes packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
    check_ptr += CLASS_ID_BYTES;
    CHECK(packed_class_id == CLASS_ID_BE);

    // Examine and check packed Integer Timestamp. Value shall be in big-endian format.
    const bytes INTEGER_TS_BE{ 0x12, 0x34, 0x56, 0x78 };
    const bytes packed_integer_ts(check_ptr, check_ptr + INTEGER_TS_BYTES);
    check_ptr += INTEGER_TS_BYTES;
    CHECK(packed_integer_ts == INTEGER_TS_BE);

    // Examine and check packed Fractional Timestamp. Value shall be in big-endian format.
    const bytes FRACTIONAL_TS_BE{ 0x00, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78 };
    const bytes packed_fractional_ts(check_ptr, check_ptr + FRACTIONAL_TS_BYTES);
    check_ptr += FRACTIONAL_TS_BYTES;
    CHECK(packed_fractional_ts == FRACTIONAL_TS_BE);

    // Examine and check packed payload
    const size_t PAYLOAD_BYTES = PAYLOAD.size();
    const bytes packed_payload(check_ptr, check_ptr + PAYLOAD_BYTES);
    check_ptr += PAYLOAD_BYTES;
    CHECK(packed_payload == PAYLOAD);

    // Check match
    CHECK_FALSE(packet_type::match(data));

    // Unpack verifed packed data
    bytes packed_bytes(data.size());
    memcpy(packed_bytes.data(), data.data(), data.size());
    packet_type packet_out({ packed_bytes.data(), packed_bytes.size() });

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID);
    CHECK(header.class_id_enable());
    CHECK_FALSE(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK(header.spectrum_or_time() == vrtgen::packing::SpectrumOrTime::TIME);
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
    auto unpack_payload = packet_out.payload();
    bytes payload(unpack_payload.begin(), unpack_payload.end());
    CHECK(payload == PAYLOAD);

} // end TEST_CASE("Data Packet Full Prologue")

TEST_CASE("Data Packet All")
{
    using packet_type = TestData10;
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
    packet_in.payload(PAYLOAD);

    // Trailer required. Set values to check
    packet_in.trailer().valid_data(true);
    packet_in.trailer().agc_mgc(true);

    // Check packet size
    const size_t EXPECTED_SIZE = 4 +  // header
                                 4 +  // stream_id
                                 8 +  // class_id
                                 12 + // timestamp
                                 4 +  // payload
                                 4;   // trailer
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

    // Examine and check packed Class ID. Value shall be in big-endian format.
    const bytes CLASS_ID_BE{ 0, 0xFF, 0xEE, 0xDD, 0, 0, 0x12, 0x34 };
    const bytes packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
    check_ptr += CLASS_ID_BYTES;
    CHECK(packed_class_id == CLASS_ID_BE);

    // Examine and check packed Integer Timestamp. Value shall be in big-endian format.
    const bytes INTEGER_TS_BE{ 0x12, 0x34, 0x56, 0x78 };
    const bytes packed_integer_ts(check_ptr, check_ptr + INTEGER_TS_BYTES);
    check_ptr += INTEGER_TS_BYTES;
    CHECK(packed_integer_ts == INTEGER_TS_BE);

    // Examine and check packed Fractional Timestamp. Value shall be in big-endian format.
    const bytes FRACTIONAL_TS_BE{ 0x00, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78 };
    const bytes packed_fractional_ts(check_ptr, check_ptr + FRACTIONAL_TS_BYTES);
    check_ptr += FRACTIONAL_TS_BYTES;
    CHECK(packed_fractional_ts == FRACTIONAL_TS_BE);

    // Examine and check packed payload
    const size_t PAYLOAD_BYTES = PAYLOAD.size();
    const bytes packed_payload(check_ptr, check_ptr + PAYLOAD_BYTES);
    check_ptr += PAYLOAD_BYTES;
    CHECK(packed_payload == PAYLOAD);

    // Examine and check packed trailer
    const uint8_t ENABLES = (0x1 << 6) | (0x1 << 4); // valid_data_enable @ 30; agc_mgc_enable @ 28
    const uint8_t INDICATORS = (0x1 << 2) | 0x1; // valid_data @ 18; agc_mgc @ 16
    const bytes TRAILER_BE{ ENABLES, INDICATORS, 0, 0 };
    const bytes packed_trailer(check_ptr, check_ptr + HEADER_BYTES);
    check_ptr += TRAILER_BYTES;
    CHECK(packed_trailer == TRAILER_BE);

    // Check match
    CHECK_FALSE(packet_type::match(data));

    // Unpack verifed packed data
    bytes packed_bytes(data.size());
    memcpy(packed_bytes.data(), data.data(), data.size());
    packet_type packet_out({ packed_bytes.data(), packed_bytes.size() });

    // Examine and check unpacked packet header
    const auto& header = packet_out.header();
    CHECK(header.packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID);
    CHECK(header.class_id_enable());
    CHECK(header.trailer_included());
    CHECK_FALSE(header.not_v49d0());
    CHECK(header.spectrum_or_time() == vrtgen::packing::SpectrumOrTime::TIME);
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
    auto unpack_payload = packet_out.payload();
    bytes payload(unpack_payload.begin(), unpack_payload.end());
    CHECK(payload == PAYLOAD);

    // Examine and check unpacked packet trailer
    const auto& trailer = packet_out.trailer();
    CHECK(trailer.valid_data().has_value());
    CHECK(trailer.valid_data().value());
    CHECK(trailer.agc_mgc().has_value());
    CHECK(trailer.agc_mgc().value());

} // end TEST_CASE("Data Packet Full Prologue")
/*
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
*/
