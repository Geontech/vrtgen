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
#ifndef _VRTGEN_PACKING_CIF1_HPP
#define _VRTGEN_PACKING_CIF1_HPP

#include <vrtgen/types.hpp>
#include <vrtgen/packing/enums.hpp>
#include <vrtgen/packing/indicator_fields.hpp>
#include <vector>
#include <type_traits>

namespace vrtgen::packing {

/**
 * Antenna polarization [9.4.8].
 */
class Polarization
{
public:
    float tilt_angle() const noexcept
    {
        return vrtgen::fixed::to_fp<16,13>(vrtgen::swap::from_be(m_tilt_angle));
    }

    void tilt_angle(float value) noexcept
    {
        m_tilt_angle = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,13>(value));
    }

    float ellipticity_angle() const noexcept
    {
        return vrtgen::fixed::to_fp<16,13>(vrtgen::swap::from_be(m_ellipticity_angle));
    }

    void ellipticity_angle(float value) noexcept
    {
        m_ellipticity_angle = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,13>(value));
    }

    constexpr size_t size() const noexcept
    {
        return sizeof(m_tilt_angle) +
               sizeof(m_ellipticity_angle);
    }

    void pack_into(uint8_t* buffer_ptr) const
    {
        std::memcpy(buffer_ptr, &m_tilt_angle, sizeof(m_tilt_angle));
        buffer_ptr += sizeof(m_tilt_angle);
        std::memcpy(buffer_ptr, &m_ellipticity_angle, sizeof(m_ellipticity_angle));
    }

    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        std::memcpy(&m_tilt_angle, ptr, sizeof(m_tilt_angle));
        ptr += sizeof(m_tilt_angle);
        std::memcpy(&m_ellipticity_angle, ptr, sizeof(m_ellipticity_angle));
    }

private:
    /*
     * tilt_angle 0/31 
     * ellipticity_angle 0/15 
     */
    int16_t m_tilt_angle{ 0 };
    int16_t m_ellipticity_angle{ 0 };

}; // end class Polarization

/**
 * 3-D Pointing Vector [9.4.1.1].
 */
class PointingVector
{
public:
    float elevation() const noexcept
    {
        return vrtgen::fixed::to_fp<16,7>(vrtgen::swap::from_be(m_elevation));
    }

    void elevation(float value) noexcept
    {
        m_elevation = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,7>(value));
    }

    float azimuthal() const noexcept
    {
        return vrtgen::fixed::to_fp<16,7>(vrtgen::swap::from_be(m_azimuthal));
    }

    void azimuthal(float value) noexcept
    {
        m_azimuthal = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,7>(value));
    }

    constexpr size_t size() const noexcept
    {
        return sizeof(m_elevation) +
               sizeof(m_azimuthal);
    }

    void pack_into(uint8_t* buffer_ptr) const
    {
        std::memcpy(buffer_ptr, &m_elevation, sizeof(m_elevation));
        buffer_ptr += sizeof(m_elevation);
        std::memcpy(buffer_ptr, &m_azimuthal, sizeof(m_azimuthal));
    }

    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        std::memcpy(&m_elevation, ptr, sizeof(m_elevation));
        ptr += sizeof(m_elevation);
        std::memcpy(&m_azimuthal, ptr, sizeof(m_azimuthal));
    }

private:
    /*
     * elevation 0/31 
     * azimuthal 0/15 
     */
    int16_t m_elevation{ 0 };
    int16_t m_azimuthal{ 0 };

}; // end class PointingVector

/**
 * Beam Widths [9.4.2].
 */
class BeamWidth
{
public:
    float horizontal() const noexcept
    {
        return vrtgen::fixed::to_fp<16,7>(vrtgen::swap::from_be(m_horizontal));
    }

