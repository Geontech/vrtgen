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

TEST_CASE("optional copy ctor", "[optional]")
{
    // Copy src to dest and verify that they have different underlying pointers
    // by changing src's value.
    vrtgen::optional<char> src('A');
    vrtgen::optional<char> dest(src);
    src.set('B');

    REQUIRE(src);
    CHECK(src.get() == 'B');
    REQUIRE(dest);
    CHECK(dest.get() == 'A');
}

TEST_CASE("optional move ctor", "[optional]")
{
    // Explicitly move src to dest, which should leave it empty.
    vrtgen::optional<int> src(1);
    vrtgen::optional<int> dest(std::move(src));

    CHECK(!src);
    REQUIRE(dest);
    CHECK(dest.get() == 1);
}

TEST_CASE("optional copy assignment", "[optional]")
{
    // Create src and dest first, then assign src to dest and verify that they
    // have different underlying pointers by changing src's value.
    vrtgen::optional<int> src(-1);
    vrtgen::optional<int> dest;
    dest = src;
    src.set(-2);
    REQUIRE(src);
    CHECK(src.get() == -2);
    REQUIRE(dest);
    CHECK(dest.get() == -1);
}

TEST_CASE("optional move assignment", "[optional]")
{
    // Explicitly use move assignment and check that the source becomes empty.
    vrtgen::optional<int> src(20);
    vrtgen::optional<int> dest;
    dest = std::move(src);
    CHECK(!src);
    REQUIRE(dest);
    CHECK(dest.get() == 20);
}

TEST_CASE("optional class", "[optional]")
{
    vrtgen::optional<float> value;
    CHECK(!value);

    value.set(1.25);
    REQUIRE(static_cast<bool>(value));
    CHECK(value.get() == 1.25);

    value.clear();
    CHECK(!value);
}
