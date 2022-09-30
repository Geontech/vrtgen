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

#include <cmath>
#include <limits>
#include <utility>
#include "catch.hpp"
#include "bytes.hpp"
#include "vrtgen/types.hpp"

/*
 * Fixed Point tests
 */
TEST_CASE("Q9.7 fixed-point conversion", "[fixed]")
{
    using int_type = int16_t;
    using float_type = float;
    constexpr std::size_t BITS = 16;
    constexpr std::size_t RADIX = 7;

    SECTION("Radix")
    {
        auto int_val = static_cast<int_type>(0x0080);
        auto float_val = static_cast<float_type>(1.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Negative one")
    {
        auto int_val = static_cast<int_type>(0xFF80);
        auto float_val = static_cast<float_type>(-1.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest positive integer")
    {
        auto int_val = static_cast<int_type>(0x7F80);
        auto float_val = static_cast<float_type>(255.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest negative integer")
    {
        auto int_val = static_cast<int_type>(0x8000);
        auto float_val = static_cast<float_type>(-256.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest positive value")
    {
        auto int_val = static_cast<int_type>(0x7FFF);
        auto float_val = static_cast<float_type>(256.0 - 1.0 / 128.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest positive fraction")
    {
        auto int_val = static_cast<int_type>(0x007F);
        auto float_val = static_cast<float_type>(1.0 - 1.0 / 128.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Smallest positive fraction")
    {
        auto int_val = static_cast<int_type>(0x0001);
        auto float_val = static_cast<float_type>(1.0 / 128.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest negative fraction")
    {
        auto int_val = static_cast<int_type>(0xFF81);
        auto float_val = static_cast<float_type>(-1.0 + 1.0 / 128.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Smallest negative fraction")
    {
        auto int_val = static_cast<int_type>(0xFFFF);
        auto float_val = static_cast<float_type>(-1.0 / 128.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
}

TEST_CASE("Q10.22 fixed-point conversion", "[fixed]")
{
    using int_type = int32_t;
    using float_type = double;
    constexpr std::size_t BITS = 32;
    constexpr std::size_t RADIX = 22;

    SECTION("Radix")
    {
        auto int_val = static_cast<int_type>(0x00400000);
        auto float_val = static_cast<float_type>(1.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Negative one")
    {
        auto int_val = static_cast<int_type>(0xFFC00000);
        auto float_val = static_cast<float_type>(-1.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest positive integer")
    {
        auto int_val = static_cast<int_type>(0x7FC00000);
        auto float_val = static_cast<float_type>(511.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest negative integer")
    {
        auto int_val = static_cast<int_type>(0x80000000);
        auto float_val = static_cast<float_type>(-512.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest positive value")
    {
        auto int_val = static_cast<int_type>(0x7FFFFFFF);
        auto float_val = static_cast<float_type>(512.0 - 1.0 / 4194304.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest positive fraction")
    {
        auto int_val = static_cast<int_type>(0x003FFFFF);
        auto float_val = static_cast<float_type>(1.0 - 1.0 / 4194304.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Smallest positive fraction")
    {
        auto int_val = static_cast<int_type>(0x00000001);
        auto float_val = static_cast<float_type>(1.0 / 4194304.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest negative fraction")
    {
        auto int_val = static_cast<int_type>(0xFFC00001);
        auto float_val = static_cast<float_type>(-1.0 + 1.0 / 4194304.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Smallest negative fraction")
    {
        auto int_val = static_cast<int_type>(0xFFFFFFFF);
        auto float_val = static_cast<float_type>(-1.0 / 4194304.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
}

TEST_CASE("Q27.5 fixed-point conversion", "[fixed]")
{
    using int_type = int32_t;
    using float_type = double;
    constexpr std::size_t BITS = 32;
    constexpr std::size_t RADIX = 5;

    SECTION("Radix")
    {
        auto int_val = static_cast<int_type>(0x00000020);
        auto float_val = static_cast<float_type>(1.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Negative one")
    {
        auto int_val = static_cast<int_type>(0xFFFFFFE0);
        auto float_val = static_cast<float_type>(-1.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest positive integer")
    {
        auto int_val = static_cast<int_type>(0x7FFFFFE0);
        auto float_val = static_cast<float_type>(67108863.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest negative integer")
    {
        auto int_val = static_cast<int_type>(0x80000000);
        auto float_val = static_cast<float_type>(-67108864.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest positive value")
    {
        auto int_val = static_cast<int_type>(0x7FFFFFFF);
        auto float_val = static_cast<float_type>(67108864.0 - 1.0 / 32.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest positive fraction")
    {
        auto int_val = static_cast<int_type>(0x0000001F);
        auto float_val = static_cast<float_type>(1.0 - 1.0 / 32.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Smallest positive fraction")
    {
        auto int_val = static_cast<int_type>(0x00000001);
        auto float_val = static_cast<float_type>(1.0 / 32.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest negative fraction")
    {
        auto int_val = static_cast<int_type>(0xFFFFFFE1);
        auto float_val = static_cast<float_type>(-1.0 + 1.0 / 32.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Smallest negative fraction")
    {
        auto int_val = static_cast<int_type>(0xFFFFFFFF);
        auto float_val = static_cast<float_type>(-1.0 / 32.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
}

TEST_CASE("Q16.16 fixed-point conversion", "[fixed]")
{
    using int_type = int32_t;
    using float_type = double;
    constexpr std::size_t BITS = 32;
    constexpr std::size_t RADIX = 16;

    SECTION("Radix")
    {
        auto int_val = static_cast<int_type>(0x00010000);
        auto float_val = static_cast<float_type>(1.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Negative one")
    {
        auto int_val = static_cast<int_type>(0xFFFF0000);
        auto float_val = static_cast<float_type>(-1.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest positive integer")
    {
        auto int_val = static_cast<int_type>(0x7FFF0000);
        auto float_val = static_cast<float_type>(32767.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest negative integer")
    {
        auto int_val = static_cast<int_type>(0x80000000);
        auto float_val = static_cast<float_type>(-32768.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest positive value")
    {
        auto int_val = static_cast<int_type>(0x7FFFFFFF);
        auto float_val = static_cast<float_type>(32768.0 - 1.0 / 65536.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest positive fraction")
    {
        auto int_val = static_cast<int_type>(0x0000FFFF);
        auto float_val = static_cast<float_type>(1.0 - 1.0 / 65536.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Smallest positive fraction")
    {
        auto int_val = static_cast<int_type>(0x00000001);
        auto float_val = static_cast<float_type>(1.0 / 65536.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest negative fraction")
    {
        auto int_val = static_cast<int_type>(0xFFFF0001);
        auto float_val = static_cast<float_type>(-1.0 + 1.0 / 65536.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Smallest negative fraction")
    {
        auto int_val = static_cast<int_type>(0xFFFFFFFF);
        auto float_val = static_cast<float_type>(-1.0 / 65536.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
}

TEST_CASE("Q44.20 fixed-point conversion", "[fixed]")
{
    using int_type = int64_t;
    using float_type = long double;
    constexpr std::size_t BITS = 64;
    constexpr std::size_t RADIX = 20;

    SECTION("Radix")
    {
        auto int_val = static_cast<int_type>(0x0000000000100000);
        auto float_val = static_cast<float_type>(1.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Negative one")
    {
        auto int_val = static_cast<int_type>(0xFFFFFFFFFFF00000);
        auto float_val = static_cast<float_type>(-1.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest positive integer")
    {
        auto int_val = static_cast<int_type>(0x7FFFFFFFFFF00000);
        auto float_val = static_cast<float_type>(8796093022207.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest negative integer")
    {
        auto int_val = static_cast<int_type>(0x8000000000000000);
        auto float_val = static_cast<float_type>(-8796093022208.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest positive value")
    {
        auto int_val = static_cast<int_type>(0x7FFFFFFFFFFFFFFF);
        auto float_val = static_cast<float_type>(8796093022208.0) - (1.0 / 1048576.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest positive fraction")
    {
        auto int_val = static_cast<int_type>(0x00000000000FFFFF);
        auto float_val = static_cast<float_type>(1.0 - 1.0 / 1048576.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Smallest positive fraction")
    {
        auto int_val = static_cast<int_type>(0x1);
        auto float_val = static_cast<float_type>(1.0 / 1048576.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Largest negative fraction")
    {
        auto int_val = static_cast<int_type>(0xFFFFFFFFFFF00001);
        auto float_val = static_cast<float_type>(-1.0 + 1.0 / 1048576.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
    SECTION("Smallest negative fraction")
    {
        auto int_val = static_cast<int_type>(0xFFFFFFFFFFFFFFFF);
        auto float_val = static_cast<float_type>(-1.0 / 1048576.0);
        CHECK(vrtgen::fixed::to_int<BITS,RADIX>(float_val) == int_val);
        CHECK(vrtgen::fixed::to_fp<BITS,RADIX>(int_val) == float_val);
    }
}

/*
 * Swapping tests
 */
TEST_CASE("To Big Endian", "[swap]")
{
    SECTION("8")
    {
        uint8_t value = 0x12;
        CHECK(vrtgen::swap::to_be(value) == value);
    }

    SECTION("16")
    {
        uint16_t value = 0x12;
        CHECK(vrtgen::swap::to_be(value) == 0x1200);
        value = 0x1234;
        CHECK(vrtgen::swap::to_be(value) == 0x3412);
    }

    SECTION("32")
    {
        uint32_t value = 0x12;
        CHECK(vrtgen::swap::to_be(value) == 0x12000000);
        value = 0x1234;
        CHECK(vrtgen::swap::to_be(value) == 0x34120000);
        value = 0x123456;
        CHECK(vrtgen::swap::to_be(value) == 0x56341200);
        value = 0x12345678;
        CHECK(vrtgen::swap::to_be(value) == 0x78563412);
    }

    SECTION("64")
    {
        uint64_t value = 0x12;
        CHECK(vrtgen::swap::to_be(value) == 0x1200000000000000);
        value = 0x1234;
        CHECK(vrtgen::swap::to_be(value) == 0x3412000000000000);
        value = 0x123456;
        CHECK(vrtgen::swap::to_be(value) == 0x5634120000000000);
        value = 0x12345678;
        CHECK(vrtgen::swap::to_be(value) == 0x7856341200000000);
        value = 0x1234567890;
        CHECK(vrtgen::swap::to_be(value) == 0x9078563412000000);
        value = 0x1234567890AB;
        CHECK(vrtgen::swap::to_be(value) == 0xAB90785634120000);
        value = 0x1234567890ABCD;
        CHECK(vrtgen::swap::to_be(value) == 0xCDAB907856341200);
        value = 0x1234567890ABCDEF;
        CHECK(vrtgen::swap::to_be(value) == 0xEFCDAB9078563412);
    }
}

TEST_CASE("From Big Endian", "[swap]")
{
    SECTION("8")
    {
        uint8_t value = 0x12;
        CHECK(vrtgen::swap::from_be(value) == value);
    }

    SECTION("16")
    {
        uint16_t value = 0x1200;
        CHECK(vrtgen::swap::from_be(value) == 0x12);
        value = 0x3412;
        CHECK(vrtgen::swap::from_be(value) == 0x1234);
    }

    SECTION("32")
    {
        uint32_t value = 0x12000000;
        CHECK(vrtgen::swap::from_be(value) == 0x12);
        value = 0x34120000;
        CHECK(vrtgen::swap::from_be(value) == 0x1234);
        value = 0x56341200;
        CHECK(vrtgen::swap::from_be(value) == 0x123456);
        value = 0x78563412;
        CHECK(vrtgen::swap::from_be(value) == 0x12345678);
    }

    SECTION("64")
    {
        uint64_t value = 0x1200000000000000;
        CHECK(vrtgen::swap::from_be(value) == 0x12);
        value = 0x3412000000000000;
        CHECK(vrtgen::swap::from_be(value) == 0x1234);
        value = 0x5634120000000000;
        CHECK(vrtgen::swap::from_be(value) == 0x123456);
        value = 0x7856341200000000;
        CHECK(vrtgen::swap::from_be(value) == 0x12345678);
        value = 0x9078563412000000;
        CHECK(vrtgen::swap::from_be(value) == 0x1234567890);
        value = 0xAB90785634120000;
        CHECK(vrtgen::swap::from_be(value) == 0x1234567890AB);
        value = 0xCDAB907856341200;
        CHECK(vrtgen::swap::from_be(value) == 0x1234567890ABCD);
        value = 0xEFCDAB9078563412;
        CHECK(vrtgen::swap::from_be(value) == 0x1234567890ABCDEF);
    }
}

/*
 * OUI Test
 */
TEST_CASE("OUI", "[oui]")
{
    using namespace vrtgen;
    OUI oui;
    OUI unpack_oui;
    bytes packed_bytes{ 0xFF, 0xFF, 0xFF };

    // Verify zero on construction
    oui.pack_into(packed_bytes.data());
    CHECK(packed_bytes == bytes{ 0, 0, 0 });
    CHECK(oui.get() == 0);
    // Setter
    oui.set(0xABCDEF);
    // Getter check set value
    CHECK(oui.get() == 0xABCDEF);
    // Verify to_string
    CHECK(oui.to_string() == "AB-CD-EF");
    // Pack
    oui.pack_into(packed_bytes.data());
    // Verify packed bits
    CHECK(packed_bytes == bytes{ 0xAB, 0xCD, 0xEF });
    // Unpack
    unpack_oui.unpack_from(packed_bytes.data());
    // Verify unpacked value
    CHECK(unpack_oui.get() == 0xABCDEF);
}

/*
 * UUID Test
 */
TEST_CASE("UUID", "[uuid]")
{
    using namespace vrtgen;
    UUID uuid;
    UUID unpack_uuid;
    bytes packed_bytes{
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF
    };

    // Verify zero on construction
    uuid.pack_into(packed_bytes.data());
    bytes check{
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    CHECK(packed_bytes == check);
    CHECK(uuid.get() == "00000000-0000-0000-0000-000000000000");
    // Setter
    uuid.set("12345678-abcd-4321-fedc-abc123456789");
    // Getter check set value
    CHECK(uuid.get() == "12345678-abcd-4321-fedc-abc123456789");
    // Pack
    uuid.pack_into(packed_bytes.data());
    // Verify packed bits
    check = bytes{
        0x12, 0x34, 0x56, 0x78,
        0xAB, 0xCD, 0x43, 0x21,
        0xFE, 0xDC, 0xAB, 0xC1,
        0x23, 0x45, 0x67, 0x89
    };
    CHECK(packed_bytes == check);
    // Unpack
    unpack_uuid.unpack_from(packed_bytes.data());
    // Verify unpacked value
    CHECK(unpack_uuid.get() == "12345678-abcd-4321-fedc-abc123456789");
}