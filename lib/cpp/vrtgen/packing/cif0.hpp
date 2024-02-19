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
#include "indicator_fields.hpp"
#include "state_event_indicators.hpp"
#include "enums.hpp"

namespace vrtgen::packing {

/**
 * @class CIF0
 * @brief Context/Command Indicator Field 0 (VITA 49.2-2017 Section 9.1)
 */
class CIF0 : public IndicatorField0
{
public:
    /**
     * @brief Returns the Command/Context Indicator Field 7 enable flag
     * @return true if packet contains Command/Context Indicator Field 7 word, otherwise false
     * 
     * Command/Context Indicator Field 7 Enable is 1 bit long at bit position 7
     * See VITA 49.2-2017-2017 Table 9.1-1
     */
    bool cif7_enable() const noexcept
    {
        return IndicatorField0::if7_enable();
    }

    /**
     * @brief Sets the Command/Context Indicator Field 7 enable flag
     * @param value Command/Context Indicator Field 7 enable flag value to set
     * 
     * Command/Context Indicator Field 7 Enable is 1 bit long at bit position 7
     * See VITA 49.2-2017-2017 Table 9.1-1
     */
    void cif7_enable(bool value) noexcept
    {
        IndicatorField0::if7_enable(value);
    }

    /**
     * @brief Returns the Command/Context Indicator Field 3 enable flag
     * @return true if packet contains Command/Context Indicator Field 3 word, otherwise false
     * 
     * Command/Context Indicator Field 3 Enable is 1 bit long at bit position 3
     * See VITA 49.2-2017-2017 Table 9.1-1
     */
    bool cif3_enable() const noexcept
    {
        return IndicatorField0::if3_enable();
    }

    /**
     * @brief Sets the Command/Context Indicator Field 3 enable flag
     * @param value Command/Context Indicator Field 3 enable flag value to set
     * 
     * Command/Context Indicator Field 3 Enable is 1 bit long at bit position 3
     * See VITA 49.2-2017-2017 Table 9.1-1
     */
    void cif3_enable(bool value) noexcept
    {
        IndicatorField0::if3_enable(value);
    }

    /**
     * @brief Returns the Command/Context Indicator Field 2 enable flag
     * @return true if packet contains Command/Context Indicator Field 2 word, otherwise false
     * 
     * Command/Context Indicator Field 2 Enable is 1 bit long at bit position 2
     * See VITA 49.2-2017-2017 Table 9.1-1
     */
    bool cif2_enable() const noexcept
    {
        return IndicatorField0::if2_enable();
    }

    /**
     * @brief Sets the Command/Context Indicator Field 2 enable flag
     * @param value Command/Context Indicator Field 2 enable flag value to set
     * 
     * Command/Context Indicator Field 2 Enable is 1 bit long at bit position 2
     * See VITA 49.2-2017-2017 Table 9.1-1
     */
    void cif2_enable(bool value) noexcept
    {
        IndicatorField0::if2_enable(value);
    }

    /**
     * @brief Returns the Command/Context Indicator Field 1 enable flag
     * @return true if packet contains Command/Context Indicator Field 1 word, otherwise false
     * 
     * Command/Context Indicator Field 1 Enable is 1 bit long at bit position 1
     * See VITA 49.2-2017-2017 Table 9.1-1
     */
    bool cif1_enable() const noexcept
    {
        return IndicatorField0::if1_enable();
    }

    /**
     * @brief Sets the Command/Context Indicator Field 1 enable flag
     * @param value Command/Context Indicator Field 1 enable flag value to set
     * 
     * Command/Context Indicator Field 1 Enable is 1 bit long at bit position 1
     * See VITA 49.2-2017-2017 Table 9.1-1
     */
    void cif1_enable(bool value) noexcept
    {
        IndicatorField0::if1_enable(value);
    }

}; // end class CIF0

/**
 * @class Gain
 * @brief Gain/Attenuation field (VITA 49.2-2017 Section 9.5.3)
 */
class Gain : public Packed32_Fixed16R<7>
{
public:
    /**
     * @brief Returns the Stage 2 Gain subfield value
     * @return Stage 2 Gain subfield value
     * 
     * Stage 2 Gain subfield is 16 bits long at bit position 31 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017-2017 Figure 9.5.3-1
     */
    double stage_2() const noexcept
    {
        return subfield_31();
    }

    /**
     * @brief Sets the Stage 2 Gain subfield value
     * @param value Stage 2 Gain subfield value to set
     * 
     * Stage 2 Gain subfield is 16 bits long at bit position 31 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017-2017 Figure 9.5.3-1
     */
    void stage_2(double value) noexcept
    {
        subfield_31(value);
    }

    /**
     * @brief Returns the Stage 1 Gain subfield value
     * @return Stage 1 Gain subfield value
     * 
     * Stage 1 Gain subfield is 16 bits long at bit position 15 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017-2017 Figure 9.5.3-1
     */
    double stage_1() const noexcept
    {
        return subfield_15();
    }

    /**
     * @brief Sets the Stage 1 Gain subfield value
     * @param value Stage 1 Gain subfield value to set
     * 
     * Stage 1 Gain subfield is 16 bits long at bit position 15 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2-2017-2017 Figure 9.5.3-1
     */
    void stage_1(double value) noexcept
    {
        subfield_15(value);
    }

}; // end class Gain

/**
 * @class DeviceIdentifier
 * @brief Device Identifier Field (VITA 49.2-2017 Section 9.10.1)
 */
class DeviceIdentifier
{
public:
    /**
     * @brief Returns the Manufacturer OUI subfield value
     * @return Manufacturer OUI subfield value
     * 
     * Manufacturer OUI subfield is 24 bits long at bit position 23 in word 1
     * See VITA 49.2-2017-2017 Figure 9.10.1-1
     */
    uint32_t manufacturer_oui() const noexcept
    {
        return m_packed.get<55,24,uint32_t>();
    }

