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

/**
 * @file vrtgen/packing/indicator_fields.hpp
 * @brief Packing structures for VRT Indicator Field words.
 */

#ifndef _VRTGEN_PACKING_INDICATOR_FIELDS_HPP
#define _VRTGEN_PACKING_INDICATOR_FIELDS_HPP

#include <vrtgen/types/packed.hpp>

namespace vrtgen::packing {

/**
 * @class IndicatorField
 * @brief Base class for Indicator Field Words
 */
class IndicatorField
{
public:
    /**
     * @brief Checks if any of the IndicatorField bits are set to true
     * @return true if any of the bits are set to true, otherwise false
     */
    bool any() const noexcept
    {
        return m_packed.any();
    }

    /**
     * @brief Checks if none of the IndicatorField bits are set to true
     * @return true if none of the bits are set to true, otherwise false
     */
    bool none() const noexcept
    {
        return m_packed.none();
    }

    /**
     * @brief Returns the number of IndicatorField bytes
     * @return Number of IndicatorField bytes
     */
    constexpr size_t size() const noexcept
    {
        return m_packed.size();
    }

    /**
     * @brief Pack IndicatorField as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add IndicatorField bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into IndicatorField
     * @param buffer_ptr Pointer to beginning of IndicatorField bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        m_packed.unpack_from(buffer_ptr);
    }

protected:
    vrtgen::packed<uint32_t> m_packed;

}; // end class IndicatorField

/**
 * @class IndicatorField0
 * @brief Base class for Indicator Field Word 0
 */
class IndicatorField0 : public IndicatorField
{
public:
    /**
     * @brief Returns the Context Field Change Indicator flag
     * @return true if at least one Context fields contains a new value, otherwise false
     * 
     * Context Field Change Indicator is 1 bit long at bit position 31
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool change_indicator() const noexcept
    {
        return m_packed.get<31>();
    }

    /**
     * @brief Sets the Context Field Change Indicator flag
     * @param value Context Field Change Indicator flag value to set
     * 
     * Context Field Change Indicator is 1 bit long at bit position 31
     * See VITA 49.2-2017 Table 9.1-1
     */
    void change_indicator(bool value) noexcept
    {
        m_packed.set<31>(value);
    }

    /**
     * @brief Returns the Reference Point ID enable flag
     * @return true if packet contains Reference Point ID field, otherwise false
     * 
     * Reference Point ID is 1 bit long at bit position 30
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool reference_point_id() const noexcept
    {
        return m_packed.get<30>();
    }

    /**
     * @brief Sets the Reference Point ID enable flag
     * @param value Reference Point ID enable flag value to set
     * 
     * Reference Point ID is 1 bit long at bit position 30
     * See VITA 49.2-2017 Table 9.1-1
     */
    void reference_point_id(bool value) noexcept
    {
        m_packed.set<30>(value);
    }

    /**
     * @brief Returns the Bandwidth enable flag
     * @return true if packet contains Bandwidth field, otherwise false
     * 
     * Bandwidth is 1 bit long at bit position 29
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool bandwidth() const noexcept
    {
        return m_packed.get<29>();
    }

    /**
     * @brief Sets the Bandwidth enable flag
     * @param value Bandwidth enable flag value to set
     * 
     * Bandwidth is 1 bit long at bit position 29
     * See VITA 49.2-2017 Table 9.1-1
     */
    void bandwidth(bool value) noexcept
    {
        m_packed.set<29>(value);
    }

    /**
     * @brief Returns the IF Reference Frequency enable flag
     * @return true if packet contains IF Reference Frequency field, otherwise false
     * 
     * IF Reference Frequency is 1 bit long at bit position 28
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool if_ref_frequency() const noexcept
    {
        return m_packed.get<28>();
    }

    /**
     * @brief Sets the IF Reference Frequency enable flag
     * @param value IF Reference Frequency enable flag value to set
     * 
     * IF Reference Frequency is 1 bit long at bit position 28
     * See VITA 49.2-2017 Table 9.1-1
     */
    void if_ref_frequency(bool value) noexcept
    {
        m_packed.set<28>(value);
    }

    /**
     * @brief Returns the RF Reference Frequency enable flag
     * @return true if packet contains RF Reference Frequency field, otherwise false
     * 
     * RF Reference Frequency is 1 bit long at bit position 27
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool rf_ref_frequency() const noexcept
    {
        return m_packed.get<27>();
    }

    /**
     * @brief Sets the RF Reference Frequency enable flag
     * @param value RF Reference Frequency enable flag value to set
     * 
     * RF Reference Frequency is 1 bit long at bit position 27
     * See VITA 49.2-2017 Table 9.1-1
     */
    void rf_ref_frequency(bool value) noexcept
    {
        m_packed.set<27>(value);
    }

    /**
     * @brief Returns the RF Reference Frequency Offset enable flag
     * @return true if packet contains RF Reference Frequency Offset field, otherwise false
     * 
     * RF Reference Frequency Offset is 1 bit long at bit position 26
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool rf_ref_frequency_offset() const noexcept
    {
        return m_packed.get<26>();
    }

    /**
     * @brief Sets the RF Reference Frequency Offset enable flag
     * @param value RF Reference Frequency Offset enable flag value to set
     * 
     * RF Reference Frequency Offset is 1 bit long at bit position 26
     * See VITA 49.2-2017 Table 9.1-1
     */
    void rf_ref_frequency_offset(bool value) noexcept
    {
        m_packed.set<26>(value);
    }

    /**
     * @brief Returns the IF Band Offset enable flag
     * @return true if packet contains IF Band Offset field, otherwise false
     * 
     * IF Band Offset is 1 bit long at bit position 25
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool if_band_offset() const noexcept
    {
        return m_packed.get<25>();
    }

    /**
     * @brief Sets the IF Band Offset enable flag
     * @param value IF Band Offset enable flag value to set
     * 
     * IF Band Offset is 1 bit long at bit position 25
     * See VITA 49.2-2017 Table 9.1-1
     */
    void if_band_offset(bool value) noexcept
    {
        m_packed.set<25>(value);
    }

    /**
     * @brief Returns the Reference Level enable flag
     * @return true if packet contains Reference Level field, otherwise false
     * 
     * Reference Level is 1 bit long at bit position 24
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool reference_level() const noexcept
    {
        return m_packed.get<24>();
    }

    /**
     * @brief Sets the Reference Level enable flag
     * @param value Reference Level enable flag value to set
     * 
     * Reference Level is 1 bit long at bit position 24
     * See VITA 49.2-2017 Table 9.1-1
     */
    void reference_level(bool value) noexcept
    {
        m_packed.set<24>(value);
    }

