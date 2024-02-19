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

#include "header/test_header_ack_packet.hpp"
#include "header/test_header_ack_t_s_packet.hpp"
#include "header/test_header_context_not_v49d0_packet.hpp"
#include "header/test_header_context_packet.hpp"
#include "header/test_header_context_t_s_packet.hpp"
#include "header/test_header_control_packet.hpp"
#include "header/test_header_control_t_s_packet.hpp"
#include "header/test_header_data_packet.hpp"
#include "header/test_header_data_t_s_packet.hpp"
#include "header/test_header_tsm_context8.hpp"
#include "stream_id/without_stream_id_data.hpp"
#include "stream_id/with_stream_id_data.hpp"
#include "stream_id/with_stream_id_context.hpp"
#include "stream_id/with_stream_id_control.hpp"
#include "class_id/test_data_class_id1.hpp"
#include "class_id/test_context_class_id1.hpp"
#include "class_id/test_control_class_id1.hpp"
#include "trailer/trailer_data1.hpp"
#include "trailer/trailer_data5.hpp"
#include "trailer/trailer_data6.hpp"
#include "constants.hpp"

using namespace header_ns::packets;
using namespace stream_id_ns::packets;
using namespace class_id_ns::packets;
using namespace trailer_ns::packets;

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
            const size_t PACKED_SIZE = packet_in.size();
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            const uint8_t PACKET_COUNT = 0xE;
            packet_in.packet_count(PACKET_COUNT);

            // Get buffer from pack
            auto data = packet_in.data();
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::SIGNAL_DATA) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            
            bytes HEADER_BE { PACKET_TYPE, PACKET_COUNT, 0, PACKET_SIZE };
            const bytes packed_header(check_ptr, check_ptr + HEADER_BYTES);
            check_ptr += HEADER_BYTES;
            CHECK(packed_header == HEADER_BE);
        }
        
        SECTION("Context Packet")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderContextPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_CONTEXT_BYTES;
            const size_t PACKED_SIZE = packet_in.size();
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            const uint8_t PACKET_COUNT = 0xE;
            packet_in.packet_count(PACKET_COUNT);

            // Get buffer from pack
            auto data = packet_in.data();
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::CONTEXT) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            
            bytes HEADER_BE { PACKET_TYPE, PACKET_COUNT, 0, PACKET_SIZE };
            const bytes packed_header(check_ptr, check_ptr + HEADER_BYTES);
            check_ptr += HEADER_BYTES;
            CHECK(packed_header == HEADER_BE);
        }
        
        SECTION("Control Packet")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderControlPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_CONTROL_BYTES;
            const size_t PACKED_SIZE = packet_in.size();
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            const uint8_t PACKET_COUNT = 0xE;
            packet_in.packet_count(PACKET_COUNT);

            // Get buffer from pack
            auto data = packet_in.data();
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::COMMAND) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            
            bytes HEADER_BE { PACKET_TYPE, PACKET_COUNT, 0, PACKET_SIZE };
            const bytes packed_header(check_ptr, check_ptr + HEADER_BYTES);
            check_ptr += HEADER_BYTES;
            CHECK(packed_header == HEADER_BE);
        }

        SECTION("Ack")
        {
            TestHeaderAckPacketVX packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_ACK_BYTES;
            const size_t PACKED_SIZE = packet_in.size();
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            const uint8_t PACKET_COUNT = 0xE;
            packet_in.packet_count(PACKET_COUNT);

            // Get buffer from pack
            auto data = packet_in.data();
            CHECK(data.size() == PACKED_SIZE);
            auto* check_ptr = data.data();

            // Examine and check packed header
            const size_t HEADER_BYTES = 4;
            const uint8_t PACKET_TYPE = static_cast<uint8_t>(vrtgen::packing::PacketType::COMMAND) << 4;
            const uint8_t PACKET_SIZE = PACKED_SIZE / 4;
            const uint8_t INDICATORS = 0x1 << 2; // acknowledge bit 26 set to true
            
            bytes HEADER_BE { PACKET_TYPE | INDICATORS, PACKET_COUNT, 0, PACKET_SIZE };
            const bytes packed_header(check_ptr, check_ptr + HEADER_BYTES);
            check_ptr += HEADER_BYTES;
            CHECK(packed_header == HEADER_BE);
        }
    }

    SECTION("Rule 5.1.1-3") 
    {
        // Stream ID Consistently Omitted/Included - "Consistency" is up to the user to design the yaml correctly
        uint8_t PACKET_TYPE;
        bytes data;

        // Moved to 6.1-3 to not have repeated tests
        SECTION("Data Packet without Stream ID")
        {
            WithoutStreamIdData packet_in;
            PACKET_TYPE = (0b0000) << 4;
            auto data = packet_in.data();
            auto* check_ptr = data.data();
        
            bytes PACKET_BE { PACKET_TYPE };
            const bytes packet_type(check_ptr, check_ptr + 1);
            check_ptr += HEADER_BYTES;
            CHECK(packet_type == PACKET_BE);

            WithoutStreamIdData packet_out(data);
            CHECK(packet_out.header().packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA);
        }

        SECTION("Data Packet with Stream ID")
        {
            WithStreamIdData packet_in;
            PACKET_TYPE = (0b0001) << 4;
            auto data = packet_in.data();
            auto* check_ptr = data.data();
        
            bytes PACKET_BE { PACKET_TYPE };
            const bytes packet_type(check_ptr, check_ptr + 1);
            check_ptr += HEADER_BYTES;
            CHECK(packet_type == PACKET_BE);

            WithStreamIdData packet_out(data);
            CHECK(packet_out.header().packet_type() == vrtgen::packing::PacketType::SIGNAL_DATA_STREAM_ID);
        }

        SECTION("Context Packet Stream ID")
        {
            WithStreamIdContext packet_in;
            PACKET_TYPE = (0b0100) << 4;
            auto data = packet_in.data();
            auto* check_ptr = data.data();

            bytes PACKET_BE { PACKET_TYPE };
            const bytes packet_type(check_ptr, check_ptr + 1);
            check_ptr += HEADER_BYTES;
            CHECK(packet_type == PACKET_BE);

            WithStreamIdContext packet_out(data);
            CHECK(packet_out.header().packet_type() == vrtgen::packing::PacketType::CONTEXT);
        }

        SECTION("Control Packet Stream ID")
        {
            WithStreamIdControl packet_in;
            PACKET_TYPE = (0b0110) << 4;
            auto data = packet_in.data();
            auto* check_ptr = data.data();

            bytes PACKET_BE { PACKET_TYPE };
            const bytes packet_type(check_ptr, check_ptr + 1);
            check_ptr += HEADER_BYTES;
            CHECK(packet_type == PACKET_BE);

            WithStreamIdControl packet_out(data);
            CHECK(packet_out.header().packet_type() == vrtgen::packing::PacketType::COMMAND);
        }
    }

    SECTION("Rule 5.1.1-4")
    {
        SECTION("Data Packet")
        {
            TestDataClassId1 packet_in;
            auto data = packet_in.data();

            // Examine and check packed header
            CHECK((data[0] & 0b00001000) >> 3 == 1);
        }
        
        SECTION("Context Packet")
        {
            TestContextClassId1 packet_in;
            auto data = packet_in.data();

            // Examine and check packed header
            CHECK((data[0] & 0b00001000) >> 3 == 1);
        }
        
        SECTION("Control Packet")
        {
            TestControlClassId1 packet_in;
            auto data = packet_in.data();

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
            const size_t PACKED_SIZE = packet_in.size();
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = packet_in.data();
            CHECK(data.size() == PACKED_SIZE);

            CHECK((data[1] & 0b11000000) >> 6 == 0b01);
        }
        
        SECTION("Context Packet")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderContextTSPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_CONTEXT_BYTES + INTEGER_TS_BYTES + FRACTIONAL_TS_BYTES;
            const size_t PACKED_SIZE = packet_in.size();
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = packet_in.data();
            CHECK(data.size() == PACKED_SIZE);

            CHECK((data[1] & 0b11000000) >> 6 == 0b01);
        }
        
        SECTION("Control Packet")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderControlTSPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_CONTROL_BYTES + INTEGER_TS_BYTES + FRACTIONAL_TS_BYTES;
            const size_t PACKED_SIZE = packet_in.size();
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = packet_in.data();
            CHECK(data.size() == PACKED_SIZE);

            CHECK((data[1] & 0b11000000) >> 6 == 0b01);
        }

        SECTION("Ack")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderAckTSPacketVX packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_ACK_BYTES + INTEGER_TS_BYTES + FRACTIONAL_TS_BYTES;
            const size_t PACKED_SIZE = packet_in.size();
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = packet_in.data();
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
            const size_t PACKED_SIZE = packet_in.size();
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = packet_in.data();
            CHECK(data.size() == PACKED_SIZE);

            CHECK((data[1] & 0b00110000) >> 4 == 0b10);
        }
        
        SECTION("Context Packet")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderContextTSPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_CONTEXT_BYTES + INTEGER_TS_BYTES + FRACTIONAL_TS_BYTES;
            const size_t PACKED_SIZE = packet_in.size();
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = packet_in.data();
            CHECK(data.size() == PACKED_SIZE);

            CHECK((data[1] & 0b00110000) >> 4 == 0b10);
        }
        
        SECTION("Control Packet")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderControlTSPacket packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_CONTROL_BYTES + INTEGER_TS_BYTES + FRACTIONAL_TS_BYTES;
            const size_t PACKED_SIZE = packet_in.size();
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = packet_in.data();
            CHECK(data.size() == PACKED_SIZE);

            CHECK((data[1] & 0b00110000) >> 4 == 0b10);
        }

        SECTION("Ack")
        {
            // Show that all fields are there in the order of Figure 5.1.1-1
            TestHeaderAckTSPacketVX packet_in;

            // Check bytes required
            const size_t EXPECTED_SIZE = BASIC_ACK_BYTES + INTEGER_TS_BYTES + FRACTIONAL_TS_BYTES;
            const size_t PACKED_SIZE = packet_in.size();
            CHECK(PACKED_SIZE == EXPECTED_SIZE);

            // Get buffer from pack
            auto data = packet_in.data();
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
        packet_in.payload(payload);

        // Check bytes required
        const size_t EXPECTED_SIZE = HEADER_BYTES + payload.size();
        const size_t PACKED_SIZE = packet_in.size();
        CHECK(PACKED_SIZE == EXPECTED_SIZE);

        // Get buffer from pack
        auto data = packet_in.data();
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
                TrailerData6 packet_in;
                auto data = packet_in.data();
                CHECK(packet_in.header().trailer_included() == true);
                CHECK((data[0] & 0b100) >> 2 == 1);

                TrailerData6 packet_out(data);
                CHECK(packet_out.header().trailer_included() == true);
            }
            
            SECTION("Not V49.0")
            {
                // FIXME #57
                TrailerData5 packet_in;
                auto data = packet_in.data();
                // FIXME #43
                // CHECK(packet_in.header().not_v49d0() == true);

                // CHECK((data[0] & 0b010) >> 1 == 1);


                TrailerData5 packet_out(data);
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
                TestHeaderContextNotV49d0Packet packet_in;
                auto data = packet_in.data();
                CHECK(packet_in.header().not_v49d0() == true);
                CHECK((data[0] & 0b010) >> 1 == 1);
                TestHeaderContextNotV49d0Packet packet_out(data);
                CHECK(packet_out.header().not_v49d0() == true);
            }

            SECTION("Timestamp Mode")
            {
                TestHeaderTsmContext8 packet_in;
                auto data = packet_in.data();
                CHECK(packet_in.header().tsm() == vrtgen::packing::TSM::COARSE);
                CHECK((data[0] & 0b001)  == 1);
                TestHeaderTsmContext8 packet_out(data);
                CHECK(packet_out.header().tsm() == vrtgen::packing::TSM::COARSE);
            }
        }
        
        SECTION("Control Packet")
        {
            SECTION("Acknowledge Packet")
            {
                TestHeaderAckPacketVX packet_in;
                auto data = packet_in.data();
                CHECK(packet_in.header().acknowledge_packet() == true);
                CHECK((data[0] & 0b100) >> 2  == 1);
                TestHeaderAckPacketVX packet_out(data);
                CHECK(packet_out.header().acknowledge_packet() == true);
            }

            SECTION("CanceLlation Packet")
            {
                // Issue #58
            }
        }
    }
}