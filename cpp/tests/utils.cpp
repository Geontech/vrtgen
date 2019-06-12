#include "catch/catch.hpp"

#include "vrtgen/types.hpp"

TEST_CASE("swap16", "[swap]")
{
    int16_t swapped = vrtgen::swap16(0xBA5E);
    REQUIRE(swapped == 0x5EBA);
    REQUIRE(vrtgen::swap16(swapped) == 0xBA5E);
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