    /**
     * @brief Returns the Gain enable flag
     * @return true if packet contains Gain field, otherwise false
     * 
     * Gain is 1 bit long at bit position 23
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool gain() const noexcept
    {
        return m_packed.get<23>();
    }

    /**
     * @brief Sets the Gain enable flag
     * @param value Gain enable flag value to set
     * 
     * Gain is 1 bit long at bit position 23
     * See VITA 49.2-2017 Table 9.1-1
     */
    void gain(bool value) noexcept
    {
        m_packed.set<23>(value);
    }

    /**
     * @brief Returns the Over-Range Count enable flag
     * @return true if packet contains Over-Range Count field, otherwise false
     * 
     * Over-Range Count is 1 bit long at bit position 22
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool over_range_count() const noexcept
    {
        return m_packed.get<22>();
    }

    /**
     * @brief Sets the Over-Range Count enable flag
     * @param value Over-Range Count enable flag value to set
     * 
     * Over-Range Count is 1 bit long at bit position 22
     * See VITA 49.2-2017 Table 9.1-1
     */
    void over_range_count(bool value) noexcept
    {
        m_packed.set<22>(value);
    }

    /**
     * @brief Returns the Sample Rate enable flag
     * @return true if packet contains Sample Rate field, otherwise false
     * 
     * Sample Rate is 1 bit long at bit position 21
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool sample_rate() const noexcept
    {
        return m_packed.get<21>();
    }

    /**
     * @brief Sets the Sample Rate enable flag
     * @param value Sample Rate enable flag value to set
     * 
     * Sample Rate is 1 bit long at bit position 21
     * See VITA 49.2-2017 Table 9.1-1
     */
    void sample_rate(bool value) noexcept
    {
        m_packed.set<21>(value);
    }

    /**
     * @brief Returns the Timestamp Adjustment enable flag
     * @return true if packet contains Timestamp Adjustment field, otherwise false
     * 
     * Timestamp Adjustment is 1 bit long at bit position 20
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool timestamp_adjustment() const noexcept
    {
        return m_packed.get<20>();
    }

    /**
     * @brief Sets the Timestamp Adjustment enable flag
     * @param value Timestamp Adjustment enable flag value to set
     * 
     * Timestamp Adjustment is 1 bit long at bit position 20
     * See VITA 49.2-2017 Table 9.1-1
     */
    void timestamp_adjustment(bool value) noexcept
    {
        m_packed.set<20>(value);
    }

    /**
     * @brief Returns the Timestamp Calibration Time enable flag
     * @return true if packet contains Timestamp Calibration Time field, otherwise false
     * 
     * Timestamp Calibration Time is 1 bit long at bit position 19
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool timestamp_calibration_time() const noexcept
    {
        return m_packed.get<19>();
    }

    /**
     * @brief Sets the Timestamp Calibration Time enable flag
     * @param value Timestamp Calibration Time enable flag value to set
     * 
     * Timestamp Calibration Time is 1 bit long at bit position 19
     * See VITA 49.2-2017 Table 9.1-1
     */
    void timestamp_calibration_time(bool value) noexcept
    {
        m_packed.set<19>(value);
    }

    /**
     * @brief Returns the Temperature enable flag
     * @return true if packet contains Temperature field, otherwise false
     * 
     * Temperature is 1 bit long at bit position 18
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool temperature() const noexcept
    {
        return m_packed.get<18>();
    }

    /**
     * @brief Sets the Temperature enable flag
     * @param value Temperature enable flag value to set
     * 
     * Temperature is 1 bit long at bit position 18
     * See VITA 49.2-2017 Table 9.1-1
     */
    void temperature(bool value) noexcept
    {
        m_packed.set<18>(value);
    }

    /**
     * @brief Returns the Device Identifier enable flag
     * @return true if packet contains Device Identifier field, otherwise false
     * 
     * Device Identifier is 1 bit long at bit position 17
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool device_id() const noexcept
    {
        return m_packed.get<17>();
    }

    /**
     * @brief Sets the Device Identifier enable flag
     * @param value Device Identifier enable flag value to set
     * 
     * Device Identifier is 1 bit long at bit position 17
     * See VITA 49.2-2017 Table 9.1-1
     */
    void device_id(bool value) noexcept
    {
        m_packed.set<17>(value);
    }

    /**
     * @brief Returns the State/Event Indicators enable flag
     * @return true if packet contains State/Event Indicators field, otherwise false
     * 
     * State/Event Indicators is 1 bit long at bit position 16
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool state_event_indicators() const noexcept
    {
        return m_packed.get<16>();
    }

    /**
     * @brief Sets the State/Event Indicators enable flag
     * @param value State/Event Indicators enable flag value to set
     * 
     * State/Event Indicators is 1 bit long at bit position 16
     * See VITA 49.2-2017 Table 9.1-1
     */
    void state_event_indicators(bool value) noexcept
    {
        m_packed.set<16>(value);
    }

    /**
     * @brief Returns the Signal Data Packet Payload Format enable flag
     * @return true if packet contains Signal Data Packet Payload Format field, otherwise false
     * 
     * Signal Data Packet Payload Format is 1 bit long at bit position 15
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool signal_data_format() const noexcept
    {
        return m_packed.get<15>();
    }

    /**
     * @brief Sets the Signal Data Packet Payload Format enable flag
     * @param value Signal Data Packet Payload Format enable flag value to set
     * 
     * Signal Data Packet Payload Format is 1 bit long at bit position 15
     * See VITA 49.2-2017 Table 9.1-1
     */
    void signal_data_format(bool value) noexcept
    {
        m_packed.set<15>(value);
    }

    /**
     * @brief Returns the Formatted GPS enable flag
     * @return true if packet contains Formatted GPS field, otherwise false
     * 
     * Formatted GPS is 1 bit long at bit position 14
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool formatted_gps() const noexcept
    {
        return m_packed.get<14>();
    }

    /**
     * @brief Sets the Formatted GPS enable flag
     * @param value Formatted GPS enable flag value to set
     * 
     * Formatted GPS is 1 bit long at bit position 14
     * See VITA 49.2-2017 Table 9.1-1
     */
    void formatted_gps(bool value) noexcept
    {
        m_packed.set<14>(value);
    }

    /**
     * @brief Returns the Formatted INS enable flag
     * @return true if packet contains Formatted INS field, otherwise false
     * 
     * Formatted INS is 1 bit long at bit position 13
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool formatted_ins() const noexcept
    {
        return m_packed.get<13>();
    }

    /**
     * @brief Sets the Formatted INS enable flag
     * @param value Formatted INS enable flag value to set
     * 
     * Formatted INS is 1 bit long at bit position 13
     * See VITA 49.2-2017 Table 9.1-1
     */
    void formatted_ins(bool value) noexcept
    {
        m_packed.set<13>(value);
    }

