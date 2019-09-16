/*
 * Copyright (C) 2019 Geon Technologies, LLC
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

#include "bytes.hpp"

using vrtgen::packed;
using vrtgen::packed_tag;

TEST_CASE("32-bit packed", "[pack]")
{
    bytes data = { 0, 0, 0, 0 };
    packed<uint32_t>& value = *reinterpret_cast<packed<uint32_t>*>(data.data());

    SECTION("1-bit boolean (31)") {
        typedef packed_tag<bool,31,1> tag_type;
        SECTION("get") {
            data[0] = 0x80;
            CHECK(value.get(tag_type()));
        }
        SECTION("set") {
            value.set(true, tag_type());
            CHECK(data == bytes({ 0x80, 0x00, 0x00, 0x00 }));
        }
    }

    SECTION("1-bit boolean (14)") {
        typedef packed_tag<bool,14,1> tag_type;
        SECTION("get") {
            data[2] = 0x40;
            CHECK(value.get(tag_type()));
        }
        SECTION("set") {
            data = { 0xFF, 0xFF, 0xFF, 0xFF };
            value.set(false, tag_type());
            CHECK(data == bytes({ 0xFF, 0xFF, 0xBF, 0xFF }));
        }
    }

    SECTION("1-bit boolean (0)") {
        typedef packed_tag<bool,0,1> tag_type;
        SECTION("get") {
            data[3] = 0x01;
            CHECK(value.get(tag_type()));
        }
        SECTION("set") {
            value.set(true, tag_type());
            CHECK(data == bytes({ 0x00, 0x00, 0x00, 0x01 }));
        }
    }

    SECTION("4-bit integer (31)") {
        typedef packed_tag<unsigned,31,4> tag_type;
        SECTION("get") {
            data[0] = 0xEF; // low nibble part of a different field
            CHECK(value.get(tag_type()) == 14);
        }
        SECTION("set") {
            value.set(9, tag_type());
            CHECK(data == bytes({ 0x90, 0x00, 0x00, 0x00 }));
        }
    }

    SECTION("7-bit integer (6)") {
        typedef packed_tag<unsigned,6,7> tag_type;
        SECTION("get") {
            data[3] = 0xF1; // high bit part of a different field
            CHECK(value.get(tag_type()) == 0x71);
        }
        SECTION("set") {
            value.set(0x41, tag_type());
            CHECK(data == bytes({ 0x00, 0x00, 0x00, 0x41 }));
        }
    }

    SECTION("12-bit integer (23)") {
        typedef packed_tag<unsigned,23,12> tag_type;
        SECTION("get") {
            data[1] = 0x95;
            data[2] = 0x2F; // low nibble part of a different field
            CHECK(value.get(tag_type()) == 0x952);
        }
        SECTION("set") {
            value.set(0xABC, tag_type());
            CHECK(data == bytes({ 0x00, 0xAB, 0xC0, 0x00 }));
        }
    }
}

TEST_CASE("16-bit packed", "[pack]")
{
    bytes data = { 0, 0 };
    packed<uint16_t>& value = *reinterpret_cast<packed<uint16_t>*>(data.data());

    SECTION("1-bit boolean (15)") {
        typedef packed_tag<bool,15,1> tag_type;
        SECTION("get") {
            data[0] = 0x80;
            CHECK(value.get(tag_type()));
        }
        SECTION("set") {
            value.set(true, tag_type());
            CHECK(data == bytes({ 0x80, 0x00 }));
        }
    }

    SECTION("1-bit boolean (9)") {
        typedef packed_tag<bool,9,1> tag_type;
        SECTION("get") {
            data[0] = 0x02;
            CHECK(value.get(tag_type()));
        }
        SECTION("set") {
            data = { 0xFF, 0xFF };
            value.set(false, tag_type());
            CHECK(data == bytes({ 0xFD, 0xFF }));
        }
    }

    SECTION("1-bit boolean (0)") {
        typedef packed_tag<bool,0,1> tag_type;
        SECTION("get") {
            data[1] = 0x01;
            CHECK(value.get(tag_type()));
        }
        SECTION("set") {
            value.set(true, tag_type());
            CHECK(data == bytes({ 0x00, 0x01 }));
        }
    }

    SECTION("7-bit integer (6)") {
        typedef packed_tag<unsigned,6,7> tag_type;
        SECTION("get") {
            data[1] = 0xF1; // high bit part of a different field
            CHECK(value.get(tag_type()) == 0x71);
        }
        SECTION("set") {
            value.set(0x41, tag_type());
            CHECK(data == bytes({ 0x00, 0x41 }));
        }
    }
}

TEST_CASE("8-bit packed", "[pack]")
{
    bytes data = { 0 };
    packed<uint8_t>& value = *reinterpret_cast<packed<uint8_t>*>(data.data());

    SECTION("1-bit boolean (7)") {
        typedef packed_tag<bool,7,1> tag_type;
        SECTION("get") {
            data[0] = 0x80;
            CHECK(value.get(tag_type()));
        }
        SECTION("set") {
            value.set(true, tag_type());
            CHECK(data == bytes({ 0x80 }));
        }
    }

    SECTION("1-bit boolean (4)") {
        typedef packed_tag<bool,4,1> tag_type;
        SECTION("get") {
            data[0] = 0x10;
            CHECK(value.get(tag_type()));
        }
        SECTION("set") {
            data = { 0xFF };
            value.set(false, tag_type());
            CHECK(data == bytes({ 0xEF }));
        }
    }

    SECTION("1-bit boolean (0)") {
        typedef packed_tag<bool,0,1> tag_type;
        SECTION("get") {
            data[0] = 0x01;
            CHECK(value.get(tag_type()));
        }
        SECTION("set") {
            value.set(true, tag_type());
            CHECK(data == bytes({ 0x01 }));
        }
    }

    SECTION("4-bit integer (7)") {
        typedef packed_tag<unsigned,7,4> tag_type;
        SECTION("get") {
            data[0] = 0xEF; // low nibble part of a different field
            CHECK(value.get(tag_type()) == 14);
        }
        SECTION("set") {
            value.set(9, tag_type());
            CHECK(data == bytes({ 0x90 }));
        }
    }
}

TEST_CASE("Packed special cases", "[pack]")
{
    bytes data = { 0, 0 };
    packed<uint16_t>& value = *reinterpret_cast<packed<uint16_t>*>(data.data());

    SECTION("Set 2-bit boolean") {
        typedef packed_tag<bool,7,2> tag_type;
        value.set(true, tag_type());
        CHECK(data == bytes({ 0x00, 0xC0 }));
    }

    SECTION("Signed fields") {
        typedef packed_tag<signed,12,5> tag_type;
        SECTION("get") {
            data[0] = 0x10;
            CHECK(value.get(tag_type()) == -16);
        }
        SECTION("set") {
            value.set(-1, tag_type());
            CHECK(data == bytes({ 0x1F, 0x00 }));
        }
    }

    SECTION("Truncation") {
        typedef packed_tag<unsigned,3,4> tag_type;
        value.set(31, tag_type());
        CHECK(data == bytes({ 0x00, 0x0F }));
    }
}