    /**
     * @brief Sets the Manufacturer OUI subfield value
     * @param value Manufacturer OUI subfield value to set
     * 
     * Manufacturer OUI subfield is 24 bits long at bit position 23 in word 1
     * See VITA 49.2-2017-2017 Figure 9.10.1-1
     */
    void manufacturer_oui(uint32_t value) noexcept
    {
        m_packed.set<55,24>(value);
    }

    /**
     * @brief Returns the Device Code subfield value
     * @return Device Code subfield value
     * 
     * Device Code subfield is 16 bits long at bit position 15 in word 2
     * See VITA 49.2-2017-2017 Figure 9.10.1-1
     */
    uint16_t device_code() const noexcept
    {
        return m_packed.get<15,16,uint16_t>();
    }

    /**
     * @brief Sets the Device Code subfield value
     * @param value Device Code subfield value to set
     * 
     * Device Code subfield is 16 bits long at bit position 15 in word 2
     * See VITA 49.2-2017-2017 Figure 9.10.1-1
     */
    void device_code(uint16_t value) noexcept
    {
        m_packed.set<15,16>(value);
    }

    /**
     * @brief Returns the number of DeviceIdentifier bytes
     * @return Number of DeviceIdentifier bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return m_packed.size();
    }

    /**
     * @brief Pack DeviceIdentifier as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add DeviceIdentifier bytes
     */
    inline void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into DeviceIdentifier
     * @param buffer_ptr Pointer to beginning of DeviceIdentifier bytes in the buffer
     */
    inline void unpack_from(const uint8_t* buffer_ptr)
    {
        m_packed.unpack_from(buffer_ptr);
    }

private:
    vrtgen::packed<uint64_t> m_packed;

}; // end class DeviceIdentifier

/**
 * @class Geolocation
 * @brief Formatted GPS Geolocation (9.4.5) and INS Geolocation (9.4.6)
 */
class Geolocation
{
public:
    /**
     * @brief Returns the TimeStamp-Integer code subfield value
     * @return TimeStamp-Integer code subfield value
     * 
     * TimeStamp-Integer code subfield is 2 bits long at bit position 27 in word 1
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    TSI tsi() const noexcept
    {
        return m_packed.get<27,2,TSI>();
    }

    /**
     * @brief Sets the TimeStamp-Integer code subfield value
     * @param value TimeStamp-Integer code subfield value to set
     * 
     * TimeStamp-Integer code subfield is 2 bits long at bit position 27 in word 1
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    void tsi(TSI value) noexcept
    {
        m_packed.set<27,2>(value);
    }

    /**
     * @brief Returns the TimeStamp-Fractional code subfield value
     * @return TimeStamp-Fractional code subfield value
     * 
     * TimeStamp-Fractional code subfield is 2 bits long at bit position 25 in word 1
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    TSF tsf() const noexcept
    {
        return m_packed.get<25,2,TSF>();
    }

    /**
     * @brief Sets the TimeStamp-Fractional code subfield value
     * @param value TimeStamp-Fractional code subfield value to set
     * 
     * TimeStamp-Fractional code subfield is 2 bits long at bit position 25 in word 1
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    void tsf(TSF value) noexcept
    {
        m_packed.set<25,2>(value);
    }

    /**
     * @brief Returns the Manufacturer OUI subfield value
     * @return Manufacturer OUI subfield value
     * 
     * Manufacturer OUI subfield is 24 bits long at bit position 23 in word 1
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    uint32_t manufacturer_oui() const noexcept
    {
        return m_packed.get<23,24,uint32_t>();
    }

    /**
     * @brief Sets the Manufacturer OUI subfield value
     * @param value Manufacturer OUI subfield value to set
     * 
     * Manufacturer OUI subfield is 24 bits long at bit position 23 in word 1
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    void manufacturer_oui(uint32_t value) noexcept
    {
        m_packed.set<23,24>(value);
    }

    /**
     * @brief Returns the Integer Timestamp subfield value
     * @return Integer Timestamp subfield value
     * 
     * Integer Timestamp subfield is 32 bits long at bit position 31 in word 2
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    uint32_t integer_timestamp() const noexcept
    {
        return vrtgen::swap::from_be(m_integer_timestamp);
    }

    /**
     * @brief Sets the Integer Timestamp subfield value
     * @param value Integer Timestamp subfield value to set
     * 
     * Integer Timestamp subfield is 32 bits long at bit position 31 in word 2
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    void integer_timestamp(uint32_t value) noexcept
    {
        m_integer_timestamp = vrtgen::swap::to_be(value);
    }

    /**
     * @brief Returns the Fractional Timestamp subfield value
     * @return Fractional Timestamp subfield value
     * 
     * Fractional Timestamp subfield is 64 bits long at bit position 31 in word 3
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    uint64_t fractional_timestamp() const noexcept
    {
        return vrtgen::swap::from_be(m_fractional_timestamp);
    }

    /**
     * @brief Sets the Fractional Timestamp subfield value
     * @param value Fractional Timestamp subfield value to set
     * 
     * Fractional Timestamp subfield is 64 bits long at bit position 31 in word 3
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    void fractional_timestamp(uint64_t value) noexcept
    {
        m_fractional_timestamp = vrtgen::swap::to_be(value);
    }

    /**
     * @brief Returns the Latitude subfield value
     * @return Latitude subfield value
     * 
     * Latitude subfield is 32 bits long at bit position 31 in word 5 expressed
     * in two's-complement format with a radix point to the right of bit 22
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    double latitude() const noexcept
    {
        return vrtgen::fixed::to_fp<32,22>(vrtgen::swap::from_be(m_latitude));
    }

    /**
     * @brief Sets the Latitude subfield value
     * @param value Latitude subfield value to set
     * 
     * Latitude subfield is 32 bits long at bit position 31 in word 5 expressed
     * in two's-complement format with a radix point to the right of bit 22
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    void latitude(double value) noexcept
    {
        m_latitude = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,22>(value));
    }

    /**
     * @brief Returns the Longitude subfield value
     * @return Longitude subfield value
     * 
     * Longitude subfield is 32 bits long at bit position 31 in word 5 expressed
     * in two's-complement format with a radix point to the right of bit 22
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    double longitude() const noexcept
    {
        return vrtgen::fixed::to_fp<32,22>(vrtgen::swap::from_be(m_longitude));
    }

    /**
     * @brief Sets the Longitude subfield value
     * @param value Longitude subfield value to set
     * 
     * Longitude subfield is 32 bits long at bit position 31 in word 5 expressed
     * in two's-complement format with a radix point to the right of bit 22
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    void longitude(double value) noexcept
    {
        m_longitude = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,22>(value));
    }

    /**
     * @brief Returns the Altitude subfield value
     * @return Altitude subfield value
     * 
     * Altitude subfield is 32 bits long at bit position 31 in word 7 expressed
     * in two's-complement format with a radix point to the right of bit 5
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    double altitude() const noexcept
    {
        return vrtgen::fixed::to_fp<32,5>(vrtgen::swap::from_be(m_altitude));
    }

    /**
     * @brief Sets the Altitude subfield value
     * @param value Altitude subfield value to set
     * 
     * Altitude subfield is 32 bits long at bit position 31 in word 7 expressed
     * in two's-complement format with a radix point to the right of bit 5
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    void altitude(double value) noexcept
    {
        m_altitude = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,5>(value));
    }

    /**
     * @brief Returns the Speed Over Ground subfield value
     * @return Speed Over Ground subfield value
     * 
     * Speed Over Ground subfield is 32 bits long at bit position 31 in word 8
     * expressed in two's-complement format with a radix point to the right of bit 16
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    double speed_over_ground() const noexcept
    {
        return vrtgen::fixed::to_fp<32,16>(vrtgen::swap::from_be(m_speed_over_ground));
    }

    /**
     * @brief Sets the Speed Over Ground subfield value
     * @param value Speed Over Ground subfield value to set
     * 
     * Speed Over Ground subfield is 32 bits long at bit position 31 in word 8
     * expressed in two's-complement format with a radix point to the right of bit 16
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    void speed_over_ground(double value) noexcept
    {
        m_speed_over_ground = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,16>(value));
    }

    /**
     * @brief Returns the Heading Angle subfield value
     * @return Heading Angle subfield value
     * 
     * Heading Angle subfield is 32 bits long at bit position 31 in word 9 expressed
     * in two's-complement format with a radix point to the right of bit 22
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    double heading_angle() const noexcept
    {
        return vrtgen::fixed::to_fp<32,22>(vrtgen::swap::from_be(m_heading_angle));
    }

    /**
     * @brief Sets the Heading Angle subfield value
     * @param value Heading Angle subfield value to set
     * 
     * Heading Angle subfield is 32 bits long at bit position 31 in word 9 expressed
     * in two's-complement format with a radix point to the right of bit 22
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    void heading_angle(double value) noexcept
    {
        m_heading_angle = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,22>(value));
    }

    /**
     * @brief Returns the Track Angle subfield value
     * @return Track Angle subfield value
     * 
     * Track Angle subfield is 32 bits long at bit position 31 in word 10 expressed
     * in two's-complement format with a radix point to the right of bit 22
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    double track_angle() const noexcept
    {
        return vrtgen::fixed::to_fp<32,22>(vrtgen::swap::from_be(m_track_angle));
    }

    /**
     * @brief Sets the Track Angle subfield value
     * @param value Track Angle subfield value to set
     * 
     * Track Angle subfield is 32 bits long at bit position 31 in word 10 expressed
     * in two's-complement format with a radix point to the right of bit 22
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    void track_angle(double value) noexcept
    {
        m_track_angle = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,22>(value));
    }

    /**
     * @brief Returns the Magnetic Variation subfield value
     * @return Magnetic Variation subfield value
     * 
     * Magnetic Variation subfield is 32 bits long at bit position 31 in word 11
     * expressed in two's-complement format with a radix point to the right of bit 22
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    double magnetic_variation() const noexcept
    {
        return vrtgen::fixed::to_fp<32,22>(vrtgen::swap::from_be(m_magnetic_variation));
    }

    /**
     * @brief Sets the Magnetic Variation subfield value
     * @param value Magnetic Variation subfield value to set
     * 
     * Magnetic Variation subfield is 32 bits long at bit position 31 in word 11
     * expressed in two's-complement format with a radix point to the right of bit 22
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    void magnetic_variation(double value) noexcept
    {
        m_magnetic_variation = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,22>(value));
    }

    /**
     * @brief Returns the number of Gelocation bytes
     * @return Number of Gelocation bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return m_packed.size() +
               sizeof(m_integer_timestamp) +
               sizeof(m_fractional_timestamp) +
               sizeof(m_latitude) +
               sizeof(m_longitude) +
               sizeof(m_altitude) +
               sizeof(m_speed_over_ground) +
               sizeof(m_heading_angle) +
               sizeof(m_track_angle) +
               sizeof(m_magnetic_variation);
    }

    /**
     * @brief Pack Gelocation as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add Gelocation bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed.pack_into(buffer_ptr);
        buffer_ptr += m_packed.size();
        std::memcpy(buffer_ptr, &m_integer_timestamp, sizeof(m_integer_timestamp));
        buffer_ptr += sizeof(m_integer_timestamp);
        std::memcpy(buffer_ptr, &m_fractional_timestamp, sizeof(m_fractional_timestamp));
        buffer_ptr += sizeof(m_fractional_timestamp);
        std::memcpy(buffer_ptr, &m_latitude, sizeof(m_latitude));
        buffer_ptr += sizeof(m_latitude);
        std::memcpy(buffer_ptr, &m_longitude, sizeof(m_longitude));
        buffer_ptr += sizeof(m_longitude);
        std::memcpy(buffer_ptr, &m_altitude, sizeof(m_altitude));
        buffer_ptr += sizeof(m_altitude);
        std::memcpy(buffer_ptr, &m_speed_over_ground, sizeof(m_speed_over_ground));
        buffer_ptr += sizeof(m_speed_over_ground);
        std::memcpy(buffer_ptr, &m_heading_angle, sizeof(m_heading_angle));
        buffer_ptr += sizeof(m_heading_angle);
        std::memcpy(buffer_ptr, &m_track_angle, sizeof(m_track_angle));
        buffer_ptr += sizeof(m_track_angle);
        std::memcpy(buffer_ptr, &m_magnetic_variation, sizeof(m_magnetic_variation));
    }

    /**
     * @brief Unpack buffer bytes into Gelocation
     * @param buffer_ptr Pointer to beginning of Gelocation bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        m_packed.unpack_from(ptr);
        ptr += m_packed.size();
        std::memcpy(&m_integer_timestamp, ptr, sizeof(m_integer_timestamp));
        ptr += sizeof(m_integer_timestamp);
        std::memcpy(&m_fractional_timestamp, ptr, sizeof(m_fractional_timestamp));
        ptr += sizeof(m_fractional_timestamp);
        std::memcpy(&m_latitude, ptr, sizeof(m_latitude));
        ptr += sizeof(m_latitude);
        std::memcpy(&m_longitude, ptr, sizeof(m_longitude));
        ptr += sizeof(m_longitude);
        std::memcpy(&m_altitude, ptr, sizeof(m_altitude));
        ptr += sizeof(m_altitude);
        std::memcpy(&m_speed_over_ground, ptr, sizeof(m_speed_over_ground));
        ptr += sizeof(m_speed_over_ground);
        std::memcpy(&m_heading_angle, ptr, sizeof(m_heading_angle));
        ptr += sizeof(m_heading_angle);
        std::memcpy(&m_track_angle, ptr, sizeof(m_track_angle));
        ptr += sizeof(m_track_angle);
        std::memcpy(&m_magnetic_variation, ptr, sizeof(m_magnetic_variation));
    }

private:
    vrtgen::packed<uint32_t> m_packed;
    uint32_t m_integer_timestamp{ static_cast<uint32_t>(0xFFFFFFFF) };
    uint64_t m_fractional_timestamp{ static_cast<uint64_t>(0xFFFFFFFFFFFFFFFF) };
    int32_t m_latitude{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };
    int32_t m_longitude{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };
    int32_t m_altitude{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };
    int32_t m_speed_over_ground{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };
    int32_t m_heading_angle{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };
    int32_t m_track_angle{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };
    int32_t m_magnetic_variation{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };

}; // end class Geolocation

/**
 * @class Ephemeris
 * @brief ECEF Ephemeris (9.4.3) and Relative Ephemeris (9.4.9)
 */
class Ephemeris
{
public:
    /**
     * @brief Returns the TimeStamp-Integer code subfield value
     * @return TimeStamp-Integer code subfield value
     * 
     * TimeStamp-Integer code subfield is 2 bits long at bit position 27 in word 1
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    TSI tsi() const noexcept
    {
        return m_packed.get<27,2,TSI>();
    }

    /**
     * @brief Sets the TimeStamp-Integer code subfield value
     * @param value TimeStamp-Integer code subfield value to set
     * 
     * TimeStamp-Integer code subfield is 2 bits long at bit position 27 in word 1
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    void tsi(TSI value) noexcept
    {
        m_packed.set<27,2>(value);
    }

    /**
     * @brief Returns the TimeStamp-Fractional code subfield value
     * @return TimeStamp-Fractional code subfield value
     * 
     * TimeStamp-Fractional code subfield is 2 bits long at bit position 25 in word 1
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    TSF tsf() const noexcept
    {
        return m_packed.get<25,2,TSF>();
    }

    /**
     * @brief Sets the TimeStamp-Fractional code subfield value
     * @param value TimeStamp-Fractional code subfield value to set
     * 
     * TimeStamp-Fractional code subfield is 2 bits long at bit position 25 in word 1
     * See VITA 49.2-2017-2017 Figure 9.4.5-1
     */
    void tsf(TSF value) noexcept
    {
        m_packed.set<25,2>(value);
    }