    /**
     * @brief Returns the ECEF Ephemeris enable flag
     * @return true if packet contains ECEF Ephemeris field, otherwise false
     * 
     * ECEF Ephemeris is 1 bit long at bit position 12
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool ecef_ephemeris() const noexcept
    {
        return m_packed.get<12>();
    }

    /**
     * @brief Sets the ECEF Ephemeris enable flag
     * @param value ECEF Ephemeris enable flag value to set
     * 
     * ECEF Ephemeris is 1 bit long at bit position 12
     * See VITA 49.2-2017 Table 9.1-1
     */
    void ecef_ephemeris(bool value) noexcept
    {
        m_packed.set<12>(value);
    }

    /**
     * @brief Returns the Relative Ephemeris enable flag
     * @return true if packet contains Relative Ephemeris field, otherwise false
     * 
     * Relative Ephemeris is 1 bit long at bit position 11
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool relative_ephemeris() const noexcept
    {
        return m_packed.get<11>();
    }

    /**
     * @brief Sets the Relative Ephemeris enable flag
     * @param value Relative Ephemeris enable flag value to set
     * 
     * Relative Ephemeris is 1 bit long at bit position 11
     * See VITA 49.2-2017 Table 9.1-1
     */
    void relative_ephemeris(bool value) noexcept
    {
        m_packed.set<11>(value);
    }

    /**
     * @brief Returns the Ephemeris Ref ID enable flag
     * @return true if packet contains Ephemeris Ref ID field, otherwise false
     * 
     * Ephemeris Ref ID is 1 bit long at bit position 10
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool ephemeris_ref_id() const noexcept
    {
        return m_packed.get<10>();
    }

    /**
     * @brief Sets the Ephemeris Ref ID enable flag
     * @param value Ephemeris Ref ID enable flag value to set
     * 
     * Ephemeris Ref ID is 1 bit long at bit position 10
     * See VITA 49.2-2017 Table 9.1-1
     */
    void ephemeris_ref_id(bool value) noexcept
    {
        m_packed.set<10>(value);
    }

    /**
     * @brief Returns the GPS ASCII enable flag
     * @return true if packet contains GPS ASCII field, otherwise false
     * 
     * GPS ASCII is 1 bit long at bit position 9
     * See VITA 49.2 Table 9.1-1
     */
    bool gps_ascii() const noexcept
    {
        return m_packed.get<9>();
    }

    /**
     * @brief Sets the GPS ASCII enable flag
     * @param value GPS ASCII enable flag value to set
     * 
     * GPS ASCII is 1 bit long at bit position 9
     * See VITA 49.2 Table 9.1-1
     */
    void gps_ascii(bool value) noexcept
    {
        m_packed.set<9>(value);
    }

    /**
     * @brief Returns the Context Assocations List enable flag
     * @return true if packet contains Context Assocations List field, otherwise false
     * 
     * Context Assocations List is 1 bit long at bit position 8
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool context_association_lists() const noexcept
    {
        return m_packed.get<8>();
    }

    /**
     * @brief Sets the Context Assocations List enable flag
     * @param value Context Assocations List enable flag value to set
     * 
     * Context Assocations List is 1 bit long at bit position 8
     * See VITA 49.2-2017 Table 9.1-1
     */
    void context_association_lists(bool value) noexcept
    {
        m_packed.set<8>(value);
    }
     
protected:
    /**
     * @brief Indicator Field 7 enable flag
     * @return true if packet contains Indicator Field 7 word, otherwise false
     * 
     * Indicator Field 7 Enable is 1 bit long at bit position 7
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool if7_enable() const noexcept
    {
        return m_packed.get<7>();
    }

    /**
     * @brief Sets the Indicator Field 7 enable flag
     * @param value Indicator Field 7 enable flag value to set
     * 
     * Indicator Field 7 Enable is 1 bit long at bit position 7
     * See VITA 49.2-2017 Table 9.1-1
     */
    void if7_enable(bool value) noexcept
    {
        m_packed.set<7>(value);
    }

    /**
     * @brief Indicator Field 3 enable flag
     * @return true if packet contains Indicator Field 3 word, otherwise false
     * 
     * Indicator Field 3 Enable is 1 bit long at bit position 3
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool if3_enable() const noexcept
    {
        return m_packed.get<3>();
    }

    /**
     * @brief Sets the Indicator Field 3 enable flag
     * @param value Indicator Field 3 enable flag value to set
     * 
     * Indicator Field 3 Enable is 1 bit long at bit position 3
     * See VITA 49.2-2017 Table 9.1-1
     */
    void if3_enable(bool value) noexcept
    {
        m_packed.set<3>(value);
    }

    /**
     * @brief Indicator Field 2 enable flag
     * @return true if packet contains Indicator Field 2 word, otherwise false
     * 
     * Indicator Field 2 Enable is 1 bit long at bit position 2
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool if2_enable() const noexcept
    {
        return m_packed.get<2>();
    }

    /**
     * @brief Sets the Indicator Field 2 enable flag
     * @param value Indicator Field 2 enable flag value to set
     * 
     * Indicator Field 2 Enable is 1 bit long at bit position 2
     * See VITA 49.2-2017 Table 9.1-1
     */
    void if2_enable(bool value) noexcept
    {
        m_packed.set<2>(value);
    }

    /**
     * @brief Indicator Field 1 enable flag
     * @return true if packet contains Indicator Field 1 word, otherwise false
     * 
     * Indicator Field 1 Enable is 1 bit long at bit position 1
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool if1_enable() const noexcept
    {
        return m_packed.get<1>();
    }

    /**
     * @brief Sets the Indicator Field 1 enable flag
     * @param value Indicator Field 1 enable flag value to set
     * 
     * Indicator Field 1 Enable is 1 bit long at bit position 1
     * See VITA 49.2-2017 Table 9.1-1
     */
    void if1_enable(bool value) noexcept
    {
        m_packed.set<1>(value);
    }

}; // end class IndicatorField0

/**
 * @class IndicatorField1
 * @brief Base class for Indicator Field Word 1
 */
class IndicatorField1 : public IndicatorField
{
public:
    /**
     * @brief Returns the Phase Offset enable flag
     * @return true if packet contains Phase Offset field, otherwise false
     * 
     * Phase Offset is 1 bit long at bit position 31
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool phase_offset() const noexcept
    {
        return m_packed.get<31>();
    }

    /**
     * @brief Sets the Phase Offset enable flag
     * @param value Phase Offset enable flag value to set
     * 
     * Phase Offset is 1 bit long at bit position 31
     * See VITA 49.2-2017 Table 9.1-1
     */
    void phase_offset(bool value) noexcept
    {
        m_packed.set<31>(value);
    }

