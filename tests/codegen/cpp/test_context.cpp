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
#include "context.hpp"
#include <bytes.hpp>
#include <iostream>

TEST_CASE("Context Packet Stream ID")
{
    using packet_type = TestContext1;
    using helper = packet_type::helper;
    packet_type packet_in;

    // Stream ID is required field. Set value to check
    const uint32_t STREAM_ID = 0x12345678;
    packet_in.stream_id(STREAM_ID);

    // Check bytes required
    const size_t EXPECTED_SIZE = 4 + // header
                                 4 + // stream_id
                                 4;  // cif 0
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == EXPECTED_SIZE);

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
    CHECK_FALSE(header.not_v49d0());
    CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked Stream ID
    CHECK(packet_out.stream_id() == STREAM_ID);

} // end TEST_CASE("Context Packet Stream ID")

TEST_CASE("Context Packet Class ID")
{
    using packet_type = TestContext2;
    using helper = packet_type::helper;
    packet_type packet_in;

    // Stream ID is required field. Set value to check
    const uint32_t STREAM_ID = 0x12345678;
    packet_in.stream_id(STREAM_ID);

    // Check bytes required
    const size_t EXPECTED_SIZE = 4 + // header
                                 4 + // stream_id
                                 8 + // class_id
                                 4;  // cif 0
    const size_t PACKED_SIZE = helper::bytes_required(packet_in);
    CHECK(PACKED_SIZE == EXPECTED_SIZE);

    // Get buffer from pack
    auto data = helper::pack(packet_in);
    CHECK(data.size() == PACKED_SIZE);
    auto* check_ptr = data.data();

    // Examine and check packed header
    const size_t HEADER_BYTES = 4;
    const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
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
    CHECK(header.class_id_enable());
    CHECK_FALSE(header.not_v49d0());
    CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
    CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
    CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
    CHECK(header.packet_size() == PACKET_SIZE);

    // Examine and check unpacked Stream ID
    CHECK(packet_out.stream_id() == STREAM_ID);

    // Examine and check unpacked Class ID
    CHECK(packet_out.class_id().oui() == 0xFFEEDD);
    CHECK(packet_out.class_id().packet_code() == 0x1234);

} // end TEST_CASE("Context Packet Class ID")

