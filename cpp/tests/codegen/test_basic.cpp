#include "catch/catch.hpp"

#include "basic.hpp"

#include <vector>

TEST_CASE("Basic context message") {
    BasicContext packet_in;
    packet_in.setStreamIdentifier(0x12345678);
    packet_in.setReferencePointIdentifier(0x10AB7DE9);

    size_t packed_size = packing::BasicContextHelper::bytes_required(packet_in);
    CHECK(packed_size == 16);

    std::vector<unsigned char> data;
    data.resize(16);
    packing::BasicContextHelper::pack(packet_in, data.data(), data.size());

    const vrtgen::packing::ContextHeader* header = reinterpret_cast<const vrtgen::packing::ContextHeader*>(data.data());
    CHECK(header->getPacketType() == vrtgen::PacketType::CONTEXT);
    CHECK(header->getTSI() == vrtgen::TSI::NONE);
    CHECK(header->getTSF() == vrtgen::TSF::NONE);
    CHECK_FALSE(header->isClassIdentifierEnabled());
    CHECK(header->getPacketSize() == 4);
    CHECK(header->getTimestampMode() == vrtgen::TSM::FINE);
    // NB: As configured, BasicContextPacket is V49.0-compatible
    CHECK(header->getNotaV49_0Packet());

    CHECK(packing::BasicContextHelper::match(data.data(), data.size()));

    BasicContext packet_out;
    packing::BasicContextHelper::unpack(packet_out, data.data(), data.size());
    CHECK(packet_out.getStreamIdentifier() == 0x12345678);
    CHECK(packet_out.getReferencePointIdentifier() == 0x10AB7DE9);
}

TEST_CASE("Basic command message") {
    BasicCommand packet_in;
    packet_in.setStreamIdentifier(0x12345678);
    packet_in.setRFReferenceFrequency(101.1e6);

    size_t packed_size = packing::BasicCommandHelper::bytes_required(packet_in);
    CHECK(packed_size == 20);

    std::vector<unsigned char> data;
    data.resize(20);
    packing::BasicCommandHelper::pack(packet_in, data.data(), data.size());

    const vrtgen::packing::CommandHeader* header = reinterpret_cast<const vrtgen::packing::CommandHeader*>(data.data());
    CHECK(header->getPacketType() == vrtgen::PacketType::COMMAND);
    CHECK(header->getTSI() == vrtgen::TSI::NONE);
    CHECK(header->getTSF() == vrtgen::TSF::NONE);
    CHECK_FALSE(header->isClassIdentifierEnabled());
    CHECK(header->getPacketSize() == 5);
    CHECK_FALSE(header->getAcknowledgePacket());
    CHECK_FALSE(header->getCancellationPacket());

    CHECK(packing::BasicCommandHelper::match(data.data(), data.size()));

    BasicCommand packet_out;
    packing::BasicCommandHelper::unpack(packet_out, data.data(), data.size());
    CHECK(packet_out.getStreamIdentifier() == 0x12345678);
    CHECK(packet_out.getRFReferenceFrequency() == 101.1e6);
}