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

TEST_CASE("Stream ID", "[stream_id]")
{
    SECTION ("Rule 5.1.2-1") 
    {
        // Stream ID is 32-bit and shall be carried in every packet in Packet Stream when used

        // The user is in charge of setting the stream id for the data packet and context packets they want 
        // this shows that it is a 32bit number and can be carried in ever VRT Packet
        const uint32_t STREAM_ID = 0x12345678;
        uint8_t* check_ptr;
        const bytes STREAM_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
        bytes data;
        
        SECTION("Data Packet 32-bit Stream ID")
        {
            WithStreamIdData packet_in;
            CHECK(packet_in.stream_id() == 0);
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            data = WithStreamIdData::helper::pack(packet_in);
            // CHECK endianess
            check_ptr = data.data();
            check_ptr += 4;
            // if equal to 0x12 big endian but if equal to 0x78 little endian
            CHECK(check_ptr[0] == 0x12);
            
            WithStreamIdData packet_out;
            WithStreamIdData::helper::unpack(packet_out, data.data(), data.size());
            CHECK(packet_out.stream_id() == STREAM_ID);
        }

        SECTION("Context Packet 32-bit Stream ID")
        {
            WithStreamIdContext packet_in;
            CHECK(packet_in.stream_id() == 0);
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            data = WithStreamIdContext::helper::pack(packet_in);
            // CHECK endianess
            check_ptr = data.data();
            check_ptr += 4;
            // if equal to 0x12 big endian but if equal to 0x78 little endian
            CHECK(check_ptr[0] == 0x12);
            
            WithStreamIdContext packet_out;
            WithStreamIdContext::helper::unpack(packet_out, data.data(), data.size());
            CHECK(packet_out.stream_id() == STREAM_ID);
        }

        SECTION ("Control Packet 32-bit Stream ID")
        {
            WithStreamIdControl packet_in;
            CHECK(packet_in.stream_id() == 0);
            packet_in.stream_id(STREAM_ID);
            CHECK(packet_in.stream_id() == STREAM_ID);

            data = WithStreamIdControl::helper::pack(packet_in);
            // CHECK endianess
            check_ptr = data.data();
            check_ptr += 4;
            // if equal to 0x12 big endian but if equal to 0x78 little endian
            CHECK(check_ptr[0] == 0x12);
            
            WithStreamIdControl packet_out;
            WithStreamIdControl::helper::unpack(packet_out, data.data(), data.size());
            CHECK(packet_out.stream_id() == STREAM_ID);
        }
    }

    SECTION ("Rule 5.1.2-2") 
    {
        // Stream ID Consistently Omitted/Included - "Consistency" is up to the user to design the yaml correctly
        uint8_t* check_ptr;
        bytes data;
        const uint32_t STREAM_ID = 0x12345678;
        bytes STREAM_ID_BE{ 0, 0, 0, 0 };

        SECTION("Data Packet without Stream ID")
        {
            WithoutStreamIdData packet_in;
            data = WithoutStreamIdData::helper::pack(packet_in);
            auto bytes_required = WithoutStreamIdData::helper::bytes_required(packet_in);

            CHECK(bytes_required == 4);
        }

        SECTION("Data Packet with default Stream ID")
        {
            WithStreamIdData packet_in;
            data = WithStreamIdData::helper::pack(packet_in);
            auto bytes_required = WithStreamIdData::helper::bytes_required(packet_in);

            CHECK(bytes_required == 8);

            check_ptr = data.data();
            check_ptr += 4;
            const decltype(data) stream_id(check_ptr, check_ptr + 4);
            check_ptr += 4;
            CHECK(stream_id == STREAM_ID_BE);
        }

        SECTION("Context Packet default Stream ID")
        {
            WithStreamIdContext packet_in;
            data = WithStreamIdContext::helper::pack(packet_in);
            auto bytes_required = WithStreamIdContext::helper::bytes_required(packet_in);

            CHECK(bytes_required == 8 + 4); // header, streamid, and cif

            check_ptr = data.data();
            check_ptr += 4;
            const decltype(data) stream_id(check_ptr, check_ptr + 4);
            check_ptr += 4;
            CHECK(stream_id == STREAM_ID_BE);
        }

        SECTION ("Control Packet default Stream ID")
        {
            WithStreamIdControl packet_in;
            data = WithStreamIdControl::helper::pack(packet_in);
            auto bytes_required = WithStreamIdControl::helper::bytes_required(packet_in);

            CHECK(bytes_required == 8 + 4 + 4 + 4); // header, stream id, cif, cam, and message_id

            check_ptr = data.data();
            check_ptr += 4;
            const decltype(data) stream_id(check_ptr, check_ptr + 4);
            check_ptr += 4;
            CHECK(stream_id == STREAM_ID_BE);
        }

        SECTION("Data Packet with default Stream ID")
        {
            WithStreamIdData packet_in;
            packet_in.stream_id(STREAM_ID);
            STREAM_ID_BE = bytes { 0x12, 0x34, 0x56, 0x78 };
            data = WithStreamIdData::helper::pack(packet_in);
            auto bytes_required = WithStreamIdData::helper::bytes_required(packet_in);

            CHECK(bytes_required == 8);

            check_ptr = data.data();
            check_ptr += 4;
            const decltype(data) stream_id(check_ptr, check_ptr + 4);
            check_ptr += 4;
            CHECK(stream_id == STREAM_ID_BE);
        }

        SECTION("Context Packet default Stream ID")
        {
            WithStreamIdContext packet_in;
            packet_in.stream_id(STREAM_ID);
            STREAM_ID_BE = bytes { 0x12, 0x34, 0x56, 0x78 };
            data = WithStreamIdContext::helper::pack(packet_in);
            auto bytes_required = WithStreamIdContext::helper::bytes_required(packet_in);

            CHECK(bytes_required == 4 + 4 + 4); // header, streamid, and cif

            check_ptr = data.data();
            check_ptr += 4;
            const decltype(data) stream_id(check_ptr, check_ptr + 4);
            check_ptr += 4;
            CHECK(stream_id == STREAM_ID_BE);
        }

        SECTION ("Control Packet default Stream ID")
        {
            WithStreamIdControl packet_in;
            packet_in.stream_id(STREAM_ID);
            STREAM_ID_BE = bytes { 0x12, 0x34, 0x56, 0x78 };
            data = WithStreamIdControl::helper::pack(packet_in);
            auto bytes_required = WithStreamIdControl::helper::bytes_required(packet_in);

            CHECK(bytes_required == 8 + 4 + 4 + 4); // header, stream id, cif, cam, and message_id

            check_ptr = data.data();
            check_ptr += 4;
            const decltype(data) stream_id(check_ptr, check_ptr + 4);
            check_ptr += 4;
            CHECK(stream_id == STREAM_ID_BE);
        }
    }

    SECTION ("Rule 5.1.2-3") 
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