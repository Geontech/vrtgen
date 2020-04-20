/*
 * Copyright (C) 2020 Geon Technologies, LLC
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

TEST_CASE("optional move operator", "[optional]")
{
    vrtgen::optional<int> src(1);

    vrtgen::optional<int> dest = std::move(src);
    CHECK(!src);
    REQUIRE(static_cast<bool>(dest));
    CHECK(dest.get() == 1);
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