    /**
     * @brief Returns the Manufacturer OUI subfield value
     * @return Manufacturer OUI subfield value
     * 
     * Manufacturer OUI subfield is 24 bits long at bit position 23 in word 1
     * See VITA 49.2-2017-2017 Figure 9.10.1-1
     */
    uint32_t manufacturer_oui() const noexcept
    {
        return m_packed.get<23,24,uint32_t>();
    }

    void manufacturer_oui(uint32_t value) noexcept
    {
        m_packed.set<23,24>(value);
    }

    uint32_t integer_timestamp() const noexcept
    {
        return vrtgen::swap::from_be(m_integer_timestamp);
    }

    void integer_timestamp(uint32_t value) noexcept
    {
        m_integer_timestamp = vrtgen::swap::to_be(value);
    }

    uint64_t fractional_timestamp() const noexcept
    {
        return vrtgen::swap::from_be(m_fractional_timestamp);
    }

    void fractional_timestamp(uint64_t value) noexcept
    {
        m_fractional_timestamp = vrtgen::swap::to_be(value);
    }

    double position_x() const noexcept
    {
        return vrtgen::fixed::to_fp<32,5>(vrtgen::swap::from_be(m_position_x));
    }

    void position_x(double value) noexcept
    {
        m_position_x = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,5>(value));
    }

