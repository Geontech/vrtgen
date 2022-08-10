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
        return m_packed_0.any();
    }

    /**
     * @brief Checks if none of the IndicatorField bits are set to true
     * @return true if none of the bits are set to true, otherwise false
     */
    bool none() const noexcept
    {
        return m_packed_0.none();
    }

    /**
     * @brief Returns the number of IndicatorField bytes
     * @return Number of IndicatorField bytes
     */
    constexpr size_t size() const noexcept
    {
        return m_packed_0.size();
    }

    /**
     * @brief Pack IndicatorField as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add IndicatorField bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed_0.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into IndicatorField
     * @param buffer_ptr Pointer to beginning of IndicatorField bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        // auto* ptr = buffer_ptr;
        m_packed_0.unpack_from(buffer_ptr);
    }

protected:
    vrtgen::packed<uint32_t> m_packed_0;

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
     * See VITA 49.2 Table 9.1-1
     */
    bool change_indicator() const noexcept
    {
        return m_packed_0.get(m_change_indicator_tag);
    }

    /**
     * @brief Sets the Context Field Change Indicator flag
     * @param value Context Field Change Indicator flag value to set
     * 
     * Context Field Change Indicator is 1 bit long at bit position 31
     * See VITA 49.2 Table 9.1-1
     */
    void change_indicator(bool value) noexcept
    {
        m_packed_0.set(value, m_change_indicator_tag);
    }

    /**
     * @brief Returns the Reference Point ID enable flag
     * @return true if packet contains Reference Point ID field, otherwise false
     * 
     * Reference Point ID is 1 bit long at bit position 30
     * See VITA 49.2 Table 9.1-1
     */
    bool reference_point_id() const noexcept
    {
        return m_packed_0.get(m_reference_point_id_tag);
    }

    /**
     * @brief Sets the Reference Point ID enable flag
     * @param value Reference Point ID enable flag value to set
     * 
     * Reference Point ID is 1 bit long at bit position 30
     * See VITA 49.2 Table 9.1-1
     */
    void reference_point_id(bool value) noexcept
    {
        m_packed_0.set(value, m_reference_point_id_tag);
    }

    /**
     * @brief Returns the Bandwidth enable flag
     * @return true if packet contains Bandwidth field, otherwise false
     * 
     * Bandwidth is 1 bit long at bit position 29
     * See VITA 49.2 Table 9.1-1
     */
    bool bandwidth() const noexcept
    {
        return m_packed_0.get(m_bandwidth_tag);
    }

    /**
     * @brief Sets the Bandwidth enable flag
     * @param value Bandwidth enable flag value to set
     * 
     * Bandwidth is 1 bit long at bit position 29
     * See VITA 49.2 Table 9.1-1
     */
    void bandwidth(bool value) noexcept
    {
        m_packed_0.set(value, m_bandwidth_tag);
    }

    /**
     * @brief Returns the IF Reference Frequency enable flag
     * @return true if packet contains IF Reference Frequency field, otherwise false
     * 
     * IF Reference Frequency is 1 bit long at bit position 28
     * See VITA 49.2 Table 9.1-1
     */
    bool if_ref_frequency() const noexcept
    {
        return m_packed_0.get(m_if_ref_frequency_tag);
    }

    /**
     * @brief Sets the IF Reference Frequency enable flag
     * @param value IF Reference Frequency enable flag value to set
     * 
     * IF Reference Frequency is 1 bit long at bit position 28
     * See VITA 49.2 Table 9.1-1
     */
    void if_ref_frequency(bool value) noexcept
    {
        m_packed_0.set(value, m_if_ref_frequency_tag);
    }

    /**
     * @brief Returns the RF Reference Frequency enable flag
     * @return true if packet contains RF Reference Frequency field, otherwise false
     * 
     * RF Reference Frequency is 1 bit long at bit position 27
     * See VITA 49.2 Table 9.1-1
     */
    bool rf_ref_frequency() const noexcept
    {
        return m_packed_0.get(m_rf_ref_frequency_tag);
    }

    /**
     * @brief Sets the RF Reference Frequency enable flag
     * @param value RF Reference Frequency enable flag value to set
     * 
     * RF Reference Frequency is 1 bit long at bit position 27
     * See VITA 49.2 Table 9.1-1
     */
    void rf_ref_frequency(bool value) noexcept
    {
        m_packed_0.set(value, m_rf_ref_frequency_tag);
    }

    /**
     * @brief Returns the RF Reference Frequency Offset enable flag
     * @return true if packet contains RF Reference Frequency Offset field, otherwise false
     * 
     * RF Reference Frequency Offset is 1 bit long at bit position 26
     * See VITA 49.2 Table 9.1-1
     */
    bool rf_ref_frequency_offset() const noexcept
    {
        return m_packed_0.get(m_rf_ref_frequency_offset_tag);
    }

    /**
     * @brief Sets the RF Reference Frequency Offset enable flag
     * @param value RF Reference Frequency Offset enable flag value to set
     * 
     * RF Reference Frequency Offset is 1 bit long at bit position 26
     * See VITA 49.2 Table 9.1-1
     */
    void rf_ref_frequency_offset(bool value) noexcept
    {
        m_packed_0.set(value, m_rf_ref_frequency_offset_tag);
    }

    /**
     * @brief Returns the IF Band Offset enable flag
     * @return true if packet contains IF Band Offset field, otherwise false
     * 
     * IF Band Offset is 1 bit long at bit position 25
     * See VITA 49.2 Table 9.1-1
     */
    bool if_band_offset() const noexcept
    {
        return m_packed_0.get(m_if_band_offset_tag);
    }

    /**
     * @brief Sets the IF Band Offset enable flag
     * @param value IF Band Offset enable flag value to set
     * 
     * IF Band Offset is 1 bit long at bit position 25
     * See VITA 49.2 Table 9.1-1
     */
    void if_band_offset(bool value) noexcept
    {
        m_packed_0.set(value, m_if_band_offset_tag);
    }

    /**
     * @brief Returns the Reference Level enable flag
     * @return true if packet contains Reference Level field, otherwise false
     * 
     * Reference Level is 1 bit long at bit position 24
     * See VITA 49.2 Table 9.1-1
     */
    bool reference_level() const noexcept
    {
        return m_packed_0.get(m_reference_level_tag);
    }

    /**
     * @brief Sets the Reference Level enable flag
     * @param value Reference Level enable flag value to set
     * 
     * Reference Level is 1 bit long at bit position 24
     * See VITA 49.2 Table 9.1-1
     */
    void reference_level(bool value) noexcept
    {
        m_packed_0.set(value, m_reference_level_tag);
    }

    /**
     * @brief Returns the Gain enable flag
     * @return true if packet contains Gain field, otherwise false
     * 
     * Gain is 1 bit long at bit position 23
     * See VITA 49.2 Table 9.1-1
     */
    bool gain() const noexcept
    {
        return m_packed_0.get(m_gain_tag);
    }

    /**
     * @brief Sets the Gain enable flag
     * @param value Gain enable flag value to set
     * 
     * Gain is 1 bit long at bit position 23
     * See VITA 49.2 Table 9.1-1
     */
    void gain(bool value) noexcept
    {
        m_packed_0.set(value, m_gain_tag);
    }

    /**
     * @brief Returns the Over-Range Count enable flag
     * @return true if packet contains Over-Range Count field, otherwise false
     * 
     * Over-Range Count is 1 bit long at bit position 22
     * See VITA 49.2 Table 9.1-1
     */
    bool over_range_count() const noexcept
    {
        return m_packed_0.get(m_over_range_count_tag);
    }

    /**
     * @brief Sets the Over-Range Count enable flag
     * @param value Over-Range Count enable flag value to set
     * 
     * Over-Range Count is 1 bit long at bit position 22
     * See VITA 49.2 Table 9.1-1
     */
    void over_range_count(bool value) noexcept
    {
        m_packed_0.set(value, m_over_range_count_tag);
    }

    /**
     * @brief Returns the Sample Rate enable flag
     * @return true if packet contains Sample Rate field, otherwise false
     * 
     * Sample Rate is 1 bit long at bit position 21
     * See VITA 49.2 Table 9.1-1
     */
    bool sample_rate() const noexcept
    {
        return m_packed_0.get(m_sample_rate_tag);
    }

    /**
     * @brief Sets the Sample Rate enable flag
     * @param value Sample Rate enable flag value to set
     * 
     * Sample Rate is 1 bit long at bit position 21
     * See VITA 49.2 Table 9.1-1
     */
    void sample_rate(bool value) noexcept
    {
        m_packed_0.set(value, m_sample_rate_tag);
    }

    /**
     * @brief Returns the Timestamp Adjustment enable flag
     * @return true if packet contains Timestamp Adjustment field, otherwise false
     * 
     * Timestamp Adjustment is 1 bit long at bit position 20
     * See VITA 49.2 Table 9.1-1
     */
    bool timestamp_adjustment() const noexcept
    {
        return m_packed_0.get(m_timestamp_adjustment_tag);
    }

    /**
     * @brief Sets the Timestamp Adjustment enable flag
     * @param value Timestamp Adjustment enable flag value to set
     * 
     * Timestamp Adjustment is 1 bit long at bit position 20
     * See VITA 49.2 Table 9.1-1
     */
    void timestamp_adjustment(bool value) noexcept
    {
        m_packed_0.set(value, m_timestamp_adjustment_tag);
    }

    /**
     * @brief Returns the Timestamp Calibration Time enable flag
     * @return true if packet contains Timestamp Calibration Time field, otherwise false
     * 
     * Timestamp Calibration Time is 1 bit long at bit position 19
     * See VITA 49.2 Table 9.1-1
     */
    bool timestamp_calibration_time() const noexcept
    {
        return m_packed_0.get(m_timestamp_calibration_time_tag);
    }

    /**
     * @brief Sets the Timestamp Calibration Time enable flag
     * @param value Timestamp Calibration Time enable flag value to set
     * 
     * Timestamp Calibration Time is 1 bit long at bit position 19
     * See VITA 49.2 Table 9.1-1
     */
    void timestamp_calibration_time(bool value) noexcept
    {
        m_packed_0.set(value, m_timestamp_calibration_time_tag);
    }

    /**
     * @brief Returns the Temperature enable flag
     * @return true if packet contains Temperature field, otherwise false
     * 
     * Temperature is 1 bit long at bit position 18
     * See VITA 49.2 Table 9.1-1
     */
    bool temperature() const noexcept
    {
        return m_packed_0.get(m_temperature_tag);
    }

    /**
     * @brief Sets the Temperature enable flag
     * @param value Temperature enable flag value to set
     * 
     * Temperature is 1 bit long at bit position 18
     * See VITA 49.2 Table 9.1-1
     */
    void temperature(bool value) noexcept
    {
        m_packed_0.set(value, m_temperature_tag);
    }

    /**
     * @brief Returns the Device Identifier enable flag
     * @return true if packet contains Device Identifier field, otherwise false
     * 
     * Device Identifier is 1 bit long at bit position 17
     * See VITA 49.2 Table 9.1-1
     */
    bool device_id() const noexcept
    {
        return m_packed_0.get(m_device_id_tag);
    }

    /**
     * @brief Sets the Device Identifier enable flag
     * @param value Device Identifier enable flag value to set
     * 
     * Device Identifier is 1 bit long at bit position 17
     * See VITA 49.2 Table 9.1-1
     */
    void device_id(bool value) noexcept
    {
        m_packed_0.set(value, m_device_id_tag);
    }

    /**
     * @brief Returns the State/Event Indicators enable flag
     * @return true if packet contains State/Event Indicators field, otherwise false
     * 
     * State/Event Indicators is 1 bit long at bit position 16
     * See VITA 49.2 Table 9.1-1
     */
    bool state_event_indicators() const noexcept
    {
        return m_packed_0.get(m_state_event_indicators_tag);
    }

    /**
     * @brief Sets the State/Event Indicators enable flag
     * @param value State/Event Indicators enable flag value to set
     * 
     * State/Event Indicators is 1 bit long at bit position 16
     * See VITA 49.2 Table 9.1-1
     */
    void state_event_indicators(bool value) noexcept
    {
        m_packed_0.set(value, m_state_event_indicators_tag);
    }

    /**
     * @brief Returns the Signal Data Packet Payload Format enable flag
     * @return true if packet contains Signal Data Packet Payload Format field, otherwise false
     * 
     * Signal Data Packet Payload Format is 1 bit long at bit position 15
     * See VITA 49.2 Table 9.1-1
     */
    bool signal_data_format() const noexcept
    {
        return m_packed_0.get(m_signal_data_format_tag);
    }

    /**
     * @brief Sets the Signal Data Packet Payload Format enable flag
     * @param value Signal Data Packet Payload Format enable flag value to set
     * 
     * Signal Data Packet Payload Format is 1 bit long at bit position 15
     * See VITA 49.2 Table 9.1-1
     */
    void signal_data_format(bool value) noexcept
    {
        m_packed_0.set(value, m_signal_data_format_tag);
    }

    /**
     * @brief Returns the Formatted GPS enable flag
     * @return true if packet contains Formatted GPS field, otherwise false
     * 
     * Formatted GPS is 1 bit long at bit position 14
     * See VITA 49.2 Table 9.1-1
     */
    bool formatted_gps() const noexcept
    {
        return m_packed_0.get(m_formatted_gps_tag);
    }

    /**
     * @brief Sets the Formatted GPS enable flag
     * @param value Formatted GPS enable flag value to set
     * 
     * Formatted GPS is 1 bit long at bit position 14
     * See VITA 49.2 Table 9.1-1
     */
    void formatted_gps(bool value) noexcept
    {
        m_packed_0.set(value, m_formatted_gps_tag);
    }

    /**
     * @brief Returns the Formatted INS enable flag
     * @return true if packet contains Formatted INS field, otherwise false
     * 
     * Formatted INS is 1 bit long at bit position 13
     * See VITA 49.2 Table 9.1-1
     */
    bool formatted_ins() const noexcept
    {
        return m_packed_0.get(m_formatted_ins_tag);
    }

    /**
     * @brief Sets the Formatted INS enable flag
     * @param value Formatted INS enable flag value to set
     * 
     * Formatted INS is 1 bit long at bit position 13
     * See VITA 49.2 Table 9.1-1
     */
    void formatted_ins(bool value) noexcept
    {
        m_packed_0.set(value, m_formatted_ins_tag);
    }

    /**
     * @brief Returns the ECEF Ephemeris enable flag
     * @return true if packet contains ECEF Ephemeris field, otherwise false
     * 
     * ECEF Ephemeris is 1 bit long at bit position 12
     * See VITA 49.2 Table 9.1-1
     */
    bool ecef_ephemeris() const noexcept
    {
        return m_packed_0.get(m_ecef_ephemeris_tag);
    }

    /**
     * @brief Sets the ECEF Ephemeris enable flag
     * @param value ECEF Ephemeris enable flag value to set
     * 
     * ECEF Ephemeris is 1 bit long at bit position 12
     * See VITA 49.2 Table 9.1-1
     */
    void ecef_ephemeris(bool value) noexcept
    {
        m_packed_0.set(value, m_ecef_ephemeris_tag);
    }

    /**
     * @brief Returns the Relative Ephemeris enable flag
     * @return true if packet contains Relative Ephemeris field, otherwise false
     * 
     * Relative Ephemeris is 1 bit long at bit position 11
     * See VITA 49.2 Table 9.1-1
     */
    bool relative_ephemeris() const noexcept
    {
        return m_packed_0.get(m_relative_ephemeris_tag);
    }

    /**
     * @brief Sets the Relative Ephemeris enable flag
     * @param value Relative Ephemeris enable flag value to set
     * 
     * Relative Ephemeris is 1 bit long at bit position 11
     * See VITA 49.2 Table 9.1-1
     */
    void relative_ephemeris(bool value) noexcept
    {
        m_packed_0.set(value, m_relative_ephemeris_tag);
    }

    /**
     * @brief Returns the Ephemeris Ref ID enable flag
     * @return true if packet contains Ephemeris Ref ID field, otherwise false
     * 
     * Ephemeris Ref ID is 1 bit long at bit position 10
     * See VITA 49.2 Table 9.1-1
     */
    bool ephemeris_ref_id() const noexcept
    {
        return m_packed_0.get(m_ephemeris_ref_id_tag);
    }

    /**
     * @brief Sets the Ephemeris Ref ID enable flag
     * @param value Ephemeris Ref ID enable flag value to set
     * 
     * Ephemeris Ref ID is 1 bit long at bit position 10
     * See VITA 49.2 Table 9.1-1
     */
    void ephemeris_ref_id(bool value) noexcept
    {
        m_packed_0.set(value, m_ephemeris_ref_id_tag);
    }

    /**
     * @brief Returns the Context Assocations List enable flag
     * @return true if packet contains Context Assocations List field, otherwise false
     * 
     * Context Assocations List is 1 bit long at bit position 8
     * See VITA 49.2 Table 9.1-1
     */
    bool context_association_lists() const noexcept
    {
        return m_packed_0.get(m_context_association_lists_tag);
    }

    /**
     * @brief Sets the Context Assocations List enable flag
     * @param value Context Assocations List enable flag value to set
     * 
     * Context Assocations List is 1 bit long at bit position 8
     * See VITA 49.2 Table 9.1-1
     */
    void context_association_lists(bool value) noexcept
    {
        m_packed_0.set(value, m_context_association_lists_tag);
    }
     
