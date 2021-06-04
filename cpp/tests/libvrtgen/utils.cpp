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

#include "catch/catch.hpp"

#include "vrtgen/types.hpp"

TEST_CASE("swap16", "[swap]")
{
    int16_t swapped = vrtgen::swap16(0xBA5E);
    REQUIRE(swapped == 0x5EBA);
    REQUIRE(vrtgen::swap16(swapped) == 0xBA5E);
}

TEST_CASE("swap24", "[swap]")
{
    int32_t swapped = vrtgen::swap24(0xBADC0D);
    REQUIRE(swapped == 0x0DDCBA);
    REQUIRE(vrtgen::swap24(swapped) == 0xBADC0D);
}

TEST_CASE("swap32", "[swap]")
{
    int32_t swapped = vrtgen::swap32(0xDEADBEEF);
    REQUIRE(swapped == 0xEFBEADDE);
    REQUIRE(vrtgen::swap32(swapped) == 0xDEADBEEF);
}

TEST_CASE("swap64", "[swap]")
{
    int64_t swapped = vrtgen::swap64(0x0123456789ABCDEF);
    REQUIRE(swapped == 0xEFCDAB8967452301);
    REQUIRE(vrtgen::swap64(swapped) == 0x0123456789ABCDEF);
}

TEST_CASE("Q9.7 fixed-point conversion", "[fixed]")
{
    typedef vrtgen::detail::fixed_converter<int16_t,7,float> fixed_type;

    // Check radix point
    CHECK(fixed_type::to_int(1.0) == 0x0080);
    CHECK(fixed_type::from_int(0x0080) == 1.0);

    // Maximum positive integer
    CHECK(fixed_type::to_int(255.0) == 0x7F80);
    CHECK(fixed_type::from_int(0x7F80) == 255.0);

    // Minimum negative integer
    CHECK(fixed_type::to_int(-256.0) == int16_t(0x8000));
    CHECK(fixed_type::from_int(int16_t(0x8000)) == -256.0);

    // Smallest positive fraction
    CHECK(fixed_type::to_int(1.0/128.0) == 0x0001);
    CHECK(fixed_type::from_int(0x0001) == 1.0/128.0);

    // Maximum positive value
    CHECK(fixed_type::to_int(256.0 - 1.0/128.0) == 0x7FFF);
    CHECK(fixed_type::from_int(0x7FFF) == 256.0 - 1.0/128.0);

    // Smallest negative fraction (binary rep is all 1s)
    CHECK(fixed_type::to_int(-1.0/128.0) == int16_t(0xFFFF));
    CHECK(fixed_type::from_int(int16_t(0xFFFF)) == -1.0/128.0);
}

TEST_CASE("UQ27.5 fixed-point conversion", "[fixed]")
{
    typedef vrtgen::detail::fixed_converter<uint32_t,5,double> fixed_type;

    // Check radix point
    CHECK(fixed_type::to_int(1.0) == 0x00000020);
    CHECK(fixed_type::from_int(0x00000020) == 1.0);

    // Maximum positive integer
    CHECK(fixed_type::to_int(134217727.0) == 0xFFFFFFE0);
    CHECK(fixed_type::from_int(0xFFFFFFE0) == 134217727.0);

    // Smallest fraction
    CHECK(fixed_type::to_int(1.0/32.0) == 0x00000001);
    CHECK(fixed_type::from_int(0x00000001) == 1.0/32.0);

    // Maximum positive value
    CHECK(fixed_type::to_int(134217728.0 - 1.0/32.0) == 0xFFFFFFFF);
    CHECK(fixed_type::from_int(0xFFFFFFFF) == (134217728.0 - 1.0/32.0));
}

TEST_CASE("Q16.16 fixed-point conversion", "[fixed]")
{
    typedef vrtgen::detail::fixed_converter<int32_t,16,double> fixed_type;

    // Check radix point
    CHECK(fixed_type::to_int(1.0) == 0x00010000);
    CHECK(fixed_type::from_int(0x00010000) == 1.0);

    // Maximum positive integer
    CHECK(fixed_type::to_int(32767.0) == 0x7FFF0000);
    CHECK(fixed_type::from_int(0x7FFF0000) == 32767.0);

    // Minimum negative integer
    CHECK(fixed_type::to_int(-32768.0) == int32_t(0x80000000));
    CHECK(fixed_type::from_int(0x80000000) == -32768.0);

    // Smallest positive fraction
    CHECK(fixed_type::to_int(0.00001526) == 0x00000001);
    CHECK(fixed_type::from_int(0x00000001) == 0.00001526);

    // Maximum positive value
    // NOTE: If a float (32 bits) is used as the nearest data type, it does
    // not have enough mantissa precision to represent this value. The first
    // test fails, while the second passes with approximate equality.
    CHECK(fixed_type::to_int(32768.0 - 0.00001526) == int32_t(0x7FFFFFFF));
    CHECK(fixed_type::from_int(0x7FFFFFFF) == 32768.0 - 0.00001526);

    // Smallest negative fraction (binary rep is all 1s)
    CHECK(fixed_type::to_int(-0.00001526) == int32_t(0xFFFFFFFF));
    CHECK(fixed_type::from_int(0xFFFFFFFF) == -0.00001526);
}

TEST_CASE("Optional value")
{
    vrtgen::optional<uint8_t> value;

    CHECK_FALSE(value);
    value.set(123);
    CHECK(value);
    CHECK(value.get() == 123);

    value.clear();
    CHECK_FALSE(value);
}