    /**
     * @brief Returns the Polarization enable flag
     * @return true if packet contains Polarization field, otherwise false
     * 
     * Polarization is 1 bit long at bit position 30
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool polarization() const noexcept
    {
        return m_packed.get<30>();
    }

    /**
     * @brief Sets the Polarization enable flag
     * @param value Polarization enable flag value to set
     * 
     * Polarization is 1 bit long at bit position 30
     * See VITA 49.2-2017 Table 9.1-1
     */
    void polarization(bool value) noexcept
    {
        m_packed.set<30>(value);
    }

    /**
     * @brief Returns the Pointing Vector enable flag
     * @return true if packet contains Pointing Vector field, otherwise false
     * 
     * Pointing Vector is 1 bit long at bit position 29
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool pointing_vector() const noexcept
    {
        return m_packed.get<29>();
    }

    /**
     * @brief Sets the Pointing Vector enable flag
     * @param value Pointing Vector enable flag value to set
     * 
     * Pointing Vector is 1 bit long at bit position 29
     * See VITA 49.2-2017 Table 9.1-1
     */
    void pointing_vector(bool value) noexcept
    {
        m_packed.set<29>(value);
    }

    /**
     * @brief Returns the Pointing Vector Structure enable flag
     * @return true if packet contains Pointing Vector field, otherwise false
     * 
     * Pointing Vector Structure is 1 bit long at bit position 28
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool pointing_vector_structure() const noexcept
    {
        return m_packed.get<28>();
    }

    /**
     * @brief Sets the Pointing Vector Structure enable flag
     * @param value Pointing Vector Structure enable flag value to set
     * 
     * Pointing Vector Structure is 1 bit long at bit position 28
     * See VITA 49.2-2017 Table 9.1-1
     */
    void pointing_vector_structure(bool value) noexcept
    {
        m_packed.set<28>(value);
    }

    /**
     * @brief Returns the Spatial Scan Type enable flag
     * @return true if packet contains Spatial Scan Type field, otherwise false
     * 
     * Spatial Scan Type is 1 bit long at bit position 27
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool spatial_scan_type() const noexcept
    {
        return m_packed.get<27>();
    }

    /**
     * @brief Sets the Spatial Scan Type enable flag
     * @param value Spatial Scan Type enable flag value to set
     * 
     * Spatial Scan Type is 1 bit long at bit position 27
     * See VITA 49.2-2017 Table 9.1-1
     */
    void spatial_scan_type(bool value) noexcept
    {
        m_packed.set<27>(value);
    }

    /**
     * @brief Returns the Spatial Reference Type enable flag
     * @return true if packet contains Spatial Reference Type field, otherwise false
     * 
     * Spatial Reference Type is 1 bit long at bit position 26
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool spatial_reference_type() const noexcept
    {
        return m_packed.get<26>();
    }

    /**
     * @brief Sets the Spatial Reference Type enable flag
     * @param value Spatial Reference Type enable flag value to set
     * 
     * Spatial Reference Type is 1 bit long at bit position 26
     * See VITA 49.2-2017 Table 9.1-1
     */
    void spatial_reference_type(bool value) noexcept
    {
        m_packed.set<26>(value);
    }

    /**
     * @brief Returns the Beam Width enable flag
     * @return true if packet contains Beam Width field, otherwise false
     * 
     * Beam Width is 1 bit long at bit position 25
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool beam_width() const noexcept
    {
        return m_packed.get<25>();
    }

    /**
     * @brief Sets the Beam Width enable flag
     * @param value Beam Width enable flag value to set
     * 
     * Beam Width is 1 bit long at bit position 25
     * See VITA 49.2-2017 Table 9.1-1
     */
    void beam_width(bool value) noexcept
    {
        m_packed.set<25>(value);
    }

    /**
     * @brief Returns the Range enable flag
     * @return true if packet contains Range field, otherwise false
     * 
     * Range is 1 bit long at bit position 24
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool range() const noexcept
    {
        return m_packed.get<24>();
    }

    /**
     * @brief Sets the Range enable flag
     * @param value Range enable flag value to set
     * 
     * Range is 1 bit long at bit position 24
     * See VITA 49.2-2017 Table 9.1-1
     */
    void range(bool value) noexcept
    {
        m_packed.set<24>(value);
    }

    /**
     * @brief Returns the Eb/No BER enable flag
     * @return true if packet contains Eb/No BER field, otherwise false
     * 
     * Eb/No BER is 1 bit long at bit position 20
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool ebno_ber() const noexcept
    {
        return m_packed.get<20>();
    }

    /**
     * @brief Sets the Eb/No BER enable flag
     * @param value Eb/No BER enable flag value to set
     * 
     * Eb/No BER is 1 bit long at bit position 20
     * See VITA 49.2-2017 Table 9.1-1
     */
    void ebno_ber(bool value) noexcept
    {
        m_packed.set<20>(value);
    }

    /**
     * @brief Returns the Threshold enable flag
     * @return true if packet contains Threshold field, otherwise false
     * 
     * Threshold is 1 bit long at bit position 19
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool threshold() const noexcept
    {
        return m_packed.get<19>();
    }

    /**
     * @brief Sets the Threshold enable flag
     * @param value Threshold enable flag value to set
     * 
     * Threshold is 1 bit long at bit position 19
     * See VITA 49.2-2017 Table 9.1-1
     */
    void threshold(bool value) noexcept
    {
        m_packed.set<19>(value);
    }

    /**
     * @brief Returns the Compression Point enable flag
     * @return true if packet contains Compression Point field, otherwise false
     * 
     * Compression Point is 1 bit long at bit position 18
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool compression_point() const noexcept
    {
        return m_packed.get<18>();
    }

    /**
     * @brief Sets the Compression Point enable flag
     * @param value Compression Point enable flag value to set
     * 
     * Compression Point is 1 bit long at bit position 18
     * See VITA 49.2-2017 Table 9.1-1
     */
    void compression_point(bool value) noexcept
    {
        m_packed.set<18>(value);
    }

    /**
     * @brief Returns the Intercept Points enable flag
     * @return true if packet contains Intercept Points field, otherwise false
     * 
     * Intercept Points is 1 bit long at bit position 17
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool intercept_points() const noexcept
    {
        return m_packed.get<17>();
    }

    /**
     * @brief Sets the Intercept Points enable flag
     * @param value Intercept Points enable flag value to set
     * 
     * Intercept Points is 1 bit long at bit position 17
     * See VITA 49.2-2017 Table 9.1-1
     */
    void intercept_points(bool value) noexcept
    {
        m_packed.set<17>(value);
    }

