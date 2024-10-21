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

#pragma once

#include <vrtgen/types.hpp>
#include <vrtgen/packing/enums.hpp>
#include <vrtgen/packing/indicator_fields.hpp>
#include <vector>
#include <type_traits>

namespace vrtgen::packing
{

/**
 * @class CIF1
 * @brief Context/Command Indicator Field 1 (VITA 49.2-2017 Section 9.1)
 */
class CIF1 : public IndicatorField1 {};

/**
 * @class Polarization
 * @brief Antenna polarization (VITA 49.2-2017 Section 9.4.8)
 */
class Polarization : public Packed32_Fixed16R<13>
{ 
public:
    /**
     * @brief Returns the Tilt Angle subfield value
     * @return Tilt Angle subfield value
     * 
     * Tilt Angle subfield is 16 bits long at bit position 31 expressed in
     * two's-complement format with a radix point to the right of bit 13
     * See VITA 49.2-2017 Figure 9.4.8-2
     */
    double tilt_angle() const noexcept
    {
        return subfield_31();
    }

    /**
     * @brief Sets the Tilt Angle subfield value
     * @param value Tilt Angle subfield value to set
     * 
     * Tilt Angle subfield is 16 bits long at bit position 31 expressed in
     * two's-complement format with a radix point to the right of bit 13
     * See VITA 49.2-2017 Figure 9.4.8-2
     */
    void tilt_angle(double value) noexcept
    {
        subfield_31(value);
    }

    /**
     * @brief Returns the Ellipticity Angle subfield value
     * @return Ellipticity Angle subfield value
     * 
     * Ellipticity Angle subfield is 16 bits long at bit position 15 expressed in
     * two's-complement format with a radix point to the right of bit 13
     * See VITA 49.2-2017 Figure 9.4.8-2
     */
    double ellipticity_angle() const noexcept
    {
        return subfield_15();
    }

    /**
     * @brief Sets the Ellipticity Angle subfield value
     * @param value Ellipticity Angle subfield value to set
     * 
     * Ellipticity Angle subfield is 16 bits long at bit position 15 expressed in
     * two's-complement format with a radix point to the right of bit 13
     * See VITA 49.2-2017 Figure 9.4.8-2
     */
    void ellipticity_angle(double value) noexcept
    {
        subfield_15(value);
    }

}; // end class Polarization

/**
 * @class PointingVector
 * @brief 3-D Pointing Vector (VITA 49.2-2017 Section 9.4.1.1)
 */
class PointingVector : public Packed32_Fixed16R<7>
{
public:
    /**
     * @brief Returns the Elevation Angle subfield value
     * @return Elevation Angle subfield value
     * 
     * Elevation Angle subfield is 16 bits long at bit position 31 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.4.1.1-1
     */
    double elevation_angle() const noexcept
    {
        return subfield_31();
    }

    /**
     * @brief Sets the Elevation Angle subfield value
     * @param value Elevation Angle subfield value to set
     * 
     * Elevation Angle subfield is 16 bits long at bit position 31 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.4.1.1-1
     */
    void elevation_angle(double value) noexcept
    {
        subfield_31(value);
    }

    /**
     * @brief Returns the Azimuthal Angle subfield value
     * @return Azimuthal Angle subfield value
     * 
     * Azimuthal Angle subfield is 16 bits long at bit position 15 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.4.1.1-1
     */
    double azimuthal_angle() const noexcept
    {
        return vrtgen::fixed::to_fp<16,7>(m_packed.get<15,16,uint16_t>());
    }

    /**
     * @brief Sets the Azimuthal Angle subfield value
     * @param value Azimuthal Angle subfield value to set
     * 
     * Azimuthal Angle subfield is 16 bits long at bit position 15 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.4.1.1-1
     */
    void azimuthal_angle(double value) noexcept
    {
        subfield_15(value);
    }

}; // end class PointingVector

/**
 * @class BeamWidth
 * @brief Beam Width (VITA 49.2-2017 Section 9.4.2)
 */
class BeamWidth : public Packed32_Fixed16R<7>
{
public:
    /**
     * @brief Returns the Horizontal Beam Width subfield value
     * @return Horizontal Beam Width subfield value
     * 
     * Horizontal Beam Width subfield is 16 bits long at bit position 31 expressed
     * in two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.4.2-2
     */
    double horizontal() const noexcept
    {
        return subfield_31();
    }