protected:
    /**
     * @brief Indicator Field 7 enable flag
     * @return true if packet contains Indicator Field 7 word, otherwise false
     * 
     * Indicator Field 7 Enable is 1 bit long at bit position 7
     * See VITA 49.2 Table 9.1-1
     */
    bool if7_enable() const noexcept
    {
        return m_packed_0.get(m_if7_enable_tag);
    }

    /**
     * @brief Sets the Indicator Field 7 enable flag
     * @param value Indicator Field 7 enable flag value to set
     * 
     * Indicator Field 7 Enable is 1 bit long at bit position 7
     * See VITA 49.2 Table 9.1-1
     */
    void if7_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_if7_enable_tag);
    }

    /**
     * @brief Indicator Field 3 enable flag
     * @return true if packet contains Indicator Field 3 word, otherwise false
     * 
     * Indicator Field 3 Enable is 1 bit long at bit position 3
     * See VITA 49.2 Table 9.1-1
     */
    bool if3_enable() const noexcept
    {
        return m_packed_0.get(m_if3_enable_tag);
    }

    /**
     * @brief Sets the Indicator Field 3 enable flag
     * @param value Indicator Field 3 enable flag value to set
     * 
     * Indicator Field 3 Enable is 1 bit long at bit position 3
     * See VITA 49.2 Table 9.1-1
     */
    void if3_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_if3_enable_tag);
    }

    /**
     * @brief Indicator Field 2 enable flag
     * @return true if packet contains Indicator Field 2 word, otherwise false
     * 
     * Indicator Field 2 Enable is 1 bit long at bit position 2
     * See VITA 49.2 Table 9.1-1
     */
    bool if2_enable() const noexcept
    {
        return m_packed_0.get(m_if2_enable_tag);
    }

    /**
     * @brief Sets the Indicator Field 2 enable flag
     * @param value Indicator Field 2 enable flag value to set
     * 
     * Indicator Field 2 Enable is 1 bit long at bit position 2
     * See VITA 49.2 Table 9.1-1
     */
    void if2_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_if2_enable_tag);
    }

    /**
     * @brief Indicator Field 1 enable flag
     * @return true if packet contains Indicator Field 1 word, otherwise false
     * 
     * Indicator Field 1 Enable is 1 bit long at bit position 1
     * See VITA 49.2 Table 9.1-1
     */
    bool if1_enable() const noexcept
    {
        return m_packed_0.get(m_if1_enable_tag);
    }

    /**
     * @brief Sets the Indicator Field 1 enable flag
     * @param value Indicator Field 1 enable flag value to set
     * 
     * Indicator Field 1 Enable is 1 bit long at bit position 1
     * See VITA 49.2 Table 9.1-1
     */
    void if1_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_if1_enable_tag);
    }

