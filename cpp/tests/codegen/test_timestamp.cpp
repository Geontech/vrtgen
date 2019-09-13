#include "catch/catch.hpp"

#include "timestamp.hpp"

#include <vector>

TEST_CASE("UTC picosecond-resolution timestamp")
{
    TimestampUTCRealTime packet_in;
    packet_in.setIntegerTimestamp(10000000);
    packet_in.setFractionalTimestamp(999999999999);

    size_t packed_size = packing::TimestampUTCRealTimeHelper::bytes_required(packet_in);
    CHECK(packed_size == 24);

    std::vector<unsigned char> data;
    data.resize(24);
    packing::TimestampUTCRealTimeHelper::pack(packet_in, data.data(), data.size());

    const vrtgen::packing::Header* header = reinterpret_cast<const vrtgen::packing::Header*>(data.data());
    CHECK(header->getTSI() == vrtgen::TSI::UTC);
    CHECK(header->getTSF() == vrtgen::TSF::REAL_TIME);

    CHECK(packing::TimestampUTCRealTimeHelper::match(data.data(), data.size()));

    TimestampUTCRealTime packet_out;
    packing::TimestampUTCRealTimeHelper::unpack(packet_out, data.data(), data.size());
    CHECK(packet_out.getIntegerTimestamp() == 10000000);
    CHECK(packet_out.getFractionalTimestamp() == 999999999999);
}

TEST_CASE("GPS seconds timestamp")
{
    TimestampGPS packet_in;
    packet_in.setIntegerTimestamp(12345678);
 
    size_t packed_size = packing::TimestampGPSHelper::bytes_required(packet_in);
    CHECK(packed_size == 16);

    std::vector<unsigned char> data;
    data.resize(16);
    packing::TimestampGPSHelper::pack(packet_in, data.data(), data.size());

    const vrtgen::packing::Header* header = reinterpret_cast<const vrtgen::packing::Header*>(data.data());
    CHECK(header->getTSI() == vrtgen::TSI::GPS);
    CHECK(header->getTSF() == vrtgen::TSF::NONE);

    CHECK(packing::TimestampGPSHelper::match(data.data(), data.size()));

    TimestampGPS packet_out;
    packing::TimestampGPSHelper::unpack(packet_out, data.data(), data.size());
    CHECK(packet_out.getIntegerTimestamp() == 12345678);
}

TEST_CASE("Free-running fractional timestamp")
{
    TimestampFreeRunning packet_in;
    packet_in.setFractionalTimestamp(0x1E1E1E1E1E1E1E1E);

    size_t packed_size = packing::TimestampFreeRunningHelper::bytes_required(packet_in);
    CHECK(packed_size == 20);

    std::vector<unsigned char> data;
    data.resize(20);
    packing::TimestampFreeRunningHelper::pack(packet_in, data.data(), data.size());

    const vrtgen::packing::Header* header = reinterpret_cast<const vrtgen::packing::Header*>(data.data());
    CHECK(header->getTSI() == vrtgen::TSI::NONE);
    CHECK(header->getTSF() == vrtgen::TSF::FREE_RUNNING);

    CHECK(packing::TimestampFreeRunningHelper::match(data.data(), data.size()));

    TimestampFreeRunning packet_out;
    packing::TimestampFreeRunningHelper::unpack(packet_out, data.data(), data.size());
    CHECK(packet_out.getFractionalTimestamp() == 0x1E1E1E1E1E1E1E1E);
}