    void horizontal(float value) noexcept
    {
        m_horizontal = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,7>(value));
    }

    float vertical() const noexcept
    {
        return vrtgen::fixed::to_fp<16,7>(vrtgen::swap::from_be(m_vertical));
    }

    void vertical(float value) noexcept
    {
        m_vertical = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,7>(value));
    }

    constexpr size_t size() const noexcept
    {
        return sizeof(m_horizontal) +
               sizeof(m_vertical);
    }

    void pack_into(uint8_t* buffer_ptr) const
    {
        std::memcpy(buffer_ptr, &m_horizontal, sizeof(m_horizontal));
        buffer_ptr += sizeof(m_horizontal);
        std::memcpy(buffer_ptr, &m_vertical, sizeof(m_vertical));
    }

    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        std::memcpy(&m_horizontal, ptr, sizeof(m_horizontal));
        ptr += sizeof(m_horizontal);
        std::memcpy(&m_vertical, ptr, sizeof(m_vertical));
    }

private:
    /*
     * horizontal 0/31 
     * vertical 0/15 
     */
    int16_t m_horizontal{ 0 };
    int16_t m_vertical{ 0 };

}; // end class BeamWidth

/**
 * Energy per Bit to Noise Density Ratio (Eb/No) and Bit Error Rate [9.5.17].
 */
class EbNoBER
{
public:
    float horizontal() const noexcept
    {
        return vrtgen::fixed::to_fp<16,7>(vrtgen::swap::from_be(m_horizontal));
    }

    void horizontal(float value) noexcept
    {
        m_horizontal = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,7>(value));
    }

    float vertical() const noexcept
    {
        return vrtgen::fixed::to_fp<16,7>(vrtgen::swap::from_be(m_vertical));
    }

    void vertical(float value) noexcept
    {
        m_vertical = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,7>(value));
    }

    constexpr size_t size() const noexcept
    {
        return sizeof(m_horizontal) +
               sizeof(m_vertical);
    }

    void pack_into(uint8_t* buffer_ptr) const
    {
        std::memcpy(buffer_ptr, &m_horizontal, sizeof(m_horizontal));
        buffer_ptr += sizeof(m_horizontal);
        std::memcpy(buffer_ptr, &m_vertical, sizeof(m_vertical));
    }

    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        std::memcpy(&m_horizontal, ptr, sizeof(m_horizontal));
        ptr += sizeof(m_horizontal);
        std::memcpy(&m_vertical, ptr, sizeof(m_vertical));
    }

private:
    /*
     * horizontal 0/31 
     * vertical 0/15 
     */
    int16_t m_horizontal{ vrtgen::swap::to_be(static_cast<int16_t>(0x7FFF)) };
    int16_t m_vertical{ vrtgen::swap::to_be(static_cast<int16_t>(0x7FFF)) };

}; // end class EbNoBER

/**
 * Signal threshold level [9.5.13].
 */
class Threshold
{
public:
    float stage_1() const noexcept
    {
        return vrtgen::fixed::to_fp<16,7>(vrtgen::swap::from_be(m_stage_1));
    }

    void stage_1(float value) noexcept
    {
        m_stage_1 = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,7>(value));
    }

    float stage_2() const noexcept
    {
        return vrtgen::fixed::to_fp<16,7>(vrtgen::swap::from_be(m_stage_2));
    }

    void stage_2(float value) noexcept
    {
        m_stage_2 = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,7>(value));
    }

    constexpr size_t size() const noexcept
    {
        return sizeof(m_stage_1) +
               sizeof(m_stage_2);
    }

    void pack_into(uint8_t* buffer_ptr) const
    {
        std::memcpy(buffer_ptr, &m_stage_1, sizeof(m_stage_1));
        buffer_ptr += sizeof(m_stage_1);
        std::memcpy(buffer_ptr, &m_stage_2, sizeof(m_stage_2));
    }

    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        std::memcpy(&m_stage_1, ptr, sizeof(m_stage_1));
        ptr += sizeof(m_stage_1);
        std::memcpy(&m_stage_2, ptr, sizeof(m_stage_2));
    }