    double position_y() const noexcept
    {
        return vrtgen::fixed::to_fp<32,5>(vrtgen::swap::from_be(m_position_y));
    }

    void position_y(double value) noexcept
    {
        m_position_y = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,5>(value));
    }

    double position_z() const noexcept
    {
        return vrtgen::fixed::to_fp<32,5>(vrtgen::swap::from_be(m_position_z));
    }

    void position_z(double value) noexcept
    {
        m_position_z = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,5>(value));
    }

    double attitude_alpha() const noexcept
    {
        return vrtgen::fixed::to_fp<32,22>(vrtgen::swap::from_be(m_attitude_alpha));
    }

    void attitude_alpha(double value) noexcept
    {
        m_attitude_alpha = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,22>(value));
    }

    double attitude_beta() const noexcept
    {
        return vrtgen::fixed::to_fp<32,22>(vrtgen::swap::from_be(m_attitude_beta));
    }

    void attitude_beta(double value) noexcept
    {
        m_attitude_beta = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,22>(value));
    }

    double attitude_phi() const noexcept
    {
        return vrtgen::fixed::to_fp<32,22>(vrtgen::swap::from_be(m_attitude_phi));
    }

    void attitude_phi(double value) noexcept
    {
        m_attitude_phi = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,22>(value));
    }

    double velocity_dx() const noexcept
    {
        return vrtgen::fixed::to_fp<32,16>(vrtgen::swap::from_be(m_velocity_dx));
    }

    void velocity_dx(double value) noexcept
    {
        m_velocity_dx = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,16>(value));
    }

    double velocity_dy() const noexcept
    {
        return vrtgen::fixed::to_fp<32,16>(vrtgen::swap::from_be(m_velocity_dy));
    }

    void velocity_dy(double value) noexcept
    {
        m_velocity_dy = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,16>(value));
    }

    double velocity_dz() const noexcept
    {
        return vrtgen::fixed::to_fp<32,16>(vrtgen::swap::from_be(m_velocity_dz));
    }

    void velocity_dz(double value) noexcept
    {
        m_velocity_dz = vrtgen::swap::to_be(vrtgen::fixed::to_int<32,16>(value));
    }

    constexpr std::size_t size() const noexcept
    {
        return m_packed.size() +
               sizeof(m_integer_timestamp) +
               sizeof(m_fractional_timestamp) +
               sizeof(m_position_x) +
               sizeof(m_position_y) +
               sizeof(m_position_z) +
               sizeof(m_attitude_alpha) +
               sizeof(m_attitude_beta) +
               sizeof(m_attitude_phi) +
               sizeof(m_velocity_dx) +
               sizeof(m_velocity_dy) +
               sizeof(m_velocity_dz);
    }

    void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed.pack_into(buffer_ptr);
        buffer_ptr += m_packed.size();
        std::memcpy(buffer_ptr, &m_integer_timestamp, sizeof(m_integer_timestamp));
        buffer_ptr += sizeof(m_integer_timestamp);
        std::memcpy(buffer_ptr, &m_fractional_timestamp, sizeof(m_fractional_timestamp));
        buffer_ptr += sizeof(m_fractional_timestamp);
        std::memcpy(buffer_ptr, &m_position_x, sizeof(m_position_x));
        buffer_ptr += sizeof(m_position_x);
        std::memcpy(buffer_ptr, &m_position_y, sizeof(m_position_y));
        buffer_ptr += sizeof(m_position_y);
        std::memcpy(buffer_ptr, &m_position_z, sizeof(m_position_z));
        buffer_ptr += sizeof(m_position_z);
        std::memcpy(buffer_ptr, &m_attitude_alpha, sizeof(m_attitude_alpha));
        buffer_ptr += sizeof(m_attitude_alpha);
        std::memcpy(buffer_ptr, &m_attitude_beta, sizeof(m_attitude_beta));
        buffer_ptr += sizeof(m_attitude_beta);
        std::memcpy(buffer_ptr, &m_attitude_phi, sizeof(m_attitude_phi));
        buffer_ptr += sizeof(m_attitude_phi);
        std::memcpy(buffer_ptr, &m_velocity_dx, sizeof(m_velocity_dx));
        buffer_ptr += sizeof(m_velocity_dx);
        std::memcpy(buffer_ptr, &m_velocity_dy, sizeof(m_velocity_dy));
        buffer_ptr += sizeof(m_velocity_dy);
        std::memcpy(buffer_ptr, &m_velocity_dz, sizeof(m_velocity_dz));
    }

    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        m_packed.unpack_from(ptr);
        ptr += m_packed.size();
        std::memcpy(&m_integer_timestamp, ptr, sizeof(m_integer_timestamp));
        ptr += sizeof(m_integer_timestamp);
        std::memcpy(&m_fractional_timestamp, ptr, sizeof(m_fractional_timestamp));
        ptr += sizeof(m_fractional_timestamp);
        std::memcpy(&m_position_x, ptr, sizeof(m_position_x));
        ptr += sizeof(m_position_x);
        std::memcpy(&m_position_y, ptr, sizeof(m_position_y));
        ptr += sizeof(m_position_y);
        std::memcpy(&m_position_z, ptr, sizeof(m_position_z));
        ptr += sizeof(m_position_z);
        std::memcpy(&m_attitude_alpha, ptr, sizeof(m_attitude_alpha));
        ptr += sizeof(m_attitude_alpha);
        std::memcpy(&m_attitude_beta, ptr, sizeof(m_attitude_beta));
        ptr += sizeof(m_attitude_beta);
        std::memcpy(&m_attitude_phi, ptr, sizeof(m_attitude_phi));
        ptr += sizeof(m_attitude_phi);
        std::memcpy(&m_velocity_dx, ptr, sizeof(m_velocity_dx));
        ptr += sizeof(m_velocity_dx);
        std::memcpy(&m_velocity_dy, ptr, sizeof(m_velocity_dy));
        ptr += sizeof(m_velocity_dy);
        std::memcpy(&m_velocity_dz, ptr, sizeof(m_velocity_dz));
    }