    /**
     * @brief Sets the Horizontal Beam Width subfield value
     * @param value Horizontal Beam Width subfield value to set
     * @throw invalid_argument value is outside of the field's valid range
     * 
     * Horizontal Beam Width subfield is 16 bits long at bit position 31 expressed
     * in two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.4.2-2
     */
    void horizontal(double value)
    {
        subfield_31(value);
    }

    /**
     * @brief Returns the Vertical Beam Width subfield value
     * @return Vertical Beam Width subfield value
     * 
     * Vertical Beam Width subfield is 16 bits long at bit position 15 expressed
     * in two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.4.2-2
     */
    double vertical() const noexcept
    {
        return subfield_15();
    }

    /**
     * @brief Sets the Vertical Beam Width subfield value
     * @param value Vertical Beam Width subfield value to set
     * 
     * Vertical Beam Width subfield is 16 bits long at bit position 15 expressed
     * in two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.4.2-2
     */
    void vertical(double value)
    {
        subfield_15(value);
    }

}; // end class BeamWidth

/**
 * @class EbNoBER
 * @brief Energy per Bit to Noise Density Ratio (Eb/No) and Bit Error Rate
 *        (VITA 49.2-2017 Section 9.5.17)
 */
class EbNoBER : public Packed32_Fixed16R<7>
{
public:
    /**
     * @brief Default constructor
     * 
     * Sets the default values for EbNo and BER per rules 9.5.17-4 and 9.5.17-8 respectively
     */
    EbNoBER()
    {
        ebno(0x7FFF);
        ber(0x7FFF);
    }

    /**
     * @brief Returns the EbNo subfield value
     * @return EbNo subfield value
     * 
     * EbNo subfield is 16 bits long at bit position 31 expressed
     * in two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.17-1
     */
    double ebno() const noexcept
    {
        return subfield_31();
    }

    /**
     * @brief Sets the EbNo subfield value
     * @param value EbNo subfield value to set
     * @throw invalid_argument value is outside of the field's valid range
     * 
     * EbNo subfield is 16 bits long at bit position 31 expressed
     * in two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.17-1
     */
    void ebno(double value) noexcept
    {
        subfield_31(value);
    }

    /**
     * @brief Returns the BER subfield value
     * @return BER subfield value
     * 
     * BER subfield is 16 bits long at bit position 31 expressed
     * in two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.17-1
     */
    double ber() const noexcept
    {
        return subfield_15();
    }

    /**
     * @brief Sets the BER subfield value
     * @param value BER subfield value to set
     * @throw invalid_argument value is outside of the field's valid range
     * 
     * BER subfield is 16 bits long at bit position 15 expressed
     * in two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.17-1
     */
    void ber(double value) noexcept
    {
        subfield_15(value);
    }

}; // end class EbNoBER

/**
 * @class Threshold
 * @brief Signal threshold level (VITA 49.2-2017 Section 9.5.13)
 */
class Threshold : public Packed32_Fixed16R<7>
{
public:
    /**
     * @brief Returns the Stage 2 Threshold subfield value
     * @return Stage 2 Threshold subfield value
     * 
     * Stage 2 Threshold subfield is 16 bits long at bit position 31 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.3-1
     */
    double stage_2() const noexcept
    {
        return subfield_31();
    }

    /**
     * @brief Sets the Stage 2 Threshold subfield value
     * @param value Stage 2 Threshold subfield value to set
     * 
     * Stage 2 Threshold subfield is 16 bits long at bit position 31 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.3-1
     */
    void stage_2(double value) noexcept
    {
        subfield_31(value);
    }

    /**
     * @brief Returns the Stage 1 Threshold subfield value
     * @return Stage 1 Threshold subfield value
     * 
     * Stage 1 Threshold subfield is 16 bits long at bit position 15 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.3-1
     */
    double stage_1() const noexcept
    {
        return subfield_15();
    }

    /**
     * @brief Sets the Stage 1 Threshold subfield value
     * @param value Stage 1 Threshold subfield value to set
     * 
     * Stage 1 Threshold subfield is 16 bits long at bit position 15 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.3-1
     */
    void stage_1(double value) noexcept
    {
        subfield_15(value);
    }

}; // end class Threshold

/**
 * @class InterceptPoint
 * @brief Second and Third Order Intercept points (VITA 49.2-2017 Section 9.5.6)
 */
class InterceptPoints : public Packed32_Fixed16R<7>
{
public:
    /**
     * @brief Returns the 2IIP subfield value
     * @return 2IIP subfield value
     * 
     * 2IIP subfield is 16 bits long at bit position 31 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.6-2
     */
    double second_order() const noexcept
    {
        return subfield_31();
    }