private:
    /**
     * change_indicator 0/31
     * reference_point_id 0/30
     * bandwidth 0/29
     * if_ref_frequency 0/28
     * rf_ref_frequency 0/27
     * rf_ref_frequency_offset 0/26
     * if_band_offset 0/25
     * reference_level 0/24
     * gain 0/23
     * over_range_counnt 0/22
     * sample_rate 0/21
     * timestamp_adjustment 0/20
     * timestamp_calibration_time 0/19
     * temperature 0/18
     * device_id 0/17
     * state_event_indicators 0/16
     * payload_format 0/15
     * formatted_gps 0/14
     * formatted_ins 0/13
     * ecef_ephemeris 0/12
     * relative_ephemeris  0/11
     * ephemeris_ref_id 0/10
     * gps_ascii 0/9
     * context_association_lists 0/8
     * if7_enable 0/7
     * if3_enable 0/3
     * if2_enable 0/2
     * if1_enable 0/1
     */
    vrtgen::packed_tag<bool,31,1> m_change_indicator_tag;
    vrtgen::packed_tag<bool,30,1> m_reference_point_id_tag;
    vrtgen::packed_tag<bool,29,1> m_bandwidth_tag;
    vrtgen::packed_tag<bool,28,1> m_if_ref_frequency_tag;
    vrtgen::packed_tag<bool,27,1> m_rf_ref_frequency_tag;
    vrtgen::packed_tag<bool,26,1> m_rf_ref_frequency_offset_tag;
    vrtgen::packed_tag<bool,25,1> m_if_band_offset_tag;
    vrtgen::packed_tag<bool,24,1> m_reference_level_tag;
    vrtgen::packed_tag<bool,23,1> m_gain_tag;
    vrtgen::packed_tag<bool,22,1> m_over_range_count_tag;
    vrtgen::packed_tag<bool,21,1> m_sample_rate_tag;
    vrtgen::packed_tag<bool,20,1> m_timestamp_adjustment_tag;
    vrtgen::packed_tag<bool,19,1> m_timestamp_calibration_time_tag;
    vrtgen::packed_tag<bool,18,1> m_temperature_tag;
    vrtgen::packed_tag<bool,17,1> m_device_id_tag;
    vrtgen::packed_tag<bool,16,1> m_state_event_indicators_tag;
    vrtgen::packed_tag<bool,15,1> m_signal_data_format_tag;
    vrtgen::packed_tag<bool,14,1> m_formatted_gps_tag;
    vrtgen::packed_tag<bool,13,1> m_formatted_ins_tag;
    vrtgen::packed_tag<bool,12,1> m_ecef_ephemeris_tag;
    vrtgen::packed_tag<bool,11,1> m_relative_ephemeris_tag;
    vrtgen::packed_tag<bool,10,1> m_ephemeris_ref_id_tag;
    vrtgen::packed_tag<bool,9,1> m_gps_ascii_tag;
    vrtgen::packed_tag<bool,8,1> m_context_association_lists_tag;
    vrtgen::packed_tag<bool,7,1> m_if7_enable_tag;
    vrtgen::packed_tag<bool,3,1> m_if3_enable_tag;
    vrtgen::packed_tag<bool,2,1> m_if2_enable_tag;
    vrtgen::packed_tag<bool,1,1> m_if1_enable_tag;

}; // end class IndicatorField0

