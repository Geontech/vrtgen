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

#include <catch2/catch_test_macros.hpp>
#include "bytes.hpp"

#include "vrtgen/packing/enums.hpp"
#include "vrtgen/packing/trailer.hpp"

using namespace vrtgen::packing;

TEST_CASE("Trailer 5.1.6", "[trailer]") 
{
    Trailer trailer;
    Trailer unpack_trailer;
    bytes packed_bytes{ 
        0xFF, 0xFF, 0xFF, 0xFF, // Trailer
    };

    SECTION("Zero on construction")
    {
        trailer.pack_into(packed_bytes.data());
        CHECK(packed_bytes == bytes{ 0, 0, 0, 0 });
    }

    SECTION("Rule 5.1.6-1")
    {
        trailer.calibrated_time(true);
        trailer.sample_loss(true);
        trailer.pack_into(packed_bytes.data());
        // enabled the first and the last to verify the range of the state and event indicators
        CHECK(packed_bytes == bytes{ 0x81, 0x08, 0x10, 0x00 });
    }

    SECTION("Rule 5.1.6-2")
    {
        SECTION("Calibrated Time")
        {
            // Verify zero on construction
            CHECK_FALSE(trailer.calibrated_time().has_value());
            // Setter
            trailer.calibrated_time(true);
            // Getter check set value
            CHECK(trailer.calibrated_time().has_value());
            CHECK(trailer.calibrated_time().value() == true);
            // Pack
            trailer.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0x80, 0x08, 0, 0 });
            // Unpack
            unpack_trailer.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_trailer.calibrated_time().has_value());
            CHECK(unpack_trailer.calibrated_time() == true);
        }

        SECTION("Valid Data")
        {
            // Verify zero on construction
            CHECK_FALSE(trailer.valid_data().has_value());
            // Setter
            trailer.valid_data(true);
            // Getter check set value
            CHECK(trailer.valid_data().has_value());
            CHECK(trailer.valid_data().value() == true);
            // Pack
            trailer.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0x40, 0x04, 0, 0 });
            // Unpack
            unpack_trailer.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_trailer.valid_data().has_value());
            CHECK(unpack_trailer.valid_data().value() == true);
        }

        SECTION("Reference Lock")
        {
            // Verify zero on construction
            CHECK_FALSE(trailer.reference_lock().has_value());
            // Setter
            trailer.reference_lock(true);
            // Getter check set value
            CHECK(trailer.reference_lock().has_value());
            CHECK(trailer.reference_lock().value() == true);
            // Pack
            trailer.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0x20, 0x02, 0, 0 });
            // Unpack
            unpack_trailer.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_trailer.reference_lock().has_value());
            CHECK(unpack_trailer.reference_lock() == true);
        }

        SECTION("AGC/MGC")
        {
            // Verify zero on construction
            CHECK_FALSE(trailer.agc_mgc().has_value());
            // Setter
            trailer.agc_mgc(true);
            // Getter check set value
            CHECK(trailer.agc_mgc().has_value());
            CHECK(trailer.agc_mgc().value() == true);
            // Pack
            trailer.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0x10, 0x01, 0, 0 });
            // Unpack
            unpack_trailer.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_trailer.agc_mgc().has_value());
            CHECK(unpack_trailer.agc_mgc().value() == true);
        }

        SECTION("Detected Signal")
        {
            // Verify zero on construction
            CHECK_FALSE(trailer.detected_signal().has_value());
            // Setter
            trailer.detected_signal(true);
            // Getter check set value
            CHECK(trailer.detected_signal().has_value());
            CHECK(trailer.detected_signal().value() == true);
            // Pack
            trailer.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0x08, 0, 0x80, 0 });
            // Unpack
            unpack_trailer.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_trailer.detected_signal().has_value());
            CHECK(unpack_trailer.detected_signal().value() == true);
        }

        SECTION("Spectral Inversion")
        {
            // Verify zero on construction
            CHECK_FALSE(trailer.spectral_inversion().has_value());
            // Setter
            trailer.spectral_inversion(true);
            // Getter check set value
            CHECK(trailer.spectral_inversion().has_value());
            CHECK(trailer.spectral_inversion().value() == true);
            // Pack
            trailer.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0x04, 0, 0x40, 0 });
            // Unpack
            unpack_trailer.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_trailer.spectral_inversion().has_value());
            CHECK(unpack_trailer.spectral_inversion().value() == true);
        }

        SECTION("Over-Range")
        {
            // Verify zero on construction
            CHECK_FALSE(trailer.over_range().has_value());
            // Setter
            trailer.over_range(true);
            // Getter check set value
            CHECK(trailer.over_range().has_value());
            CHECK(trailer.over_range().value() == true);
            // Pack
            trailer.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0x2, 0, 0x20, 0 });
            // Unpack
            unpack_trailer.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_trailer.over_range().has_value());
            CHECK(unpack_trailer.over_range().value() == true);
        }

        SECTION("Sample Loss")
        {
            // Verify zero on construction
            CHECK_FALSE(trailer.sample_loss().has_value());
            // Setter
            trailer.sample_loss(true);
            // Getter check set value
            CHECK(trailer.sample_loss().has_value());
            CHECK(trailer.sample_loss().value() == true);
            // Pack
            trailer.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0x01, 0, 0x10, 0 });
            // Unpack
            unpack_trailer.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_trailer.sample_loss().has_value());
            CHECK(unpack_trailer.sample_loss().value() == true);
        }
    }

    SECTION("Rule 5.1.6-14")
    {
        trailer.associated_context_packets_count(0x7F);
        trailer.pack_into(packed_bytes.data());
        // enabled the first and the last to verify the range of the state and event indicators
        CHECK(packed_bytes == bytes{ 0x00, 0x00, 0x00, 0xFF });
    }
}

TEST_CASE("Sample Frames 5.1.6.1-1", "[trailer][sample_frames]") {
    Trailer trailer;
    Trailer unpack_trailer;
    bytes packed_bytes{ 
        0xFF, 0xFF, 0xFF, 0xFF, // Trailer
    };
    
    trailer.sample_frame(SSI(3));
    trailer.pack_into(packed_bytes.data());
    // enabled the first and the last to verify the range of the state and event indicators
    CHECK(packed_bytes == bytes{ 0x00, 0xC0, 0x0C, 0x00 });

    unpack_trailer.unpack_from(packed_bytes.data());
    CHECK(unpack_trailer.sample_frame() == SSI(3));
}