    /**
     * @brief Sets the 2IIP subfield value
     * @param value 2IIP subfield value to set
     * 
     * 2IIP subfield is 16 bits long at bit position 31 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.6-2
     */
    void second_order(double value) noexcept
    {
        subfield_31(value);
    }

    /**
     * @brief Returns the 3IIP subfield value
     * @return 3IIP subfield value
     * 
     * 3IIP subfield is 16 bits long at bit position 15 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.6-2
     */
    double third_order() const noexcept
    {
        return subfield_15();
    }

    /**
     * @brief Sets the 3IIP subfield value
     * @param value 3IIP subfield value to set
     * 
     * 3IIP subfield is 16 bits long at bit position 15 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.6-2
     */
    void third_order(double value) noexcept
    {
        subfield_15(value);
    }

}; // end class InterceptPoints

/**
 * @class SNRNoise
 * @brief Signal-to-Noise Ratio (SNR) and Noise Figure (VITA 49.2-2017 Section 9.5.7)
 */
class SNRNoise : public Packed32_Fixed16R<7>
{
public:
    /**
     * @brief Default constructor
     * 
     * Sets the default value for SNR per rule 9.5.7-4
     */
    SNRNoise()
    {
        snr(0x7FFF);
    }

    /**
     * @brief Returns the SNR subfield value
     * @return SNR subfield value
     * 
     * SNR subfield is 16 bits long at bit position 31 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.7-1
     */
    double snr() const noexcept
    {
        return subfield_31();
    }

    /**
     * @brief Sets the SNR subfield value
     * @param value SNR subfield value to set
     * 
     * SNR subfield is 16 bits long at bit position 31 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.7-1
     */
    void snr(double value) noexcept
    {
        subfield_31(value);
    }

    /**
     * @brief Returns the Noise subfield value
     * @return Noise subfield value
     * 
     * Noise subfield is 16 bits long at bit position 15 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.7-1
     */
    double noise() const noexcept
    {
        return subfield_15();
    }