private:
    /*
     * stage_1 0/31 
     * stage_2 0/15 
     */
    int16_t m_stage_1{ 0 };
    int16_t m_stage_2{ 0 };

}; // end class Threshold

/**
 * Second and Third Order Intercept points [9.5.6].
 */
class InterceptPoints
{
public:
    float second_order() const noexcept
    {
        return vrtgen::fixed::to_fp<16,7>(vrtgen::swap::from_be(m_second_order));
    }

    void second_order(float value) noexcept
    {
        m_second_order = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,7>(value));
    }

    float third_order() const noexcept
    {
        return vrtgen::fixed::to_fp<16,7>(vrtgen::swap::from_be(m_third_order));
    }

    void third_order(float value) noexcept
    {
        m_third_order = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,7>(value));
    }

    constexpr size_t size() const noexcept
    {
        return sizeof(m_second_order) +
               sizeof(m_third_order);
    }

    void pack_into(uint8_t* buffer_ptr) const
    {
        std::memcpy(buffer_ptr, &m_second_order, sizeof(m_second_order));
        buffer_ptr += sizeof(m_second_order);
        std::memcpy(buffer_ptr, &m_third_order, sizeof(m_third_order));
    }

    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        std::memcpy(&m_second_order, ptr, sizeof(m_second_order));
        ptr += sizeof(m_second_order);
        std::memcpy(&m_third_order, ptr, sizeof(m_third_order));
    }

private:
    /*
     * second_order 0/31 
     * third_order 0/15 
     */
    int16_t m_second_order{ 0 };
    int16_t m_third_order{ 0 };

}; // end class InterceptPoints

/**
 * Signal-to-Noise Ratio (SNR) and Noise Figure [9.5.7].
 */
class SNRNoise
{
public:
    float snr() const noexcept
    {
        return vrtgen::fixed::to_fp<16,7>(vrtgen::swap::from_be(m_snr));
    }

    void snr(float value) noexcept
    {
        m_snr = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,7>(value));
    }

    float noise() const noexcept
    {
        return vrtgen::fixed::to_fp<16,7>(vrtgen::swap::from_be(m_noise));
    }

    void noise(float value) noexcept
    {
        m_noise = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,7>(value));
    }

    constexpr size_t size() const noexcept
    {
        return sizeof(m_snr) +
               sizeof(m_noise);
    }

    void pack_into(uint8_t* buffer_ptr) const
    {
        std::memcpy(buffer_ptr, &m_snr, sizeof(m_snr));
        buffer_ptr += sizeof(m_snr);
        std::memcpy(buffer_ptr, &m_noise, sizeof(m_noise));
    }

    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        std::memcpy(&m_snr, ptr, sizeof(m_snr));
        ptr += sizeof(m_snr);
        std::memcpy(&m_noise, ptr, sizeof(m_noise));
    }

private:
    /*
     * snr 0/31 
     * noise 0/15 
     */
    int16_t m_snr{ vrtgen::swap::to_be(static_cast<int16_t>(0x7FFF)) };
    int16_t m_noise{ 0 };

}; // end class SNRNoise

// /**
//  * Sector/Step-Scan Control/Context Indicator Word [9.6.2.1]
//  */
// class SectorStepScanCIF : public IndicatorField
// {
// public:
//     bool sector_number() const noexcept
//     {
//         return m_packed_0.get<31>();
//     }

//     void sector_number(bool value) noexcept
//     {
//         m_packed_0.set<31>(value);
//     }

//     bool f1_start_frequency() const noexcept
//     {
//         return m_packed_0.get<30>();
//     }

//     void f1_start_frequency(bool value) noexcept
//     {
//         m_packed_0.set<30>(value);
//     }

//     bool f2_stop_frequency() const noexcept
//     {
//         return m_packed_0.get<29>();
//     }