/**
 * @class IndicatorField1
 * @brief Base class for Indicator Field Word 1
 */
class IndicatorField1 : public IndicatorField
{
public:
    bool phase_offset() const noexcept
    {
        return m_packed_0.get(m_phase_offset_tag);
    }

    void phase_offset(bool value) noexcept
    {
        m_packed_0.set(value, m_phase_offset_tag);
    }

    bool polarization() const noexcept
    {
        return m_packed_0.get(m_polarization_tag);
    }

    void polarization(bool value) noexcept
    {
        m_packed_0.set(value, m_polarization_tag);
    }

    bool pointing_vector() const noexcept
    {
        return m_packed_0.get(m_pointing_vector_tag);
    }

    void pointing_vector(bool value) noexcept
    {
        m_packed_0.set(value, m_pointing_vector_tag);
    }

    bool pointing_vector_structure() const noexcept
    {
        return m_packed_0.get(m_pointing_vector_structure_tag);
    }

    void pointing_vector_structure(bool value) noexcept
    {
        m_packed_0.set(value, m_pointing_vector_structure_tag);
    }

    bool spatial_scan_type() const noexcept
    {
        return m_packed_0.get(m_spatial_scan_type_tag);
    }

    void spatial_scan_type(bool value) noexcept
    {
        m_packed_0.set(value, m_spatial_scan_type_tag);
    }

    bool spatial_reference_type() const noexcept
    {
        return m_packed_0.get(m_spatial_reference_type_tag);
    }

    void spatial_reference_type(bool value) noexcept
    {
        m_packed_0.set(value, m_spatial_reference_type_tag);
    }

    bool beam_width() const noexcept
    {
        return m_packed_0.get(m_beam_width_tag);
    }

    void beam_width(bool value) noexcept
    {
        m_packed_0.set(value, m_beam_width_tag);
    }

    bool range() const noexcept
    {
        return m_packed_0.get(m_range_tag);
    }

    void range(bool value) noexcept
    {
        m_packed_0.set(value, m_range_tag);
    }

    bool ebno_ber() const noexcept
    {
        return m_packed_0.get(m_ebno_ber_tag);
    }

    void ebno_ber(bool value) noexcept
    {
        m_packed_0.set(value, m_ebno_ber_tag);
    }

    bool threshold() const noexcept
    {
        return m_packed_0.get(m_threshold_tag);
    }

    void threshold(bool value) noexcept
    {
        m_packed_0.set(value, m_threshold_tag);
    }

    bool compression_point() const noexcept
    {
        return m_packed_0.get(m_compression_point_tag);
    }

    void compression_point(bool value) noexcept
    {
        m_packed_0.set(value, m_compression_point_tag);
    }

    bool intercept_points() const noexcept
    {
        return m_packed_0.get(m_intercept_points_tag);
    }

    void intercept_points(bool value) noexcept
    {
        m_packed_0.set(value, m_intercept_points_tag);
    }

    bool snr_noise_figure() const noexcept
    {
        return m_packed_0.get(m_snr_noise_figure_tag);
    }

    void snr_noise_figure(bool value) noexcept
    {
        m_packed_0.set(value, m_snr_noise_figure_tag);
    }

    bool aux_frequency() const noexcept
    {
        return m_packed_0.get(m_aux_frequency_tag);
    }

    void aux_frequency(bool value) noexcept
    {
        m_packed_0.set(value, m_aux_frequency_tag);
    }

    bool aux_gain() const noexcept
    {
        return m_packed_0.get(m_aux_gain_tag);
    }

    void aux_gain(bool value) noexcept
    {
        m_packed_0.set(value, m_aux_gain_tag);
    }

    bool aux_bandwidth() const noexcept
    {
        return m_packed_0.get(m_aux_bandwidth_tag);
    }

    void aux_bandwidth(bool value) noexcept
    {
        m_packed_0.set(value, m_aux_bandwidth_tag);
    }

    bool array_of_cifs() const noexcept
    {
        return m_packed_0.get(m_array_of_cifs_tag);
    }

    void array_of_cifs(bool value) noexcept
    {
        m_packed_0.set(value, m_array_of_cifs_tag);
    }

    bool spectrum() const noexcept
    {
        return m_packed_0.get(m_spectrum_tag);
    }

    void spectrum(bool value) noexcept
    {
        m_packed_0.set(value, m_spectrum_tag);
    }

    bool sector_step_scan() const noexcept
    {
        return m_packed_0.get(m_sector_step_scan_tag);
    }

    void sector_step_scan(bool value) noexcept
    {
        m_packed_0.set(value, m_sector_step_scan_tag);
    }

    bool index_list() const noexcept
    {
        return m_packed_0.get(m_index_list_tag);
    }

    void index_list(bool value) noexcept
    {
        m_packed_0.set(value, m_index_list_tag);
    }

    bool discrete_io_32() const noexcept
    {
        return m_packed_0.get(m_discrete_io_32_tag);
    }

    void discrete_io_32(bool value) noexcept
    {
        m_packed_0.set(value, m_discrete_io_32_tag);
    }

    bool discrete_io_64() const noexcept
    {
        return m_packed_0.get(m_discrete_io_64_tag);
    }

    void discrete_io_64(bool value) noexcept
    {
        m_packed_0.set(value, m_discrete_io_64_tag);
    }

    bool health_status() const noexcept
    {
        return m_packed_0.get(m_health_status_tag);
    }

    void health_status(bool value) noexcept
    {
        m_packed_0.set(value, m_health_status_tag);
    }

    bool v49_spec_compliance() const noexcept
    {
        return m_packed_0.get(m_v49_spec_compliance_tag);
    }

    void v49_spec_compliance(bool value) noexcept
    {
        m_packed_0.set(value, m_v49_spec_compliance_tag);
    }

    bool version_build_code() const noexcept
    {
        return m_packed_0.get(m_version_build_code_tag);
    }

