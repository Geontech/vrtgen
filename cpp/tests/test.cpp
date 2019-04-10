#include <vrtgen/enums.hpp>
#include <vrtgen/packing/cif0.hpp>
#include <vrtgen/packing/cif1.hpp>
#include <vrtgen/packing/header.hpp>
#include <vrtgen/packing/trailer.hpp>

#include <iostream>
#include <stdexcept>

#include <arpa/inet.h>

#define ASSERT_EQUAL(x,y)                       \
    if ((x) != (y)) {                           \
        throw std::runtime_error(#x " != " #y); \
    }

void test_header_get()
{
    using vrtgen::packing::Header;

    uint8_t data[] = {
        0x10, 0x5c, 0x12, 0x34,
        0x12, 0x34, 0x56, 0x78,
    };
    Header& header = *reinterpret_cast<Header*>(data);
    ASSERT_EQUAL(header.getPacketType(), vrtgen::PacketType::SIGNAL_DATA_STREAM_ID);
    ASSERT_EQUAL(header.getClassIdentifierEnable(), false);
    ASSERT_EQUAL(header.getTSI(), vrtgen::TSI::UTC);
    ASSERT_EQUAL(header.getTSF(), vrtgen::TSF::SAMPLE_COUNT);
    ASSERT_EQUAL(header.getPacketCount(), 12);
    ASSERT_EQUAL(header.getPacketSize(), 0x1234);
}

void test_header_set()
{
    using vrtgen::packing::Header;

    uint8_t data[] = { 0, 0, 0, 0 };
    Header& header = *reinterpret_cast<Header*>(data);

    header.setPacketType(vrtgen::PacketType::CONTEXT);
    ASSERT_EQUAL(data[0] >> 4, 0x4);
}

#define RUN_TEST(x) std::cout << #x ": "; \
    try { x(); std::cout << "OK" << std::endl; }  \
    catch (const std::exception& exc) {           \
        std::cout << "FAIL" << std::endl;         \
        std::cerr << exc.what() << std::endl;     \
    }

int main(int argc, const char* argv[])
{
    RUN_TEST(test_header_get);
    RUN_TEST(test_header_set);
    return 0;
}