TEST_CASE("Context Packet CIF0 Optional")
{
    using packet_type = TestContext4;
    using packet_helper = packet_type::helper;

    // Sizes for all required fields
    size_t PACKED_SIZE = 4 + // header
                         4 + // stream_id
                         4;  // cif 0
    
    SECTION("Context Field Change Indicator") {
        bool CHANGE_INDICATOR;
        bytes CIF0_BE;
        packet_type packet_in;

        // Stream ID is required field. Set value to check
        const uint32_t STREAM_ID = 0x12345678;
        packet_in.stream_id(STREAM_ID);
        CHECK(packet_in.stream_id() == STREAM_ID);

        // Set change indicator value
        SECTION("Rule 9.1.1-1") {
            /*
            * Rule 9.1.1-1: The Context Field Change Indicator bit shall be set
            * to zero when all the Context fields within the Context Packet
            * were conveyed in earlier Context packets with the same
            * information.
            */
            CHANGE_INDICATOR = false;
            CHECK_FALSE(packet_in.change_indicator());
            packet_in.change_indicator(CHANGE_INDICATOR);
            CHECK_FALSE(packet_in.change_indicator());

            CIF0_BE = bytes { 0, 0, 0, 0 }; // change indicator is false
        } // end SECTION("Rule 9.1.1-1")

        SECTION("Rule 9.1.1-2") {
            /*
             * Rule 9.1.1-2: The Context Field Change Indicator bit shall be
             * set to one when at least one Context field contains a new value.
             */
            CHANGE_INDICATOR = true;
            CHECK_FALSE(packet_in.change_indicator());
            packet_in.change_indicator(CHANGE_INDICATOR);
            CHECK(packet_in.change_indicator());

            const uint8_t CHANGE_INDICATOR_ENABLE = 0x1 << 7; // CIF 0 bit 31
            CIF0_BE = bytes { CHANGE_INDICATOR_ENABLE, 0, 0, 0 };
        } // end SECTION("Rule 9.1.1-2")

        // Check bytes required
        CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        // Get buffer from pack
        auto data = packet_helper::pack(packet_in);
        CHECK(data.size() == PACKED_SIZE);
        auto* check_ptr = data.data();

        // Examine and check packed header
        const size_t HEADER_BYTES = 4;
        const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
        const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
        const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
        const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
        check_ptr += CIF0_BYTES;
        CHECK(packed_cif0 == CIF0_BE);

        // Check match
        CHECK_FALSE(packet_helper::match(data.data(), data.size()));

        // Unpack verifed packed data
        packet_type packet_out;
        packet_helper::unpack(packet_out, data.data(), data.size());

        // Examine and check unpacked packet header
        const auto& header = packet_out.header();
        CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
        CHECK_FALSE(header.class_id_enable());
        CHECK_FALSE(header.not_v49d0());
        CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
        CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
        CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
        CHECK(header.packet_size() == PACKET_SIZE);

        // Examine and check unpacked Stream ID
        CHECK(packet_out.stream_id() == STREAM_ID);

        // Examine and check unpacked Change Indicator
        CHECK(packet_out.change_indicator() == CHANGE_INDICATOR);
    } // SECTION("Context Field Change Indicator")

    SECTION("Reference Point Identifier") {
        /*
         * Rule 9.2-1: The Reference Point ID shall use the format shown in Figure 9.2-1.
         *
         * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
         * | 1    |                                  Reference Point Identifier                                   |
         */
        // Add bytes to PACKED_SIZE count
        const size_t REF_POINT_ID_BYTES = 4;
        PACKED_SIZE += REF_POINT_ID_BYTES;

        SECTION("Rule 9.2-2") {
            /*
             * The Reference Point ID, when used, shall contain the Stream
             * Identifier of the Reference Point.
             */
            packet_type packet_in;

            // Stream ID is required field. Set value to check
            const uint32_t STREAM_ID = 0x12345678;
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            // Set reference point id value to check
            const uint32_t REF_POINT_ID = 0x12345678;
            CHECK_FALSE(packet_in.has_reference_point_id());
            packet_in.reference_point_id(REF_POINT_ID);
            CHECK(packet_in.has_reference_point_id());
            CHECK(packet_in.reference_point_id() == REF_POINT_ID);

            // Check bytes required
            CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = packet_helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
            const uint8_t REF_POINT_ID_ENABLE = 0x1 << 6; // CIF 0 bit 30
            const bytes CIF0_BE{ REF_POINT_ID_ENABLE, 0, 0, 0 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed Reference Point ID. Value shall be in big-endian format.
            const bytes REF_POINT_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
            const decltype(data) packed_ref_point_id(check_ptr, check_ptr + REF_POINT_ID_BYTES);
            check_ptr += REF_POINT_ID_BYTES;
            CHECK(packed_ref_point_id == REF_POINT_ID_BE);

            // Check match
            CHECK_FALSE(packet_helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked Ref Point ID
            CHECK(packet_out.has_reference_point_id());
            CHECK(packet_out.reference_point_id() == REF_POINT_ID);
        } // end SECTION("Rule 9.2-2")
    } // end SECTION("Reference Point Identifier")

    SECTION("Bandwidth") {
        // Add bytes to PACKED_SIZE count
        const size_t BANDWIDTH_BYTES = 8;
        PACKED_SIZE += BANDWIDTH_BYTES;

        SECTION("Rule 9.5.1-2") {
            /*
             * The value of the Bandwidth field shall be expressed in units of
             * Hertz. The Bandwidth field shall use the 64-bit,
             * two’s-complement format shown in Figure 9.5.1-1. This field has
             * an integer and a fractional part with the radix point to the
             * right of bit 20 in the second 32-bit word.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                                    Bandwidth (63..32), Hz                                     |
             * | 2    |                                    Bandwidth (31..0), Hz                                      |
             *                                            ^
             *                                            |
             *                                       Radix Point
             *
             * Observation 9.5.1-3: The value of the Bandwidth field has a valid
             * range of 0.00 to 8.79 terahertz with a resolution of 0.95
             * micro-Hertz. Negative values of Bandwidth are not valid.
             * 
             * Observation 9.5.1-4: A Bandwidth field value of 0x0000 0000 0010 0000
             * represents a bandwidth of 1 Hz. A Bandwidth field value of
             * 0x0000 0000 0000 0001 represents a bandwidth of 0.95 micro-Hertz.
             */
            double BANDWIDTH;
            bytes BANDWIDTH_BE;
            packet_type packet_in;

            // Stream ID is required field. Set value to check
            const uint32_t STREAM_ID = 0x12345678;
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            // Set bandwidth value to check
            SECTION("Bandwidth Value 1") {
                BANDWIDTH = 1.0; // represents 0x0000 0000 0010 0000
                BANDWIDTH_BE = bytes { 0, 0, 0, 0, 0, 0x10, 0, 0 };
            }
            SECTION("Bandwidth Value 2") {
                BANDWIDTH = 1.0 / (1 << 20); // represents 0x0000 0000 0000 0001
                BANDWIDTH_BE = bytes { 0, 0, 0, 0, 0, 0, 0, 0x01 };
            }
            SECTION("Bandwidth Value 3") {
                BANDWIDTH = 1.0 + 1.0 / (1 << 20); // represents 0x0000 0000 0010 0001
                BANDWIDTH_BE = bytes { 0, 0, 0, 0, 0, 0x10, 0, 0x01 };
            }

            CHECK_FALSE(packet_in.has_bandwidth());
            packet_in.bandwidth(BANDWIDTH);
            CHECK(packet_in.has_bandwidth());
            CHECK(packet_in.bandwidth() == BANDWIDTH);

            // Check bytes required
            CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = packet_helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
            const uint8_t BW_ENABLE = 0x1 << 5; // CIF 0 bit 29
            const bytes CIF0_BE{ BW_ENABLE, 0, 0, 0 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed Bandwidth. Value shall be in big-endian format.
            const decltype(data) packed_bandwidth(check_ptr, check_ptr + BANDWIDTH_BYTES);
            check_ptr += BANDWIDTH_BYTES;
            CHECK(packed_bandwidth == BANDWIDTH_BE);

            // Check match
            CHECK_FALSE(packet_helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked Bandwidth
            CHECK(packet_out.has_bandwidth());
            CHECK(packet_out.bandwidth() == BANDWIDTH);
        } // end SECTION("Rule 9.5.1-2")
    } // end SECTION("Bandwidth")

    SECTION("IF Reference Frequency") {
        // Add bytes to PACKED_SIZE count
        const size_t IF_REF_FREQ_BYTES = 8;
        PACKED_SIZE += IF_REF_FREQ_BYTES;

        SECTION("Rule 9.5.5-3") {
            /*
             * The value of the IF Reference Frequency shall be expressed in
             * units of Hertz. The IF Reference Frequency subfield shall use
             * the 64-bit, two’s-complement format as shown in Figure 9.5.5-1.
             * This field has an integer and a fractional part, with the radix
             * point to the right of bit 20 in the second 32-bit word.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                         IF Reference Frequency (63..32), Hz                                   |
             * | 2    |                         IF Reference Frequency (31..0), Hz                                    |
             *                                            ^
             *                                            |
             *                                       Radix Point
             * 
             * Observation 9.5.5-2: The value of the IF Reference Frequency
             * field has a range of +/-8.79 terahertz with a resolution of 0.95
             * micro-Hertz.
             * 
             * Observation 9.5.5-3: An IF Reference Frequency field value of
             * 0x0000 0000 0010 0000 represents a frequency of 1 Hz. An IF
             * Reference Frequency field value of 0xFFFF FFFF FFF0 0000
             * represents a frequency of -1 Hz. An IF Reference Frequency field
             * value of 0x0000 0000 0000 0001 represents a frequency of 0.95
             * micro-Hertz. An IF Reference Frequency field value of
             * 0xFFFF FFFF FFFF FFFF represents a frequency of -0.95 micro-Hertz.
             */
            double IF_REF_FREQ;
            bytes IF_REF_FREQ_BE;
            packet_type packet_in;

            // Stream ID is required field. Set value to check
            const uint32_t STREAM_ID = 0x12345678;
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            // Set if frequency value to check
            SECTION ("IF Reference Frequency Value 1") {
                IF_REF_FREQ = 1.0; // represents 0x0000 0000 0010 0000
                IF_REF_FREQ_BE = bytes { 0, 0, 0, 0, 0, 0x10, 0, 0 };
            }
            SECTION ("IF Reference Frequency Value 2") {
                IF_REF_FREQ = 1.0 / (1 << 20); // represents 0x0000 0000 0000 0001
                IF_REF_FREQ_BE = bytes { 0, 0, 0, 0, 0, 0, 0, 0x01 };
            }
            SECTION ("IF Reference Frequency Value 3") {
                IF_REF_FREQ = -1.0; // represents 0xFFFF FFFF FFF0 0000
                IF_REF_FREQ_BE = bytes { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00 };
            }
            SECTION ("IF Reference Frequency Value 4") {
                IF_REF_FREQ = -1.0 / (1 << 20); // represents 0xFFFF FFFF FFFF FFFF
                IF_REF_FREQ_BE = bytes { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
            }

            CHECK_FALSE(packet_in.has_if_ref_frequency());
            packet_in.if_ref_frequency(IF_REF_FREQ);
            CHECK(packet_in.has_if_ref_frequency());
            CHECK(packet_in.if_ref_frequency() == IF_REF_FREQ);

            // Check bytes required
            CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = packet_helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
            const uint8_t IF_REF_FREQ_ENABLE = 0x1 << 4; // CIF 0 bit 28
            const bytes CIF0_BE{ IF_REF_FREQ_ENABLE, 0, 0, 0 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed IF Reference Frequency. Value shall be in big-endian format.
            const decltype(data) packed_if_ref_freq(check_ptr, check_ptr + IF_REF_FREQ_BYTES);
            check_ptr += IF_REF_FREQ_BYTES;
            CHECK(packed_if_ref_freq == IF_REF_FREQ_BE);

            // Check match
            CHECK_FALSE(packet_helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked IF Reference Frequency
            CHECK(packet_out.has_if_ref_frequency());
            CHECK(packet_out.if_ref_frequency() == IF_REF_FREQ);
        } // end SECTION("Rule 9.5.5-3")
    } // end SECTION("IF Reference Frequency")

    SECTION("RF Reference Frequency") {
        // Add bytes to PACKED_SIZE count
        const size_t RF_REF_FREQ_BYTES = 8;
        PACKED_SIZE += RF_REF_FREQ_BYTES;

        SECTION("Rule 9.5.10-2") {
            /*
             * The value of the RF Reference Frequency shall be expressed in
             * units of Hertz. The RF Reference Frequency value field shall use
             * the 64-bit, two’s-complement format as shown in Figure 9.5.10-1.
             * This subfield has an integer and a fractional part, with the
             * radix point to the right of bit 20 in the second 32-bit word.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                         RF Reference Frequency (63..32), Hz                                   |
             * | 2    |                         RF Reference Frequency (31..0), Hz                                    |
             *                                            ^
             *                                            |
             *                                       Radix Point
             * 
             * Observation 9.5.10-1: The value of the RF Reference Frequency
             * field has a range of +/-8.79 terahertz with a resolution of 0.95
             * micro-Hertz.
             * 
             * Observation 9.5.10-2: An RF Reference Frequency field value of
             * 0x0000 0000 0010 0000 represents a frequency of +1 Hz. An RF
             * Reference Frequency field value of 0xFFFF FFFF FFF0 0000
             * represents a frequency of -1 Hz. An RF Reference Frequency field
             * value of 0x0000 0000 0000 0001 represents a frequency of +0.95
             * microhertz. An RF Reference Frequency field value of
             * 0xFFFF FFFF FFFF FFFF represents a frequency of -0.95 micro-Hertz.
             */
            double RF_REF_FREQ;
            bytes RF_REF_FREQ_BE;
            packet_type packet_in;

            // Stream ID is required field. Set value to check
            const uint32_t STREAM_ID = 0x12345678;
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            // Set rf frequency value to check
            SECTION ("RF Reference Frequency Value 1") {
                RF_REF_FREQ = 1.0; // represents 0x0000 0000 0010 0000
                RF_REF_FREQ_BE = bytes { 0, 0, 0, 0, 0, 0x10, 0, 0 };
            }
            SECTION ("RF Reference Frequency Value 2") {
                RF_REF_FREQ = 1.0 / (1 << 20); // represents 0x0000 0000 0000 0001
                RF_REF_FREQ_BE = bytes { 0, 0, 0, 0, 0, 0, 0, 0x01 };
            }
            SECTION ("RF Reference Frequency Value 3") {
                RF_REF_FREQ = -1.0; // represents 0xFFFF FFFF FFF0 0000
                RF_REF_FREQ_BE = bytes { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0, 0 };
            }
            SECTION ("RF Reference Frequency Value 4") {
                RF_REF_FREQ = -1.0 / (1 << 20); // represents 0xFFFF FFFF FFFF FFFF
                RF_REF_FREQ_BE = bytes { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
            }

            CHECK_FALSE(packet_in.has_rf_ref_frequency());
            packet_in.rf_ref_frequency(RF_REF_FREQ);
            CHECK(packet_in.has_rf_ref_frequency());
            CHECK(packet_in.rf_ref_frequency() == RF_REF_FREQ);
                
            // Check bytes required
            CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = packet_helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
            const uint8_t RF_REF_FREQ_ENABLE = 0x1 << 3; // CIF 0 bit 27
            const bytes CIF0_BE{ RF_REF_FREQ_ENABLE, 0, 0, 0 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed RF Reference Frequency. Value shall be in big-endian format.
            const decltype(data) packed_rf_ref_freq(check_ptr, check_ptr + RF_REF_FREQ_BYTES);
            check_ptr += RF_REF_FREQ_BYTES;
            CHECK(packed_rf_ref_freq == RF_REF_FREQ_BE);

            // Check match
            CHECK_FALSE(packet_helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked RF Reference Frequency
            CHECK(packet_out.has_rf_ref_frequency());
            CHECK(packet_out.rf_ref_frequency() == RF_REF_FREQ);
        } // end SECTION("Rule 9.5.10-2")
    } // end SECTION("RF Reference Frequency")

    SECTION("RF Reference Frequency Offset") {
        // Add bytes to PACKED_SIZE count
        const size_t RF_REF_FREQ_OFFSET_BYTES = 8;
        PACKED_SIZE += RF_REF_FREQ_OFFSET_BYTES;

        SECTION("Rule 9.5.11-3") {
            /*
             * Rule 9.5.11-3: The value of the RF Reference Frequency Offset
             * shall be expressed in units of Hertz. The RF Reference Frequency
             * Offset field shall use the 64-bit, two’s-complement format shown
             * in Figure 9.5.11-1. This field has an integer and a fractional
             * part, with the radix point to the right of bit 20 in the second
             * 32-bit word.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                   RF Reference Frequency Offset (63..32), Hz                                  |
             * | 2    |                   RF Reference Frequency Offset (31..0), Hz                                   |
             *                                            ^
             *                                            |
             *                                       Radix Point
             * 
             * Observation 9.5.11-3: The value of the RF Reference Frequency
             * Offset field has a range of +/-8.79 terahertz with a resolution
             * of 0.95 micro-Hertz.
             * 
             * Observation 9.5.11-4: An RF Reference Frequency Offset field
             * value of 0x0000 0000 0010 0000 represents a frequency of +1 Hz.
             * An RF Reference Frequency Offset field value of 0xFFFF FFFF FFF0
             * 0000 represents a frequency of -1 Hz. An RF Reference Frequency
             * Offset field value of 0x0000 0000 0000 0001 represents a
             * frequency of +0.95 microhertz. An RF Reference Frequency Offset
             * field value of 0xFFFF FFFF FFFF FFFF represents a frequency of
             * -0.95 micro-Hertz.
             */
            double RF_REF_FREQ_OFFSET;
            bytes RF_REF_FREQ_OFFSET_BE;
            packet_type packet_in;

            // Stream ID is required field. Set value to check
            const uint32_t STREAM_ID = 0x12345678;
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            // Set rf frequency offset value to check
            SECTION ("RF Reference Frequency Offset Value 1") {
                RF_REF_FREQ_OFFSET = 1.0; // represents 0x0000 0000 0010 0000
                RF_REF_FREQ_OFFSET_BE = bytes { 0, 0, 0, 0, 0, 0x10, 0, 0 };
            }
            SECTION ("RF Reference Frequency Offset Value 2") {
                RF_REF_FREQ_OFFSET = 1.0 / (1 << 20); // represents 0x0000 0000 0000 0001
                RF_REF_FREQ_OFFSET_BE = bytes { 0, 0, 0, 0, 0, 0, 0, 0x01 };
            } 
            SECTION ("RF Reference Frequency Offset Value 3") {
                RF_REF_FREQ_OFFSET = -1.0; // represents 0xFFFF FFFF FFF0 0000
                RF_REF_FREQ_OFFSET_BE = bytes { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0, 0 };
            }
            SECTION ("RF Reference Frequency Offset Value 4") {
                RF_REF_FREQ_OFFSET = -1.0 / (1 << 20); // represents 0xFFFF FFFF FFFF FFFF
                RF_REF_FREQ_OFFSET_BE = bytes { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
            }

            CHECK_FALSE(packet_in.has_rf_ref_frequency_offset());
            packet_in.rf_ref_frequency_offset(RF_REF_FREQ_OFFSET);
            CHECK(packet_in.rf_ref_frequency_offset());
            CHECK(packet_in.rf_ref_frequency_offset() == RF_REF_FREQ_OFFSET);

            // Check bytes required
            CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = packet_helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
            const uint8_t RF_REF_FREQ_OFFSET_ENABLE = 0x1 << 2; // CIF 0 bit 26
            const bytes CIF0_BE{ RF_REF_FREQ_OFFSET_ENABLE, 0, 0, 0 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed RF Reference Frequency Offset. Value shall be in big-endian format.
            const decltype(data) packed_rf_ref_freq_offset(check_ptr, check_ptr + RF_REF_FREQ_OFFSET_BYTES);
            check_ptr += RF_REF_FREQ_OFFSET_BYTES;
            CHECK(packed_rf_ref_freq_offset == RF_REF_FREQ_OFFSET_BE);

            // Check match
            CHECK_FALSE(packet_helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked RF Reference Frequency Offset
            CHECK(packet_out.has_rf_ref_frequency_offset());
            CHECK(packet_out.rf_ref_frequency_offset() == RF_REF_FREQ_OFFSET);
        } // end SECTION("Rule 9.5.11-3")
    } // end SECTION("RF Reference Frequency Offset")

    SECTION("IF Band Offset") {
        // Add bytes to PACKED_SIZE count
        const size_t IF_BAND_OFFSET_BYTES = 8;
        PACKED_SIZE += IF_BAND_OFFSET_BYTES;

        SECTION("Rule 9.5.4-2") {
            /*
             * Rule 9.5.4-2: The value of the IF Band Offset shall be expressed
             * in units of Hertz. The IF Band Offset field shall use the 64-bit,
             * two’s-complement format shown in Figure 9.5.4-1. This field has
             * an integer and a fractional part, with the radix point to the
             * right of bit 20 in the second 32-bit word.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                                IF Band Offset (63..32), Hz                                    |
             * | 2    |                                IF Band Offset (31..0), Hz                                     |
             *                                            ^
             *                                            |
             *                                       Radix Point
             * 
             * Observation 9.5.4-2: The value of the IF Band Offset field has a
             * range of +/-8.79 terahertz with a resolution of 0.95 micro-Hertz.
             *
             * Observation 9.5.4-3: A IF Band Offset field value of 0x0000 0000
             * 0010 0000 represents a band offset of +1 Hz. A IF Band Offset
             * field value of 0xFFFF FFFF FFF0 0000 represents a band offset of
             * -1 Hz. An IF Band Offset field value of 0x0000 0000 0000 0001
             * represents a band offset of +0.95 micro-Hertz. An IF Band Offset
             * field value of 0xFFFF FFFF FFFF FFFF represents a band offset of
             * -0.95 micro-Hertz.
             */
            double IF_BAND_OFFSET;
            bytes IF_BAND_OFFSET_BE;
            packet_type packet_in;

            // Stream ID is required field. Set value to check
            const uint32_t STREAM_ID = 0x12345678;
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            // Set if band offset value to check
            SECTION ("IF Band Offset Value 1") {
                IF_BAND_OFFSET = 1.0; // represents 0x0000 0000 0010 0000
                IF_BAND_OFFSET_BE = bytes { 0, 0, 0, 0, 0, 0x10, 0, 0 };
            }
            SECTION ("IF Band Offset Value 2") {
                IF_BAND_OFFSET = 1.0 / (1 << 20); // represents 0x0000 0000 0000 0001
                IF_BAND_OFFSET_BE = bytes { 0, 0, 0, 0, 0, 0, 0, 0x01 };
            }
            SECTION ("IF Band Offset Value 3") {
                IF_BAND_OFFSET = -1.0; // represents 0xFFFF FFFF FFF0 0000
                IF_BAND_OFFSET_BE = bytes { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0, 0 };
            }
            SECTION ("IF Band Offset Value 4") {
                IF_BAND_OFFSET = -1.0 / (1 << 20); // represents 0xFFFF FFFF FFFF FFFF
                IF_BAND_OFFSET_BE = bytes { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
            }

            CHECK_FALSE(packet_in.has_if_band_offset());
            packet_in.if_band_offset(IF_BAND_OFFSET);
            CHECK(packet_in.if_band_offset());
            CHECK(packet_in.if_band_offset() == IF_BAND_OFFSET);


            // Check bytes required
            CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = packet_helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
            const uint8_t IF_BAND_OFFSET_ENABLE = 0x1 << 1; // CIF 0 bit 25
            const bytes CIF0_BE{ IF_BAND_OFFSET_ENABLE, 0, 0, 0 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed IF Band Offset. Value shall be in big-endian format.
            const decltype(data) packed_if_band_offset(check_ptr, check_ptr + IF_BAND_OFFSET_BYTES);
            check_ptr += IF_BAND_OFFSET_BYTES;
            CHECK(packed_if_band_offset == IF_BAND_OFFSET_BE);

            // Check match
            CHECK_FALSE(packet_helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked IF Band Offset
            CHECK(packet_out.has_if_band_offset());
            CHECK(packet_out.if_band_offset() == IF_BAND_OFFSET);
        } // end SECTION("Rule 9.5.4-2")
    } // end SECTION("IF Band Offset")

    SECTION("Reference Level") {
        // Add bytes to PACKED_SIZE count
        const size_t REF_LEVEL_BYTES = 4;
        PACKED_SIZE += REF_LEVEL_BYTES;

        SECTION("Rule 9.5.9-2") {
            /*
             * Rule 9.5.9-2: The value of the Reference Level field shall be
             * expressed in units of dBm, where 0 dBm is one milliWatt
             * (0.001 watt) measured with respect to a load termination of 50
             * ohms. The Reference Level field shall use the 32-bit format
             * shown in Figure 9.5.9-2. The upper 16 bits of this field are
             * reserved and shall be set to zero. The Reference Level value
             * shall be expressed in two’s-complement format in the lower 16
             * bits of this field. This field has an integer and a fractional
             * part, with the radix point to the right of bit 7.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                    Reserved                   |         Reference Level (15..0), dBm          |
             *                                                                                   ^
             *                                                                                   |
             *                                                                              Radix Point
             * 
             * Observation 9.5.9-2: The value of the Reference Level field has
             * a range of nearly +/-256 dBm with a resolution of 1/128 dBm
             * (0.0078125 dBm).
             * 
             * Observation 9.5.9-3: A Reference Level field value of 0x0000 0080
             * represents a reference level of +1 dBm. A Reference Level field
             * value of 0x0000 FF80 represents a reference level of -1 dBm. A
             * Reference Level field value of 0x0000 0001 represents a reference
             * level of +0.0078125 dBm. A Reference Level field value of 0x0000
             * FFFF represents a reference level of -0.0078125 dBm.
             */
            float REF_LEVEL;
            bytes REF_LEVEL_BE;
            packet_type packet_in;

            // Stream ID is required field. Set value to check
            const uint32_t STREAM_ID = 0x12345678;
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            // Set reference level value to check
            SECTION ("Reference Level Value 1") {
                REF_LEVEL = 1.0; // represents 0x0000 0080
                REF_LEVEL_BE = bytes { 0, 0, 0, 0x80 };
            }
            SECTION ("Reference Level Value 2") {
                REF_LEVEL = 0.0078125; // represents 0x0000 0001
                REF_LEVEL_BE = bytes { 0, 0, 0, 0x01 };
            }
            SECTION ("Reference Level Value 3") {
                REF_LEVEL = -1.0; // represents 0x0000 FF80
                REF_LEVEL_BE = bytes { 0, 0, 0xFF, 0x80 };
            }
            SECTION ("Reference Level Value 4") {
                REF_LEVEL = -0.0078125; // represents 0x0000 FFFF
                REF_LEVEL_BE = bytes { 0, 0, 0xFF, 0xFF };
            }

            CHECK_FALSE(packet_in.has_reference_level());
            packet_in.reference_level(REF_LEVEL);
            CHECK(packet_in.reference_level());
            CHECK(packet_in.reference_level() == REF_LEVEL);

            // Check bytes required
            CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = packet_helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
            const uint8_t REF_LEVEL_ENABLE = 0x1; // CIF 0 bit 24
            const bytes CIF0_BE{ REF_LEVEL_ENABLE, 0, 0, 0 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed Reference Level. Value shall be in big-endian format.
            const decltype(data) packed_reference_level(check_ptr, check_ptr + REF_LEVEL_BYTES);
            check_ptr += REF_LEVEL_BYTES;
            CHECK(packed_reference_level == REF_LEVEL_BE);

            // Check match
            CHECK_FALSE(packet_helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked Reference Level
            CHECK(packet_out.has_reference_level());
            CHECK(packet_out.reference_level() == REF_LEVEL);
        } // end SECTION("Rule 9.5.9-2")
    } // end SECTION("Reference Level")

    SECTION("Gain") {
        // Add bytes to PACKED_SIZE count
        const size_t GAIN_BYTES = 4;
        PACKED_SIZE += GAIN_BYTES;

        SECTION("Rule 9.5.3-3") {
            /*
             * The Gain field shall use the 32-bit format shown in Figure
             * 9.5.3-1, containing two gain subfields.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |       Stage 2 Gain (15..0), dB (Optional)     |       Stage 1 Gain (15..0), dB (Optional)     |
             *                                   ^                                               ^
             *                                   |                                               |
             *                              Radix Point                                     Radix Point
             * 
             * Observation 9.5.3-3: The values of the Gain subfields have a
             * range of near +/-256 dB with a resolution of 1/128 dB
             * (0.0078125 dB).
             * 
             * Observation 9.5.3-4: A Gain field value of 0x0000 0080
             * represents a gain of +1 dB. A Gain field value of
             * 0x0000 FF80 represents a gain of -1 dB. A Gain field value of
             * 0x0000 0001 represents a gain of +0.0078125 dB. A Gain field
             * value of 0x0000 FFFF represents a gain of -0.0078125 dB.
             * 
             * Observation 9.5.3-5: A Gain field value of 0x0080 0080
             * represents front and back-end gains of +1 dB. A Gain field
             * value of 0xFF80 FF80 represents front and back-end gains of -1
             * dB. A Gain field value of 0x0001 0001 represents front and
             * back-end gains of +0.0078125 dB. A Gain field value of 0xFFFF FFFF
             * represents front and back-end gains of -0.0078125 dB.
             */
            float GAIN_STAGE_1 { 0.0 };
            float GAIN_STAGE_2 { 0.0 };
            bytes GAIN_BE;
            packet_type packet_in;

            // Stream ID is required field. Set value to check
            const uint32_t STREAM_ID = 0x12345678;
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            // Set gain value to check
            SECTION ("Gain Value 1") {
                GAIN_STAGE_1 = 1.0; // represents 0x0000 0080
                GAIN_BE = bytes { 0, 0, 0, 0x80 };
            }
            SECTION ("Gain Value 2") {
                GAIN_STAGE_1 = -1.0; // represents 0x0000 FF80
                GAIN_BE = bytes { 0, 0, 0xFF, 0x80 };
            }
            SECTION ("Gain Value 3") {
                GAIN_STAGE_1 = 0.0078125; // represents 0x0000 0001
                GAIN_BE = bytes { 0, 0, 0, 0x01 };
            }
            SECTION ("Gain Value 4") {
                GAIN_STAGE_1 = -0.0078125; // represents 0x0000 FFFF
                GAIN_BE = bytes { 0, 0, 0xFF, 0xFF };
            }
            SECTION ("Gain Value 5") {
                GAIN_STAGE_1 = GAIN_STAGE_2 = 1.0; // represents 0x0080 0080
                GAIN_BE = bytes { 0, 0x80, 0, 0x80 };
            }
            SECTION ("Gain Value 6") {
                GAIN_STAGE_1 = GAIN_STAGE_2 = -1.0; // represents 0xFF80 FF80
                GAIN_BE = bytes { 0xFF, 0x80, 0xFF, 0x80 };
            }
            SECTION ("Gain Value 7") {
                GAIN_STAGE_1 = GAIN_STAGE_2 = 0.0078125; // represents 0x0001 0001
                GAIN_BE = bytes { 0, 0x01, 0, 0x01 };
            }
            SECTION ("Gain Value 8") {
                GAIN_STAGE_1 = GAIN_STAGE_2 = -0.0078125; // represents 0xFFFF FFFF
                GAIN_BE = bytes { 0xFF, 0xFF, 0xFF, 0xFF };
            }

            vrtgen::packing::Gain gain;
            gain.stage_1(GAIN_STAGE_1);
            gain.stage_2(GAIN_STAGE_2);
            CHECK_FALSE(packet_in.has_gain());
            packet_in.gain(gain);
            CHECK(packet_in.has_gain());
            CHECK(packet_in.gain().stage_1() == GAIN_STAGE_1);
            CHECK(packet_in.gain().stage_2() == GAIN_STAGE_2);

            // Check bytes required
            CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = packet_helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
            const uint8_t GAIN_ENABLE = 0x1 << 7; // CIF 0 bit 23
            const bytes CIF0_BE{ 0, GAIN_ENABLE, 0, 0 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed Gain. Value shall be in big-endian format.
            const decltype(data) packed_gain(check_ptr, check_ptr + GAIN_BYTES);
            check_ptr += GAIN_BYTES;
            CHECK(packed_gain == GAIN_BE);

            // Check match
            CHECK_FALSE(packet_helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked Gain
            CHECK(packet_out.has_gain());
            CHECK(packet_out.gain().stage_1() == GAIN_STAGE_1);
            CHECK(packet_out.gain().stage_2() == GAIN_STAGE_2);
        } // end SECTION("Rule 9.5.5-3")
    } // end SECTION("Gain")

    SECTION("Over-Range Count") {
        // Add bytes to PACKED_SIZE count
        const size_t OVER_RANGE_COUNT_BYTES = 4;
        PACKED_SIZE += OVER_RANGE_COUNT_BYTES;

        SECTION("Rule 9.10.6-3") {
            /*
             * Rule 9.10.6-3: The Over-range Count field shall use the 32-bit,
             * unsigned integer format shown in Figure 9.10.6-1.
             *
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                                      Overflow Count (31..0)                                   |
             */
            packet_type packet_in;

            // Stream ID is required field. Set value to check
            const uint32_t STREAM_ID = 0x12345678;
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            // Set over range count value to check
            const uint32_t OVER_RANGE_COUNT = 0x12345678;
            CHECK_FALSE(packet_in.has_over_range_count());
            packet_in.over_range_count(OVER_RANGE_COUNT);
            CHECK(packet_in.has_over_range_count());
            CHECK(packet_in.over_range_count() == OVER_RANGE_COUNT);

            // Check bytes required
            CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = packet_helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
            const uint8_t OVER_RANGE_COUNT_ENABLE = 0x1 << 6; // CIF 0 bit 22
            const bytes CIF0_BE{ 0, OVER_RANGE_COUNT_ENABLE, 0, 0 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed Reference Point ID. Value shall be in big-endian format.
            const bytes OVER_RANGE_COUNT_BE{ 0x12, 0x34, 0x56, 0x78 };
            const decltype(data) packed_over_range_count(check_ptr, check_ptr + OVER_RANGE_COUNT_BYTES);
            check_ptr += OVER_RANGE_COUNT_BYTES;
            CHECK(packed_over_range_count == OVER_RANGE_COUNT_BE);

            // Check match
            CHECK_FALSE(packet_helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked Ref Point ID
            CHECK(packet_out.has_over_range_count());
            CHECK(packet_out.over_range_count() == OVER_RANGE_COUNT);
        } // end SECTION("Rule 9.10.6-3")
    } // end SECTION("Over-Range Count")

    SECTION("Sample Rate") {
        // Add bytes to PACKED_SIZE count
        const size_t SAMPLE_RATE_BYTES = 8;
        PACKED_SIZE += SAMPLE_RATE_BYTES;

        SECTION("Rule 9.5.12-2") {
            /*
             * Rule 9.5.12-2: The value of the Sample Rate field shall be
             * expressed in units of Hertz. The Sample Rate field shall use the
             * 64-bit, two’s-complement format shown in Figure 9.5.12-1. This
             * field has an integer and a fractional part, with the radix point
             * to the right of bit 20 in the second 32-bit word.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                                  Sample Rate (63..32), Hz                                     |
             * | 2    |                                  Sample Rate (31..0), Hz                                      |
             *                                            ^
             *                                            |
             *                                       Radix Point
             *
             * Observation 9.5.12-1: The value of the Sample Rate field has a
             * valid range of 0.00 to 8.79 terahertz with a resolution of 0.95
             * micro-Hertz.
             * 
             * Observation 9.5.12-2: A Sample Rate field value of 0x0000 0000
             * 0010 0000 represents a sample rate of 1 Hz. A Sample Rate field
             * value of 0x0000 0000 0000 0001 represents a bandwidth of 0.95
             * micro-Hertz. Negative values of Sample Rate are not valid.
             */
            double SAMPLE_RATE;
            bytes SAMPLE_RATE_BE;
            packet_type packet_in;

            // Stream ID is required field. Set value to check
            const uint32_t STREAM_ID = 0x12345678;
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            // Set sample rate value to check
            SECTION("Sample Rate Value 1") {
                SAMPLE_RATE = 1.0; // represents 0x0000 0000 0010 0000
                SAMPLE_RATE_BE = bytes { 0, 0, 0, 0, 0, 0x10, 0, 0 };
            }
            SECTION("Sample Rate Value 2") {
                SAMPLE_RATE = 1.0 / (1 << 20); // represents 0x0000 0000 0000 0001
                SAMPLE_RATE_BE = bytes { 0, 0, 0, 0, 0, 0, 0, 0x01 };
            }

            CHECK_FALSE(packet_in.has_sample_rate());
            packet_in.sample_rate(SAMPLE_RATE);
            CHECK(packet_in.has_sample_rate());
            CHECK(packet_in.sample_rate() == SAMPLE_RATE);

            // Check bytes required
            CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = packet_helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
            const uint8_t SAMPLE_RATE_ENABLE = 0x1 << 5; // CIF 0 bit 21
            const bytes CIF0_BE{ 0, SAMPLE_RATE_ENABLE, 0, 0 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed Sample Rate. Value shall be in big-endian format.
            const decltype(data) packed_sample_rate(check_ptr, check_ptr + SAMPLE_RATE_BYTES);
            check_ptr += SAMPLE_RATE_BYTES;
            CHECK(packed_sample_rate == SAMPLE_RATE_BE);

            // Check match
            CHECK_FALSE(packet_helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked Sample Rate
            CHECK(packet_out.has_sample_rate());
            CHECK(packet_out.sample_rate() == SAMPLE_RATE);
        } // end SECTION("Rule 9.5.12-2")
    } // end SECTION("Sample Rate")

    SECTION("Timestamp Adjustment") {
        // Add bytes to PACKED_SIZE count
        const size_t TIMESTAMP_ADJ_BYTES = 8;
        PACKED_SIZE += TIMESTAMP_ADJ_BYTES;

        SECTION("Rule 9.7.3.1-2") {
            /*
             * Rule 9.7.3.1-2: The Timestamp Adjustment field shall use
             * Fractional Time data type in Section 9.7.
             *
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                            Most-significant (Upper 32 bits)                                   |
             * | 2    |                            Least-significant (Upper 32 bits)                                  |
             */
            packet_type packet_in;

            // Stream ID is required field. Set value to check
            const uint32_t STREAM_ID = 0x12345678;
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            // Set timestamp adjustment value to check
            const int64_t TIMESTAMP_ADJ = 0xABCDEF12345678;
            CHECK_FALSE(packet_in.has_timestamp_adjustment());
            packet_in.timestamp_adjustment(TIMESTAMP_ADJ);
            CHECK(packet_in.has_timestamp_adjustment());
            CHECK(packet_in.timestamp_adjustment() == TIMESTAMP_ADJ);

            // Check bytes required
            CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = packet_helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
            const uint8_t TIMESTAMP_ADJ_ENABLE = 0x1 << 4; // CIF 0 bit 20
            const bytes CIF0_BE{ 0, TIMESTAMP_ADJ_ENABLE, 0, 0 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed Timestamp Adjustment. Value shall be in big-endian format.
            const bytes TIMESTAMP_ADJ_BE{ 0, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78 };
            const decltype(data) packed_timestamp_adjustment(check_ptr, check_ptr + TIMESTAMP_ADJ_BYTES);
            check_ptr += TIMESTAMP_ADJ_BYTES;
            CHECK(packed_timestamp_adjustment == TIMESTAMP_ADJ_BE);

            // Check match
            CHECK_FALSE(packet_helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked Timestamp Adjustment
            CHECK(packet_out.has_timestamp_adjustment());
            CHECK(packet_out.timestamp_adjustment() == TIMESTAMP_ADJ);
        } // end SECTION("Rule 9.7.3.1-2")
    } // end SECTION("Timestamp Adjustment")

    SECTION("Timestamp Calibration Time") {
        // Add bytes to PACKED_SIZE count
        const size_t TIMESTAMP_CAL_TIME_BYTES = 4;
        PACKED_SIZE += TIMESTAMP_CAL_TIME_BYTES;

        SECTION("Rule 9.7.3.3-2") {
            /*
             * Rule 9.7.3.3-2: The Timestamp Calibration Time field shall use
             * the same time base format used in the Timestamp field as given
             * by the TSI field of the Context packet.
             *
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                                  Timestamp Calibration Time                                   |
             */
            packet_type packet_in;

            // Stream ID is required field. Set value to check
            const uint32_t STREAM_ID = 0x12345678;
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            // Set timestamp calibration time value to check
            const uint32_t TIMESTAMP_CAL_TIME = 0x87654321;
            CHECK_FALSE(packet_in.has_timestamp_calibration_time());
            packet_in.timestamp_calibration_time(TIMESTAMP_CAL_TIME);
            CHECK(packet_in.has_timestamp_calibration_time());
            CHECK(packet_in.timestamp_calibration_time() == TIMESTAMP_CAL_TIME);

            // Check bytes required
            CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = packet_helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
            const uint8_t TIMESTAMP_CAL_TIME_ENABLE = 0x1 << 3; // CIF 0 bit 19
            const bytes CIF0_BE{ 0, TIMESTAMP_CAL_TIME_ENABLE, 0, 0 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed Timestamp Calibration Time. Value shall be in big-endian format.
            const bytes TIMESTAMP_CAL_TIME_BE{ 0x87, 0x65, 0x43, 0x21 };
            const decltype(data) packed_timestamp_calibration_time(check_ptr, check_ptr + TIMESTAMP_CAL_TIME_BYTES);
            check_ptr += TIMESTAMP_CAL_TIME_BYTES;
            CHECK(packed_timestamp_calibration_time == TIMESTAMP_CAL_TIME_BE);

            // Check match
            CHECK_FALSE(packet_helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked Timestamp Calibration Time
            CHECK(packet_out.has_timestamp_calibration_time());
            CHECK(packet_out.timestamp_calibration_time() == TIMESTAMP_CAL_TIME);
        } // end SECTION("Rule 9.7.3.3-2")
    } // end SECTION("Timestamp Calibration Time")

    SECTION("Temperature") {
        // Add bytes to PACKED_SIZE count
        const size_t TEMP_BYTES = 4;
        PACKED_SIZE += TEMP_BYTES;

        SECTION("Rule 9.10.5-1") {
            /*
             * Rule 9.10.5-1: The value of the Temperature field shall be
             * expressed in units of degrees Celsius (*C). The Temperature
             * field shall use the 32-bit format shown in Figure 9.10.5-1. The
             * upper 16 bits of this field are reserved and shall be set to
             * zero. The Temperature value shall be expressed in two’s-
             * complement format in the lower 16 bits of this field. This field
             * has an integer and a fractional part, with the radix point to
             * the right of bit 6.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                    Reserved                   |     Temperature (15..0), degrees Celsius      |
             *                                                                                      ^
             *                                                                                      |
             *                                                                                 Radix Point
             * 
             * Observation 9.10.5-1: The valid range of the Temperature field
             * is -273.15 *C to +511.984375 *C. The resolution of the
             * Temperature field is 0.015625 *C (1/64 *C).
             * 
             * Observation 9.10.5-2: A Temperature field value of 0x0000 0040
             * represents a temperature of +1 *C. A Temperature field value of
             * 0x0000 FFC0 represents a temperature of -1 *C. A Temperature
             * field value of 0x0000 0001 represents a temperature of +0.015625
             * *C. A Temperature field value of 0x0000 FFFF represents a
             * temperature of -0.015625 *C.
             */
            float TEMP;
            bytes TEMP_BE;
            packet_type packet_in;

            // Stream ID is required field. Set value to check
            const uint32_t STREAM_ID = 0x12345678;
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            // Set temperature value to check
            SECTION ("Temperature Value 1") {
                TEMP = 1.0; // represents 0x0000 0040
                TEMP_BE = bytes { 0, 0, 0, 0x40 };
            }
            SECTION ("Temperature Value 2") {
                TEMP = 0.015625; // represents 0x0000 0001
                TEMP_BE = bytes { 0, 0, 0, 0x01 };
            }
            SECTION ("Temperature Value 3") {
                TEMP = -1.0; // represents 0x0000 FFC0
                TEMP_BE = bytes { 0, 0, 0xFF, 0xC0 };
            }
            SECTION ("Temperature Value 4") {
                TEMP = -0.015625; // represents 0x0000 FFFF
                TEMP_BE = bytes { 0, 0, 0xFF, 0xFF };
            }

            CHECK_FALSE(packet_in.has_temperature());
            packet_in.temperature(TEMP);
            CHECK(packet_in.temperature());
            CHECK(packet_in.temperature() == TEMP);

            // Check bytes required
            CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = packet_helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
            const uint8_t TEMP_ENABLE = 0x1 << 2; // CIF 0 bit 18
            const bytes CIF0_BE{ 0, TEMP_ENABLE, 0, 0 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed Temperature. Value shall be in big-endian format.
            const decltype(data) packed_temperature(check_ptr, check_ptr + TEMP_BYTES);
            check_ptr += TEMP_BYTES;
            CHECK(packed_temperature == TEMP_BE);

            // Check match
            CHECK_FALSE(packet_helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked Temperature
            CHECK(packet_out.has_temperature());
            CHECK(packet_out.temperature() == TEMP);
        } // end SECTION("Rule 9.10.5-1")
    } // end SECTION("Temperature")

    SECTION("Device Identifier") {
        // Add bytes to PACKED_SIZE count
        const size_t DEVICE_ID_BYTES = 8;
        PACKED_SIZE += DEVICE_ID_BYTES;

        /*
         * Rule 9.10.1-1: The Device Identifier field shall use the format shown in Figure 9.10.1-1.
         * 
         * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
         * | 1    |       Reserved        |                       Manufacturer OUI (23..0)                        |
         * | 2    |                    Reserved                   |              Device Code (15..0)              |
         * 
         * Rule 9.10.1-2: The Manufacturer OUI subfield shall contain the
         * 24-bit, IEEE-registered, Organizationally Unique Identifier (company
         * identifier) in the range 00-00-00 to FF-FE-FF (inclusive) of the
         * manufacturer of the device that generated the Context Packet.
         * 
         * Rule 9.10.1-3: The Device Code subfield shall contain a 16-bit
         * number to identify the model of the device emitting the Control
         * Packet Stream. For each manufacturer the Device Code shall be unique
         * for each device model that emits VRT Packet Streams.
         */
        packet_type packet_in;

        // Stream ID is required field. Set value to check
        const uint32_t STREAM_ID = 0x12345678;
        packet_in.stream_id(STREAM_ID);
        CHECK(packet_in.stream_id() == STREAM_ID);

        // Set device id values to check
        const uint32_t OUI = 0x123456;
        const uint16_t DEVICE_CODE = 0x789A;
        CHECK_FALSE(packet_in.has_device_id());
        vrtgen::packing::DeviceIdentifier device_id;
        device_id.manufacturer_oui(OUI);
        device_id.device_code(DEVICE_CODE);
        packet_in.device_id(device_id);
        CHECK(packet_in.has_device_id());
        CHECK(packet_in.device_id().manufacturer_oui() == OUI);
        CHECK(packet_in.device_id().device_code() == DEVICE_CODE);

        // Check bytes required
        CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        // Get buffer from pack
        auto data = packet_helper::pack(packet_in);
        CHECK(data.size() == PACKED_SIZE);
        auto* check_ptr = data.data();

        // Examine and check packed header
        const size_t HEADER_BYTES = 4;
        const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
        const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
        const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
        const uint8_t DEVICE_ID_ENABLE = 0x1 << 1; // CIF 0 bit 17
        const bytes CIF0_BE{ 0, DEVICE_ID_ENABLE, 0, 0 };
        const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
        check_ptr += CIF0_BYTES;
        CHECK(packed_cif0 == CIF0_BE);

        // Examine and check packed Device Identifier. Value shall be in big-endian format.
        const bytes DEVICE_ID_BE{ 0, 0x12, 0x34, 0x56, 0, 0, 0x78, 0x9A };
        const decltype(data) packed_device_id(check_ptr, check_ptr + DEVICE_ID_BYTES);
        check_ptr += DEVICE_ID_BYTES;
        CHECK(packed_device_id == DEVICE_ID_BE);

        // Check match
        CHECK_FALSE(packet_helper::match(data.data(), data.size()));

        // Unpack verifed packed data
        packet_type packet_out;
        packet_helper::unpack(packet_out, data.data(), data.size());

        // Examine and check unpacked packet header
        const auto& header = packet_out.header();
        CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
        CHECK_FALSE(header.class_id_enable());
        CHECK_FALSE(header.not_v49d0());
        CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
        CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
        CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
        CHECK(header.packet_size() == PACKET_SIZE);

        // Examine and check unpacked Stream ID
        CHECK(packet_out.stream_id() == STREAM_ID);

        // Examine and check unpacked Device Identifier
        CHECK(packet_out.has_device_id());
        CHECK(packet_out.device_id().manufacturer_oui() == OUI);
        CHECK(packet_out.device_id().device_code() == DEVICE_CODE);
    } // SECTION("Device Identifier")

    SECTION("State and Event Indicators") {
        // Add bytes to PACKED_SIZE count
        const size_t STATE_EVENT_INDICATOR_BYTES = 4;
        PACKED_SIZE += STATE_EVENT_INDICATOR_BYTES;

        /*
         * Rule 9.10.8-1: The format of the State and Event Indicator field
         * shall follow that shown in Figure 9.10.8-1, shown in greater detail
         * in Table 9.10.8-1.
         * 
         * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
         * | 1    |              Enables              |            Indicators             |     User-Defined      |
         * 
         * ----------------------------------------------------------------------------------------------------
         * | Enable Bit Position | Indicator Bit Position | Indicator Name               | Period of Validity |
         * | 31                  | 19                     | Calibrated Time Indicator    | Persistent         |
         * | 30                  | 18                     | Valid Data Indicator         | Persistent         |
         * | 29                  | 17                     | Reference Lock Indicator     | Persistent         |
         * | 28                  | 16                     | AGC/MGC Indicator            | Persistent         |
         * | 27                  | 15                     | Detected Signal Indicator    | Persistent         |
         * | 26                  | 14                     | Spectral Inversion Indicator | Persistent         |
         * | 25                  | 13                     | Over-range Indicator         | Single Data Packet |
         * | 24                  | 12                     | Sample Loss Indicator        | Single Data Packet |
         * | [23..20]            | [11..8]                | Reserved                     | N/A                |
         * ----------------------------------------------------------------------------------------------------
         * | Bit Position                                 | Function
         * | [7..0]                                       | User-Defined | User-Defined
         * ----------------------------------------------------------------------------------------------------
         * 
         */
        packet_type packet_in;

        // Stream ID is required field. Set value to check
        const uint32_t STREAM_ID = 0x12345678;
        packet_in.stream_id(STREAM_ID);
        CHECK(packet_in.stream_id() == STREAM_ID);

        // Set state and event indicator values to check
        CHECK_FALSE(packet_in.has_state_event_indicators());
        vrtgen::packing::StateEventIndicators state_event_indicators;
        state_event_indicators.valid_data_enable(true);
        state_event_indicators.valid_data(true);
        state_event_indicators.agc_mgc_enable(true);
        state_event_indicators.agc_mgc(false);
        packet_in.state_event_indicators(state_event_indicators);
        CHECK(packet_in.has_state_event_indicators());
        CHECK(packet_in.state_event_indicators().valid_data_enable());
        CHECK(packet_in.state_event_indicators().valid_data());
        CHECK(packet_in.state_event_indicators().agc_mgc_enable());
        CHECK_FALSE(packet_in.state_event_indicators().agc_mgc());

        // Check bytes required
        CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        // Get buffer from pack
        auto data = packet_helper::pack(packet_in);
        CHECK(data.size() == PACKED_SIZE);
        auto* check_ptr = data.data();

        // Examine and check packed header
        const size_t HEADER_BYTES = 4;
        const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
        const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
        const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
        const uint8_t STATE_EVENT_INDICATOR_ENABLE = 0x1; // CIF 0 bit 16
        const bytes CIF0_BE{ 0, STATE_EVENT_INDICATOR_ENABLE, 0, 0 };
        const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
        check_ptr += CIF0_BYTES;
        CHECK(packed_cif0 == CIF0_BE);

        // Examine and check packed State Event Indicators. Value shall be in big-endian format.
        const uint8_t ENABLES = (0x1 << 6) | (0x1 << 4); // valid_data_enable @ 30; agc_mgc_enable @ 28
        const uint8_t INDICATORS = (0x1 << 2) | 0x0; // valid_data @ 18; agc_mgc @ 16 (0)
        const bytes STATE_EVENT_INDICATOR_BE{ ENABLES, INDICATORS, 0, 0 };
        const decltype(data) packed_device_id(check_ptr, check_ptr + STATE_EVENT_INDICATOR_BYTES);
        check_ptr += STATE_EVENT_INDICATOR_BYTES;
        CHECK(packed_device_id == STATE_EVENT_INDICATOR_BE);

        // Check match
        CHECK_FALSE(packet_helper::match(data.data(), data.size()));

        // Unpack verifed packed data
        packet_type packet_out;
        packet_helper::unpack(packet_out, data.data(), data.size());

        // Examine and check unpacked packet header
        const auto& header = packet_out.header();
        CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
        CHECK_FALSE(header.class_id_enable());
        CHECK_FALSE(header.not_v49d0());
        CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
        CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
        CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
        CHECK(header.packet_size() == PACKET_SIZE);

        // Examine and check unpacked Stream ID
        CHECK(packet_out.stream_id() == STREAM_ID);

        // Examine and check unpacked State Event Indicators
        CHECK(packet_out.has_state_event_indicators());
        CHECK(packet_out.state_event_indicators().valid_data_enable());
        CHECK(packet_out.state_event_indicators().valid_data());
        CHECK(packet_out.state_event_indicators().agc_mgc_enable());
        CHECK_FALSE(packet_out.state_event_indicators().agc_mgc());
    } // SECTION("State and Event Indicators")

    SECTION("Data Packet Payload Format") {
        // Add bytes to PACKED_SIZE count
        const size_t PAYLOAD_FORMAT_BYTES = 8;
        PACKED_SIZE += PAYLOAD_FORMAT_BYTES;

        /*
         * Rule 9.13.3-1: The Data Packet Payload Format field shall be arranged
         * as shown in Figure 9.13.3-1; the constituent parameter fields are
         * described in Table 9.13.3-1 and Table 9.13.3-2.
         * 
         * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
         * | 1    |P |Real/|  Data Item   |R | Event- | Channel-  | Data Item |  Item Packing   | Data Item Size  |
         * |      |a |Cmplx|    Format    |p | Tag    | Tag Size  | Frac Size |   Field Size    |                 |
         * |      |c |     |              |t | Size   |           |           |                 |                 |
         * |      |k |     |              |  |        |           |           |                 |                 |
         * | 2    |                 Repeat Count               |                   Vector Size                    |
         */
        packet_type packet_in;

        // Stream ID is required field. Set value to check
        const uint32_t STREAM_ID = 0x12345678;
        packet_in.stream_id(STREAM_ID);
        CHECK(packet_in.stream_id() == STREAM_ID);

        // Set Payload Format values to check
        CHECK_FALSE(packet_in.has_signal_data_format());
        vrtgen::packing::PayloadFormat payload_format;
        payload_format.packing_method(vrtgen::packing::PackingMethod::LINK_EFFICIENT);
        payload_format.real_complex_type(vrtgen::packing::DataSampleType::REAL);
        payload_format.data_item_format(vrtgen::packing::DataItemFormat::UNSIGNED_FIXED);
        payload_format.data_item_size(16);
        packet_in.signal_data_format(payload_format);
        CHECK(packet_in.has_signal_data_format());
        CHECK(packet_in.signal_data_format().packing_method() == vrtgen::packing::PackingMethod::LINK_EFFICIENT);
        CHECK(packet_in.signal_data_format().real_complex_type() == vrtgen::packing::DataSampleType::REAL);
        CHECK(packet_in.signal_data_format().data_item_format() == vrtgen::packing::DataItemFormat::UNSIGNED_FIXED);
        CHECK(packet_in.signal_data_format().data_item_size() == 16);

        // Check bytes required
        CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        // Get buffer from pack
        auto data = packet_helper::pack(packet_in);
        CHECK(data.size() == PACKED_SIZE);
        auto* check_ptr = data.data();

        // Examine and check packed header
        const size_t HEADER_BYTES = 4;
        const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
        const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
        const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
        const uint8_t PAYLOAD_FORMAT_ENABLE = 0x1 << 7; // CIF 0 bit 15
        const bytes CIF0_BE{ 0, 0, PAYLOAD_FORMAT_ENABLE, 0 };
        const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
        check_ptr += CIF0_BYTES;
        CHECK(packed_cif0 == CIF0_BE);

        // Examine and check packed Payload Format. Value shall be in big-endian format.
        const bytes PAYLOAD_FORMAT_BE{ 0x90, 0, 0, 0x0F, 0, 0, 0, 0 };
        const decltype(data) packed_payload_format(check_ptr, check_ptr + PAYLOAD_FORMAT_BYTES);
        check_ptr += PAYLOAD_FORMAT_BYTES;
        CHECK(packed_payload_format == PAYLOAD_FORMAT_BE);

        // Check match
        CHECK_FALSE(packet_helper::match(data.data(), data.size()));

        // Unpack verifed packed data
        packet_type packet_out;
        packet_helper::unpack(packet_out, data.data(), data.size());

        // Examine and check unpacked packet header
        const auto& header = packet_out.header();
        CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
        CHECK_FALSE(header.class_id_enable());
        CHECK_FALSE(header.not_v49d0());
        CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
        CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
        CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
        CHECK(header.packet_size() == PACKET_SIZE);

        // Examine and check unpacked Stream ID
        CHECK(packet_out.stream_id() == STREAM_ID);

        // Examine and check unpacked Payload Format
        CHECK(packet_out.has_signal_data_format());
        CHECK(packet_out.signal_data_format().packing_method() == vrtgen::packing::PackingMethod::LINK_EFFICIENT);
        CHECK(packet_out.signal_data_format().real_complex_type() == vrtgen::packing::DataSampleType::REAL);
        CHECK(packet_out.signal_data_format().data_item_format() == vrtgen::packing::DataItemFormat::UNSIGNED_FIXED);
        CHECK(packet_out.signal_data_format().data_item_size() == 16);
    } // end SECTION("Data Packet Payload Format")

    SECTION("Formatted GPS/INS") {
        /*
         * Rule 9.4.5-1: The Formatted GPS Geolocation field shall be formatted
         * as shown in Figure 9.4.5-1.
         * 
         * Rule 9.4.6-1: The Formatted INS Geolocation field shall follow the
         * same rules as the Formatted GPS Geolocation field.
         * 
         * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
         * | 1    | Reserved  | TSI | TSF |                        GPS/INS Manufacturer OUI                       |
         * | 2    |                       Integer-second Timestamp of Position Fix (31..0)                        |
         * | 3    |                      Fractional-second Timestamp of Position Fix (63..32)                     |
         * | 4    |                      Fractional-second Timestamp of Position Fix (31..0)                      |
         * | 5    |                                   Latitude (31..0), degrees                                   |
         * | 6    |                                  Longitude (31..0), degrees                                   |
         * | 7    |                                   Altitude (31..0), meters                                    |
         * | 8    |                          Speed over Ground (31..0), meters/second                             |
         * | 9    |                              Heading Angle (31..0), degrees                                   |
         * | 10   |                                Track Angle (31..0), degrees                                   |
         * | 11   |                         Magnetic Variation (31..0), degrees                                   |
         */
        const size_t FORMATTED_GEO_BYTES { 44 };
        PACKED_SIZE += FORMATTED_GEO_BYTES;

        vrtgen::packing::Geolocation geo;
        vrtgen::packing::TSI TSI { vrtgen::packing::TSI::NONE };
        vrtgen::packing::TSF TSF { vrtgen::packing::TSF::NONE };
        uint32_t OUI { 0 };
        uint32_t INTEGER_TIMESTAMP { 0xFFFFFFFFu };
        uint64_t FRACTIONAL_TIMESTAMP { 0xFFFFFFFFFFFFFFFFu };
        // set fixed points to values that produce 0x7FFFFFFF per rule 9.4.5-18
        double LATITUDE = vrtgen::fixed::to_fp<32,22>(0x7FFFFFFF);
        double LONGITUDE = vrtgen::fixed::to_fp<32,22>(0x7FFFFFFF);
        double ALTITUDE = vrtgen::fixed::to_fp<32,5>(0x7FFFFFFF);
        double GROUND_SPEED = vrtgen::fixed::to_fp<32,16>(0x7FFFFFFF);
        double HEADING_ANGLE = vrtgen::fixed::to_fp<32,22>(0x7FFFFFFF);
        double TRACK_ANGLE = vrtgen::fixed::to_fp<32,22>(0x7FFFFFFF);
        double MAGNETIC_VARIATION = vrtgen::fixed::to_fp<32,22>(0x7FFFFFFF);
        bytes FORMATTED_GEO_BE;
        bool GPS_TEST { false };
        bool INS_TEST { false };
        packet_type packet_in;

        // Stream ID is required field. Set value to check
        const uint32_t STREAM_ID = 0x12345678;
        packet_in.stream_id(STREAM_ID);
        CHECK(packet_in.stream_id() == STREAM_ID);

        // Set GPS values to check
        SECTION("Rule 9.4.5-2") {
            /*
             * Rule 9.4.5-2: The GPS/INS Manufacturer OUI subfield shall
             * contain the 24-bit field for the IEEE registered,
             * Organizationally Unique Identifier (company identifier) in the
             * range 00-00-00 to FF-FE-FF (inclusive) of the GPS/INS
             * manufacturer.
             */
            OUI = 0x123456;
            geo.manufacturer_oui(OUI);
            SECTION("GPS") {
                GPS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_gps());
                packet_in.formatted_gps(geo);
                CHECK(packet_in.has_formatted_gps());
                CHECK(packet_in.formatted_gps().manufacturer_oui() == OUI);
            }
            SECTION("INS") {
                INS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_ins());
                packet_in.formatted_ins(geo);
                CHECK(packet_in.has_formatted_ins());
                CHECK(packet_in.formatted_ins().manufacturer_oui() == OUI);
            }
            FORMATTED_GEO_BE = bytes { 
                0x00, 0x12, 0x34, 0x56, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            };
        } // end SECTION("Rule 9.4.5-2")

        SECTION("Rule 9.4.5-3") {
            /*
             * Rule 9.4.5-3: The TSI field in the Formatted GPS Geolocation
             * field shall accurately indicate the type of Integer-seconds
             * Timestamp included in the packet according to the code
             * assignments in Table 9.4.5-1.
             * 
             * | TSI Code | Meaning   |
             * | -------- | --------- |
             * | 00       | Undefined |
             * | 01       | UTC       |
             * | 10       | GPS Time  |
             * | 11       | Other     |
             * | -------- | --------- |
             */
            SECTION("Undefined") {
                TSI = vrtgen::packing::TSI::NONE;
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.tsi(TSI);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().tsi() == TSI);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().tsi() == TSI);
                }
            } // end SECTION("Undefined")
            SECTION("UTC") {
                TSI = vrtgen::packing::TSI::UTC;
                FORMATTED_GEO_BE = bytes { 
                    0x04, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.tsi(TSI);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().tsi() == TSI);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().tsi() == TSI);
                }
            } // end SECTION("UTC")
            SECTION("GPS Time") {
                TSI = vrtgen::packing::TSI::GPS;
                FORMATTED_GEO_BE = bytes { 
                    0x08, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.tsi(TSI);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().tsi() == TSI);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().tsi() == TSI);
                }
            } // end SECTION("GPS Time")
            SECTION("Other") {
                TSI = vrtgen::packing::TSI::OTHER;
                FORMATTED_GEO_BE = bytes { 
                    0x0C, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.tsi(TSI);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().tsi() == TSI);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().tsi() == TSI);
                }
            } // end SECTION("Other")
        } // end SECTION("Rule 9.4.5-3")

        SECTION("Rule 9.4.5-4") {
            /*
             * Rule 9.4.5-4: The TSF field in the Formatted GPS Geolocation
             * field shall accurately indicate the type of Fractional-seconds
             * Timestamp included in the packet according to the code
             * assignments in Table 9.4.5-2.
             * 
             * | TSF Code | Meaning                 |
             * | -------- | ----------------------- |
             * | 00       | Undefined               |
             * | 01       | Sample Count Time       |
             * | 10       | Real (Picoseconds) Time |
             * | 11       | Free-Running Count Time |
             * | -------- | ----------------------- |
             */
            SECTION("Undefined") {
                TSF = vrtgen::packing::TSF::NONE;
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.tsf(TSF);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().tsf() == TSF);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().tsf() == TSF);
                }
            } // end SECTION("Undefined")
            SECTION("Sample Count Time") {
                TSF = vrtgen::packing::TSF::SAMPLE_COUNT;
                FORMATTED_GEO_BE = bytes { 
                    0x01, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.tsf(TSF);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().tsf() == TSF);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().tsf() == TSF);
                }
            } // end SECTION("Sample Count Time")
            SECTION("Real (Picoseconds) Time") {
                TSF = vrtgen::packing::TSF::REAL_TIME;
                FORMATTED_GEO_BE = bytes { 
                    0x02, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.tsf(TSF);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().tsf() == TSF);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().tsf() == TSF);
                }
            } // end SECTION("Real (Picoseconds) Time")
            SECTION("Other") {
                TSF = vrtgen::packing::TSF::FREE_RUNNING;
                FORMATTED_GEO_BE = bytes { 
                    0x03, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.tsf(TSF);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().tsf() == TSF);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().tsf() == TSF);
                }
            } // end SECTION("Other")
        } // end SECTION("Rule 9.4.5-4")

        SECTION("Rule 9.4.5-5") {
            /*
             * Rule 9.4.5-5: When the TSI or TSF fields are non-zero the
             * corresponding Timestamp of Position Fix subfield shall express
             * the time of the most recent location fix in the format given in
             * Sections 6.1.45.1.4 and 5.1.4.2
             */
            TSI = vrtgen::packing::TSI::UTC;
            TSF = vrtgen::packing::TSF::REAL_TIME;
            INTEGER_TIMESTAMP = 0x12345678;
            FRACTIONAL_TIMESTAMP = 0x12345678ABCDEF;
            geo.tsi(TSI);
            geo.tsf(TSF);
            geo.integer_timestamp(INTEGER_TIMESTAMP);
            geo.fractional_timestamp(FRACTIONAL_TIMESTAMP);
            SECTION("GPS") {
                GPS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_gps());
                packet_in.formatted_gps(geo);
                CHECK(packet_in.has_formatted_gps());
                CHECK(packet_in.formatted_gps().tsi() == TSI);
                CHECK(packet_in.formatted_gps().tsf() == TSF);
                CHECK(packet_in.formatted_gps().integer_timestamp() == INTEGER_TIMESTAMP);
                CHECK(packet_in.formatted_gps().fractional_timestamp() == FRACTIONAL_TIMESTAMP);
            }
            SECTION("INS") {
                INS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_ins());
                packet_in.formatted_ins(geo);
                CHECK(packet_in.has_formatted_ins());
                CHECK(packet_in.formatted_ins().tsi() == TSI);
                CHECK(packet_in.formatted_ins().tsf() == TSF);
                CHECK(packet_in.formatted_ins().integer_timestamp() == INTEGER_TIMESTAMP);
                CHECK(packet_in.formatted_ins().fractional_timestamp() == FRACTIONAL_TIMESTAMP);
            }
            FORMATTED_GEO_BE = bytes { 
                0x06, 0x00, 0x00, 0x00, // word 1
                0x12, 0x34, 0x56, 0x78, // word 2
                0x00, 0x12, 0x34, 0x56, // word 3
                0x78, 0xAB, 0xCD, 0xEF, // word 4
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            };
        } // end SECTION("Rule 9.4.5-5")

        SECTION("Rule 9.4.5-6") {
            /*
             * Rule 9.4.5-6: When the TSI or TSF fields are zero the
             * corresponding Timestamp of Position Fix subfield words shall
             * take the value 0xFFFFFFFF.
             */
            TSI = vrtgen::packing::TSI::NONE;
            TSF = vrtgen::packing::TSF::NONE;
            INTEGER_TIMESTAMP = 0xFFFFFFFF;
            FRACTIONAL_TIMESTAMP = 0xFFFFFFFFFFFFFFFF;
            geo.tsi(TSI);
            geo.tsf(TSF);
            geo.integer_timestamp(INTEGER_TIMESTAMP);
            geo.fractional_timestamp(FRACTIONAL_TIMESTAMP);
            SECTION("GPS") {
                GPS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_gps());
                packet_in.formatted_gps(geo);
                CHECK(packet_in.has_formatted_gps());
                CHECK(packet_in.formatted_gps().tsi() == TSI);
                CHECK(packet_in.formatted_gps().tsf() == TSF);
                CHECK(packet_in.formatted_gps().integer_timestamp() == INTEGER_TIMESTAMP);
                CHECK(packet_in.formatted_gps().fractional_timestamp() == FRACTIONAL_TIMESTAMP);
            }
            SECTION("INS") {
                INS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_ins());
                packet_in.formatted_ins(geo);
                CHECK(packet_in.has_formatted_ins());
                CHECK(packet_in.formatted_ins().tsi() == TSI);
                CHECK(packet_in.formatted_ins().tsf() == TSF);
                CHECK(packet_in.formatted_ins().integer_timestamp() == INTEGER_TIMESTAMP);
                CHECK(packet_in.formatted_ins().fractional_timestamp() == FRACTIONAL_TIMESTAMP);
            }
            FORMATTED_GEO_BE = bytes { 
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            };
        } // end SECTION("Rule 9.4.5-6")

        SECTION("Rule 9.4.5-7") {
            /*
             * Rule 9.4.5-7: The Latitude and Longitude subfields shall use the
             * Geolocation Angle Format shown in Figure 9.4.5-2.
             *
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                                    Angle (31..0), degrees                                     |
             *                                      ^
             *                                      |
             *                                 Radix Point
             */
            // Observation 9.4.5-2 - resolution of 2.38e-7 degrees
            LATITUDE = 1.0 + (1.0 / (1 << 22));
            LONGITUDE = 1.0 + (1.0 / (1 << 22));
            geo.latitude(LATITUDE);
            geo.longitude(LONGITUDE);
            SECTION("GPS") {
                GPS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_gps());
                packet_in.formatted_gps(geo);
                CHECK(packet_in.has_formatted_gps());
                CHECK(packet_in.formatted_gps().longitude() == LATITUDE);
                CHECK(packet_in.formatted_gps().longitude() == LONGITUDE);
            }
            SECTION("INS") {
                INS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_ins());
                packet_in.formatted_ins(geo);
                CHECK(packet_in.has_formatted_ins());
                CHECK(packet_in.formatted_ins().longitude() == LATITUDE);
                CHECK(packet_in.formatted_ins().longitude() == LONGITUDE);
            }
            FORMATTED_GEO_BE = bytes { 
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x00, 0x40, 0x00, 0x01, // word 5 
                0x00, 0x40, 0x00, 0x01, // word 6
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            };
        } // end SECTION("Rule 9.4.5-7")

        SECTION("Rule 9.4.5-8") {
            /*
             * Rule 9.4.5-8: The Latitude subfield in the GPS Geolocation Field
             * value shall range from -90.0 (South) to +90.0 (North) degrees.
             */
            SECTION("Min") {
                LATITUDE = -90.0; // hex(-90<<22(radix)) = 0xe9800000
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0xE9, 0x80, 0x00, 0x00, // word 5 
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.latitude(LATITUDE);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().latitude() == LATITUDE);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().latitude() == LATITUDE);
                }
            } // end SECTION("Min")
            SECTION("Max") {
                LATITUDE = 90.0; // hex(90<<22(radix)) = 0x16800000
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x16, 0x80, 0x00, 0x00, // word 5 
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.latitude(LATITUDE);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().latitude() == LATITUDE);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().latitude() == LATITUDE);
                }
            } // end SECTION("Max")
        } // end SECTION("Rule 9.4.5-8")

        SECTION("Rule 9.4.5-9") {
            /*
             * Rule 9.4.5-9: The Longitude subfield in the GPS Geolocation
             * Field value shall range from -180.0 (West) to +180.0 (East)
             * degrees.
             */
            SECTION("Min") {
                // Set GPS values to check
                LONGITUDE = -180.0; // hex(-180<<22(radix)) = 0xd3000000
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0xD3, 0x00, 0x00, 0x00, // word 6 
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.longitude(LONGITUDE);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().longitude() == LONGITUDE);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().longitude() == LONGITUDE);
                }
            } // end SECTION("Min")
            SECTION("Max") {
                // Set GPS values to check
                LONGITUDE = 180.0; // hex(180<<22(radix)) = 0x2d000000
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18) 
                    0x2D, 0x00, 0x00, 0x00, // word 6
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.longitude(LONGITUDE);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().longitude() == LONGITUDE);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().longitude() == LONGITUDE);
                }
            } // end SECTION("Max")
        } // end SECTION("Rule 9.4.5-9")

        SECTION("Rule 9.4.5-10") {
            /*
             * Rule 9.4.5-10: The Altitude subfield in the GPS Geolocation Field
             * shall use the 32-bit, two’s-complement format shown in Figure
             * 9.4.5-3 with the value of the Altitude subfield expressed in
             * units of meters. This field has an integer and a fractional part
             * with the radix point to the right of bit 5.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                                  Altitude (31..0), meters                                     |
             *                                                                                         ^
             *                                                                                         |
             *                                                                                    Radix Point
             */
            SECTION("Resolution") {
                // Observation 9.4.5-4: range +-67108 km, resolution 3.1 cm
                ALTITUDE = 1.0 + (1.0 / (1 << 5));
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x00, 0x00, 0x00, 0x21, // word 7
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.altitude(ALTITUDE);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().altitude() == ALTITUDE);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().altitude() == ALTITUDE);
                }
            } //  end SECTION("Resolution")
            SECTION("Min") {
                ALTITUDE = -67108; // hex(-67108<<5(radix)) = 0xffdf3b80
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0xFF, 0xDF, 0x3B, 0x80, // word 7
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.altitude(ALTITUDE);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().altitude() == ALTITUDE);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().altitude() == ALTITUDE);
                }
            } //  end SECTION("Min")
            SECTION("Max") {
                ALTITUDE = 67108; // hex(67108<<5(radix)) = 0x20c480
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x00, 0x20, 0xC4, 0x80, // word 7
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.altitude(ALTITUDE);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().altitude() == ALTITUDE);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().altitude() == ALTITUDE);
                }
            } //  end SECTION("Max")
        } // end SECTION("Rule 9.4.5-10")

        SECTION("Rule 9.4.5-11") {
            /*
             * The Speed Over Ground subfield shall use the 32-bit,
             * two’s-complement format shown in Figure 9.4.5-4 with the value
             * of the Speed Over Ground subfield expressed in units of meters
             * per second. This field has an integer and a fractional part with
             * the radix point to the right of bit 16.
             *
             * Observation 9.4.5-5: range 0 to 65636 m/s, resolution 1.5e-5 m/s
             *
             * Developer's Note: above observation believed to be typo, max
             * signed integer part can only be 32767 (0x7FFF), however this is
             * also the unspecified value from Rule 9.4.5-18. Instead using
             * 0x3FFF for test, which is still magnitudes higher than reasonable
             * live values.
             */
            SECTION("Resolution") {
                GROUND_SPEED = 1.0 + (1.0 / (1 << 16)); // represents 0001 0001
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x00, 0x01, 0x00, 0x01, // word 8
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.speed_over_ground(GROUND_SPEED);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().speed_over_ground() == GROUND_SPEED);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().speed_over_ground() == GROUND_SPEED);
                }
            } //  end SECTION("Resolution")
            SECTION("Min") {
                GROUND_SPEED = 0;
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x00, 0x00, 0x00, 0x00, // word 8
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.speed_over_ground(GROUND_SPEED);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().speed_over_ground() == GROUND_SPEED);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().speed_over_ground() == GROUND_SPEED);
                }
            } //  end SECTION("Min")
            SECTION("Max") {
                GROUND_SPEED = 16384 - (1.0 / (1 << 16)); // represents 3FFF FFFF
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x3F, 0xFF, 0xFF, 0xFF, // word 8
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.speed_over_ground(GROUND_SPEED);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().speed_over_ground() == GROUND_SPEED);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().speed_over_ground() == GROUND_SPEED);
                }
            } //  end SECTION("Max")
        } // end SECTION("Rule 9.4.5-11")

        SECTION("Rule 9.4.5-12") {
            /*
             * The Heading Angle subfield shall use the Geolocation Angle
             * Format shown in Figure 9.4.5-2, with the Heading Angle
             * expressing the platform’s orientation with respect to true North
             * in decimal degrees.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                                    Angle (31..0), degrees                                     |
             *                                      ^
             *                                      |
             *                                 Radix Point
             */
            // Observation 9.4.5-2 - resolution of 2.38e-7 degrees
            HEADING_ANGLE = 1.0 + (1.0 / (1 << 22)); // represents 0040 0001
            geo.heading_angle(HEADING_ANGLE);
            SECTION("GPS") {
                GPS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_gps());
                packet_in.formatted_gps(geo);
                CHECK(packet_in.has_formatted_gps());
                CHECK(packet_in.formatted_gps().heading_angle() == HEADING_ANGLE);
            }
            SECTION("INS") {
                INS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_ins());
                packet_in.formatted_ins(geo);
                CHECK(packet_in.has_formatted_ins());
                CHECK(packet_in.formatted_ins().heading_angle() == HEADING_ANGLE);
            }
            FORMATTED_GEO_BE = bytes { 
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x00, 0x40, 0x00, 0x01, // word 9
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            };
        } // end SECTION("Rule 9.4.5-12")

        SECTION("Rule 9.4.5-13") {
            /*
             * The Heading Angle value shall range from 0.0 to
             * +359.999999761582 degrees.
             * 
             * Note: with resolution of 2.38e-7 from Rule 9.4.5-12, the above
             * maximum is out of reach
             */
            SECTION("Min") {
                HEADING_ANGLE = 0;
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x00, 0x00, 0x00, 0x00, // word 9
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.heading_angle(HEADING_ANGLE);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().heading_angle() == HEADING_ANGLE);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().heading_angle() == HEADING_ANGLE);
                }
            } //  end SECTION("Min")
            SECTION("Max") {
                HEADING_ANGLE = 360.0 - (1.0 / (1 << 22)); // represents 59FF FFFFF
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x59, 0xFF, 0xFF, 0xFF, // word 9
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.heading_angle(HEADING_ANGLE);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().heading_angle() == HEADING_ANGLE);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().heading_angle() == HEADING_ANGLE);
                }
            } //  end SECTION("Max")
        } // end SECTION("Rule 9.4.5-13")

        SECTION("Rule 9.4.5-14") {
            /*
             * The Track Angle subfield shall use the Geolocation Angle
             * Format shown in Figure 9.4.5-2, with the Track Angle expressing
             * the platform’s direction of travel with respect to true North in
             * decimal degrees.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                                    Angle (31..0), degrees                                     |
             *                                      ^
             *                                      |
             *                                 Radix Point
             */
            // Observation 9.4.5-2 - resolution of 2.38e-7 degrees
            TRACK_ANGLE = 1.0 + (1.0 / (1 << 22)); // represents 0040 0001
            geo.track_angle(TRACK_ANGLE);
            SECTION("GPS") {
                GPS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_gps());
                packet_in.formatted_gps(geo);
                CHECK(packet_in.has_formatted_gps());
                CHECK(packet_in.formatted_gps().track_angle() == TRACK_ANGLE);
            }
            SECTION("INS") {
                INS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_ins());
                packet_in.formatted_ins(geo);
                CHECK(packet_in.has_formatted_ins());
                CHECK(packet_in.formatted_ins().track_angle() == TRACK_ANGLE);
            }
            FORMATTED_GEO_BE = bytes { 
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x00, 0x40, 0x00, 0x01, // word 10
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            };
        } // end SECTION("Rule 9.4.5-14")

        SECTION("Rule 9.4.5-15") {
            /*
             * The Track Angle value shall range from 0.0 to
             * +359.999999761582 degrees.
             * 
             * Note: with resolution of 2.38e-7 from Rule 9.4.5-12, the above
             * maximum is out of reach
             */
            SECTION("Min") {
                TRACK_ANGLE = 0;
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x00, 0x00, 0x00, 0x00, // word 10
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.track_angle(TRACK_ANGLE);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().track_angle() == TRACK_ANGLE);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().track_angle() == TRACK_ANGLE);
                }
            } //  end SECTION("Min")
            SECTION("Max") {
                TRACK_ANGLE = 360.0 - (1.0 / (1 << 22)); // represents 59FF FFFFF
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x59, 0xFF, 0xFF, 0xFF, // word 10
                    0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
                };
                geo.track_angle(TRACK_ANGLE);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().track_angle() == TRACK_ANGLE);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().track_angle() == TRACK_ANGLE);
                }
            } //  end SECTION("Max")
        } // end SECTION("Rule 9.4.5-15")

        SECTION("Rule 9.4.5-16") {
            /*
             * The Magnetic Variation subfield shall use the Geolocation Angle
             * Format shown in Figure 9.4.5-2, above, with the Magnetic
             * Variation subfield expressing magnetic variation from true North
             * in decimal degrees.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                                    Angle (31..0), degrees                                     |
             *                                      ^
             *                                      |
             *                                 Radix Point
             */
            // Observation 9.4.5-2 - resolution of 2.38e-7 degrees
            MAGNETIC_VARIATION = 1.0 + (1.0 / (1 << 22)); // represents 0040 0001
            geo.magnetic_variation(MAGNETIC_VARIATION);
            SECTION("GPS") {
                GPS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_gps());
                packet_in.formatted_gps(geo);
                CHECK(packet_in.has_formatted_gps());
                CHECK(packet_in.formatted_gps().magnetic_variation() == MAGNETIC_VARIATION);
            }
            SECTION("INS") {
                INS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_ins());
                packet_in.formatted_ins(geo);
                CHECK(packet_in.has_formatted_ins());
                CHECK(packet_in.formatted_ins().magnetic_variation() == MAGNETIC_VARIATION);
            }
            FORMATTED_GEO_BE = bytes { 
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x00, 0x40, 0x00, 0x01  // word 11 
            };
        } // end SECTION("Rule 9.4.5-16")

        SECTION("Rule 9.4.5-17") {
            /*
             * The Magnetic Variation value shall range from -180.0 (West) to
             * +180.0 (East) degrees.
             */
            SECTION("Min") {
                MAGNETIC_VARIATION = -180.0; // hex(-180<<22(radix)) = 0xd3000000
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0xD3, 0x00, 0x00, 0x00  // word 11
                };
                geo.magnetic_variation(MAGNETIC_VARIATION);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().magnetic_variation() == MAGNETIC_VARIATION);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().magnetic_variation() == MAGNETIC_VARIATION);
                }
            } //  end SECTION("Min")
            SECTION("Max") {
                MAGNETIC_VARIATION = 180.0; // hex(180<<22(radix)) = 0x2d000000
                FORMATTED_GEO_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                    0x2D, 0x00, 0x00, 0x00  // word 11
                };
                geo.magnetic_variation(MAGNETIC_VARIATION);
                SECTION("GPS") {
                    GPS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_gps());
                    packet_in.formatted_gps(geo);
                    CHECK(packet_in.has_formatted_gps());
                    CHECK(packet_in.formatted_gps().magnetic_variation() == MAGNETIC_VARIATION);
                }
                SECTION("INS") {
                    INS_TEST = true;
                    CHECK_FALSE(packet_in.has_formatted_ins());
                    packet_in.formatted_ins(geo);
                    CHECK(packet_in.has_formatted_ins());
                    CHECK(packet_in.formatted_ins().magnetic_variation() == MAGNETIC_VARIATION);
                }
            } //  end SECTION("Max")
        } // end SECTION("Rule 9.4.5-17")

        SECTION("Rule 9.4.5-18") {
            /*
             * The Latitude, Longitude, Altitude, Speed Over Ground, Heading,
             * Track Angle, and Magnetic Variation subfields shall take the
             * value 0x7FFFFFFF when unspecified.
             * 
             * Observation 9.4.5-7: These default subfield values are invalid
             * or practically impossible for all GPS and INS Geolocation
             * subfields.
             */
            SECTION("GPS") {
                GPS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_gps());
                packet_in.formatted_gps(geo);
                CHECK(packet_in.has_formatted_gps());
            }
            SECTION("INS") {
                INS_TEST = true;
                CHECK_FALSE(packet_in.has_formatted_ins());
                packet_in.formatted_ins(geo);
                CHECK(packet_in.has_formatted_ins());
            }
            FORMATTED_GEO_BE = bytes { 
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5
                0x7F, 0xFF, 0xFF, 0xFF, // word 6
                0x7F, 0xFF, 0xFF, 0xFF, // word 7
                0x7F, 0xFF, 0xFF, 0xFF, // word 8
                0x7F, 0xFF, 0xFF, 0xFF, // word 9
                0x7F, 0xFF, 0xFF, 0xFF, // word 10
                0x7F, 0xFF, 0xFF, 0xFF  // word 11
            };
        } // end SECTION("Rule 9.4.5-18")

        // Check bytes required
        CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        // Get buffer from pack
        auto data = packet_helper::pack(packet_in);
        CHECK(data.size() == PACKED_SIZE);
        auto* check_ptr = data.data();

        // Examine and check packed header
        const size_t HEADER_BYTES = 4;
        const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
        const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
        const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
        uint8_t FORMATTED_GEO_ENABLE;
        if (GPS_TEST) {
            FORMATTED_GEO_ENABLE = 0x1 << 6; // CIF 0 bit 14 (gps)
        } else if (INS_TEST) {
            FORMATTED_GEO_ENABLE = 0x1 << 5; // CIF 0 bit 13 (ins)
        }
        const bytes CIF0_BE{ 0, 0, FORMATTED_GEO_ENABLE, 0 };
        const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
        check_ptr += CIF0_BYTES;
        CHECK(packed_cif0 == CIF0_BE);

        // Examine and check packed Formatted Geolocation. Value shall be in big-endian format.
        const decltype(data) packed_formatted_geo(check_ptr, check_ptr + FORMATTED_GEO_BYTES);
        check_ptr += FORMATTED_GEO_BYTES;
        CHECK(packed_formatted_geo == FORMATTED_GEO_BE);

        // Check match
        CHECK_FALSE(packet_helper::match(data.data(), data.size()));

        // Unpack verifed packed data
        packet_type packet_out;
        packet_helper::unpack(packet_out, data.data(), data.size());

        // Examine and check unpacked packet header
        const auto& header = packet_out.header();
        CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
        CHECK_FALSE(header.class_id_enable());
        CHECK_FALSE(header.not_v49d0());
        CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
        CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
        CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
        CHECK(header.packet_size() == PACKET_SIZE);

        // Examine and check unpacked Stream ID
        CHECK(packet_out.stream_id() == STREAM_ID);

        // Examine and check unpacked Formatted Geolocation
        if (GPS_TEST) {
            CHECK(packet_out.has_formatted_gps());
            CHECK(packet_out.formatted_gps().tsi() == TSI);
            CHECK(packet_out.formatted_gps().tsf() == TSF);
            CHECK(packet_out.formatted_gps().integer_timestamp() == INTEGER_TIMESTAMP);
            CHECK(packet_out.formatted_gps().fractional_timestamp() == FRACTIONAL_TIMESTAMP);
            CHECK(packet_out.formatted_gps().latitude() == LATITUDE);
            CHECK(packet_out.formatted_gps().longitude() == LONGITUDE);
            CHECK(packet_out.formatted_gps().altitude() == ALTITUDE);
            CHECK(packet_out.formatted_gps().speed_over_ground() == GROUND_SPEED);
            CHECK(packet_out.formatted_gps().heading_angle() == HEADING_ANGLE);
            CHECK(packet_out.formatted_gps().track_angle() == TRACK_ANGLE);
            CHECK(packet_out.formatted_gps().magnetic_variation() == MAGNETIC_VARIATION);
        } else if (INS_TEST) {
            CHECK(packet_out.has_formatted_ins());
            CHECK(packet_out.formatted_ins().tsi() == TSI);
            CHECK(packet_out.formatted_ins().tsf() == TSF);
            CHECK(packet_out.formatted_ins().integer_timestamp() == INTEGER_TIMESTAMP);
            CHECK(packet_out.formatted_ins().fractional_timestamp() == FRACTIONAL_TIMESTAMP);
            CHECK(packet_out.formatted_ins().latitude() == LATITUDE);
            CHECK(packet_out.formatted_ins().longitude() == LONGITUDE);
            CHECK(packet_out.formatted_ins().altitude() == ALTITUDE);
            CHECK(packet_out.formatted_ins().speed_over_ground() == GROUND_SPEED);
            CHECK(packet_out.formatted_ins().heading_angle() == HEADING_ANGLE);
            CHECK(packet_out.formatted_ins().track_angle() == TRACK_ANGLE);
            CHECK(packet_out.formatted_ins().magnetic_variation() == MAGNETIC_VARIATION);
        }
    } // end SECTION("Formatted GPS")

    SECTION("Ephemeris") {
        /*
         * Rule 9.4.3-3: The ECEF Ephemeris field shall be expressed using the
         * format shown in Figure 9.4.3-2.
         * 
         * Rule 9.4.9-1: The Relative Ephemeris field shall be expressed using
         * the formats shown in Figure 9.4.3-2 and Figure 9.4.3-3. from
         * Section 9.4.3 regarding ECEF Ephemeris.
         * 
         * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
         * | 1    | Reserved  | TSI | TSF |                        GPS/INS Manufacturer OUI                       |
         * | 2    |                       Integer-second Timestamp of Position Fix (31..0)                        |
         * | 3    |                      Fractional-second Timestamp of Position Fix (63..32)                     |
         * | 4    |                      Fractional-second Timestamp of Position Fix (31..0)                      |
         * | 5    |                                   Position X (31..0), meters                                  |
         * | 6    |                                   Position Y (31..0), meters                                  |
         * | 7    |                                   Position Z (31..0), meters                                  |
         * | 8    |                               Attitude Alpha (31..0), degrees                                 |
         * | 9    |                                Attitude Beta (31..0), degrees                                 |
         * | 10   |                                 Attitude Phi (31..0), degrees                                 |
         * | 11   |                                  Velocity dX (31..0), meters/second                           |
         * | 12   |                                  Velocity dY (31..0), meters/second                           |
         * | 13   |                                  Velocity dZ (31..0), meters/second                           |
         */
        const size_t EPHEMERIS_BYTES { 52 };
        PACKED_SIZE += EPHEMERIS_BYTES;

        vrtgen::packing::Ephemeris ephemeris;
        vrtgen::packing::TSI TSI { vrtgen::packing::TSI::NONE };
        vrtgen::packing::TSF TSF { vrtgen::packing::TSF::NONE };
        uint32_t OUI { 0 };
        uint32_t INTEGER_TIMESTAMP { 0xFFFFFFFFu };
        uint64_t FRACTIONAL_TIMESTAMP { 0xFFFFFFFFFFFFFFFFu };
        // set fixed points to values that produce 0x7FFFFFFF per rule 9.4.3-8
        double POSITION_X = vrtgen::fixed::to_fp<32,5>(0x7FFFFFFF);
        double POSITION_Y = vrtgen::fixed::to_fp<32,5>(0x7FFFFFFF);
        double POSITION_Z = vrtgen::fixed::to_fp<32,5>(0x7FFFFFFF);
        double ATTITUDE_ALPHA = vrtgen::fixed::to_fp<32,22>(0x7FFFFFFF);
        double ATTITUDE_BETA = vrtgen::fixed::to_fp<32,22>(0x7FFFFFFF);
        double ATTITUDE_PHI = vrtgen::fixed::to_fp<32,22>(0x7FFFFFFF);
        double VELOCITY_DX = vrtgen::fixed::to_fp<32,16>(0x7FFFFFFF);
        double VELOCITY_DY = vrtgen::fixed::to_fp<32,16>(0x7FFFFFFF);
        double VELOCITY_DZ = vrtgen::fixed::to_fp<32,16>(0x7FFFFFFF);
        bytes EPHEMERIS_BE;
        bool ECEF_TEST { false };
        bool REL_TEST { false };
        packet_type packet_in;

        // Stream ID is required field. Set value to check
        const uint32_t STREAM_ID = 0x12345678;
        packet_in.stream_id(STREAM_ID);
        CHECK(packet_in.stream_id() == STREAM_ID);

        SECTION("Rule 9.4.3-4") {
            /*
             * Rule 9.4.3-4: The TSI, TSF, OUI, and Timestamp of Position Fix
             * fields shall follow the rules of the corresponding Formatted GPS
             * Geolocation fields given in Section 9.4.5.
             */
            SECTION("OUI - Rule 9.4.5-2") {
                /*
                * Rule 9.4.5-2: The GPS/INS Manufacturer OUI subfield shall
                * contain the 24-bit field for the IEEE registered,
                * Organizationally Unique Identifier (company identifier) in the
                * range 00-00-00 to FF-FE-FF (inclusive) of the GPS/INS
                * manufacturer.
                */
                OUI = 0x123456;
                ephemeris.manufacturer_oui(OUI);
                EPHEMERIS_BE = bytes { 
                    0x00, 0x12, 0x34, 0x56, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                };
                SECTION("ECEF") {
                    ECEF_TEST = true;
                    CHECK_FALSE(packet_in.has_ecef_ephemeris());
                    packet_in.ecef_ephemeris(ephemeris);
                    CHECK(packet_in.has_ecef_ephemeris());
                    CHECK(packet_in.ecef_ephemeris().manufacturer_oui() == OUI);
                }
                SECTION("Relative") {
                    REL_TEST = true;
                    CHECK_FALSE(packet_in.has_relative_ephemeris());
                    packet_in.relative_ephemeris(ephemeris);
                    CHECK(packet_in.has_relative_ephemeris());
                    CHECK(packet_in.relative_ephemeris().manufacturer_oui() == OUI);
                }
            } // end SECTION("OUI - Rule 9.4.5-2")

            SECTION("TSI - Rule 9.4.5-3") {
                /*
                 * Rule 9.4.5-3: The TSI field in the Formatted GPS Geolocation
                 * field shall accurately indicate the type of Integer-seconds
                 * Timestamp included in the packet according to the code
                 * assignments in Table 9.4.5-1.
                 * 
                 * | TSI Code | Meaning   |
                 * | -------- | --------- |
                 * | 00       | Undefined |
                 * | 01       | UTC       |
                 * | 10       | GPS Time  |
                 * | 11       | Other     |
                 * | -------- | --------- |
                 */
                SECTION("Undefined") {
                    TSI = vrtgen::packing::TSI::NONE;
                    EPHEMERIS_BE = bytes { 
                        0x00, 0x00, 0x00, 0x00, // word 1
                        0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                    };
                    ephemeris.tsi(TSI);
                    SECTION("ECEF") {
                        ECEF_TEST = true;
                        CHECK_FALSE(packet_in.has_ecef_ephemeris());
                        packet_in.ecef_ephemeris(ephemeris);
                        CHECK(packet_in.has_ecef_ephemeris());
                        CHECK(packet_in.ecef_ephemeris().tsi() == TSI);
                    }
                    SECTION("Relative") {
                        REL_TEST = true;
                        CHECK_FALSE(packet_in.has_relative_ephemeris());
                        packet_in.relative_ephemeris(ephemeris);
                        CHECK(packet_in.has_relative_ephemeris());
                        CHECK(packet_in.relative_ephemeris().tsi() == TSI);
                    }
                } // end SECTION("Undefined")
                SECTION("UTC") {
                    TSI = vrtgen::packing::TSI::UTC;
                    EPHEMERIS_BE = bytes { 
                        0x04, 0x00, 0x00, 0x00, // word 1
                        0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                    };
                    ephemeris.tsi(TSI);
                    SECTION("ECEF") {
                        ECEF_TEST = true;
                        CHECK_FALSE(packet_in.has_ecef_ephemeris());
                        packet_in.ecef_ephemeris(ephemeris);
                        CHECK(packet_in.has_ecef_ephemeris());
                        CHECK(packet_in.ecef_ephemeris().tsi() == TSI);
                    }
                    SECTION("Relative") {
                        REL_TEST = true;
                        CHECK_FALSE(packet_in.has_relative_ephemeris());
                        packet_in.relative_ephemeris(ephemeris);
                        CHECK(packet_in.has_relative_ephemeris());
                        CHECK(packet_in.relative_ephemeris().tsi() == TSI);
                    }
                } // end SECTION("UTC")
                SECTION("GPS Time") {
                    TSI = vrtgen::packing::TSI::GPS;
                    EPHEMERIS_BE = bytes { 
                        0x08, 0x00, 0x00, 0x00, // word 1
                        0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                    };
                    ephemeris.tsi(TSI);
                    SECTION("ECEF") {
                        ECEF_TEST = true;
                        CHECK_FALSE(packet_in.has_ecef_ephemeris());
                        packet_in.ecef_ephemeris(ephemeris);
                        CHECK(packet_in.has_ecef_ephemeris());
                        CHECK(packet_in.ecef_ephemeris().tsi() == TSI);
                    }
                    SECTION("Relative") {
                        REL_TEST = true;
                        CHECK_FALSE(packet_in.has_relative_ephemeris());
                        packet_in.relative_ephemeris(ephemeris);
                        CHECK(packet_in.has_relative_ephemeris());
                        CHECK(packet_in.relative_ephemeris().tsi() == TSI);
                    }
                } // end SECTION("GPS Time")
                SECTION("Other") {
                    TSI = vrtgen::packing::TSI::OTHER;
                    EPHEMERIS_BE = bytes { 
                        0x0C, 0x00, 0x00, 0x00, // word 1
                        0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                    };
                    ephemeris.tsi(TSI);
                    SECTION("ECEF") {
                        ECEF_TEST = true;
                        CHECK_FALSE(packet_in.has_ecef_ephemeris());
                        packet_in.ecef_ephemeris(ephemeris);
                        CHECK(packet_in.has_ecef_ephemeris());
                        CHECK(packet_in.ecef_ephemeris().tsi() == TSI);
                    }
                    SECTION("Relative") {
                        REL_TEST = true;
                        CHECK_FALSE(packet_in.has_relative_ephemeris());
                        packet_in.relative_ephemeris(ephemeris);
                        CHECK(packet_in.has_relative_ephemeris());
                        CHECK(packet_in.relative_ephemeris().tsi() == TSI);
                    }
                } // end SECTION("Other")
            } // end SECTION("TSI - Rule 9.4.5-3")

            SECTION("TSF - Rule 9.4.5-4") {
                /*
                * The TSF field in the Formatted GPS Geolocation field shall
                * accurately indicate the type of Fractional-seconds Timestamp
                * included in the packet according to the code assignments in
                * Table 9.4.5-2.
                * 
                * | TSF Code | Meaning                 |
                * | -------- | ----------------------- |
                * | 00       | Undefined               |
                * | 01       | Sample Count Time       |
                * | 10       | Real (Picoseconds) Time |
                * | 11       | Free-Running Count Time |
                * | -------- | ----------------------- |
                */
                SECTION("Undefined") {
                    TSF = vrtgen::packing::TSF::NONE;
                    EPHEMERIS_BE = bytes { 
                        0x00, 0x00, 0x00, 0x00, // word 1
                        0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                    };
                    ephemeris.tsf(TSF);
                    SECTION("ECEF") {
                        ECEF_TEST = true;
                        CHECK_FALSE(packet_in.has_ecef_ephemeris());
                        packet_in.ecef_ephemeris(ephemeris);
                        CHECK(packet_in.has_ecef_ephemeris());
                        CHECK(packet_in.ecef_ephemeris().tsf() == TSF);
                    }
                    SECTION("Relative") {
                        REL_TEST = true;
                        CHECK_FALSE(packet_in.has_relative_ephemeris());
                        packet_in.relative_ephemeris(ephemeris);
                        CHECK(packet_in.has_relative_ephemeris());
                        CHECK(packet_in.relative_ephemeris().tsf() == TSF);
                    }
                } // end SECTION("Undefined")
                SECTION("Sample Count Time") {
                    TSF = vrtgen::packing::TSF::SAMPLE_COUNT;
                    EPHEMERIS_BE = bytes { 
                        0x01, 0x00, 0x00, 0x00, // word 1
                        0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                    };
                    ephemeris.tsf(TSF);
                    SECTION("ECEF") {
                        ECEF_TEST = true;
                        CHECK_FALSE(packet_in.has_ecef_ephemeris());
                        packet_in.ecef_ephemeris(ephemeris);
                        CHECK(packet_in.has_ecef_ephemeris());
                        CHECK(packet_in.ecef_ephemeris().tsf() == TSF);
                    }
                    SECTION("Relative") {
                        REL_TEST = true;
                        CHECK_FALSE(packet_in.has_relative_ephemeris());
                        packet_in.relative_ephemeris(ephemeris);
                        CHECK(packet_in.has_relative_ephemeris());
                        CHECK(packet_in.relative_ephemeris().tsf() == TSF);
                    }
                } // end SECTION("Sample Count Time")
                SECTION("Real (Picoseconds) Time") {
                    TSF = vrtgen::packing::TSF::REAL_TIME;
                    EPHEMERIS_BE = bytes { 
                        0x02, 0x00, 0x00, 0x00, // word 1
                        0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                    };
                    ephemeris.tsf(TSF);
                    SECTION("ECEF") {
                        ECEF_TEST = true;
                        CHECK_FALSE(packet_in.has_ecef_ephemeris());
                        packet_in.ecef_ephemeris(ephemeris);
                        CHECK(packet_in.has_ecef_ephemeris());
                        CHECK(packet_in.ecef_ephemeris().tsf() == TSF);
                    }
                    SECTION("Relative") {
                        REL_TEST = true;
                        CHECK_FALSE(packet_in.has_relative_ephemeris());
                        packet_in.relative_ephemeris(ephemeris);
                        CHECK(packet_in.has_relative_ephemeris());
                        CHECK(packet_in.relative_ephemeris().tsf() == TSF);
                    }
                } // end SECTION("Real (Picoseconds) Time")
                SECTION("Other") {
                    TSF = vrtgen::packing::TSF::FREE_RUNNING;
                    EPHEMERIS_BE = bytes { 
                        0x03, 0x00, 0x00, 0x00, // word 1
                        0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                        0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                        0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                    };
                    ephemeris.tsf(TSF);
                    SECTION("ECEF") {
                        ECEF_TEST = true;
                        CHECK_FALSE(packet_in.has_ecef_ephemeris());
                        packet_in.ecef_ephemeris(ephemeris);
                        CHECK(packet_in.has_ecef_ephemeris());
                        CHECK(packet_in.ecef_ephemeris().tsf() == TSF);
                    }
                    SECTION("Relative") {
                        REL_TEST = true;
                        CHECK_FALSE(packet_in.has_relative_ephemeris());
                        packet_in.relative_ephemeris(ephemeris);
                        CHECK(packet_in.has_relative_ephemeris());
                        CHECK(packet_in.relative_ephemeris().tsf() == TSF);
                    }
                } // end SECTION("Other")
            } // end SECTION("TSF - Rule 9.4.5-4")

            SECTION("Timestamps - Rule 9.4.5-5") {
                /*
                * When the TSI or TSF fields are non-zero the corresponding
                * Timestamp of Position Fix subfield shall express the time of the
                * most recent location fix in the format given in Sections
                * 6.1.45.1.4 and 5.1.4.2
                */
                TSI = vrtgen::packing::TSI::UTC;
                TSF = vrtgen::packing::TSF::REAL_TIME;
                INTEGER_TIMESTAMP = 0x12345678;
                FRACTIONAL_TIMESTAMP = 0x12345678ABCDEF;
                ephemeris.tsi(TSI);
                ephemeris.tsf(TSF);
                ephemeris.integer_timestamp(INTEGER_TIMESTAMP);
                ephemeris.fractional_timestamp(FRACTIONAL_TIMESTAMP);
                SECTION("ECEF") {
                    ECEF_TEST = true;
                    CHECK_FALSE(packet_in.has_ecef_ephemeris());
                    packet_in.ecef_ephemeris(ephemeris);
                    CHECK(packet_in.has_ecef_ephemeris());
                    CHECK(packet_in.ecef_ephemeris().tsi() == TSI);
                    CHECK(packet_in.ecef_ephemeris().tsf() == TSF);
                    CHECK(packet_in.ecef_ephemeris().integer_timestamp() == INTEGER_TIMESTAMP);
                    CHECK(packet_in.ecef_ephemeris().fractional_timestamp() == FRACTIONAL_TIMESTAMP);
                }
                SECTION("Relative") {
                    REL_TEST = true;
                    CHECK_FALSE(packet_in.has_relative_ephemeris());
                    packet_in.relative_ephemeris(ephemeris);
                    CHECK(packet_in.has_relative_ephemeris());
                    CHECK(packet_in.relative_ephemeris().tsi() == TSI);
                    CHECK(packet_in.relative_ephemeris().tsf() == TSF);
                    CHECK(packet_in.relative_ephemeris().integer_timestamp() == INTEGER_TIMESTAMP);
                    CHECK(packet_in.relative_ephemeris().fractional_timestamp() == FRACTIONAL_TIMESTAMP);
                }
                EPHEMERIS_BE = bytes { 
                    0x06, 0x00, 0x00, 0x00, // word 1
                    0x12, 0x34, 0x56, 0x78, // word 2
                    0x00, 0x12, 0x34, 0x56, // word 3
                    0x78, 0xAB, 0xCD, 0xEF, // word 4
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                };
            } // end SECTION("Timestamps - Rule 9.4.5-5")

            SECTION("Timestamps - Rule 9.4.5-6") {
                /*
                * When the TSI or TSF fields are zero the corresponding Timestamp
                * of Position Fix subfield words shall take the value 0xFFFFFFFF.
                */
                TSI = vrtgen::packing::TSI::NONE;
                TSF = vrtgen::packing::TSF::NONE;
                INTEGER_TIMESTAMP = 0xFFFFFFFF;
                FRACTIONAL_TIMESTAMP = 0xFFFFFFFFFFFFFFFF;
                ephemeris.tsi(TSI);
                ephemeris.tsf(TSF);
                ephemeris.integer_timestamp(INTEGER_TIMESTAMP);
                ephemeris.fractional_timestamp(FRACTIONAL_TIMESTAMP);
                SECTION("ECEF") {
                    ECEF_TEST = true;
                    CHECK_FALSE(packet_in.has_ecef_ephemeris());
                    packet_in.ecef_ephemeris(ephemeris);
                    CHECK(packet_in.has_ecef_ephemeris());
                    CHECK(packet_in.ecef_ephemeris().tsi() == TSI);
                    CHECK(packet_in.ecef_ephemeris().tsf() == TSF);
                    CHECK(packet_in.ecef_ephemeris().integer_timestamp() == INTEGER_TIMESTAMP);
                    CHECK(packet_in.ecef_ephemeris().fractional_timestamp() == FRACTIONAL_TIMESTAMP);
                }
                SECTION("Relative") {
                    REL_TEST = true;
                    CHECK_FALSE(packet_in.has_relative_ephemeris());
                    packet_in.relative_ephemeris(ephemeris);
                    CHECK(packet_in.has_relative_ephemeris());
                    CHECK(packet_in.relative_ephemeris().tsi() == TSI);
                    CHECK(packet_in.relative_ephemeris().tsf() == TSF);
                    CHECK(packet_in.relative_ephemeris().integer_timestamp() == INTEGER_TIMESTAMP);
                    CHECK(packet_in.relative_ephemeris().fractional_timestamp() == FRACTIONAL_TIMESTAMP);
                }
                EPHEMERIS_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                };
            } // end SECTION("Timestamps - Rule 9.4.5-6")
        } // end SECTION("Rule 9.4.3-4")
    
        SECTION("Rule 9.4.3-5") {
            /*
             * The position coordinates of the ECEF Ephemeris field shall use
             * the 32-bit, two’s complement "Position" format shown in Figure
             * 9.4.3-3. The position values shall be expressed in units of
             * meters. This field has an integer and a fractional part with the
             * radix point to the right of bit 5.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                                   Position (31..0), meters                                    |
             *                                                                                         ^
             *                                                                                         |
             *                                                                                    Radix Point
             */
            SECTION("Position X") {
                POSITION_X = 1.0 + (1.0 / (1 << 5)); // represents 0000 0021
                ephemeris.position_x(POSITION_X);
                SECTION("ECEF") {
                    ECEF_TEST = true;
                    CHECK_FALSE(packet_in.has_ecef_ephemeris());
                    packet_in.ecef_ephemeris(ephemeris);
                    CHECK(packet_in.has_ecef_ephemeris());
                    CHECK(packet_in.ecef_ephemeris().position_x() == POSITION_X);
                }
                SECTION("Relative") {
                    REL_TEST = true;
                    CHECK_FALSE(packet_in.has_relative_ephemeris());
                    packet_in.relative_ephemeris(ephemeris);
                    CHECK(packet_in.has_relative_ephemeris());
                    CHECK(packet_in.relative_ephemeris().position_x() == POSITION_X);
                }
                EPHEMERIS_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x00, 0x00, 0x00, 0x21, // word 5
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                };
            } // end SECTION("Position X")
            SECTION("Position Y") {
                POSITION_Y = 1.0 + (1.0 / (1 << 5)); // represents 0000 0021
                ephemeris.position_y(POSITION_Y);
                SECTION("ECEF") {
                    ECEF_TEST = true;
                    CHECK_FALSE(packet_in.has_ecef_ephemeris());
                    packet_in.ecef_ephemeris(ephemeris);
                    CHECK(packet_in.has_ecef_ephemeris());
                    CHECK(packet_in.ecef_ephemeris().position_y() == POSITION_Y);
                }
                SECTION("Relative") {
                    REL_TEST = true;
                    CHECK_FALSE(packet_in.has_relative_ephemeris());
                    packet_in.relative_ephemeris(ephemeris);
                    CHECK(packet_in.has_relative_ephemeris());
                    CHECK(packet_in.relative_ephemeris().position_y() == POSITION_Y);
                }
                EPHEMERIS_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                    0x00, 0x00, 0x00, 0x21, // word 6 
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                };
            } // end SECTION("Position Y")
            SECTION("Position X") {
                POSITION_Z = 1.0 + (1.0 / (1 << 5)); // represents 0000 0021
                ephemeris.position_z(POSITION_Z);
                SECTION("ECEF") {
                    ECEF_TEST = true;
                    CHECK_FALSE(packet_in.has_ecef_ephemeris());
                    packet_in.ecef_ephemeris(ephemeris);
                    CHECK(packet_in.has_ecef_ephemeris());
                    CHECK(packet_in.ecef_ephemeris().position_z() == POSITION_Z);
                }
                SECTION("Relative") {
                    REL_TEST = true;
                    CHECK_FALSE(packet_in.has_relative_ephemeris());
                    packet_in.relative_ephemeris(ephemeris);
                    CHECK(packet_in.has_relative_ephemeris());
                    CHECK(packet_in.relative_ephemeris().position_z() == POSITION_Z);
                }
                EPHEMERIS_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                    0x00, 0x00, 0x00, 0x21, // word 7
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                };
            } // end SECTION("Position Z")
        } // end SECTION("Rule 9.4.3-5")

        SECTION ("Rule 9.4.3-6") {
            /*
             * The attitude coordinates of the ECEF Ephemeris field shall use
             * the 32-bit, two’scomplement "Attitude" format shown in Figure
             * 9.4.3-3. The attitude values shall be expressed in units of
             * decimal degrees. This field has an integer and a fractional part
             * with the radix point to the right of bit 22.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                                  Attitude (31..0), degrees                                    |
             *                                      ^
             *                                      |
             *                                 Radix Point
             */
            SECTION("Attitude Alpha") {
                ATTITUDE_ALPHA = 1.0 + (1.0 / (1 << 22)); // represents 0040 0001
                ephemeris.attitude_alpha(ATTITUDE_ALPHA);
                SECTION("ECEF") {
                    ECEF_TEST = true;
                    CHECK_FALSE(packet_in.has_ecef_ephemeris());
                    packet_in.ecef_ephemeris(ephemeris);
                    CHECK(packet_in.has_ecef_ephemeris());
                    CHECK(packet_in.ecef_ephemeris().attitude_alpha() == ATTITUDE_ALPHA);
                }
                SECTION("Relative") {
                    REL_TEST = true;
                    CHECK_FALSE(packet_in.has_relative_ephemeris());
                    packet_in.relative_ephemeris(ephemeris);
                    CHECK(packet_in.has_relative_ephemeris());
                    CHECK(packet_in.relative_ephemeris().attitude_alpha() == ATTITUDE_ALPHA);
                }
                EPHEMERIS_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                    0x00, 0x40, 0x00, 0x01, // word 8
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                };
            } // end SECTION("Attitude Alpha")
            SECTION("Attitude Beta") {
                ATTITUDE_BETA = 1.0 + (1.0 / (1 << 22)); // represents 0040 0001
                ephemeris.attitude_beta(ATTITUDE_BETA);
                SECTION("ECEF") {
                    ECEF_TEST = true;
                    CHECK_FALSE(packet_in.has_ecef_ephemeris());
                    packet_in.ecef_ephemeris(ephemeris);
                    CHECK(packet_in.has_ecef_ephemeris());
                    CHECK(packet_in.ecef_ephemeris().attitude_beta() == ATTITUDE_BETA);
                }
                SECTION("Relative") {
                    REL_TEST = true;
                    CHECK_FALSE(packet_in.has_relative_ephemeris());
                    packet_in.relative_ephemeris(ephemeris);
                    CHECK(packet_in.has_relative_ephemeris());
                    CHECK(packet_in.relative_ephemeris().attitude_beta() == ATTITUDE_BETA);
                }
                EPHEMERIS_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                    0x00, 0x40, 0x00, 0x01, // word 9
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                };
            } // end SECTION("Attitude Beta")
            SECTION("Attitude Phi") {
                ATTITUDE_PHI = 1.0 + (1.0 / (1 << 22)); // represents 0040 0001
                ephemeris.attitude_phi(ATTITUDE_PHI);
                SECTION("ECEF") {
                    ECEF_TEST = true;
                    CHECK_FALSE(packet_in.has_ecef_ephemeris());
                    packet_in.ecef_ephemeris(ephemeris);
                    CHECK(packet_in.has_ecef_ephemeris());
                    CHECK(packet_in.ecef_ephemeris().attitude_phi() == ATTITUDE_PHI);
                }
                SECTION("Relative") {
                    REL_TEST = true;
                    CHECK_FALSE(packet_in.has_relative_ephemeris());
                    packet_in.relative_ephemeris(ephemeris);
                    CHECK(packet_in.has_relative_ephemeris());
                    CHECK(packet_in.relative_ephemeris().attitude_phi() == ATTITUDE_PHI);
                }
                EPHEMERIS_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                    0x00, 0x40, 0x00, 0x01, // word 10
                    0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                };
            } // end SECTION("Attitude Phi")
        } // end SECTION ("Rule 9.4.3-6")
    
        SECTION ("Rule 9.4.3-7") {
            /*
             * The velocity coordinates of the ECEF Ephemeris field shall use
             * the 32-bit, two’scomplement "Velocity" format shown in Figure
             * 9.4.3-3. The velocity values shall be expressed in units of
             * meters per second. This field has an integer and a fractional
             * part with the radix point to the right of bit 16.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                               Velocity (31..0), meters/second                                 |
             *                                                        ^
             *                                                        |
             *                                                   Radix Point
             */
            SECTION("Velocity dX") {
                VELOCITY_DX = 1.0 + (1.0 / (1 << 16)); // represents 0001 0001
                ephemeris.velocity_dx(VELOCITY_DX);
                SECTION("ECEF") {
                    ECEF_TEST = true;
                    CHECK_FALSE(packet_in.has_ecef_ephemeris());
                    packet_in.ecef_ephemeris(ephemeris);
                    CHECK(packet_in.has_ecef_ephemeris());
                    CHECK(packet_in.ecef_ephemeris().velocity_dx() == VELOCITY_DX);
                }
                SECTION("Relative") {
                    REL_TEST = true;
                    CHECK_FALSE(packet_in.has_relative_ephemeris());
                    packet_in.relative_ephemeris(ephemeris);
                    CHECK(packet_in.has_relative_ephemeris());
                    CHECK(packet_in.relative_ephemeris().velocity_dx() == VELOCITY_DX);
                }
                EPHEMERIS_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                    0x00, 0x01, 0x00, 0x01, // word 11
                    0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                };
            } // end SECTION("Velocity dX")
            SECTION("Velocity dY") {
                VELOCITY_DY = 1.0 + (1.0 / (1 << 16)); // represents 0001 0001
                ephemeris.velocity_dy(VELOCITY_DY);
                SECTION("ECEF") {
                    ECEF_TEST = true;
                    CHECK_FALSE(packet_in.has_ecef_ephemeris());
                    packet_in.ecef_ephemeris(ephemeris);
                    CHECK(packet_in.has_ecef_ephemeris());
                    CHECK(packet_in.ecef_ephemeris().velocity_dy() == VELOCITY_DY);
                }
                SECTION("Relative") {
                    REL_TEST = true;
                    CHECK_FALSE(packet_in.has_relative_ephemeris());
                    packet_in.relative_ephemeris(ephemeris);
                    CHECK(packet_in.has_relative_ephemeris());
                    CHECK(packet_in.relative_ephemeris().velocity_dy() == VELOCITY_DY);
                }
                EPHEMERIS_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                    0x00, 0x01, 0x00, 0x01, // word 12
                    0x7F, 0xFF, 0xFF, 0xFF  // word 13 (per rule 9.4.3-8)
                };
            } // end SECTION("Velocity dY")
            SECTION("Velocity dZ") {
                VELOCITY_DZ = 1.0 + (1.0 / (1 << 16)); // represents 0001 0001
                ephemeris.velocity_dz(VELOCITY_DZ);
                SECTION("ECEF") {
                    ECEF_TEST = true;
                    CHECK_FALSE(packet_in.has_ecef_ephemeris());
                    packet_in.ecef_ephemeris(ephemeris);
                    CHECK(packet_in.has_ecef_ephemeris());
                    CHECK(packet_in.ecef_ephemeris().velocity_dz() == VELOCITY_DZ);
                }
                SECTION("Relative") {
                    REL_TEST = true;
                    CHECK_FALSE(packet_in.has_relative_ephemeris());
                    packet_in.relative_ephemeris(ephemeris);
                    CHECK(packet_in.has_relative_ephemeris());
                    CHECK(packet_in.relative_ephemeris().velocity_dz() == VELOCITY_DZ);
                }
                EPHEMERIS_BE = bytes { 
                    0x00, 0x00, 0x00, 0x00, // word 1
                    0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                    0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 11 (per rule 9.4.3-8)
                    0x7F, 0xFF, 0xFF, 0xFF, // word 12 (per rule 9.4.3-8)
                    0x00, 0x01, 0x00, 0x01  // word 13
                };
            } // end SECTION("Velocity dZ")
        } // end SECTION ("Rule 9.4.3-7")

        SECTION ("Rule 9.4.3-8") {
            /*
             * Each word of the Position, Attitude, and Velocity subfields in
             * the ECEF Ephemeris field shall take the value 0x7FFFFFFF when
             * the value is unknown.
             */
            SECTION("ECEF") {
                ECEF_TEST = true;
                CHECK_FALSE(packet_in.has_ecef_ephemeris());
                packet_in.ecef_ephemeris(ephemeris);
                CHECK(packet_in.has_ecef_ephemeris());
            }
            SECTION("Relative") {
                REL_TEST = true;
                CHECK_FALSE(packet_in.has_relative_ephemeris());
                packet_in.relative_ephemeris(ephemeris);
                CHECK(packet_in.has_relative_ephemeris());
            }
            EPHEMERIS_BE = bytes {
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5
                0x7F, 0xFF, 0xFF, 0xFF, // word 6
                0x7F, 0xFF, 0xFF, 0xFF, // word 7
                0x7F, 0xFF, 0xFF, 0xFF, // word 8
                0x7F, 0xFF, 0xFF, 0xFF, // word 9
                0x7F, 0xFF, 0xFF, 0xFF, // word 10
                0x7F, 0xFF, 0xFF, 0xFF, // word 11
                0x7F, 0xFF, 0xFF, 0xFF, // word 12
                0x7F, 0xFF, 0xFF, 0xFF  // word 13
            };
        } // end SECTION ("Rule 9.4.3-8")

        // Check bytes required
        CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        // Get buffer from pack
        auto data = packet_helper::pack(packet_in);
        CHECK(data.size() == PACKED_SIZE);
        auto* check_ptr = data.data();

        // Examine and check packed header
        const size_t HEADER_BYTES = 4;
        const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
        const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
        const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
        uint8_t EPHEMERIS_ENABLE;
        if (ECEF_TEST) {
            EPHEMERIS_ENABLE = 0x1 << 4; // CIF 0 bit 12 (ecef ephemeris)
        } else if (REL_TEST) {
            EPHEMERIS_ENABLE = 0x1 << 3; // CIF 0 bit 11 (relative ephemeris)
        }
        const bytes CIF0_BE{ 0, 0, EPHEMERIS_ENABLE, 0 };
        const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
        check_ptr += CIF0_BYTES;
        CHECK(packed_cif0 == CIF0_BE);

        // Examine and check packed Ephemeris. Value shall be in big-endian format.
        const decltype(data) packed_ephemeris(check_ptr, check_ptr + EPHEMERIS_BYTES);
        check_ptr += EPHEMERIS_BYTES;
        CHECK(packed_ephemeris == EPHEMERIS_BE);

        // Check match
        CHECK_FALSE(packet_helper::match(data.data(), data.size()));

        // Unpack verifed packed data
        packet_type packet_out;
        packet_helper::unpack(packet_out, data.data(), data.size());

        // Examine and check unpacked packet header
        const auto& header = packet_out.header();
        CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
        CHECK_FALSE(header.class_id_enable());
        CHECK_FALSE(header.not_v49d0());
        CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
        CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
        CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
        CHECK(header.packet_size() == PACKET_SIZE);

        // Examine and check unpacked Stream ID
        CHECK(packet_out.stream_id() == STREAM_ID);

        // Examine and check unpacked Ephemeris
        if (ECEF_TEST) {
            CHECK(packet_out.has_ecef_ephemeris());
            CHECK(packet_out.ecef_ephemeris().tsi() == TSI);
            CHECK(packet_out.ecef_ephemeris().tsf() == TSF);
            CHECK(packet_out.ecef_ephemeris().integer_timestamp() == INTEGER_TIMESTAMP);
            CHECK(packet_out.ecef_ephemeris().fractional_timestamp() == FRACTIONAL_TIMESTAMP);
            CHECK(packet_out.ecef_ephemeris().position_x() == POSITION_X);
            CHECK(packet_out.ecef_ephemeris().position_y() == POSITION_Y);
            CHECK(packet_out.ecef_ephemeris().position_z() == POSITION_Z);
            CHECK(packet_out.ecef_ephemeris().attitude_alpha() == ATTITUDE_ALPHA);
            CHECK(packet_out.ecef_ephemeris().attitude_beta() == ATTITUDE_BETA);
            CHECK(packet_out.ecef_ephemeris().attitude_phi() == ATTITUDE_PHI);
            CHECK(packet_out.ecef_ephemeris().velocity_dx() == VELOCITY_DX);
            CHECK(packet_out.ecef_ephemeris().velocity_dy() == VELOCITY_DY);
            CHECK(packet_out.ecef_ephemeris().velocity_dz() == VELOCITY_DZ);
        } else if (REL_TEST) {
            CHECK(packet_out.has_relative_ephemeris());
            CHECK(packet_out.relative_ephemeris().tsi() == TSI);
            CHECK(packet_out.relative_ephemeris().tsf() == TSF);
            CHECK(packet_out.relative_ephemeris().integer_timestamp() == INTEGER_TIMESTAMP);
            CHECK(packet_out.relative_ephemeris().fractional_timestamp() == FRACTIONAL_TIMESTAMP);
            CHECK(packet_out.relative_ephemeris().position_x() == POSITION_X);
            CHECK(packet_out.relative_ephemeris().position_y() == POSITION_Y);
            CHECK(packet_out.relative_ephemeris().position_z() == POSITION_Z);
            CHECK(packet_out.relative_ephemeris().attitude_alpha() == ATTITUDE_ALPHA);
            CHECK(packet_out.relative_ephemeris().attitude_beta() == ATTITUDE_BETA);
            CHECK(packet_out.relative_ephemeris().attitude_phi() == ATTITUDE_PHI);
            CHECK(packet_out.relative_ephemeris().velocity_dx() == VELOCITY_DX);
            CHECK(packet_out.relative_ephemeris().velocity_dy() == VELOCITY_DY);
            CHECK(packet_out.relative_ephemeris().velocity_dz() == VELOCITY_DZ);
        }
    } // end SECTION("Epehmeris")

    SECTION("Ephemeris Ref ID") {
        const size_t EPHEM_REF_ID_BYTES = 4;
        PACKED_SIZE += EPHEM_REF_ID_BYTES;

        SECTION("Rule 9.4.4-1") {
            /*
            * Rule 9.4.4-1: The Ephemeris Reference Identifier, when used, shall
            * contain the Stream ID of the VRT Context Packet Stream whose ECEF
            * Ephemeris is necessary to translate the Relative Ephemeris given in
            * this Context Packet Stream to ECEF coordinates.
            */
            packet_type packet_in;

            // Stream ID is required field. Set value to check
            const uint32_t STREAM_ID = 0x12345678;
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            // Set ephemeris ref id value to check
            const uint32_t EPHEM_REF_ID = 0x12345678;
            CHECK_FALSE(packet_in.has_ephemeris_ref_id());
            packet_in.ephemeris_ref_id(EPHEM_REF_ID);
            CHECK(packet_in.has_ephemeris_ref_id());
            CHECK(packet_in.ephemeris_ref_id() == EPHEM_REF_ID);

            // Check bytes required
            CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = packet_helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
            const uint8_t EPHEM_REF_ID_ENABLE = 0x1 << 2; // CIF 0 bit 10
            const bytes CIF0_BE{ 0, 0, EPHEM_REF_ID_ENABLE, 0 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed Ephemeris Ref ID. Value shall be in big-endian format.
            const bytes EPHEM_REF_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
            const decltype(data) packed_ref_point_id(check_ptr, check_ptr + EPHEM_REF_ID_BYTES);
            check_ptr += EPHEM_REF_ID_BYTES;
            CHECK(packed_ref_point_id == EPHEM_REF_ID_BE);

            // Check match
            CHECK_FALSE(packet_helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked Ephemeris Ref ID
            CHECK(packet_out.has_ephemeris_ref_id());
            CHECK(packet_out.ephemeris_ref_id() == EPHEM_REF_ID);
        } // end SECTION("Rule 9.4.4-1")
    } // end SECTION("Ephemeris Ref ID")

} // end TEST_CASE("Context Packet CIF0 Optional")

TEST_CASE("Context Packet CIF1 Optional")
{
    using packet_type = TestContext5;
    using packet_helper = packet_type::helper;

    // Sizes for all required fields
    size_t PACKED_SIZE = 4 + // header
                         4 + // stream_id
                         4 + // cif 0
                         4;  // cif 1

    SECTION("Phase Offset") {
        // Add bytes to PACKED_SIZE count
        const size_t PHASE_OFFSET_BYTES = 4;
        PACKED_SIZE += PHASE_OFFSET_BYTES;

        SECTION("Rule 9.5.8-2") {
            /*
             * Rule 9.5.8-2: The Phase field shall be expressed in units of
             * radians (rad). The Phase value shall be expressed in
             * two’s-complement format in the lower 16 bits of the Phase field
             * as shown in Figure 9.5.8-2. This field has an integer and a
             * fractional part, with the radix point to the right of bit 7 of
             * the field.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |                   Reserved                    |        Phase Offset (15..0) Radians           |
             *                                                                                   ^
             *                                                                                   |
             *                                                                              Radix Point
             */
            SECTION("Phase Offset Value 1") {
                packet_type packet_in;

                // Stream ID is required field. Set value to check
                const uint32_t STREAM_ID = 0x12345678;
                packet_in.stream_id(STREAM_ID);
                CHECK(packet_in.stream_id() == STREAM_ID);

                // Set phase offset value to check
                const double PHASE_OFFSET_1 = 1.0; // represents 0x0000 0080
                CHECK_FALSE(packet_in.has_phase_offset());
                packet_in.phase_offset(PHASE_OFFSET_1);
                CHECK(packet_in.has_phase_offset());
                CHECK(packet_in.phase_offset() == PHASE_OFFSET_1);

                // Check bytes required
                CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                // Get buffer from pack
                auto data = packet_helper::pack(packet_in);
                CHECK(data.size() == PACKED_SIZE);
                auto* check_ptr = data.data();

                // Examine and check packed header
                const size_t HEADER_BYTES = 4;
                const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
                const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
                const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
                const uint8_t CIF1_ENABLE = 0x1 << 1; // CIF 0 bit 1
                const bytes CIF0_BE{ 0, 0, 0, CIF1_ENABLE };
                const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
                check_ptr += CIF0_BYTES;
                CHECK(packed_cif0 == CIF0_BE);

                // Examine and check packed CIF1
                const size_t CIF1_BYTES = 4;
                const uint8_t PHASE_OFFSET_ENABLE = 0x1 << 7; // CIF 1 bit 31
                const bytes CIF1_BE{ PHASE_OFFSET_ENABLE, 0, 0, 0 };
                const decltype(data) packed_cif1(check_ptr, check_ptr + CIF1_BYTES);
                check_ptr += CIF1_BYTES;
                CHECK(packed_cif1 == CIF1_BE);

                // Examine and check packed Phase Offset. Value shall be in big-endian format.
                const bytes PHASE_OFFSET_BE{ 0, 0, 0, 0x80 };
                const decltype(data) packed_phase_offset(check_ptr, check_ptr + PHASE_OFFSET_BYTES);
                check_ptr += PHASE_OFFSET_BYTES;
                CHECK(packed_phase_offset == PHASE_OFFSET_BE);

                // Check match
                CHECK_FALSE(packet_helper::match(data.data(), data.size()));

                // Unpack verifed packed data
                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                // Examine and check unpacked packet header
                const auto& header = packet_out.header();
                CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
                CHECK_FALSE(header.class_id_enable());
                CHECK_FALSE(header.not_v49d0());
                CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
                CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
                CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
                CHECK(header.packet_size() == PACKET_SIZE);

                // Examine and check unpacked Stream ID
                CHECK(packet_out.stream_id() == STREAM_ID);

                // Examine and check unpacked Phase Offset
                CHECK(packet_out.has_phase_offset());
                CHECK(packet_out.phase_offset() == PHASE_OFFSET_1);
            } // end SECTION("Phase Offset Value 1")
        
            SECTION("Phase Offset Value 1") {
                packet_type packet_in;

                // Stream ID is required field. Set value to check
                const uint32_t STREAM_ID = 0x12345678;
                packet_in.stream_id(STREAM_ID);
                CHECK(packet_in.stream_id() == STREAM_ID);

                // Set phase offset value to check
                const double PHASE_OFFSET_1 = 1.0; // represents 0x0000 0080
                CHECK_FALSE(packet_in.has_phase_offset());
                packet_in.phase_offset(PHASE_OFFSET_1);
                CHECK(packet_in.has_phase_offset());
                CHECK(packet_in.phase_offset() == PHASE_OFFSET_1);

                // Check bytes required
                CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                // Get buffer from pack
                auto data = packet_helper::pack(packet_in);
                CHECK(data.size() == PACKED_SIZE);
                auto* check_ptr = data.data();

                // Examine and check packed header
                const size_t HEADER_BYTES = 4;
                const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
                const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
                const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
                const uint8_t CIF1_ENABLE = 0x1 << 1; // CIF 0 bit 1
                const bytes CIF0_BE{ 0, 0, 0, CIF1_ENABLE };
                const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
                check_ptr += CIF0_BYTES;
                CHECK(packed_cif0 == CIF0_BE);

                // Examine and check packed CIF1
                const size_t CIF1_BYTES = 4;
                const uint8_t PHASE_OFFSET_ENABLE = 0x1 << 7; // CIF 1 bit 31
                const bytes CIF1_BE{ PHASE_OFFSET_ENABLE, 0, 0, 0 };
                const decltype(data) packed_cif1(check_ptr, check_ptr + CIF1_BYTES);
                check_ptr += CIF1_BYTES;
                CHECK(packed_cif1 == CIF1_BE);

                // Examine and check packed Phase Offset. Value shall be in big-endian format.
                const bytes PHASE_OFFSET_BE{ 0, 0, 0, 0x80 };
                const decltype(data) packed_phase_offset(check_ptr, check_ptr + PHASE_OFFSET_BYTES);
                check_ptr += PHASE_OFFSET_BYTES;
                CHECK(packed_phase_offset == PHASE_OFFSET_BE);

                // Check match
                CHECK_FALSE(packet_helper::match(data.data(), data.size()));

                // Unpack verifed packed data
                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                // Examine and check unpacked packet header
                const auto& header = packet_out.header();
                CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
                CHECK_FALSE(header.class_id_enable());
                CHECK_FALSE(header.not_v49d0());
                CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
                CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
                CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
                CHECK(header.packet_size() == PACKET_SIZE);

                // Examine and check unpacked Stream ID
                CHECK(packet_out.stream_id() == STREAM_ID);

                // Examine and check unpacked Phase Offset
                CHECK(packet_out.has_phase_offset());
                CHECK(packet_out.phase_offset() == PHASE_OFFSET_1);
            } // end SECTION("Phase Offset Value 1")
        
            SECTION("Phase Offset Value 2") {
                packet_type packet_in;

                // Stream ID is required field. Set value to check
                const uint32_t STREAM_ID = 0x12345678;
                packet_in.stream_id(STREAM_ID);
                CHECK(packet_in.stream_id() == STREAM_ID);

                // Set phase offset value to check
                const double PHASE_OFFSET_2 = -1.0; // represents 0x0000 FF80
                CHECK_FALSE(packet_in.has_phase_offset());
                packet_in.phase_offset(PHASE_OFFSET_2);
                CHECK(packet_in.has_phase_offset());
                CHECK(packet_in.phase_offset() == PHASE_OFFSET_2);

                // Check bytes required
                CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                // Get buffer from pack
                auto data = packet_helper::pack(packet_in);
                CHECK(data.size() == PACKED_SIZE);
                auto* check_ptr = data.data();

                // Examine and check packed header
                const size_t HEADER_BYTES = 4;
                const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
                const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
                const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
                const uint8_t CIF1_ENABLE = 0x1 << 1; // CIF 0 bit 1
                const bytes CIF0_BE{ 0, 0, 0, CIF1_ENABLE };
                const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
                check_ptr += CIF0_BYTES;
                CHECK(packed_cif0 == CIF0_BE);

                // Examine and check packed CIF1
                const size_t CIF1_BYTES = 4;
                const uint8_t PHASE_OFFSET_ENABLE = 0x1 << 7; // CIF 1 bit 31
                const bytes CIF1_BE{ PHASE_OFFSET_ENABLE, 0, 0, 0 };
                const decltype(data) packed_cif1(check_ptr, check_ptr + CIF1_BYTES);
                check_ptr += CIF1_BYTES;
                CHECK(packed_cif1 == CIF1_BE);

                // Examine and check packed Phase Offset. Value shall be in big-endian format.
                const bytes PHASE_OFFSET_BE{ 0, 0, 0xFF, 0x80 };
                const decltype(data) packed_phase_offset(check_ptr, check_ptr + PHASE_OFFSET_BYTES);
                check_ptr += PHASE_OFFSET_BYTES;
                CHECK(packed_phase_offset == PHASE_OFFSET_BE);

                // Check match
                CHECK_FALSE(packet_helper::match(data.data(), data.size()));

                // Unpack verifed packed data
                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                // Examine and check unpacked packet header
                const auto& header = packet_out.header();
                CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
                CHECK_FALSE(header.class_id_enable());
                CHECK_FALSE(header.not_v49d0());
                CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
                CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
                CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
                CHECK(header.packet_size() == PACKET_SIZE);

                // Examine and check unpacked Stream ID
                CHECK(packet_out.stream_id() == STREAM_ID);

                // Examine and check unpacked Phase Offset
                CHECK(packet_out.has_phase_offset());
                CHECK(packet_out.phase_offset() == PHASE_OFFSET_2);
            } // end SECTION("Phase Offset Value 2")
        
            SECTION("Phase Offset Value 3") {
                packet_type packet_in;

                // Stream ID is required field. Set value to check
                const uint32_t STREAM_ID = 0x12345678;
                packet_in.stream_id(STREAM_ID);
                CHECK(packet_in.stream_id() == STREAM_ID);

                // Set phase offset value to check
                const double PHASE_OFFSET_3 = 0.0078125; // represents 0x0000 0001
                CHECK_FALSE(packet_in.has_phase_offset());
                packet_in.phase_offset(PHASE_OFFSET_3);
                CHECK(packet_in.has_phase_offset());
                CHECK(packet_in.phase_offset() == PHASE_OFFSET_3);

                // Check bytes required
                CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                // Get buffer from pack
                auto data = packet_helper::pack(packet_in);
                CHECK(data.size() == PACKED_SIZE);
                auto* check_ptr = data.data();

                // Examine and check packed header
                const size_t HEADER_BYTES = 4;
                const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
                const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
                const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
                const uint8_t CIF1_ENABLE = 0x1 << 1; // CIF 0 bit 1
                const bytes CIF0_BE{ 0, 0, 0, CIF1_ENABLE };
                const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
                check_ptr += CIF0_BYTES;
                CHECK(packed_cif0 == CIF0_BE);

                // Examine and check packed CIF1
                const size_t CIF1_BYTES = 4;
                const uint8_t PHASE_OFFSET_ENABLE = 0x1 << 7; // CIF 1 bit 31
                const bytes CIF1_BE{ PHASE_OFFSET_ENABLE, 0, 0, 0 };
                const decltype(data) packed_cif1(check_ptr, check_ptr + CIF1_BYTES);
                check_ptr += CIF1_BYTES;
                CHECK(packed_cif1 == CIF1_BE);

                // Examine and check packed Phase Offset. Value shall be in big-endian format.
                const bytes PHASE_OFFSET_BE{ 0, 0, 0, 0x01 };
                const decltype(data) packed_phase_offset(check_ptr, check_ptr + PHASE_OFFSET_BYTES);
                check_ptr += PHASE_OFFSET_BYTES;
                CHECK(packed_phase_offset == PHASE_OFFSET_BE);

                // Check match
                CHECK_FALSE(packet_helper::match(data.data(), data.size()));

                // Unpack verifed packed data
                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                // Examine and check unpacked packet header
                const auto& header = packet_out.header();
                CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
                CHECK_FALSE(header.class_id_enable());
                CHECK_FALSE(header.not_v49d0());
                CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
                CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
                CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
                CHECK(header.packet_size() == PACKET_SIZE);

                // Examine and check unpacked Stream ID
                CHECK(packet_out.stream_id() == STREAM_ID);

                // Examine and check unpacked Phase Offset
                CHECK(packet_out.has_phase_offset());
                CHECK(packet_out.phase_offset() == PHASE_OFFSET_3);
            } // end SECTION("Phase Offset Value 3")

            SECTION("Phase Offset Value 4") {
                packet_type packet_in;

                // Stream ID is required field. Set value to check
                const uint32_t STREAM_ID = 0x12345678;
                packet_in.stream_id(STREAM_ID);
                CHECK(packet_in.stream_id() == STREAM_ID);

                // Set phase offset value to check
                const double PHASE_OFFSET_4 = -0.0078125; // represents 0x0000 FFFF
                CHECK_FALSE(packet_in.has_phase_offset());
                packet_in.phase_offset(PHASE_OFFSET_4);
                CHECK(packet_in.has_phase_offset());
                CHECK(packet_in.phase_offset() == PHASE_OFFSET_4);

                // Check bytes required
                CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                // Get buffer from pack
                auto data = packet_helper::pack(packet_in);
                CHECK(data.size() == PACKED_SIZE);
                auto* check_ptr = data.data();

                // Examine and check packed header
                const size_t HEADER_BYTES = 4;
                const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
                const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
                const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
                const uint8_t CIF1_ENABLE = 0x1 << 1; // CIF 0 bit 1
                const bytes CIF0_BE{ 0, 0, 0, CIF1_ENABLE };
                const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
                check_ptr += CIF0_BYTES;
                CHECK(packed_cif0 == CIF0_BE);

                // Examine and check packed CIF1
                const size_t CIF1_BYTES = 4;
                const uint8_t PHASE_OFFSET_ENABLE = 0x1 << 7; // CIF 1 bit 31
                const bytes CIF1_BE{ PHASE_OFFSET_ENABLE, 0, 0, 0 };
                const decltype(data) packed_cif1(check_ptr, check_ptr + CIF1_BYTES);
                check_ptr += CIF1_BYTES;
                CHECK(packed_cif1 == CIF1_BE);

                // Examine and check packed Phase Offset. Value shall be in big-endian format.
                const bytes PHASE_OFFSET_BE{ 0, 0, 0xFF, 0xFF };
                const decltype(data) packed_phase_offset(check_ptr, check_ptr + PHASE_OFFSET_BYTES);
                check_ptr += PHASE_OFFSET_BYTES;
                CHECK(packed_phase_offset == PHASE_OFFSET_BE);

                // Check match
                CHECK_FALSE(packet_helper::match(data.data(), data.size()));

                // Unpack verifed packed data
                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                // Examine and check unpacked packet header
                const auto& header = packet_out.header();
                CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
                CHECK_FALSE(header.class_id_enable());
                CHECK_FALSE(header.not_v49d0());
                CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
                CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
                CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
                CHECK(header.packet_size() == PACKET_SIZE);

                // Examine and check unpacked Stream ID
                CHECK(packet_out.stream_id() == STREAM_ID);

                // Examine and check unpacked Phase Offset
                CHECK(packet_out.has_phase_offset());
                CHECK(packet_out.phase_offset() == PHASE_OFFSET_4);
            } // end SECTION("Phase Offset Value 4")
        } // end SECTION("Rule 9.5.8-1")
    } // end SECTION("Phase Offset")

    SECTION("Polarization") {
        // Add bytes to PACKED_SIZE count
        const size_t POLARIZATION_BYTES = 4;
        PACKED_SIZE += POLARIZATION_BYTES;

        SECTION("Rule 9.4.8-1") {
            /*
             * Rule 9.4.8-1: The Polarization field shall contain two subfields,
             * Tilt Angle and Ellipticity Angle formatted in the 32 bit word as
             * shown in Figure 9.4.8-2.
             * 
             * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
             * | 1    |         Tilt Angle (31..16), Radians          |        Ellipticity Angle (15..0), Radians     |
             *                 ^                                               ^
             *                 |                                               |
             *            Radix Point                                     Radix Point
             */
        } // end SECTION("Rule 9.4.8-1")
    } // end SECTION("Polarization")
} // end TEST_CASE("Context Packet CIF1 Optional")

TEST_CASE("Context Packet Context Association Lists")
{
    // Sizes for all required fields
    size_t PACKED_SIZE = 4 + // header
                         4 + // stream_id
                         4; // cif 0

    SECTION("Context Association Lists") {
        using packet_type = TestContextAssociationLists;
        using packet_helper = packet_type::helper;
        
        // Add required bytes to PACKED_SIZE count
        size_t CONTEXT_ASSOCIATION_LISTS_HEADER_BYTES = 8;
        PACKED_SIZE += CONTEXT_ASSOCIATION_LISTS_HEADER_BYTES;

        /*
         * Rule 9.13.2-1: The organization of the Context Association Lists
         * Section shall be as shown in Figure 9.13.2-1.
         * 
         * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
         * | 1    |      Reserved      |     Source List Size     |      Reserved      |     System List Size     |
         * | 2    |          Vector-Component List Size           | A|       Asynchronous-Channel List Size       |
         * | ...  |                                 Source Context Association List                               |
         * | ...  |                                 System Context Association List                               |
         * | ...  |                       Vector-Component Context Association List                               |
         * | ...  |                   Asynchronous-Channel Context Association List                               |
         * | ...  |               Asynchronous-Channel Tag Context Association List (optional)                    |
         */
        packet_type packet_in;

        // Stream ID is required field. Set value to check
        const uint32_t STREAM_ID = 0x12345678;
        packet_in.stream_id(STREAM_ID);
        CHECK(packet_in.stream_id() == STREAM_ID);

        // Add index list entries to check
        vrtgen::packing::ContextAssociationLists context_association_lists;
        const uint32_t NUM_ASSOCIATIONS = 3;
        for (uint32_t i=1; i<=NUM_ASSOCIATIONS; ++i) {
            context_association_lists.source_list().push_back(i);
            context_association_lists.system_list().push_back(i);
            context_association_lists.vector_component_list().push_back(i);
            context_association_lists.async_channel_list().push_back(i);
        }
        packet_in.context_association_lists(context_association_lists);
        const uint32_t LIST_WORDS = NUM_ASSOCIATIONS * 4; // 4 lists included
        PACKED_SIZE += LIST_WORDS * 4;

        // Check bytes required
        CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        // Get buffer from pack
        auto data = packet_helper::pack(packet_in);
        CHECK(data.size() == PACKED_SIZE);
        auto* check_ptr = data.data();

        // Examine and check packed header
        const size_t HEADER_BYTES = 4;
        const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
        const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
        const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
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
        const uint8_t CONTEXT_ASSOCIATION_LISTS_ENABLE = 0x1; // CIF 0 bit 8
        const bytes CIF0_BE{ 0, 0, CONTEXT_ASSOCIATION_LISTS_ENABLE, 0 };
        const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
        check_ptr += CIF0_BYTES;
        CHECK(packed_cif0 == CIF0_BE);

        // Examine and check packed Context Association Lists header fields. Values shall be in big-endian format.
        const bytes CONTEXT_ASSOCIATION_LISTS_HEADER_BE{
            0, NUM_ASSOCIATIONS, 0, NUM_ASSOCIATIONS,
            0, NUM_ASSOCIATIONS, 0, NUM_ASSOCIATIONS
        };
        const decltype(data) packed_context_association_lists_header(check_ptr, check_ptr + CONTEXT_ASSOCIATION_LISTS_HEADER_BYTES);
        check_ptr += CONTEXT_ASSOCIATION_LISTS_HEADER_BYTES;
        CHECK(packed_context_association_lists_header == CONTEXT_ASSOCIATION_LISTS_HEADER_BE);

        // Examine and check packed  Context Association Lists list fields.
        const bytes CONTEXT_ASSOCIATION_LISTS_LISTS_BE{
            // source 
            1, 0, 0, 0,
            2, 0, 0, 0,
            3, 0, 0, 0,
            // system 
            1, 0, 0, 0,
            2, 0, 0, 0,
            3, 0, 0, 0,
            // vector component 
            1, 0, 0, 0,
            2, 0, 0, 0,
            3, 0, 0, 0,
            // async channel 
            1, 0, 0, 0,
            2, 0, 0, 0,
            3, 0, 0, 0,
        };
        const decltype(data) packed_context_association_lists_lists(check_ptr, check_ptr + LIST_WORDS * 4);
        check_ptr += LIST_WORDS * 4;
        CHECK(packed_context_association_lists_lists == CONTEXT_ASSOCIATION_LISTS_LISTS_BE);

        // Check match
        CHECK_FALSE(packet_helper::match(data.data(), data.size()));

        // Unpack verifed packed data
        packet_type packet_out;
        packet_helper::unpack(packet_out, data.data(), data.size());

        // Examine and check unpacked packet header
        const auto& header = packet_out.header();
        CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
        CHECK_FALSE(header.class_id_enable());
        CHECK_FALSE(header.not_v49d0());
        CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
        CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
        CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
        CHECK(header.packet_size() == PACKET_SIZE);

        // Examine and check unpacked Stream ID
        CHECK(packet_out.stream_id() == STREAM_ID);

        // Examine and check unpacked Context Association Lists
        CHECK(packet_out.context_association_lists().source_list_size() == NUM_ASSOCIATIONS);
        CHECK(packet_out.context_association_lists().system_list_size() == NUM_ASSOCIATIONS);
        CHECK(packet_out.context_association_lists().vector_component_list_size() == NUM_ASSOCIATIONS);
        CHECK_FALSE(packet_out.context_association_lists().async_channel_tag_list_enable());
        CHECK(packet_out.context_association_lists().async_channel_list_size() == NUM_ASSOCIATIONS);
        auto& source_list = packet_out.context_association_lists().source_list();
        CHECK(source_list.size() == NUM_ASSOCIATIONS);
        for (uint32_t i=0; i<NUM_ASSOCIATIONS; ++i) {
            CHECK(source_list[i] == (i+1));
        }
        auto& system_list = packet_out.context_association_lists().system_list();
        CHECK(system_list.size() == NUM_ASSOCIATIONS);
        for (uint32_t i=0; i<NUM_ASSOCIATIONS; ++i) {
            CHECK(system_list[i] == (i+1));
        }
        auto& vector_component_list = packet_out.context_association_lists().vector_component_list();
        CHECK(vector_component_list.size() == NUM_ASSOCIATIONS);
        for (uint32_t i=0; i<NUM_ASSOCIATIONS; ++i) {
            CHECK(vector_component_list[i] == (i+1));
        }
        auto& async_channel_list = packet_out.context_association_lists().async_channel_list();
        CHECK(async_channel_list.size() == NUM_ASSOCIATIONS);
        for (uint32_t i=0; i<NUM_ASSOCIATIONS; ++i) {
            CHECK(async_channel_list[i] == (i+1));
        }
    } // end SECTION("Context Association Lists")
} // end TEST_CASE("Context Packet Context Association Lists")


TEST_CASE("Context Packet CIF7 Packet")
{
    using packet_type = TestContext6;
    using helper = packet_type::helper;

    // Sizes for all required fields
    size_t PACKED_SIZE = 4 + // header
                         4 + // stream_id
                         4 + // cif 0
                         4; // cif 7

    packet_type packet_in;

    // Stream ID is required field. Set value to check
    const uint32_t STREAM_ID = 0x12345678;
    packet_in.stream_id(STREAM_ID);
    CHECK(packet_in.stream_id() == STREAM_ID);

    uint8_t PROBABILITY_PERCENT = 0xAF;
    uint8_t PROBABILITY_FUNC = 0x01;
    uint8_t BELIEF_PERCENT = 0xFF;

    // Check bytes required
    CHECK(helper::bytes_required(packet_in) == PACKED_SIZE);
    bytes PROB_BE   = bytes { 0, 0, PROBABILITY_FUNC, PROBABILITY_PERCENT };
    bytes BELIEF_BE = bytes { 0, 0, 0, BELIEF_PERCENT };

    vrtgen::packing::Probability probability;
    vrtgen::packing::Belief belief;
    size_t PROBABILITY_BYTES = 4;
    size_t BELIEF_BYTES = 4;
    probability.probability_percent(PROBABILITY_PERCENT);
    probability.probability_function(PROBABILITY_FUNC);
    belief.belief_percent(BELIEF_PERCENT);

    SECTION("CIF0") {
        SECTION("Reference Level - common type test") {
            const size_t REF_LEVEL_BYTES = 4;
            PACKED_SIZE += REF_LEVEL_BYTES;

            // Add bytes to PACKED_SIZE count
            float REF_LEVEL = 1.0; // represents 0x0000 0080
            float REF_LEVEL_MEAN = 3.0; // represents 0x0000 0180
            float REF_LEVEL_MIN = 1.0;
            float REF_LEVEL_MAX = 4.0; // represents 0x0000 0000
            bytes REF_LEVEL_BE        = bytes { 0, 0, 0, 0x80 };
            bytes REF_LEVEL_MEAN_BE   = bytes { 0, 0, 0x01, 0x80 };
            bytes REF_LEVEL_MAX_BE    = bytes { 0, 0, 0x02, 0 };
            bytes REF_LEVEL_MIN_BE    = bytes { 0, 0, 0, 0x80 };

            CHECK_FALSE(packet_in.has_reference_level());
            packet_in.reference_level(REF_LEVEL);
            CHECK(packet_in.reference_level());
            CHECK(packet_in.reference_level() == REF_LEVEL);

            test_context6::structs::CIF7Attributes<float> attributes;
            attributes.mean_value(REF_LEVEL_MEAN);
            PACKED_SIZE += REF_LEVEL_BYTES;
            attributes.max_value(REF_LEVEL_MAX);
            PACKED_SIZE += REF_LEVEL_BYTES;
            attributes.min_value(REF_LEVEL_MIN);
            PACKED_SIZE += REF_LEVEL_BYTES;
            
            attributes.probability(probability);
            attributes.belief(belief);
            packet_in.reference_level_attributes(attributes);
            PACKED_SIZE += PROBABILITY_BYTES;
            PACKED_SIZE += BELIEF_BYTES;
            CHECK(packet_in.reference_level_attributes().probability().probability_percent() == int(PROBABILITY_PERCENT));
            CHECK(packet_in.reference_level_attributes().belief().belief_percent() == int(BELIEF_PERCENT));
            CHECK(packet_in.reference_level_attributes().mean_value() == REF_LEVEL_MEAN);
            CHECK(packet_in.reference_level_attributes().max_value() == REF_LEVEL_MAX);
            CHECK(packet_in.reference_level_attributes().min_value() == REF_LEVEL_MIN);

            // Check bytes required
            CHECK(helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
            const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
            check_ptr += HEADER_BYTES;
            //CHECK(packed_header == HEADER_BE);

            // Examine and check packed Stream ID. Value shall be in big-endian format.
            const size_t STREAM_ID_BYTES = 4;
            const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
            const decltype(data) packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
            check_ptr += STREAM_ID_BYTES;
            CHECK(packed_stream_id == STREAM_ID_BE);

            // Examine and check packed CIF0
            const size_t CIF0_BYTES = 4;
            const uint8_t REF_LEVEL_ENABLE = 0x1; // CIF 0 bit 24
            const bytes CIF0_BE{ REF_LEVEL_ENABLE, 0, 0, 0x1 << 7 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed CIF0
            const size_t CIF7_BYTES = 4;
            const uint8_t MEAN_ENABLE = 1 << 6; // CIF 7 bit 30
            const uint8_t MIN_ENABLE = 1 << 2; // CIF 7 bit 26
            const uint8_t MAX_ENABLE = 1 << 3; // CIF 7 bit 27
            const uint8_t BELIEF_ENABLE = 1 << 3; // CIF 7 bit 19
            const uint8_t PROBABILITY_ENABLE = 1 << 4; // CIF 7 bit 20
            const bytes CIF7_BE{ MEAN_ENABLE | MIN_ENABLE | MAX_ENABLE, BELIEF_ENABLE | PROBABILITY_ENABLE, 0 , 0 };
            const decltype(data) packed_cif7(check_ptr, check_ptr + CIF7_BYTES);
            check_ptr += CIF7_BYTES;
            CHECK(packed_cif7 == CIF7_BE);

            // // Examine and check packed Reference Level. Value shall be in big-endian format.
            const decltype(data) packed_reference_level(check_ptr, check_ptr + REF_LEVEL_BYTES);
            check_ptr += REF_LEVEL_BYTES;
            CHECK(packed_reference_level == REF_LEVEL_BE);
            const decltype(data) packed_ref_mean(check_ptr, check_ptr + REF_LEVEL_BYTES);
            check_ptr += REF_LEVEL_BYTES;
            CHECK(packed_ref_mean == REF_LEVEL_MEAN_BE);
            const decltype(data) packed_ref_max(check_ptr, check_ptr + REF_LEVEL_BYTES);
            check_ptr += REF_LEVEL_BYTES;
            CHECK(packed_ref_max == REF_LEVEL_MAX_BE);
            const decltype(data) packed_ref_min(check_ptr, check_ptr + REF_LEVEL_BYTES);
            check_ptr += REF_LEVEL_BYTES;
            CHECK(packed_ref_min == REF_LEVEL_MIN_BE);
            const decltype(data) packed_ref_probability(check_ptr, check_ptr + REF_LEVEL_BYTES);
            check_ptr += REF_LEVEL_BYTES;
            CHECK(packed_ref_probability == PROB_BE);
            const decltype(data) packed_ref_belief(check_ptr, check_ptr + REF_LEVEL_BYTES);
            check_ptr += REF_LEVEL_BYTES;
            CHECK(packed_ref_belief == BELIEF_BE);

            // Check match
            CHECK_FALSE(helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked Reference Level
            CHECK(packet_out.has_reference_level());
            CHECK(packet_out.reference_level() == REF_LEVEL);
        }

        SECTION("Gain - class type test") {
        // Add bytes to PACKED_SIZE count
        const size_t GAIN_BYTES = 4;
        PACKED_SIZE += GAIN_BYTES;

        float GAIN_STAGE_2 = 0;
        float GAIN_STAGE_1 = 1.0; // represents 0x0000 0080
        bytes GAIN_BE = bytes { 0, 0, 0, 0x80 };
        bytes GAIN_MEAN_BE = bytes { 0, 0, 0, 0x80 };
        bytes GAIN_MAX_BE = bytes { 0, 0, 0, 0x80 };
        bytes GAIN_MIN_BE = bytes { 0, 0, 0, 0x80 };

        vrtgen::packing::Gain gain;
        gain.stage_1(GAIN_STAGE_1);
        gain.stage_2(GAIN_STAGE_2);
        CHECK_FALSE(packet_in.has_gain());
        packet_in.gain(gain);
        CHECK(packet_in.has_gain());
        CHECK(packet_in.gain().stage_1() == GAIN_STAGE_1);
        CHECK(packet_in.gain().stage_2() == GAIN_STAGE_2);

        test_context6::structs::CIF7Attributes<vrtgen::packing::Gain> attributes;
        attributes.mean_value(gain);
        PACKED_SIZE += GAIN_BYTES;
        attributes.max_value(gain);
        PACKED_SIZE += GAIN_BYTES;
        attributes.min_value(gain);
        PACKED_SIZE += GAIN_BYTES;
        
        attributes.probability(probability);
        attributes.belief(belief);
        packet_in.gain_attributes(attributes);
        PACKED_SIZE += PROBABILITY_BYTES;
        PACKED_SIZE += BELIEF_BYTES;
        CHECK(packet_in.gain_attributes().probability().probability_percent() == int(PROBABILITY_PERCENT));
        CHECK(packet_in.gain_attributes().belief().belief_percent() == int(BELIEF_PERCENT));
        CHECK(packet_in.gain_attributes().mean_value().stage_1() == GAIN_STAGE_1);
        CHECK(packet_in.gain_attributes().max_value().stage_1() == GAIN_STAGE_1);
        CHECK(packet_in.gain_attributes().min_value().stage_1() == GAIN_STAGE_1);
        CHECK(packet_in.gain_attributes().mean_value().stage_2() == GAIN_STAGE_2);
        CHECK(packet_in.gain_attributes().max_value().stage_2() == GAIN_STAGE_2);
        CHECK(packet_in.gain_attributes().min_value().stage_2() == GAIN_STAGE_2);

        // Check bytes required
        CHECK(helper::bytes_required(packet_in) == PACKED_SIZE);

        // Get buffer from pack
        auto data = helper::pack(packet_in);
        CHECK(data.size() == PACKED_SIZE);
        auto* check_ptr = data.data();

        // Examine and check packed header
        const size_t HEADER_BYTES = 4;
        const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
        const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
        const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
        const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
        check_ptr += HEADER_BYTES;
        //CHECK(packed_header == HEADER_BE);

        // Examine and check packed Stream ID. Value shall be in big-endian format.
        const size_t STREAM_ID_BYTES = 4;
        const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
        const decltype(data) packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
        check_ptr += STREAM_ID_BYTES;
        CHECK(packed_stream_id == STREAM_ID_BE);

        // Examine and check packed CIF0
        const size_t CIF0_BYTES = 4;
        const uint8_t GAIN_ENABLE = 0x80; // CIF 0 bit 23
        const bytes CIF0_BE{ 0, GAIN_ENABLE, 0, 0x1 << 7 };
        const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
        check_ptr += CIF0_BYTES;
        CHECK(packed_cif0 == CIF0_BE);

        // Examine and check packed CIF0
        const size_t CIF7_BYTES = 4;
        const uint8_t MEAN_ENABLE = 1 << 6; // CIF 7 bit 30
        const uint8_t MIN_ENABLE = 1 << 2; // CIF 7 bit 26
        const uint8_t MAX_ENABLE = 1 << 3; // CIF 7 bit 27
        const uint8_t BELIEF_ENABLE = 1 << 3; // CIF 7 bit 19
        const uint8_t PROBABILITY_ENABLE = 1 << 4; // CIF 7 bit 20
        const bytes CIF7_BE{ MEAN_ENABLE | MIN_ENABLE | MAX_ENABLE, BELIEF_ENABLE | PROBABILITY_ENABLE, 0 , 0 };
        const decltype(data) packed_cif7(check_ptr, check_ptr + CIF7_BYTES);
        check_ptr += CIF7_BYTES;
        CHECK(packed_cif7 == CIF7_BE);

        // // Examine and check packed Reference Level. Value shall be in big-endian format.
        const decltype(data) packed_reference_level(check_ptr, check_ptr + GAIN_BYTES);
        check_ptr += GAIN_BYTES;
        CHECK(packed_reference_level == GAIN_BE);
        const decltype(data) packed_ref_mean(check_ptr, check_ptr + GAIN_BYTES);
        check_ptr += GAIN_BYTES;
        CHECK(packed_ref_mean == GAIN_MEAN_BE);
        const decltype(data) packed_ref_max(check_ptr, check_ptr + GAIN_BYTES);
        check_ptr += GAIN_BYTES;
        CHECK(packed_ref_max == GAIN_MAX_BE);
        const decltype(data) packed_ref_min(check_ptr, check_ptr + GAIN_BYTES);
        check_ptr += GAIN_BYTES;
        CHECK(packed_ref_min == GAIN_MIN_BE);
        const decltype(data) packed_ref_probability(check_ptr, check_ptr + GAIN_BYTES);
        check_ptr += GAIN_BYTES;
        CHECK(packed_ref_probability == PROB_BE);
        const decltype(data) packed_ref_belief(check_ptr, check_ptr + GAIN_BYTES);
        check_ptr += GAIN_BYTES;
        CHECK(packed_ref_belief == BELIEF_BE);

        // Check match
        CHECK_FALSE(helper::match(data.data(), data.size()));

        // Unpack verifed packed data
        packet_type packet_out;
        helper::unpack(packet_out, data.data(), data.size());

        // Examine and check unpacked packet header
        const auto& header = packet_out.header();
        CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
        CHECK_FALSE(header.class_id_enable());
        CHECK_FALSE(header.not_v49d0());
        CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
        CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
        CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
        CHECK(header.packet_size() == PACKET_SIZE);

        // Examine and check unpacked Stream ID
        CHECK(packet_out.stream_id() == STREAM_ID);

        // Examine and check unpacked Gain
        CHECK(packet_out.gain().stage_1() == GAIN_STAGE_1);
        CHECK(packet_out.gain().stage_2() == GAIN_STAGE_2);
    }
    }

    SECTION("CIF1") {
        size_t CIF1_BYTES = 4;
        PACKED_SIZE += CIF1_BYTES;

        SECTION("Phase Offset") {
            // Add bytes to PACKED_SIZE count
            const size_t PHASE_OFFSET_BYTES = 4;
            PACKED_SIZE += PHASE_OFFSET_BYTES;

            // Set phase offset value to check
            const double PHASE_OFFSET = 1.0; // represents 0x0000 0080
            CHECK_FALSE(packet_in.has_phase_offset());
            packet_in.phase_offset(PHASE_OFFSET);
            CHECK(packet_in.has_phase_offset());
            CHECK(packet_in.phase_offset() == PHASE_OFFSET);

            // Add bytes to PACKED_SIZE count
            double PHASE_OFFSET_MEAN = 3.0; // represents 0x0000 0180
            double PHASE_OFFSET_MIN = 1.0;
            double PHASE_OFFSET_MAX = 4.0; // represents 0x0000 0000
            bytes PHASE_OFFSET_BE        = bytes { 0, 0, 0, 0x80 };
            bytes PHASE_OFFSET_MEAN_BE   = bytes { 0, 0, 0x01, 0x80 };
            bytes PHASE_OFFSET_MAX_BE    = bytes { 0, 0, 0x02, 0 };
            bytes PHASE_OFFSET_MIN_BE    = bytes { 0, 0, 0, 0x80 };

            test_context6::structs::CIF7Attributes<float> attributes;
            attributes.mean_value(PHASE_OFFSET_MEAN);
            PACKED_SIZE += PHASE_OFFSET_BYTES;
            attributes.max_value(PHASE_OFFSET_MAX);
            PACKED_SIZE += PHASE_OFFSET_BYTES;
            attributes.min_value(PHASE_OFFSET_MIN);
            PACKED_SIZE += PHASE_OFFSET_BYTES;
            
            attributes.probability(probability);
            attributes.belief(belief);
            packet_in.phase_offset_attributes(attributes);
            PACKED_SIZE += PROBABILITY_BYTES;
            PACKED_SIZE += BELIEF_BYTES;
            CHECK(packet_in.phase_offset_attributes().probability().probability_percent() == int(PROBABILITY_PERCENT));
            CHECK(packet_in.phase_offset_attributes().belief().belief_percent() == int(BELIEF_PERCENT));
            CHECK(packet_in.phase_offset_attributes().mean_value() == PHASE_OFFSET_MEAN);
            CHECK(packet_in.phase_offset_attributes().max_value() == PHASE_OFFSET_MAX);
            CHECK(packet_in.phase_offset_attributes().min_value() == PHASE_OFFSET_MIN);

            // Check bytes required
            CHECK(helper::bytes_required(packet_in) == PACKED_SIZE);

            // Get buffer from pack
            auto data = helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
            const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
            check_ptr += HEADER_BYTES;
            //CHECK(packed_header == HEADER_BE);

            // Examine and check packed Stream ID. Value shall be in big-endian format.
            const size_t STREAM_ID_BYTES = 4;
            const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
            const decltype(data) packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
            check_ptr += STREAM_ID_BYTES;
            CHECK(packed_stream_id == STREAM_ID_BE);

            // Examine and check packed CIF0
            const size_t CIF0_BYTES = 4;
            const bytes CIF0_BE{ 0, 0, 0, 0x1 << 7 | 0x1 << 1 };
            const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
            check_ptr += CIF0_BYTES;
            CHECK(packed_cif0 == CIF0_BE);

            // Examine and check packed CIF1
            const size_t CIF1_BYTES = 4;
            const uint8_t PHASE_OFFSET_ENABLE = 0x1 << 7; // CIF 1 bit 31
            const bytes CIF1_BE{ PHASE_OFFSET_ENABLE, 0, 0, 0 };
            const decltype(data) packed_cif1(check_ptr, check_ptr + CIF1_BYTES);
            check_ptr += CIF1_BYTES;
            CHECK(packed_cif1 == CIF1_BE);

            // Examine and check packed CIF0
            const size_t CIF7_BYTES = 4;
            const uint8_t MEAN_ENABLE = 1 << 6; // CIF 7 bit 30
            const uint8_t MIN_ENABLE = 1 << 2; // CIF 7 bit 26
            const uint8_t MAX_ENABLE = 1 << 3; // CIF 7 bit 27
            const uint8_t BELIEF_ENABLE = 1 << 3; // CIF 7 bit 19
            const uint8_t PROBABILITY_ENABLE = 1 << 4; // CIF 7 bit 20
            const bytes CIF7_BE{ MEAN_ENABLE | MIN_ENABLE | MAX_ENABLE, BELIEF_ENABLE | PROBABILITY_ENABLE, 0 , 0 };
            const decltype(data) packed_cif7(check_ptr, check_ptr + CIF7_BYTES);
            check_ptr += CIF7_BYTES;
            CHECK(packed_cif7 == CIF7_BE);

            // // Examine and check packed Reference Level. Value shall be in big-endian format.
            const decltype(data) packed_reference_level(check_ptr, check_ptr + PHASE_OFFSET_BYTES);
            check_ptr += PHASE_OFFSET_BYTES;
            CHECK(packed_reference_level == PHASE_OFFSET_BE);
            const decltype(data) packed_ref_mean(check_ptr, check_ptr + PHASE_OFFSET_BYTES);
            check_ptr += PHASE_OFFSET_BYTES;
            CHECK(packed_ref_mean == PHASE_OFFSET_MEAN_BE);
            const decltype(data) packed_ref_max(check_ptr, check_ptr + PHASE_OFFSET_BYTES);
            check_ptr += PHASE_OFFSET_BYTES;
            CHECK(packed_ref_max == PHASE_OFFSET_MAX_BE);
            const decltype(data) packed_ref_min(check_ptr, check_ptr + PHASE_OFFSET_BYTES);
            check_ptr += PHASE_OFFSET_BYTES;
            CHECK(packed_ref_min == PHASE_OFFSET_MIN_BE);
            const decltype(data) packed_ref_probability(check_ptr, check_ptr + PHASE_OFFSET_BYTES);
            check_ptr += PHASE_OFFSET_BYTES;
            CHECK(packed_ref_probability == PROB_BE);
            const decltype(data) packed_ref_belief(check_ptr, check_ptr + PHASE_OFFSET_BYTES);
            check_ptr += PHASE_OFFSET_BYTES;
            CHECK(packed_ref_belief == BELIEF_BE);

            // Check match
            CHECK_FALSE(helper::match(data.data(), data.size()));

            // Unpack verifed packed data
            packet_type packet_out;
            helper::unpack(packet_out, data.data(), data.size());

            // Examine and check unpacked packet header
            const auto& header = packet_out.header();
            CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
            CHECK_FALSE(header.class_id_enable());
            CHECK_FALSE(header.not_v49d0());
            CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
            CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
            CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
            CHECK(header.packet_size() == PACKET_SIZE);

            // Examine and check unpacked Stream ID
            CHECK(packet_out.stream_id() == STREAM_ID);

            // Examine and check unpacked Reference Level
            CHECK(packet_out.phase_offset());
            CHECK(packet_out.phase_offset() == PHASE_OFFSET);
        }
    }
}


// TEST_CASE("Context Packet Index List")
// {
//     // Sizes for all required fields
//     size_t PACKED_SIZE = 4 + // header
//                          4 + // stream_id
//                          4 + // cif 0
//                          4;  // cif 1

//     SECTION("Index List 32-bit Entry Size") {
//         using packet_type = TestContextIndexList;
//         using packet_helper = packet_type::helper;
        
//         // Add required bytes to PACKED_SIZE count
//         size_t INDEX_LIST_HEADER_BYTES = 8;
//         PACKED_SIZE += INDEX_LIST_HEADER_BYTES;

//         /*
//          * Rule 9.3.2-1: The form of the Index List field shall follow that
//          * shown in Figure 9.3.2-1.
//          * 
//          * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
//          * | 1    |                           Total size of field in words (31..0)                                |
//          * | 2    |Entry Size |   Reserved (27..20)   |                   # of entries (19..0)                    |
//          * | 3..N |                   Index Entries (32-bit OR 16-bit packed or 8-bit packed)                     |
//          */
//         packet_type packet_in;

//         // Stream ID is required field. Set value to check
//         const uint32_t STREAM_ID = 0x12345678;
//         packet_in.stream_id(STREAM_ID);
//         CHECK(packet_in.stream_id() == STREAM_ID);

//         // Add index list entries to check
//         vrtgen::packing::IndexList<uint32_t> index_list;
//         const uint32_t NUM_ENTRIES = 10;
//         for (uint32_t i=1; i<=NUM_ENTRIES; ++i) {
//             index_list.entries().push_back(i);
//         }
//         packet_in.index_list(index_list);
//         const uint32_t ENTRY_WORDS = NUM_ENTRIES;
//         PACKED_SIZE += ENTRY_WORDS * 4;

//         // Check bytes required
//         CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

//         // Get buffer from pack
//         auto data = packet_helper::pack(packet_in);
//         CHECK(data.size() == PACKED_SIZE);
//         auto* check_ptr = data.data();

//         // Examine and check packed header
//         const size_t HEADER_BYTES = 4;
//         const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
//         const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
//         const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
//         const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
//         check_ptr += HEADER_BYTES;
//         CHECK(packed_header == HEADER_BE);

//         // Examine and check packed Stream ID. Value shall be in big-endian format.
//         const size_t STREAM_ID_BYTES = 4;
//         const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
//         const decltype(data) packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
//         check_ptr += STREAM_ID_BYTES;
//         CHECK(packed_stream_id == STREAM_ID_BE);

//         // Examine and check packed CIF0
//         const size_t CIF0_BYTES = 4;
//         const uint8_t CIF1_ENABLE = 0x1 << 1; // CIF 0 bit 1
//         const bytes CIF0_BE{ 0, 0, 0, CIF1_ENABLE };
//         const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
//         check_ptr += CIF0_BYTES;
//         CHECK(packed_cif0 == CIF0_BE);

//         // Examine and check packed CIF1
//         const size_t CIF1_BYTES = 4;
//         const uint8_t INDEX_LIST_ENABLE = 0x1 << 7; // CIF 1 bit 7
//         const bytes CIF1_BE{ 0, 0, 0, INDEX_LIST_ENABLE };
//         const decltype(data) packed_cif1(check_ptr, check_ptr + CIF1_BYTES);
//         check_ptr += CIF1_BYTES;
//         CHECK(packed_cif1 == CIF1_BE);

//         // Examine and check packed Index List header fields. Values shall be in big-endian format.
//         const bytes INDEX_LIST_HEADER_BE{ 0, 0, 0, 12, 0x40, 0, 0, 0xA };
//         const decltype(data) packed_index_list_header(check_ptr, check_ptr + INDEX_LIST_HEADER_BYTES);
//         check_ptr += INDEX_LIST_HEADER_BYTES;
//         CHECK(packed_index_list_header == INDEX_LIST_HEADER_BE);

//         // Examine and check packed Index List entry fields.
//         const bytes INDEX_LIST_ENTRIES_BE{ 
//             1, 0, 0, 0,
//             2, 0, 0, 0,
//             3, 0, 0, 0,
//             4, 0, 0, 0,
//             5, 0, 0, 0,
//             6, 0, 0, 0,
//             7, 0, 0, 0,
//             8, 0, 0, 0,
//             9, 0, 0, 0,
//             10, 0, 0, 0
//         };
//         const decltype(data) packed_index_list_entries(check_ptr, check_ptr + ENTRY_WORDS * 4);
//         check_ptr += ENTRY_WORDS * 4;
//         CHECK(packed_index_list_entries == INDEX_LIST_ENTRIES_BE);

//         // Check match
//         CHECK_FALSE(packet_helper::match(data.data(), data.size()));

//         // Unpack verifed packed data
//         packet_type packet_out;
//         packet_helper::unpack(packet_out, data.data(), data.size());

//         // Examine and check unpacked packet header
//         const auto& header = packet_out.header();
//         CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
//         CHECK_FALSE(header.class_id_enable());
//         CHECK_FALSE(header.not_v49d0());
//         CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
//         CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
//         CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
//         CHECK(header.packet_size() == PACKET_SIZE);

//         // Examine and check unpacked Stream ID
//         CHECK(packet_out.stream_id() == STREAM_ID);

//         // Examine and check unpacked Index List
//         CHECK(packet_out.index_list().total_size() == 12);
//         CHECK(packet_out.index_list().entry_size() == vrtgen::packing::EntrySize::THIRTY_TWO_BIT);
//         CHECK(packet_out.index_list().num_entries() == NUM_ENTRIES);
//         auto& entries = packet_out.index_list().entries();
//         CHECK(entries.size() == NUM_ENTRIES);
//         for (uint32_t i=0; i<NUM_ENTRIES; ++i) {
//             CHECK(entries[i] == (i+1));
//         }
//     } // end SECTION("Index List 32-bit Entry Size")
// } // end TEST_CASE("Context Packet IndexList")

// TEST_CASE("Context Packet Sector Step/Scan")
// {
//     // Sizes for all required fields
//     size_t PACKED_SIZE = 4 + // header
//                          4 + // stream_id
//                          4 + // cif 0
//                          4;  // cif 1

//     using packet_type = TestContextSectorStepScan;
//     using packet_helper = packet_type::helper;
//     using record_type = test_context_sector_step_scan::structs::SectorStepScanRecord;
    
//     // Add required bytes to PACKED_SIZE count
//     size_t SECTOR_STEP_SCAN_HEADER_BYTES = 12;
//     PACKED_SIZE += SECTOR_STEP_SCAN_HEADER_BYTES;

//     /*
//      * Rule 9.6.2.1-1: The internal structure of the Sector/Step-Scan field
//      * shall be as shown in Figure 9.6.2.1-1 where each Record in the field are
//      * identical.
//      * 
//      * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
//      * | 1    |                                      SizeofArray (31..0)                                      |
//      * | 2    |  HeaderSize (31..24)  |    NumWords/Record (23..12)       |        NumRecords (11..0)         |
//      * | 3    |                    Bitmapped Control/Context Indicator subfield (31..0)                       |
//      * | 4..N |              Records #1-N with set of subfields as described by above subfield                |
//      */
//     packet_type packet_in;

//     // Stream ID is required field. Set value to check
//     const uint32_t STREAM_ID = 0x12345678;
//     packet_in.stream_id(STREAM_ID);
//     CHECK(packet_in.stream_id() == STREAM_ID);

//     // Add sector step scan records to check
//     vrtgen::packing::SectorStepScan<record_type> sector_step_scan;
//     std::vector<record_type> RECORDS;
//     const uint32_t NUM_RECORDS = 3;
//     for (uint32_t i=0; i<NUM_RECORDS; ++i) {
//         record_type record;
//         record.sector_number(i+1);
//         record.f1_start_frequency((i+1)*2);
//         record.f2_stop_frequency((i+1)*4);
//         RECORDS.push_back(record);
//     }
//     sector_step_scan.records(RECORDS);
//     packet_in.sector_step_scan(sector_step_scan);
//     const uint32_t NUM_WORDS_PER_RECORD = ceil(RECORDS.front().size() / 4);
//     PACKED_SIZE += (NUM_WORDS_PER_RECORD * NUM_RECORDS * 4);

//     // Check bytes required
//     CHECK(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

//     // Get buffer from pack
//     auto data = packet_helper::pack(packet_in);
//     CHECK(data.size() == PACKED_SIZE);
//     auto* check_ptr = data.data();

//     // Examine and check packed header
//     const size_t HEADER_BYTES = 4;
//     const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
//     const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
//     const bytes HEADER_BE{ PACKET_TYPE , 0, 0, PACKET_SIZE };
//     const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
//     check_ptr += HEADER_BYTES;
//     CHECK(packed_header == HEADER_BE);

//     // Examine and check packed Stream ID. Value shall be in big-endian format.
//     const size_t STREAM_ID_BYTES = 4;
//     const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
//     const decltype(data) packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
//     check_ptr += STREAM_ID_BYTES;
//     CHECK(packed_stream_id == STREAM_ID_BE);

//     // Examine and check packed CIF0
//     const size_t CIF0_BYTES = 4;
//     const uint8_t CIF1_ENABLE = 0x1 << 1; // CIF 0 bit 1
//     const bytes CIF0_BE{ 0, 0, 0, CIF1_ENABLE };
//     const decltype(data) packed_cif0(check_ptr, check_ptr + CIF0_BYTES);
//     check_ptr += CIF0_BYTES;
//     CHECK(packed_cif0 == CIF0_BE);

//     // Examine and check packed CIF1
//     const size_t CIF1_BYTES = 4;
//     const uint8_t SECTOR_STEP_SCAN_ENABLE = 0x1 << 1; // CIF 1 bit 9
//     const bytes CIF1_BE{ 0, 0, SECTOR_STEP_SCAN_ENABLE, 0 };
//     const decltype(data) packed_cif1(check_ptr, check_ptr + CIF1_BYTES);
//     check_ptr += CIF1_BYTES;
//     CHECK(packed_cif1 == CIF1_BE);

//     // Examine and check packed Sector Step/Scan header fields. Values shall be in big-endian format.
//     const bytes SECTOR_STEP_SCAN_HEADER_BE{ 
//         0, 0, 0, 0x12, // array size (18 words)
//         0, 0, 0x50, 3, // header size / num words per record (5) / num records (3)
//         (0x1<<7|0x1<<6|0x1<<5), 0, 0, 0 // subfield cif
//     };
//     const decltype(data) packed_sector_step_scan_header(check_ptr, check_ptr + SECTOR_STEP_SCAN_HEADER_BYTES);
//     check_ptr += SECTOR_STEP_SCAN_HEADER_BYTES;
//     CHECK(packed_sector_step_scan_header == SECTOR_STEP_SCAN_HEADER_BE);

//     // Examine and check packed Sector Step/Scan record fields.
//     const bytes SECTOR_STEP_SCAN_RECORDS_BE{
//         // record 1
//         0, 0, 0, 1, // sector number
//         0, 0, 0, 0, // f1 start frequency
//         0, 0x20, 0, 0,
//         0, 0, 0, 0, // f2 stop frequency
//         0, 0x40, 0, 0,
//         // record 2
//         0, 0, 0, 2, // sector number
//         0, 0, 0, 0, // f1 start frequency
//         0, 0x40, 0, 0,
//         0, 0, 0, 0, // f2 stop frequency
//         0, 0x80, 0, 0,
//         // record 3
//         0, 0, 0, 3, // sector number
//         0, 0, 0, 0, // f1 start frequency
//         0, 0x60, 0, 0,
//         0, 0, 0, 0, // f2 stop frequency
//         0, 0xC0, 0, 0
//     };
//     const decltype(data) packed_sector_step_scan_records(check_ptr, check_ptr + NUM_WORDS_PER_RECORD * NUM_RECORDS * 4);
//     check_ptr += NUM_WORDS_PER_RECORD * NUM_RECORDS * 4;
//     CHECK(packed_sector_step_scan_records == SECTOR_STEP_SCAN_RECORDS_BE);

//     // Check match
//     CHECK_FALSE(packet_helper::match(data.data(), data.size()));

//     // Unpack verifed packed data
//     packet_type packet_out;
//     packet_helper::unpack(packet_out, data.data(), data.size());

//     // Examine and check unpacked packet header
//     const auto& header = packet_out.header();
//     CHECK(header.packet_type() == vrtgen::packing::PacketType::CONTEXT);
//     CHECK_FALSE(header.class_id_enable());
//     CHECK_FALSE(header.not_v49d0());
//     CHECK(header.tsm() == vrtgen::packing::TSM::FINE);
//     CHECK(header.tsi() == vrtgen::packing::TSI::NONE);
//     CHECK(header.tsf() == vrtgen::packing::TSF::NONE);
//     CHECK(header.packet_size() == PACKET_SIZE);

//     // Examine and check unpacked Stream ID
//     CHECK(packet_out.stream_id() == STREAM_ID);

//     // Examine and check unpacked Sector/Step Scan
//     CHECK(packet_out.sector_step_scan().array_size() == 18); // 3 header words + 5*3 record words
//     CHECK(packet_out.sector_step_scan().header_size() == 0);
//     CHECK(packet_out.sector_step_scan().num_words_record() == NUM_WORDS_PER_RECORD);
//     CHECK(packet_out.sector_step_scan().num_records() == NUM_RECORDS);
//     auto& records = packet_out.sector_step_scan().records();
//     CHECK(records.size() == NUM_RECORDS);
//     for (uint32_t i=0; i<NUM_RECORDS; ++i) {
//         CHECK(records[i].sector_number() == RECORDS[i].sector_number());
//         CHECK(records[i].f1_start_frequency() == RECORDS[i].f1_start_frequency());
//         CHECK(records[i].f2_stop_frequency() == RECORDS[i].f2_stop_frequency());
//     }
// } // end TEST_CASE("Context Packet Sector Step/Scan")