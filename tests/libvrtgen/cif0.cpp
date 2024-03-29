/*
 * Copyright (C) 2023 Geon Technologies, LLC
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

#include "vrtgen/packing/cif0.hpp"

using namespace vrtgen::packing;

TEST_CASE("CIF0", "[cif0]")
{
    CIF0 cif0;
    CIF0 unpack_cif0;
    bytes packed_bytes{ 0xFF, 0xFF, 0xFF, 0xFF };

    SECTION("Zero on construction")
    {
        cif0.pack_into(packed_bytes.data());
        CHECK(packed_bytes == bytes{ 0, 0, 0, 0 });
    }

    SECTION("Change Indicator")
    {
        // Verify zero on construction
        CHECK(cif0.change_indicator() == false);
        // Setter
        cif0.change_indicator(true);
        // Getter check set value
        CHECK(cif0.change_indicator() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x80);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.change_indicator() == true);
    }

    SECTION("Reference Point ID")
    {
        // Verify zero on construction
        CHECK(cif0.reference_point_id() == false);
        // Setter
        cif0.reference_point_id(true);
        // Getter check set value
        CHECK(cif0.reference_point_id() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x40);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.reference_point_id() == true);
    }

    SECTION("Bandwidth")
    {
        // Verify zero on construction
        CHECK(cif0.bandwidth() == false);
        // Setter
        cif0.bandwidth(true);
        // Getter check set value
        CHECK(cif0.bandwidth() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x20);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.bandwidth() == true);
    }

    SECTION("IF Reference Frequency")
    {
        // Verify zero on construction
        CHECK(cif0.if_ref_frequency() == false);
        // Setter
        cif0.if_ref_frequency(true);
        // Getter check set value
        CHECK(cif0.if_ref_frequency() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x10);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.if_ref_frequency() == true);
    }

    SECTION("RF Reference Frequency")
    {
        // Verify zero on construction
        CHECK(cif0.rf_ref_frequency() == false);
        // Setter
        cif0.rf_ref_frequency(true);
        // Getter check set value
        CHECK(cif0.rf_ref_frequency() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x08);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.rf_ref_frequency() == true);
    }

    SECTION("RF Reference Frequency Offset")
    {
        // Verify zero on construction
        CHECK(cif0.rf_ref_frequency_offset() == false);
        // Setter
        cif0.rf_ref_frequency_offset(true);
        // Getter check set value
        CHECK(cif0.rf_ref_frequency_offset() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x04);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.rf_ref_frequency_offset() == true);
    }

    SECTION("IF Band Offset")
    {
        // Verify zero on construction
        CHECK(cif0.if_band_offset() == false);
        // Setter
        cif0.if_band_offset(true);
        // Getter check set value
        CHECK(cif0.if_band_offset() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x02);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.if_band_offset() == true);
    }

    SECTION("Reference Level")
    {
        // Verify zero on construction
        CHECK(cif0.reference_level() == false);
        // Setter
        cif0.reference_level(true);
        // Getter check set value
        CHECK(cif0.reference_level() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x01);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.reference_level() == true);
    }

    SECTION("Gain")
    {
        // Verify zero on construction
        CHECK(cif0.gain() == false);
        // Setter
        cif0.gain(true);
        // Getter check set value
        CHECK(cif0.gain() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x80);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.gain() == true);
    }

    SECTION("Over-range Count")
    {
        // Verify zero on construction
        CHECK(cif0.over_range_count() == false);
        // Setter
        cif0.over_range_count(true);
        // Getter check set value
        CHECK(cif0.over_range_count() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x40);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.over_range_count() == true);
    }

    SECTION("Sample Rate")
    {
        // Verify zero on construction
        CHECK(cif0.sample_rate() == false);
        // Setter
        cif0.sample_rate(true);
        // Getter check set value
        CHECK(cif0.sample_rate() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x20);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.sample_rate() == true);
    }

    SECTION("Timestamp Adjustment")
    {
        // Verify zero on construction
        CHECK(cif0.timestamp_adjustment() == false);
        // Setter
        cif0.timestamp_adjustment(true);
        // Getter check set value
        CHECK(cif0.timestamp_adjustment() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x10);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.timestamp_adjustment() == true);
    }

    SECTION("Timestamp Calibration Time")
    {
        // Verify zero on construction
        CHECK(cif0.timestamp_calibration_time() == false);
        // Setter
        cif0.timestamp_calibration_time(true);
        // Getter check set value
        CHECK(cif0.timestamp_calibration_time() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x08);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.timestamp_calibration_time() == true);
    }

    SECTION("Temperature")
    {
        // Verify zero on construction
        CHECK(cif0.temperature() == false);
        // Setter
        cif0.temperature(true);
        // Getter check set value
        CHECK(cif0.temperature() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x04);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.temperature() == true);
    }

    SECTION("Device Identifier")
    {
        // Verify zero on construction
        CHECK(cif0.device_id() == false);
        // Setter
        cif0.device_id(true);
        // Getter check set value
        CHECK(cif0.device_id() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x02);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.device_id() == true);
    }

    SECTION("State/Event Indicators")
    {
        // Verify zero on construction
        CHECK(cif0.state_event_indicators() == false);
        // Setter
        cif0.state_event_indicators(true);
        // Getter check set value
        CHECK(cif0.state_event_indicators() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x01);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.state_event_indicators() == true);
    }

    SECTION("Signal Data Packet Payload Format")
    {
        // Verify zero on construction
        CHECK(cif0.signal_data_format() == false);
        // Setter
        cif0.signal_data_format(true);
        // Getter check set value
        CHECK(cif0.signal_data_format() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x80);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.signal_data_format() == true);
    }

    SECTION("Formatted GPS")
    {
        // Verify zero on construction
        CHECK(cif0.formatted_gps() == false);
        // Setter
        cif0.formatted_gps(true);
        // Getter check set value
        CHECK(cif0.formatted_gps() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x40);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.formatted_gps() == true);
    }

    SECTION("Formatted INS")
    {
        // Verify zero on construction
        CHECK(cif0.formatted_ins() == false);
        // Setter
        cif0.formatted_ins(true);
        // Getter check set value
        CHECK(cif0.formatted_ins() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x20);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.formatted_ins() == true);
    }

    SECTION("ECEF Ephemeris")
    {
        // Verify zero on construction
        CHECK(cif0.ecef_ephemeris() == false);
        // Setter
        cif0.ecef_ephemeris(true);
        // Getter check set value
        CHECK(cif0.ecef_ephemeris() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x10);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.ecef_ephemeris() == true);
    }

    SECTION("Relative Ephemeris")
    {
        // Verify zero on construction
        CHECK(cif0.relative_ephemeris() == false);
        // Setter
        cif0.relative_ephemeris(true);
        // Getter check set value
        CHECK(cif0.relative_ephemeris() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x08);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.relative_ephemeris() == true);
    }

    SECTION("Ephemeris Ref ID")
    {
        // Verify zero on construction
        CHECK(cif0.ephemeris_ref_id() == false);
        // Setter
        cif0.ephemeris_ref_id(true);
        // Getter check set value
        CHECK(cif0.ephemeris_ref_id() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x04);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.ephemeris_ref_id() == true);
    }

    SECTION("GPS ASCII")
    {
        // Verify zero on construction
        CHECK(cif0.gps_ascii() == false);
        // Setter
        cif0.gps_ascii(true);
        // Getter check set value
        CHECK(cif0.gps_ascii() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x02);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.gps_ascii() == true);
    }

    SECTION("Context Association Lists")
    {
        // Verify zero on construction
        CHECK(cif0.context_association_lists() == false);
        // Setter
        cif0.context_association_lists(true);
        // Getter check set value
        CHECK(cif0.context_association_lists() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x01);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.context_association_lists() == true);
    }

    SECTION("CIF7 Enable")
    {
        // Verify zero on construction
        CHECK(cif0.cif7_enable() == false);
        // Setter
        cif0.cif7_enable(true);
        // Getter check set value
        CHECK(cif0.cif7_enable() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[3] == 0x80);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.cif7_enable() == true);
    }

    SECTION("CIF7 Enable")
    {
        // Verify zero on construction
        CHECK(cif0.cif3_enable() == false);
        // Setter
        cif0.cif3_enable(true);
        // Getter check set value
        CHECK(cif0.cif3_enable() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[3] == 0x08);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.cif3_enable() == true);
    }

    SECTION("CIF7 Enable")
    {
        // Verify zero on construction
        CHECK(cif0.cif2_enable() == false);
        // Setter
        cif0.cif2_enable(true);
        // Getter check set value
        CHECK(cif0.cif2_enable() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[3] == 0x04);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.cif2_enable() == true);
    }

    SECTION("CIF1 Enable")
    {
        // Verify zero on construction
        CHECK(cif0.cif1_enable() == false);
        // Setter
        cif0.cif1_enable(true);
        // Getter check set value
        CHECK(cif0.cif1_enable() == true);
        // Pack
        cif0.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[3] == 0x02);
        // Unpack
        unpack_cif0.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif0.cif1_enable() == true);
    }
}

TEST_CASE("GPS ASCII") {
    GPS_ASCII gps_ascii;
    GPS_ASCII unpack_gps_ascii;
    bytes packed_bytes{ 
        0xFF, 0xFF, 0xFF, 0xFF, // OUI
        0xFF, 0xFF, 0xFF, 0xFF, // Number of words
        0xFF, 0xFF, 0xFF, 0xFF, // Sentence
    };

    SECTION("Rule 9.4.7-1") {
        CHECK(true);
    }

    SECTION("Rule 9.4.7-2") {
        // Verify Size
        CHECK(gps_ascii.size() == 8); // oui and num of words
        // Setters
        uint32_t OUI = 0x123456;
        gps_ascii.manufacturer_oui(OUI);
        // Getters check set value
        CHECK(gps_ascii.manufacturer_oui() == OUI);
        // Pack
        gps_ascii.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 
                0x00, 0x12, 0x34, 0x56, // OUI
                0x00, 0x00, 0x00, 0x00, // Number of words
                0xFF, 0xFF, 0xFF, 0xFF, // Sentence
        });
        // Unpack
        unpack_gps_ascii.unpack_from(packed_bytes.data());
        // Verify unpacked values
        CHECK(unpack_gps_ascii.size() == 8); // oui and num of words
        CHECK(unpack_gps_ascii.manufacturer_oui() == OUI);
        CHECK(unpack_gps_ascii.number_of_words() == 0);
    }

    SECTION("Rule 9.4.7-3") {
        // Verify Size and Number of Words
        CHECK(gps_ascii.size() == 8);
        CHECK(gps_ascii.number_of_words() == 0);
        // Setters
        std::vector<uint8_t> ascii_sentences { 0xDE, 0xAD, 0xBE, 0xEF};
        gps_ascii.ascii_sentences(ascii_sentences);
        // Getters check set value
        CHECK(gps_ascii.number_of_words() == 1);
        // Pack
        gps_ascii.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 
                0x00, 0x00, 0x00, 0x00, // OUI
                0x00, 0x00, 0x00, 0x01, // Number of words
                0xDE, 0xAD, 0xBE, 0xEF  // ASCII WORD
        });
        // Unpack
        unpack_gps_ascii.unpack_from(packed_bytes.data());
        // Verify unpacked values
        CHECK(unpack_gps_ascii.size() == 12);
        CHECK(unpack_gps_ascii.number_of_words() == 1);
        CHECK(unpack_gps_ascii.ascii_sentences() == ascii_sentences);
    }

    SECTION("Rule 9.4.7-4") {
        /*
         * Developer's note: Rule does not apply to tool
         */
        CHECK(true);
    }

    SECTION("Rule 9.4.7-5"){
        // Verify Size and Number of Words
        CHECK(gps_ascii.size() == 8);
        CHECK(gps_ascii.number_of_words() == 0);
        // Setters
        std::vector<uint8_t> ascii_sentences { 0xBE, 0xEF};
        gps_ascii.ascii_sentences(ascii_sentences);
        // Getters check set value
        CHECK(gps_ascii.number_of_words() == 1);
        // Pack
        gps_ascii.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 
                0x00, 0x00, 0x00, 0x00, // OUI
                0x00, 0x00, 0x00, 0x01, // Number of words
                0xBE, 0xEF, 0x00, 0x00  // ACII WORD
        });
        // Unpack
        unpack_gps_ascii.unpack_from(packed_bytes.data());
        // Verify unpacked values
        CHECK(unpack_gps_ascii.size() == 12);
        CHECK(unpack_gps_ascii.number_of_words() == 1);
        CHECK(unpack_gps_ascii.ascii_sentences() == bytes { 0xBE, 0xEF, 0x00, 0x00});
    }
}

