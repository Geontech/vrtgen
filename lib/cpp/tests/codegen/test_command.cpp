/*
 * Copyright (C) 2024 Geon Technologies, LLC
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

#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <bytes.hpp>

#include "command/test_command_packet1.hpp"
#include "command/test_ack_packet1.hpp"
#include "command/test_command_packet3.hpp"
#include "command/test_ack_packet3.hpp"
#include "command/test_command_packet4.hpp"
#include "command/test_ack_packet4.hpp"
#include "command/test_command_packet5.hpp"
#include "command/test_ack_packet5.hpp"
#include "command/test_command_packet9.hpp"
#include "command/test_ack_packet9.hpp"
#include "command/test_command_packet_wif010.hpp"
#include "command/test_ack_packet_wif010.hpp"
#include "command/test_command_packet11.hpp"
#include "command/test_ack_packet11.hpp"
#include "command/test_command_packet_wif110.hpp"
#include "command/test_ack_packet_wif110.hpp"
#include "constants.hpp"

bytes parse_32(uint32_t var)
{
    return bytes{ static_cast<uint8_t>(var >> 24), static_cast<uint8_t>((var >> 16) & 0xFF), static_cast<uint8_t>((var >> 8) & 0xFF), static_cast<uint8_t>(var & 0xFF) };
}

bytes parse_64(uint64_t var)
{
    uint32_t var_upper = (var >> 32) & 0xFFFFFFFF;
    uint32_t var_lower = var & 0xFFFFFFFF;
    return bytes{ static_cast<uint8_t>(var_upper >> 24), static_cast<uint8_t>((var_upper >> 16) & 0xFF), static_cast<uint8_t>((var_upper >> 8) & 0xFF), static_cast<uint8_t>(var_upper & 0xFF),
                  static_cast<uint8_t>(var_lower >> 24), static_cast<uint8_t>((var_lower >> 16) & 0xFF), static_cast<uint8_t>((var_lower >> 8) & 0xFF), static_cast<uint8_t>(var_lower & 0xFF) };
}

const uint32_t STREAM_ID = 0x12345678;
const bytes STREAM_ID_BE = parse_32(STREAM_ID);
const uint32_t MESSAGE_ID = 0x0f100ded;
const bytes MESSAGE_ID_BE = parse_32(MESSAGE_ID);
const uint32_t CLASS_ID = 0x00ffeedd;
const uint32_t INTEGER = 0x57eadf57;
const bytes INTEGER_BE = parse_32(INTEGER);
const uint64_t FRACTIONAL = 0xde7e57ab1ed00d1e;
const bytes FRACTIONAL_BE = parse_64(FRACTIONAL);
const std::string CONTROLLEE_ID_128 = "12345678-dead-beef-0abc-def087654321";
const bytes CONTROLLEE_ID_128_BE { 0x12, 0x34, 0x56, 0x78,
                                   0xDE, 0xAD, 0xBE, 0xEF,
                                   0x0A, 0xBC, 0xDE, 0xF0,
                                   0x87, 0x65, 0x43, 0x21 };
const std::string CONTROLLER_ID_128 = "12345678-dead-beef-0abc-def087654321";
const bytes CONTROLLER_ID_128_BE { 0x12, 0x34, 0x56, 0x78,
                                   0xDE, 0xAD, 0xBE, 0xEF,
                                   0x0A, 0xBC, 0xDE, 0xF0,
                                   0x87, 0x65, 0x43, 0x21 };

using namespace command_ns::packets;

TEST_CASE("Section 8.2", "[command_packet][8.2]")
{
    SECTION("Rule 8.2-2")
    {
        size_t PACKET_SIZE = HEADER_BYTES + 
                            STREAM_ID_BYTES + 
                            CLASS_ID_BYTES + 
                            INTEGER_TS_BYTES + 
                            FRACTIONAL_TS_BYTES + 
                            CAM_BYTES + 
                            MESSAGE_ID_BYTES + 
                            CONTROLLEE_ID_128_BYTES +
                            CONTROLLER_ID_128_BYTES;

        SECTION("Control")
        {
            TestCommandPacket9 packet_in;
            packet_in.stream_id(STREAM_ID);
            packet_in.message_id(MESSAGE_ID);
            packet_in.integer_timestamp(INTEGER);
            packet_in.fractional_timestamp(FRACTIONAL);
            vrtgen::UUID controllee_uuid;
            controllee_uuid.set(CONTROLLEE_ID_128);
            vrtgen::UUID controller_uuid;
            controller_uuid.set(CONTROLLER_ID_128);
            packet_in.controllee_id() = controllee_uuid;
            packet_in.controller_id() = controller_uuid;

            PACKET_SIZE += CIF0_BYTES;

            CHECK(packet_in.size() == PACKET_SIZE);

            bytes CLASS_ID_BE { 0x0, static_cast<uint8_t>((CLASS_ID >> 16) & 0xFF), static_cast<uint8_t>((CLASS_ID >> 8) & 0xFF), static_cast<uint8_t>(CLASS_ID & 0xFF),
                                0x0, 0x0, 0x0, 0x0 };
            // auto CONTROLLEE_ENABLED = uint8_t{ 0x1 << 7 }; // Bit 31 in CAM
            // auto CONTROLLEE_FORMAT_128 = uint8_t{0x1 << 6 }; // Bit 30 in CAM
            // auto CONTROLLER_ENABLED = uint8_t{0x1 << 5 }; // Bit 29 in CAM
            // auto CONTROLLER_FORMAT_128 = uint8_t{0x1 << 4 }; // Bit 28 in CAM
            bytes CAM_BE = { 0xF0, 0, 0, 0 };
            auto MESSAGE_ID_BE = parse_32(MESSAGE_ID);
            auto data = packet_in.data();
            auto* check_ptr = data.data();

            check_ptr += HEADER_BYTES; // HEADER is already tested elsewhere
            const bytes packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
            check_ptr += STREAM_ID_BYTES;
            const bytes packed_class_id(check_ptr, check_ptr + CLASS_ID_BYTES);
            check_ptr += CLASS_ID_BYTES;
            const bytes packed_integer(check_ptr, check_ptr + INTEGER_TS_BYTES);
            check_ptr += INTEGER_TS_BYTES;
            const bytes packed_fractional(check_ptr, check_ptr + FRACTIONAL_TS_BYTES);
            check_ptr += FRACTIONAL_TS_BYTES;
            const bytes packed_cam(check_ptr, check_ptr + CAM_BYTES);
            check_ptr += CAM_BYTES;
            const bytes packed_message_id(check_ptr, check_ptr + MESSAGE_ID_BYTES);
            check_ptr += MESSAGE_ID_BYTES;
            const bytes packed_controllee_id(check_ptr, check_ptr + CONTROLLEE_ID_128_BYTES);
            check_ptr += CONTROLLEE_ID_128_BYTES;
            const bytes packed_controller_id(check_ptr, check_ptr + CONTROLLER_ID_128_BYTES);
            check_ptr += CONTROLLER_ID_128_BYTES;

            CHECK(packed_stream_id == STREAM_ID_BE);
            CHECK(packed_class_id == CLASS_ID_BE);
            CHECK(packed_integer == INTEGER_BE);
            CHECK(packed_fractional == FRACTIONAL_BE);
            CHECK(packed_cam == CAM_BE);
            CHECK(packed_message_id == MESSAGE_ID_BE);
            CHECK(packed_controllee_id == CONTROLLEE_ID_128_BE);
            CHECK(packed_controller_id == CONTROLLER_ID_128_BE);
        }
    }

    SECTION("Rule 8.2-3")
    {
        TestCommandPacket1 packet_in;
        uint32_t STREAM_ID = 0x12345678;
        uint32_t MESSAGE_ID = 0x0f100ded;
        packet_in.stream_id(STREAM_ID);
        packet_in.message_id(MESSAGE_ID);

        size_t PACKET_SIZE = HEADER_BYTES + STREAM_ID_BYTES + CAM_BYTES + MESSAGE_ID_BYTES + CIF0_BYTES;

        CHECK(packet_in.size() == PACKET_SIZE);


        bytes HEADER_BE { 0x60, 0x00, 0x00, static_cast<uint8_t>(PACKET_SIZE/4) };
        auto STREAM_ID_BE = parse_32(STREAM_ID);
        bytes CAM_BE = { 0, 0, 0, 0 };
        auto MESSAGE_ID_BE = parse_32(MESSAGE_ID);
        auto data = packet_in.data();
        auto* check_ptr = data.data();

        const bytes packed_header(check_ptr, check_ptr + HEADER_BYTES);
        check_ptr += HEADER_BYTES;
        const bytes packed_stream_id(check_ptr, check_ptr + STREAM_ID_BYTES);
        check_ptr += STREAM_ID_BYTES;
        const bytes packed_cam(check_ptr, check_ptr + CAM_BYTES);
        check_ptr += CAM_BYTES;
        const bytes packed_message_id(check_ptr, check_ptr + MESSAGE_ID_BYTES);
        check_ptr += MESSAGE_ID_BYTES;

        CHECK(packed_header == HEADER_BE);
        CHECK(packed_stream_id == STREAM_ID_BE);
        CHECK(packed_cam == CAM_BE);
        CHECK(packed_message_id == MESSAGE_ID_BE);

        TestCommandPacket1 packet_out(data);
        CHECK(packet_out.stream_id() == STREAM_ID);
        CHECK(packet_out.message_id() == MESSAGE_ID);
    }

    SECTION("Rule 8.2-6")
    {
        SECTION("Command")
        {
            TestCommandPacket1 packet_in;
            CHECK(packet_in.header().acknowledge_packet() == false);
            auto data = packet_in.data();
            TestCommandPacket1 packet_out(data);
            CHECK(packet_out.header().acknowledge_packet() == false);
        }

        SECTION("AckVX")
        {
            TestAckPacket1VX packet_in;
            CHECK(packet_in.header().acknowledge_packet() == true);
            auto data = packet_in.data();
            TestAckPacket1VX packet_out(data);
            CHECK(packet_out.header().acknowledge_packet() == true);
        }

        SECTION("AckS")
        {
            TestAckPacket1S packet_in;
            CHECK(packet_in.header().acknowledge_packet() == true);
            auto data = packet_in.data();
            TestAckPacket1S packet_out(data);
            CHECK(packet_out.header().acknowledge_packet() == true);
        }

        SECTION("Rule 8.2.6-2")
        {
            SECTION("Command")
            {
                TestCommandPacket5 packet_in;
                std::string CONTROLLEE_ID = "87654321-dead-beef-0abc-def087654321";
                std::string CONTROLLER_ID = "12345678-dead-beef-0abc-def087654321";
                vrtgen::UUID controllee_uuid;
                controllee_uuid.set(CONTROLLEE_ID);
                vrtgen::UUID controller_uuid;
                controller_uuid.set(CONTROLLER_ID);
                packet_in.controllee_id() = controllee_uuid;
                packet_in.controller_id() = controller_uuid;

                auto data = packet_in.data();
                auto* check_ptr = data.data();

                const bytes CONTROLLEE_ID_BE{ 0x87, 0x65, 0x43, 0x21,
                                                0xDE, 0xAD, 0xBE, 0xEF,
                                                0x0A, 0xBC, 0xDE, 0xF0,
                                                0x87, 0x65, 0x43, 0x21 };
                const bytes CONTROLLER_ID_BE{ 0x12, 0x34, 0x56, 0x78,
                                                0xDE, 0xAD, 0xBE, 0xEF,
                                                0x0A, 0xBC, 0xDE, 0xF0,
                                                0x87, 0x65, 0x43, 0x21 };
                check_ptr += HEADER_BYTES + STREAM_ID_BYTES + CAM_BYTES + MESSAGE_ID_BYTES;
                const bytes packed_controllee_id(check_ptr, check_ptr + CONTROLLEE_ID_128_BYTES);
                check_ptr += CONTROLLEE_ID_128_BYTES;
                const bytes packed_controller_id(check_ptr, check_ptr + CONTROLLER_ID_128_BYTES);
                check_ptr += CONTROLLER_ID_128_BYTES;
                CHECK(packed_controllee_id == CONTROLLEE_ID_BE);
                CHECK(packed_controller_id == CONTROLLER_ID_BE);

                TestCommandPacket5 packet_out(data);
                CHECK(packet_out.controllee_id().get() == CONTROLLEE_ID);
                CHECK(packet_out.controller_id().get() == CONTROLLER_ID);
            }

            SECTION("AckVX")
            {
                TestAckPacket5VX packet_in;
                std::string CONTROLLEE_ID = "87654321-dead-beef-0abc-def087654321";
                std::string CONTROLLER_ID = "12345678-dead-beef-0abc-def087654321";
                vrtgen::UUID controllee_uuid;
                controllee_uuid.set(CONTROLLEE_ID);
                vrtgen::UUID controller_uuid;
                controller_uuid.set(CONTROLLER_ID);
                packet_in.controllee_id() = controllee_uuid;
                packet_in.controller_id() = controller_uuid;

                auto data = packet_in.data();
                auto* check_ptr = data.data();

                const bytes CONTROLLEE_ID_BE{ 0x87, 0x65, 0x43, 0x21,
                                                0xDE, 0xAD, 0xBE, 0xEF,
                                                0x0A, 0xBC, 0xDE, 0xF0,
                                                0x87, 0x65, 0x43, 0x21 };
                const bytes CONTROLLER_ID_BE{ 0x12, 0x34, 0x56, 0x78,
                                                0xDE, 0xAD, 0xBE, 0xEF,
                                                0x0A, 0xBC, 0xDE, 0xF0,
                                                0x87, 0x65, 0x43, 0x21 };
                check_ptr += HEADER_BYTES + STREAM_ID_BYTES + CAM_BYTES + MESSAGE_ID_BYTES;
                const bytes packed_controllee_id(check_ptr, check_ptr + CONTROLLEE_ID_128_BYTES);
                check_ptr += CONTROLLEE_ID_128_BYTES;
                const bytes packed_controller_id(check_ptr, check_ptr + CONTROLLER_ID_128_BYTES);
                check_ptr += CONTROLLER_ID_128_BYTES;
                CHECK(packed_controllee_id == CONTROLLEE_ID_BE);
                CHECK(packed_controller_id == CONTROLLER_ID_BE);

                TestAckPacket5VX packet_out(data);
                CHECK(packet_out.controllee_id().get() == CONTROLLEE_ID);
                CHECK(packet_out.controller_id().get() == CONTROLLER_ID);
            }
        }
    }
    
    SECTION("Rule 8.2-7")
    {
        SECTION("Command")
        {
            TestCommandPacket1 packet_in;
            auto data = packet_in.data();
            // Check the reserved bit
            CHECK((data[0] & 0b00000010) >> 1 == 0);
            TestCommandPacket1 packet_out(data);
            data = packet_out.data();
            // Check the reserved bit
            CHECK((data[0] & 0b00000010) >> 1 == 0);
        }

        SECTION("AckVX")
        {
            TestAckPacket1VX packet_in;
            auto data = packet_in.data();
            // Check the reserved bit
            CHECK((data[0] & 0b00000010) >> 1 == 0);
            TestAckPacket1VX packet_out(data);
            data = packet_out.data();
            // Check the reserved bit
            CHECK((data[0] & 0b00000010) >> 1 == 0);
        }

        SECTION("AckS")
        {
            TestAckPacket1S packet_in;
            auto data = packet_in.data();
            // Check the reserved bit
            CHECK((data[0] & 0b00000010) >> 1 == 0);
            TestAckPacket1S packet_out(data);
            data = packet_out.data();
            // Check the reserved bit
            CHECK((data[0] & 0b00000010) >> 1 == 0);
        }
    }

    SECTION("Rule 8.2-11")
    {
        SECTION("Command")
        {
            TestCommandPacket1 packet_in;
            uint32_t MESSAGE_ID = 0xBA5EBA11;
            packet_in.message_id(MESSAGE_ID);

            auto data = packet_in.data();
            auto* check_ptr = data.data();

            bytes MESSAGE_ID_BE = { 0xBA, 0x5E, 0xBA, 0x11 };
            check_ptr += HEADER_BYTES + STREAM_ID_BYTES + CAM_BYTES;
            const bytes packed_message_id(check_ptr, check_ptr + MESSAGE_ID_BYTES);
            check_ptr += MESSAGE_ID_BYTES;
            CHECK(packed_message_id == MESSAGE_ID_BE);

            TestCommandPacket1 packet_out(data);
            CHECK(packet_out.message_id() == MESSAGE_ID);
        }
        
        SECTION("AckVX")
        {
            TestAckPacket1VX packet_in;
            uint32_t MESSAGE_ID = 0xBA5EBA11;
            packet_in.message_id(MESSAGE_ID);

            auto data = packet_in.data();
            auto* check_ptr = data.data();

            bytes MESSAGE_ID_BE = { 0xBA, 0x5E, 0xBA, 0x11 };
            check_ptr += HEADER_BYTES + STREAM_ID_BYTES + CAM_BYTES;
            const bytes packed_message_id(check_ptr, check_ptr + MESSAGE_ID_BYTES);
            check_ptr += MESSAGE_ID_BYTES;
            CHECK(packed_message_id == MESSAGE_ID_BE);


            TestAckPacket1VX packet_out(data);
            CHECK(packet_out.message_id() == MESSAGE_ID);
        }

        SECTION("AckS")
        {
            TestAckPacket1S packet_in;
            uint32_t MESSAGE_ID = 0xBA5EBA11;
            packet_in.message_id(MESSAGE_ID);

            auto data = packet_in.data();
            auto* check_ptr = data.data();

            bytes MESSAGE_ID_BE = { 0xBA, 0x5E, 0xBA, 0x11 };
            check_ptr += HEADER_BYTES + STREAM_ID_BYTES + CAM_BYTES;
            const bytes packed_message_id(check_ptr, check_ptr + MESSAGE_ID_BYTES);
            check_ptr += MESSAGE_ID_BYTES;
            CHECK(packed_message_id == MESSAGE_ID_BE);

            TestAckPacket1S packet_out(data);
            CHECK(packet_out.message_id() == MESSAGE_ID);
        }
    }

    SECTION("Rule 8.2-12")
    {
        SECTION("Rule 8.2.4-2: Word")
        {
            TestCommandPacket4 packet_in;
            uint32_t CONTROLLEE_ID = 0x12345678;
            packet_in.controllee_id(CONTROLLEE_ID);

            CHECK(packet_in.controllee_id() == CONTROLLEE_ID);

            auto data = packet_in.data();
            auto* check_ptr = data.data();

            const bytes CONTROLLEE_ID_BE{ 0x12, 0x34, 0x56, 0x78 };
            check_ptr += HEADER_BYTES + STREAM_ID_BYTES + CAM_BYTES + MESSAGE_ID_BYTES;
            const bytes packed_controllee_id(check_ptr, check_ptr + CONTROLLEE_ID_32_BYTES);
            check_ptr += CONTROLLEE_ID_32_BYTES;
            CHECK(packed_controllee_id == CONTROLLEE_ID_BE);

            TestCommandPacket4 packet_out(data);
            CHECK(packet_out.controllee_id() == CONTROLLEE_ID);
        }

        SECTION("Rule 8.2.4-3: UUID")
        {
            TestCommandPacket5 packet_in;
            std::string CONTROLLEE_ID = "12345678-dead-beef-0abc-def087654321";
            vrtgen::UUID uuid;
            uuid.set(CONTROLLEE_ID);
            packet_in.controllee_id() = uuid;

            auto data = packet_in.data();
            auto* check_ptr = data.data();

            const bytes CONTROLLEE_ID_BE{ 0x12, 0x34, 0x56, 0x78,
                                          0xDE, 0xAD, 0xBE, 0xEF,
                                          0x0A, 0xBC, 0xDE, 0xF0,
                                          0x87, 0x65, 0x43, 0x21 };
            check_ptr += HEADER_BYTES + STREAM_ID_BYTES + CAM_BYTES + MESSAGE_ID_BYTES;
            const bytes packed_controllee_id(check_ptr, check_ptr + CONTROLLEE_ID_128_BYTES);
            check_ptr += CONTROLLEE_ID_128_BYTES;
            CHECK(packed_controllee_id == CONTROLLEE_ID_BE);

            TestCommandPacket5 packet_out(data);
            CHECK(packet_out.controllee_id().get() == CONTROLLEE_ID);
        }
    }

    SECTION("Rule 8.2-13")
    {
        SECTION("Rule 8.2.5-2: Word")
        {
            TestCommandPacket4 packet_in;
            uint32_t CONTROLLER_ID = 0xDEADBEEF;
            packet_in.controller_id(CONTROLLER_ID);

            CHECK(packet_in.controller_id() == CONTROLLER_ID);

            auto data = packet_in.data();
            auto* check_ptr = data.data();

            const bytes CONTROLLER_ID_BE{ 0xDE, 0xAD, 0xBE, 0xEF };
            check_ptr += HEADER_BYTES + STREAM_ID_BYTES + CAM_BYTES + MESSAGE_ID_BYTES + CONTROLLEE_ID_32_BYTES;
            const bytes packed_controller_id(check_ptr, check_ptr + CONTROLLER_ID_32_BYTES);
            check_ptr += CONTROLLER_ID_32_BYTES;
            CHECK(packed_controller_id == CONTROLLER_ID_BE);

            TestCommandPacket4 packet_out(data);
            CHECK(packet_out.controller_id() == CONTROLLER_ID);
        }

        SECTION("Rule 8.2.5-3: UUID")
        {
            TestCommandPacket5 packet_in;
            std::string CONTROLLER_ID = "12345678-dead-beef-0abc-def087654321";
            vrtgen::UUID uuid;
            uuid.set(CONTROLLER_ID);
            packet_in.controller_id() = uuid;

            auto data = packet_in.data();
            auto* check_ptr = data.data();

            const bytes CONTROLLER_ID_BE{ 0x12, 0x34, 0x56, 0x78,
                                          0xDE, 0xAD, 0xBE, 0xEF,
                                          0x0A, 0xBC, 0xDE, 0xF0,
                                          0x87, 0x65, 0x43, 0x21 };
            check_ptr += HEADER_BYTES + STREAM_ID_BYTES + CAM_BYTES + MESSAGE_ID_BYTES + CONTROLLEE_ID_128_BYTES;
            const bytes packed_controller_id(check_ptr, check_ptr + CONTROLLER_ID_128_BYTES);
            check_ptr += CONTROLLER_ID_128_BYTES;
            CHECK(packed_controller_id == CONTROLLER_ID_BE);

            TestCommandPacket5 packet_out(data);
            CHECK(packet_out.controller_id().get() == CONTROLLER_ID);
        }
    }

} // end TEST_CASE("Section 8.2")

TEST_CASE("Section 8.3", "[control_packet_subtype][8.3]")
{
    SECTION("Rule 8.3-1")
    {
        // using packet_type = TestCommandPacketNoAction12;
        // packet_type packet_in;

        // CHECK(packet_in.size() == BASIC_CONTROL_BYTES);
    }

    SECTION("Rule 8.3-2")
    {
        SECTION("Dry Run")
        {
            // using packet_type = TestCommandPacketDryRun12;
            // packet_type packet_in;

            // CHECK(packet_in.size() == BASIC_CONTROL_BYTES + BANDWIDTH_BYTES);
        }

        SECTION("Execute")
        {
            // using packet_type = TestCommandPacketExecute12;
            // packet_type packet_in;

            // CHECK(packet_in.size() == BASIC_CONTROL_BYTES + BANDWIDTH_BYTES);
        }
    }

    SECTION("Rule 8.3.1.1-1")
    {
        SECTION("Controllee Identifier Included")
        {
            TestCommandPacket4 packet_in;
            auto data = packet_in.data();
            CHECK((data[HEADER_BYTES + STREAM_ID_BYTES + 0] >> 7) == 1);
        }
        
        SECTION("No Controllee Identifier Included")
        {
            TestCommandPacket1 packet_in;
            auto data = packet_in.data();
            CHECK((data[HEADER_BYTES + STREAM_ID_BYTES + 0] >> 7) == 0);
        }
    }
    
    SECTION("Rule 8.3.1.1-2")
    {
        SECTION("Identifier Format: WORD")
        {
            TestCommandPacket4 packet_in;
            auto data = packet_in.data();
            CHECK(((data[HEADER_BYTES + STREAM_ID_BYTES + 0] >> 6) & 0b1) == 0);
        }
        
        SECTION("Identifier Format: UUID")
        {
            TestCommandPacket5 packet_in;
            auto data = packet_in.data();
            CHECK(((data[HEADER_BYTES + STREAM_ID_BYTES + 0] >> 6) & 0b1) == 1);
        }
    }

    SECTION("Rule 8.3.1.1-3")
    {
        SECTION("Controller Identifier Included")
        {
            TestCommandPacket4 packet_in;
            auto data = packet_in.data();
            CHECK(((data[HEADER_BYTES + STREAM_ID_BYTES + 0] >> 5) & 0b1) == 1);
        }
        
        SECTION("No Controller Identifier Included")
        {
            TestCommandPacket1 packet_in;
            auto data = packet_in.data();
            CHECK(((data[HEADER_BYTES + STREAM_ID_BYTES + 0] >> 5) & 0b1) == 0);
        }
    }
    
    SECTION("Rule 8.3.1.1-4")
    {
        SECTION("Identifier Format: WORD")
        {
            TestCommandPacket4 packet_in;
            auto data = packet_in.data();
            CHECK(((data[HEADER_BYTES + STREAM_ID_BYTES + 0] >> 4) & 0b1) == 0);
        }
        
        SECTION("Identifier Format: UUID")
        {
            TestCommandPacket5 packet_in;
            auto data = packet_in.data();
            CHECK(((data[HEADER_BYTES + STREAM_ID_BYTES + 0] >> 4) & 0b1) == 1);
        }
    }

    SECTION("Rule 8.3.1.6-1")
    {
        SECTION ("WIF0 - no error")
        {
            TestAckPacketWif010VX packet_in;
            vrtgen::packing::WarningErrorFields warning_fields_in;
            warning_fields_in.device_failure(true);
            packet_in.bandwidth_warnings() = warning_fields_in;

            auto data = packet_in.data();
            auto check_ptr = data.data();

            CHECK(packet_in.cam().ack_w() == true);
            CHECK(packet_in.cam().ack_er() == false);
            CHECK(packet_in.wif_0().has_value() == true);
            CHECK(packet_in.eif_0().has_value() == false);
            CHECK(packet_in.wif_0()->bandwidth() == true);
            CHECK(packet_in.bandwidth_warnings()->device_failure() == true);
            CHECK(packet_in.bandwidth_errors().has_value() == false);

            CHECK(data.size() == BASIC_ACK_BYTES + WIF_BYTES + WARNING_FIELDS_BYTES);

            uint8_t DEVICE_FAILURE = 0x1 << 6;
            uint8_t BANDWIDTH = 0x1 << 5;
            bytes WIF_BE { BANDWIDTH, 0, 0, 0 };
            bytes WARNING_FIELDS_BE { DEVICE_FAILURE, 0, 0, 0 };
            check_ptr += BASIC_ACK_BYTES;
            const bytes packed_wif(check_ptr, check_ptr + WIF_BYTES);
            check_ptr += WIF_BYTES;
            const bytes packed_warning_fields(check_ptr, check_ptr + WARNING_FIELDS_BYTES);
            check_ptr += WARNING_FIELDS_BYTES;

            CHECK(packed_wif == WIF_BE);
            CHECK(packed_warning_fields == WARNING_FIELDS_BE);

            TestAckPacketWif010VX packet_out(data);
            CHECK(packet_out.cam().ack_w() == true);
            CHECK(packet_out.cam().ack_er() == false);
            CHECK(packet_out.wif_0().has_value() == true);
            CHECK(packet_out.eif_0().has_value() == false);
            CHECK(packet_out.wif_0()->bandwidth() == true);
            CHECK(packet_out.bandwidth_warnings()->device_failure() == true);
            CHECK(packet_out.bandwidth_errors().has_value() == false);
        }

        SECTION("WIF0 change reference")
        {
            //FIXME
            TestAckPacketWif010VX packet_in;
            vrtgen::packing::WarningErrorFields error_fields_in;
            error_fields_in.field_not_executed(true);
            packet_in.sample_rate_errors(error_fields_in);
            auto& sample_error = packet_in.sample_rate_errors();
            sample_error->hazardous_power_levels(true);

            CHECK(packet_in.sample_rate_errors().has_value());
            CHECK(packet_in.sample_rate_errors()->hazardous_power_levels() == true);
            sample_error->hazardous_power_levels(false);

            CHECK(packet_in.sample_rate_errors()->hazardous_power_levels() == false);
        }

        SECTION("WIF0")
        {
            TestAckPacketWif010VX packet_in;
            vrtgen::packing::WarningErrorFields warning_fields_in;
            warning_fields_in.field_not_executed(true);
            vrtgen::packing::WarningErrorFields error_fields_in;
            error_fields_in.hazardous_power_levels(true);
            packet_in.sample_rate_errors() = error_fields_in;
            packet_in.bandwidth_warnings() = warning_fields_in;

            auto data = packet_in.data();
            auto check_ptr = data.data();

            CHECK(packet_in.cam().ack_w() == true);
            CHECK(packet_in.cam().ack_er() == true);
            CHECK(data.size() == BASIC_ACK_BYTES + WIF_BYTES + EIF_BYTES + WARNING_FIELDS_BYTES + ERROR_FIELDS_BYTES);

            uint8_t FIELD_NOT_EXECUTED = 0x1 << 7;
            uint8_t HAZARDOUS = 0x1;
            uint8_t BANDWIDTH = 0x1 << 5;
            uint8_t SAMPLE_RATE = 0x1 << 5;
            bytes WIF_BE { BANDWIDTH, 0, 0, 0 };
            bytes EIF_BE { 0, SAMPLE_RATE, 0, 0 };
            bytes WARNING_FIELDS_BE { FIELD_NOT_EXECUTED, 0, 0, 0 };
            bytes ERROR_FIELDS_BE { HAZARDOUS, 0, 0, 0 };
            check_ptr += BASIC_ACK_BYTES;
            const bytes packed_wif(check_ptr, check_ptr + WIF_BYTES);
            check_ptr += WIF_BYTES;
            const bytes packed_eif(check_ptr, check_ptr + EIF_BYTES);
            check_ptr += EIF_BYTES;
            const bytes packed_warning_fields(check_ptr, check_ptr + WARNING_FIELDS_BYTES);
            check_ptr += WARNING_FIELDS_BYTES;
            const bytes packed_error_fields(check_ptr, check_ptr + ERROR_FIELDS_BYTES);
            check_ptr += ERROR_FIELDS_BYTES;

            CHECK(packed_wif == WIF_BE);
            CHECK(packed_eif == EIF_BE);
            CHECK(packed_warning_fields == WARNING_FIELDS_BE);
            CHECK(packed_error_fields == ERROR_FIELDS_BE);

            TestAckPacketWif010VX packet_out(data);
            CHECK(packet_out.cam().ack_w() == true);
            CHECK(packet_out.cam().ack_er() == true);
            CHECK(packet_out.wif_0()->bandwidth() == true);
            CHECK(packet_out.eif_0()->sample_rate() == true);
            CHECK(packet_out.bandwidth_warnings()->field_not_executed() == true);
            CHECK(packet_out.sample_rate_errors()->hazardous_power_levels() == true);
        }
        
        SECTION("WIF1")
        {
            TestAckPacketWif110VX packet_in;
            vrtgen::packing::WarningErrorFields warning_fields_in;
            warning_fields_in.device_failure(true);
            vrtgen::packing::WarningErrorFields error_fields_in;
            error_fields_in.device_failure(true);
            packet_in.pointing_vector_warnings() = warning_fields_in;
            packet_in.pointing_vector_errors() = error_fields_in;
            
            auto data = packet_in.data();
            auto* check_ptr = data.data();

            CHECK(packet_in.cam().ack_w());
            CHECK(packet_in.cam().ack_er());

            CHECK(data.size() == BASIC_ACK_BYTES + // basic ack prologue
                                 WIF_BYTES +  // wif0
                                 WIF_BYTES +  // wif1
                                 EIF_BYTES +  // eif0
                                 EIF_BYTES +  // eif1
                                 WARNING_FIELDS_BYTES + // warning fields 1
                                 ERROR_FIELDS_BYTES); // warning fields 1

            uint8_t DEVICE_FAILURE = 0x1 << 6;
            uint8_t POINTING_VECTOR = 0x1 << 5;
            uint8_t WIF1 = 0x1 << 1;
            uint8_t EIF1 = 0x1 << 1;
            bytes WIF0_BE { 0, 0, 0, WIF1 };
            bytes WIF1_BE { POINTING_VECTOR, 0, 0, 0 };
            bytes EIF0_BE { 0, 0, 0, EIF1 };
            bytes EIF1_BE { POINTING_VECTOR, 0, 0, 0 };
            bytes WARNING_FIELDS_BE { DEVICE_FAILURE, 0, 0, 0 };
            bytes ERROR_FIELDS_BE { DEVICE_FAILURE, 0, 0, 0 };
            check_ptr += BASIC_ACK_BYTES;
            const bytes packed_wif0(check_ptr, check_ptr + WIF_BYTES);
            check_ptr += WIF_BYTES;
            const bytes packed_wif1(check_ptr, check_ptr + WIF_BYTES);
            check_ptr += WIF_BYTES;
            const bytes packed_eif0(check_ptr, check_ptr + EIF_BYTES);
            check_ptr += EIF_BYTES;
            const bytes packed_eif1(check_ptr, check_ptr + EIF_BYTES);
            check_ptr += EIF_BYTES;
            const bytes packed_warning_fields(check_ptr, check_ptr + WARNING_FIELDS_BYTES);
            check_ptr += WARNING_FIELDS_BYTES;
            const bytes packed_error_fields(check_ptr, check_ptr + ERROR_FIELDS_BYTES);
            check_ptr += ERROR_FIELDS_BYTES;

            CHECK(packed_wif0 == WIF0_BE);
            CHECK(packed_wif1 == WIF1_BE);
            CHECK(packed_eif0 == EIF0_BE);
            CHECK(packed_eif1 == EIF1_BE);
            CHECK(packed_warning_fields == WARNING_FIELDS_BE);
            CHECK(packed_error_fields == ERROR_FIELDS_BE);

            TestAckPacketWif110VX packet_out(data);
            CHECK(packet_out.cam().ack_w());
            CHECK(packet_out.cam().ack_er());
            CHECK(packet_out.wif_0()->wif1_enable() == true);
            CHECK(packet_out.eif_0()->eif1_enable() == true);
            CHECK(packet_out.wif_1()->pointing_vector() == true);
            CHECK(packet_out.eif_1()->pointing_vector() == true);
            CHECK(packet_out.pointing_vector_warnings()->device_failure() == true);
            CHECK(packet_out.pointing_vector_errors()->device_failure() == true);
        }
    }

} // end TEST_CASE("Section 8.3")


TEST_CASE("Section 8.4", "[acknowledge_packet_subtype][8.4]")
{
    SECTION("8.4.1.3")
    {
        SECTION("AckP")
        {
            TestAckPacket9VX packet_in;
            packet_in.partial_action(true);
            CHECK(packet_in.partial_action() == true);
            CHECK(packet_in.cam().partial_action() == true);

            auto data = packet_in.data();
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES + STREAM_ID_BYTES;
            CHECK(((check_ptr[2] >> 3) & 0b1) == 1);

            TestAckPacket9VX packet_out(data);
            CHECK(packet_out.partial_action() == true);
            CHECK(packet_out.cam().partial_action() == true);
        }
    }

    SECTION("8.4.1.4")
    {
        SECTION("SchX")
        {
            TestAckPacket9VX packet_in;
            packet_in.scheduled_or_executed(true);
            CHECK(packet_in.scheduled_or_executed() == true);
            CHECK(packet_in.cam().scheduled_or_executed() == true);

            auto data = packet_in.data();
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES + STREAM_ID_BYTES;
            CHECK(((check_ptr[2] >> 2) & 0b1) == 1);

            TestAckPacket9VX packet_out(data);
            CHECK(packet_out.scheduled_or_executed() == true);
            CHECK(packet_out.cam().scheduled_or_executed() == true);
        }
    }
}