private:
    vrtgen::packed<uint32_t> m_packed;
    uint32_t m_integer_timestamp{ vrtgen::swap::to_be(static_cast<uint32_t>(0xFFFFFFFF)) };
    uint64_t m_fractional_timestamp{ vrtgen::swap::to_be(static_cast<uint64_t>(0xFFFFFFFFFFFFFFFF)) };
    int32_t m_position_x{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };
    int32_t m_position_y{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };
    int32_t m_position_z{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };
    int32_t m_attitude_alpha{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };
    int32_t m_attitude_beta{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };
    int32_t m_attitude_phi{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };
    int32_t m_velocity_dx{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };
    int32_t m_velocity_dy{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };
    int32_t m_velocity_dz{ vrtgen::swap::to_be(static_cast<int32_t>(0x7FFFFFFF)) };

}; // end class Ephemeris

/**
 * @class GpsAscii
 * @brief GPS ASCII Field (9.4.7)
 */
class GpsAscii
{
public:

    /**
     * @brief Returns the Manufacturer OUI subfield value
     * @return Manufacturer OUI subfield value
     * 
     * Manufacturer OUI subfield is 24 bits long at bit position 23 in word 1
     * See VITA 49.2-2017 Figure 9.4.7-1
     */
    uint32_t manufacturer_oui() const noexcept
    {
        return m_packed.get<55,24,uint32_t>();
    }