TEST_CASE("Gain/Attenuation Field (9.5.3)", "[cif0][gain]")
{
    Gain gain;
    Gain unpack_gain;
    constexpr std::size_t NUM_BYTES{ 4 };
    auto packed_bytes = bytes(NUM_BYTES, 0xFF);

    SECTION("Zero on construction")
    {
        gain.pack_into(packed_bytes.data());
        CHECK(packed_bytes == bytes{ 0, 0, 0, 0 });
    }

    SECTION("Rule 9.5.3-1")
    {
        // Developer's note: Rule does not apply to tool
        CHECK(true);
    }

    SECTION("Rule 9.5.3-2")
    {
        // Developer's note: Rule does not apply to tool
        CHECK(true);
    }

    SECTION("Rule 9.5.3-3")
    {
        // See Figure 9.5.3-1
        // Verify size
        CHECK(gain.size() == NUM_BYTES);
        // Setters
        gain.stage_1(1.0);
        gain.stage_2(1.0);
        // Getters check set value
        CHECK(gain.stage_1() == 1.0);
        CHECK(gain.stage_2() == 1.0);
        // Pack
        gain.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 0, 0x80, 0, 0x80 });
        // Unpack
        unpack_gain.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_gain.stage_1() == 1.0);
        CHECK(unpack_gain.stage_2() == 1.0);
    }

    SECTION("Rule 9.5.3-4")
    {
        // See Observations 9.5.3-3 and 9.5.3-4 for values
        SECTION("Gain Value 1")
        {
            // Setter
            gain.stage_1(1.0);
            // Getter check set value
            CHECK(gain.stage_1() == 1.0);
            // Pack
            gain.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0, 0, 0, 0x80 });
            // Unpack
            unpack_gain.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_gain.stage_1() == 1.0);
        }
        SECTION("Gain Value 2")
        {
            // Setter
            gain.stage_1(-1.0);
            // Getter check set value
            CHECK(gain.stage_1() == -1.0);
            // Pack
            gain.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0, 0, 0xFF, 0x80 });
            // Unpack
            unpack_gain.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_gain.stage_1() == -1.0);
        }
        SECTION("Gain Value 3")
        {
            // Setter
            gain.stage_1(0.0078125);
            // Getter check set value
            CHECK(gain.stage_1() == 0.0078125);
            // Pack
            gain.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0, 0, 0, 0x01 });
            // Unpack
            unpack_gain.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_gain.stage_1() == 0.0078125);
        }
        SECTION("Gain Value 4")
        {
            // Setter
            gain.stage_1(-0.0078125);
            // Getter check set value
            CHECK(gain.stage_1() == -0.0078125);
            // Pack
            gain.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0, 0, 0xFF, 0xFF });
            // Unpack
            unpack_gain.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_gain.stage_1() == -0.0078125);
        }
    }

    SECTION("Rule 9.5.3-5")
    {
        // See Observations 9.5.3-3, 9.5.3-4, 9.5.3-5 for values
        SECTION("Gain Value 1")
        {
            // Setters
            gain.stage_1(1.0);
            gain.stage_2(1.0);
            // Getters check set value
            CHECK(gain.stage_1() == 1.0);
            CHECK(gain.stage_2() == 1.0);
            // Pack
            gain.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0, 0x80, 0, 0x80 });
            // Unpack
            unpack_gain.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_gain.stage_1() == 1.0);
            CHECK(unpack_gain.stage_2() == 1.0);
        }
        SECTION("Gain Value 2")
        {
            // Setters
            gain.stage_1(-1.0);
            gain.stage_2(-1.0);
            // Getters check set value
            CHECK(gain.stage_1() == -1.0);
            CHECK(gain.stage_2() == -1.0);
            // Pack
            gain.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0xFF, 0x80, 0xFF, 0x80 });
            // Unpack
            unpack_gain.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_gain.stage_1() == -1.0);
            CHECK(unpack_gain.stage_2() == -1.0);
        }
        SECTION("Gain Value 3")
        {
            // Setters
            gain.stage_1(0.0078125);
            gain.stage_2(0.0078125);
            // Getters check set value
            CHECK(gain.stage_1() == 0.0078125);
            CHECK(gain.stage_2() == 0.0078125);
            // Pack
            gain.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0, 0x01, 0, 0x01 });
            // Unpack
            unpack_gain.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_gain.stage_1() == 0.0078125);
            CHECK(unpack_gain.stage_2() == 0.0078125);
        }
        SECTION("Gain Value 4")
        {
            // Setters
            gain.stage_1(-0.0078125);
            gain.stage_2(-0.0078125);
            // Getters check set value
            CHECK(gain.stage_1() == -0.0078125);
            CHECK(gain.stage_2() == -0.0078125);
            // Pack
            gain.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0xFF, 0xFF, 0xFF, 0xFF });
            // Unpack
            unpack_gain.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_gain.stage_1() == -0.0078125);
            CHECK(unpack_gain.stage_2() == -0.0078125);
        }
    }

    SECTION("Rule 9.5.3-6")
    {
        // Setter
        gain.stage_1(1.0);
        // Getter check set value
        CHECK(gain.stage_1() == 1.0);
        CHECK(gain.stage_2() == 0.0);
        // Pack
        gain.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 0, 0, 0, 0x80 });
        // Unpack
        unpack_gain.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_gain.stage_1() == 1.0);
        CHECK(unpack_gain.stage_2() == 0.0);
    }

    SECTION("Rule 9.5.3-7")
    {
        // Developer's note: Rule does not apply to tool
        CHECK(true);
    }

    SECTION("Rule 9.5.3-8")
    {
        // Developer's note: Rule does not apply to tool
        CHECK(true);
    }
}