    void version_build_code(bool value) noexcept
    {
        m_packed_0.set(value, m_version_build_code_tag);
    }

    bool buffer_size() const noexcept
    {
        return m_packed_0.get(m_buffer_size_tag);
    }

    void buffer_size(bool value) noexcept
    {
        m_packed_0.set(value, m_buffer_size_tag);
    }

private:
    /*
     * phase_offset 0/31 
     * polarization 0/30 
     * pointing_vector 0/29 
     * pointing_vector_structure 0/28 
     * spatial_scan_type 0/27 
     * spatial_reference_type 0/26 
     * beam_width 0/25 
     * range 0/24 
     * ebno_ber 0/20 
     * threshold 0/19 
     * compression_point 0/18 
     * intercept_points 0/17 
     * snr_noise_figure 0/16 
     * aux_frequency 0/15 
     * aux_gain 0/14 
     * aux_bandwidth 0/13 
     * array_of_cifs 0/11 
     * spectrum 0/10 
     * sector_step_scan 0/9
     * reserved_8 0/8
     * index_list 0/7 
     * discrete_io_32 0/6 
     * discrete_io_64 0/5 
     * health_status 0/4 
     * v49_spec_compliance 0/3 
     * version_build_code 0/2 
     * buffer_size 0/1 
     */
    vrtgen::packed_tag<bool,31,1> m_phase_offset_tag;
    vrtgen::packed_tag<bool,30,1> m_polarization_tag;
    vrtgen::packed_tag<bool,29,1> m_pointing_vector_tag;
    vrtgen::packed_tag<bool,28,1> m_pointing_vector_structure_tag;
    vrtgen::packed_tag<bool,27,1> m_spatial_scan_type_tag;
    vrtgen::packed_tag<bool,26,1> m_spatial_reference_type_tag;
    vrtgen::packed_tag<bool,25,1> m_beam_width_tag;
    vrtgen::packed_tag<bool,24,1> m_range_tag;
    vrtgen::packed_tag<bool,20,1> m_ebno_ber_tag;
    vrtgen::packed_tag<bool,19,1> m_threshold_tag;
    vrtgen::packed_tag<bool,18,1> m_compression_point_tag;
    vrtgen::packed_tag<bool,17,1> m_intercept_points_tag;
    vrtgen::packed_tag<bool,16,1> m_snr_noise_figure_tag;
    vrtgen::packed_tag<bool,15,1> m_aux_frequency_tag;
    vrtgen::packed_tag<bool,14,1> m_aux_gain_tag;
    vrtgen::packed_tag<bool,13,1> m_aux_bandwidth_tag;
    vrtgen::packed_tag<bool,11,1> m_array_of_cifs_tag;
    vrtgen::packed_tag<bool,10,1> m_spectrum_tag;
    vrtgen::packed_tag<bool,9,1> m_sector_step_scan_tag;
    vrtgen::packed_tag<bool,7,1> m_index_list_tag;
    vrtgen::packed_tag<bool,6,1> m_discrete_io_32_tag;
    vrtgen::packed_tag<bool,5,1> m_discrete_io_64_tag;
    vrtgen::packed_tag<bool,4,1> m_health_status_tag;
    vrtgen::packed_tag<bool,3,1> m_v49_spec_compliance_tag;
    vrtgen::packed_tag<bool,2,1> m_version_build_code_tag;
    vrtgen::packed_tag<bool,1,1> m_buffer_size_tag;

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
        return m_packed_0.get(m_bind_tag);
    }

    void bind(bool value) noexcept
    {
        m_packed_0.set(value, m_bind_tag);
    }

    bool cited_sid() const noexcept
    {
        return m_packed_0.get(m_cited_sid_tag);
    }

    void cited_sid(bool value) noexcept
    {
        m_packed_0.set(value, m_cited_sid_tag);
    }

    bool sibling_sid() const noexcept
    {
        return m_packed_0.get(m_sibling_sid_tag);
    }

    void sibling_sid(bool value) noexcept
    {
        m_packed_0.set(value, m_sibling_sid_tag);
    }

    bool parent_sid() const noexcept
    {
        return m_packed_0.get(m_parent_sid_tag);
    }

    void parent_sid(bool value) noexcept
    {
        m_packed_0.set(value, m_parent_sid_tag);
    }

    bool child_sid() const noexcept
    {
        return m_packed_0.get(m_child_sid_tag);
    }

    void child_sid(bool value) noexcept
    {
        m_packed_0.set(value, m_child_sid_tag);
    }

    bool cited_message_id() const noexcept
    {
        return m_packed_0.get(m_cited_message_id_tag);
    }

    void cited_message_id(bool value) noexcept
    {
        m_packed_0.set(value, m_cited_message_id_tag);
    }

    bool controllee_id() const noexcept
    {
        return m_packed_0.get(m_controllee_id_tag);
    }

    void controllee_id(bool value) noexcept
    {
        m_packed_0.set(value, m_controllee_id_tag);
    }

    bool controllee_uuid() const noexcept
    {
        return m_packed_0.get(m_controllee_uuid_tag);
    }

    void controllee_uuid(bool value) noexcept
    {
        m_packed_0.set(value, m_controllee_uuid_tag);
    }

    bool controller_id() const noexcept
    {
        return m_packed_0.get(m_controller_id_tag);
    }

    void controller_id(bool value) noexcept
    {
        m_packed_0.set(value, m_controller_id_tag);
    }

    bool controller_uuid() const noexcept
    {
        return m_packed_0.get(m_controller_uuid_tag);
    }

    void controller_uuid(bool value) noexcept
    {
        m_packed_0.set(value, m_controller_uuid_tag);
    }

    bool information_source() const noexcept
    {
        return m_packed_0.get(m_information_source_tag);
    }

    void information_source(bool value) noexcept
    {
        m_packed_0.set(value, m_information_source_tag);
    }

    bool track_id() const noexcept
    {
        return m_packed_0.get(m_track_id_tag);
    }

    void track_id(bool value) noexcept
    {
        m_packed_0.set(value, m_track_id_tag);
    }

    bool country_code() const noexcept
    {
        return m_packed_0.get(m_country_code_tag);
    }

    void country_code(bool value) noexcept
    {
        m_packed_0.set(value, m_country_code_tag);
    }

    bool operator_() const noexcept
    {
        return m_packed_0.get(m_operator_tag);
    }

    void operator_(bool value) noexcept
    {
        m_packed_0.set(value, m_operator_tag);
    }

    bool platform_class() const noexcept
    {
        return m_packed_0.get(m_platform_class_tag);
    }

    void platform_class(bool value) noexcept
    {
        m_packed_0.set(value, m_platform_class_tag);
    }

    bool platform_instance() const noexcept
    {
        return m_packed_0.get(m_platform_instance_tag);
    }

    void platform_instance(bool value) noexcept
    {
        m_packed_0.set(value, m_platform_instance_tag);
    }

    bool platform_display() const noexcept
    {
        return m_packed_0.get(m_platform_display_tag);
    }

    void platform_display(bool value) noexcept
    {
        m_packed_0.set(value, m_platform_display_tag);
    }

    bool ems_device_class() const noexcept
    {
        return m_packed_0.get(m_ems_device_class_tag);
    }

    void ems_device_class(bool value) noexcept
    {
        m_packed_0.set(value, m_ems_device_class_tag);
    }

    bool ems_device_type() const noexcept
    {
        return m_packed_0.get(m_ems_device_type_tag);
    }

    void ems_device_type(bool value) noexcept
    {
        m_packed_0.set(value, m_ems_device_type_tag);
    }

    bool ems_device_instance() const noexcept
    {
        return m_packed_0.get(m_ems_device_instance_tag);
    }

    void ems_device_instance(bool value) noexcept
    {
        m_packed_0.set(value, m_ems_device_instance_tag);
    }

    bool modulation_class() const noexcept
    {
        return m_packed_0.get(m_modulation_class_tag);
    }

    void modulation_class(bool value) noexcept
    {
        m_packed_0.set(value, m_modulation_class_tag);
    }

    bool modulation_type() const noexcept
    {
        return m_packed_0.get(m_modulation_type_tag);
    }

    void modulation_type(bool value) noexcept
    {
        m_packed_0.set(value, m_modulation_type_tag);
    }

    bool function_id() const noexcept
    {
        return m_packed_0.get(m_function_id_tag);
    }

    void function_id(bool value) noexcept
    {
        m_packed_0.set(value, m_function_id_tag);
    }

    bool mode_id() const noexcept
    {
        return m_packed_0.get(m_mode_id_tag);
    }

    void mode_id(bool value) noexcept
    {
        m_packed_0.set(value, m_mode_id_tag);
    }

    bool event_id() const noexcept
    {
        return m_packed_0.get(m_event_id_tag);
    }

    void event_id(bool value) noexcept
    {
        m_packed_0.set(value, m_event_id_tag);
    }

    bool function_priority_id() const noexcept
    {
        return m_packed_0.get(m_function_priority_id_tag);
    }

    void function_priority_id(bool value) noexcept
    {
        m_packed_0.set(value, m_function_priority_id_tag);
    }

    bool communication_priority_id() const noexcept
    {
        return m_packed_0.get(m_communication_priority_id_tag);
    }

    void communication_priority_id(bool value) noexcept
    {
        m_packed_0.set(value, m_communication_priority_id_tag);
    }

    bool rf_footprint() const noexcept
    {
        return m_packed_0.get(m_rf_footprint_tag);
    }

    void rf_footprint(bool value) noexcept
    {
        m_packed_0.set(value, m_rf_footprint_tag);
    }

    bool rf_footprint_range() const noexcept
    {
        return m_packed_0.get(m_rf_footprint_range_tag);
    }

    void rf_footprint_range(bool value) noexcept
    {
        m_packed_0.set(value, m_rf_footprint_range_tag);
    }