    /**
     * @brief Sets the Manufacturer OUI subfield value
     * @param value Manufacturer OUI subfield value to set
     * 
     * Manufacturer OUI subfield is 24 bits long at bit position 23 in word 1
     * See VITA 49.2-2017 Figure 9.4.7-1
     */
    void manufacturer_oui(uint32_t value) noexcept
    {
        m_packed.set<55,24>(value);
    }

    /**
     * @brief Returns the Number of Words subfield value
     * @return Number of Words subfield value
     * 
     * Number of Words subfield is 32 bits long at bit position 31 in word 2
     * See VITA 49.2-2017 Figure 9.4.7-1
     */
    uint32_t number_of_words() const noexcept
    {
        return m_packed.get<31,32,uint32_t>();
    }

    /**
     * @brief Returns the ASCII Sentences subfield value
     * @return ASCII Sentences subfield value
     * 
     * ASCII Sentences subfield is a vector of 8 bits long values packed into 32 bit words 
     * starting at word 3 and going to word N+2
     * See VITA 49.2-2017 Figure 9.4.7-1
     */
    std::vector<uint8_t>& ascii_sentences() noexcept
    {
        return m_ascii_sentences;
    }

    /**
     * @brief Returns the ASCII Sentences subfield value
     * @return ASCII Sentences subfield value
     * 
     * ASCII Sentences subfield is a vector of 8 bits long values packed into 32 bit words 
     * starting at word 3 and going to word N+2
     * See VITA 49.2-2017 Figure 9.4.7-1
     */
    const std::vector<uint8_t>& ascii_sentences() const noexcept
    {
        return m_ascii_sentences;
    }

    /**
     * @brief Sets the ASCII Sentences subfield value
     * @param value ASCII Sentences subfield value to set
     * 
     * ASCII Sentences subfield is a vector of 8 bits long values packed into 32 bit words 
     * starting at word 3 and going to word N+2
     * See VITA 49.2-2017 Figure 9.4.7-1
     */
    void ascii_sentences(const std::vector<uint8_t>& value) noexcept
    {
        m_ascii_sentences = value;
        m_ascii_sentences.resize(m_ascii_sentences.size() + (m_ascii_sentences.size() % 4));
        number_of_words(m_ascii_sentences.size()/4);
    }

    /**
     * @brief Returns the number of GPS_ASCII bytes
     * @return Number of GPS_ASCII bytes
     */
    std::size_t size() const noexcept
    {
        return m_packed.size() +
               m_ascii_sentences.size();
    }

