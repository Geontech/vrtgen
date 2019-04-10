#include <vrtgen/enums.hpp>
#include <vrtgen/packing/cif0.hpp>
#include <vrtgen/packing/cif1.hpp>
#include <vrtgen/packing/header.hpp>
#include <vrtgen/packing/trailer.hpp>

#include <iostream>

#include <arpa/inet.h>

int main(int argc, const char* argv[])
{
    uint8_t data[] = {
        0x10, 0x5c, 0x12, 0x34,
        0x12, 0x34, 0x56, 0x78,
    };
    uint32_t* ptr = reinterpret_cast<uint32_t*>(data);
    using vrtgen::packing::Header;
    Header& header = *reinterpret_cast<Header*>(ptr);
    ++ptr;
    std::cout << "Packet type: " << header.getPacketType() << std::endl;
    std::cout << "Has Class Identifier: " << header.getClassIdentifierEnable() << std::endl;
    std::cout << "TSI: " << header.getTSI() << std::endl;
    std::cout << "TSF: " << header.getTSF() << std::endl;
    std::cout << "Packet count: " << (uint32_t) header.getPacketCount() << std::endl;
    std::cout << "Packet size: " << std::hex << (uint32_t) header.getPacketSize() << std::endl;
    switch (header.getPacketType()) {
    case vrtgen::PacketType::SIGNAL_DATA:
    case vrtgen::PacketType::EXTENSION_DATA:
        break;
    default:
        std::cout << "Stream ID: " << ntohl(*ptr) /*vrtgen::get_int(*ptr, 31, 32)*/ << std::endl;
        ++ptr;
        break;
    }
    return 0;
}