    /**
     * @brief Returns the SNR/Noise Figure enable flag
     * @return true if packet contains SNR/Noise Figure field, otherwise false
     * 
     * SNR/Noise Figure is 1 bit long at bit position 16
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool snr_noise_figure() const noexcept
    {
        return m_packed.get<16>();
    }

    /**
     * @brief Sets the SNR/Noise Figure enable flag
     * @param value SNR/Noise Figure enable flag value to set
     * 
     * SNR/Noise Figure is 1 bit long at bit position 16
     * See VITA 49.2-2017 Table 9.1-1
     */
    void snr_noise_figure(bool value) noexcept
    {
        m_packed.set<16>(value);
    }

    /**
     * @brief Returns the Aux Frequency enable flag
     * @return true if packet contains Aux Frequency field, otherwise false
     * 
     * Aux Frequency is 1 bit long at bit position 15
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool aux_frequency() const noexcept
    {
        return m_packed.get<15>();
    }

    /**
     * @brief Sets the Aux Frequency enable flag
     * @param value Aux Frequency enable flag value to set
     * 
     * Aux Frequency is 1 bit long at bit position 15
     * See VITA 49.2-2017 Table 9.1-1
     */
    void aux_frequency(bool value) noexcept
    {
        m_packed.set<15>(value);
    }

    /**
     * @brief Returns the Aux Gain enable flag
     * @return true if packet contains Aux Gain field, otherwise false
     * 
     * Aux Gain is 1 bit long at bit position 14
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool aux_gain() const noexcept
    {
        return m_packed.get<14>();
    }

    /**
     * @brief Sets the Aux Gain enable flag
     * @param value Aux Gain enable flag value to set
     * 
     * Aux Gain is 1 bit long at bit position 14
     * See VITA 49.2-2017 Table 9.1-1
     */
    void aux_gain(bool value) noexcept
    {
        m_packed.set<14>(value);
    }

    /**
     * @brief Returns the Aux Bandwidth enable flag
     * @return true if packet contains Aux Bandwidth field, otherwise false
     * 
     * Aux Bandwidth is 1 bit long at bit position 13
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool aux_bandwidth() const noexcept
    {
        return m_packed.get<13>();
    }

    /**
     * @brief Sets the Aux Bandwidth enable flag
     * @param value Aux Bandwidth enable flag value to set
     * 
     * Aux Bandwidth is 1 bit long at bit position 13
     * See VITA 49.2-2017 Table 9.1-1
     */
    void aux_bandwidth(bool value) noexcept
    {
        m_packed.set<13>(value);
    }

    /**
     * @brief Returns the Array of CIFs enable flag
     * @return true if packet contains Array of CIFs field, otherwise false
     * 
     * Array of CIFs is 1 bit long at bit position 11
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool array_of_cifs() const noexcept
    {
        return m_packed.get<11>();
    }

    /**
     * @brief Sets the Array of CIFs enable flag
     * @param value Array of CIFs enable flag value to set
     * 
     * Array of CIFs is 1 bit long at bit position 11
     * See VITA 49.2-2017 Table 9.1-1
     */
    void array_of_cifs(bool value) noexcept
    {
        m_packed.set<11>(value);
    }

    /**
     * @brief Returns the Spectrum enable flag
     * @return true if packet contains Spectrum field, otherwise false
     * 
     * Spectrum is 1 bit long at bit position 10
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool spectrum() const noexcept
    {
        return m_packed.get<10>();
    }

    /**
     * @brief Sets the Spectrum enable flag
     * @param value Spectrum enable flag value to set
     * 
     * Spectrum is 1 bit long at bit position 10
     * See VITA 49.2-2017 Table 9.1-1
     */
    void spectrum(bool value) noexcept
    {
        m_packed.set<10>(value);
    }

    /**
     * @brief Returns the Sector Step/Scan enable flag
     * @return true if packet contains Sector Step/Scan field, otherwise false
     * 
     * Sector Step/Scan is 1 bit long at bit position 9
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool sector_step_scan() const noexcept
    {
        return m_packed.get<9>();
    }

    /**
     * @brief Sets the Sector Step/Scan enable flag
     * @param value Sector Step/Scan enable flag value to set
     * 
     * Sector Step/Scan is 1 bit long at bit position 9
     * See VITA 49.2-2017 Table 9.1-1
     */
    void sector_step_scan(bool value) noexcept
    {
        m_packed.set<9>(value);
    }

    /**
     * @brief Returns the Index List enable flag
     * @return true if packet contains Index List field, otherwise false
     * 
     * Index List is 1 bit long at bit position 7
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool index_list() const noexcept
    {
        return m_packed.get<7>();
    }

    /**
     * @brief Sets the Index List enable flag
     * @param value Index List enable flag value to set
     * 
     * Index List is 1 bit long at bit position 7
     * See VITA 49.2-2017 Table 9.1-1
     */
    void index_list(bool value) noexcept
    {
        m_packed.set<7>(value);
    }

    /**
     * @brief Returns the Discrete I/O 32 enable flag
     * @return true if packet contains Discrete I/O 32 field, otherwise false
     * 
     * Discrete I/O 32 is 1 bit long at bit position 6
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool discrete_io_32() const noexcept
    {
        return m_packed.get<6>();
    }

    /**
     * @brief Sets the Discrete I/O 32 enable flag
     * @param value Discrete I/O 32 enable flag value to set
     * 
     * Discrete I/O 32 is 1 bit long at bit position 6
     * See VITA 49.2-2017 Table 9.1-1
     */
    void discrete_io_32(bool value) noexcept
    {
        m_packed.set<6>(value);
    }

    /**
     * @brief Returns the Discrete I/O 64 enable flag
     * @return true if packet contains Discrete I/O 64 field, otherwise false
     * 
     * Discrete I/O 64 is 1 bit long at bit position 5
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool discrete_io_64() const noexcept
    {
        return m_packed.get<5>();
    }

    /**
     * @brief Sets the Discrete I/O 64 enable flag
     * @param value Discrete I/O 64 enable flag value to set
     * 
     * Discrete I/O 64 is 1 bit long at bit position 5
     * See VITA 49.2-2017 Table 9.1-1
     */
    void discrete_io_64(bool value) noexcept
    {
        m_packed.set<5>(value);
    }