    /**
     * @brief Sets the Noise subfield value
     * @param value Noise subfield value to set
     * 
     * Noise subfield is 16 bits long at bit position 15 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017 Figure 9.5.7-1
     */
    void noise(double value) noexcept
    {
        subfield_15(value);
    }

}; // end class SNRNoise

/**
 * @enum SpectrumType
 * @brief Spectrum Type Bit field in the Spectrum Type subfield
 * 
 * Constants for the 8-bit Spectrum Type Bit field in the Spectrum Type subfield
 * Refer to VITA 49.2-2017 Table 9.6.1.1.1-1.
 */
enum class SpectrumType : uint8_t
{
    NONE = 0b00000000, //!< Default "no setting" value
    LOG_POWER = 0b00000001, //!< Log power (dB)
    CARTESIAN = 0b00000010, //!< Cartesian (I,Q)
    POLAR = 0b00000011, //!< Polar (magnitude, phase)
    MAGNITUDE = 0b00000100 //!< Magnitude
    // Remaining values reserved or user-defined

}; // end enum class SpectrumType

/**
 * @enum AveragingType
 * @brief Averaging Type Bit field in the Spectrum Type subfield
 * 
 * Constants for the 8-bit Averaging Type Bit field in the Spectrum Type subfield
 * Refer to VITA 49.2-2017 Table 9.6.1.1.2-1.
 */
enum class AveragingType : uint8_t
{
    NONE = 0b00000000, //!< No averaging
    LINEAR = 0b00000001, //!< Linear averaging
    PEAK_HOLD = 0b00000010, //!< Peak hold
    MIN_HOLD = 0b00000100, //!< Min hold
    EXPONENTIAL = 0b00001000, //!< Exponential averaging
    MEDIAN = 0b00010000, //!< Median averaging
    SMOOTHING = 0b00100000 //!< Smoothing (within the sample frame)
    // Remaining values reserved

}; // end enum class AveragingType

/**
 * @enum WindowTimeDelta
 * @brief Window Time-Delta Interpretation Bit field in the Spectrum Type subfield
 * 
 * Constants for the 8-bit Window Time-Delta Interpretation field in the Spectrum Type subfield
 * Refer to VITA 49.2-2017 Table 9.6.1.1.3-1.
 */
enum class WindowTimeDelta : uint8_t
{
    NONE = 0b0000, //!< Overlap is NOT controlled
    PERCENT = 0b0001, //!< Percent overlap
    SAMPLES = 0b0010, //!< Samples
    TIME = 0b0011 //!< Time
    // Remaining values reserved

}; // end enum class WindowTimeDelta

/**
 * @enum WindowType
 * @brief Window Type subfield
 * 
 * Constants for the 8-bit Window Type Bit subfield
 * Refer to VITA 49.2-2017 Table 9.6.1.2-1.
 */
enum class WindowType : uint8_t
{
    RECTANGLE = 0,
    TRIANGLE = 1,
    HANNING_ALPHA_1 = 2,
    HANNING_ALPHA_2 = 3,
    HANNING_ALPHA_3 = 4,
    HANNING_ALPHA_4 = 5,
    HAMMING = 6,
    RIESZ = 7,
    RIEMANN = 8,
    DE_LA_VALLEPOUSSIN = 9,
    TUKEY_POINT_25 = 10,
    TUKEY_POINT_50 = 11,
    TUKEY_POINT_75 = 12,
    BOHMAN = 13,
    POISSON_2 = 14,
    POISSON_3 = 15,
    POISSON_4 = 16,
    HANNING_POISSON_POINT_50 = 17,
    HANNING_POISSON_1 = 18,
    HANNING_POISSON_2 = 19,
    CAUCHY_3 = 20,
    CAUCHY_4 = 21,
    CAUCHY_5 = 22,
    GAUSSIAN_2_POINT_50 = 23,
    GAUSSIAN_3 = 24,
    GAUSSIAN_3_POINT_50 = 25,
    DOLPH_CHEBYSHEV_2_POINT_50 = 26,
    DOLPH_CHEBYSHEV_3 = 27,
    DOLPH_CHEBYSHEV_3_POINT_50 = 28,
    DOLPH_CHEBYSHEV_4 = 29,
    KAISER_BESSEL_2 = 30,
    KAISER_BESSEL_2_POINT_50 = 31,
    KAISER_BESSEL_3 = 32,
    KAISER_BESSEL_3_POINT_50 = 33,
    BARCILON_TEMES_3 = 34,
    BARCILON_TEMES_3_POINT_50 = 35,
    BARCILON_TEMES_4 = 36,
    EXACT_BLACKMAN = 37,
    BLACKMAN = 38,
    MIN_THREE_SAMPLE_BLACKMAN_HARRIS = 39,
    MIN_FOUR_SAMPLE_BLACKMAN_HARRIS = 40,
    THREE_SAMPLE_BLACKMAN_HARRIS_61DB = 41,
    FOUR_SAMPLE_BLACKMAN_HARRIS_74DB = 42,
    FOUR_SAMPLE_KAISER_BESSEL = 43
    // Remaining values reserved or user-defined

}; // end enum class WindowType

/**
 * @class Spectrum
 * @brief Spectrum field (VITA 49.2-2017 Section 9.6.1)
 */
class Spectrum
{
public:
    SpectrumType spectrum_type() const noexcept
    {
        return SpectrumType{m_spectrum_type.get<7,8,uint8_t>()};
    }

    void spectrum_type(SpectrumType value) noexcept
    {
        m_spectrum_type.set<7,8>(static_cast<uint8_t>(value));
    }

    AveragingType averaging_type() const noexcept
    {
        return AveragingType{m_spectrum_type.get<15,8,uint8_t>()};
    }

    void averaging_type(AveragingType value) noexcept
    {
        m_spectrum_type.set<15,8>(static_cast<uint8_t>(value));
    }

    WindowTimeDelta window_time() const noexcept
    {
        return WindowTimeDelta{m_spectrum_type.get<19,4,uint8_t>()};
    }

    void window_time(WindowTimeDelta value) noexcept
    {
        m_spectrum_type.set<19,4>(static_cast<uint8_t>(value));
    }

    WindowType window_type() const noexcept
    {
        return WindowType{m_window_type.get<7,8,uint8_t>()};
    }

    void window_type(WindowType value) noexcept
    {
        m_window_type.set<7,8>(static_cast<uint8_t>(value));
    }

    uint32_t num_transform_points() const noexcept
    {
        return vrtgen::swap::from_be(m_num_transform_points);
    }