private:
    /*
     * bind 0/31
     * cited_sid 0/30
     * sibling_sid 0/29
     * parent_sid 0/28
     * child_sid 0/27
     * cited_message_id 0/26
     * controllee_id 0/25
     * controllee_uuid 0/24
     * controller_id 0/23
     * controller_uuid 0/22
     * information_source 0/21
     * track_id 0/20
     * country_code 0/19
     * operator_ 0/18
     * platform_class 0/17
     * platform_instance 0/16
     * platform_display 0/15
     * ems_device_class 0/14 
     * ems_device_type 0/13
     * ems_device_instance 0/12
     * modulation_class 0/11
     * modulation_type 0/10
     * function_id 0/9
     * mode_id 0/8
     * event_id 0/7
     * function_priority_id 0/6
     * communication_priority_id 0/5
     * rf_footprint 0/4
     * rf_footprint_range 0/3
     */
    vrtgen::packed_tag<bool,31,1> m_bind_tag;
    vrtgen::packed_tag<bool,30,1> m_cited_sid_tag;
    vrtgen::packed_tag<bool,29,1> m_sibling_sid_tag;
    vrtgen::packed_tag<bool,28,1> m_parent_sid_tag;
    vrtgen::packed_tag<bool,27,1> m_child_sid_tag;
    vrtgen::packed_tag<bool,26,1> m_cited_message_id_tag;
    vrtgen::packed_tag<bool,25,1> m_controllee_id_tag;
    vrtgen::packed_tag<bool,24,1> m_controllee_uuid_tag;
    vrtgen::packed_tag<bool,23,1> m_controller_id_tag;
    vrtgen::packed_tag<bool,22,1> m_controller_uuid_tag;
    vrtgen::packed_tag<bool,21,1> m_information_source_tag;
    vrtgen::packed_tag<bool,20,1> m_track_id_tag;
    vrtgen::packed_tag<bool,19,1> m_country_code_tag;
    vrtgen::packed_tag<bool,18,1> m_operator_tag;
    vrtgen::packed_tag<bool,17,1> m_platform_class_tag;
    vrtgen::packed_tag<bool,16,1> m_platform_instance_tag;
    vrtgen::packed_tag<bool,15,1> m_platform_display_tag;
    vrtgen::packed_tag<bool,14,1> m_ems_device_class_tag;
    vrtgen::packed_tag<bool,13,1> m_ems_device_type_tag;
    vrtgen::packed_tag<bool,12,1> m_ems_device_instance_tag;
    vrtgen::packed_tag<bool,11,1> m_modulation_class_tag;
    vrtgen::packed_tag<bool,10,1> m_modulation_type_tag;
    vrtgen::packed_tag<bool,9,1> m_function_id_tag;
    vrtgen::packed_tag<bool,8,1> m_mode_id_tag;
    vrtgen::packed_tag<bool,7,1> m_event_id_tag;
    vrtgen::packed_tag<bool,6,1> m_function_priority_id_tag;
    vrtgen::packed_tag<bool,5,1> m_communication_priority_id_tag;
    vrtgen::packed_tag<bool,4,1> m_rf_footprint_tag;
    vrtgen::packed_tag<bool,3,1> m_rf_footprint_range_tag;

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
        return m_packed_0.get(m_timestamp_details_tag);
    }

    void timestamp_details(bool value) noexcept
    {
        m_packed_0.set(value, m_timestamp_details_tag);
    }

    bool timestamp_skew() const noexcept
    {
        return m_packed_0.get(m_timestamp_skew_tag);
    }

    void timestamp_skew(bool value) noexcept
    {
        m_packed_0.set(value, m_timestamp_skew_tag);
    }

    bool rise_time() const noexcept
    {
        return m_packed_0.get(m_rise_time_tag);
    }

    void rise_time(bool value) noexcept
    {
        m_packed_0.set(value, m_rise_time_tag);
    }

    bool fall_time() const noexcept
    {
        return m_packed_0.get(m_fall_time_tag);
    }

    void fall_time(bool value) noexcept
    {
        m_packed_0.set(value, m_fall_time_tag);
    }

    bool offset_time() const noexcept
    {
        return m_packed_0.get(m_offset_time_tag);
    }

    void offset_time(bool value) noexcept
    {
        m_packed_0.set(value, m_offset_time_tag);
    }

    bool pulse_width() const noexcept
    {
        return m_packed_0.get(m_pulse_width_tag);
    }

    void pulse_width(bool value) noexcept
    {
        m_packed_0.set(value, m_pulse_width_tag);
    }

    bool period() const noexcept
    {
        return m_packed_0.get(m_period_tag);
    }

    void period(bool value) noexcept
    {
        m_packed_0.set(value, m_period_tag);
    }

    bool duration() const noexcept
    {
        return m_packed_0.get(m_duration_tag);
    }

    void duration(bool value) noexcept
    {
        m_packed_0.set(value, m_duration_tag);
    }

    bool dwell() const noexcept
    {
        return m_packed_0.get(m_dwell_tag);
    }

    void dwell(bool value) noexcept
    {
        m_packed_0.set(value, m_dwell_tag);
    }

    bool jitter() const noexcept
    {
        return m_packed_0.get(m_jitter_tag);
    }

    void jitter(bool value) noexcept
    {
        m_packed_0.set(value, m_jitter_tag);
    }

    bool age() const noexcept
    {
        return m_packed_0.get(m_age_tag);
    }

    void age(bool value) noexcept
    {
        m_packed_0.set(value, m_age_tag);
    }

    bool shelf_life() const noexcept
    {
        return m_packed_0.get(m_shelf_life_tag);
    }

    void shelf_life(bool value) noexcept
    {
        m_packed_0.set(value, m_shelf_life_tag);
    }

    bool air_temperature() const noexcept
    {
        return m_packed_0.get(m_air_temperature_tag);
    }

    void air_temperature(bool value) noexcept
    {
        m_packed_0.set(value, m_air_temperature_tag);
    }

    bool sea_ground_temperature() const noexcept
    {
        return m_packed_0.get(m_sea_ground_temperature_tag);
    }

    void sea_ground_temperature(bool value) noexcept
    {
        m_packed_0.set(value, m_sea_ground_temperature_tag);
    }

    bool humidity() const noexcept
    {
        return m_packed_0.get(m_humidity_tag);
    }

    void humidity(bool value) noexcept
    {
        m_packed_0.set(value, m_humidity_tag);
    }

    bool barometric_pressure() const noexcept
    {
        return m_packed_0.get(m_barometric_pressure_tag);
    }

    void barometric_pressure(bool value) noexcept
    {
        m_packed_0.set(value, m_barometric_pressure_tag);
    }

    bool sea_swell_state() const noexcept
    {
        return m_packed_0.get(m_sea_swell_state_tag);
    }

    void sea_swell_state(bool value) noexcept
    {
        m_packed_0.set(value, m_sea_swell_state_tag);
    }

    bool tropospheric_state() const noexcept
    {
        return m_packed_0.get(m_tropospheric_state_tag);
    }

    void tropospheric_state(bool value) noexcept
    {
        m_packed_0.set(value, m_tropospheric_state_tag);
    }

    bool network_id() const noexcept
    {
        return m_packed_0.get(m_network_id_tag);
    }

    void network_id(bool value) noexcept
    {
        m_packed_0.set(value, m_network_id_tag);
    }