    /**
     * @brief Returns the Health Status enable flag
     * @return true if packet contains Health Status field, otherwise false
     * 
     * Health Status is 1 bit long at bit position 4
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool health_status() const noexcept
    {
        return m_packed.get<4>();
    }

    /**
     * @brief Sets the Health Status enable flag
     * @param value Health Status enable flag value to set
     * 
     * Health Status is 1 bit long at bit position 4
     * See VITA 49.2-2017 Table 9.1-1
     */
    void health_status(bool value) noexcept
    {
        m_packed.set<4>(value);
    }

    /**
     * @brief Returns the V49 Spec Compliance enable flag
     * @return true if packet contains V49 Spec Compliance field, otherwise false
     * 
     * V49 Spec Compliance is 1 bit long at bit position 3
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool v49_spec_compliance() const noexcept
    {
        return m_packed.get<3>();
    }

    /**
     * @brief Sets the V49 Spec Compliance enable flag
     * @param value V49 Spec Compliance enable flag value to set
     * 
     * V49 Spec Compliance is 1 bit long at bit position 3
     * See VITA 49.2-2017 Table 9.1-1
     */
    void v49_spec_compliance(bool value) noexcept
    {
        m_packed.set<3>(value);
    }

    /**
     * @brief Returns the Version Build Code enable flag
     * @return true if packet contains Version Build Code field, otherwise false
     * 
     * Version Build Code is 1 bit long at bit position 2
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool version_build_code() const noexcept
    {
        return m_packed.get<2>();
    }

    /**
     * @brief Sets the Version Build Code enable flag
     * @param value Version Build Code enable flag value to set
     * 
     * Version Build Code is 1 bit long at bit position 2
     * See VITA 49.2-2017 Table 9.1-1
     */
    void version_build_code(bool value) noexcept
    {
        m_packed.set<2>(value);
    }

    /**
     * @brief Returns the Buffer Size enable flag
     * @return true if packet contains Buffer Size field, otherwise false
     * 
     * Buffer Size is 1 bit long at bit position 1
     * See VITA 49.2-2017 Table 9.1-1
     */
    bool buffer_size() const noexcept
    {
        return m_packed.get<1>();
    }

    /**
     * @brief Sets the Buffer Size enable flag
     * @param value Buffer Size enable flag value to set
     * 
     * Buffer Size is 1 bit long at bit position 1
     * See VITA 49.2-2017 Table 9.1-1
     */
    void buffer_size(bool value) noexcept
    {
        m_packed.set<1>(value);
    }

}; // end class IndicatorField1

/**
 * @class IndicatorField2
 * @brief Base class for Indicator Field Word 2
 */
class IndicatorField2 : public IndicatorField
{
public:
    bool bind() const noexcept
    {
        return m_packed.get<31>();
    }

    void bind(bool value) noexcept
    {
        m_packed.set<31>(value);
    }

    bool cited_sid() const noexcept
    {
        return m_packed.get<30>();
    }

    void cited_sid(bool value) noexcept
    {
        m_packed.set<30>(value);
    }

    bool sibling_sid() const noexcept
    {
        return m_packed.get<29>();
    }

    void sibling_sid(bool value) noexcept
    {
        m_packed.set<29>(value);
    }

    bool parent_sid() const noexcept
    {
        return m_packed.get<28>();
    }

    void parent_sid(bool value) noexcept
    {
        m_packed.set<28>(value);
    }

    bool child_sid() const noexcept
    {
        return m_packed.get<27>();
    }

    void child_sid(bool value) noexcept
    {
        m_packed.set<27>(value);
    }

    bool cited_message_id() const noexcept
    {
        return m_packed.get<26>();
    }

    void cited_message_id(bool value) noexcept
    {
        m_packed.set<26>(value);
    }

    bool controllee_id() const noexcept
    {
        return m_packed.get<25>();
    }

    void controllee_id(bool value) noexcept
    {
        m_packed.set<25>(value);
    }

    bool controllee_uuid() const noexcept
    {
        return m_packed.get<24>();
    }

    void controllee_uuid(bool value) noexcept
    {
        m_packed.set<24>(value);
    }

    bool controller_id() const noexcept
    {
        return m_packed.get<23>();
    }

    void controller_id(bool value) noexcept
    {
        m_packed.set<23>(value);
    }

    bool controller_uuid() const noexcept
    {
        return m_packed.get<22>();
    }

    void controller_uuid(bool value) noexcept
    {
        m_packed.set<22>(value);
    }

    bool information_source() const noexcept
    {
        return m_packed.get<21>();
    }

    void information_source(bool value) noexcept
    {
        m_packed.set<21>(value);
    }

    bool track_id() const noexcept
    {
        return m_packed.get<20>();
    }

    void track_id(bool value) noexcept
    {
        m_packed.set<20>(value);
    }

    bool country_code() const noexcept
    {
        return m_packed.get<19>();
    }

    void country_code(bool value) noexcept
    {
        m_packed.set<19>(value);
    }

    bool operator_() const noexcept
    {
        return m_packed.get<18>();
    }

    void operator_(bool value) noexcept
    {
        m_packed.set<18>(value);
    }

    bool platform_class() const noexcept
    {
        return m_packed.get<17>();
    }

    void platform_class(bool value) noexcept
    {
        m_packed.set<17>(value);
    }

    bool platform_instance() const noexcept
    {
        return m_packed.get<16>();
    }

    void platform_instance(bool value) noexcept
    {
        m_packed.set<16>(value);
    }

    bool platform_display() const noexcept
    {
        return m_packed.get<15>();
    }

    void platform_display(bool value) noexcept
    {
        m_packed.set<15>(value);
    }

    bool ems_device_class() const noexcept
    {
        return m_packed.get<14>();
    }

    void ems_device_class(bool value) noexcept
    {
        m_packed.set<14>(value);
    }

    bool ems_device_type() const noexcept
    {
        return m_packed.get<13>();
    }

    void ems_device_type(bool value) noexcept
    {
        m_packed.set<13>(value);
    }

    bool ems_device_instance() const noexcept
    {
        return m_packed.get<12>();
    }

    void ems_device_instance(bool value) noexcept
    {
        m_packed.set<12>(value);
    }

    bool modulation_class() const noexcept
    {
        return m_packed.get<11>();
    }

    void modulation_class(bool value) noexcept
    {
        m_packed.set<11>(value);
    }

    bool modulation_type() const noexcept
    {
        return m_packed.get<10>();
    }

    void modulation_type(bool value) noexcept
    {
        m_packed.set<10>(value);
    }

    bool function_id() const noexcept
    {
        return m_packed.get<9>();
    }

    void function_id(bool value) noexcept
    {
        m_packed.set<9>(value);
    }

    bool mode_id() const noexcept
    {
        return m_packed.get<8>();
    }

    void mode_id(bool value) noexcept
    {
        m_packed.set<8>(value);
    }

