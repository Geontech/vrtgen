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

#include "vrtgen/packing/cif1.hpp"

using namespace vrtgen::packing;

TEST_CASE("CIF1", "[cif1]")
{
    CIF1 cif1;
    CIF1 unpack_cif1;
    bytes packed_bytes{ 0xFF, 0xFF, 0xFF, 0xFF };

    SECTION("Zero on construction")
    {
        cif1.pack_into(packed_bytes.data());
        CHECK(packed_bytes == bytes{ 0, 0, 0, 0 });
    }

    SECTION("Phase Offset")
    {
        // Verify zero on construction
        CHECK(cif1.phase_offset() == false);
        // Setter
        cif1.phase_offset(true);
        // Getter check set value
        CHECK(cif1.phase_offset() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x80);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.phase_offset() == true);
    }

    SECTION("Polarization")
    {
        // Verify zero on construction
        CHECK(cif1.polarization() == false);
        // Setter
        cif1.polarization(true);
        // Getter check set value
        CHECK(cif1.polarization() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x40);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.polarization() == true);
    }

    SECTION("3-D Pointing Vector")
    {
        // Verify zero on construction
        CHECK(cif1.pointing_vector() == false);
        // Setter
        cif1.pointing_vector(true);
        // Getter check set value
        CHECK(cif1.pointing_vector() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x20);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.pointing_vector() == true);
    }

    SECTION("3-D Pointing Vector Structure")
    {
        // Verify zero on construction
        CHECK(cif1.pointing_vector_structure() == false);
        // Setter
        cif1.pointing_vector_structure(true);
        // Getter check set value
        CHECK(cif1.pointing_vector_structure() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x10);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.pointing_vector_structure() == true);
    }

    SECTION("Spatial Scan Type")
    {
        // Verify zero on construction
        CHECK(cif1.spatial_scan_type() == false);
        // Setter
        cif1.spatial_scan_type(true);
        // Getter check set value
        CHECK(cif1.spatial_scan_type() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x08);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.spatial_scan_type() == true);
    }

    SECTION("Spatial Reference Type")
    {
        // Verify zero on construction
        CHECK(cif1.spatial_reference_type() == false);
        // Setter
        cif1.spatial_reference_type(true);
        // Getter check set value
        CHECK(cif1.spatial_reference_type() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x04);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.spatial_reference_type() == true);
    }

    SECTION("Beam Width")
    {
        // Verify zero on construction
        CHECK(cif1.beam_width() == false);
        // Setter
        cif1.beam_width(true);
        // Getter check set value
        CHECK(cif1.beam_width() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x02);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.beam_width() == true);
    }

    SECTION("Range")
    {
        // Verify zero on construction
        CHECK(cif1.range() == false);
        // Setter
        cif1.range(true);
        // Getter check set value
        CHECK(cif1.range() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x01);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.range() == true);
    }

    SECTION("Eb/No BER")
    {
        // Verify zero on construction
        CHECK(cif1.ebno_ber() == false);
        // Setter
        cif1.ebno_ber(true);
        // Getter check set value
        CHECK(cif1.ebno_ber() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x10);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.ebno_ber() == true);
    }

    SECTION("Threshold")
    {
        // Verify zero on construction
        CHECK(cif1.threshold() == false);
        // Setter
        cif1.threshold(true);
        // Getter check set value
        CHECK(cif1.threshold() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x08);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.threshold() == true);
    }

    SECTION("Compression Point")
    {
        // Verify zero on construction
        CHECK(cif1.compression_point() == false);
        // Setter
        cif1.compression_point(true);
        // Getter check set value
        CHECK(cif1.compression_point() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x04);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.compression_point() == true);
    }

    SECTION("2nd and 3rd Order Intercept Points")
    {
        // Verify zero on construction
        CHECK(cif1.intercept_points() == false);
        // Setter
        cif1.intercept_points(true);
        // Getter check set value
        CHECK(cif1.intercept_points() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x02);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.intercept_points() == true);
    }

    SECTION("SNR/Noise Figure")
    {
        // Verify zero on construction
        CHECK(cif1.snr_noise_figure() == false);
        // Setter
        cif1.snr_noise_figure(true);
        // Getter check set value
        CHECK(cif1.snr_noise_figure() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x01);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.snr_noise_figure() == true);
    }

    SECTION("Aux Frequency")
    {
        // Verify zero on construction
        CHECK(cif1.aux_frequency() == false);
        // Setter
        cif1.aux_frequency(true);
        // Getter check set value
        CHECK(cif1.aux_frequency() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x80);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.aux_frequency() == true);
    }

    SECTION("Aux Gain")
    {
        // Verify zero on construction
        CHECK(cif1.aux_gain() == false);
        // Setter
        cif1.aux_gain(true);
        // Getter check set value
        CHECK(cif1.aux_gain() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x40);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.aux_gain() == true);
    }

    SECTION("Aux Bandwidth")
    {
        // Verify zero on construction
        CHECK(cif1.aux_bandwidth() == false);
        // Setter
        cif1.aux_bandwidth(true);
        // Getter check set value
        CHECK(cif1.aux_bandwidth() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x20);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.aux_bandwidth() == true);
    }

    SECTION("Array of CIFs")
    {
        // Verify zero on construction
        CHECK(cif1.array_of_cifs() == false);
        // Setter
        cif1.array_of_cifs(true);
        // Getter check set value
        CHECK(cif1.array_of_cifs() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x08);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.array_of_cifs() == true);
    }

    SECTION("Spectrum")
    {
        // Verify zero on construction
        CHECK(cif1.spectrum() == false);
        // Setter
        cif1.spectrum(true);
        // Getter check set value
        CHECK(cif1.spectrum() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x04);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.spectrum() == true);
    }

    SECTION("Sector Step/Scan")
    {
        // Verify zero on construction
        CHECK(cif1.sector_step_scan() == false);
        // Setter
        cif1.sector_step_scan(true);
        // Getter check set value
        CHECK(cif1.sector_step_scan() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x02);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.sector_step_scan() == true);
    }

    SECTION("Index List")
    {
        // Verify zero on construction
        CHECK(cif1.index_list() == false);
        // Setter
        cif1.index_list(true);
        // Getter check set value
        CHECK(cif1.index_list() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[3] == 0x80);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.index_list() == true);
    }

    SECTION("Discrete I/O (32 bit)")
    {
        // Verify zero on construction
        CHECK(cif1.discrete_io_32() == false);
        // Setter
        cif1.discrete_io_32(true);
        // Getter check set value
        CHECK(cif1.discrete_io_32() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[3] == 0x40);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.discrete_io_32() == true);
    }

    SECTION("Discrete I/O (64 bit)")
    {
        // Verify zero on construction
        CHECK(cif1.discrete_io_64() == false);
        // Setter
        cif1.discrete_io_64(true);
        // Getter check set value
        CHECK(cif1.discrete_io_64() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[3] == 0x20);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.discrete_io_64() == true);
    }

    SECTION("Health Status")
    {
        // Verify zero on construction
        CHECK(cif1.health_status() == false);
        // Setter
        cif1.health_status(true);
        // Getter check set value
        CHECK(cif1.health_status() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[3] == 0x10);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.health_status() == true);
    }

    SECTION("V49 Spec Compliance")
    {
        // Verify zero on construction
        CHECK(cif1.v49_spec_compliance() == false);
        // Setter
        cif1.v49_spec_compliance(true);
        // Getter check set value
        CHECK(cif1.v49_spec_compliance() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[3] == 0x08);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.v49_spec_compliance() == true);
    }

    SECTION("Version and Build Code")
    {
        // Verify zero on construction
        CHECK(cif1.version_build_code() == false);
        // Setter
        cif1.version_build_code(true);
        // Getter check set value
        CHECK(cif1.version_build_code() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[3] == 0x04);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.version_build_code() == true);
    }

    SECTION("Buffer Size")
    {
        // Verify zero on construction
        CHECK(cif1.buffer_size() == false);
        // Setter
        cif1.buffer_size(true);
        // Getter check set value
        CHECK(cif1.buffer_size() == true);
        // Pack
        cif1.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[3] == 0x02);
        // Unpack
        unpack_cif1.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cif1.buffer_size() == true);
    }
}

