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
            CHECK(packet_in.user_defined1_enable() == false);
            CHECK(packet_in.user_defined1() == false);
            packet_in.user_defined1(true);
            CHECK(packet_in.user_defined1() == true);

            auto data = TrailerData3::helper::pack(packet_in);
            auto* check_ptr = data.data();

            check_ptr += HEADER_BYTES; // get the last 4 bytes
            const decltype(data) trailer(check_ptr, check_ptr + TRAILER_BYTES);
            CHECK(trailer == TRAILER_BE);

            TrailerData3 packet_out;
            TrailerData3::helper::unpack(packet_out, data.data(), data.size());
            CHECK(packet_out.user_defined1() == true);
        }
        
        SECTION("User-defined Enum Indicators")
        {
            bytes TRAILER_BE{ 0, 0xF0, 0x0F, 0 };
            TrailerData4 packet_in;
            CHECK(packet_in.user_defined1_enable() == false);
            CHECK(packet_in.user_defined1() == trailer_data4::enums::user_defined1::zero);
            packet_in.user_defined1(trailer_data4::enums::user_defined1::fifteen);
            CHECK(packet_in.user_defined1() == trailer_data4::enums::user_defined1::fifteen);

            auto data = TrailerData4::helper::pack(packet_in);
            auto* check_ptr = data.data();

            check_ptr += HEADER_BYTES; // get the last 4 bytes
            const decltype(data) trailer(check_ptr, check_ptr + TRAILER_BYTES);
            CHECK(trailer == TRAILER_BE);

            TrailerData4 packet_out;
            TrailerData4::helper::unpack(packet_out, data.data(), data.size());
            CHECK(packet_out.user_defined1() == trailer_data4::enums::user_defined1::fifteen);
        }

        SECTION("Sample Frame and User Defined")
        {
            bytes TRAILER_BE{ 0, 0xF0, 0x06, 0 };
            TrailerData6 packet_in;
            CHECK(packet_in.user_defined1_enable() == false);
            CHECK(packet_in.sample_frame() ==  vrtgen::packing::SSI::SINGLE);
            CHECK(packet_in.user_defined1() == trailer_data6::enums::user_defined1::zero);
            packet_in.user_defined1(trailer_data6::enums::user_defined1::two);
            packet_in.sample_frame(vrtgen::packing::SSI::FIRST);
            CHECK(packet_in.user_defined1() == trailer_data6::enums::user_defined1::two);
            CHECK(packet_in.sample_frame() == vrtgen::packing::SSI::FIRST);
            // FIXME Does not sync up when user_defined1 is set
            // CHECK(packet_out.user_defined1_enable() == true);

            auto data = TrailerData6::helper::pack(packet_in);
            auto* check_ptr = data.data();

            check_ptr += HEADER_BYTES; // get the last 4 bytes
            const decltype(data) trailer(check_ptr, check_ptr + TRAILER_BYTES);
            CHECK(trailer == TRAILER_BE);

            TrailerData6 packet_out;
            TrailerData6::helper::unpack(packet_out, data.data(), data.size());
            CHECK(packet_out.user_defined1() == trailer_data6::enums::user_defined1::two);
            CHECK(packet_out.sample_frame() == vrtgen::packing::SSI::FIRST);
            CHECK(packet_out.user_defined1_enable() == true);
        }
    }

    SECTION("Rule 5.1.6-13")
    {
        bytes TRAILER_BE{ 0, 0, 0, 0xFF };

        SECTION("Required")
        {
            TrailerData1 packet_in;
            CHECK(packet_in.associated_context_packet_count() == 0);
            packet_in.associated_context_packet_count(0x7F);
            CHECK(packet_in.associated_context_packet_count() == 0x7F);

            auto data = TrailerData1::helper::pack(packet_in);
            auto* check_ptr = data.data();

            check_ptr += HEADER_BYTES; // get the last 4 bytes
            const decltype(data) trailer(check_ptr, check_ptr + TRAILER_BYTES);
            CHECK(trailer == TRAILER_BE);

            TrailerData1 packet_out;
            TrailerData1::helper::unpack(packet_out, data.data(), data.size());
            CHECK(packet_out.associated_context_packet_count() == 0x7F);
        }

        SECTION("Optional OFF")
        {
            bytes TRAILER_BE{ 0, 0, 0, 0 };
            TrailerData2 packet_in;
            CHECK(packet_in.has_associated_context_packet_count() == false);
            REQUIRE_THROWS(packet_in.associated_context_packet_count());

            auto data = TrailerData2::helper::pack(packet_in);
            auto* check_ptr = data.data();

            check_ptr += HEADER_BYTES; // get the last 4 bytes
            const decltype(data) trailer(check_ptr, check_ptr + TRAILER_BYTES);
            CHECK(trailer == TRAILER_BE);

            TrailerData2 packet_out;
            TrailerData2::helper::unpack(packet_out, data.data(), data.size());
            REQUIRE_THROWS(packet_out.associated_context_packet_count());
            CHECK(packet_out.has_associated_context_packet_count() == false);
        }

        SECTION("Optional ON")
        {
            TrailerData2 packet_in;
            CHECK(packet_in.has_associated_context_packet_count() == false);
            REQUIRE_THROWS(packet_in.associated_context_packet_count());
            packet_in.associated_context_packet_count(0x7F);
            CHECK(packet_in.has_associated_context_packet_count() == true);
            CHECK(packet_in.associated_context_packet_count() == 0x7F);

            auto data = TrailerData2::helper::pack(packet_in);
            auto* check_ptr = data.data();

            check_ptr += HEADER_BYTES; // get the last 4 bytes
            const decltype(data) trailer(check_ptr, check_ptr + TRAILER_BYTES);
            CHECK(trailer == TRAILER_BE);

            TrailerData2 packet_out;
            TrailerData2::helper::unpack(packet_out, data.data(), data.size());
            CHECK(packet_out.has_associated_context_packet_count() == true);
            CHECK(packet_out.associated_context_packet_count() == 0x7F);
        }
    }

    SECTION("Rule 5.1.6.1-1")
    {
        bytes TRAILER_BE{ 0, 0xC0, 0x08, 0x00 };
        TrailerData5 packet_in;
        CHECK(packet_in.sample_frame() == vrtgen::packing::SSI::SINGLE);
        packet_in.sample_frame(vrtgen::packing::SSI::MIDDLE);
        CHECK(packet_in.sample_frame() == vrtgen::packing::SSI::MIDDLE);

        auto data = TrailerData5::helper::pack(packet_in);

        CHECK(((data[HEADER_BYTES + 2] >> 2) & 0b11) == 0b10); // Check bits 11,10
        CHECK(((data[HEADER_BYTES + 1] >> 6) & 0b11) == 0b11); // Check bits 23,22

        TrailerData5 packet_out;
        TrailerData5::helper::unpack(packet_out, data.data(), data.size());
        CHECK(packet_in.sample_frame() == vrtgen::packing::SSI::MIDDLE);
    }

    SECTION("Rule 5.1.6.1-2")
    {
        TrailerData5 packet_in;
        packet_in.sample_frame(vrtgen::packing::SSI::MIDDLE);

        auto data = TrailerData5::helper::pack(packet_in);
        // FIXME the input header is never set to true even with sample_frame on
        // CHECK(packet_in.header().not_v49d0() == true);

        CHECK(((data[0] >> 1) & 0b1)  == 0b1); // check the 25th bit in the header

        TrailerData5 packet_out;
        TrailerData5::helper::unpack(packet_out, data.data(), data.size());
        packet_out.sample_frame(vrtgen::packing::SSI::MIDDLE);
        CHECK(packet_out.header().not_v49d0() == true);
    }
}