    bool event_id() const noexcept
    {
        return m_packed.get<7>();
    }

    void event_id(bool value) noexcept
    {
        m_packed.set<7>(value);
    }

    bool function_priority_id() const noexcept
    {
        return m_packed.get<6>();
    }

    void function_priority_id(bool value) noexcept
    {
        m_packed.set<6>(value);
    }

    bool communication_priority_id() const noexcept
    {
        return m_packed.get<5>();
    }

    void communication_priority_id(bool value) noexcept
    {
        m_packed.set<5>(value);
    }

    bool rf_footprint() const noexcept
    {
        return m_packed.get<4>();
    }

    void rf_footprint(bool value) noexcept
    {
        m_packed.set<4>(value);
    }

    bool rf_footprint_range() const noexcept
    {
        return m_packed.get<3>();
    }

    void rf_footprint_range(bool value) noexcept
    {
        m_packed.set<3>(value);
    }

}; // end class IndicatorField2

/**
 * @class IndicatorField3
 * @brief Base class for Indicator Field Word 3
 */
class IndicatorField3 : public IndicatorField
{
public:
    bool timestamp_details() const noexcept
    {
        return m_packed.get<31>();
    }

    void timestamp_details(bool value) noexcept
    {
        m_packed.set<31>(value);
    }

    bool timestamp_skew() const noexcept
    {
        return m_packed.get<30>();
    }

    void timestamp_skew(bool value) noexcept
    {
        m_packed.set<30>(value);
    }

    bool rise_time() const noexcept
    {
        return m_packed.get<27>();
    }

    void rise_time(bool value) noexcept
    {
        m_packed.set<27>(value);
    }

    bool fall_time() const noexcept
    {
        return m_packed.get<26>();
    }

    void fall_time(bool value) noexcept
    {
        m_packed.set<26>(value);
    }

    bool offset_time() const noexcept
    {
        return m_packed.get<25>();
    }

    void offset_time(bool value) noexcept
    {
        m_packed.set<25>(value);
    }

    bool pulse_width() const noexcept
    {
        return m_packed.get<24>();
    }

    void pulse_width(bool value) noexcept
    {
        m_packed.set<24>(value);
    }

    bool period() const noexcept
    {
        return m_packed.get<23>();
    }

    void period(bool value) noexcept
    {
        m_packed.set<23>(value);
    }

    bool duration() const noexcept
    {
        return m_packed.get<22>();
    }

    void duration(bool value) noexcept
    {
        m_packed.set<22>(value);
    }

    bool dwell() const noexcept
    {
        return m_packed.get<21>();
    }

    void dwell(bool value) noexcept
    {
        m_packed.set<21>(value);
    }

    bool jitter() const noexcept
    {
        return m_packed.get<20>();
    }

    void jitter(bool value) noexcept
    {
        m_packed.set<20>(value);
    }

    bool age() const noexcept
    {
        return m_packed.get<17>();
    }

    void age(bool value) noexcept
    {
        m_packed.set<17>(value);
    }

    bool shelf_life() const noexcept
    {
        return m_packed.get<16>();
    }

    void shelf_life(bool value) noexcept
    {
        m_packed.set<16>(value);
    }

    bool air_temperature() const noexcept
    {
        return m_packed.get<7>();
    }

    void air_temperature(bool value) noexcept
    {
        m_packed.set<7>(value);
    }

    bool sea_ground_temperature() const noexcept
    {
        return m_packed.get<6>();
    }

    void sea_ground_temperature(bool value) noexcept
    {
        m_packed.set<6>(value);
    }

    bool humidity() const noexcept
    {
        return m_packed.get<5>();
    }

    void humidity(bool value) noexcept
    {
        m_packed.set<5>(value);
    }

    bool barometric_pressure() const noexcept
    {
        return m_packed.get<4>();
    }

    void barometric_pressure(bool value) noexcept
    {
        m_packed.set<4>(value);
    }

    bool sea_swell_state() const noexcept
    {
        return m_packed.get<3>();
    }

    void sea_swell_state(bool value) noexcept
    {
        m_packed.set<3>(value);
    }

    bool tropospheric_state() const noexcept
    {
        return m_packed.get<2>();
    }

    void tropospheric_state(bool value) noexcept
    {
        m_packed.set<2>(value);
    }

    bool network_id() const noexcept
    {
        return m_packed.get<1>();
    }

    void network_id(bool value) noexcept
    {
        m_packed.set<1>(value);
    }

}; // end class IndicatorField3

/**
 * @class IndicatorField7
 * @brief Base class for Indicator Field Word 7
 */
class IndicatorField7 : public IndicatorField
{
public:
    /**
     * @brief Returns the Current Value Indicator flag
     * @return true if Current Value is attached to each field
     * 
     * Current Value Indicator is 1 bit long at bit position 31
     * See VITA 49.2-2017 Table 9.12-2
     */
    bool current_value() const noexcept
    {
        return m_packed.get<31>();
    }

    /**
     * @brief Sets the Current Value Indicator flag
     * @param value true if Current Value is attached to each field
     * 
     * Current Value Indicator is 1 bit long at bit position 31
     * See VITA 49.2-2017 Table 9.12-2
     */
    void current_value(bool value) noexcept
    {
        m_packed.set<31>(value);
    }

    /**
     * @brief Returns the Mean Value Indicator flag
     * @return true if Mean Value is attached to each field
     * 
     * Mean Value Indicator is 1 bit long at bit position 30
     * See VITA 49.2-2017 Table 9.12-2
     */
    bool mean_value() const noexcept
    {
        return m_packed.get<30>();
    }

    /**
     * @brief Sets the Mean Value Indicator flag
     * @param value true if Mean Value is attached to each field
     * 
     * Mean Value Indicator is 1 bit long at bit position 30
     * See VITA 49.2-2017 Table 9.12-2
     */
    void mean_value(bool value) noexcept
    {
        m_packed.set<30>(value);
    }

    /**
     * @brief Returns the Median Value Indicator flag
     * @return true if Median Value is attached to each field
     * 
     * Median Value Indicator is 1 bit long at bit position 29
     * See VITA 49.2-2017 Table 9.12-2
     */
    bool median_value() const noexcept
    {
        return m_packed.get<29>();
    }

    /**
     * @brief Sets the Median Value Indicator flag
     * @param value true if Median Value is attached to each field
     * 
     * Median Value Indicator is 1 bit long at bit position 29
     * See VITA 49.2-2017 Table 9.12-2
     */
    void median_value(bool value) noexcept
    {
        m_packed.set<29>(value);
    }

