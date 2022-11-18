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
#include "streamid.hpp"
#include <bytes.hpp>
#include <vrtgen/packing/enums.hpp>
#include "constants.hpp"

TEST_CASE("StreamID 5.1.2", "[stream_id]")
{
    SECTION("Rule 5.1.2-1") 
    {
        // Stream ID is 32-bit and shall be carried in every packet in Packet Stream when used

        // The user is in charge of setting the stream id for the data packet and context packets they want 
        // this shows that it is a 32bit number and can be carried in ever VRT Packet
        const uint32_t STREAM_ID = 0x12345678;
        const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
        // bytes data;
        
        SECTION("Data Packet 32-bit Stream ID")
        {
            WithStreamIdData packet_in;
            CHECK(packet_in.stream_id() == 0);
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            auto data = packet_in.data();
            // CHECK endianess
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            // if equal to 0x12 big endian but if equal to 0x78 little endian
            CHECK(check_ptr[0] == 0x12);
            
            WithStreamIdData packet_out(data);
            CHECK(packet_out.stream_id() == STREAM_ID);
        }

        SECTION("Context Packet 32-bit Stream ID")
        {
            WithStreamIdContext packet_in;
            CHECK(packet_in.stream_id() == 0);
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            auto data = packet_in.data();
            // CHECK endianess
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            // if equal to 0x12 big endian but if equal to 0x78 little endian
            CHECK(check_ptr[0] == 0x12);
            
            WithStreamIdContext packet_out(data);
            CHECK(packet_out.stream_id() == STREAM_ID);
        }

        SECTION("Control Packet 32-bit Stream ID")
        {
            WithStreamIdControl packet_in;
            CHECK(packet_in.stream_id() == 0);
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            auto data = packet_in.data();
            // CHECK endianess
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            // if equal to 0x12 big endian but if equal to 0x78 little endian
            CHECK(check_ptr[0] == 0x12);
            
            WithStreamIdControl packet_out(data);
            CHECK(packet_out.stream_id() == STREAM_ID);
        }
    }

    SECTION("Rule 5.1.2-2") 
    {
        // Stream ID Consistently Omitted/Included - "Consistency" is up to the user to design the yaml correctly
        const uint32_t STREAM_ID = 0x12345678;
        bytes STREAM_ID_BE{ 0, 0, 0, 0 };

        SECTION("Data Packet without Stream ID")
        {
            WithoutStreamIdData packet_in;
            auto data = packet_in.data();
            auto bytes_required = packet_in.size();

            CHECK(bytes_required == HEADER_BYTES);
        }

        SECTION("Data Packet with default Stream ID")
        {
            WithStreamIdData packet_in;
            auto data = packet_in.data();
            auto bytes_required = packet_in.size();

            CHECK(bytes_required == HEADER_BYTES + STREAM_ID_BYTES);

            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            const bytes stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
            check_ptr += STREAM_ID_BYTES;
            CHECK(stream_id == STREAM_ID_BE);
        }

        SECTION("Context Packet default Stream ID")
        {
            WithStreamIdContext packet_in;
            auto data = packet_in.data();
            auto bytes_required = packet_in.size();

            CHECK(bytes_required == BASIC_CONTEXT_BYTES); // header, streamid, and cif

            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            const bytes stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
            check_ptr += STREAM_ID_BYTES;
            CHECK(stream_id == STREAM_ID_BE);
        }

        SECTION("Control Packet default Stream ID")
        {
            WithStreamIdControl packet_in;
            auto data = packet_in.data();
            auto bytes_required = packet_in.size();

            CHECK(bytes_required == BASIC_CONTROL_BYTES); // header, stream id, cif, cam, and message_id

            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            const bytes stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
            check_ptr += STREAM_ID_BYTES;
            CHECK(stream_id == STREAM_ID_BE);
        }

        SECTION("Data Packet with default Stream ID")
        {
            WithStreamIdData packet_in;
            packet_in.stream_id(STREAM_ID);
            STREAM_ID_BE = bytes { 0x12, 0x34, 0x56, 0x78 };
            auto data = packet_in.data();
            auto bytes_required = packet_in.size();

            CHECK(bytes_required == BASIC_DATA_BYTES + STREAM_ID_BYTES);

            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            const bytes stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
            check_ptr += STREAM_ID_BYTES;
            CHECK(stream_id == STREAM_ID_BE);
        }

        // Context Packet Stream ID proven in rule 7.1.2-1

        SECTION("Control Packet default Stream ID")
        {
            WithStreamIdContext packet_in;
            packet_in.stream_id(STREAM_ID);
            STREAM_ID_BE = bytes { 0x12, 0x34, 0x56, 0x78 };
            auto data = packet_in.data();
            auto bytes_required = packet_in.size();

            CHECK(bytes_required == BASIC_CONTEXT_BYTES); // header, streamid, and cif

            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            const bytes stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
            check_ptr += STREAM_ID_BYTES;
            CHECK(stream_id == STREAM_ID_BE);
        }

        SECTION("Control Packet default Stream ID")
        {
            WithStreamIdControl packet_in;
            packet_in.stream_id(STREAM_ID);
            STREAM_ID_BE = bytes { 0x12, 0x34, 0x56, 0x78 };
            auto data = packet_in.data();
            auto bytes_required = packet_in.size();

            CHECK(bytes_required == BASIC_CONTROL_BYTES); // header, stream id, cif, cam, and message_id

            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            const bytes stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
            check_ptr += STREAM_ID_BYTES;
            CHECK(stream_id == STREAM_ID_BE);
        }
    }

    SECTION("Rule 5.1.2-3") 
    {
        // This is user determined when a user chooses to pair Data and Context packets by using the same Stream ID
        CHECK(true);
    }
}

TEST_CASE("Stream ID default value", "[stream_id][default_value]")
{
    TestStreamIdData3 packet_in;
    CHECK(packet_in.stream_id() == 0xDEADBEEF);
}

TEST_CASE("Stream ID User Defined", "[stream_id][user_defined]")
{
    const bytes STREAM_ID_BE{ 0, 0, 0x03, 0xFF };
    TestStreamIdData4 packet_in;
    CHECK(packet_in.stream_id().thing1() == 0);
    test_stream_id_data4::structs::StreamIdentifier id;
    id.thing1(0x3FF);
    packet_in.stream_id(id);
    CHECK(packet_in.stream_id().thing1() == 0x3FF);

    auto data = packet_in.data();
    auto* check_ptr = data.data();
    check_ptr += HEADER_BYTES;
    const bytes stream_id(check_ptr, check_ptr + 4);
    CHECK(stream_id == STREAM_ID_BE);

    TestStreamIdData4 packet_out(data);
    CHECK(packet_out.stream_id().thing1() == 0x3FF);
}