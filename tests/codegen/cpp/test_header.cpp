/*
 * Copyright (C) 2022 Geon Technologies, LLC
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
#include "packetheader.hpp"
#include "classid.hpp"
#include "streamid.hpp"
#include "data.hpp"
#include "packettrailer.hpp"
#include <bytes.hpp>
#include <vrtgen/packing/enums.hpp>
#include "constants.hpp"

TEST_CASE("VRT Packet Header", "[header]")
{
    SECTION("Rule 5.1.1-1") 
    {
        SECTION("Data Packet")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderDataPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = HEADER_BYTES;
            const size_t PACKED_SIZE = TestHeaderDataPacket::helper::bytes_required(packet_in);
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            const uint8_t PACKET_COUNT = 0xE;
            packet_in.packet_count(PACKET_COUNT);

            // Get buffer from pack
            auto data = TestHeaderDataPacket::helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::SIGNAL_DATA) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE, PACKET_COUNT, static_cast<uint8_t>((PACKET_SIZE & 0xFF00) >> 8), static_cast<uint8_t>(PACKET_SIZE & 0xFF) };
            const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
            check_ptr += HEADER_BYTES;
            CHECK(packed_header == HEADER_BE);
        }
        
        SECTION("Context Packet")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderContextPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_CONTEXT_BYTES;
            const size_t PACKED_SIZE = TestHeaderContextPacket::helper::bytes_required(packet_in);
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            const uint8_t PACKET_COUNT = 0xE;
            packet_in.packet_count(PACKET_COUNT);

            // Get buffer from pack
            auto data = TestHeaderContextPacket::helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE, PACKET_COUNT, static_cast<uint8_t>((PACKET_SIZE & 0xFF00) >> 8), static_cast<uint8_t>(PACKET_SIZE & 0xFF) };
            const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
            check_ptr += HEADER_BYTES;
            CHECK(packed_header == HEADER_BE);
        }
        
        SECTION("Control Packet")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderControlPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_CONTROL_BYTES;
            const size_t PACKED_SIZE = TestHeaderControlPacket::helper::bytes_required(packet_in);
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            const uint8_t PACKET_COUNT = 0xE;
            packet_in.packet_count(PACKET_COUNT);

            // Get buffer from pack
            auto data = TestHeaderControlPacket::helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::COMMAND) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const bytes HEADER_BE{ PACKET_TYPE, PACKET_COUNT, static_cast<uint8_t>((PACKET_SIZE & 0xFF00) >> 8), static_cast<uint8_t>(PACKET_SIZE & 0xFF) };
            const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
            check_ptr += HEADER_BYTES;
            CHECK(packed_header == HEADER_BE);
        }

        SECTION("Ack")
        {
            TestHeaderAckPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_ACK_BYTES;
            const size_t PACKED_SIZE = TestHeaderAckPacket::helper::bytes_required(packet_in);
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            const uint8_t PACKET_COUNT = 0xE;
            packet_in.packet_count(PACKET_COUNT);

            // Get buffer from pack
            auto data = TestHeaderAckPacket::helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::COMMAND) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const uint8_t INDICATORS = 0x1 << 2; // acknowledge bit 26 set to true
            const bytes HEADER_BE{ PACKET_TYPE | INDICATORS, PACKET_COUNT, static_cast<uint8_t>((PACKET_SIZE & 0xFF00) >> 8), static_cast<uint8_t>(PACKET_SIZE & 0xFF) };
            const decltype(data) packed_header(check_ptr, check_ptr + HEADER_BYTES);
            check_ptr += HEADER_BYTES;
            CHECK(packed_header == HEADER_BE);
        }
    }

    SECTION("Rule 5.1.1-3") 
    {
        // Stream ID Consistently Omitted/Included - "Consistency" is up to the user to design the yaml correctly
        uint8_t PACKET_TYPE;
        uint8_t* check_ptr;
        bytes data;

        SECTION("Data Packet without Stream ID")
        {
            WithoutStreamIdData packet_in;
            PACKET_TYPE = (0b0000) << 4;
            data = WithoutStreamIdData::helper::pack(packet_in);
            check_ptr = data.data();
            
            WithoutStreamIdData packet_out;
            WithoutStreamIdData::helper::unpack(packet_out, data.data(), data.size());
            CHECK(packet_out.header().packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA);
        }

        SECTION("Data Packet with Stream ID")
        {
            WithStreamIdData packet_in;
            PACKET_TYPE = (0b0001) << 4;
            const bytes HEADER_BE{ PACKET_TYPE };
            data = WithStreamIdData::helper::pack(packet_in);
            check_ptr = data.data();
            
            WithStreamIdData packet_out;
            WithStreamIdData::helper::unpack(packet_out, data.data(), data.size());
            CHECK(packet_out.header().packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID);
        }

        SECTION("Context Packet Stream ID")
        {
            WithStreamIdContext packet_in;
            PACKET_TYPE = (0b0100) << 4;
            const bytes HEADER_BE{ PACKET_TYPE };
            data = WithStreamIdContext::helper::pack(packet_in);
            check_ptr = data.data();
            
            WithStreamIdContext packet_out;
            WithStreamIdContext::helper::unpack(packet_out, data.data(), data.size());
            CHECK(packet_out.header().packet_type() == vrtgen::packing::PacketType::CONTEXT);
        }

        SECTION("Control Packet Stream ID")
        {
            WithStreamIdControl packet_in;
            PACKET_TYPE = (0b0110) << 4;
            data = WithStreamIdControl::helper::pack(packet_in);
            check_ptr = data.data();
            
            WithStreamIdControl packet_out;
            WithStreamIdControl::helper::unpack(packet_out, data.data(), data.size());
            CHECK(packet_out.header().packet_type() == vrtgen::packing::PacketType::COMMAND);
        }

        const bytes HEADER_BE{ PACKET_TYPE };
        const decltype(data) packet_type(check_ptr, check_ptr + 1);
        check_ptr += HEADER_BYTES;
        CHECK(packet_type == HEADER_BE);
    }

    SECTION("Rule 5.1.1-4")
    {
        SECTION("Data Packet")
        {
            TestDataClassId1 packet_in;
            auto data = TestDataClassId1::helper::pack(packet_in);

            // Examine and check packed header
            CHECK((data[0] & 0b00001000) >> 3 == 1);
        }
        
        SECTION("Context Packet")
        {
            TestContextClassId1 packet_in;
            auto data = TestContextClassId1::helper::pack(packet_in);

            // Examine and check packed header
            CHECK((data[0] & 0b00001000) >> 3 == 1);
        }
        
        SECTION("Control Packet")
        {
            TestControlClassId1 packet_in;
            auto data = TestControlClassId1::helper::pack(packet_in);

            // Examine and check packed header
            CHECK((data[0] & 0b00001000) >> 3 == 1);
        }
    }

    SECTION("Rule 5.1.1-5") 
    {
        SECTION("Signal Data Packet")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderDataTSPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = HEADER_BYTES + INTEGER_TS_BYTES + FRACTIONAL_TS_BYTES;
            const size_t PACKED_SIZE = TestHeaderDataTSPacket::helper::bytes_required(packet_in);
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = TestHeaderDataTSPacket::helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);

            CHECK((data[1] & 0b11000000) >> 6 == 0b01);
        }
        
        SECTION("Context Packet")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderContextTSPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_CONTEXT_BYTES + INTEGER_TS_BYTES + FRACTIONAL_TS_BYTES;
            const size_t PACKED_SIZE = TestHeaderContextTSPacket::helper::bytes_required(packet_in);
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = TestHeaderContextTSPacket::helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);

            CHECK((data[1] & 0b11000000) >> 6 == 0b01);
        }
        
        SECTION("Control Packet")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderControlTSPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_CONTROL_BYTES + INTEGER_TS_BYTES + FRACTIONAL_TS_BYTES;
            const size_t PACKED_SIZE = TestHeaderControlTSPacket::helper::bytes_required(packet_in);
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = TestHeaderControlTSPacket::helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);

            CHECK((data[1] & 0b11000000) >> 6 == 0b01);
        }

        SECTION("Ack")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderAckTSPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_ACK_BYTES + INTEGER_TS_BYTES + FRACTIONAL_TS_BYTES;
            const size_t PACKED_SIZE = TestHeaderAckTSPacket::helper::bytes_required(packet_in);
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = TestHeaderAckTSPacket::helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);

            CHECK((data[1] & 0b11000000) >> 6 == 0b01);
        }
    }

    SECTION("Rule 5.1.1-7") 
    {
        SECTION("Signal Data Packet")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderDataTSPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = HEADER_BYTES + INTEGER_TS_BYTES + FRACTIONAL_TS_BYTES;
            const size_t PACKED_SIZE = TestHeaderDataTSPacket::helper::bytes_required(packet_in);
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = TestHeaderDataTSPacket::helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);

            CHECK((data[1] & 0b00110000) >> 4 == 0b10);
        }
        
        SECTION("Context Packet")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderContextTSPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_CONTEXT_BYTES + INTEGER_TS_BYTES + FRACTIONAL_TS_BYTES;
            const size_t PACKED_SIZE = TestHeaderContextTSPacket::helper::bytes_required(packet_in);
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = TestHeaderContextTSPacket::helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);

            CHECK((data[1] & 0b00110000) >> 4 == 0b10);
        }
        
        SECTION("Control Packet")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderControlTSPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_CONTROL_BYTES + INTEGER_TS_BYTES + FRACTIONAL_TS_BYTES;
            const size_t PACKED_SIZE = TestHeaderControlTSPacket::helper::bytes_required(packet_in);
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = TestHeaderControlTSPacket::helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);

            CHECK((data[1] & 0b00110000) >> 4 == 0b10);
        }

        SECTION("Ack")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderAckTSPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_ACK_BYTES + INTEGER_TS_BYTES + FRACTIONAL_TS_BYTES;
            const size_t PACKED_SIZE = TestHeaderAckTSPacket::helper::bytes_required(packet_in);
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = TestHeaderAckTSPacket::helper::pack(packet_in);
            CHECK(data.size() == PACKED_SIZE);

            CHECK((data[1] & 0b00110000) >> 4 == 0b10);
        }
    }
    
    SECTION("Rule 5.1.1-10")
    {
        // Show that all fields are there in the order of Figure 5.1.1-1
        WithoutStreamIdData packet_in;

        // Set small payload to verify
        bytes payload;
        payload.resize(2048);
        for (size_t i = 0; i < payload.size(); i++) {
            payload[i] = i;
        }
        packet_in.payload(payload.data(), payload.size());

        // Check bytes required
        const size_t EXPECTED_SIZE = HEADER_BYTES + payload.size();
        const size_t PACKED_SIZE = WithoutStreamIdData::helper::bytes_required(packet_in);
        CHECK(PACKED_SIZE == EXPECTED_SIZE);

        // Get buffer from pack
        auto data = WithoutStreamIdData::helper::pack(packet_in);
        CHECK(data.size() == PACKED_SIZE);
        CHECK(static_cast<uint16_t>((data[2] << 8) | data[3]) == EXPECTED_SIZE / 4);
    }
}

TEST_CASE("Indicator Bits 5.1.1.1", "[header][indicator_bits]")
{
    SECTION("Rule 5.1.1.1-1")
    {
        SECTION("Signal Data Packet")
        {
            SECTION("Trailer Included")
            {
                TrailerData5 packet_in;
                auto data = TrailerData5::helper::pack(packet_in);
                // FIXME #43
                // CHECK(packet_in.header().trailer_included() == true);

                CHECK((data[0] & 0b100) >> 2 == 1);

                TrailerData5 packet_out;
                TrailerData5::helper::unpack(packet_out, data.data(), data.size());
                CHECK(packet_out.header().trailer_included() == true);
            }
            
            SECTION("Not V49.0")
            {
                // FIXME #57
                TrailerData5 packet_in;
                auto data = TrailerData5::helper::pack(packet_in);
                // FIXME #43
                // CHECK(packet_in.header().not_v49d0() == true);

                // CHECK((data[0] & 0b010) >> 1 == 1);

                TrailerData5 packet_out;
                TrailerData5::helper::unpack(packet_out, data.data(), data.size());
                // CHECK(packet_out.header().not_v49d0() == true);
            }
            
            SECTION("Signal Spectrum or Signal Time Data Packet")
            {
                // FIXME This can be set by the user but I do not believe ::pack takes care of this properly
            }
        }
        
        SECTION("Context Packet")
        {
            SECTION("Not V49.0")
            {
                // Use a context packet that uses CIF1 (for example) and then check that 'not V49.0' is true
                // FIXME #57
                TestHeaderContextNotV49d0Packet packet_in;
                auto data = TestHeaderContextNotV49d0Packet::helper::pack(packet_in);
                // FIXME #43
                // CHECK(packet_in.header().not_v49d0() == true);

                CHECK((data[0] & 0b010) >> 1 == 1);

                TestHeaderContextNotV49d0Packet packet_out;
                TestHeaderContextNotV49d0Packet::helper::unpack(packet_out, data.data(), data.size());
                CHECK(packet_out.header().not_v49d0() == true);
            }

            SECTION("Timestamp Mode")
            {
                TestHeaderTsmContext8 packet_in;
                auto data = TestHeaderTsmContext8::helper::pack(packet_in);
                // FIXME #43
                // CHECK(packet_in.header().tsm() == vrtgen::packing::TSM::COARSE);

                CHECK((data[0] & 0b001)  == 1);

                TestHeaderTsmContext8 packet_out;
                TestHeaderTsmContext8::helper::unpack(packet_out, data.data(), data.size());
                CHECK(packet_out.header().tsm() == vrtgen::packing::TSM::COARSE);
            }
        }
        
        SECTION("Control Packet")
        {
            SECTION("Acknowledge Packet")
            {
                TestHeaderAckPacket packet_in;
                auto data = TestHeaderAckPacket::helper::pack(packet_in);
                // FIXME #43
                // CHECK(packet_in.header().acknowledge_packet() == true);

                CHECK((data[0] & 0b100) >> 2  == 1);

                TestHeaderAckPacket packet_out;
                TestHeaderAckPacket::helper::unpack(packet_out, data.data(), data.size());
                CHECK(packet_out.header().acknowledge_packet() == true);
            }

            SECTION("CanceLlation Packet")
            {
                // Issue #58
            }
        }
    }
}