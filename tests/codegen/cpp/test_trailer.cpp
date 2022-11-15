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
#include "packettrailer.hpp"
#include <bytes.hpp>
#include <vrtgen/packing/enums.hpp>
#include "constants.hpp"

TEST_CASE("Trailer", "[trailer]")
{
    SECTION("Rule 5.1.6-4")
    {
        SECTION("User-defined Single-bit Indicators")
        {
            bytes TRAILER_BE{ 0, 0x30, 0x01, 0 };
            TrailerData3 packet_in;
            CHECK_FALSE(packet_in.trailer().user_defined1_enable());
            CHECK_FALSE(packet_in.trailer().user_defined2_enable());
            CHECK_FALSE(packet_in.user_defined1());
            packet_in.user_defined1(true);
            packet_in.user_defined2(false);
            CHECK(packet_in.user_defined1() == true);
            CHECK(packet_in.user_defined2() == false);

            auto data = packet_in.data();
            auto* check_ptr = data.data();

            check_ptr += HEADER_BYTES; // get the last 4 bytes
            const bytes trailer(check_ptr, check_ptr + TRAILER_BYTES);
            CHECK(trailer == TRAILER_BE);

            TrailerData3 packet_out(data);
            CHECK(packet_out.user_defined1() == true);
        }
        
        SECTION("User-defined Enum Indicators")
        {
            bytes TRAILER_BE{ 0, 0xF0, 0x0F, 0 };
            TrailerData4 packet_in;
            // CHECK(packet_in.user_defined1_enable() == false);
            CHECK_FALSE(packet_in.user_defined1());
            packet_in.user_defined1(trailer_data4::enums::user_defined1::fifteen);
            CHECK(packet_in.user_defined1() == trailer_data4::enums::user_defined1::fifteen);

            auto data = packet_in.data();
            auto* check_ptr = data.data();

            check_ptr += HEADER_BYTES; // get the last 4 bytes
            const bytes trailer(check_ptr, check_ptr + TRAILER_BYTES);
            CHECK(trailer == TRAILER_BE);

            TrailerData4 packet_out(data);
            CHECK(packet_out.user_defined1() == trailer_data4::enums::user_defined1::fifteen);
        }

        SECTION("Sample Frame and User Defined")
        {
            bytes TRAILER_BE{ 0, 0xF0, 0x06, 0 };
            TrailerData6 packet_in;
            // CHECK(packet_in.user_defined1_enable() == false);
            CHECK_FALSE(packet_in.sample_frame());
            CHECK_FALSE(packet_in.user_defined1());
            packet_in.user_defined1(trailer_data6::enums::user_defined1::two);
            packet_in.sample_frame(vrtgen::packing::SSI::FIRST);
            CHECK(packet_in.user_defined1() == trailer_data6::enums::user_defined1::two);
            CHECK(packet_in.sample_frame() == vrtgen::packing::SSI::FIRST);
            // FIXME Does not sync up when user_defined1 is set
            // CHECK(packet_out.user_defined1_enable() == true);

            auto data = packet_in.data();
            auto* check_ptr = data.data();

            check_ptr += HEADER_BYTES; // get the last 4 bytes
            const bytes trailer(check_ptr, check_ptr + TRAILER_BYTES);
            CHECK(trailer == TRAILER_BE);

            TrailerData6 packet_out(data);
            CHECK(packet_out.user_defined1() == trailer_data6::enums::user_defined1::two);
            CHECK(packet_out.sample_frame() == vrtgen::packing::SSI::FIRST);
            // CHECK(packet_out.user_defined1_enable() == true);
        }
    }

//     SECTION("Rule 5.1.6-13")
//     {
//         bytes TRAILER_BE{ 0, 0, 0, 0xFF };

//         SECTION("Required")
//         {
//             TrailerData1 packet_in;
//             CHECK_FALSE(packet_in.associated_context_packets_count());
//             packet_in.associated_context_packets_count(0x7F);
//             CHECK(packet_in.associated_context_packets_count() == 0x7F);

//             auto data = packet_in.data();
//             auto* check_ptr = data.data();

//             check_ptr += HEADER_BYTES; // get the last 4 bytes
//             const bytes trailer(check_ptr, check_ptr + TRAILER_BYTES);
//             CHECK(trailer == TRAILER_BE);

//             TrailerData1 packet_out(data);
//             CHECK(packet_out.associated_context_packets_count() == 0x7F);
//         }

//         SECTION("Optional OFF")
//         {
//             bytes TRAILER_BE{ 0, 0, 0, 0 };
//             TrailerData2 packet_in;
//             CHECK_FALSE(packet_in.associated_context_packets_count());

//             auto data = packet_in.data();
//             auto* check_ptr = data.data();

//             check_ptr += HEADER_BYTES; // get the last 4 bytes
//             const bytes trailer(check_ptr, check_ptr + TRAILER_BYTES);
//             CHECK(trailer == TRAILER_BE);

//             TrailerData2 packet_out(data);
//             // REQUIRE_THROWS(packet_out.associated_context_packets_count());
//         }

//         SECTION("Optional ON")
//         {
//             TrailerData2 packet_in;
//             CHECK_FALSE(packet_in.associated_context_packets_count());
//             packet_in.associated_context_packets_count(0x7F);
//             CHECK(packet_in.associated_context_packets_count() == 0x7F);

//             auto data = packet_in.data();
//             auto* check_ptr = data.data();

//             check_ptr += HEADER_BYTES; // get the last 4 bytes
//             const bytes trailer(check_ptr, check_ptr + TRAILER_BYTES);
//             CHECK(trailer == TRAILER_BE);

//             TrailerData2 packet_out(data);
//             CHECK(packet_out.associated_context_packets_count() == 0x7F);
//         }
//     }

    SECTION("Rule 5.1.6.1-1")
    {
        bytes TRAILER_BE{ 0, 0xC0, 0x08, 0x00 };
        TrailerData5 packet_in;
        CHECK_FALSE(packet_in.sample_frame());
        packet_in.sample_frame(vrtgen::packing::SSI::MIDDLE);
        CHECK(packet_in.sample_frame() == vrtgen::packing::SSI::MIDDLE);

        auto data = packet_in.data();

        CHECK(((data[HEADER_BYTES + 2] >> 2) & 0b11) == 0b10); // Check bits 11,10
        CHECK(((data[HEADER_BYTES + 1] >> 6) & 0b11) == 0b11); // Check bits 23,22

        TrailerData5 packet_out(data);
        CHECK(packet_in.sample_frame() == vrtgen::packing::SSI::MIDDLE);
    }

    SECTION("Rule 5.1.6.1-2")
    {
        TrailerData5 packet_in;
        packet_in.sample_frame(vrtgen::packing::SSI::MIDDLE);

        auto data = packet_in.data();
        // FIXME the input header is never set to true even with sample_frame on
        CHECK(packet_in.header().not_v49d0() == true);

        CHECK(((data[0] >> 1) & 0b1)  == 0b1); // check the 25th bit in the header

        TrailerData5 packet_out(data);
        packet_out.sample_frame(vrtgen::packing::SSI::MIDDLE);
        CHECK(packet_out.header().not_v49d0() == true);
    }
}