TEST_CASE("Device Identifier", "[cif0][device_id]")
{
    DeviceIdentifier device_id;
    DeviceIdentifier unpack_device_id;
    constexpr std::size_t NUM_BYTES{ 8 };
    auto packed_bytes = bytes(NUM_BYTES, 0xFF);

    SECTION("Zero on construction")
    {
        device_id.pack_into(packed_bytes.data());
        CHECK(packed_bytes == bytes{ 0, 0, 0, 0, 0, 0, 0, 0 });
    }

    SECTION("Rule 9.10.1-1")
    {
        // See Figure 9.10.1-1
        // Verify size
        CHECK(device_id.size() == NUM_BYTES);
        // Setters
        device_id.manufacturer_oui(0xABCDEF);
        device_id.device_code(0x123);
        // Getters check set value
        CHECK(device_id.manufacturer_oui() == 0xABCDEF);
        CHECK(device_id.device_code() == 0x123);
        // Pack
        device_id.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 0, 0xAB, 0xCD, 0xEF, 0, 0, 0x01, 0x23 });
        // Unpack
        unpack_device_id.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_device_id.manufacturer_oui() == 0xABCDEF);
        CHECK(unpack_device_id.device_code() == 0x123);
    }

    SECTION("Rule 9.10.1-2")
    {
        // Setter
        device_id.manufacturer_oui(0x12AB34);
        // Getter check set value
        CHECK(device_id.manufacturer_oui() == 0x12AB34);
        // Pack
        device_id.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 0, 0x12, 0xAB, 0x34, 0, 0, 0, 0 });
        // Unpack
        unpack_device_id.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_device_id.manufacturer_oui() == 0x12AB34);
    }

    SECTION("Rule 9.10.1-3")
    {
        // Setter
        device_id.device_code(0xABC);
        // Getter check set value
        CHECK(device_id.device_code() == 0xABC);
        // Pack
        device_id.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 0, 0, 0, 0, 0, 0, 0x0A, 0xBC });
        // Unpack
        unpack_device_id.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_device_id.device_code() == 0xABC);
    }
}

