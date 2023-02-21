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
#include "bytes.hpp"

#include "vrtgen/types/packed.hpp"
#include "vrtgen/packing/prologue.hpp"

using namespace vrtgen::packing;

TEST_CASE("ClassIdentifier", "[class_id]")
{
    ClassIdentifier class_id;
    ClassIdentifier unpack_class_id;
    bytes packed_bytes{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    SECTION("Zero on construction")
    {
        class_id.pack_into(packed_bytes.data());
        CHECK(packed_bytes == bytes{ 0, 0, 0, 0, 0, 0, 0, 0 });
    }

    SECTION("Rule 5.1.3-1")
    {
        // Verify zero on construction
        CHECK(class_id.oui() == 0);
        // Setter
        class_id.oui(0xABCDEF);
        // Getter check set value
        CHECK(class_id.oui() == 0xABCDEF);
        // Pack
        class_id.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(range(packed_bytes, 1, 4) == bytes{ 0xAB, 0xCD, 0xEF });
        // Unpack
        unpack_class_id.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_class_id.oui() == 0xABCDEF);
    }

    SECTION("Rule 5.1.3-2")
    {
        // Verify zero on construction
        CHECK(class_id.information_code() == 0);
        // Setter
        class_id.information_code(0x123);
        // Getter check set value
        CHECK(class_id.information_code() == 0x123);
        // Pack
        class_id.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(range(packed_bytes, 4, 6) == bytes{ 0x01, 0x23 });
        // Unpack
        unpack_class_id.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_class_id.information_code() == 0x123);
    }

    SECTION("Rule 5.1.3-3")
    {
        // Verify zero on construction
        CHECK(class_id.packet_code() == 0);
        // Setter
        class_id.packet_code(0x123);
        // Getter check set value
        CHECK(class_id.packet_code() == 0x123);
        // Pack
        class_id.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(range(packed_bytes, 6, 8) == bytes{ 0x01, 0x23 });
        // Unpack
        unpack_class_id.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_class_id.packet_code() == 0x123);
    }

    SECTION("Rule 5.1.3-4")
    {
        uint8_t pad_bit_count = 0b10101;
        int number_bits = 5;
        // Verify zero on construction
        CHECK(class_id.pad_bits() == 0);
        // Setter
        class_id.pad_bits(pad_bit_count);
        // Getter check set value
        CHECK(class_id.pad_bits() == pad_bit_count);
        // Pack
        class_id.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == pad_bit_count << (8-number_bits));
        // Unpack
        unpack_class_id.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_class_id.pad_bits() == pad_bit_count);
    }
}