    void num_transform_points(uint32_t value) noexcept
    {
        m_num_transform_points = vrtgen::swap::to_be(value);
    }

    uint32_t num_window_points() const noexcept
    {
        return vrtgen::swap::from_be(m_num_window_points);
    }

    void num_window_points(uint32_t value) noexcept
    {
        m_num_window_points = vrtgen::swap::to_be(value);
    }

    double resolution() const noexcept
    {
        return vrtgen::fixed::to_fp<64,20>(vrtgen::swap::from_be(m_resolution));
    }

    void resolution(double value) noexcept
    {
        m_resolution = vrtgen::swap::to_be(vrtgen::fixed::to_int<64,20>(value));
    }

    double span() const noexcept
    {
        return vrtgen::fixed::to_fp<64,20>(vrtgen::swap::from_be(m_span));
    }

    void span(double value) noexcept
    {
        m_span = vrtgen::swap::to_be(vrtgen::fixed::to_int<64,20>(value));
    }

    uint32_t num_averages() const noexcept
    {
        return vrtgen::swap::from_be(m_num_averages);
    }

    void num_averages(uint32_t value) noexcept
    {
        m_num_averages = vrtgen::swap::to_be(value);
    }

    uint32_t weighting_factor() const noexcept
    {
        return vrtgen::swap::from_be(m_weighting_factor);
    }

    void weighting_factor(uint32_t value) noexcept
    {
        m_weighting_factor = vrtgen::swap::to_be(value);
    }

    int32_t f1_index() const noexcept
    {
        return vrtgen::swap::from_be(m_f1_index);
    }

    void f1_index(int32_t value) noexcept
    {
        m_f1_index = vrtgen::swap::to_be(value);
    }

    int32_t f2_index() const noexcept
    {
        return vrtgen::swap::from_be(m_f2_index);
    }

    void f2_index(int32_t value) noexcept
    {
        m_f2_index = vrtgen::swap::to_be(value);
    }

    uint32_t window_time_delta() const noexcept
    {
        return vrtgen::swap::from_be(m_window_time_delta);
    }

    void window_time_delta(uint32_t value) noexcept
    {
        m_window_time_delta = vrtgen::swap::to_be(value);
    }

    std::size_t size() const
    {
        return m_spectrum_type.size() +
            m_window_type.size() +
            sizeof(m_num_transform_points) +
            sizeof(m_num_window_points) +
            sizeof(m_resolution) +
            sizeof(m_span) +
            sizeof(m_num_averages) +
            sizeof(m_weighting_factor) +
            sizeof(m_f1_index) +
            sizeof(m_f2_index) +
            sizeof(m_window_time_delta);
    }

    void pack_into(uint8_t* buffer_ptr) const
    {
        m_spectrum_type.pack_into(buffer_ptr);
        buffer_ptr += m_spectrum_type.size();
        m_window_type.pack_into(buffer_ptr);
        buffer_ptr += m_window_type.size();
        std::memcpy(buffer_ptr, &m_num_transform_points, sizeof(m_num_transform_points));
        buffer_ptr += sizeof(m_num_transform_points);
        std::memcpy(buffer_ptr, &m_num_window_points, sizeof(m_num_window_points));
        buffer_ptr += sizeof(m_num_window_points);
        std::memcpy(buffer_ptr, &m_resolution, sizeof(m_resolution));
        buffer_ptr += sizeof(m_resolution);
        std::memcpy(buffer_ptr, &m_span, sizeof(m_span));
        buffer_ptr += sizeof(m_span);
        std::memcpy(buffer_ptr, &m_num_averages, sizeof(m_num_averages));
        buffer_ptr += sizeof(m_num_averages);
        std::memcpy(buffer_ptr, &m_weighting_factor, sizeof(m_weighting_factor));
        buffer_ptr += sizeof(m_weighting_factor);
        std::memcpy(buffer_ptr, &m_f1_index, sizeof(m_f1_index));
        buffer_ptr += sizeof(m_f1_index);
        std::memcpy(buffer_ptr, &m_f2_index, sizeof(m_f2_index));
        buffer_ptr += sizeof(m_f2_index);
        std::memcpy(buffer_ptr, &m_window_time_delta, sizeof(m_window_time_delta));
    }

    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        m_spectrum_type.unpack_from(ptr);
        ptr += m_spectrum_type.size();
        m_window_type.unpack_from(ptr);
        ptr += m_window_type.size();
        std::memcpy(&m_num_transform_points, ptr, sizeof(m_num_transform_points));
        ptr += sizeof(m_num_transform_points);
        std::memcpy(&m_num_window_points, ptr, sizeof(m_num_window_points));
        ptr += sizeof(m_num_window_points);
        std::memcpy(&m_resolution, ptr, sizeof(m_resolution));
        ptr += sizeof(m_resolution);
        std::memcpy(&m_span, ptr, sizeof(m_span));
        ptr += sizeof(m_span);
        std::memcpy(&m_num_averages, ptr, sizeof(m_num_averages));
        ptr += sizeof(m_num_averages);
        std::memcpy(&m_weighting_factor, ptr, sizeof(m_weighting_factor));
        ptr += sizeof(m_weighting_factor);
        std::memcpy(&m_f1_index, ptr, sizeof(m_f1_index));
        ptr += sizeof(m_f1_index);
        std::memcpy(&m_f2_index, ptr, sizeof(m_f2_index));
        ptr += sizeof(m_f2_index);
        std::memcpy(&m_window_time_delta, ptr, sizeof(m_window_time_delta));
    }

private:
    vrtgen::packed<uint32_t> m_spectrum_type;
    vrtgen::packed<uint32_t> m_window_type;
    uint32_t m_num_transform_points;
    uint32_t m_num_window_points;
    uint64_t m_resolution;
    uint64_t m_span;
    uint32_t m_num_averages;
    uint32_t m_weighting_factor;
    int32_t m_f1_index;
    int32_t m_f2_index;
    uint32_t m_window_time_delta;

}; // class Spectrum