private:
    /*
     * timestamp_details 0/31
     * timestamp_skew 0/30
     * rise_time 0/27
     * fall_time 0/26
     * offset_time 0/25
     * pulse_width 0/24
     * period 0/23
     * duration 0/22
     * dwell 0/21
     * jitter 0/20
     * age 0/17
     * shelf_life 0/16
     * air_temperature 0/7
     * sea_ground_temperature 0/6
     * humidity 0/5
     * barometric_pressure 0/4
     * sea_swell_state 0/3
     * tropospheric_state 0/2
     * network_id 0/1
     */
    vrtgen::packed_tag<bool,31,1> m_timestamp_details_tag;
    vrtgen::packed_tag<bool,30,1> m_timestamp_skew_tag;
    vrtgen::packed_tag<bool,27,1> m_rise_time_tag;
    vrtgen::packed_tag<bool,26,1> m_fall_time_tag;
    vrtgen::packed_tag<bool,25,1> m_offset_time_tag;
    vrtgen::packed_tag<bool,24,1> m_pulse_width_tag;
    vrtgen::packed_tag<bool,23,1> m_period_tag;
    vrtgen::packed_tag<bool,22,1> m_duration_tag;
    vrtgen::packed_tag<bool,21,1> m_dwell_tag;
    vrtgen::packed_tag<bool,20,1> m_jitter_tag;
    vrtgen::packed_tag<bool,17,1> m_age_tag;
    vrtgen::packed_tag<bool,16,1> m_shelf_life_tag;
    vrtgen::packed_tag<bool,7,1> m_air_temperature_tag;
    vrtgen::packed_tag<bool,6,1> m_sea_ground_temperature_tag;
    vrtgen::packed_tag<bool,5,1> m_humidity_tag;
    vrtgen::packed_tag<bool,4,1> m_barometric_pressure_tag;
    vrtgen::packed_tag<bool,3,1> m_sea_swell_state_tag;
    vrtgen::packed_tag<bool,2,1> m_tropospheric_state_tag;
    vrtgen::packed_tag<bool,1,1> m_network_id_tag;

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
     * See VITA 49.2 Table 9.12-2
     */
    bool current_value() const noexcept
    {
        return m_packed_0.get(m_current_value_tag);
    }

    /**
     * @brief Sets the Current Value Indicator flag
     * @param value true if Current Value is attached to each field
     * 
     * Current Value Indicator is 1 bit long at bit position 31
     * See VITA 49.2 Table 9.12-2
     */
    void current_value(bool value) noexcept
    {
        m_packed_0.set(value, m_current_value_tag);
    }

    /**
     * @brief Returns the Mean Value Indicator flag
     * @return true if Mean Value is attached to each field
     * 
     * Mean Value Indicator is 1 bit long at bit position 30
     * See VITA 49.2 Table 9.12-2
     */
    bool mean_value() const noexcept
    {
        return m_packed_0.get(m_mean_value_tag);
    }

    /**
     * @brief Sets the Mean Value Indicator flag
     * @param value true if Mean Value is attached to each field
     * 
     * Mean Value Indicator is 1 bit long at bit position 30
     * See VITA 49.2 Table 9.12-2
     */
    void mean_value(bool value) noexcept
    {
        m_packed_0.set(value, m_mean_value_tag);
    }

    /**
     * @brief Returns the Median Value Indicator flag
     * @return true if Median Value is attached to each field
     * 
     * Median Value Indicator is 1 bit long at bit position 29
     * See VITA 49.2 Table 9.12-2
     */
    bool median_value() const noexcept
    {
        return m_packed_0.get(m_median_value_tag);
    }

    /**
     * @brief Sets the Median Value Indicator flag
     * @param value true if Median Value is attached to each field
     * 
     * Median Value Indicator is 1 bit long at bit position 29
     * See VITA 49.2 Table 9.12-2
     */
    void median_value(bool value) noexcept
    {
        m_packed_0.set(value, m_median_value_tag);
    }

    /**
     * @brief Returns the Standard Deviation Value Indicator flag
     * @return true if Standard Deviation Value is attached to each field
     * 
     * Standard Deviation Value Indicator is 1 bit long at bit position 28
     * See VITA 49.2 Table 9.12-2
     */
    bool standard_deviation() const noexcept
    {
        return m_packed_0.get(m_standard_deviation_tag);
    }

    /**
     * @brief Sets the Standard Deviation Value Indicator flag
     * @param value true if Standard Deviation Value is attached to each field
     * 
     * Standard Deviation Value Indicator is 1 bit long at bit position 28
     * See VITA 49.2 Table 9.12-2
     */
    void standard_deviation(bool value) noexcept
    {
        m_packed_0.set(value, m_standard_deviation_tag);
    }

    /**
     * @brief Returns the Maximum Value Indicator flag
     * @return true if Maximum Value is attached to each field
     * 
     * Maximum Value Indicator is 1 bit long at bit position 27
     * See VITA 49.2 Table 9.12-2
     */
    bool max_value() const noexcept
    {
        return m_packed_0.get(m_max_value_tag);
    }

    /**
     * @brief Sets the Maximum Value Indicator flag
     * @param value true if Maximum Value is attached to each field
     * 
     * Maximum Value Indicator is 1 bit long at bit position 27
     * See VITA 49.2 Table 9.12-2
     */
    void max_value(bool value) noexcept
    {
        m_packed_0.set(value, m_max_value_tag);
    }

    /**
     * @brief Returns the Minimum Value Indicator flag
     * @return true if Minimum Value is attached to each field
     * 
     * Minimum Value Indicator is 1 bit long at bit position 26
     * See VITA 49.2 Table 9.12-2
     */
    bool min_value() const noexcept
    {
        return m_packed_0.get(m_min_value_tag);
    }

    /**
     * @brief Sets the Minimum Value Indicator flag
     * @param value true if Minimum Value is attached to each field
     * 
     * Minimum Value Indicator is 1 bit long at bit position 26
     * See VITA 49.2 Table 9.12-2
     */
    void min_value(bool value) noexcept
    {
        m_packed_0.set(value, m_min_value_tag);
    }

    /**
     * @brief Returns the Precision Value Indicator flag
     * @return true if Precision Value is attached to each field
     * 
     * Precision Value Indicator is 1 bit long at bit position 25
     * See VITA 49.2 Table 9.12-2
     */
    bool precision() const noexcept
    {
        return m_packed_0.get(m_precision_tag);
    }

    /**
     * @brief Sets the Precision Value Indicator flag
     * @param value true if Precision Value is attached to each field
     * 
     * Precision Value Indicator is 1 bit long at bit position 25
     * See VITA 49.2 Table 9.12-2
     */
    void precision(bool value) noexcept
    {
        m_packed_0.set(value, m_precision_tag);
    }

    /**
     * @brief Returns the Accuracy Indicator flag
     * @return true if Accuracy is attached to each field
     * 
     * Accuracy Indicator is 1 bit long at bit position 24
     * See VITA 49.2 Table 9.12-2
     */
    bool accuracy() const noexcept
    {
        return m_packed_0.get(m_accuracy_tag);
    }

    /**
     * @brief Sets the Accuracy Indicator flag
     * @param value true if Accuracy is attached to each field
     * 
     * Accuracy Indicator is 1 bit long at bit position 24
     * See VITA 49.2 Table 9.12-2
     */
    void accuracy(bool value) noexcept
    {
        m_packed_0.set(value, m_accuracy_tag);
    }

    /**
     * @brief Returns the First Derivative Indicator flag
     * @return true if First Derivative is attached to each field
     * 
     * First Derivative Indicator is 1 bit long at bit position 23
     * See VITA 49.2 Table 9.12-2
     */
    bool first_derivative() const noexcept
    {
        return m_packed_0.get(m_first_derivative_tag);
    }

    /**
     * @brief Sets the First Derivative Indicator flag
     * @param value true if First Derivative is attached to each field
     * 
     * First Derivative Indicator is 1 bit long at bit position 23
     * See VITA 49.2 Table 9.12-2
     */
    void first_derivative(bool value) noexcept
    {
        m_packed_0.set(value, m_first_derivative_tag);
    }

    /**
     * @brief Returns the Second Derivative Indicator flag
     * @return true if First Derivative is attached to each field
     * 
     * First Derivative Indicator is 1 bit long at bit position 22
     * See VITA 49.2 Table 9.12-2
     */
    bool second_derivative() const noexcept
    {
        return m_packed_0.get(m_second_derivative_tag);
    }

    /**
     * @brief Sets the Second Derivative Indicator flag
     * @param value true if Second Derivative is attached to each field
     * 
     * Second Derivative Indicator is 1 bit long at bit position 22
     * See VITA 49.2 Table 9.12-2
     */
    void second_derivative(bool value) noexcept
    {
        m_packed_0.set(value, m_second_derivative_tag);
    }

    /**
     * @brief Returns the Third Derivative Indicator flag
     * @return true if Third Derivative is attached to each field
     * 
     * Third Derivative Indicator is 1 bit long at bit position 21
     * See VITA 49.2 Table 9.12-2
     */
    bool third_derivative() const noexcept
    {
        return m_packed_0.get(m_third_derivative_tag);
    }

    /**
     * @brief Sets the Third Derivative Indicator flag
     * @param value true if Third Derivative is attached to each field
     * 
     * Third Derivative Indicator is 1 bit long at bit position 21
     * See VITA 49.2 Table 9.12-2
     */
    void third_derivative(bool value) noexcept
    {
        m_packed_0.set(value, m_third_derivative_tag);
    }

    /**
     * @brief Returns the Probability Indicator flag
     * @return true if Probability is attached to each field
     * 
     * Probability Indicator is 1 bit long at bit position 20
     * See VITA 49.2 Table 9.12-2
     */
    bool probability() const noexcept
    {
        return m_packed_0.get(m_probability_tag);
    }

    /**
     * @brief Sets the Probability Indicator flag
     * @param value true if Probability is attached to each field
     * 
     * Probability Indicator is 1 bit long at bit position 20
     * See VITA 49.2 Table 9.12-2
     */
    void probability(bool value) noexcept
    {
        m_packed_0.set(value, m_probability_tag);
    }

    /**
     * @brief Returns the Belief Indicator flag
     * @return true if Belief is attached to each field
     * 
     * Belief Indicator is 1 bit long at bit position 19
     * See VITA 49.2 Table 9.12-2
     */
    bool belief() const noexcept
    {
        return m_packed_0.get(m_belief_tag);
    }

    /**
     * @brief Sets the Belief Indicator flag
     * @param value true if Belief is attached to each field
     * 
     * Belief Indicator is 1 bit long at bit position 19
     * See VITA 49.2 Table 9.12-2
     */
    void belief(bool value) noexcept
    {
        m_packed_0.set(value, m_belief_tag);
    }