    /**
     * @brief Pack GPS_ASCII as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add GPS_ASCII bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed.pack_into(buffer_ptr);
        buffer_ptr += m_packed.size();
        std::copy(m_ascii_sentences.data(), m_ascii_sentences.data() + m_ascii_sentences.size(), buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into GPS_ASCII
     * @param buffer_ptr Pointer to beginning of GPS_ASCII bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto ptr = buffer_ptr;
        m_packed.unpack_from(ptr);
        ptr += m_packed.size();
        m_ascii_sentences.resize(number_of_words()*4);
        std::copy(ptr, ptr + number_of_words()*4, m_ascii_sentences.data());
    }

private:
    /**
     * @brief Sets the Number of Words subfield value
     * @param value Number of Words subfield value to set
     * 
     * Number of Words subfield is 32 bits long at bit position 31 in word 2
     * See VITA 49.2-2017 Figure 9.4.7-1
     */
    void number_of_words(uint32_t value) noexcept
    {
        m_packed.set<31,32>(value);
    }

    vrtgen::packed<uint64_t> m_packed;
    std::vector<uint8_t> m_ascii_sentences;

}; // end class GPS_ASCII

/**
 * @class PayloadFormat
 * @brief Signal Data Packet Payload Format Field (9.13.3)
 */
class PayloadFormat
{
public:
    PackingMethod packing_method() const noexcept
    {
        return PackingMethod{ m_packed.get<63>() };
    }

    void packing_method(PackingMethod value) noexcept
    {
        m_packed.set<63>(static_cast<bool>(value));
    }

    DataSampleType real_complex_type() const noexcept
    {
        return m_packed.get<62,2,DataSampleType>();
    }

    void real_complex_type(DataSampleType value) noexcept
    {
        m_packed.set<62,2>(value);
    }

    DataItemFormat data_item_format() const noexcept
    {
        return m_packed.get<60,5,DataItemFormat>();
    }

    void data_item_format(DataItemFormat value) noexcept
    {
        m_packed.set<60,5>(value);
    }

    bool repeat_indicator() const noexcept
    {
        return m_packed.get<55>();
    }

    void repeat_indicator(bool value) noexcept
    {
        m_packed.set<55>(value);
    }

    uint8_t event_tag_size() const noexcept
    {
        return m_packed.get<54,3,uint8_t>();
    }

    void event_tag_size(uint8_t value) noexcept
    {
        m_packed.set<54,3>(value);
    }

    uint8_t channel_tag_size() const noexcept
    {
        return m_packed.get<51,4,uint8_t>();
    }

    void channel_tag_size(uint8_t value) noexcept
    {
        m_packed.set<51,4>(value);
    }

    uint8_t data_item_fraction_size() const noexcept
    {
        return m_packed.get<47,4,uint8_t>();
    }

    void data_item_fraction_size(uint8_t value) noexcept
    {
        m_packed.set<47,4>(value);
    }

    uint8_t item_packing_field_size() const noexcept
    {
        return m_packed.get<43,6,uint8_t>() + 1;
    }

    void item_packing_field_size(uint8_t value) noexcept
    {
        m_packed.set<43,6,uint8_t>(value - 1);
    }

    uint8_t data_item_size() const noexcept
    {
        return m_packed.get<37,6,uint8_t>() + 1;
    }

    void data_item_size(uint8_t value) noexcept
    {
        m_packed.set<37,6,uint8_t>(value - 1);
    }

    uint16_t repeat_count() const noexcept
    {
        return m_packed.get<31,16,uint16_t>() + 1;
    }

    void repeat_count(uint16_t value) noexcept
    {
        m_packed.set<31,16,uint16_t>(value - 1);
    }

    uint16_t vector_size() const noexcept
    {
        return m_packed.get<15,16,uint16_t>() + 1;
    }

    void vector_size(uint16_t value) noexcept
    {
        m_packed.set<15,16,uint16_t>(value - 1);
    }

    /**
     * @brief Returns the number of PayloadFormat bytes
     * @return Number of PayloadFormat bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return m_packed.size();
    }

    /**
     * @brief Pack PayloadFormat as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add PayloadFormat bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into PayloadFormat
     * @param buffer_ptr Pointer to beginning of PayloadFormat bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        m_packed.unpack_from(buffer_ptr);
    }

private:
    vrtgen::packed<uint64_t> m_packed;

}; // end class PayloadFormat

/**
 * @class ContextAssociationLists
 * @brief Context Association Lists (9.13.2)
 */
class ContextAssociationLists
{
public:
    uint16_t source_list_size() const noexcept
    {
        return m_packed.get<56,9,uint16_t>();
    }

    void source_list_size(uint16_t value) noexcept
    {
        m_packed.set<56,9>(value);
    }

    uint16_t system_list_size() const noexcept
    {
        return m_packed.get<40,9,uint16_t>();
    }

    void system_list_size(uint16_t value) noexcept
    {
        m_packed.set<40,9>(value);
    }

    uint16_t vector_component_list_size() const noexcept
    {
        return m_packed.get<31,16,uint16_t>();
    }

    void vector_component_list_size(uint16_t value) noexcept
    {
        m_packed.set<31,16>(value);
    }

    bool async_channel_tag_list_enable() const noexcept
    {
        return m_packed.get<15>();
    }

    void async_channel_tag_list_enable(bool value) noexcept
    {
        m_packed.set<15>(value);
    }

    uint16_t async_channel_list_size() const noexcept
    {
        return m_packed.get<14,15,uint16_t>();
    }

    void async_channel_list_size(uint16_t value) noexcept
    {
        m_packed.set<14,15>(value);
    }

    std::vector<uint32_t>& source_list() noexcept
    {
        return m_source_list;
    }

    const std::vector<uint32_t>& source_list() const noexcept
    {
        return m_source_list;
    }

    void source_list(const std::vector<uint32_t>& value) noexcept
    {
        m_source_list = value;
    }

