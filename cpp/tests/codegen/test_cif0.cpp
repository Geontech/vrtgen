/*
 * Copyright (C) 2021 Geon Technologies, LLC
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

#include "catch/catch.hpp"

#include "yamls/cif0.hpp"

#include <vector>
#include <bytes.hpp>
#include <arpa/inet.h>

TEST_CASE("All CIF0 Context Optional") {
    using packet_type = AllCIF0ContextOptional;
    using packet_helper = packing::AllCIF0ContextOptionalHelper;

    SECTION("Field Change Indicator") {
        SECTION("Rule 9.1.1-1") {
            /*
            * Rule 9.1.1-1: The Context Field Change Indicator bit shall be set
            * to zero when all the Context fields within the Context Packet
            * were conveyed in earlier Context packets with the same
            * information.
            */
            const size_t PACKED_SIZE = 12;

            packet_type packet_in;
            packet_in.setContextFieldChangeIndicator(false);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[8])) == 0x0);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasContextFieldChangeIndicator() == false);
        }

        SECTION("9.1.1-2") {
            /*
             * Rule 9.1.1-2: The Context Field Change Indicator bit shall be
             * set to one when at least one Context field contains a new value.
             */
            const size_t PACKED_SIZE = 12;

            packet_type packet_in;
            packet_in.setContextFieldChangeIndicator(true);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[8])) == htonl(0x80000000));

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasContextFieldChangeIndicator());
            CHECK(packet_out.getContextFieldChangeIndicator() == true);
        }
    }
    
    SECTION("Reference Point Identifier") {
        /*
         * Rule 9.2-1: The Reference Point ID shall use the format shown in
         * Figure 9.2-1.
         */
        const size_t PACKED_SIZE = 16;

        SECTION("Rule 9.2-2") {
            /*
             * The Reference Point ID, when used, shall contain the Stream
             * Identifier of the Reference Point.
             */
            const vrtgen::StreamIdentifier REF_POINT_ID = 0x12345678;
            const vrtgen::StreamIdentifier REF_POINT_ID_BE = 0x78563412;

            packet_type packet_in;
            packet_in.setReferencePointIdentifier(REF_POINT_ID);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const vrtgen::StreamIdentifier*>(&data[12])) == REF_POINT_ID_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasReferencePointIdentifier());
            CHECK(packet_out.getReferencePointIdentifier() == REF_POINT_ID);
        }
    }

    SECTION("Bandwidth") {
        SECTION("Rule 9.5.1-2") {
            /*
             * The value of the Bandwidth field shall be expressed in units of
             * Hertz. The Bandwidth field shall use the 64-bit,
             * two’s-complement format shown in Figure 9.5.1-1. This field has
             * an integer and a fractional part with the radix point to the
             * right of bit 20 in the second 32-bit word.
             *
             * Observation 9.5.1-3: The value of the Bandwidth field has a valid
             * range of 0.00 to 8.79 terahertz with a resolution of 0.95
             * micro-Hertz. Negative values of Bandwidth are not valid.
             * 
             * Observation 9.5.1-4: A Bandwidth field value of 0x0000 0000 0010 0000
             * represents a bandwidth of 1 Hz. A Bandwidth field value of
             * 0x0000 0000 0000 0001 represents a bandwidth of 0.95 micro-Hertz.
             */
            const double BANDWIDTH_1 = 1.0; // represents 0x0000 0000 0010 0000
            const double BANDWIDTH_2 = 0.95e-6; // represents 0x0000 0000 0000 0001
            const int64_t BANDWIDTH_INT_BE_1 = 0x0000100000000000;
            const int64_t BANDWIDTH_INT_BE_2 = 0x0100000000000000;
            const size_t PACKED_SIZE = 20;

            // Bandwidth 1
            packet_type packet_in;
            packet_in.setBandwidth(BANDWIDTH_1);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == BANDWIDTH_INT_BE_1);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasBandwidth());
            CHECK(packet_out.getBandwidth() == BANDWIDTH_1);

            // Bandwidth 2
            packet_in = packet_type();
            packet_in.setBandwidth(BANDWIDTH_2);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == BANDWIDTH_INT_BE_2);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasBandwidth());
            CHECK(packet_out.getBandwidth() == BANDWIDTH_2);
        }
    }

    SECTION("IF Reference Frequency") {
        SECTION("Rule 9.5.5-3") {
            /*
             * The value of the IF Reference Frequency shall be expressed in
             * units of Hertz. The IF Reference Frequency subfield shall use
             * the 64-bit, two’s-complement format as shown in Figure 9.5.5-1.
             * This field has an integer and a fractional part, with the radix
             * point to the right of bit 20 in the second 32-bit word.
             * 
             * Observation 9.5.5-2: The value of the IF Reference Frequency
             * field has a range of 8.79 terahertz with a resolution of 0.95
             * micro-Hertz.
             * 
             * Observation 9.5.5-3: An IF Reference Frequency field value of
             * 0x0000 0000 0010 0000 represents a frequency of 1 Hz. An IF
             * Reference Frequency field value of 0xFFFF FFFF FFF0 0000
             * represents a frequency of -1 Hz. An IF Reference Frequency field
             * value of 0x0000 0000 0000 0001 represents a frequency of 0.95
             * micro-Hertz. An IF Reference Frequency field value of
             * 0xFFFF FFFF FFFF FFFF represents a frequency of -0.95 micro-Hertz.
             */
            const double IF_REF_FREQ_1 = 1.0; // represents 0x0000 0000 0010 0000
            const double IF_REF_FREQ_2 = 0.95e-6; // represents 0x0000 0000 0000 0001
            const double IF_REF_FREQ_3 = -1.0; // represents 0xFFFF FFFF FFF0 0000
            const double IF_REF_FREQ_4 = -0.95e-6; // represents 0xFFFF FFFF FFFF FFFF
            const int64_t IF_REF_FREQ_INT_BE_1 = 0x0000100000000000;
            const int64_t IF_REF_FREQ_INT_BE_2 = 0x0100000000000000;
            const int64_t IF_REF_FREQ_INT_BE_3 = 0x0000F0FFFFFFFFFF;
            const int64_t IF_REF_FREQ_INT_BE_4 = 0xFFFFFFFFFFFFFFFF;
            const size_t PACKED_SIZE = 20;

            // IF Reference Frequency 1
            packet_type packet_in;
            packet_in.setIFReferenceFrequency(IF_REF_FREQ_1);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_REF_FREQ_INT_BE_1);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasIFReferenceFrequency());
            CHECK(packet_out.getIFReferenceFrequency() == IF_REF_FREQ_1);

            // IF Reference Frequency 2
            packet_in = packet_type();
            packet_in.setIFReferenceFrequency(IF_REF_FREQ_2);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_REF_FREQ_INT_BE_2);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasIFReferenceFrequency());
            CHECK(packet_out.getIFReferenceFrequency() == IF_REF_FREQ_2);

            // IF Reference Frequency 3
            packet_in = packet_type();
            packet_in.setIFReferenceFrequency(IF_REF_FREQ_3);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_REF_FREQ_INT_BE_3);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasIFReferenceFrequency());
            CHECK(packet_out.getIFReferenceFrequency() == IF_REF_FREQ_3);

            // IF Reference Frequency 4
            packet_in = packet_type();
            packet_in.setIFReferenceFrequency(IF_REF_FREQ_4);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_REF_FREQ_INT_BE_4);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasIFReferenceFrequency());
            CHECK(packet_out.getIFReferenceFrequency() == IF_REF_FREQ_4);
        }
    }

    SECTION("RF Reference Frequency") {
        SECTION("Rule 9.5.10-2") {
            /*
             * The value of the RF Reference Frequency shall be expressed in
             * units of Hertz. The RF Reference Frequency value field shall use
             * the 64-bit, two’s-complement format as shown in Figure 9.5.10-1.
             * This subfield has an integer and a fractional part, with the
             * radix point to the right of bit 20 in the second 32-bit word.
             * 
             * Observation 9.5.10-1: The value of the RF Reference Frequency
             * field has a range of +/-8.79 terahertz with a resolution of 0.95
             * micro-Hertz.
             * 
             * Observation 9.5.10-2: An RF Reference Frequency field value of
             * 0x0000 0000 0010 0000 represents a frequency of +1 Hz. An RF
             * Reference Frequency field value of 0xFFFF FFFF FFF0 0000
             * represents a frequency of -1 Hz. An RF Reference Frequency field
             * value of 0x0000 0000 0000 0001 represents a frequency of +0.95
             * microhertz. An RF Reference Frequency field value of
             * 0xFFFF FFFF FFFF FFFF represents a frequency of -0.95 micro-Hertz.
             */
            const double RF_REF_FREQ_1 = 1.0; // represents 0x0000 0000 0010 0000
            const double RF_REF_FREQ_2 = 0.95e-6; // represents 0x0000 0000 0000 0001
            const double RF_REF_FREQ_3 = -1.0; // represents 0xFFFF FFFF FFF0 0000
            const double RF_REF_FREQ_4 = -0.95e-6; // represents 0xFFFF FFFF FFFF FFFF
            const int64_t RF_REF_FREQ_INT_BE_1 = 0x0000100000000000;
            const int64_t RF_REF_FREQ_INT_BE_2 = 0x0100000000000000;
            const int64_t RF_REF_FREQ_INT_BE_3 = 0x0000F0FFFFFFFFFF;
            const int64_t RF_REF_FREQ_INT_BE_4 = 0xFFFFFFFFFFFFFFFF;
            const size_t PACKED_SIZE = 20;

            // RF Reference Frequency 1
            packet_type packet_in;
            packet_in.setRFReferenceFrequency(RF_REF_FREQ_1);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_INT_BE_1);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasRFReferenceFrequency());
            CHECK(packet_out.getRFReferenceFrequency() == RF_REF_FREQ_1);

            // IF Reference Frequency 2
            packet_in = packet_type();
            packet_in.setRFReferenceFrequency(RF_REF_FREQ_2);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_INT_BE_2);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasRFReferenceFrequency());
            CHECK(packet_out.getRFReferenceFrequency() == RF_REF_FREQ_2);

            // IF Reference Frequency 3
            packet_in = packet_type();
            packet_in.setRFReferenceFrequency(RF_REF_FREQ_3);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_INT_BE_3);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasRFReferenceFrequency());
            CHECK(packet_out.getRFReferenceFrequency() == RF_REF_FREQ_3);

            // IF Reference Frequency 4
            packet_in = packet_type();
            packet_in.setRFReferenceFrequency(RF_REF_FREQ_4);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_INT_BE_4);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasRFReferenceFrequency());
            CHECK(packet_out.getRFReferenceFrequency() == RF_REF_FREQ_4);
        }
    }

    SECTION("RF Reference Frequency Offset") {
        SECTION("Rule 9.5.11-3") {
            /*
             * The value of the RF Reference Frequency Offset shall be
             * expressed in units of Hertz. The RF Reference Frequency Offset
             * field shall use the 64-bit, two’s-complement format shown in
             * Figure 9.5.11-1. This field has an integer and a fractional
             * part, with the radix point to the right of bit 20 in the second
             * 32-bit word.
             * 
             * Observation 9.5.11-3: The value of the RF Reference Frequency
             * Offset field has a range of +/-8.79 terahertz with a resolution
             * of 0.95 micro-Hertz.
             * 
             * Observation 9.5.11-4: An RF Reference Frequency Offset field
             * value of 0x0000 0000 0010 0000 represents a frequency offset of
             * +1 Hz. An RF Reference Frequency Offset field value of
             * 0xFFFF FFFF FFF0 0000 represents a frequency offset of -1 Hz. An
             * RF Reference Frequency Offset field value of 0x0000 0000 0000 0001
             * represents a frequency offset of +0.95 micro-Hertz. An RF
             * Reference Frequency Offset field value of 0xFFFF FFFF FFFF FFFF
             * represents a frequency offset of -0.95 micro-Hertz.
             */
            const double RF_REF_FREQ_OFFSET_1 = 1.0; // represents 0x0000 0000 0010 0000
            const double RF_REF_FREQ_OFFSET_2 = 0.95e-6; // represents 0x0000 0000 0000 0001
            const double RF_REF_FREQ_OFFSET_3 = -1.0; // represents 0xFFFF FFFF FFF0 0000
            const double RF_REF_FREQ_OFFSET_4 = -0.95e-6; // represents 0xFFFF FFFF FFFF FFFF
            const int64_t RF_REF_FREQ_OFFSET_INT_BE_1 = 0x0000100000000000;
            const int64_t RF_REF_FREQ_OFFSET_INT_BE_2 = 0x0100000000000000;
            const int64_t RF_REF_FREQ_OFFSET_INT_BE_3 = 0x0000F0FFFFFFFFFF;
            const int64_t RF_REF_FREQ_OFFSET_INT_BE_4 = 0xFFFFFFFFFFFFFFFF;
            const size_t PACKED_SIZE = 20;

            // RF Reference Frequency Offset 1
            packet_type packet_in;
            packet_in.setRFReferenceFrequencyOffset(RF_REF_FREQ_OFFSET_1);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_OFFSET_INT_BE_1);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasRFReferenceFrequencyOffset());
            CHECK(packet_out.getRFReferenceFrequencyOffset() == RF_REF_FREQ_OFFSET_1);

            // RF Reference Frequency Offset 2
            packet_in = packet_type();
            packet_in.setRFReferenceFrequencyOffset(RF_REF_FREQ_OFFSET_2);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_OFFSET_INT_BE_2);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasRFReferenceFrequencyOffset());
            CHECK(packet_out.getRFReferenceFrequencyOffset() == RF_REF_FREQ_OFFSET_2);

            // RF Reference Frequency Offset 3
            packet_in = packet_type();
            packet_in.setRFReferenceFrequencyOffset(RF_REF_FREQ_OFFSET_3);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_OFFSET_INT_BE_3);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasRFReferenceFrequencyOffset());
            CHECK(packet_out.getRFReferenceFrequencyOffset() == RF_REF_FREQ_OFFSET_3);

            // RF Reference Frequency Offset 4
            packet_in = packet_type();
            packet_in.setRFReferenceFrequencyOffset(RF_REF_FREQ_OFFSET_4);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == RF_REF_FREQ_OFFSET_INT_BE_4);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasRFReferenceFrequencyOffset());
            CHECK(packet_out.getRFReferenceFrequencyOffset() == RF_REF_FREQ_OFFSET_4);
        }
    }

    SECTION("IF Band Offset") {
        SECTION("Rule 9.5.4-2") {
            /*
             * The value of the IF Band Offset shall be expressed in units of
             * Hertz. The IF Band Offset field shall use the 64-bit,
             * two’s-complement format shown in Figure 9.5.4-1. This field has
             * an integer and a fractional part, with the radix point to the
             * right of bit 20 in the second 32-bit word.
             * 
             * Observation 9.5.4-2: The value of the IF Band Offset field has a
             * range of +/-8.79 terahertz with a resolution of 0.95 micro-Hertz.
             * 
             * Observation 9.5.4-3: A IF Band Offset field value of
             * 0x0000 0000 0010 0000 represents a band offset of +1 Hz. A IF
             * Band Offset field value of 0xFFFF FFFF FFF0 0000 represents a
             * band offset of -1 Hz. An IF Band Offset field value of
             * 0x0000 0000 0000 0001 represents a band offset of +0.95
             * micro-Hertz. An IF Band Offset field value of 0xFFFF FFFF FFFF FFFF
             * represents a band offset of –0.95 micro-Hertz.
             */
            const double IF_BAND_OFFSET_1 = 1.0; // represents 0x0000 0000 0010 0000
            const double IF_BAND_OFFSET_2 = 0.95e-6; // represents 0x0000 0000 0000 0001
            const double IF_BAND_OFFSET_3 = -1.0; // represents 0xFFFF FFFF FFF0 0000
            const double IF_BAND_OFFSET_4 = -0.95e-6; // represents 0xFFFF FFFF FFFF FFFF
            const int64_t IF_BAND_OFFSET_INT_BE_1 = 0x0000100000000000;
            const int64_t IF_BAND_OFFSET_INT_BE_2 = 0x0100000000000000;
            const int64_t IF_BAND_OFFSET_INT_BE_3 = 0x0000F0FFFFFFFFFF;
            const int64_t IF_BAND_OFFSET_INT_BE_4 = 0xFFFFFFFFFFFFFFFF;
            const size_t PACKED_SIZE = 20;

            // IF Band Offset 1
            packet_type packet_in;
            packet_in.setIFBandOffset(IF_BAND_OFFSET_1);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_BAND_OFFSET_INT_BE_1);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasIFBandOffset());
            CHECK(packet_out.getIFBandOffset() == IF_BAND_OFFSET_1);

            // IF Band Offset 2
            packet_in = packet_type();
            packet_in.setIFBandOffset(IF_BAND_OFFSET_2);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_BAND_OFFSET_INT_BE_2);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasIFBandOffset());
            CHECK(packet_out.getIFBandOffset() == IF_BAND_OFFSET_2);

            // IF band Offset 3
            packet_in = packet_type();
            packet_in.setIFBandOffset(IF_BAND_OFFSET_3);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_BAND_OFFSET_INT_BE_3);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasIFBandOffset());
            CHECK(packet_out.getIFBandOffset() == IF_BAND_OFFSET_3);

            // IF Band Offset 4
            packet_in = packet_type();
            packet_in.setIFBandOffset(IF_BAND_OFFSET_4);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == IF_BAND_OFFSET_INT_BE_4);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasIFBandOffset());
            CHECK(packet_out.getIFBandOffset() == IF_BAND_OFFSET_4);
        }
    }

    SECTION("Reference Level") {
        SECTION("Rule 9.5.9-2") {
            /*
             * The value of the Reference Level field shall be expressed in
             * units of dBm, where 0 dBm is one milliWatt (0.001 watt) measured
             * with respect to a load termination of 50 ohms. The Reference
             * Level field shall use the 32-bit format shown in Figure 9.5.9-2.
             * The upper 16 bits of this field are reserved and shall be set to
             * zero. The Reference Level value shall be expressed in
             * two’s-complement format in the lower 16 bits of this field. This
             * field has an integer and a fractional part, with the radix point
             * to the right of bit 7.
             * 
             * Observation 9.5.9-2: The value of the Reference Level field has
             * a range of nearly +/-256 dBm with a resolution of 1/128 dBm
             * (0.0078125 dBm).
             * 
             * Observation 9.5.9-3: A Reference Level field value of 0x0000 0080
             * represents a reference level of +1 dBm. A Reference Level field
             * value of 0x0000 FF80 represents a reference level of -1 dBm. A
             * Reference Level field value of 0x0000 0001 represents a reference
             * level of +0.0078125 dBm. A Reference Level field value of
             * 0x0000 FFFF represents a reference level of -0.0078125 dBm.
             */
            const float REF_LEVEL_1 = 1.0; // represents 0x0000 0080
            const float REF_LEVEL_2 = 0.0078125; // represents 0x0000 0001
            const float REF_LEVEL_3 = -1.0; // represents 0x0000 FF80
            const float REF_LEVEL_4 = -0.0078125; // represents 0x0000 FFFF
            const int32_t REF_LEVEL_INT_BE_1 = 0x80000000;
            const int32_t REF_LEVEL_INT_BE_2 = 0x01000000;
            const int32_t REF_LEVEL_INT_BE_3 = 0x80FF0000;
            const int32_t REF_LEVEL_INT_BE_4 = 0xFFFF0000;
            const size_t PACKED_SIZE = 16;

            // Reference Level 1
            packet_type packet_in;
            packet_in.setReferenceLevel(REF_LEVEL_1);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == REF_LEVEL_INT_BE_1);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasReferenceLevel());
            CHECK(packet_out.getReferenceLevel() == REF_LEVEL_1);

            // Reference Level 2
            packet_in = packet_type();
            packet_in.setReferenceLevel(REF_LEVEL_2);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == REF_LEVEL_INT_BE_2);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasReferenceLevel());
            CHECK(packet_out.getReferenceLevel() == REF_LEVEL_2);

            // Reference Level 3
            packet_in = packet_type();
            packet_in.setReferenceLevel(REF_LEVEL_3);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == REF_LEVEL_INT_BE_3);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasReferenceLevel());
            CHECK(packet_out.getReferenceLevel() == REF_LEVEL_3);

            // Reference Level 4
            packet_in = packet_type();
            packet_in.setReferenceLevel(REF_LEVEL_4);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == REF_LEVEL_INT_BE_4);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasReferenceLevel());
            CHECK(packet_out.getReferenceLevel() == REF_LEVEL_4);
        }
    }

    SECTION("Gain") {
        SECTION("Rule 9.5.3-3") {
            /*
             * The Gain field shall use the 32-bit format shown in Figure
             * 9.5.3-1, containing two gain subfields.
             */
            // Observation 9.5.3-5
            const float GAIN_1 = 1.0; // represents 0x0080 0080
            const float GAIN_2 = -1; // represents 0xFF80 FF80
            const float GAIN_3 = 0.0078125; // represents 0x0001 0001
            const float GAIN_4 = -0.0078125; // represents 0xFFFF FFFF
            const int32_t GAIN_INT_BE_1 = 0x80008000;
            const int32_t GAIN_INT_BE_2 = 0x80FF80FF;
            const int32_t GAIN_INT_BE_3 = 0x01000100;
            const int32_t GAIN_INT_BE_4 = 0xFFFFFFFF;
            const size_t PACKED_SIZE = 16;

            // Gain 1
            packet_type packet_in;
            vrtgen::packing::Gain gain;
            gain.setStage1(GAIN_1);
            gain.setStage2(GAIN_1);
            packet_in.setGain(gain);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == GAIN_INT_BE_1);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasGain());
            CHECK(packet_out.getGain().getStage1() == GAIN_1);
            CHECK(packet_out.getGain().getStage2() == GAIN_1);

            // Gain 2
            packet_in = packet_type();
            gain.setStage1(GAIN_2);
            gain.setStage2(GAIN_2);
            packet_in.setGain(gain);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == GAIN_INT_BE_2);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasGain());
            CHECK(packet_out.getGain().getStage1() == GAIN_2);
            CHECK(packet_out.getGain().getStage2() == GAIN_2);

            // Gain 3
            packet_in = packet_type();
            gain.setStage1(GAIN_3);
            gain.setStage2(GAIN_3);
            packet_in.setGain(gain);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == GAIN_INT_BE_3);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasGain());
            CHECK(packet_out.getGain().getStage1() == GAIN_3);
            CHECK(packet_out.getGain().getStage2() == GAIN_3);

            // Gain 4
            packet_in = packet_type();
            gain.setStage1(GAIN_4);
            gain.setStage2(GAIN_4);
            packet_in.setGain(gain);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const int32_t*>(&data[12])) == GAIN_INT_BE_4);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_out = packet_type();
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasGain());
            CHECK(packet_out.getGain().getStage1() == GAIN_4);
            CHECK(packet_out.getGain().getStage2() == GAIN_4);
        }
    }

    SECTION("Over-Range Count") {
        // Rule 9.10.6-3
        const uint32_t COUNT_1 = 0x12345678;
        const uint32_t COUNT_BE_1 = 0x78563412;
        const size_t PACKED_SIZE = 16;

        // Gain 1
        packet_type packet_in;
        packet_in.setOverRangeCount(COUNT_1);
        REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packet_helper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == COUNT_BE_1);

        CHECK(packet_helper::match(data.data(), data.size()));

        packet_type packet_out;
        packet_helper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasOverRangeCount());
        CHECK(packet_out.getOverRangeCount() == COUNT_1);
    }

    SECTION("Sample Rate") {
        // Observation 9.5.12-2
        const double SAMPLE_RATE_1 = 1.0; // represents 0x0000 0000 0010 0000
        const double SAMPLE_RATE_2 = 0.95e-6; // represents 0x0000 0000 0000 0001
        //const double SAMPLE_RATE_3 = -1.0; // invalid TODO
        const int64_t SAMPLE_RATE_INT_BE_1 = 0x0000100000000000;
        const int64_t SAMPLE_RATE_INT_BE_2 = 0x0100000000000000;
        const size_t PACKED_SIZE = 20;

        // Sample Rate 1
        packet_type packet_in;
        packet_in.setSampleRate(SAMPLE_RATE_1);
        REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packet_helper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == SAMPLE_RATE_INT_BE_1);

        CHECK(packet_helper::match(data.data(), data.size()));

        packet_type packet_out;
        packet_helper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasSampleRate());
        CHECK(packet_out.getSampleRate() == SAMPLE_RATE_1);

        // Sample Rate 2
        packet_in = packet_type();
        packet_in.setSampleRate(SAMPLE_RATE_2);
        REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packet_helper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == SAMPLE_RATE_INT_BE_2);

        CHECK(packet_helper::match(data.data(), data.size()));

        packet_out = packet_type();
        packet_helper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasSampleRate());
        CHECK(packet_out.getSampleRate() == SAMPLE_RATE_2);

        // Sample Rate 3 (invalid)
        // TODO
    }

    SECTION("Timestamp Adjustment") {
        // Rule 9.7-1, 9.7-2
        // Observation 9.7-4, 9.7-5
        const int64_t TS_ADJUSTMENT_1 = 0x1234567812345678;
        const int64_t TS_ADJUSTMENT_BE_1 = 0x7856341278563412;
        const int64_t TS_ADJUSTMENT_2 = 0x0000000000000001; // 1 femtosecond
        const int64_t TS_ADJUSTMENT_BE_2 = 0x0100000000000000;
        const size_t PACKED_SIZE = 20;

        // Timestamp Adjustment 1
        packet_type packet_in;
        packet_in.setTimestampAdjustment(TS_ADJUSTMENT_1);
        REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packet_helper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == TS_ADJUSTMENT_BE_1);

        CHECK(packet_helper::match(data.data(), data.size()));

        packet_type packet_out;
        packet_helper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasTimestampAdjustment());
        CHECK(packet_out.getTimestampAdjustment() == TS_ADJUSTMENT_1);

        // Timestamp Adjustment 2
        packet_in = packet_type();
        packet_in.setTimestampAdjustment(TS_ADJUSTMENT_2);

        REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packet_helper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == TS_ADJUSTMENT_BE_2);

        CHECK(packet_helper::match(data.data(), data.size()));

        packet_out = packet_type();
        packet_helper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasTimestampAdjustment());
        CHECK(packet_out.getTimestampAdjustment() == TS_ADJUSTMENT_2);
    }

    SECTION("Timestamp Calibration Time") {
        // Rule 9.7.3.3-2
        const uint32_t TS_CALIBRATION = 0x12345678;
        const uint32_t TS_CALIBRATION_BE = 0x78563412;
        const size_t PACKED_SIZE = 16;

        packet_type packet_in;
        packet_in.setTimestampCalibrationTime(TS_CALIBRATION);
        REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packet_helper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TS_CALIBRATION_BE);

        CHECK(packet_helper::match(data.data(), data.size()));

        packet_type packet_out;
        packet_helper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasTimestampCalibrationTime());
        CHECK(packet_out.getTimestampCalibrationTime() == TS_CALIBRATION);
    }

    SECTION("Temperature") {
        // Observation 9.10.5-2
        const float TEMP_1 = 1.0; // represents 0x0000 0040
        const float TEMP_2 = -1.0; // represents 0x0000 FFC0
        const float TEMP_3 = 0.015625; // represents 0x0000 0001
        const float TEMP_4 = -0.015625 ; // represents 0x0000 FFFF
        const uint32_t TEMP_BE_1 = 0x40000000;
        const uint32_t TEMP_BE_2 = 0xC0FF0000;
        const uint32_t TEMP_BE_3 = 0x01000000;
        const uint32_t TEMP_BE_4 = 0xFFFF0000;
        const size_t PACKED_SIZE = 16;

        // Temperature 1
        packet_type packet_in;
        packet_in.setTemperature(TEMP_1);
        REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packet_helper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TEMP_BE_1);

        CHECK(packet_helper::match(data.data(), data.size()));

        packet_type packet_out;
        packet_helper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasTemperature());
        CHECK(packet_out.getTemperature() == TEMP_1);

        // Temperature 2
        packet_in = packet_type();
        packet_in.setTemperature(TEMP_2);
        REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packet_helper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TEMP_BE_2);

        CHECK(packet_helper::match(data.data(), data.size()));

        packet_out = packet_type();
        packet_helper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasTemperature());
        CHECK(packet_out.getTemperature() == TEMP_2);

        // Temperature 3
        packet_in = packet_type();
        packet_in.setTemperature(TEMP_3);
        REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packet_helper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TEMP_BE_3);

        CHECK(packet_helper::match(data.data(), data.size()));

        packet_out = packet_type();
        packet_helper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasTemperature());
        CHECK(packet_out.getTemperature() == TEMP_3);

        // Temperature 4
        packet_in = packet_type();
        packet_in.setTemperature(TEMP_4);
        REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        data.resize(PACKED_SIZE);
        packet_helper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TEMP_BE_4);

        CHECK(packet_helper::match(data.data(), data.size()));

        packet_out = packet_type();
        packet_helper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasTemperature());
        CHECK(packet_out.getTemperature() == TEMP_4);
    }

    SECTION("Device Identifier") {
        const uint32_t OUI = 0x00123456;
        const int16_t DEV_CODE = 0x0000789A;
        const int64_t DEVICE_ID_BE = 0x9A78000056341200;
        const size_t PACKED_SIZE = 20;

        packet_type packet_in;
        vrtgen::packing::DeviceIdentifier devid;
        devid.setManufacturerOUI(OUI);
        devid.setDeviceCode(DEV_CODE);
        packet_in.setDeviceID(devid);
        REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packet_helper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == DEVICE_ID_BE);

        CHECK(packet_helper::match(data.data(), data.size()));

        packet_type packet_out;
        packet_helper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasDeviceID());
        CHECK(packet_out.getDeviceID().getManufacturerOUI() == OUI);
        CHECK(packet_out.getDeviceID().getDeviceCode() == DEV_CODE);
    }

    SECTION("State/Event Indicators") {
        // TODO - not currently supported
        CHECK(true);
    }

    SECTION("Signal Data Packet Payload Format") {
        // Observation 9.13.3-1
        // Creating 0x1001 1100 0010 0100 0000 0001 1000 0111
        const vrtgen::PackingMethod::Code METHOD = vrtgen::PackingMethod::LINK_EFFICIENT; // 1
        const vrtgen::DataSampleType::Code SAMPLE_TYPE = vrtgen::DataSampleType::REAL; // 00
        const vrtgen::DataItemFormat::Code ITEM_FORMAT = vrtgen::DataItemFormat::IEEE754_SINGLE_PRECISION; // 01110
        const bool REPEAT = false; // 0
        const uint8_t EVENT_TAG_SIZE = 1; // 001
        const uint8_t CHANNEL_TAG_SIZE = 2; // 0010
        const uint8_t FRACTION_SIZE = 0; // 0000
        const size_t PACKING_FIELD_SIZE = 4; // 000100 (-1 = 000011)
        const size_t DATA_ITEM_SIZE = 8; // 001000 (-1 = 000111)
        // Together = 1000 1110 0001 0010 0000 0000 1100 0111
        //          = 8    E    1    2    0    0    C    7
        const uint16_t REPEAT_COUNT = 0x1234; // (-1 = 0x1233)
        const uint16_t VECTOR_SIZE = 0x5678; // (-1 = 0x5677)
        const int64_t FORMAT_INT_BE = 0x77563312C700128E;
        const size_t PACKED_SIZE = 20;

        packet_type packet_in;
        vrtgen::packing::PayloadFormat format;
        format.setPackingMethod(METHOD);
        format.setRealComplexType(SAMPLE_TYPE);
        format.setDataItemFormat(ITEM_FORMAT);
        format.setSampleComponentRepeatIndicator(REPEAT);
        format.setEventTagSize(EVENT_TAG_SIZE);
        format.setChannelTagSize(CHANNEL_TAG_SIZE);
        format.setDataItemFractionSize(FRACTION_SIZE);
        format.setItemPackingFieldSize(PACKING_FIELD_SIZE);
        format.setDataItemSize(DATA_ITEM_SIZE);
        format.setRepeatCount(REPEAT_COUNT);
        format.setVectorSize(VECTOR_SIZE);
        packet_in.setSignalDataPacketPayloadFormat(format);
        REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packet_helper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const int64_t*>(&data[12])) == FORMAT_INT_BE);

        CHECK(packet_helper::match(data.data(), data.size()));

        packet_type packet_out;
        packet_helper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasSignalDataPacketPayloadFormat());
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getPackingMethod() == METHOD);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getRealComplexType() == SAMPLE_TYPE);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getDataItemFormat() == ITEM_FORMAT);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getSampleComponentRepeatIndicator() == REPEAT);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getEventTagSize() == EVENT_TAG_SIZE);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getChannelTagSize() == CHANNEL_TAG_SIZE);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getDataItemFractionSize() == FRACTION_SIZE);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getItemPackingFieldSize() == PACKING_FIELD_SIZE);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getDataItemSize() == DATA_ITEM_SIZE);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getRepeatCount() == REPEAT_COUNT);
        CHECK(packet_out.getSignalDataPacketPayloadFormat().getVectorSize() == VECTOR_SIZE);
    }

    SECTION("Formatted GPS") {
        // Rule 9.4.5-1
        const size_t PACKED_SIZE = 60;

        SECTION("Rule 9.4.5-2") {
            /*
             * The GPS/INS Manufacturer OUI subfield shall contain the 24-bit
             * field for the IEEE registered, Organizationally Unique Identifier
             * (company identifier) in the range 00-00-00 to FF-FE-FF
             * (inclusive) of the GPS/INS manufacturer.
             */
            const uint32_t OUI = 0x123456;
            const uint32_t OUI_BE = 0x56341200; // 00 for empty tsi and tsf

            packet_type packet_in;
            vrtgen::packing::Geolocation gps{};
            gps.setManufacturerOUI(OUI);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == OUI_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getManufacturerOUI() == OUI);
        }
    
        SECTION("Rule 9.4.5-3") {
            /*
             * The TSI field in the Formatted GPS Geolocation field shall
             * accurately indicate the type of Integer-seconds Timestamp
             * included in the packet according to the code assignments in
             * Table 9.4.5-1.
             * 
             * | TSI Code | Meaning   |
             * | -------- | --------- |
             * | 00       | Undefined |
             * | 01       | UTC       |
             * | 10       | GPS Time  |
             * | 11       | Other     |
             * | -------- | --------- |
             */
            SECTION("Undefined") {
                const vrtgen::TSI::Code TSI = vrtgen::TSI::NONE;
                const uint32_t TSI_BE = htonl(TSI << 26);

                packet_type packet_in;
                vrtgen::packing::Geolocation gps{};
                gps.setTSI(TSI);
                packet_in.setFormattedGPS(gps);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedGPS());
                CHECK(packet_out.getFormattedGPS().getTSI() == TSI);
            }
            SECTION("UTC") {
                const vrtgen::TSI::Code TSI = vrtgen::TSI::UTC;
                const uint32_t TSI_BE = htonl(TSI << 26);

                packet_type packet_in;
                vrtgen::packing::Geolocation gps{};
                gps.setTSI(TSI);
                packet_in.setFormattedGPS(gps);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedGPS());
                CHECK(packet_out.getFormattedGPS().getTSI() == TSI);
            }
            SECTION("GPS Time") {
                const vrtgen::TSI::Code TSI = vrtgen::TSI::GPS;
                const uint32_t TSI_BE = htonl(TSI << 26);

                packet_type packet_in;
                vrtgen::packing::Geolocation gps{};
                gps.setTSI(TSI);
                packet_in.setFormattedGPS(gps);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedGPS());
                CHECK(packet_out.getFormattedGPS().getTSI() == TSI);
            }
            SECTION("Other") {
                const vrtgen::TSI::Code TSI = vrtgen::TSI::OTHER;
                const uint32_t TSI_BE = htonl(TSI << 26);

                packet_type packet_in;
                vrtgen::packing::Geolocation gps{};
                gps.setTSI(TSI);
                packet_in.setFormattedGPS(gps);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedGPS());
                CHECK(packet_out.getFormattedGPS().getTSI() == TSI);
            }
        }

        SECTION("Rule 9.4.5-4") {
            /*
             * The TSF field in the Formatted GPS Geolocation field shall
             * accurately indicate the type of Fractional-seconds Timestamp
             * included in the packet according to the code assignments in
             * Table 9.4.5-2.
             * 
             * | TSF Code | Meaning                 |
             * | -------- | ----------------------- |
             * | 00       | Undefined               |
             * | 01       | Sample Count Time       |
             * | 10       | Real (Picoseconds) Time |
             * | 11       | Free-Running Count Time |
             * | -------- | ----------------------- |
             */
            SECTION("Undefined") {
                const vrtgen::TSF::Code TSF = vrtgen::TSF::NONE;
                const uint32_t TSF_BE = htonl(TSF << 24);

                packet_type packet_in;
                vrtgen::packing::Geolocation gps{};
                gps.setTSF(TSF);
                packet_in.setFormattedGPS(gps);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.clear();
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedGPS());
                CHECK(packet_out.getFormattedGPS().getTSF() == TSF);
            }
            SECTION("Sample Count Time") {
                const vrtgen::TSF::Code TSF = vrtgen::TSF::SAMPLE_COUNT;
                const uint32_t TSF_BE = htonl(TSF << 24);

                packet_type packet_in;
                vrtgen::packing::Geolocation gps{};
                gps.setTSF(TSF);
                packet_in.setFormattedGPS(gps);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedGPS());
                CHECK(packet_out.getFormattedGPS().getTSF() == TSF);
            }
            SECTION("Real (Picoseconds) Time") {
                const vrtgen::TSF::Code TSF = vrtgen::TSF::REAL_TIME;
                const uint32_t TSF_BE = htonl(TSF << 24);

                packet_type packet_in;
                vrtgen::packing::Geolocation gps{};
                gps.setTSF(TSF);
                packet_in.setFormattedGPS(gps);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedGPS());
                CHECK(packet_out.getFormattedGPS().getTSF() == TSF);
            }
            SECTION("Other") {
                const vrtgen::TSF::Code TSF = vrtgen::TSF::FREE_RUNNING;
                const uint32_t TSF_BE = htonl(TSF << 24);

                packet_type packet_in;
                vrtgen::packing::Geolocation gps{};
                gps.setTSF(TSF);
                packet_in.setFormattedGPS(gps);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedGPS());
                CHECK(packet_out.getFormattedGPS().getTSF() == TSF);
            }
        }

        SECTION("Rule 9.4.5-5") {
            /*
             * When the TSI or TSF fields are non-zero the corresponding
             * Timestamp of Position Fix subfield shall express the time of the
             * most recent location fix in the format given in Sections
             * 6.1.45.1.4 and 5.1.4.2
             */
            const vrtgen::TSI::Code TSI = vrtgen::TSI::UTC;
            const vrtgen::TSF::Code TSF = vrtgen::TSF::REAL_TIME;
            const uint32_t WORD_1_BE = htonl((TSI << 26) | (TSF << 24));
            const uint32_t INTEGER_TIMESTAMP = 0x12345678;
            const uint32_t INTEGER_TIMESTAMP_BE = 0x78563412;
            const uint64_t FRACTIONAL_TIMESTAMP = 0x12345678ABCDEF00;
            const uint64_t FRACTIONAL_TIMESTAMP_BE = 0xEFCDAB78563412;

            packet_type packet_in;
            vrtgen::packing::Geolocation gps{};
            gps.setTSI(TSI);
            gps.setTSF(TSF);
            gps.setIntegersecondTimestamp(INTEGER_TIMESTAMP);
            gps.setFractionalsecondTimestamp(FRACTIONAL_TIMESTAMP);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == WORD_1_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[16])) == INTEGER_TIMESTAMP_BE);
            CHECK(*(reinterpret_cast<const uint64_t*>(&data[20])) == FRACTIONAL_TIMESTAMP_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getTSI() == TSI);
            CHECK(packet_out.getFormattedGPS().getTSF() == TSF);
            CHECK(packet_out.getFormattedGPS().getIntegersecondTimestamp() == INTEGER_TIMESTAMP);
            CHECK(packet_out.getFormattedGPS().getFractionalsecondTimestamp() == FRACTIONAL_TIMESTAMP);
        }

        SECTION("Rule 9.4.5-6") {
            /*
             * When the TSI or TSF fields are zero the corresponding Timestamp
             * of Position Fix subfield words shall take the value 0xFFFFFFFF.
             */
            const vrtgen::TSI::Code TSI = vrtgen::TSI::NONE;
            const vrtgen::TSF::Code TSF = vrtgen::TSF::NONE;
            const uint32_t WORD_1_BE = htonl((TSI << 26) | (TSF << 24));
            const uint32_t INTEGER_TIMESTAMP_BE = 0xFFFFFFFF;
            const uint64_t FRACTIONAL_TIMESTAMP_BE = 0xFFFFFFFFFFFFFFFF;

            // Defaults
            packet_type packet_in;
            vrtgen::packing::Geolocation gps{};
            packet_in.setFormattedGPS(gps);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == WORD_1_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[16])) == INTEGER_TIMESTAMP_BE);
            CHECK(*(reinterpret_cast<const uint64_t*>(&data[20])) == FRACTIONAL_TIMESTAMP_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getTSI() == TSI);
            CHECK(packet_out.getFormattedGPS().getTSF() == TSF);

            // Set to zero
            gps.setTSI(TSI);
            gps.setTSF(TSF);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == WORD_1_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[16])) == INTEGER_TIMESTAMP_BE);
            CHECK(*(reinterpret_cast<const uint64_t*>(&data[20])) == FRACTIONAL_TIMESTAMP_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getTSI() == TSI);
            CHECK(packet_out.getFormattedGPS().getTSF() == TSF);
        }

        SECTION("Rule 9.4.5-7") {
            /*
             * The Latitude and Longitude subfields shall use the Geolocation
             * Angle Format shown in Figure 9.4.5-2.
             * 
             * Note: Geolocation Angle Format is 32-bit, two's complement with
             *       radix point to the right of bit 22
             */
            // Observation 9.4.5-2 - resolution of 2.38e-7 degrees
            const double LATITIUDE = 1.000000238;
            const uint32_t LATITIUDE_BE = htonl(0x00400001);
            const double LONGITUDE = 1.000000238;
            const uint32_t LONGITUDE_BE = htonl(0x00400001);

            packet_type packet_in;
            vrtgen::packing::Geolocation gps{};
            gps.setLatitude(LATITIUDE);
            gps.setLongitude(LONGITUDE);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[28])) == LATITIUDE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[32])) == LONGITUDE_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getLatitude() == LATITIUDE);
            CHECK(packet_out.getFormattedGPS().getLongitude() == LONGITUDE);
        }

        SECTION("Rule 9.4.5-8") {
            /*
             * The Latitude subfield in the GPS Geolocation Field value shall
             * range from -90.0 (South) to +90.0 (North) degrees.
             */
            const double LATITIUDE_MAX = 90.0; // hex(90<<22(radix)) = 0x16800000
            const uint32_t LATITIUDE_MAX_BE = htonl(0x16800000);
            const double LATITIUDE_MIN = -90.0; // hex(-90<<22(radix)) = 0xe9800000
            const uint32_t LATITIUDE_MIN_BE = htonl(0xe9800000);

            // Check max
            packet_type packet_in;
            vrtgen::packing::Geolocation gps{};
            gps.setLatitude(LATITIUDE_MAX);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[28])) == LATITIUDE_MAX_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getLatitude() == LATITIUDE_MAX);

            // Check min
            gps.setLatitude(LATITIUDE_MIN);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.clear();
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[28])) == LATITIUDE_MIN_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getLatitude() == LATITIUDE_MIN);
        }

        SECTION("Rule 9.4.5-9") {
            /*
             * The Longitude subfield in the GPS Geolocation Field value shall
             * range from -180.0 (West) to +180.0 (East) degrees.
             */
            const double LONGITUDE_MAX = 180.0; // hex(180<<22(radix)) = 0x2d000000
            const uint32_t LONGITUDE_MAX_BE = htonl(0x2d000000);
            const double LONGITUDE_MIN = -180.0; // hex(-180<<22(radix)) = 0xd3000000
            const uint32_t LONGITUDE_MIN_BE = htonl(0xd3000000);

            // Check max
            packet_type packet_in;
            vrtgen::packing::Geolocation gps{};
            gps.setLongitude(LONGITUDE_MAX);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[32])) == LONGITUDE_MAX_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getLongitude() == LONGITUDE_MAX);

            // Check min
            gps.setLongitude(LONGITUDE_MIN);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.clear();
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[32])) == LONGITUDE_MIN_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getLongitude() == LONGITUDE_MIN);
        }
    
        SECTION("Rule 9.4.5-10") {
            /*
             * The Altitude subfield in the GPS Geolocation Field shall use the
             * 32-bit, two’s-complement format shown in Figure 9.4.5-3 with the
             * value of the Altitude subfield expressed in units of meters. This
             * field has an integer and a fractional part with the radix point
             * to the right of bit 5.
             */
            // Observation 9.4.5-4: range +-67108 km, resolution 3.1 cm
            const double ALTITUDE = 1.031;
            const uint32_t ALTITUDE_BE = htonl(0x00000021);
            const double ALTITUDE_MAX = 67108; // hex(67108<<5(radix)) = 0x20c480
            const uint32_t ALTITUDE_MAX_BE = htonl(0x0020c480);
            const double ALTITUDE_MIN = -67108; // hex(-67108<<5(radix)) = 0xffdf3b80
            const uint32_t ALTITUDE_MIN_BE = htonl(0xffdf3b80);

            // Check resolution
            packet_type packet_in;
            vrtgen::packing::Geolocation gps{};
            gps.setAltitude(ALTITUDE);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[36])) == ALTITUDE_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getAltitude() == ALTITUDE);

            // Check max
            gps.setAltitude(ALTITUDE_MAX);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.clear();
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[36])) == ALTITUDE_MAX_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getAltitude() == ALTITUDE_MAX);

            // Check min
            gps.setAltitude(ALTITUDE_MIN);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.clear();
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[36])) == ALTITUDE_MIN_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getAltitude() == ALTITUDE_MIN);
        }
    
        SECTION("Rule 9.4.5-11") {
            /*
             * The Speed Over Ground subfield shall use the 32-bit,
             * two’s-complement format shown in Figure 9.4.5-4 with the value
             * of the Speed Over Ground subfield expressed in units of meters
             * per second. This field has an integer and a fractional part with
             * the radix point to the right of bit 16.
             */
            // Observation 9.4.5-5: range 0 to 65636 m/s, resolution 1.5e-5 m/s
            // Developer's Note: above observation believed to be typo, max
            // signed integer part can only be 32767 (0x7FFF), however this is
            // also the unspecified value from Rule 9.4.5-18. Instead using
            // 0x3FFF for test, which is still magnitudes higher than reasonable
            // live values.
            const double SPEED_OVER_GROUND = 1.000015;
            const uint32_t SPEED_OVER_GROUND_BE = htonl(0x00010001);
            const double SPEED_OVER_GROUND_MAX = 16383.999985;
            const uint32_t SPEED_OVER_GROUND_MAX_BE = htonl(0x3FFFFFFF);
            const double SPEED_OVER_GROUND_MIN = 0;
            const uint32_t SPEED_OVER_GROUND_MIN_BE = 0;

            // Check resolution
            packet_type packet_in;
            vrtgen::packing::Geolocation gps{};
            gps.setSpeedOverGround(SPEED_OVER_GROUND);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[40])) == SPEED_OVER_GROUND_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getSpeedOverGround() == SPEED_OVER_GROUND);

            // Check max
            gps.setSpeedOverGround(SPEED_OVER_GROUND_MAX);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[40])) == SPEED_OVER_GROUND_MAX_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getSpeedOverGround() == SPEED_OVER_GROUND_MAX);

            // Check min
            gps.setSpeedOverGround(SPEED_OVER_GROUND_MIN);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[40])) == SPEED_OVER_GROUND_MIN_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getSpeedOverGround() == SPEED_OVER_GROUND_MIN);
        }
    
        SECTION("Rule 9.4.5-12") {
            /*
             * The Heading Angle subfield shall use the Geolocation Angle
             * Format shown in Figure 9.4.5-2, with the Heading Angle
             * expressing the platform’s orientation with respect to true North
             * in decimal degrees.
             * 
             * Note: Geolocation Angle Format is 32-bit, two's complement with
             *       radix point to the right of bit 22
             */
            // Observation 9.4.5-2 - resolution of 2.38e-7 degrees
            const double HEADING_ANGLE = 1.000000238;
            const uint32_t HEADING_ANGLE_BE = htonl(0x00400001);

            packet_type packet_in;
            vrtgen::packing::Geolocation gps{};
            gps.setHeadingAngle(HEADING_ANGLE);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[44])) == HEADING_ANGLE_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getHeadingAngle() == HEADING_ANGLE);
        }
    
        SECTION("Rule 9.4.5-13") {
            /*
             * The Heading Angle value shall range from 0.0 to
             * +359.999999761582 degrees.
             */
            // Note: with resolution of 2.38e-7 from Rule 9.4.5-12, the above
            //       maximum is out of reach
            const double HEADING_ANGLE_MAX = 359.999999762;
            const uint32_t HEADING_ANGLE_MAX_BE = htonl(0x59FFFFFF);
            const double HEADING_ANGLE_MIN = 0.0;
            const uint32_t HEADING_ANGLE_MIN_BE = 0x0;

            // Check max
            packet_type packet_in;
            vrtgen::packing::Geolocation gps{};
            gps.setHeadingAngle(HEADING_ANGLE_MAX);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[44])) == HEADING_ANGLE_MAX_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getHeadingAngle() == HEADING_ANGLE_MAX);

            // Check min
            gps.setHeadingAngle(HEADING_ANGLE_MIN);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[44])) == HEADING_ANGLE_MIN_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getHeadingAngle() == HEADING_ANGLE_MIN);
        }
    
        SECTION("Rule 9.4.5-14") {
            /*
             * The Track Angle subfield shall use the Geolocation Angle Format
             * shown in Figure 9.4.5-2, with the Track Angle expressing the
             * platform’s direction of travel with respect to true North in
             * decimal degrees.
             * 
             * Note: Geolocation Angle Format is 32-bit, two's complement with
             *       radix point to the right of bit 22
             */
            // Observation 9.4.5-2 - resolution of 2.38e-7 degrees
            const double TRACK_ANGLE = 1.000000238;
            const uint32_t TRACK_ANGLE_BE = htonl(0x00400001);

            packet_type packet_in;
            vrtgen::packing::Geolocation gps{};
            gps.setTrackAngle(TRACK_ANGLE);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[48])) == TRACK_ANGLE_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getTrackAngle() == TRACK_ANGLE);
        }
    
        SECTION("Rule 9.4.5-15") {
            /*
             * The Track Angle value shall range from 0.0 to +359.999999761582
             * degrees.
             */
            // Note: with resolution of 2.38e-7 from Rule 9.4.5-14, the above
            //       maximum is out of reach
            const double TRACK_ANGLE_MAX = 359.999999762;
            const uint32_t TRACK_ANGLE_MAX_BE = htonl(0x59FFFFFF);
            const double TRACK_ANGLE_MIN = 0.0;
            const uint32_t TRACK_ANGLE_MIN_BE = 0x0;

            // Check max
            packet_type packet_in;
            vrtgen::packing::Geolocation gps{};
            gps.setTrackAngle(TRACK_ANGLE_MAX);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[48])) == TRACK_ANGLE_MAX_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getTrackAngle() == TRACK_ANGLE_MAX);

            // Check min
            gps.setTrackAngle(TRACK_ANGLE_MIN);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[48])) == TRACK_ANGLE_MIN_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getTrackAngle() == TRACK_ANGLE_MIN);
        }
    
        SECTION("Rule 9.4.5-16") {
            /*
             * The Magnetic Variation subfield shall use the Geolocation Angle
             * Format shown in Figure 9.4.5-2, above, with the Magnetic
             * Variation subfield expressing magnetic variation from true North
             * in decimal degrees.
             * 
             * Note: Geolocation Angle Format is 32-bit, two's complement with
             *       radix point to the right of bit 22
             */
            // Observation 9.4.5-2 - resolution of 2.38e-7 degrees
            const double MAGNETIC_VARIATION = 1.000000238;
            const uint32_t MAGNETIC_VARIATION_BE = htonl(0x00400001);

            packet_type packet_in;
            vrtgen::packing::Geolocation gps{};
            gps.setMagneticVariation(MAGNETIC_VARIATION);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[52])) == MAGNETIC_VARIATION_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getMagneticVariation() == MAGNETIC_VARIATION);
        }
    
        SECTION("Rule 9.4.5-17") {
            /*
             * The Magnetic Variation value shall range from -180.0 (West) to
             * +180.0 (East) degrees.
             */
            const double MAGNETIC_VARIATION_MAX = 180.0; // hex(180<<22(radix)) = 0x2d000000
            const uint32_t MAGNETIC_VARIATION_MAX_BE = htonl(0x2d000000);
            const double MAGNETIC_VARIATION_MIN = -180.0; // hex(-180<<22(radix)) = 0xd3000000
            const uint32_t MAGNETIC_VARIATION_MIN_BE = htonl(0xd3000000);

            // Check max
            packet_type packet_in;
            vrtgen::packing::Geolocation gps{};
            gps.setMagneticVariation(MAGNETIC_VARIATION_MAX);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[52])) == MAGNETIC_VARIATION_MAX_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getMagneticVariation() == MAGNETIC_VARIATION_MAX);

            // Check min
            gps.setMagneticVariation(MAGNETIC_VARIATION_MIN);
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.clear();
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[52])) == MAGNETIC_VARIATION_MIN_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
            CHECK(packet_out.getFormattedGPS().getMagneticVariation() == MAGNETIC_VARIATION_MIN);
        }
    
        SECTION("Rule 9.4.5-18") {
            /*
             * The Latitude, Longitude, Altitude, Speed Over Ground, Heading,
             * Track Angle, and Magnetic Variation subfields shall take the
             * value 0x7FFFFFFF when unspecified.
             */
            // Observation 9.4.5-7: These default subfield values are invalid
            // or practically impossible for all GPS and INS Geolocation
            // subfields.
            const uint32_t UNSPECIFIED_VALUE = 0x7FFFFFFF;
            const uint32_t UNSPECIFIED_VALUE_BE = htonl(UNSPECIFIED_VALUE);

            // Check default (unset)
            packet_type packet_in;
            vrtgen::packing::Geolocation gps{};
            packet_in.setFormattedGPS(gps);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[28])) == UNSPECIFIED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[32])) == UNSPECIFIED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[36])) == UNSPECIFIED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[40])) == UNSPECIFIED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[44])) == UNSPECIFIED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[48])) == UNSPECIFIED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[52])) == UNSPECIFIED_VALUE_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedGPS());
        }
    
    }

    SECTION("Formatted INS") {
        /*
        * Rule 9.4.6-1: The Formatted INS Geolocation field shall follow the
        * same rules as the Formatted GPS Geolocation field.
        * 
        * Developer's Note: Using same tests as above
        */
        const size_t PACKED_SIZE = 60;

        SECTION("Rule 9.4.5-2") {
            /*
             * The GPS/INS Manufacturer OUI subfield shall contain the 24-bit
             * field for the IEEE registered, Organizationally Unique Identifier
             * (company identifier) in the range 00-00-00 to FF-FE-FF
             * (inclusive) of the GPS/INS manufacturer.
             */
            const uint32_t OUI = 0x123456;
            const uint32_t OUI_BE = 0x56341200; // 00 for empty tsi and tsf

            packet_type packet_in;
            vrtgen::packing::Geolocation ins{};
            ins.setManufacturerOUI(OUI);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == OUI_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getManufacturerOUI() == OUI);
        }
    
        SECTION("Rule 9.4.5-3") {
            /*
             * The TSI field in the Formatted GPS Geolocation field shall
             * accurately indicate the type of Integer-seconds Timestamp
             * included in the packet according to the code assignments in
             * Table 9.4.5-1.
             * 
             * | TSI Code | Meaning   |
             * | -------- | --------- |
             * | 00       | Undefined |
             * | 01       | UTC       |
             * | 10       | GPS Time  |
             * | 11       | Other     |
             * | -------- | --------- |
             */
            SECTION("Undefined") {
                const vrtgen::TSI::Code TSI = vrtgen::TSI::NONE;
                const uint32_t TSI_BE = htonl(TSI << 26);

                packet_type packet_in;
                vrtgen::packing::Geolocation ins{};
                ins.setTSI(TSI);
                packet_in.setFormattedINS(ins);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedINS());
                CHECK(packet_out.getFormattedINS().getTSI() == TSI);
            }
            SECTION("UTC") {
                const vrtgen::TSI::Code TSI = vrtgen::TSI::UTC;
                const uint32_t TSI_BE = htonl(TSI << 26);

                packet_type packet_in;
                vrtgen::packing::Geolocation ins{};
                ins.setTSI(TSI);
                packet_in.setFormattedINS(ins);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedINS());
                CHECK(packet_out.getFormattedINS().getTSI() == TSI);
            }
            SECTION("GPS Time") {
                const vrtgen::TSI::Code TSI = vrtgen::TSI::GPS;
                const uint32_t TSI_BE = htonl(TSI << 26);

                packet_type packet_in;
                vrtgen::packing::Geolocation ins{};
                ins.setTSI(TSI);
                packet_in.setFormattedINS(ins);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedINS());
                CHECK(packet_out.getFormattedINS().getTSI() == TSI);
            }
            SECTION("Other") {
                const vrtgen::TSI::Code TSI = vrtgen::TSI::OTHER;
                const uint32_t TSI_BE = htonl(TSI << 26);

                packet_type packet_in;
                vrtgen::packing::Geolocation ins{};
                ins.setTSI(TSI);
                packet_in.setFormattedINS(ins);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedINS());
                CHECK(packet_out.getFormattedINS().getTSI() == TSI);
            }
        }

        SECTION("Rule 9.4.5-4") {
            /*
             * The TSF field in the Formatted GPS Geolocation field shall
             * accurately indicate the type of Fractional-seconds Timestamp
             * included in the packet according to the code assignments in
             * Table 9.4.5-2.
             * 
             * | TSF Code | Meaning                 |
             * | -------- | ----------------------- |
             * | 00       | Undefined               |
             * | 01       | Sample Count Time       |
             * | 10       | Real (Picoseconds) Time |
             * | 11       | Free-Running Count Time |
             * | -------- | ----------------------- |
             */
            SECTION("Undefined") {
                const vrtgen::TSF::Code TSF = vrtgen::TSF::NONE;
                const uint32_t TSF_BE = htonl(TSF << 24);

                packet_type packet_in;
                vrtgen::packing::Geolocation ins{};
                ins.setTSF(TSF);
                packet_in.setFormattedINS(ins);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.clear();
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedINS());
                CHECK(packet_out.getFormattedINS().getTSF() == TSF);
            }
            SECTION("Sample Count Time") {
                const vrtgen::TSF::Code TSF = vrtgen::TSF::SAMPLE_COUNT;
                const uint32_t TSF_BE = htonl(TSF << 24);

                packet_type packet_in;
                vrtgen::packing::Geolocation ins{};
                ins.setTSF(TSF);
                packet_in.setFormattedINS(ins);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedINS());
                CHECK(packet_out.getFormattedINS().getTSF() == TSF);
            }
            SECTION("Real (Picoseconds) Time") {
                const vrtgen::TSF::Code TSF = vrtgen::TSF::REAL_TIME;
                const uint32_t TSF_BE = htonl(TSF << 24);

                packet_type packet_in;
                vrtgen::packing::Geolocation ins{};
                ins.setTSF(TSF);
                packet_in.setFormattedINS(ins);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedINS());
                CHECK(packet_out.getFormattedINS().getTSF() == TSF);
            }
            SECTION("Other") {
                const vrtgen::TSF::Code TSF = vrtgen::TSF::FREE_RUNNING;
                const uint32_t TSF_BE = htonl(TSF << 24);

                packet_type packet_in;
                vrtgen::packing::Geolocation ins{};
                ins.setTSF(TSF);
                packet_in.setFormattedINS(ins);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasFormattedINS());
                CHECK(packet_out.getFormattedINS().getTSF() == TSF);
            }
        }

        SECTION("Rule 9.4.5-5") {
            /*
             * When the TSI or TSF fields are non-zero the corresponding
             * Timestamp of Position Fix subfield shall express the time of the
             * most recent location fix in the format given in Sections
             * 6.1.45.1.4 and 5.1.4.2
             */
            const vrtgen::TSI::Code TSI = vrtgen::TSI::UTC;
            const vrtgen::TSF::Code TSF = vrtgen::TSF::REAL_TIME;
            const uint32_t WORD_1_BE = htonl((TSI << 26) | (TSF << 24));
            const uint32_t INTEGER_TIMESTAMP = 0x12345678;
            const uint32_t INTEGER_TIMESTAMP_BE = 0x78563412;
            const uint64_t FRACTIONAL_TIMESTAMP = 0x12345678ABCDEF00;
            const uint64_t FRACTIONAL_TIMESTAMP_BE = 0xEFCDAB78563412;

            packet_type packet_in;
            vrtgen::packing::Geolocation ins{};
            ins.setTSI(TSI);
            ins.setTSF(TSF);
            ins.setIntegersecondTimestamp(INTEGER_TIMESTAMP);
            ins.setFractionalsecondTimestamp(FRACTIONAL_TIMESTAMP);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == WORD_1_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[16])) == INTEGER_TIMESTAMP_BE);
            CHECK(*(reinterpret_cast<const uint64_t*>(&data[20])) == FRACTIONAL_TIMESTAMP_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getTSI() == TSI);
            CHECK(packet_out.getFormattedINS().getTSF() == TSF);
            CHECK(packet_out.getFormattedINS().getIntegersecondTimestamp() == INTEGER_TIMESTAMP);
            CHECK(packet_out.getFormattedINS().getFractionalsecondTimestamp() == FRACTIONAL_TIMESTAMP);
        }

        SECTION("Rule 9.4.5-6") {
            /*
             * When the TSI or TSF fields are zero the corresponding Timestamp
             * of Position Fix subfield words shall take the value 0xFFFFFFFF.
             */
            const vrtgen::TSI::Code TSI = vrtgen::TSI::NONE;
            const vrtgen::TSF::Code TSF = vrtgen::TSF::NONE;
            const uint32_t WORD_1_BE = htonl((TSI << 26) | (TSF << 24));
            const uint32_t INTEGER_TIMESTAMP_BE = 0xFFFFFFFF;
            const uint64_t FRACTIONAL_TIMESTAMP_BE = 0xFFFFFFFFFFFFFFFF;

            // Defaults
            packet_type packet_in;
            vrtgen::packing::Geolocation ins{};
            packet_in.setFormattedINS(ins);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == WORD_1_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[16])) == INTEGER_TIMESTAMP_BE);
            CHECK(*(reinterpret_cast<const uint64_t*>(&data[20])) == FRACTIONAL_TIMESTAMP_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getTSI() == TSI);
            CHECK(packet_out.getFormattedINS().getTSF() == TSF);

            // Set to zero
            ins.setTSI(TSI);
            ins.setTSF(TSF);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == WORD_1_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[16])) == INTEGER_TIMESTAMP_BE);
            CHECK(*(reinterpret_cast<const uint64_t*>(&data[20])) == FRACTIONAL_TIMESTAMP_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getTSI() == TSI);
            CHECK(packet_out.getFormattedINS().getTSF() == TSF);
        }

        SECTION("Rule 9.4.5-7") {
            /*
             * The Latitude and Longitude subfields shall use the Geolocation
             * Angle Format shown in Figure 9.4.5-2.
             * 
             * Note: Geolocation Angle Format is 32-bit, two's complement with
             *       radix point to the right of bit 22
             */
            // Observation 9.4.5-2 - resolution of 2.38e-7 degrees
            const double LATITIUDE = 1.000000238;
            const uint32_t LATITIUDE_BE = htonl(0x00400001);
            const double LONGITUDE = 1.000000238;
            const uint32_t LONGITUDE_BE = htonl(0x00400001);

            packet_type packet_in;
            vrtgen::packing::Geolocation ins{};
            ins.setLatitude(LATITIUDE);
            ins.setLongitude(LONGITUDE);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[28])) == LATITIUDE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[32])) == LONGITUDE_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getLatitude() == LATITIUDE);
            CHECK(packet_out.getFormattedINS().getLongitude() == LONGITUDE);
        }

        SECTION("Rule 9.4.5-8") {
            /*
             * The Latitude subfield in the GPS Geolocation Field value shall
             * range from -90.0 (South) to +90.0 (North) degrees.
             */
            const double LATITIUDE_MAX = 90.0; // hex(90<<22(radix)) = 0x16800000
            const uint32_t LATITIUDE_MAX_BE = htonl(0x16800000);
            const double LATITIUDE_MIN = -90.0; // hex(-90<<22(radix)) = 0xe9800000
            const uint32_t LATITIUDE_MIN_BE = htonl(0xe9800000);

            // Check max
            packet_type packet_in;
            vrtgen::packing::Geolocation ins{};
            ins.setLatitude(LATITIUDE_MAX);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[28])) == LATITIUDE_MAX_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getLatitude() == LATITIUDE_MAX);

            // Check min
            ins.setLatitude(LATITIUDE_MIN);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.clear();
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[28])) == LATITIUDE_MIN_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getLatitude() == LATITIUDE_MIN);
        }

        SECTION("Rule 9.4.5-9") {
            /*
             * The Longitude subfield in the GPS Geolocation Field value shall
             * range from -180.0 (West) to +180.0 (East) degrees.
             */
            const double LONGITUDE_MAX = 180.0; // hex(180<<22(radix)) = 0x2d000000
            const uint32_t LONGITUDE_MAX_BE = htonl(0x2d000000);
            const double LONGITUDE_MIN = -180.0; // hex(-180<<22(radix)) = 0xd3000000
            const uint32_t LONGITUDE_MIN_BE = htonl(0xd3000000);

            // Check max
            packet_type packet_in;
            vrtgen::packing::Geolocation ins{};
            ins.setLongitude(LONGITUDE_MAX);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[32])) == LONGITUDE_MAX_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getLongitude() == LONGITUDE_MAX);

            // Check min
            ins.setLongitude(LONGITUDE_MIN);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.clear();
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[32])) == LONGITUDE_MIN_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getLongitude() == LONGITUDE_MIN);
        }
    
        SECTION("Rule 9.4.5-10") {
            /*
             * The Altitude subfield in the GPS Geolocation Field shall use the
             * 32-bit, two’s-complement format shown in Figure 9.4.5-3 with the
             * value of the Altitude subfield expressed in units of meters. This
             * field has an integer and a fractional part with the radix point
             * to the right of bit 5.
             */
            // Observation 9.4.5-4: range +-67108 km, resolution 3.1 cm
            const double ALTITUDE = 1.031;
            const uint32_t ALTITUDE_BE = htonl(0x00000021);
            const double ALTITUDE_MAX = 67108; // hex(67108<<5(radix)) = 0x20c480
            const uint32_t ALTITUDE_MAX_BE = htonl(0x0020c480);
            const double ALTITUDE_MIN = -67108; // hex(-67108<<5(radix)) = 0xffdf3b80
            const uint32_t ALTITUDE_MIN_BE = htonl(0xffdf3b80);

            // Check resolution
            packet_type packet_in;
            vrtgen::packing::Geolocation ins{};
            ins.setAltitude(ALTITUDE);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[36])) == ALTITUDE_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getAltitude() == ALTITUDE);

            // Check max
            ins.setAltitude(ALTITUDE_MAX);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.clear();
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[36])) == ALTITUDE_MAX_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getAltitude() == ALTITUDE_MAX);

            // Check min
            ins.setAltitude(ALTITUDE_MIN);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.clear();
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[36])) == ALTITUDE_MIN_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getAltitude() == ALTITUDE_MIN);
        }
    
        SECTION("Rule 9.4.5-11") {
            /*
             * The Speed Over Ground subfield shall use the 32-bit,
             * two’s-complement format shown in Figure 9.4.5-4 with the value
             * of the Speed Over Ground subfield expressed in units of meters
             * per second. This field has an integer and a fractional part with
             * the radix point to the right of bit 16.
             */
            // Observation 9.4.5-5: range 0 to 65636 m/s, resolution 1.5e-5 m/s
            // Developer's Note: above observation believed to be typo, max
            // signed integer part can only be 32767 (0x7FFF), however this is
            // also the unspecified value from Rule 9.4.5-18. Instead using
            // 0x3FFF for test, which is still magnitudes higher than reasonable
            // live values.
            const double SPEED_OVER_GROUND = 1.000015;
            const uint32_t SPEED_OVER_GROUND_BE = htonl(0x00010001);
            const double SPEED_OVER_GROUND_MAX = 16383.999985;
            const uint32_t SPEED_OVER_GROUND_MAX_BE = htonl(0x3FFFFFFF);
            const double SPEED_OVER_GROUND_MIN = 0;
            const uint32_t SPEED_OVER_GROUND_MIN_BE = 0;

            // Check resolution
            packet_type packet_in;
            vrtgen::packing::Geolocation ins{};
            ins.setSpeedOverGround(SPEED_OVER_GROUND);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[40])) == SPEED_OVER_GROUND_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getSpeedOverGround() == SPEED_OVER_GROUND);

            // Check max
            ins.setSpeedOverGround(SPEED_OVER_GROUND_MAX);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[40])) == SPEED_OVER_GROUND_MAX_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getSpeedOverGround() == SPEED_OVER_GROUND_MAX);

            // Check min
            ins.setSpeedOverGround(SPEED_OVER_GROUND_MIN);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[40])) == SPEED_OVER_GROUND_MIN_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getSpeedOverGround() == SPEED_OVER_GROUND_MIN);
        }
    
        SECTION("Rule 9.4.5-12") {
            /*
             * The Heading Angle subfield shall use the Geolocation Angle
             * Format shown in Figure 9.4.5-2, with the Heading Angle
             * expressing the platform’s orientation with respect to true North
             * in decimal degrees.
             * 
             * Note: Geolocation Angle Format is 32-bit, two's complement with
             *       radix point to the right of bit 22
             */
            // Observation 9.4.5-2 - resolution of 2.38e-7 degrees
            const double HEADING_ANGLE = 1.000000238;
            const uint32_t HEADING_ANGLE_BE = htonl(0x00400001);

            packet_type packet_in;
            vrtgen::packing::Geolocation ins{};
            ins.setHeadingAngle(HEADING_ANGLE);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[44])) == HEADING_ANGLE_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getHeadingAngle() == HEADING_ANGLE);
        }
    
        SECTION("Rule 9.4.5-13") {
            /*
             * The Heading Angle value shall range from 0.0 to
             * +359.999999761582 degrees.
             */
            // Note: with resolution of 2.38e-7 from Rule 9.4.5-12, the above
            //       maximum is out of reach
            const double HEADING_ANGLE_MAX = 359.999999762;
            const uint32_t HEADING_ANGLE_MAX_BE = htonl(0x59FFFFFF);
            const double HEADING_ANGLE_MIN = 0.0;
            const uint32_t HEADING_ANGLE_MIN_BE = 0x0;

            // Check max
            packet_type packet_in;
            vrtgen::packing::Geolocation ins{};
            ins.setHeadingAngle(HEADING_ANGLE_MAX);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[44])) == HEADING_ANGLE_MAX_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getHeadingAngle() == HEADING_ANGLE_MAX);

            // Check min
            ins.setHeadingAngle(HEADING_ANGLE_MIN);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[44])) == HEADING_ANGLE_MIN_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getHeadingAngle() == HEADING_ANGLE_MIN);
        }
    
        SECTION("Rule 9.4.5-14") {
            /*
             * The Track Angle subfield shall use the Geolocation Angle Format
             * shown in Figure 9.4.5-2, with the Track Angle expressing the
             * platform’s direction of travel with respect to true North in
             * decimal degrees.
             * 
             * Note: Geolocation Angle Format is 32-bit, two's complement with
             *       radix point to the right of bit 22
             */
            // Observation 9.4.5-2 - resolution of 2.38e-7 degrees
            const double TRACK_ANGLE = 1.000000238;
            const uint32_t TRACK_ANGLE_BE = htonl(0x00400001);

            packet_type packet_in;
            vrtgen::packing::Geolocation ins{};
            ins.setTrackAngle(TRACK_ANGLE);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[48])) == TRACK_ANGLE_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getTrackAngle() == TRACK_ANGLE);
        }
    
        SECTION("Rule 9.4.5-15") {
            /*
             * The Track Angle value shall range from 0.0 to +359.999999761582
             * degrees.
             */
            // Note: with resolution of 2.38e-7 from Rule 9.4.5-14, the above
            //       maximum is out of reach
            const double TRACK_ANGLE_MAX = 359.999999762;
            const uint32_t TRACK_ANGLE_MAX_BE = htonl(0x59FFFFFF);
            const double TRACK_ANGLE_MIN = 0.0;
            const uint32_t TRACK_ANGLE_MIN_BE = 0x0;

            // Check max
            packet_type packet_in;
            vrtgen::packing::Geolocation ins{};
            ins.setTrackAngle(TRACK_ANGLE_MAX);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[48])) == TRACK_ANGLE_MAX_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getTrackAngle() == TRACK_ANGLE_MAX);

            // Check min
            ins.setTrackAngle(TRACK_ANGLE_MIN);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[48])) == TRACK_ANGLE_MIN_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getTrackAngle() == TRACK_ANGLE_MIN);
        }
    
        SECTION("Rule 9.4.5-16") {
            /*
             * The Magnetic Variation subfield shall use the Geolocation Angle
             * Format shown in Figure 9.4.5-2, above, with the Magnetic
             * Variation subfield expressing magnetic variation from true North
             * in decimal degrees.
             * 
             * Note: Geolocation Angle Format is 32-bit, two's complement with
             *       radix point to the right of bit 22
             */
            // Observation 9.4.5-2 - resolution of 2.38e-7 degrees
            const double MAGNETIC_VARIATION = 1.000000238;
            const uint32_t MAGNETIC_VARIATION_BE = htonl(0x00400001);

            packet_type packet_in;
            vrtgen::packing::Geolocation ins{};
            ins.setMagneticVariation(MAGNETIC_VARIATION);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[52])) == MAGNETIC_VARIATION_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getMagneticVariation() == MAGNETIC_VARIATION);
        }
    
        SECTION("Rule 9.4.5-17") {
            /*
             * The Magnetic Variation value shall range from -180.0 (West) to
             * +180.0 (East) degrees.
             */
            const double MAGNETIC_VARIATION_MAX = 180.0; // hex(180<<22(radix)) = 0x2d000000
            const uint32_t MAGNETIC_VARIATION_MAX_BE = htonl(0x2d000000);
            const double MAGNETIC_VARIATION_MIN = -180.0; // hex(-180<<22(radix)) = 0xd3000000
            const uint32_t MAGNETIC_VARIATION_MIN_BE = htonl(0xd3000000);

            // Check max
            packet_type packet_in;
            vrtgen::packing::Geolocation ins{};
            ins.setMagneticVariation(MAGNETIC_VARIATION_MAX);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[52])) == MAGNETIC_VARIATION_MAX_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getMagneticVariation() == MAGNETIC_VARIATION_MAX);

            // Check min
            ins.setMagneticVariation(MAGNETIC_VARIATION_MIN);
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            data.clear();
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[52])) == MAGNETIC_VARIATION_MIN_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
            CHECK(packet_out.getFormattedINS().getMagneticVariation() == MAGNETIC_VARIATION_MIN);
        }
    
        SECTION("Rule 9.4.5-18") {
            /*
             * The Latitude, Longitude, Altitude, Speed Over Ground, Heading,
             * Track Angle, and Magnetic Variation subfields shall take the
             * value 0x7FFFFFFF when unspecified.
             */
            // Observation 9.4.5-7: These default subfield values are invalid
            // or practically impossible for all GPS and INS Geolocation
            // subfields.
            const uint32_t UNSPECIFIED_VALUE = 0x7FFFFFFF;
            const uint32_t UNSPECIFIED_VALUE_BE = htonl(UNSPECIFIED_VALUE);

            // Check default (unset)
            packet_type packet_in;
            vrtgen::packing::Geolocation ins{};
            packet_in.setFormattedINS(ins);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[28])) == UNSPECIFIED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[32])) == UNSPECIFIED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[36])) == UNSPECIFIED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[40])) == UNSPECIFIED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[44])) == UNSPECIFIED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[48])) == UNSPECIFIED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[52])) == UNSPECIFIED_VALUE_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasFormattedINS());
        }
    
    }

    SECTION("ECEF Ephemeris") {
        /*
         * Rule 9.4.3-3: The ECEF Ephemeris field shall be expressed using the
         * format shown in Figure 9.4.3-2.
         */
        const size_t PACKED_SIZE = 68;

        SECTION("Rule 9.4.3-4") {
            /*
             * Rule 9.4.3-4: The TSI, TSF, OUI, and Timestamp of Position Fix
             * fields shall follow the rules of the corresponding Formatted GPS
             * Geolocation fields given in Section 9.4.5.
             */
            SECTION("OUI - Rule 9.4.5-2") {
                /*
                * The GPS/INS Manufacturer OUI subfield shall contain the 24-bit
                * field for the IEEE registered, Organizationally Unique Identifier
                * (company identifier) in the range 00-00-00 to FF-FE-FF
                * (inclusive) of the GPS/INS manufacturer.
                */
                const uint32_t OUI = 0x123456;
                const uint32_t OUI_BE = 0x56341200; // 00 for empty tsi and tsf

                packet_type packet_in;
                vrtgen::packing::Ephemeris ephemeris{};
                ephemeris.setManufacturerOUI(OUI);
                packet_in.setECEFEphemeris(ephemeris);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == OUI_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasECEFEphemeris());
                CHECK(packet_out.getECEFEphemeris().getManufacturerOUI() == OUI);
            }
            SECTION("TSI - Rule 9.4.5-3") {
                /*
                * The TSI field in the Formatted GPS Geolocation field shall
                * accurately indicate the type of Integer-seconds Timestamp
                * included in the packet according to the code assignments in
                * Table 9.4.5-1.
                * 
                * | TSI Code | Meaning   |
                * | -------- | --------- |
                * | 00       | Undefined |
                * | 01       | UTC       |
                * | 10       | GPS Time  |
                * | 11       | Other     |
                * | -------- | --------- |
                */
                SECTION("Undefined") {
                    const vrtgen::TSI::Code TSI = vrtgen::TSI::NONE;
                    const uint32_t TSI_BE = htonl(TSI << 26);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSI(TSI);
                    packet_in.setECEFEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasECEFEphemeris());
                    CHECK(packet_out.getECEFEphemeris().getTSI() == TSI);
                }
                SECTION("UTC") {
                    const vrtgen::TSI::Code TSI = vrtgen::TSI::UTC;
                    const uint32_t TSI_BE = htonl(TSI << 26);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSI(TSI);
                    packet_in.setECEFEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasECEFEphemeris());
                    CHECK(packet_out.getECEFEphemeris().getTSI() == TSI);
                }
                SECTION("GPS Time") {
                    const vrtgen::TSI::Code TSI = vrtgen::TSI::GPS;
                    const uint32_t TSI_BE = htonl(TSI << 26);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSI(TSI);
                    packet_in.setECEFEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasECEFEphemeris());
                    CHECK(packet_out.getECEFEphemeris().getTSI() == TSI);
                }
                SECTION("Other") {
                    const vrtgen::TSI::Code TSI = vrtgen::TSI::OTHER;
                    const uint32_t TSI_BE = htonl(TSI << 26);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSI(TSI);
                    packet_in.setECEFEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasECEFEphemeris());
                    CHECK(packet_out.getECEFEphemeris().getTSI() == TSI);
                }
            }
            SECTION("TSF - Rule 9.4.5-4") {
                /*
                * The TSF field in the Formatted GPS Geolocation field shall
                * accurately indicate the type of Fractional-seconds Timestamp
                * included in the packet according to the code assignments in
                * Table 9.4.5-2.
                * 
                * | TSF Code | Meaning                 |
                * | -------- | ----------------------- |
                * | 00       | Undefined               |
                * | 01       | Sample Count Time       |
                * | 10       | Real (Picoseconds) Time |
                * | 11       | Free-Running Count Time |
                * | -------- | ----------------------- |
                */
                SECTION("Undefined") {
                    const vrtgen::TSF::Code TSF = vrtgen::TSF::NONE;
                    const uint32_t TSF_BE = htonl(TSF << 24);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSF(TSF);
                    packet_in.setECEFEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.clear();
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasECEFEphemeris());
                    CHECK(packet_out.getECEFEphemeris().getTSF() == TSF);
                }
                SECTION("Sample Count Time") {
                    const vrtgen::TSF::Code TSF = vrtgen::TSF::SAMPLE_COUNT;
                    const uint32_t TSF_BE = htonl(TSF << 24);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSF(TSF);
                    packet_in.setECEFEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasECEFEphemeris());
                    CHECK(packet_out.getECEFEphemeris().getTSF() == TSF);
                }
                SECTION("Real (Picoseconds) Time") {
                    const vrtgen::TSF::Code TSF = vrtgen::TSF::REAL_TIME;
                    const uint32_t TSF_BE = htonl(TSF << 24);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSF(TSF);
                    packet_in.setECEFEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasECEFEphemeris());
                    CHECK(packet_out.getECEFEphemeris().getTSF() == TSF);
                }
                SECTION("Other") {
                    const vrtgen::TSF::Code TSF = vrtgen::TSF::FREE_RUNNING;
                    const uint32_t TSF_BE = htonl(TSF << 24);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSF(TSF);
                    packet_in.setECEFEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasECEFEphemeris());
                    CHECK(packet_out.getECEFEphemeris().getTSF() == TSF);
                }
            }
            SECTION("Timestamps - Rule 9.4.5-5") {
                /*
                * When the TSI or TSF fields are non-zero the corresponding
                * Timestamp of Position Fix subfield shall express the time of the
                * most recent location fix in the format given in Sections
                * 6.1.45.1.4 and 5.1.4.2
                */
                const vrtgen::TSI::Code TSI = vrtgen::TSI::UTC;
                const vrtgen::TSF::Code TSF = vrtgen::TSF::REAL_TIME;
                const uint32_t WORD_1_BE = htonl((TSI << 26) | (TSF << 24));
                const uint32_t INTEGER_TIMESTAMP = 0x12345678;
                const uint32_t INTEGER_TIMESTAMP_BE = 0x78563412;
                const uint64_t FRACTIONAL_TIMESTAMP = 0x12345678ABCDEF00;
                const uint64_t FRACTIONAL_TIMESTAMP_BE = 0xEFCDAB78563412;

                packet_type packet_in;
                vrtgen::packing::Ephemeris ephemeris{};
                ephemeris.setTSI(TSI);
                ephemeris.setTSF(TSF);
                ephemeris.setIntegersecondTimestamp(INTEGER_TIMESTAMP);
                ephemeris.setFractionalsecondTimestamp(FRACTIONAL_TIMESTAMP);
                packet_in.setECEFEphemeris(ephemeris);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == WORD_1_BE);
                CHECK(*(reinterpret_cast<const uint32_t*>(&data[16])) == INTEGER_TIMESTAMP_BE);
                CHECK(*(reinterpret_cast<const uint64_t*>(&data[20])) == FRACTIONAL_TIMESTAMP_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasECEFEphemeris());
                CHECK(packet_out.getECEFEphemeris().getTSI() == TSI);
                CHECK(packet_out.getECEFEphemeris().getTSF() == TSF);
                CHECK(packet_out.getECEFEphemeris().getIntegersecondTimestamp() == INTEGER_TIMESTAMP);
                CHECK(packet_out.getECEFEphemeris().getFractionalsecondTimestamp() == FRACTIONAL_TIMESTAMP);
            }
            SECTION("Timestamps - Rule 9.4.5-6") {
                /*
                * When the TSI or TSF fields are zero the corresponding Timestamp
                * of Position Fix subfield words shall take the value 0xFFFFFFFF.
                */
                const vrtgen::TSI::Code TSI = vrtgen::TSI::NONE;
                const vrtgen::TSF::Code TSF = vrtgen::TSF::NONE;
                const uint32_t WORD_1_BE = htonl((TSI << 26) | (TSF << 24));
                const uint32_t INTEGER_TIMESTAMP_BE = 0xFFFFFFFF;
                const uint64_t FRACTIONAL_TIMESTAMP_BE = 0xFFFFFFFFFFFFFFFF;

                // Defaults
                packet_type packet_in;
                vrtgen::packing::Ephemeris ephemeris{};
                packet_in.setECEFEphemeris(ephemeris);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == WORD_1_BE);
                CHECK(*(reinterpret_cast<const uint32_t*>(&data[16])) == INTEGER_TIMESTAMP_BE);
                CHECK(*(reinterpret_cast<const uint64_t*>(&data[20])) == FRACTIONAL_TIMESTAMP_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasECEFEphemeris());
                CHECK(packet_out.getECEFEphemeris().getTSI() == TSI);
                CHECK(packet_out.getECEFEphemeris().getTSF() == TSF);

                // Set to zero
                ephemeris.setTSI(TSI);
                ephemeris.setTSF(TSF);
                packet_in.setECEFEphemeris(ephemeris);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == WORD_1_BE);
                CHECK(*(reinterpret_cast<const uint32_t*>(&data[16])) == INTEGER_TIMESTAMP_BE);
                CHECK(*(reinterpret_cast<const uint64_t*>(&data[20])) == FRACTIONAL_TIMESTAMP_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasECEFEphemeris());
                CHECK(packet_out.getECEFEphemeris().getTSI() == TSI);
                CHECK(packet_out.getECEFEphemeris().getTSF() == TSF);
            }
        }
    
        SECTION("Rule 9.4.3-5") {
            /*
             * The position coordinates of the ECEF Ephemeris field shall use
             * the 32-bit, two’s complement "Position" format shown in Figure
             * 9.4.3-3. The position values shall be expressed in units of
             * meters. This field has an integer and a fractional part with the
             * radix point to the right of bit 5.
             */
            const double POSITION_X = 1.03125;
            const uint32_t POSITION_X_BE = htonl(0x00000021);
            const double POSITION_Y = 1.03125;
            const uint32_t POSITION_Y_BE = htonl(0x00000021);
            const double POSITION_Z = 1.03125;
            const uint32_t POSITION_Z_BE = htonl(0x00000021);

            packet_type packet_in;
            vrtgen::packing::Ephemeris ephemeris{};
            ephemeris.setPositionX(POSITION_X);
            ephemeris.setPositionY(POSITION_Y);
            ephemeris.setPositionZ(POSITION_Z);
            packet_in.setECEFEphemeris(ephemeris);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[28])) == POSITION_X_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[32])) == POSITION_Y_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[36])) == POSITION_Z_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasECEFEphemeris());
            CHECK(packet_out.getECEFEphemeris().getPositionX() == POSITION_X);
            CHECK(packet_out.getECEFEphemeris().getPositionY() == POSITION_Y);
            CHECK(packet_out.getECEFEphemeris().getPositionZ() == POSITION_Z);
        }

        SECTION ("Rule 9.4.3-6") {
            /*
             * The attitude coordinates of the ECEF Ephemeris field shall use
             * the 32-bit, two’scomplement "Attitude" format shown in Figure
             * 9.4.3-3. The attitude values shall be expressed in units of
             * decimal degrees. This field has an integer and a fractional part
             * with the radix point to the right of bit 22.
             */
            const double ATTITUDE_ALPHA = 1.000000238;
            const uint32_t ATTITUDE_ALPHA_BE = htonl(0x00400001);
            const double ATTITUDE_BETA = 1.000000238;
            const uint32_t ATTITUDE_BETA_BE = htonl(0x00400001);
            const double ATTITUDE_PHI = 1.000000238;
            const uint32_t ATTITUDE_PHI_BE = htonl(0x00400001);

            packet_type packet_in;
            vrtgen::packing::Ephemeris ephemeris{};
            ephemeris.setAttitudeAlpha(ATTITUDE_ALPHA);
            ephemeris.setAttitudeBeta(ATTITUDE_BETA);
            ephemeris.setAttitudePhi(ATTITUDE_PHI);
            packet_in.setECEFEphemeris(ephemeris);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[40])) == ATTITUDE_ALPHA_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[44])) == ATTITUDE_BETA_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[48])) == ATTITUDE_PHI_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasECEFEphemeris());
            CHECK(packet_out.getECEFEphemeris().getAttitudeAlpha() == ATTITUDE_ALPHA);
            CHECK(packet_out.getECEFEphemeris().getAttitudeBeta() == ATTITUDE_BETA);
            CHECK(packet_out.getECEFEphemeris().getAttitudePhi() == ATTITUDE_PHI);
        }
    
        SECTION ("Rule 9.4.3-7") {
            /*
             * The velocity coordinates of the ECEF Ephemeris field shall use
             * the 32-bit, two’scomplement "Velocity" format shown in Figure
             * 9.4.3-3. The velocity values shall be expressed in units of
             * meters per second. This field has an integer and a fractional
             * part with the radix point to the right of bit 16.
             */
            const double VELOCITY_DX = 1.00001526;
            const uint32_t VELOCITY_DX_BE = htonl(0x00010001);
            const double VELOCITY_DY = 1.00001526;
            const uint32_t VELOCITY_DY_BE = htonl(0x00010001);
            const double VELOCITY_DZ = 1.00001526;
            const uint32_t VELOCITY_DZ_BE = htonl(0x00010001);

            packet_type packet_in;
            vrtgen::packing::Ephemeris ephemeris{};
            ephemeris.setVelocitydX(VELOCITY_DX);
            ephemeris.setVelocitydY(VELOCITY_DY);
            ephemeris.setVelocitydZ(VELOCITY_DZ);
            packet_in.setECEFEphemeris(ephemeris);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[52])) == VELOCITY_DX_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[56])) == VELOCITY_DY_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[60])) == VELOCITY_DZ_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasECEFEphemeris());
            CHECK(packet_out.getECEFEphemeris().getVelocitydX() == VELOCITY_DX);
            CHECK(packet_out.getECEFEphemeris().getVelocitydY() == VELOCITY_DY);
            CHECK(packet_out.getECEFEphemeris().getVelocitydZ() == VELOCITY_DZ);
        }

        SECTION ("Rule 9.4.3-8") {
            /*
             * Each word of the Position, Attitude, and Velocity subfields in
             * the ECEF Ephemeris field shall take the value 0x7FFFFFFF when
             * the value is unknown.
             */
            const double UNDEFINED_VALUE = 0x7FFFFFFF;
            const uint32_t UNDEFINED_VALUE_BE = htonl(UNDEFINED_VALUE);

            packet_type packet_in;
            vrtgen::packing::Ephemeris ephemeris{};
            packet_in.setECEFEphemeris(ephemeris);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[28])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[32])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[36])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[40])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[44])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[48])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[52])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[56])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[60])) == UNDEFINED_VALUE_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasECEFEphemeris());
        }

    }

    SECTION("Relative Ephemeris") {
        /*
         * Rule 9.4.9-1: The Relative Ephemeris field shall be expressed using
         * the formats shown in Figure 9.4.3-2 and Figure 9.4.3-3. from Section
         * 9.4.3 regarding ECEF Ephemeris.
         * 
         * Developer's Note: Using same tests as above
         */
        /*
         * Rule 9.4.3-3: The ECEF Ephemeris field shall be expressed using the
         * format shown in Figure 9.4.3-2.
         */
        const size_t PACKED_SIZE = 68;

        SECTION("Rule 9.4.3-4") {
            /*
             * Rule 9.4.3-4: The TSI, TSF, OUI, and Timestamp of Position Fix
             * fields shall follow the rules of the corresponding Formatted GPS
             * Geolocation fields given in Section 9.4.5.
             */
            SECTION("OUI - Rule 9.4.5-2") {
                /*
                * The GPS/INS Manufacturer OUI subfield shall contain the 24-bit
                * field for the IEEE registered, Organizationally Unique Identifier
                * (company identifier) in the range 00-00-00 to FF-FE-FF
                * (inclusive) of the GPS/INS manufacturer.
                */
                const uint32_t OUI = 0x123456;
                const uint32_t OUI_BE = 0x56341200; // 00 for empty tsi and tsf

                packet_type packet_in;
                vrtgen::packing::Ephemeris ephemeris{};
                ephemeris.setManufacturerOUI(OUI);
                packet_in.setRelativeEphemeris(ephemeris);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == OUI_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasRelativeEphemeris());
                CHECK(packet_out.getRelativeEphemeris().getManufacturerOUI() == OUI);
            }
            SECTION("TSI - Rule 9.4.5-3") {
                /*
                * The TSI field in the Formatted GPS Geolocation field shall
                * accurately indicate the type of Integer-seconds Timestamp
                * included in the packet according to the code assignments in
                * Table 9.4.5-1.
                * 
                * | TSI Code | Meaning   |
                * | -------- | --------- |
                * | 00       | Undefined |
                * | 01       | UTC       |
                * | 10       | GPS Time  |
                * | 11       | Other     |
                * | -------- | --------- |
                */
                SECTION("Undefined") {
                    const vrtgen::TSI::Code TSI = vrtgen::TSI::NONE;
                    const uint32_t TSI_BE = htonl(TSI << 26);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSI(TSI);
                    packet_in.setRelativeEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasRelativeEphemeris());
                    CHECK(packet_out.getRelativeEphemeris().getTSI() == TSI);
                }
                SECTION("UTC") {
                    const vrtgen::TSI::Code TSI = vrtgen::TSI::UTC;
                    const uint32_t TSI_BE = htonl(TSI << 26);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSI(TSI);
                    packet_in.setRelativeEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasRelativeEphemeris());
                    CHECK(packet_out.getRelativeEphemeris().getTSI() == TSI);
                }
                SECTION("GPS Time") {
                    const vrtgen::TSI::Code TSI = vrtgen::TSI::GPS;
                    const uint32_t TSI_BE = htonl(TSI << 26);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSI(TSI);
                    packet_in.setRelativeEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasRelativeEphemeris());
                    CHECK(packet_out.getRelativeEphemeris().getTSI() == TSI);
                }
                SECTION("Other") {
                    const vrtgen::TSI::Code TSI = vrtgen::TSI::OTHER;
                    const uint32_t TSI_BE = htonl(TSI << 26);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSI(TSI);
                    packet_in.setRelativeEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSI_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasRelativeEphemeris());
                    CHECK(packet_out.getRelativeEphemeris().getTSI() == TSI);
                }
            }
            SECTION("TSF - Rule 9.4.5-4") {
                /*
                * The TSF field in the Formatted GPS Geolocation field shall
                * accurately indicate the type of Fractional-seconds Timestamp
                * included in the packet according to the code assignments in
                * Table 9.4.5-2.
                * 
                * | TSF Code | Meaning                 |
                * | -------- | ----------------------- |
                * | 00       | Undefined               |
                * | 01       | Sample Count Time       |
                * | 10       | Real (Picoseconds) Time |
                * | 11       | Free-Running Count Time |
                * | -------- | ----------------------- |
                */
                SECTION("Undefined") {
                    const vrtgen::TSF::Code TSF = vrtgen::TSF::NONE;
                    const uint32_t TSF_BE = htonl(TSF << 24);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSF(TSF);
                    packet_in.setRelativeEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.clear();
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasRelativeEphemeris());
                    CHECK(packet_out.getRelativeEphemeris().getTSF() == TSF);
                }
                SECTION("Sample Count Time") {
                    const vrtgen::TSF::Code TSF = vrtgen::TSF::SAMPLE_COUNT;
                    const uint32_t TSF_BE = htonl(TSF << 24);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSF(TSF);
                    packet_in.setRelativeEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasRelativeEphemeris());
                    CHECK(packet_out.getRelativeEphemeris().getTSF() == TSF);
                }
                SECTION("Real (Picoseconds) Time") {
                    const vrtgen::TSF::Code TSF = vrtgen::TSF::REAL_TIME;
                    const uint32_t TSF_BE = htonl(TSF << 24);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSF(TSF);
                    packet_in.setRelativeEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasRelativeEphemeris());
                    CHECK(packet_out.getRelativeEphemeris().getTSF() == TSF);
                }
                SECTION("Other") {
                    const vrtgen::TSF::Code TSF = vrtgen::TSF::FREE_RUNNING;
                    const uint32_t TSF_BE = htonl(TSF << 24);

                    packet_type packet_in;
                    vrtgen::packing::Ephemeris ephemeris{};
                    ephemeris.setTSF(TSF);
                    packet_in.setRelativeEphemeris(ephemeris);
                    REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                    bytes data;
                    data.resize(PACKED_SIZE);
                    packet_helper::pack(packet_in, data.data(), data.size());

                    CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == TSF_BE);

                    CHECK(packet_helper::match(data.data(), data.size()));

                    packet_type packet_out;
                    packet_helper::unpack(packet_out, data.data(), data.size());

                    REQUIRE(packet_out.hasRelativeEphemeris());
                    CHECK(packet_out.getRelativeEphemeris().getTSF() == TSF);
                }
            }
            SECTION("Timestamps - Rule 9.4.5-5") {
                /*
                * When the TSI or TSF fields are non-zero the corresponding
                * Timestamp of Position Fix subfield shall express the time of the
                * most recent location fix in the format given in Sections
                * 6.1.45.1.4 and 5.1.4.2
                */
                const vrtgen::TSI::Code TSI = vrtgen::TSI::UTC;
                const vrtgen::TSF::Code TSF = vrtgen::TSF::REAL_TIME;
                const uint32_t WORD_1_BE = htonl((TSI << 26) | (TSF << 24));
                const uint32_t INTEGER_TIMESTAMP = 0x12345678;
                const uint32_t INTEGER_TIMESTAMP_BE = 0x78563412;
                const uint64_t FRACTIONAL_TIMESTAMP = 0x12345678ABCDEF00;
                const uint64_t FRACTIONAL_TIMESTAMP_BE = 0xEFCDAB78563412;

                packet_type packet_in;
                vrtgen::packing::Ephemeris ephemeris{};
                ephemeris.setTSI(TSI);
                ephemeris.setTSF(TSF);
                ephemeris.setIntegersecondTimestamp(INTEGER_TIMESTAMP);
                ephemeris.setFractionalsecondTimestamp(FRACTIONAL_TIMESTAMP);
                packet_in.setRelativeEphemeris(ephemeris);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == WORD_1_BE);
                CHECK(*(reinterpret_cast<const uint32_t*>(&data[16])) == INTEGER_TIMESTAMP_BE);
                CHECK(*(reinterpret_cast<const uint64_t*>(&data[20])) == FRACTIONAL_TIMESTAMP_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasRelativeEphemeris());
                CHECK(packet_out.getRelativeEphemeris().getTSI() == TSI);
                CHECK(packet_out.getRelativeEphemeris().getTSF() == TSF);
                CHECK(packet_out.getRelativeEphemeris().getIntegersecondTimestamp() == INTEGER_TIMESTAMP);
                CHECK(packet_out.getRelativeEphemeris().getFractionalsecondTimestamp() == FRACTIONAL_TIMESTAMP);
            }
            SECTION("Timestamps - Rule 9.4.5-6") {
                /*
                * When the TSI or TSF fields are zero the corresponding Timestamp
                * of Position Fix subfield words shall take the value 0xFFFFFFFF.
                */
                const vrtgen::TSI::Code TSI = vrtgen::TSI::NONE;
                const vrtgen::TSF::Code TSF = vrtgen::TSF::NONE;
                const uint32_t WORD_1_BE = htonl((TSI << 26) | (TSF << 24));
                const uint32_t INTEGER_TIMESTAMP_BE = 0xFFFFFFFF;
                const uint64_t FRACTIONAL_TIMESTAMP_BE = 0xFFFFFFFFFFFFFFFF;

                // Defaults
                packet_type packet_in;
                vrtgen::packing::Ephemeris ephemeris{};
                packet_in.setRelativeEphemeris(ephemeris);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                bytes data;
                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == WORD_1_BE);
                CHECK(*(reinterpret_cast<const uint32_t*>(&data[16])) == INTEGER_TIMESTAMP_BE);
                CHECK(*(reinterpret_cast<const uint64_t*>(&data[20])) == FRACTIONAL_TIMESTAMP_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_type packet_out;
                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasRelativeEphemeris());
                CHECK(packet_out.getRelativeEphemeris().getTSI() == TSI);
                CHECK(packet_out.getRelativeEphemeris().getTSF() == TSF);

                // Set to zero
                ephemeris.setTSI(TSI);
                ephemeris.setTSF(TSF);
                packet_in.setRelativeEphemeris(ephemeris);
                REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

                data.resize(PACKED_SIZE);
                packet_helper::pack(packet_in, data.data(), data.size());

                CHECK(*(reinterpret_cast<const uint32_t*>(&data[12])) == WORD_1_BE);
                CHECK(*(reinterpret_cast<const uint32_t*>(&data[16])) == INTEGER_TIMESTAMP_BE);
                CHECK(*(reinterpret_cast<const uint64_t*>(&data[20])) == FRACTIONAL_TIMESTAMP_BE);

                CHECK(packet_helper::match(data.data(), data.size()));

                packet_helper::unpack(packet_out, data.data(), data.size());

                REQUIRE(packet_out.hasRelativeEphemeris());
                CHECK(packet_out.getRelativeEphemeris().getTSI() == TSI);
                CHECK(packet_out.getRelativeEphemeris().getTSF() == TSF);
            }
        }
    
        SECTION("Rule 9.4.3-5") {
            /*
             * The position coordinates of the ECEF Ephemeris field shall use
             * the 32-bit, two’s complement "Position" format shown in Figure
             * 9.4.3-3. The position values shall be expressed in units of
             * meters. This field has an integer and a fractional part with the
             * radix point to the right of bit 5.
             */
            const double POSITION_X = 1.03125;
            const uint32_t POSITION_X_BE = htonl(0x00000021);
            const double POSITION_Y = 1.03125;
            const uint32_t POSITION_Y_BE = htonl(0x00000021);
            const double POSITION_Z = 1.03125;
            const uint32_t POSITION_Z_BE = htonl(0x00000021);

            packet_type packet_in;
            vrtgen::packing::Ephemeris ephemeris{};
            ephemeris.setPositionX(POSITION_X);
            ephemeris.setPositionY(POSITION_Y);
            ephemeris.setPositionZ(POSITION_Z);
            packet_in.setRelativeEphemeris(ephemeris);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[28])) == POSITION_X_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[32])) == POSITION_Y_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[36])) == POSITION_Z_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasRelativeEphemeris());
            CHECK(packet_out.getRelativeEphemeris().getPositionX() == POSITION_X);
            CHECK(packet_out.getRelativeEphemeris().getPositionY() == POSITION_Y);
            CHECK(packet_out.getRelativeEphemeris().getPositionZ() == POSITION_Z);
        }

        SECTION ("Rule 9.4.3-6") {
            /*
             * The attitude coordinates of the ECEF Ephemeris field shall use
             * the 32-bit, two’scomplement "Attitude" format shown in Figure
             * 9.4.3-3. The attitude values shall be expressed in units of
             * decimal degrees. This field has an integer and a fractional part
             * with the radix point to the right of bit 22.
             */
            const double ATTITUDE_ALPHA = 1.000000238;
            const uint32_t ATTITUDE_ALPHA_BE = htonl(0x00400001);
            const double ATTITUDE_BETA = 1.000000238;
            const uint32_t ATTITUDE_BETA_BE = htonl(0x00400001);
            const double ATTITUDE_PHI = 1.000000238;
            const uint32_t ATTITUDE_PHI_BE = htonl(0x00400001);

            packet_type packet_in;
            vrtgen::packing::Ephemeris ephemeris{};
            ephemeris.setAttitudeAlpha(ATTITUDE_ALPHA);
            ephemeris.setAttitudeBeta(ATTITUDE_BETA);
            ephemeris.setAttitudePhi(ATTITUDE_PHI);
            packet_in.setRelativeEphemeris(ephemeris);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[40])) == ATTITUDE_ALPHA_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[44])) == ATTITUDE_BETA_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[48])) == ATTITUDE_PHI_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasRelativeEphemeris());
            CHECK(packet_out.getRelativeEphemeris().getAttitudeAlpha() == ATTITUDE_ALPHA);
            CHECK(packet_out.getRelativeEphemeris().getAttitudeBeta() == ATTITUDE_BETA);
            CHECK(packet_out.getRelativeEphemeris().getAttitudePhi() == ATTITUDE_PHI);
        }
    
        SECTION ("Rule 9.4.3-7") {
            /*
             * The velocity coordinates of the ECEF Ephemeris field shall use
             * the 32-bit, two’scomplement "Velocity" format shown in Figure
             * 9.4.3-3. The velocity values shall be expressed in units of
             * meters per second. This field has an integer and a fractional
             * part with the radix point to the right of bit 16.
             */
            const double VELOCITY_DX = 1.00001526;
            const uint32_t VELOCITY_DX_BE = htonl(0x00010001);
            const double VELOCITY_DY = 1.00001526;
            const uint32_t VELOCITY_DY_BE = htonl(0x00010001);
            const double VELOCITY_DZ = 1.00001526;
            const uint32_t VELOCITY_DZ_BE = htonl(0x00010001);

            packet_type packet_in;
            vrtgen::packing::Ephemeris ephemeris{};
            ephemeris.setVelocitydX(VELOCITY_DX);
            ephemeris.setVelocitydY(VELOCITY_DY);
            ephemeris.setVelocitydZ(VELOCITY_DZ);
            packet_in.setRelativeEphemeris(ephemeris);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[52])) == VELOCITY_DX_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[56])) == VELOCITY_DY_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[60])) == VELOCITY_DZ_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasRelativeEphemeris());
            CHECK(packet_out.getRelativeEphemeris().getVelocitydX() == VELOCITY_DX);
            CHECK(packet_out.getRelativeEphemeris().getVelocitydY() == VELOCITY_DY);
            CHECK(packet_out.getRelativeEphemeris().getVelocitydZ() == VELOCITY_DZ);
        }

        SECTION ("Rule 9.4.3-8") {
            /*
             * Each word of the Position, Attitude, and Velocity subfields in
             * the ECEF Ephemeris field shall take the value 0x7FFFFFFF when
             * the value is unknown.
             */
            const double UNDEFINED_VALUE = 0x7FFFFFFF;
            const uint32_t UNDEFINED_VALUE_BE = htonl(UNDEFINED_VALUE);

            packet_type packet_in;
            vrtgen::packing::Ephemeris ephemeris{};
            packet_in.setRelativeEphemeris(ephemeris);
            REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

            bytes data;
            data.resize(PACKED_SIZE);
            packet_helper::pack(packet_in, data.data(), data.size());

            CHECK(*(reinterpret_cast<const uint32_t*>(&data[28])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[32])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[36])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[40])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[44])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[48])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[52])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[56])) == UNDEFINED_VALUE_BE);
            CHECK(*(reinterpret_cast<const uint32_t*>(&data[60])) == UNDEFINED_VALUE_BE);

            CHECK(packet_helper::match(data.data(), data.size()));

            packet_type packet_out;
            packet_helper::unpack(packet_out, data.data(), data.size());

            REQUIRE(packet_out.hasRelativeEphemeris());
        }

    }

    SECTION("Ephemeris Ref ID") {
        /*
         * Rule 9.4.4-1: The Ephemeris Reference Identifier, when used, shall
         * contain the Stream ID of the VRT Context Packet Stream whose ECEF
         * Ephemeris is necessary to translate the Relative Ephemeris given in
         * this Context Packet Stream to ECEF coordinates.
         */
        const vrtgen::StreamIdentifier EPHEM_REF_ID = 0x12345678;
        const vrtgen::StreamIdentifier EPHEM_REF_ID_BE = 0x78563412;
        const size_t PACKED_SIZE = 16;

        packet_type packet_in;
        packet_in.setEphemerisReferenceID(EPHEM_REF_ID);
        REQUIRE(packet_helper::bytes_required(packet_in) == PACKED_SIZE);

        bytes data;
        data.resize(PACKED_SIZE);
        packet_helper::pack(packet_in, data.data(), data.size());

        CHECK(*(reinterpret_cast<const vrtgen::StreamIdentifier*>(&data[12])) == EPHEM_REF_ID_BE);

        CHECK(packet_helper::match(data.data(), data.size()));

        packet_type packet_out;
        packet_helper::unpack(packet_out, data.data(), data.size());

        REQUIRE(packet_out.hasEphemerisReferenceID());
        CHECK(packet_out.getEphemerisReferenceID() == EPHEM_REF_ID);
    }

    SECTION("GPS ASCII") {
        // TODO - not currently supported
        CHECK(true);
    }

    SECTION("Context Association Lists") {
        // TODO - not currently supported
        CHECK(true);
    }
}