//     void f2_stop_frequency(bool value) noexcept
//     {
//         m_packed_0.set<29>(value);
//     }

//     bool resolution_bandwidth() const noexcept
//     {
//         return m_packed_0.get<28>();
//     }

//     void resolution_bandwidth(bool value) noexcept
//     {
//         m_packed_0.set<28>(value);
//     }

//     bool tune_step_size() const noexcept
//     {
//         return m_packed_0.get<27>();
//     }

//     void tune_step_size(bool value) noexcept
//     {
//         m_packed_0.set<27>(value);
//     }

//     bool number_points() const noexcept
//     {
//         return m_packed_0.get<26>();
//     }

//     void number_points(bool value) noexcept
//     {
//         m_packed_0.set<26>(value);
//     }

//     bool default_gain() const noexcept
//     {
//         return m_packed_0.get<25>();
//     }

//     void default_gain(bool value) noexcept
//     {
//         m_packed_0.set<25>(value);
//     }

//     bool threshold() const noexcept
//     {
//         return m_packed_0.get<24>();
//     }

//     void threshold(bool value) noexcept
//     {
//         m_packed_0.set<24>(value);
//     }

//     bool dwell_time() const noexcept
//     {
//         return m_packed_0.get<23>();
//     }

//     void dwell_time(bool value) noexcept
//     {
//         m_packed_0.set<23>(value);
//     }

//     bool start_time() const noexcept
//     {
//         return m_packed_0.get<22>();
//     }

//     void start_time(bool value) noexcept
//     {
//         m_packed_0.set<22>(value);
//     }

//     bool time_3() const noexcept
//     {
//         return m_packed_0.get<21>();
//     }

//     void time_3(bool value) noexcept
//     {
//         m_packed_0.set<21>(value);
//     }

//     bool time_4() const noexcept
//     {
//         return m_packed_0.get<20>();
//     }

//     void time_4(bool value) noexcept
//     {
//         m_packed_0.set<20>(value);
//     }

// }; // end class SectorStepScanCIF

// /**
//  * Sector/Step-Scan Field [9.6.2]
//  */
// template<typename T>
// class SectorStepScan
// {
// public:
//     uint32_t array_size() const noexcept
//     {
//         return vrtgen::swap::from_be(m_array_size);
//     }

//     void array_size(uint32_t value) noexcept
//     {
//         m_array_size = vrtgen::swap::to_be(value);
//     }

//     uint8_t header_size() const noexcept
//     {
//         return m_packed_0.get<uint8_t,31,8>();
//     }

//     void header_size(uint8_t value) noexcept
//     {
//         m_packed_0.set<uint8_t,31,8>(value);
//     }

//     uint16_t num_words_record() const noexcept
//     {
//         return m_packed_0.get<uint16_t,23,12>();
//     }

//     void num_words_record(uint16_t value) noexcept
//     {
//         m_packed_0.set<uint16_t,23,12>(value);
//     }

//     uint16_t num_records() const noexcept
//     {
//         return m_packed_0.get<uint16_t,11,12>();
//     }

//     void num_records(uint16_t value) noexcept
//     {
//         m_packed_0.set<uint16_t,11,12>(value);
//     }

//     SectorStepScanCIF& subfield_cif() noexcept
//     {
//         return m_subfield_cif;
//     }

//     const SectorStepScanCIF& subfield_cif() const noexcept
//     {
//         return m_subfield_cif;
//     }

//     void subfield_cif(const SectorStepScanCIF& value) noexcept
//     {
//         m_subfield_cif = value;
//     }

//     std::vector<T>& records() noexcept
//     {
//         return m_records;
//     }

//     const std::vector<T>& records() const noexcept
//     {
//         return m_records;
//     }

//     void records(const std::vector<T>& value) noexcept
//     {
//         m_records = value;
//     }