private:
    /*
     * current_value 0/31
     * mean_value 0/30
     * median_value 0/29
     * standard_deviation 0/28
     * max_value 0/27
     * min_value 0/26
     * precision 0/25
     * accuracy 0/24
     * first_derivative 0/23
     * second_derivative 0/22
     * third_derivative 0/21
     * probability 0/20
     * belief 0/19
     */
    vrtgen::packed_tag<bool,31,1> m_current_value_tag;
    vrtgen::packed_tag<bool,30,1> m_mean_value_tag;
    vrtgen::packed_tag<bool,29,1> m_median_value_tag;
    vrtgen::packed_tag<bool,28,1> m_standard_deviation_tag;
    vrtgen::packed_tag<bool,27,1> m_max_value_tag;
    vrtgen::packed_tag<bool,26,1> m_min_value_tag;
    vrtgen::packed_tag<bool,25,1> m_precision_tag;
    vrtgen::packed_tag<bool,24,1> m_accuracy_tag;
    vrtgen::packed_tag<bool,23,1> m_first_derivative_tag;
    vrtgen::packed_tag<bool,22,1> m_second_derivative_tag;
    vrtgen::packed_tag<bool,21,1> m_third_derivative_tag;
    vrtgen::packed_tag<bool,20,1> m_probability_tag;
    vrtgen::packed_tag<bool,19,1> m_belief_tag;

}; // end class IndicatorField7

} // end namespace vrtgen::packing

#endif // _VRTGEN_PACKING_INDICATOR_FIELDS_HPP