/**
 * @class SectorStepScanCIF
 * @brief Sector/Step-Scan Control/Context Indicator Word (VITA 49.2-2017 Section 9.6.2.1)
 */
class SectorStepScanCIF : public IndicatorField
{
public:
    bool sector_number() const noexcept
    {
        return m_packed.get<31>();
    }

    void sector_number(bool value) noexcept
    {
        m_packed.set<31>(value);
    }

    bool f1_start_frequency() const noexcept
    {
        return m_packed.get<30>();
    }

    void f1_start_frequency(bool value) noexcept
    {
        m_packed.set<30>(value);
    }

    bool f2_stop_frequency() const noexcept
    {
        return m_packed.get<29>();
    }

    void f2_stop_frequency(bool value) noexcept
    {
        m_packed.set<29>(value);
    }

    bool resolution_bandwidth() const noexcept
    {
        return m_packed.get<28>();
    }

    void resolution_bandwidth(bool value) noexcept
    {
        m_packed.set<28>(value);
    }

    bool tune_step_size() const noexcept
    {
        return m_packed.get<27>();
    }

    void tune_step_size(bool value) noexcept
    {
        m_packed.set<27>(value);
    }

    bool number_points() const noexcept
    {
        return m_packed.get<26>();
    }

    void number_points(bool value) noexcept
    {
        m_packed.set<26>(value);
    }

    bool default_gain() const noexcept
    {
        return m_packed.get<25>();
    }

    void default_gain(bool value) noexcept
    {
        m_packed.set<25>(value);
    }

    bool threshold() const noexcept
    {
        return m_packed.get<24>();
    }

    void threshold(bool value) noexcept
    {
        m_packed.set<24>(value);
    }

    bool dwell_time() const noexcept
    {
        return m_packed.get<23>();
    }

    void dwell_time(bool value) noexcept
    {
        m_packed.set<23>(value);
    }

    bool start_time() const noexcept
    {
        return m_packed.get<22>();
    }

    void start_time(bool value) noexcept
    {
        m_packed.set<22>(value);
    }

    bool time_3() const noexcept
    {
        return m_packed.get<21>();
    }

    void time_3(bool value) noexcept
    {
        m_packed.set<21>(value);
    }

    bool time_4() const noexcept
    {
        return m_packed.get<20>();
    }

    void time_4(bool value) noexcept
    {
        m_packed.set<20>(value);
    }

}; // end class SectorStepScanCIF

/**
 * Sector/Step-Scan Field [9.6.2]
 */
template<typename T> 
class SectorStepScan
{
public:
    uint32_t array_size() const noexcept
    {
        return vrtgen::swap::from_be(m_array_size);
    }

    void array_size(uint32_t value) noexcept
    {
        m_array_size = vrtgen::swap::to_be(value);
    }