TEST_CASE("State/Event Indicators", "[cif0][state_event_indicators]")
{
    StateEventIndicators state_event_ind;
    StateEventIndicators unpack_state_event_ind;
    bytes packed_bytes{ 0xFF, 0xFF, 0xFF, 0xFF };

    SECTION("Zero on construction")
    {
        state_event_ind.pack_into(packed_bytes.data());
        CHECK(packed_bytes == bytes{ 0, 0, 0, 0 });
    }

    SECTION("Calibrated Time")
    {
        // Verify zero on construction
        CHECK(state_event_ind.calibrated_time().has_value() == false);
        // Setter
        state_event_ind.calibrated_time(true);
        // Getter check set value
        CHECK(state_event_ind.calibrated_time().has_value());
        CHECK(state_event_ind.calibrated_time().value() == true);
        // Pack
        state_event_ind.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 0x80, 0x08, 0, 0 });
        // Unpack
        unpack_state_event_ind.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_state_event_ind.calibrated_time().has_value());
        CHECK(unpack_state_event_ind.calibrated_time() == true);
    }

    SECTION("Valid Data")
    {
        // Verify zero on construction
        CHECK(state_event_ind.valid_data().has_value() == false);
        // Setter
        state_event_ind.valid_data(true);
        // Getter check set value
        CHECK(state_event_ind.valid_data().has_value());
        CHECK(state_event_ind.valid_data().value() == true);
        // Pack
        state_event_ind.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 0x40, 0x04, 0, 0 });
        // Unpack
        unpack_state_event_ind.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_state_event_ind.valid_data().has_value());
        CHECK(unpack_state_event_ind.valid_data().value() == true);
    }

    SECTION("Reference Lock")
    {
        // Verify zero on construction
        CHECK(state_event_ind.reference_lock().has_value() == false);
        // Setter
        state_event_ind.reference_lock(true);
        // Getter check set value
        CHECK(state_event_ind.reference_lock().has_value());
        CHECK(state_event_ind.reference_lock().value() == true);
        // Pack
        state_event_ind.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 0x20, 0x02, 0, 0 });
        // Unpack
        unpack_state_event_ind.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_state_event_ind.reference_lock().has_value());
        CHECK(unpack_state_event_ind.reference_lock() == true);
    }

    SECTION("AGC/MGC")
    {
        // Verify zero on construction
        CHECK(state_event_ind.agc_mgc().has_value() == false);
        // Setter
        state_event_ind.agc_mgc(true);
        // Getter check set value
        CHECK(state_event_ind.agc_mgc().has_value());
        CHECK(state_event_ind.agc_mgc().value() == true);
        // Pack
        state_event_ind.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 0x10, 0x01, 0, 0 });
        // Unpack
        unpack_state_event_ind.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_state_event_ind.agc_mgc().has_value());
        CHECK(unpack_state_event_ind.agc_mgc().value() == true);
    }

    SECTION("Detected Signal")
    {
        // Verify zero on construction
        CHECK(state_event_ind.detected_signal().has_value() == false);
        // Setter
        state_event_ind.detected_signal(true);
        // Getter check set value
        CHECK(state_event_ind.detected_signal().has_value());
        CHECK(state_event_ind.detected_signal().value() == true);
        // Pack
        state_event_ind.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 0x08, 0, 0x80, 0 });
        // Unpack
        unpack_state_event_ind.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_state_event_ind.detected_signal().has_value());
        CHECK(unpack_state_event_ind.detected_signal().value() == true);
    }

    SECTION("Spectral Inversion")
    {
        // Verify zero on construction
        CHECK(state_event_ind.spectral_inversion().has_value() == false);
        // Setter
        state_event_ind.spectral_inversion(true);
        // Getter check set value
        CHECK(state_event_ind.spectral_inversion().has_value());
        CHECK(state_event_ind.spectral_inversion().value() == true);
        // Pack
        state_event_ind.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 0x04, 0, 0x40, 0 });
        // Unpack
        unpack_state_event_ind.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_state_event_ind.spectral_inversion().has_value());
        CHECK(unpack_state_event_ind.spectral_inversion().value() == true);
    }

    SECTION("Over-Range")
    {
        // Verify zero on construction
        CHECK(state_event_ind.over_range().has_value() == false);
        // Setter
        state_event_ind.over_range(true);
        // Getter check set value
        CHECK(state_event_ind.over_range().has_value());
        CHECK(state_event_ind.over_range().value() == true);
        // Pack
        state_event_ind.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 0x2, 0, 0x20, 0 });
        // Unpack
        unpack_state_event_ind.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_state_event_ind.over_range().has_value());
        CHECK(unpack_state_event_ind.over_range().value() == true);
    }

    SECTION("Sample Loss")
    {
        // Verify zero on construction
        CHECK(state_event_ind.sample_loss().has_value() == false);
        // Setter
        state_event_ind.sample_loss(true);
        // Getter check set value
        CHECK(state_event_ind.sample_loss().has_value());
        CHECK(state_event_ind.sample_loss().value() == true);
        // Pack
        state_event_ind.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 0x01, 0, 0x10, 0 });
        // Unpack
        unpack_state_event_ind.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_state_event_ind.sample_loss().has_value());
        CHECK(unpack_state_event_ind.sample_loss().value() == true);
    }
}

