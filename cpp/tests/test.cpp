#include <vrtgen/enums.hpp>
#include <vrtgen/packing/cif0.hpp>
#include <vrtgen/packing/cif1.hpp>
#include <vrtgen/packing/header.hpp>
#include <vrtgen/packing/trailer.hpp>

#include <iostream>
#include <stdexcept>
#include <iomanip>

#include <arpa/inet.h>

class test_failure : public std::runtime_error
{
public:
    test_failure(const std::string& file, int line, const char* msg) :
        std::runtime_error(msg),
        _M_file(file),
        _M_line(line)
    {
    }

    virtual ~test_failure() throw()
    {
    }


    const std::string& file() const
    {
        return _M_file;
    }

    int line() const
    {
        return _M_line;
    }

private:
    const std::string _M_file;
    const int _M_line;
};

#define ASSERT_EQUAL(x,y)                               \
    if ((x) != (y)) {                                   \
        throw test_failure(__FILE__, __LINE__, #x " != " #y);     \
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
    ASSERT_EQUAL(data[0], 0x40);

    header.setClassIdentifierEnable(true);
    ASSERT_EQUAL(data[0], 0x48);

    ASSERT_EQUAL(data[1], 0);
    header.setTSI(vrtgen::TSI::GPS);
    ASSERT_EQUAL(data[1], 0x80);

    header.setTSF(vrtgen::TSF::REAL_TIME);
    ASSERT_EQUAL(data[1], 0xA0);

    header.setPacketCount(12);
    ASSERT_EQUAL(data[1], 0xAC);

    ASSERT_EQUAL(data[2], 0);
    ASSERT_EQUAL(data[3], 0);
    header.setPacketSize(0xCDEF);
    ASSERT_EQUAL(data[2], 0xCD);
    ASSERT_EQUAL(data[3], 0xEF);
}

#define RUN_TEST(x) std::cout << #x ": ";                               \
    try { x(); std::cout << "OK" << std::endl; }                        \
    catch (const test_failure& exc) {                                   \
        std::cout << "FAIL" << std::endl;                               \
        std::cerr << exc.file() << ":" << exc.line() << ": " << exc.what() << std::endl; \
    }

int main(int argc, const char* argv[])
{
    RUN_TEST(test_header_get);
    RUN_TEST(test_header_set);
    return 0;
}