TEST_CASE("Polarization (9.4.8)", "[cif1][polarization]")
{
    Polarization polarization;
    Polarization unpack_polarization;
    constexpr std::size_t NUM_BYTES{ 4 };
    auto packed_bytes = bytes(NUM_BYTES, 0xFF);

    SECTION("Zero on construction")
    {
        polarization.pack_into(packed_bytes.data());
        CHECK(packed_bytes == bytes{ 0, 0, 0, 0 });
    }

    SECTION("Rule 9.4.8-1")
    {
        // See Figure 9.4.8-2
        // Verify size
        CHECK(polarization.size() == NUM_BYTES);
        // Setters
        polarization.tilt_angle(1.0);
        polarization.ellipticity_angle(1.0);
        // Getters check set value
        CHECK(polarization.tilt_angle() == 1.0);
        CHECK(polarization.ellipticity_angle() == 1.0);
        // Pack
        polarization.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 0x20, 0, 0x20, 0 });
        // Unpack
        unpack_polarization.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_polarization.tilt_angle() == 1.0);
        CHECK(unpack_polarization.ellipticity_angle() == 1.0);
    }

    SECTION("Rule 9.4.8-2")
    {
        SECTION("Angle Value 1")
        {
            // Setter
            polarization.tilt_angle(1.0);
            // Getter check set value
            CHECK(polarization.tilt_angle() == 1.0);
            // Pack
            polarization.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0x20, 0, 0, 0 });
            // Unpack
            unpack_polarization.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_polarization.tilt_angle() == 1.0);
        }
        SECTION("Angle Value 2")
        {
            // Setter
            polarization.tilt_angle(-1.0);
            // Getter check set value
            CHECK(polarization.tilt_angle() == -1.0);
            // Pack
            polarization.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0xE0, 0, 0, 0 });
            // Unpack
            unpack_polarization.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_polarization.tilt_angle() == -1.0);
        }
        SECTION("Angle Value 3")
        {
            auto value = 1./(1<<13); // radix=13
            // Setter
            polarization.tilt_angle(value);
            // Getter check set value
            CHECK(polarization.tilt_angle() == value);
            // Pack
            polarization.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0, 0x01, 0, 0 });
            // Unpack
            unpack_polarization.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_polarization.tilt_angle() == value);
        }
        SECTION("Angle Value 4")
        {
            auto value = -1./(1<<13); // radix=13
            // Setter
            polarization.tilt_angle(value);
            // Getter check set value
            CHECK(polarization.tilt_angle() == value);
            // Pack
            polarization.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0xFF, 0xFF, 0, 0 });
            // Unpack
            unpack_polarization.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_polarization.tilt_angle() == value);
        }
    }

    SECTION("Rule 9.4.8-3")
    {
        SECTION("Angle Value 1")
        {
            // Setter
            polarization.ellipticity_angle(1.0);
            // Getter check set value
            CHECK(polarization.ellipticity_angle() == 1.0);
            // Pack
            polarization.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0, 0, 0x20, 0 });
            // Unpack
            unpack_polarization.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_polarization.ellipticity_angle() == 1.0);
        }
        SECTION ("Angle Value 2")
        {
            // Setter
            polarization.ellipticity_angle(-1.0);
            // Getter check set value
            CHECK(polarization.ellipticity_angle() == -1.0);
            // Pack
            polarization.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0, 0, 0xE0, 0 });
            // Unpack
            unpack_polarization.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_polarization.ellipticity_angle() == -1.0);
        }
        SECTION("Angle Value 3")
        {
            auto value = 1./(1<<13); // radix=13
            // Setter
            polarization.ellipticity_angle(value);
            // Getter check set value
            CHECK(polarization.ellipticity_angle() == value);
            // Pack
            polarization.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0, 0, 0, 0x01 });
            // Unpack
            unpack_polarization.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_polarization.ellipticity_angle() == value);
        }
        SECTION("Angle Value 4")
        {
            auto value = -1./(1<<13); // radix=13
            // Setter
            polarization.ellipticity_angle(value);
            // Getter check set value
            CHECK(polarization.ellipticity_angle() == value);
            // Pack
            polarization.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0, 0, 0xFF, 0xFF });
            // Unpack
            unpack_polarization.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_polarization.ellipticity_angle() == value);
        }
    }
}

