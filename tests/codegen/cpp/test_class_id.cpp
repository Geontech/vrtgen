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
#include "classid.hpp"
#include <bytes.hpp>
#include <vrtgen/packing/enums.hpp>
#include "constants.hpp"

TEST_CASE("ClassID 5.1.3", "[class_id]")
{
    SECTION("Rule 5.1.3-5")
    {
        bytes data;
        bytes CLASS_ID_BE;
        TestDataClassId1 packet_in;
        data = TestDataClassId1::helper::pack(packet_in);
        CHECK((data[0] & 0b00000111) == 0);
    }

    SECTION("Zero on construction (either oui or packet code)") 
    {
        bytes data;
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
        auto data = TestDataClassId1::helper::pack(packet_in);
        CHECK(data.size() == HEADER_BYTES + CLASS_ID_BYTES);
    }

    SECTION("Header Flag and Byte Size")
    {
        TestDataClassId1 packet_in;
        auto data = TestDataClassId1::helper::pack(packet_in);

        // Examine and check packed header
        CHECK((data[0] & 0b00001000) == 1 << 3);
    }

    SECTION("Yaml Input") 
    {
        bytes data;
        uint8_t* check_ptr;
        bytes CLASS_ID_BE;

        SECTION ("Data: OUI")
        {
            TestDataClassId1 packet_in;
            CHECK(packet_in.class_id().oui() == 0xAABBCC);
            data = TestDataClassId1::helper::pack(packet_in);
            CLASS_ID_BE = bytes{ 0, 0xAA, 0xBB, 0xCC, 0, 0, 0, 0 };

            TestDataClassId1 packet_out;
            CHECK(packet_out.class_id().oui() == 0xAABBCC);
            check_ptr = data.data();
            check_ptr += HEADER_BYTES;
        }

        SECTION("Data: Packet Code")
        {
            TestDataClassId2 packet_in;
            CHECK(packet_in.class_id().packet_code() == 0x1234);
            data = TestDataClassId2::helper::pack(packet_in);
            CLASS_ID_BE = bytes{ 0, 0, 0, 0, 0, 0, 0x12, 0x34 };

            TestDataClassId2 packet_out;
            CHECK(packet_out.class_id().packet_code() == 0x1234);
            check_ptr = data.data();
            check_ptr += HEADER_BYTES;
        }

        SECTION ("Context: OUI")
        {
            TestContextClassId1 packet_in;
            CHECK(packet_in.class_id().oui() == 0xAABBCC);
            data = TestContextClassId1::helper::pack(packet_in);
            CLASS_ID_BE = bytes{ 0, 0xAA, 0xBB, 0xCC, 0, 0, 0, 0 };

            TestContextClassId1 packet_out;
            CHECK(packet_out.class_id().oui() == 0xAABBCC);
            check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            check_ptr += CIF_BYTES;
        }

        SECTION("Context: Packet Code")
        {
            TestContextClassId2 packet_in;
            CHECK(packet_in.class_id().packet_code() == 0x1234);
            data = TestContextClassId2::helper::pack(packet_in);
            CLASS_ID_BE = bytes{ 0, 0, 0, 0, 0, 0, 0x12, 0x34 };

            TestContextClassId2 packet_out;
            CHECK(packet_out.class_id().packet_code() == 0x1234);
            check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            check_ptr += CIF_BYTES;
        }

        SECTION ("Control: OUI")
        {
            TestControlClassId1 packet_in;
            CHECK(packet_in.class_id().oui() == 0xAABBCC);
            data = TestControlClassId1::helper::pack(packet_in);
            CLASS_ID_BE = bytes{ 0, 0xAA, 0xBB, 0xCC, 0, 0, 0, 0 };

            TestControlClassId1 packet_out;
            CHECK(packet_out.class_id().oui() == 0xAABBCC);
            check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            check_ptr += CIF_BYTES;
        }

        SECTION("Control: Packet Code")
        {
            TestControlClassId2 packet_in;
            CHECK(packet_in.class_id().packet_code() == 0x1234);
            data = TestControlClassId2::helper::pack(packet_in);
            CLASS_ID_BE = bytes{ 0, 0, 0, 0, 0, 0, 0x12, 0x34 };

            TestControlClassId2 packet_out;
            CHECK(packet_out.class_id().packet_code() == 0x1234);
            check_ptr = data.data();
            check_ptr += HEADER_BYTES;
            check_ptr += CIF_BYTES;
        }

        // Examine and check packed Class ID. Value shall be in big-endian format.
        const decltype(data) packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
        check_ptr += CLASS_ID_BYTES;
        CHECK(packed_class_id == CLASS_ID_BE);
    }
}