    /**
     * @brief Returns the Standard Deviation Value Indicator flag
     * @return true if Standard Deviation Value is attached to each field
     * 
     * Standard Deviation Value Indicator is 1 bit long at bit position 28
     * See VITA 49.2-2017 Table 9.12-2
     */
    bool standard_deviation() const noexcept
    {
        return m_packed.get<28>();
    }

    /**
     * @brief Sets the Standard Deviation Value Indicator flag
     * @param value true if Standard Deviation Value is attached to each field
     * 
     * Standard Deviation Value Indicator is 1 bit long at bit position 28
     * See VITA 49.2-2017 Table 9.12-2
     */
    void standard_deviation(bool value) noexcept
    {
        m_packed.set<28>(value);
    }

    /**
     * @brief Returns the Maximum Value Indicator flag
     * @return true if Maximum Value is attached to each field
     * 
     * Maximum Value Indicator is 1 bit long at bit position 27
     * See VITA 49.2-2017 Table 9.12-2
     */
    bool max_value() const noexcept
    {
        return m_packed.get<27>();
    }

    /**
     * @brief Sets the Maximum Value Indicator flag
     * @param value true if Maximum Value is attached to each field
     * 
     * Maximum Value Indicator is 1 bit long at bit position 27
     * See VITA 49.2-2017 Table 9.12-2
     */
    void max_value(bool value) noexcept
    {
        m_packed.set<27>(value);
    }

    /**
     * @brief Returns the Minimum Value Indicator flag
     * @return true if Minimum Value is attached to each field
     * 
     * Minimum Value Indicator is 1 bit long at bit position 26
     * See VITA 49.2-2017 Table 9.12-2
     */
    bool min_value() const noexcept
    {
        return m_packed.get<26>();
    }

    /**
     * @brief Sets the Minimum Value Indicator flag
     * @param value true if Minimum Value is attached to each field
     * 
     * Minimum Value Indicator is 1 bit long at bit position 26
     * See VITA 49.2-2017 Table 9.12-2
     */
    void min_value(bool value) noexcept
    {
        m_packed.set<26>(value);
    }

    /**
     * @brief Returns the Precision Value Indicator flag
     * @return true if Precision Value is attached to each field
     * 
     * Precision Value Indicator is 1 bit long at bit position 25
     * See VITA 49.2-2017 Table 9.12-2
     */
    bool precision() const noexcept
    {
        return m_packed.get<25>();
    }

    /**
     * @brief Sets the Precision Value Indicator flag
     * @param value true if Precision Value is attached to each field
     * 
     * Precision Value Indicator is 1 bit long at bit position 25
     * See VITA 49.2-2017 Table 9.12-2
     */
    void precision(bool value) noexcept
    {
        m_packed.set<25>(value);
    }

    /**
     * @brief Returns the Accuracy Indicator flag
     * @return true if Accuracy is attached to each field
     * 
     * Accuracy Indicator is 1 bit long at bit position 24
     * See VITA 49.2-2017 Table 9.12-2
     */
    bool accuracy() const noexcept
    {
        return m_packed.get<24>();
    }

    /**
     * @brief Sets the Accuracy Indicator flag
     * @param value true if Accuracy is attached to each field
     * 
     * Accuracy Indicator is 1 bit long at bit position 24
     * See VITA 49.2-2017 Table 9.12-2
     */
    void accuracy(bool value) noexcept
    {
        m_packed.set<24>(value);
    }

    /**
     * @brief Returns the First Derivative Indicator flag
     * @return true if First Derivative is attached to each field
     * 
     * First Derivative Indicator is 1 bit long at bit position 23
     * See VITA 49.2-2017 Table 9.12-2
     */
    bool first_derivative() const noexcept
    {
        return m_packed.get<23>();
    }

    /**
     * @brief Sets the First Derivative Indicator flag
     * @param value true if First Derivative is attached to each field
     * 
     * First Derivative Indicator is 1 bit long at bit position 23
     * See VITA 49.2-2017 Table 9.12-2
     */
    void first_derivative(bool value) noexcept
    {
        m_packed.set<23>(value);
    }

    /**
     * @brief Returns the Second Derivative Indicator flag
     * @return true if First Derivative is attached to each field
     * 
     * First Derivative Indicator is 1 bit long at bit position 22
     * See VITA 49.2-2017 Table 9.12-2
     */
    bool second_derivative() const noexcept
    {
        return m_packed.get<22>();
    }

    /**
     * @brief Sets the Second Derivative Indicator flag
     * @param value true if Second Derivative is attached to each field
     * 
     * Second Derivative Indicator is 1 bit long at bit position 22
     * See VITA 49.2-2017 Table 9.12-2
     */
    void second_derivative(bool value) noexcept
    {
        m_packed.set<22>(value);
    }

    /**
     * @brief Returns the Third Derivative Indicator flag
     * @return true if Third Derivative is attached to each field
     * 
     * Third Derivative Indicator is 1 bit long at bit position 21
     * See VITA 49.2-2017 Table 9.12-2
     */
    bool third_derivative() const noexcept
    {
        return m_packed.get<21>();
    }

    /**
     * @brief Sets the Third Derivative Indicator flag
     * @param value true if Third Derivative is attached to each field
     * 
     * Third Derivative Indicator is 1 bit long at bit position 21
     * See VITA 49.2-2017 Table 9.12-2
     */
    void third_derivative(bool value) noexcept
    {
        m_packed.set<21>(value);
    }

    /**
     * @brief Returns the Probability Indicator flag
     * @return true if Probability is attached to each field
     * 
     * Probability Indicator is 1 bit long at bit position 20
     * See VITA 49.2-2017 Table 9.12-2
     */
    bool probability() const noexcept
    {
        return m_packed.get<20>();
    }

    /**
     * @brief Sets the Probability Indicator flag
     * @param value true if Probability is attached to each field
     * 
     * Probability Indicator is 1 bit long at bit position 20
     * See VITA 49.2-2017 Table 9.12-2
     */
    void probability(bool value) noexcept
    {
        m_packed.set<20>(value);
    }

    /**
     * @brief Returns the Belief Indicator flag
     * @return true if Belief is attached to each field
     * 
     * Belief Indicator is 1 bit long at bit position 19
     * See VITA 49.2-2017 Table 9.12-2
     */
    bool belief() const noexcept
    {
        return m_packed.get<19>();
    }

    /**
     * @brief Sets the Belief Indicator flag
     * @param value true if Belief is attached to each field
     * 
     * Belief Indicator is 1 bit long at bit position 19
     * See VITA 49.2-2017 Table 9.12-2
     */
    void belief(bool value) noexcept
    {
        m_packed.set<19>(value);
    }

}; // end class IndicatorField7

} // end namespace vrtgen::packing

#endif // _VRTGEN_PACKING_INDICATOR_FIELDS_HPP