TEST_CASE("Geolocation", "[cif0][gps][ins]")
{
    Geolocation geo;
    Geolocation unpack_geo;
    constexpr std::size_t NUM_BYTES{ 44 };
    bytes packed_bytes(NUM_BYTES, 0xFF);

    SECTION("Default construction")
    {
        geo.pack_into(packed_bytes.data());
        CHECK(packed_bytes == bytes { 
            0x00, 0x00, 0x00, 0x00, // word 1
            0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
            0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
        });
    }

    SECTION("Rule 9.4.5-1 and Rule 9.4.6-1")
    {
        // See Figure 9.4.5-1
        // Verify size
        CHECK(geo.size() == NUM_BYTES);
        // Setters
        geo.tsi(TSI::UTC);
        geo.tsf(TSF::REAL_TIME);
        geo.manufacturer_oui(0xABCDEF);
        geo.integer_timestamp(0x123ABC);
        geo.fractional_timestamp(0xABC123456789DEF);
        geo.latitude(1.0);
        geo.longitude(1.0);
        geo.altitude(1.0);
        geo.speed_over_ground(1.0);
        geo.heading_angle(1.0);
        geo.track_angle(1.0);
        geo.magnetic_variation(1.0);
        // Getters check set value
        CHECK(geo.tsi() == TSI::UTC);
        CHECK(geo.tsf() == TSF::REAL_TIME);
        CHECK(geo.manufacturer_oui() == 0xABCDEF);
        CHECK(geo.integer_timestamp() == 0x123ABC);
        CHECK(geo.fractional_timestamp() == 0xABC123456789DEF);
        CHECK(geo.latitude() == 1.0);
        CHECK(geo.longitude() == 1.0);
        CHECK(geo.altitude() == 1.0);
        CHECK(geo.speed_over_ground() == 1.0);
        CHECK(geo.heading_angle() == 1.0);
        CHECK(geo.track_angle() == 1.0);
        CHECK(geo.magnetic_variation() == 1.0);
        // Pack
        geo.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{
            0x06, 0xAB, 0xCD, 0xEF,
            0x00, 0x12, 0x3A, 0xBC,
            0x0A, 0xBC, 0x12, 0x34,
            0x56, 0x78, 0x9D, 0xEF,
            0x00, 0x40, 0x00, 0x00,
            0x00, 0x40, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x20,
            0x00, 0x01, 0x00, 0x00,
            0x00, 0x40, 0x00, 0x00,
            0x00, 0x40, 0x00, 0x00,
            0x00, 0x40, 0x00, 0x00
        });
        // Unpack
        unpack_geo.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_geo.tsi() == TSI::UTC);
        CHECK(unpack_geo.tsf() == TSF::REAL_TIME);
        CHECK(unpack_geo.manufacturer_oui() == 0xABCDEF);
        CHECK(unpack_geo.integer_timestamp() == 0x123ABC);
        CHECK(unpack_geo.fractional_timestamp() == 0xABC123456789DEF);
        CHECK(unpack_geo.latitude() == 1.0);
        CHECK(unpack_geo.longitude() == 1.0);
        CHECK(unpack_geo.altitude() == 1.0);
        CHECK(unpack_geo.speed_over_ground() == 1.0);
        CHECK(unpack_geo.heading_angle() == 1.0);
        CHECK(unpack_geo.track_angle() == 1.0);
        CHECK(unpack_geo.magnetic_variation() == 1.0);
    }

    SECTION("Rule 9.4.5-2 and Rule 9.4.6-1")
    {
        // Setter
        geo.manufacturer_oui(0x12AB34);
        // Getter check set value
        CHECK(geo.manufacturer_oui() == 0x12AB34);
        // Pack
        geo.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{
            0x00, 0x12, 0xAB, 0x34, // word 1
            0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
            0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
        });
        // Unpack
        unpack_geo.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_geo.manufacturer_oui() == 0x12AB34);
    }

    SECTION("Rule 9.4.5-3 and Rule 9.4.6-1")
    {
        // See Table 9.4.5-1
        SECTION("Undefined")
        {
            // Setter
            geo.tsi(TSI::NONE);
            // Getter check set value
            CHECK(geo.tsi() == TSI::NONE);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes { 
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.tsi() == TSI::NONE);
        }
        SECTION("UTC")
        {
            // Setter
            geo.tsi(TSI::UTC);
            geo.integer_timestamp(0x1234);
            // Getter check set value
            CHECK(geo.tsi() == TSI::UTC);
            CHECK(geo.integer_timestamp() == 0x1234);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes { 
                0x04, 0x00, 0x00, 0x00, // word 1
                0x00, 0x00, 0x12, 0x34, // word 2
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.tsi() == TSI::UTC);
            CHECK(unpack_geo.integer_timestamp() == 0x1234);
        }
        SECTION("GPS Time")
        {
            // Setter
            geo.tsi(TSI::GPS);
            geo.integer_timestamp(0x1234);
            // Getter check set value
            CHECK(geo.tsi() == TSI::GPS);
            CHECK(geo.integer_timestamp() == 0x1234);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes { 
                0x08, 0x00, 0x00, 0x00, // word 1
                0x00, 0x00, 0x12, 0x34, // word 2
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.tsi() == TSI::GPS);
            CHECK(unpack_geo.integer_timestamp() == 0x1234);
        }
        SECTION("Other")
        {
            // Setter
            geo.tsi(TSI::OTHER);
            geo.integer_timestamp(0x1234);
            // Getter check set value
            CHECK(geo.tsi() == TSI::OTHER);
            CHECK(geo.integer_timestamp() == 0x1234);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes { 
                0x0C, 0x00, 0x00, 0x00, // word 1
                0x00, 0x00, 0x12, 0x34, // word 2
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.tsi() == TSI::OTHER);
            CHECK(unpack_geo.integer_timestamp() == 0x1234);
        }
    }

    SECTION("Rule 9.4.5-4 and Rule 9.4.6-1")
    {
        // See Table 9.4.5-2
        SECTION("Undefined")
        {
            // Setter
            geo.tsf(TSF::NONE);
            // Getter check set value
            CHECK(geo.tsf() == TSF::NONE);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes { 
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.tsf() == TSF::NONE);
        }
        SECTION("Sample Count")
        {
            // Setter
            geo.tsf(TSF::SAMPLE_COUNT);
            geo.fractional_timestamp(0x1234ABCDEF);
            // Getter check set value
            CHECK(geo.tsf() == TSF::SAMPLE_COUNT);
            CHECK(geo.fractional_timestamp() == 0x1234ABCDEF);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes { 
                0x01, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0x00, 0x00, 0x00, 0x12, // word 3
                0x34, 0xAB, 0xCD, 0xEF, // word 4
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.tsf() == TSF::SAMPLE_COUNT);
            CHECK(unpack_geo.fractional_timestamp() == 0x1234ABCDEF);
        }
        SECTION("Real (picoseconds) Time")
        {
            // Setter
            geo.tsf(TSF::REAL_TIME);
            geo.fractional_timestamp(0x1234ABCDEF);
            // Getter check set value
            CHECK(geo.tsf() == TSF::REAL_TIME);
            CHECK(geo.fractional_timestamp() == 0x1234ABCDEF);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes { 
                0x02, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0x00, 0x00, 0x00, 0x12, // word 3
                0x34, 0xAB, 0xCD, 0xEF, // word 4
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.tsf() == TSF::REAL_TIME);
            CHECK(unpack_geo.fractional_timestamp() == 0x1234ABCDEF);
        }
        SECTION("Free-Running")
        {
            // Setters
            geo.tsf(TSF::FREE_RUNNING);
            geo.fractional_timestamp(0x1234ABCDEF);
            // Getters check set value
            CHECK(geo.tsf() == TSF::FREE_RUNNING);
            CHECK(geo.fractional_timestamp() == 0x1234ABCDEF);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes { 
                0x03, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0x00, 0x00, 0x00, 0x12, // word 3
                0x34, 0xAB, 0xCD, 0xEF, // word 4
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.tsf() == TSF::FREE_RUNNING);
            CHECK(unpack_geo.fractional_timestamp() == 0x1234ABCDEF);
        }
    }

    SECTION("Rule 9.4.5-5 and Rule 9.4.6-1")
    {
        /*
         * Rule 9.4.5-5: When the TSI or TSF fields are non-zero the
         * corresponding Timestamp of Position Fix subfield shall express the
         * time of the most recent location fix in the format given in Sections
         * 5.1.4.1 and 5.1.4.2
         */
        // Setters
        geo.tsi(TSI::UTC);
        geo.tsf(TSF::REAL_TIME);
        geo.integer_timestamp(0x12345678);
        geo.fractional_timestamp(0x12345678ABCDEF);
        // Getters check set value
        CHECK(geo.tsi() == TSI::UTC);
        CHECK(geo.tsf() == TSF::REAL_TIME);
        CHECK(geo.integer_timestamp() == 0x12345678);
        CHECK(geo.fractional_timestamp() == 0x12345678ABCDEF);
        // Pack
        geo.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 
            0x06, 0x00, 0x00, 0x00, // word 1
            0x12, 0x34, 0x56, 0x78, // word 2
            0x00, 0x12, 0x34, 0x56, // word 3
            0x78, 0xAB, 0xCD, 0xEF, // word 4
            0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
        });
        // Unpack
        unpack_geo.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_geo.tsi() == TSI::UTC);
        CHECK(unpack_geo.tsf() == TSF::REAL_TIME);
        CHECK(unpack_geo.integer_timestamp() == 0x12345678);
        CHECK(unpack_geo.fractional_timestamp() == 0x12345678ABCDEF);
    }

    SECTION("Rule 9.4.5-6 and Rule 9.4.6-1")
    {
        /*
         * Rule 9.4.5-6: When the TSI or TSF fields are zero the
         * corresponding Timestamp of Position Fix subfield words shall
         * take the value 0xFFFFFFFF.
         */
        // Setters
        geo.tsi(TSI::NONE);
        geo.tsf(TSF::NONE);
        // Pack
        geo.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 
            0x00, 0x00, 0x00, 0x00, // word 1
            0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
            0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
        });
        // Unpack
        unpack_geo.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_geo.tsi() == TSI::NONE);
        CHECK(unpack_geo.tsf() == TSF::NONE);
        CHECK(unpack_geo.integer_timestamp() == 0xFFFFFFFF);
        CHECK(unpack_geo.fractional_timestamp() == 0xFFFFFFFFFFFFFFFF);
    }

    SECTION("Rule 9.4.5-7 and Rule 9.4.6-1")
    {
        /*
         * Rule 9.4.5-7: The Latitude and Longitude subfields shall use the
         * Geolocation Angle Format shown in Figure 9.4.5-2.
         *
         * --------------------------------------------------------------------------------------------------------
         * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
         * | 1    |                                    Angle (31..0), degrees                                     |
         * --------------------------------------------------------------------------------------------------------
         *                                      ^
         *                                      |
         *                                 Radix Point
         */
        double LATITUDE = 1.0 + (1.0 / (1 << 22));
        double LONGITUDE = 1.0 + (1.0 / (1 << 22));
        // Setters
        geo.latitude(LATITUDE);
        geo.longitude(LONGITUDE);
        // Getter check set value
        CHECK(geo.latitude() == LATITUDE);
        CHECK(geo.longitude() == LONGITUDE);
        // Pack
        geo.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 
            0x00, 0x00, 0x00, 0x00, // word 1
            0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-18)
            0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-18)
            0x00, 0x40, 0x00, 0x01, // word 5 
            0x00, 0x40, 0x00, 0x01, // word 6
            0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
        });
        // Unpack
        unpack_geo.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_geo.latitude() == LATITUDE);
        CHECK(unpack_geo.longitude() == LONGITUDE);
    }

    SECTION("Rule 9.4.5-8 and Rule 9.4.6-1")
    {
        /*
         * Rule 9.4.5-8: The Latitude subfield in the GPS Geolocation Field
         * value shall range from -90.0 (South) to +90.0 (North) degrees.
         */
        SECTION("Min")
        {
            // Setters
            geo.latitude(-90.0); // hex(-90<<22(radix)) = 0xe9800000
            // Getter check set value
            CHECK(geo.latitude() == -90.0);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0xE9, 0x80, 0x00, 0x00, // word 5 
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.latitude() == -90.0);
        }
        SECTION("Max")
        {
            // Setters
            geo.latitude(90.0); // hex(90<<22(radix)) = 0x16800000
            // Getter check set value
            CHECK(geo.latitude() == 90.0);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x16, 0x80, 0x00, 0x00, // word 5 
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.latitude() == 90.0);
        }
    }

    SECTION("Rule 9.4.5-9 and Rule 9.4.6-1")
    {
        /*
         * Rule 9.4.5-9: The Longitude subfield in the GPS Geolocation Field
         * value shall range from -180.0 (West) to +180.0 (East) degrees.
         */
        SECTION("Min")
        {
            // Setters
            geo.longitude(-180.0); // hex(-180<<22(radix)) = 0xd3000000
            // Getter check set value
            CHECK(geo.longitude() == -180.0);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0xD3, 0x00, 0x00, 0x00, // word 6
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.longitude() == -180.0);
        }
        SECTION("Max")
        {
            // Setters
            geo.longitude(180.0); // hex(180<<22(radix)) = 0x2d000000
            // Getter check set value
            CHECK(geo.longitude() == 180.0);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18) 
                0x2D, 0x00, 0x00, 0x00, // word 6
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.longitude() == 180.0);
        }
    }

    SECTION("Rule 9.4.5-9 and Rule 9.4.6-1")
    {
        /*
         * Rule 9.4.5-9: The Longitude subfield in the GPS Geolocation Field
         * value shall range from -180.0 (West) to +180.0 (East) degrees.
         */
        SECTION("Min")
        {
            // Setters
            geo.longitude(-180.0); // hex(-180<<22(radix)) = 0xd3000000
            // Getter check set value
            CHECK(geo.longitude() == -180.0);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0xD3, 0x00, 0x00, 0x00, // word 6
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.longitude() == -180.0);
        }
        SECTION("Max")
        {
            // Setters
            geo.longitude(180.0); // hex(180<<22(radix)) = 0x2d000000
            // Getter check set value
            CHECK(geo.longitude() == 180.0);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18) 
                0x2D, 0x00, 0x00, 0x00, // word 6
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.longitude() == 180.0);
        }
    }

    SECTION("Rule 9.4.5.10 and Rule 9.4.6-1")
    {
        /*
         * Rule 9.4.5-10: The Altitude subfield in the GPS Geolocation Field
         * shall use the 32-bit, two’s-complement format shown in Figure
         * 9.4.5-3 with the value of the Altitude subfield expressed in
         * units of meters. This field has an integer and a fractional part
         * with the radix point to the right of bit 5.
         * 
         * --------------------------------------------------------------------------------------------------------
         * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
         * | 1    |                                  Altitude (31..0), meters                                     |
         * --------------------------------------------------------------------------------------------------------
         *                                                                                         ^
         *                                                                                         |
         *                                                                                    Radix Point
         */
        double ALTITUDE = 1.0 + (1.0 / (1 << 5));
        // Setters
        geo.altitude(ALTITUDE);
        // Getter check set value
        CHECK(geo.altitude() == ALTITUDE);
        // Pack
        geo.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{
            0x00, 0x00, 0x00, 0x00, // word 1
            0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
            0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18) 
            0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
            0x00, 0x00, 0x00, 0x21, // word 7
            0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
        });
        // Unpack
        unpack_geo.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_geo.altitude() == ALTITUDE);
    }

    SECTION("Rule 9.4.5.11 and Rule 9.4.6-1")
    {
        /*
         * Rule 9.4.5.11: The Speed Over Ground subfield shall use the 32-bit,
         * two’s-complement format shown in Figure 9.4.5-4 with the value of
         * the Speed Over Ground subfield expressed in units of meters per
         * second. This field has an integer and a fractional part with the
         * radix point to the right of bit 16.
         * 
         * --------------------------------------------------------------------------------------------------------
         * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
         * | 1    |                           Speed over Ground (31..0), meters                                   |
         * --------------------------------------------------------------------------------------------------------
         *                                                        ^
         *                                                        |
         *                                                   Radix Point
         */
        double GROUND_SPEED = 1.0 + (1.0 / (1 << 16)); // represents 0001 0001
        // Setter
        geo.speed_over_ground(GROUND_SPEED);
        // Getter check set value
        CHECK(geo.speed_over_ground() == GROUND_SPEED);
        // Pack
        geo.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{
            0x00, 0x00, 0x00, 0x00, // word 1
            0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
            0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18) 
            0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
            0x00, 0x01, 0x00, 0x01, // word 8
            0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
        });
        // Unpack
        unpack_geo.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_geo.speed_over_ground() == GROUND_SPEED);
    }

    SECTION("Rule 9.4.5.12 and Rule 9.4.6-1")
    {
        /* 
         * Rule 9.4.5.12: The Heading Angle subfield shall use the Geolocation
         * Angle Format shown in Figure 9.4.5-2, with the Heading Angle
         * expressing the platform’s orientation with respect to true North in
         * decimal degrees.
         * 
         * --------------------------------------------------------------------------------------------------------
         * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
         * | 1    |                                    Angle (31..0), degrees                                     |
         * --------------------------------------------------------------------------------------------------------
         *                                      ^
         *                                      |
         *                                 Radix Point
         */
        double HEADING_ANGLE = 1.0 + (1.0 / (1 << 22)); // represents 0040 0001
        // Setter
        geo.heading_angle(HEADING_ANGLE);
        // Getter check set value
        CHECK(geo.heading_angle() == HEADING_ANGLE);
        // Pack
        geo.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{
            0x00, 0x00, 0x00, 0x00, // word 1
            0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
            0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18) 
            0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
            0x00, 0x40, 0x00, 0x01, // word 9
            0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
        });
        // Unpack
        unpack_geo.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_geo.heading_angle() == HEADING_ANGLE);
    }

    SECTION("Rule 9.4.5.13 and Rule 9.4.6-1")
    {
        /* 
         * Rule 9.4.5.13: The Heading Angle value shall range from 0.0 to
         * +359.999999761582 degrees.
         */
        SECTION("Min")
        {
            // Setter
            geo.heading_angle(0.0);
            // Getter check set value
            CHECK(geo.heading_angle() == 0.0);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18) 
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x00, 0x00, 0x00, 0x00, // word 9
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.heading_angle() == 0.0);
        }
        SECTION("Max")
        {
            double HEADING_ANGLE = 360.0 - (1.0 / (1 << 22)); // represents 59FF FFFFF
            // Setter
            geo.heading_angle(HEADING_ANGLE); 
            // Getter check set value
            CHECK(geo.heading_angle() == HEADING_ANGLE);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18) 
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x59, 0xFF, 0xFF, 0xFF, // word 9
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.heading_angle() == HEADING_ANGLE);
        }
    }

    SECTION("Rule 9.4.5.14 and Rule 9.4.6-1")
    {
        /* 
         * Rule 9.4.5.12: The Track Angle subfield shall use the Geolocation Angle
         * Format shown in Figure 9.4.5-2, with the Track Angle expressing
         * the platform’s direction of travel with respect to true North in
         * decimal degrees.
         * 
         * --------------------------------------------------------------------------------------------------------
         * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
         * | 1    |                                    Angle (31..0), degrees                                     |
         * --------------------------------------------------------------------------------------------------------
         *                                      ^
         *                                      |
         *                                 Radix Point
         */
        double TRACK_ANGLE = 1.0 + (1.0 / (1 << 22)); // represents 0040 0001
        // Setter
        geo.track_angle(TRACK_ANGLE);
        // Getter check set value
        CHECK(geo.track_angle() == TRACK_ANGLE);
        // Pack
        geo.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{
            0x00, 0x00, 0x00, 0x00, // word 1
            0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
            0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18) 
            0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
            0x00, 0x40, 0x00, 0x01, // word 10
            0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
        });
        // Unpack
        unpack_geo.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_geo.track_angle() == TRACK_ANGLE);
    }

    SECTION("Rule 9.4.5.15 and Rule 9.4.6-1")
    {
        /* 
         * Rule 9.4.5.15: The Track Angle value shall range from 0.0 to
         * +359.999999761582 degrees.
         */
        SECTION("Min")
        {
            // Setter
            geo.track_angle(0.0);
            // Getter check set value
            CHECK(geo.track_angle() == 0.0);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18) 
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x00, 0x00, 0x00, 0x00, // word 10
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.track_angle() == 0.0);
        }
        SECTION("Max")
        {
            double HEADING_ANGLE = 360.0 - (1.0 / (1 << 22)); // represents 59FF FFFFF
            // Setter
            geo.track_angle(HEADING_ANGLE); 
            // Getter check set value
            CHECK(geo.track_angle() == HEADING_ANGLE);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18) 
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x59, 0xFF, 0xFF, 0xFF, // word 10
                0x7F, 0xFF, 0xFF, 0xFF  // word 11 (per rule 9.4.5-18)
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.track_angle() == HEADING_ANGLE);
        }
    }

    SECTION("Rule 9.4.5.16 and Rule 9.4.6-1")
    {
        /* 
         * Rule 9.4.5.16: The Magnetic Variation subfield shall use the
         * Geolocation Angle Format shown in Figure 9.4.5-2, above, with the
         * Magnetic Variation subfield expressing magnetic variation from true
         * North in decimal degrees.
         * 
         * --------------------------------------------------------------------------------------------------------
         * | Word |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
         * | 1    |                                    Angle (31..0), degrees                                     |
         * --------------------------------------------------------------------------------------------------------
         *                                      ^
         *                                      |
         *                                 Radix Point
         */
        double MAGNETIC_VARIATION = 1.0 + (1.0 / (1 << 22)); // represents 0040 0001
        // Setter
        geo.magnetic_variation(MAGNETIC_VARIATION);
        // Getter check set value
        CHECK(geo.magnetic_variation() == MAGNETIC_VARIATION);
        // Pack
        geo.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{
            0x00, 0x00, 0x00, 0x00, // word 1
            0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
            0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18) 
            0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
            0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
            0x00, 0x40, 0x00, 0x01  // word 11
        });
        // Unpack
        unpack_geo.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_geo.magnetic_variation() == MAGNETIC_VARIATION);
    }
    
    SECTION("Rule 9.4.5.17 and Rule 9.4.6-1")
    {
        /*
         * Rule 9.4.5.17: The Magnetic Variation value shall range from -180.0
         * (West) to +180.0 (East) degrees.
         */
        SECTION("Min")
        {
            // Setters
            geo.magnetic_variation(-180.0); // hex(-180<<22(radix)) = 0xd3000000
            // Getter check set value
            CHECK(geo.magnetic_variation() == -180.0);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0xD3, 0x00, 0x00, 0x00  // word 11
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.magnetic_variation() == -180.0);
        }
        SECTION("Max")
        {
            // Setters
            geo.magnetic_variation(180.0); // hex(180<<22(radix)) = 0x2d000000
            // Getter check set value
            CHECK(geo.magnetic_variation() == 180.0);
            // Pack
            geo.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{
                0x00, 0x00, 0x00, 0x00, // word 1
                0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
                0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
                0x7F, 0xFF, 0xFF, 0xFF, // word 5 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 6 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 7 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 8 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 9 (per rule 9.4.5-18)
                0x7F, 0xFF, 0xFF, 0xFF, // word 10 (per rule 9.4.5-18)
                0x2D, 0x00, 0x00, 0x00  // word 11
            });
            // Unpack
            unpack_geo.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_geo.magnetic_variation() == 180.0);
        }
    }

    SECTION("Rule 9.4.5-18 and Rule 9.4.6-1") {
        /*
         * Rule 9.4.5-18: The Latitude, Longitude, Altitude, Speed Over Ground,
         * Heading, Track Angle, and Magnetic Variation subfields shall take
         * the value 0x7FFFFFFF when unspecified.
         * 
         * Observation 9.4.5-7: These default subfield values are invalid
         * or practically impossible for all GPS and INS Geolocation
         * subfields.
         */
        // Pack
        geo.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{
            0x00, 0x00, 0x00, 0x00, // word 1
            0xFF, 0xFF, 0xFF, 0xFF, // word 2 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 3 (per rule 9.4.5-6)
            0xFF, 0xFF, 0xFF, 0xFF, // word 4 (per rule 9.4.5-6)
            0x7F, 0xFF, 0xFF, 0xFF, // word 5
            0x7F, 0xFF, 0xFF, 0xFF, // word 6
            0x7F, 0xFF, 0xFF, 0xFF, // word 7
            0x7F, 0xFF, 0xFF, 0xFF, // word 8
            0x7F, 0xFF, 0xFF, 0xFF, // word 9
            0x7F, 0xFF, 0xFF, 0xFF, // word 10
            0x7F, 0xFF, 0xFF, 0xFF  // word 11
        });
    }
}