    std::vector<uint32_t>& system_list() noexcept
    {
        return m_system_list;
    }

    const std::vector<uint32_t>& system_list() const noexcept
    {
        return m_system_list;
    }

    void system_list(const std::vector<uint32_t>& value) noexcept
    {
        m_system_list = value;
    }

    std::vector<uint32_t>& vector_component_list() noexcept
    {
        return m_vector_component_list;
    }

    const std::vector<uint32_t>& vector_component_list() const noexcept
    {
        return m_vector_component_list;
    }

    void vector_component_list(const std::vector<uint32_t>& value) noexcept
    {
        m_vector_component_list = value;
    }

    std::vector<uint32_t>& async_channel_list() noexcept
    {
        return m_async_channel_list;
    }

    const std::vector<uint32_t>& async_channel_list() const noexcept
    {
        return m_async_channel_list;
    }

    void async_channel_list(const std::vector<uint32_t>& value) noexcept
    {
        m_async_channel_list = value;
    }

    std::vector<uint32_t>& async_channel_tag_list() noexcept
    {
        return m_async_channel_tag_list;
    }

    const std::vector<uint32_t>& async_channel_tag_list() const noexcept
    {
        return m_async_channel_tag_list;
    }

    void async_channel_tag_list(const std::vector<uint32_t>& value) noexcept
    {
        m_async_channel_tag_list = value;
    }

    /**
     * @brief Returns the number of ContextAssociationLists bytes
     * @return Number of ContextAssociationLists bytes
     */
    std::size_t size() const noexcept
    {
        std::size_t retval = 0;
        retval += m_packed.size();
        if (!m_source_list.empty()) {
            retval += m_source_list.size() * sizeof(uint32_t);
        }
        if (!m_system_list.empty()) {
            retval += m_system_list.size() * sizeof(uint32_t);
        }
        if (!m_vector_component_list.empty()) {
            retval += m_vector_component_list.size() * sizeof(uint32_t);
        }
        if (!m_async_channel_list.empty()) {
            retval += m_async_channel_list.size() * sizeof(uint32_t);
        }
        if (!m_async_channel_tag_list.empty()) {
            retval += m_async_channel_tag_list.size() * sizeof(uint32_t);
        }
        return retval;
    }

    /**
     * @brief Pack ContextAssociationLists as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add ContextAssociationLists bytes
     */
    void pack_into(uint8_t* buffer_ptr)
    {
        source_list_size(m_source_list.size());
        system_list_size(m_system_list.size());
        vector_component_list_size(m_vector_component_list.size());
        async_channel_list_size(m_async_channel_list.size());
        m_packed.pack_into(buffer_ptr);
        buffer_ptr += m_packed.size();
        for (const auto elem : m_source_list) {
            std::memcpy(buffer_ptr, &elem, sizeof(elem));
            buffer_ptr += sizeof(elem);
        }
        for (const auto elem : m_system_list) {
            std::memcpy(buffer_ptr, &elem, sizeof(elem));
            buffer_ptr += sizeof(elem);
        }
        for (const auto elem : m_vector_component_list) {
            std::memcpy(buffer_ptr, &elem, sizeof(elem));
            buffer_ptr += sizeof(elem);
        }
        for (const auto elem : m_async_channel_list) {
            std::memcpy(buffer_ptr, &elem, sizeof(elem));
            buffer_ptr += sizeof(elem);
        }
        for (const auto elem : m_async_channel_tag_list) {
            std::memcpy(buffer_ptr, &elem, sizeof(elem));
            buffer_ptr += sizeof(elem);
        }
    }

    /**
     * @brief Unpack buffer bytes into ContextAssociationLists
     * @param buffer_ptr Pointer to beginning of ContextAssociationLists bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        m_packed.unpack_from(ptr);
        ptr += m_packed.size();
        m_source_list.resize(source_list_size());
        for (std::size_t i=0; i<m_source_list.size(); ++i) {
            std::memcpy(m_source_list.data() + i, ptr, sizeof(uint32_t));
            ptr += sizeof(uint32_t);
        }
        m_system_list.resize(system_list_size());
        for (std::size_t i=0; i<m_system_list.size(); ++i) {
            std::memcpy(m_system_list.data() + i, ptr, sizeof(uint32_t));
            ptr += sizeof(uint32_t);
        }
        m_vector_component_list.resize(vector_component_list_size());
        for (std::size_t i=0; i<m_vector_component_list.size(); ++i) {
            std::memcpy(m_vector_component_list.data() + i, ptr, sizeof(uint32_t));
            ptr += sizeof(uint32_t);
        }
        m_async_channel_list.resize(async_channel_list_size());
        for (std::size_t i=0; i<m_async_channel_list.size(); ++i) {
            std::memcpy(m_async_channel_list.data() + i, ptr, sizeof(uint32_t));
            ptr += sizeof(uint32_t);
        }
        m_async_channel_tag_list.resize(async_channel_list_size());
        for (std::size_t i=0; i<m_async_channel_tag_list.size(); ++i) {
            std::memcpy(m_async_channel_tag_list.data() + i, ptr, sizeof(uint32_t));
            ptr += sizeof(uint32_t);
        }
    }

private:
    vrtgen::packed<uint64_t> m_packed;
    std::vector<uint32_t> m_source_list;
    std::vector<uint32_t> m_system_list;
    std::vector<uint32_t> m_vector_component_list;
    std::vector<uint32_t> m_async_channel_list;
    std::vector<uint32_t> m_async_channel_tag_list;

}; // end class ContextAssociationLists

} // end namespace vrtgen::packing