    uint8_t header_size() const noexcept
    {
        return m_packed.get<31,8,uint8_t>();
    }

    void header_size(uint8_t value) noexcept
    {
        m_packed.set<31,8>(value);
    }

    uint16_t num_words_record() const noexcept
    {
        return m_packed.get<23,12,uint16_t>();
    }

    void num_words_record(uint16_t value) noexcept
    {
        m_packed.set<23,12>(value);
    }

    uint16_t num_records() const noexcept
    {
        return m_packed.get<11,12,uint16_t>();
    }

    void num_records(uint16_t value) noexcept
    {
        m_packed.set<11,12>(value);
    }

    SectorStepScanCIF& subfield_cif() noexcept
    {
        return m_subfield_cif;
    }

    const SectorStepScanCIF& subfield_cif() const noexcept
    {
        return m_subfield_cif;
    }

    void subfield_cif(const SectorStepScanCIF& value) noexcept
    {
        m_subfield_cif = value;
    }

    std::vector<T>& records() noexcept
    {
        return m_records;
    }

    const std::vector<T>& records() const noexcept
    {
        return m_records;
    }

    void records(const std::vector<T>& value) noexcept
    {
        m_records = value;
    }

    std::size_t size() const
    {
        std::size_t retval = 0;
        retval += sizeof(m_array_size);
        retval += m_packed.size();
        retval += m_subfield_cif.size();
        if (!m_records.empty()) {
            retval += m_records.size() * m_records.front().size();
        }
        return retval;
    }

    void pack_into(uint8_t* buffer_ptr) const
    {
        std::memcpy(buffer_ptr, &m_array_size, sizeof(m_array_size));
        buffer_ptr += sizeof(m_array_size);
        m_packed.pack_into(buffer_ptr);
        buffer_ptr += m_packed.size();
        m_subfield_cif.pack_into(buffer_ptr);
        buffer_ptr += m_subfield_cif.size();
        for (const auto& elem : m_records) {
            elem.pack_into(buffer_ptr);
            buffer_ptr += elem.size();
        }
    }

    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        std::memcpy(&m_array_size, ptr, sizeof(m_array_size));
        ptr += sizeof(m_array_size);
        m_packed.unpack_from(ptr);
        ptr += m_packed.size();
        m_subfield_cif.unpack_from(ptr);
        ptr += m_subfield_cif.size();
        m_records.resize(num_records());
        for (size_t i=0; i<m_records.size(); ++i) {
            m_records[i].unpack_from(ptr);
            ptr += m_records[i].size();
        }
    }

private:
    uint32_t m_array_size{ 0 };
    vrtgen::packed<uint32_t> m_packed;
    SectorStepScanCIF m_subfield_cif;
    std::vector<T> m_records;

}; // end class SectorStepScan

/**
 * Index List Field [9.3.2]
 */
template<typename T>
class IndexList
{
public:
    uint32_t total_size() const noexcept
    {
        return vrtgen::swap::from_be(m_total_size);
    }

    void total_size(uint32_t value) noexcept
    {
        m_total_size = vrtgen::swap::to_be(value);
    }

    EntrySize entry_size() const noexcept
    {
        return EntrySize{ m_packed.get<31,4,uint8_t>() };
    }

    void entry_size(EntrySize value) noexcept
    {
        m_packed.set<31,4>(static_cast<uint8_t>(value));
    }

    uint32_t num_entries() const noexcept
    {
        return m_packed.get<19,20,uint32_t>();
    }

    void num_entries(uint32_t value) noexcept
    {
        m_packed.set<19,20>(value);
    }

    std::vector<T>& entries() noexcept
    {
        return m_entries;
    }

    const std::vector<T>& entries() const noexcept
    {
        return m_entries;
    }

    void entries(const std::vector<T>& value) noexcept
    {
        m_entries = value;
    }

    std::size_t size() const
    {
        std::size_t retval = 0;
        retval += sizeof(m_total_size);
        retval += m_packed.size();
        if (!m_entries.empty()) {
            retval += m_entries.size() * sizeof(T);
        }
        return retval;
    }

