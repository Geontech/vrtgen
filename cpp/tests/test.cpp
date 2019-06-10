
#include <catch/catch.hpp>

#include <vrtgen/enums.hpp>
#include <vrtgen/packing/cif0.hpp>
#include <vrtgen/packing/cif1.hpp>
#include <vrtgen/packing/header.hpp>
#include <vrtgen/packing/trailer.hpp>

namespace Catch {
    template <size_t N>
    struct StringMaker< std::array<uint8_t, N> >
    {
        static std::string convert(const std::array<uint8_t, N>& value)
        {
            std::ostringstream oss;
            oss << "0x" << std::hex << std::setfill('0') << std::setiosflags(std::ios::uppercase);
            for (auto item : value) {
                oss << std::setw(2) << static_cast<unsigned>(item);
            }
            return oss.str();
        }
    };
}

TEST_CASE("Header getter methods")
{
    using vrtgen::packing::Header;

    std::array<uint8_t,4> data = {
        0x10, 0x5c, 0x12, 0x34,
    };
    Header& header = *reinterpret_cast<Header*>(data.data());
    SECTION("Packet Type") {
        REQUIRE(header.getPacketType() == vrtgen::PacketType::SIGNAL_DATA_STREAM_ID);
    }
    SECTION("Class Identifier Enable") {
        REQUIRE(header.isClassIdentifierEnabled() == false);
    }
    SECTION("TSI") {
        REQUIRE(header.getTSI() == vrtgen::TSI::UTC);
    }
    SECTION("TSF") {
        REQUIRE(header.getTSF() == vrtgen::TSF::SAMPLE_COUNT);
    }
    SECTION("Packet Count") {
        REQUIRE(header.getPacketCount() == 12);
    }
    SECTION("Packet Size") {
        REQUIRE(header.getPacketSize() == 0x1234);
    }
}

TEST_CASE("Header setter methods")
{
    using vrtgen::packing::Header;

    std::array<uint8_t,4> data = {}; // zero-initialize
    std::array<uint8_t,4> expected;
    Header& header = *reinterpret_cast<Header*>(data.data());

    SECTION("Packet Type") {
        expected = { 0x40, 0x00, 0x00, 0x00 };
        header.setPacketType(vrtgen::PacketType::CONTEXT);
        REQUIRE(data == expected);
    }
    SECTION("Class Identifier Enable") {
        expected = { 0x08, 0x00, 0x00, 0x00 };
        header.setClassIdentifierEnabled(true);
        REQUIRE(data == expected);
    }
    SECTION("TSI") {
        expected = { 0x00, 0x80, 0x00, 0x00 };
        header.setTSI(vrtgen::TSI::GPS);
        REQUIRE(data == expected);
    }
    SECTION("TSF") {
        expected = { 0x00, 0x20, 0x00, 0x00 };
        header.setTSF(vrtgen::TSF::REAL_TIME);
        REQUIRE(data == expected);
    }
    SECTION("Packet Count") {
        expected = { 0x00, 0x0C, 0x00, 0x00 };
        header.setPacketCount(12);
        REQUIRE(data == expected);
    }
    SECTION("Packet Size") {
        expected = { 0x00, 0x00, 0xCD, 0xEF };
        header.setPacketSize(0xCDEF);
        REQUIRE(data == expected);
    }
}

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
