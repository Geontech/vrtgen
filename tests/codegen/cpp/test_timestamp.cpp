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
#include "timestamp.hpp"
#include <bytes.hpp>
#include <vrtgen/packing/enums.hpp>
#include "constants.hpp"

TEST_CASE("Timestamp", "[timestamp]")
{
    SECTION("Rule 5.1.4.1-4")
    {
        const uint32_t INTEGER_TIMESTAMP = 0xABCDEF12;
        const bytes INTEGER_TS_BE{ 0xAB, 0xCD, 0xEF, 0x12 };
        bytes data;

        SECTION("Data Packet")
        {
            TimestampData1 packet_in;
            packet_in.integer_timestamp(INTEGER_TIMESTAMP);
            CHECK(packet_in.integer_timestamp() == INTEGER_TIMESTAMP);

            auto data = packet_in.data();

            CHECK(int(data[(HEADER_BYTES + STREAM_ID_BYTES + CLASS_ID_BYTES + INTEGER_TS_BYTES)-1]) == 0x12);
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES + STREAM_ID_BYTES + CLASS_ID_BYTES;
            const bytes packed_integer_ts(check_ptr, check_ptr + INTEGER_TS_BYTES);
            CHECK(packed_integer_ts == INTEGER_TS_BE);

            TimestampData1 packet_out(data);
            CHECK(packet_out.integer_timestamp() == INTEGER_TIMESTAMP);
        }

        SECTION("Context Packet")
        {
            TimestampContext1 packet_in;
            packet_in.integer_timestamp(INTEGER_TIMESTAMP);
            CHECK(packet_in.integer_timestamp() == INTEGER_TIMESTAMP);

            auto data = packet_in.data();

            CHECK(int(data[(HEADER_BYTES + STREAM_ID_BYTES + CLASS_ID_BYTES + INTEGER_TS_BYTES)-1]) == 0x12);
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES + STREAM_ID_BYTES + CLASS_ID_BYTES;
            const bytes packed_integer_ts(check_ptr, check_ptr + INTEGER_TS_BYTES);
            CHECK(packed_integer_ts == INTEGER_TS_BE);

            TimestampContext1 packet_out(data);
            CHECK(packet_out.integer_timestamp() == INTEGER_TIMESTAMP);
        }

        SECTION("Control Packet")
        {
            TimestampControl1 packet_in;
            packet_in.integer_timestamp(INTEGER_TIMESTAMP);
            CHECK(packet_in.integer_timestamp() == INTEGER_TIMESTAMP);

            auto data = packet_in.data();

            CHECK(int(data[(HEADER_BYTES + STREAM_ID_BYTES + CLASS_ID_BYTES + INTEGER_TS_BYTES)-1]) == 0x12);
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES + STREAM_ID_BYTES + CLASS_ID_BYTES;
            const bytes packed_integer_ts(check_ptr, check_ptr + INTEGER_TS_BYTES);
            CHECK(packed_integer_ts == INTEGER_TS_BE);

            TimestampControl1 packet_out(data);
            CHECK(packet_out.integer_timestamp() == INTEGER_TIMESTAMP);
        }
    }

    SECTION("Rule 5.1.4.2-1")
    {
        const uint64_t FRACIONAL_TS = 0x123456789ABCDEF0;
        const bytes MOST_SIGNIFICANT_BE{ 0x12, 0x34, 0x56, 0x78 };
        const bytes LEAST_SIGNIFICANT_BE{ 0x9A, 0xBC, 0xDE, 0xF0 };
        bytes FRACTIONAL_TS_BE;
        FRACTIONAL_TS_BE.insert( FRACTIONAL_TS_BE.end(), MOST_SIGNIFICANT_BE.begin(), MOST_SIGNIFICANT_BE.end() );
        FRACTIONAL_TS_BE.insert( FRACTIONAL_TS_BE.end(), LEAST_SIGNIFICANT_BE.begin(), LEAST_SIGNIFICANT_BE.end() );

        SECTION("Data Packet")
        {
            TimestampData1 packet_in;
            packet_in.fractional_timestamp(FRACIONAL_TS);
            CHECK(packet_in.fractional_timestamp() == FRACIONAL_TS);

            auto data = packet_in.data();

            // CHECK(int(data[(HEADER_BYTES + STREAM_ID_BYTES + CLASS_ID_BYTES + FRACTIONAL_TS_BYTES)-1]) == 0xF0);
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES + STREAM_ID_BYTES + CLASS_ID_BYTES + INTEGER_TS_BYTES;
            const bytes packed_most_significant(check_ptr, check_ptr + FRACTIONAL_TS_BYTES/2);
            const bytes packed_least_significant(check_ptr + FRACTIONAL_TS_BYTES/2, check_ptr + FRACTIONAL_TS_BYTES);
            const bytes packed_fractional_ts(check_ptr, check_ptr + FRACTIONAL_TS_BYTES);
            CHECK(packed_most_significant == MOST_SIGNIFICANT_BE);
            CHECK(packed_least_significant == LEAST_SIGNIFICANT_BE);
            CHECK(packed_fractional_ts == FRACTIONAL_TS_BE);

            TimestampData1 packet_out(data);
            CHECK(packet_out.fractional_timestamp() == FRACIONAL_TS);
        }
        
        SECTION("Context Packet")
        {
            TimestampContext1 packet_in;
            packet_in.fractional_timestamp(FRACIONAL_TS);
            CHECK(packet_in.fractional_timestamp() == FRACIONAL_TS);

            auto data = packet_in.data();

            // CHECK(int(data[(HEADER_BYTES + STREAM_ID_BYTES + CLASS_ID_BYTES + FRACTIONAL_TS_BYTES)-1]) == 0xF0);
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES + STREAM_ID_BYTES + CLASS_ID_BYTES + INTEGER_TS_BYTES;
            const bytes packed_most_significant(check_ptr, check_ptr + FRACTIONAL_TS_BYTES/2);
            const bytes packed_least_significant(check_ptr + FRACTIONAL_TS_BYTES/2, check_ptr + FRACTIONAL_TS_BYTES);
            const bytes packed_fractional_ts(check_ptr, check_ptr + FRACTIONAL_TS_BYTES);
            CHECK(packed_most_significant == MOST_SIGNIFICANT_BE);
            CHECK(packed_least_significant == LEAST_SIGNIFICANT_BE);
            CHECK(packed_fractional_ts == FRACTIONAL_TS_BE);

            TimestampContext1 packet_out(data);
            CHECK(packet_out.fractional_timestamp() == FRACIONAL_TS);
        }
        
        SECTION("Control Packet")
        {
            TimestampControl1 packet_in;
            packet_in.fractional_timestamp(FRACIONAL_TS);
            CHECK(packet_in.fractional_timestamp() == FRACIONAL_TS);

            auto data = packet_in.data();

            // CHECK(int(data[(HEADER_BYTES + STREAM_ID_BYTES + CLASS_ID_BYTES + FRACTIONAL_TS_BYTES)-1]) == 0xF0);
            auto* check_ptr = data.data();
            check_ptr += HEADER_BYTES + STREAM_ID_BYTES + CLASS_ID_BYTES + INTEGER_TS_BYTES;
            const bytes packed_most_significant(check_ptr, check_ptr + FRACTIONAL_TS_BYTES/2);
            const bytes packed_least_significant(check_ptr + FRACTIONAL_TS_BYTES/2, check_ptr + FRACTIONAL_TS_BYTES);
            const bytes packed_fractional_ts(check_ptr, check_ptr + FRACTIONAL_TS_BYTES);
            CHECK(packed_most_significant == MOST_SIGNIFICANT_BE);
            CHECK(packed_least_significant == LEAST_SIGNIFICANT_BE);
            CHECK(packed_fractional_ts == FRACTIONAL_TS_BE);

            TimestampControl1 packet_out(data);
            CHECK(packet_out.fractional_timestamp() == FRACIONAL_TS);
        }
    }
}