TEST_CASE("3-D Pointing Vector (9.4.1.1)", "[cif1][pointing_vector]")
{
    PointingVector pv;
    PointingVector unpack_pv;
    constexpr std::size_t NUM_BYTES{ 4 };
    auto packed_bytes = bytes(NUM_BYTES, 0xFF);

    SECTION("Zero on construction")
    {
        pv.pack_into(packed_bytes.data());
        CHECK(packed_bytes == bytes{ 0, 0, 0, 0 });
    }

    SECTION("Rule 9.4.1.1-1")
    {
        // Developer's note: Rule does not apply to tool
        CHECK(true);
    }

    SECTION("Rule 9.4.1.1-2")
    {
        SECTION("Angle Value 1")
        {
            // Setter
            pv.azimuthal_angle(1.0);
            // Getter check set value
            CHECK(pv.azimuthal_angle() == 1.0);
            // Pack
            pv.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0, 0, 0, 0x80 });
            // Unpack
            unpack_pv.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_pv.azimuthal_angle() == 1.0);
        }
        SECTION("Angle Value 2")
        {
            // Setter
            pv.azimuthal_angle(511.9921875);
            // Getter check set value
            CHECK(pv.azimuthal_angle() == 511.9921875);
            // Pack
            pv.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0, 0, 0xFF, 0xFF });
            // Unpack
            unpack_pv.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_pv.azimuthal_angle() == 511.9921875);
        }
    }

    SECTION("Rule 9.4.1.1-3")
    {
        SECTION("Angle Value 1")
        {
            // Setter
            pv.elevation_angle(1.0);
            // Getter check set value
            CHECK(pv.elevation_angle() == 1.0);
            // Pack
            pv.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0, 0x80, 0, 0 });
            // Unpack
            unpack_pv.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_pv.elevation_angle() == 1.0);
        }
        SECTION("Angle Value 2")
        {
            // Setter
            pv.elevation_angle(-1.0);
            // Getter check set value
            CHECK(pv.elevation_angle() == -1.0);
            // Pack
            pv.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0xFF, 0x80, 0, 0 });
            // Unpack
            unpack_pv.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_pv.elevation_angle() == -1.0);
        }
        SECTION("Angle Value 3")
        {
            // Setter
            pv.elevation_angle(0.0078125);
            // Getter check set value
            CHECK(pv.elevation_angle() == 0.0078125);
            // Pack
            pv.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0, 0x01, 0, 0 });
            // Unpack
            unpack_pv.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_pv.elevation_angle() == 0.0078125);
        }
        SECTION("Angle Value 4")
        {
            // Setter
            pv.elevation_angle(-0.0078125);
            // Getter check set value
            CHECK(pv.elevation_angle() == -0.0078125);
            // Pack
            pv.pack_into(packed_bytes.data());
            // Verify packed bits
            CHECK(packed_bytes == bytes{ 0xFF, 0xFF, 0, 0 });
            // Unpack
            unpack_pv.unpack_from(packed_bytes.data());
            // Verify unpacked value
            CHECK(unpack_pv.elevation_angle() == -0.0078125);
        }
    }
}