    void pack_into(uint8_t* buffer_ptr) const
    {
        std::memcpy(buffer_ptr, &m_total_size, sizeof(m_total_size));
        buffer_ptr += sizeof(m_total_size);
        m_packed.pack_into(buffer_ptr);
        buffer_ptr += m_packed.size();
        for (const auto elem : m_entries) {
            static_assert(std::is_trivially_copyable_v<decltype(elem)>);
            std::memcpy(buffer_ptr, &elem, sizeof(elem));
            buffer_ptr += sizeof(elem);
        }
    }

    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        std::memcpy(&m_total_size, ptr, sizeof(m_total_size));
        ptr += sizeof(m_total_size);
        m_packed.unpack_from(ptr);
        ptr += m_packed.size();
        m_entries.resize(num_entries());
        for (size_t i=0; i<m_entries.size(); ++i) {
            std::memcpy(m_entries.data() + i, ptr, sizeof(m_entries[i]));
            ptr += sizeof(m_entries[i]);
        }
    }

private:
    uint32_t m_total_size{ 0 };
    vrtgen::packed<uint32_t> m_packed;
    std::vector<T> m_entries;

}; // end class IndexList

/**
 * @class VersionInformation
 * @brief VersionInformation field
 */
class VersionInformation
{
public:
    /**
     * @brief Returns the VersionInformation Year
     * @return VersionInformation Year
     * 
     * VersionInformation Year is 7 bits long at bit position 31
     * See VITA 49.2-2017 Table 9.10.4-1
     */
    constexpr uint8_t year() const noexcept
    {
        return m_packed.get<31,7,uint8_t>();
    }

    /**
     * @brief Sets the VersionInformation Year
     * @param value Year value to set
     * 
     * VersionInformation Year is 7 bits long at bit position 31
     * See VITA 49.2-2017 Table 9.10.4-1
     */
    constexpr void year(uint8_t value) noexcept
    {
        m_packed.set<31,7>(value);
    }

    /**
     * @brief Returns the VersionInformation Day
     * @return VersionInformation Day
     * 
     * VersionInformation Day is 9 bits long at bit position 24
     * See VITA 49.2-2017 Table 9.10.4-1
     */
    constexpr uint16_t day() const noexcept
    {
        return m_packed.get<24,9,uint16_t>();
    }

    /**
     * @brief Sets the VersionInformation Day
     * @param value Day value to set
     * 
     * VersionInformation Day is 9 bits long at bit position 24
     * See VITA 49.2-2017 Table 9.10.4-1
     */
    constexpr void day(uint16_t value) noexcept
    {
        m_packed.set<24,9>(value);
    }

    /**
     * @brief Returns the VersionInformation Revision
     * @return VersionInformation Revision
     * 
     * VersionInformation Revision is 6 bits long at bit position 15
     * See VITA 49.2-2017 Table 9.10.4-1
     */
    constexpr uint8_t revision() const noexcept
    {
        return m_packed.get<15,6,uint8_t>();
    }

    /**
     * @brief Sets the VersionInformation Revision
     * @param value Revision value to set
     * 
     * VersionInformation Revision is 6 bits long at bit position 15
     * See VITA 49.2-2017 Table 9.10.4-1
     */
    constexpr void revision(uint8_t value) noexcept
    {
        m_packed.set<15,6>(value);
    }

    /**
     * @brief Returns the VersionInformation User-Defined
     * @return VersionInformation User-Defined
     * 
     * VersionInformation User-Defined is 10 bits long at bit position 9
     * See VITA 49.2-2017 Table 9.10.4-1
     */
    constexpr uint16_t user_defined() const noexcept
    {
        return m_packed.get<9,10,uint16_t>();
    }

    /**
     * @brief Sets the VersionInformation User-Defined
     * @param value User-Defined value to set
     * 
     * VersionInformation User-Defined is 10 bits long at bit position 9
     * See VITA 49.2-2017 Table 9.10.4-1
     */
    constexpr void user_defined(uint16_t value) noexcept
    {
        m_packed.set<9,10>(value);
    }

    /**
     * @brief Returns the number of VersionInformation bytes
     * @return Number of VersionInformation bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return m_packed.size();
    }

    /**
     * @brief Pack VersionInformation as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add VersionInformation bytes
     */
    inline constexpr void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into VersionInformation
     * @param buffer_ptr Pointer to beginning of VersionInformation bytes in the buffer
     */
    inline constexpr void unpack_from(const uint8_t* buffer_ptr)
    {
        m_packed.unpack_from(buffer_ptr);
    }

protected:
    vrtgen::packed<uint32_t> m_packed; //!< Packed VersionInformation bits structure

}; // end class VersionInformation

} // end namespace vrtgen::packing