//     std::size_t size() const
//     {
//         std::size_t retval = 0;
//         retval += sizeof(m_array_size);
//         retval += m_packed_0.size();
//         retval += m_subfield_cif.size();
//         if (!m_records.empty()) {
//             retval += m_records.size() * m_records.front().size();
//         }
//         return retval;
//     }

//     void pack_into(uint8_t* buffer_ptr) const
//     {
//         std::memcpy(buffer_ptr, &m_array_size, sizeof(m_array_size));
//         buffer_ptr += sizeof(m_array_size);
//         m_packed_0.pack_into(buffer_ptr);
//         buffer_ptr += m_packed_0.size();
//         m_subfield_cif.pack_into(buffer_ptr);
//         buffer_ptr += m_subfield_cif.size();
//         for (const auto& elem : m_records) {
//             elem.pack_into(buffer_ptr);
//             buffer_ptr += elem.size();
//         }
//     }

//     void unpack_from(const uint8_t* buffer_ptr)
//     {
//         auto* ptr = buffer_ptr;
//         std::memcpy(&m_array_size, ptr, sizeof(m_array_size));
//         ptr += sizeof(m_array_size);
//         m_packed_0.unpack_from(ptr);
//         ptr += m_packed_0.size();
//         m_subfield_cif.unpack_from(ptr);
//         ptr += m_subfield_cif.size();
//         m_records.resize(num_records());
//         for (size_t i=0; i<m_records.size(); ++i) {
//             m_records[i].unpack_from(ptr);
//             ptr += m_records[i].size();
//         }
//     }

// private:
//     /*
//      * array_size 0/31 
//      * header_size 1/31 
//      * num_words_record 1/23 
//      * num_records 1/11 
//      * subfield_cif 2/31 
//      * records 3/31 
//      */
//     uint32_t m_array_size{ 0 };
//     using header_size_tag = vrtgen::packed_tag;
//     using num_words_record_tag = vrtgen::packed_tag;
//     using num_records_tag = vrtgen::packed_tag;
//     vrtgen::packed<32> m_packed_0;
//     SectorStepScanCIF m_subfield_cif;
//     std::vector<T> m_records;

// }; // end class SectorStepScan

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
        return m_packed_0.get(m_entry_size_tag);
    }

    void entry_size(EntrySize value) noexcept
    {
        m_packed_0.set(value, m_entry_size_tag);
    }

    uint32_t num_entries() const noexcept
    {
        return m_packed_0.get(m_num_entries_tag);
    }

    void num_entries(uint32_t value) noexcept
    {
        m_packed_0.set(value, m_num_entries_tag);
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
        retval += m_packed_0.size();
        if (!m_entries.empty()) {
            retval += m_entries.size() * sizeof(T);
        }
        return retval;
    }

    void pack_into(uint8_t* buffer_ptr) const
    {
        std::memcpy(buffer_ptr, &m_total_size, sizeof(m_total_size));
        buffer_ptr += sizeof(m_total_size);
        m_packed_0.pack_into(buffer_ptr);
        buffer_ptr += m_packed_0.size();
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
        m_packed_0.unpack_from(ptr);
        ptr += m_packed_0.size();
        m_entries.resize(num_entries());
        for (size_t i=0; i<m_entries.size(); ++i) {
            std::memcpy(m_entries.data() + i, ptr, sizeof(m_entries[i]));
            ptr += sizeof(m_entries[i]);
        }
    }

private:
    uint32_t m_total_size{ 0 };
    /**
     * entry_size 1/31
     * num_entries 1/19
     */
    vrtgen::packed_tag<EntrySize,31,4> m_entry_size_tag;
    vrtgen::packed_tag<uint32_t,19,20> m_num_entries_tag;
    vrtgen::packed<uint32_t> m_packed_0;
    std::vector<T> m_entries;

}; // end class IndexList

/**
 * CIF1 Enables
 */
class CIF1 : public IndicatorField1 {};

} // end namespace vrtgen::packing

#endif // _VRTGEN_PACKING_CIF1_HPP
