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

#include "catch.hpp"
#include "bytes.hpp"
#include <vrtgen/packing/enums.hpp>

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
        CHECK(packed_bytes == bytes{ 0x00, 0x08, 0x10, 0x00 });
    }

    SECTION("Rule 5.1.6-2")
    {

        SECTION("Calibrated Time") {
            CHECK(trailer.calibrated_time() == false);
            CHECK(trailer.calibrated_time_enable() == false);
            trailer.calibrated_time(true);
            trailer.calibrated_time_enable(true);
            trailer.pack_into(packed_bytes.data());
            // enabled the first and the last to verify the range of the state and event indicators
            CHECK(packed_bytes == bytes{ 0x80, 0x08, 0x00, 0x00 });

            unpack_trailer.unpack_from(packed_bytes.data());
            CHECK(unpack_trailer.calibrated_time() == true);
            CHECK(unpack_trailer.calibrated_time_enable() == true);
        }
        
        SECTION("Valid Data")
        {
            CHECK(trailer.valid_data() == false);
            CHECK(trailer.valid_data_enable() == false);
            trailer.valid_data(true);
            trailer.valid_data_enable(true);
            trailer.pack_into(packed_bytes.data());
            // enabled the first and the last to verify the range of the state and event indicators
            CHECK(packed_bytes == bytes{ 0x40, 0x04, 0x00, 0x00 });

            unpack_trailer.unpack_from(packed_bytes.data());
            CHECK(unpack_trailer.valid_data() == true);
            CHECK(unpack_trailer.valid_data_enable() == true);
        }

        SECTION("Reference Lock")
        {
            CHECK(trailer.reference_lock() == false);
            CHECK(trailer.reference_lock_enable() == false);
            trailer.reference_lock(true);
            trailer.reference_lock_enable(true);
            trailer.pack_into(packed_bytes.data());
            // enabled the first and the last to verify the range of the state and event indicators
            CHECK(packed_bytes == bytes{ 0x20, 0x02, 0x00, 0x00 });

            unpack_trailer.unpack_from(packed_bytes.data());
            CHECK(unpack_trailer.reference_lock() == true);
            CHECK(unpack_trailer.reference_lock_enable() == true);
        }

        SECTION("AGC/MGC")
        {
            CHECK(trailer.agc_mgc() == false);
            CHECK(trailer.agc_mgc_enable() == false);
            trailer.agc_mgc(true);
            trailer.agc_mgc_enable(true);
            trailer.pack_into(packed_bytes.data());
            // enabled the first and the last to verify the range of the state and event indicators
            CHECK(packed_bytes == bytes{ 0x10, 0x01, 0x00, 0x00 });

            unpack_trailer.unpack_from(packed_bytes.data());
            CHECK(unpack_trailer.agc_mgc() == true);
            CHECK(unpack_trailer.agc_mgc_enable() == true);
        }

        SECTION("Detected Signal")
        {
            CHECK(trailer.detected_signal() == false);
            CHECK(trailer.detected_signal_enable() == false);
            trailer.detected_signal(true);
            trailer.detected_signal_enable(true);
            trailer.pack_into(packed_bytes.data());
            // enabled the first and the last to verify the range of the state and event indicators
            CHECK(packed_bytes == bytes{ 0x08, 0x00, 0x80, 0x00 });

            unpack_trailer.unpack_from(packed_bytes.data());
            CHECK(unpack_trailer.detected_signal() == true);
            CHECK(unpack_trailer.detected_signal_enable() == true);
        }

        SECTION("Spectral Inversion")
        {
            CHECK(trailer.spectral_inversion() == false);
            CHECK(trailer.spectral_inversion_enable() == false);
            trailer.spectral_inversion(true);
            trailer.spectral_inversion_enable(true);
            trailer.pack_into(packed_bytes.data());
            // enabled the first and the last to verify the range of the state and event indicators
            CHECK(packed_bytes == bytes{ 0x04, 0x00, 0x40, 0x00 });

            unpack_trailer.unpack_from(packed_bytes.data());
            CHECK(unpack_trailer.spectral_inversion() == true);
            CHECK(unpack_trailer.spectral_inversion_enable() == true);
        }

        SECTION("Over-range")
        {
            CHECK(trailer.over_range() == false);
            CHECK(trailer.over_range_enable() == false);
            trailer.over_range(true);
            trailer.over_range_enable(true);
            trailer.pack_into(packed_bytes.data());
            // enabled the first and the last to verify the range of the state and event indicators
            CHECK(packed_bytes == bytes{ 0x02, 0x00, 0x20, 0x00 });

            unpack_trailer.unpack_from(packed_bytes.data());
            CHECK(unpack_trailer.over_range() == true);
            CHECK(unpack_trailer.over_range_enable() == true);
        }

        SECTION("Sample Loss")
        {
            CHECK(trailer.sample_loss() == false);
            CHECK(trailer.sample_loss_enable() == false);
            trailer.sample_loss(true);
            trailer.sample_loss_enable(true);
            CHECK(trailer.sample_loss() == true);
            CHECK(trailer.sample_loss_enable() == true);
            trailer.pack_into(packed_bytes.data());
            // enabled the first and the last to verify the range of the state and event indicators
            CHECK(packed_bytes == bytes{ 0x01, 0x00, 0x10, 0x00 });

            unpack_trailer.unpack_from(packed_bytes.data());
            CHECK(unpack_trailer.sample_loss() == true);
            CHECK(unpack_trailer.sample_loss_enable() == true);
        }
    }

    SECTION("Rule 5.1.6-14")
    {
        trailer.associated_context_packet_count_enable(true);
        trailer.associated_context_packet_count(0x7F);
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
    trailer.sample_frame_enable(true);
    trailer.pack_into(packed_bytes.data());
    // enabled the first and the last to verify the range of the state and event indicators
    CHECK(packed_bytes == bytes{ 0x00, 0xC0, 0x0C, 0x00 });

    unpack_trailer.unpack_from(packed_bytes.data());
    CHECK(unpack_trailer.sample_frame() == SSI(3));
    CHECK(unpack_trailer.sample_frame_enable() == true);
}