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
        typedef packed_tag<uint8_t,31,4> tag_type;
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
        typedef packed_tag<uint8_t,6,7> tag_type;
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
        typedef packed_tag<uint16_t,23,12> tag_type;
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