TEST_CASE("Beam Width (9.4.2)", "[cif1][beam_width]")
{
    BeamWidth beam_width;
    BeamWidth unpack_beam_width;
    constexpr std::size_t NUM_BYTES{ 4 };
    auto packed_bytes = bytes(NUM_BYTES, 0xFF);

    SECTION("Zero on construction")
    {
        beam_width.pack_into(packed_bytes.data());
        CHECK(packed_bytes == bytes{ 0, 0, 0, 0 });
    }

    SECTION("Rule 9.4.2-1")
    {
        // See Figure 9.4.2-2
        // Verify size
        CHECK(beam_width.size() == NUM_BYTES);
        // Setters
        beam_width.horizontal(1.0);
        beam_width.vertical(1.0);
        // Getters check set value
        CHECK(beam_width.horizontal() == 1.0);
        CHECK(beam_width.vertical() == 1.0);
        // Pack
        beam_width.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes == bytes{ 0, 0x80, 0, 0x80 });
        // Unpack
        unpack_beam_width.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_beam_width.horizontal() == 1.0);
        CHECK(unpack_beam_width.vertical() == 1.0);
    }

    SECTION("Rule 9.4.2-2")
    {
        /* TODO
        SECTION("Horizontal Below Min")
        {
            CHECK_THROWS_AS(beam_width.horizontal(-1), std::invalid_argument);
        }
        SECTION("Horizontal Above Max")
        {
            CHECK_THROWS_AS(beam_width.horizontal(361), std::invalid_argument);
        }
        SECTION("Vertical Below Min")
        {
            CHECK_THROWS_AS(beam_width.vertical(-1), std::invalid_argument);
        }
        SECTION("Vertical Above Max")
        {
            CHECK_THROWS_AS(beam_width.vertical(361), std::invalid_argument);
        }
        */
    }
}