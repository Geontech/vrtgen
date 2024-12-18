/*
 * Copyright (C) 2023 Geon Technologies, LLC
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
#include "class_id/test_context_class_id1.hpp"
#include "class_id/test_context_class_id2.hpp"
#include "class_id/test_control_class_id1.hpp"
#include "class_id/test_control_class_id2.hpp"
#include "class_id/test_data_class_id1.hpp"
#include "class_id/test_data_class_id2.hpp"
#include <bytes.hpp>
#include <vrtgen/packing/enums.hpp>
#include "constants.hpp"

using namespace class_id_ns::packets;

TEST_CASE("ClassID 5.1.3", "[class_id]")
{
    SECTION("Rule 5.1.3-5")
    {
        bytes CLASS_ID_BE;
        TestDataClassId1 packet_in;
        auto data = packet_in.data();
        CHECK((data[0] & 0b00000111) == 0);
    }

    SECTION("Zero on construction (either oui or packet code)") 
    {
        bytes CLASS_ID_BE;

        SECTION ("OUI")
        {
            TestDataClassId1 packet_in;
            CHECK(packet_in.class_id().information_code() == 0);
            CHECK(packet_in.class_id().packet_code() == 0);
            CHECK(packet_in.class_id().pad_bits() == 0);
        }

        SECTION ("Packet Code")
        {
            TestDataClassId2 packet_in;
            CHECK(packet_in.class_id().information_code() == 0);
            CHECK(packet_in.class_id().oui() == 0);
            CHECK(packet_in.class_id().pad_bits() == 0);
        }
    }

    SECTION("Byte Size")
    {
        TestDataClassId1 packet_in;
        auto data = packet_in.data();
        CHECK(data.size() == HEADER_BYTES + CLASS_ID_BYTES);
    }

    SECTION("Header Flag and Byte Size")
    {
        TestDataClassId1 packet_in;
        auto data = packet_in.data();

        // Examine and check packed header
        CHECK((data[0] & 0b00001000) == 1 << 3);
    }

    SECTION("Bit pad count")
    {
        TestDataClassId1 packet_in;

	const uint8_t PAD_BITS = 0xE;
	packet_in.pad_bits(PAD_BITS);
	CHECK(packet_in.pad_bits() == PAD_BITS);
	auto data = packet_in.data();
	bytes CLASS_ID_BE = bytes{ PAD_BITS << 3, 0xAA, 0xBB, 0xCC, 0, 0, 0, 0 };

	TestDataClassId1 packet_out(data);
	CHECK(packet_out.pad_bits() == PAD_BITS);
	auto* check_ptr = data.data();
	check_ptr += HEADER_BYTES;

	// Examine and check packed Class ID. Value shall be in big-endian format.
	const bytes packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
	check_ptr += CLASS_ID_BYTES;
	CHECK(packed_class_id == CLASS_ID_BE);
    }

    SECTION("Yaml Input") 
    {
        bytes data;
        bytes CLASS_ID_BE;

        SECTION ("Data: OUI")
        {
            TestDataClassId1 packet_in;
            CHECK(packet_in.class_id().oui() == 0xAABBCC);
            auto data = packet_in.data();
            CLASS_ID_BE = bytes{ 0, 0xAA, 0xBB, 0xCC, 0, 0, 0, 0 };

            TestDataClassId1 packet_out(data);
            CHECK(packet_out.class_id().oui() == 0xAABBCC);
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES;

            // Examine and check packed Class ID. Value shall be in big-endian format.
            const bytes packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
            check_ptr += CLASS_ID_BYTES;
            CHECK(packed_class_id == CLASS_ID_BE);
        }

        SECTION("Data: Packet Code")
        {
            TestDataClassId2 packet_in;
            CHECK(packet_in.class_id().packet_code() == 0x1234);
            auto data = packet_in.data();
            CLASS_ID_BE = bytes{ 0, 0, 0, 0, 0, 0, 0x12, 0x34 };

            TestDataClassId2 packet_out(data);
            CHECK(packet_out.class_id().packet_code() == 0x1234);
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES;

            // Examine and check packed Class ID. Value shall be in big-endian format.
            const bytes packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
            check_ptr += CLASS_ID_BYTES;
            CHECK(packed_class_id == CLASS_ID_BE);
        }

        SECTION ("Context: OUI")
        {
            TestContextClassId1 packet_in;
            CHECK(packet_in.class_id().oui() == 0xAABBCC);
            auto data = packet_in.data();
            CLASS_ID_BE = bytes{ 0, 0xAA, 0xBB, 0xCC, 0, 0, 0, 0 };

            TestContextClassId1 packet_out(data);
            CHECK(packet_out.class_id().oui() == 0xAABBCC);
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            check_ptr += STREAM_ID_BYTES;

            // Examine and check packed Class ID. Value shall be in big-endian format.
            const bytes packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
            check_ptr += CLASS_ID_BYTES;
            CHECK(packed_class_id == CLASS_ID_BE);
        }

        SECTION("Context: Packet Code")
        {
            TestContextClassId2 packet_in;
            CHECK(packet_in.class_id().packet_code() == 0x1234);
            auto data = packet_in.data();
            CLASS_ID_BE = bytes{ 0, 0, 0, 0, 0, 0, 0x12, 0x34 };

            TestContextClassId2 packet_out(data);
            CHECK(packet_out.class_id().packet_code() == 0x1234);
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            check_ptr += STREAM_ID_BYTES;

            // Examine and check packed Class ID. Value shall be in big-endian format.
            const bytes packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
            check_ptr += CLASS_ID_BYTES;
            CHECK(packed_class_id == CLASS_ID_BE);
        }

        SECTION ("Control: OUI")
        {
            TestControlClassId1 packet_in;
            CHECK(packet_in.class_id().oui() == 0xAABBCC);
            auto data = packet_in.data();
            CLASS_ID_BE = bytes{ 0, 0xAA, 0xBB, 0xCC, 0, 0, 0, 0 };

            TestControlClassId1 packet_out(data);
            CHECK(packet_out.class_id().oui() == 0xAABBCC);
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            check_ptr += STREAM_ID_BYTES;

            // Examine and check packed Class ID. Value shall be in big-endian format.
            const bytes packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
            check_ptr += CLASS_ID_BYTES;
            CHECK(packed_class_id == CLASS_ID_BE);
        }

        SECTION("Control: Packet Code")
        {
            TestControlClassId2 packet_in;
            CHECK(packet_in.class_id().packet_code() == 0x1234);
            auto data = packet_in.data();
            CLASS_ID_BE = bytes{ 0, 0, 0, 0, 0, 0, 0x12, 0x34 };

            TestControlClassId2 packet_out(data);
            CHECK(packet_out.class_id().packet_code() == 0x1234);
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            check_ptr += STREAM_ID_BYTES;

            // Examine and check packed Class ID. Value shall be in big-endian format.
            const bytes packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
            check_ptr += CLASS_ID_BYTES;
            CHECK(packed_class_id == CLASS_ID_BE);
        }
    }
}
