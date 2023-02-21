/*
 * Copyright (C) 2022 Geon Technologies, LLC
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

#include "catch.hpp"
#include "bytes.hpp"

#include "vrtgen/packing/header.hpp"

using namespace vrtgen::packing;

TEST_CASE("Base Header", "[header]")
{
    Header header;
    Header unpack_header;
    bytes packed_bytes{ 0xFF, 0xFF, 0xFF, 0xFF };

    SECTION("Zero on construction")
    {
        header.pack_into(packed_bytes.data());
        CHECK(packed_bytes == bytes{ 0, 0, 0, 0 });
    }

    SECTION("Rule 5.1.1-2")
    {
        SECTION("Packet Type")
        {
            // Verify zero on construction
            CHECK(header.packet_type() == PacketType::SIGNAL_DATA);
            // Setter
            header.packet_type(PacketType::CONTEXT);
            // Getter check set value
            CHECK(header.packet_type() == PacketType::CONTEXT);
            // Pack
            header.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes[0] == 0x40);
            // Unpack
            unpack_header.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_header.packet_type() == PacketType::CONTEXT);
        }

        SECTION("Class ID Enable")
        {
            // Verify zero on construction
            CHECK(header.class_id_enable() == false);
            // Setter
            header.class_id_enable(true);
            // Getter check set value
            CHECK(header.class_id_enable() == true);
            // Pack
            header.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes[0] == 0x08);
            // Unpack
            unpack_header.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_header.class_id_enable() == true);
        }

        SECTION("Rule 5.1.1-6")
        {
            // Verify zero on construction
            CHECK(header.tsi() == TSI::NONE);

            SECTION("UTC")
            {
                // Setter
                header.tsi(TSI::UTC);
                // Getter check set value
                CHECK(header.tsi() == TSI::UTC);
                // Pack
                header.pack_into(packed_bytes.data());
                // Verify packed bits
                CHECK(packed_bytes[1] == 0b01 << 6);
                // Unpack
                unpack_header.unpack_from(packed_bytes.data());
                // Verify unpacked value
                CHECK(unpack_header.tsi() == TSI::UTC);
            }

            SECTION("GPS")
            {
                // Setter
                header.tsi(TSI::GPS);
                // Getter check set value
                CHECK(header.tsi() == TSI::GPS);
                // Pack
                header.pack_into(packed_bytes.data());
                // Verify packed bits
                CHECK(packed_bytes[1] == 0b10 << 6);
                // Unpack
                unpack_header.unpack_from(packed_bytes.data());
                // Verify unpacked value
                CHECK(unpack_header.tsi() == TSI::GPS);
            }

            SECTION("OTHER")
            {
                // Setter
                header.tsi(TSI::OTHER);
                // Getter check set value
                CHECK(header.tsi() == TSI::OTHER);
                // Pack
                header.pack_into(packed_bytes.data());
                // Verify packed bits
                CHECK(packed_bytes[1] == 0b11 << 6);
                // Unpack
                unpack_header.unpack_from(packed_bytes.data());
                // Verify unpacked value
                CHECK(unpack_header.tsi() == TSI::OTHER);
            }
        }

        SECTION("Rule 5.1.1-8")
        {
            // Verify zero on construction
            CHECK(header.tsf() == TSF::NONE);

            SECTION("SAMPLE_COUNT")
            {
                // Setter
                header.tsf(TSF::SAMPLE_COUNT);
                // Getter check set value
                CHECK(header.tsf() == TSF::SAMPLE_COUNT);
                // Pack
                header.pack_into(packed_bytes.data());
                // Verify packed bits
                CHECK(packed_bytes[1] == 0x01 << 4);
                // Unpack
                unpack_header.unpack_from(packed_bytes.data());
                // Verify unpacked value
                CHECK(unpack_header.tsf() == TSF::SAMPLE_COUNT);
            }

            SECTION("REAL TIME")
            {
                // Setter
                header.tsf(TSF::REAL_TIME);
                // Getter check set value
                CHECK(header.tsf() == TSF::REAL_TIME);
                // Pack
                header.pack_into(packed_bytes.data());
                // Verify packed bits
                CHECK(packed_bytes[1] == 0b10 << 4);
                // Unpack
                unpack_header.unpack_from(packed_bytes.data());
                // Verify unpacked value
                CHECK(unpack_header.tsf() == TSF::REAL_TIME);
            }

            SECTION("FREE_RUNNING")
            {
                // Setter
                header.tsf(TSF::FREE_RUNNING);
                // Getter check set value
                CHECK(header.tsf() == TSF::FREE_RUNNING);
                // Pack
                header.pack_into(packed_bytes.data());
                // Verify packed bits
                CHECK(packed_bytes[1] == 0b11 << 4);
                // Unpack
                unpack_header.unpack_from(packed_bytes.data());
                // Verify unpacked value
                CHECK(unpack_header.tsf() == TSF::FREE_RUNNING);
            }
        }

        SECTION("Rule 5.1.1-9")
        {
            SECTION("Packet Count")
            {
                // Verify zero on construction
                CHECK(header.packet_count() == 0);
                // Setter
                header.packet_count(0xA);
                // Getter check set value
                CHECK(header.packet_count() == 0xA);
                // Pack
                header.pack_into(packed_bytes.data());
                // Verify packed bits
                CHECK(packed_bytes[1] == 0x0A);
                // Unpack
                unpack_header.unpack_from(packed_bytes.data());
                // Verify unpacked value
                CHECK(unpack_header.packet_count() == 0xA);
            }
        }

        SECTION("Packet Size")
        {
            // Verify zero on construction
            CHECK(header.packet_size() == 0);
            // Setter
            header.packet_size(0x1234);
            // Getter check set value
            CHECK(header.packet_size() == 0x1234);
            // Pack
            header.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(range(packed_bytes, 2, 4) == bytes{ 0x12, 0x34 });
            // Unpack
            unpack_header.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_header.packet_size() == 0x1234);
        }
    }
}

TEST_CASE("Indicator Bits 5.1.1.1", "[header][indicator_bits]")
{
    SECTION("Data Header")
    {
        DataHeader header;
        DataHeader unpack_header;
        bytes packed_bytes{ 0xFF, 0xFF, 0xFF, 0xFF };

        SECTION("Zero on construction")
        {
            header.pack_into(packed_bytes.data());
            CHECK(packed_bytes == bytes{ 0, 0, 0, 0 });
        }

        SECTION("Trailer Included")
        {
            // Verify zero on construction
            CHECK(header.trailer_included() == false);
            // Setter
            header.trailer_included(true);
            // Getter check set value
            CHECK(header.trailer_included() == true);
            // Pack
            header.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes[0] == 0x04);
            // Unpack
            unpack_header.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_header.trailer_included() == true);
        }

        SECTION("Rule 5.1.1.1-2: Not a V49.0 Packet Indicator")
        {
            // Verify zero on construction
            CHECK(header.not_v49d0() == false);
            // Setter
            header.not_v49d0(true);
            // Getter check set value
            CHECK(header.not_v49d0() == true);
            // Pack
            header.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes[0] == 0x02);
            // Unpack
            unpack_header.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_header.not_v49d0() == true);
        }

        SECTION("Signal Spectrum or Signal Time Data Packet")
        {
            // Verify zero on construction
            CHECK(header.spectrum_or_time() == vrtgen::packing::SPECTRUM_OR_TIME::TIME);
            // Setter
            header.spectrum_or_time(vrtgen::packing::SPECTRUM_OR_TIME::SPECTRUM);
            // Getter check set value
            CHECK(header.spectrum_or_time() == vrtgen::packing::SPECTRUM_OR_TIME::SPECTRUM);
            // Pack
            header.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes[0] == 0x01);
            // Unpack
            unpack_header.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_header.spectrum_or_time() == vrtgen::packing::SPECTRUM_OR_TIME::SPECTRUM);
        }
    }

    SECTION("Context Header")
    {
        ContextHeader header;
        ContextHeader unpack_header;
        bytes packed_bytes{ 0xFF, 0xFF, 0xFF, 0xFF };

        SECTION("Zero on construction")
        {
            header.pack_into(packed_bytes.data());
            CHECK(packed_bytes == bytes{ 0, 0, 0, 0 });
        }

        SECTION("Rule 5.1.1.1-2: Not a V49.0 Packet Indicator")
        {
            // Verify zero on construction
            CHECK(header.not_v49d0() == false);
            // Setter
            header.not_v49d0(true);
            // Getter check set value
            CHECK(header.not_v49d0() == true);
            // Pack
            header.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes[0] == 0x02);
            // Unpack
            unpack_header.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_header.not_v49d0() == true);
        }

        SECTION("Rule 7.1.1-3")
        {
            // Verify zero on construction
            CHECK(header.tsm() == TSM::FINE);
            // Setter
            header.tsm(TSM::FINE);
            // Getter check set value
            CHECK(header.tsm() == TSM::FINE);
            // Pack
            header.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes[0] == 0x00);
            // Unpack
            unpack_header.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_header.tsm() == TSM::FINE);
        }

        SECTION("Rule 7.1.1-4")
        {
            // Verify zero on construction
            CHECK(header.tsm() == TSM::FINE);
            // Setter
            header.tsm(TSM::COARSE);
            // Getter check set value
            CHECK(header.tsm() == TSM::COARSE);
            // Pack
            header.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes[0] == 0x01);
            // Unpack
            unpack_header.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_header.tsm() == TSM::COARSE);
        }
    }

    SECTION("Command Header")
    {
        CommandHeader header;
        CommandHeader unpack_header;
        bytes packed_bytes{ 0xFF, 0xFF, 0xFF, 0xFF };

        SECTION("Zero on construction")
        {
            header.pack_into(packed_bytes.data());
            CHECK(packed_bytes == bytes{ 0, 0, 0, 0 });
        }

        SECTION("Acknowledge Packet")
        {
            // Verify zero on construction
            CHECK(header.acknowledge_packet() == false);
            // Setter
            header.acknowledge_packet(true);
            // Getter check set value
            CHECK(header.acknowledge_packet() == true);
            // Pack
            header.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes[0] == 0x04);
            // Unpack
            unpack_header.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_header.acknowledge_packet() == true);
        }

        SECTION("Cancellation Packet")
        {
            // Verify zero on construction
            CHECK(header.cancellation_packet() == false);
            // Setter
            header.cancellation_packet(true);
            // Getter check set value
            CHECK(header.cancellation_packet() == true);
            // Pack
            header.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes[0] == 0x01);
            // Unpack
            unpack_header.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_header.cancellation_packet() == true);
        }
    }
}