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
#ifndef _VRTGEN_PACKING_CIF0_HPP
#define _VRTGEN_PACKING_CIF0_HPP

#include <vrtgen/types.hpp>
#include "indicator_fields.hpp"
#include "enums.hpp"

namespace vrtgen::packing {

/**
 * @class CIF0
 * @brief Context/Command Indicator Field 0 (9.1)
 */
class CIF0 : public IndicatorField0
{
public:
    /**
     * @brief Returns the Command/Context Indicator Field 7 enable flag
     * @return true if packet contains Command/Context Indicator Field 7 word, otherwise false
     * 
     * Command/Context Indicator Field 7 Enable is 1 bit long at bit position 7
     * See VITA 49.2 Table 9.1-1
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
     * See VITA 49.2 Table 9.1-1
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
     * See VITA 49.2 Table 9.1-1
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
     * See VITA 49.2 Table 9.1-1
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
     * See VITA 49.2 Table 9.1-1
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
     * See VITA 49.2 Table 9.1-1
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
     * See VITA 49.2 Table 9.1-1
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
     * See VITA 49.2 Table 9.1-1
     */
    void cif1_enable(bool value) noexcept
    {
        IndicatorField0::if1_enable(value);
    }

}; // end class CIF0

/**
 * @class Gain
 * @brief Gain/Attenuation field (9.5.3)
 */
class Gain
{
public:
    /**
     * @brief Returns the Stage 2 Gain subfield value
     * @return Stage 2 Gain subfield value
     * 
     * Stage 2 Gain subfield is 16 bits long at bit position 31 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2 Figure 9.5.3-1
     */
    float stage_2() const noexcept
    {
        return vrtgen::fixed::to_fp<16,7>(vrtgen::swap::from_be(m_stage_2));
    }

    /**
     * @brief Sets the Stage 2 Gain subfield value
     * @param value Stage 2 Gain subfield value to set
     * 
     * Stage 2 Gain subfield is 16 bits long at bit position 31 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2 Figure 9.5.3-1
     */
    void stage_2(float value) noexcept
    {
        m_stage_2 = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,7>(value));
    }

    /**
     * @brief Returns the Stage 2 Gain subfield value
     * @return Stage 2 Gain subfield value
     * 
     * Stage 1 Gain subfield is 16 bits long at bit position 15 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2 Figure 9.5.3-1
     */
    float stage_1() const noexcept
    {
        return vrtgen::fixed::to_fp<16,7>(vrtgen::swap::from_be(m_stage_1));
    }

    /**
     * @brief Sets the Stage 1 Gain subfield value
     * @param value Stage 1 Gain subfield value to set
     * 
     * Stage 1 Gain subfield is 16 bits long at bit position 15 expressed in
     * two's-complement format with a radix point to the right of bit 7
     * See VITA 49.2 Figure 9.5.3-1
     */
    void stage_1(float value) noexcept
    {
        m_stage_1 = vrtgen::swap::to_be(vrtgen::fixed::to_int<16,7>(value));
    }

    /**
     * @brief Returns the number of Gain bytes
     * @return Number of Gain bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return sizeof(m_stage_2) +
               sizeof(m_stage_1);
    }

    /**
     * @brief Pack Gain as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add Gain bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        std::memcpy(buffer_ptr, &m_stage_2, sizeof(m_stage_2));
        buffer_ptr += sizeof(m_stage_2);
        std::memcpy(buffer_ptr, &m_stage_1, sizeof(m_stage_1));
    }

    /**
     * @brief Unpack buffer bytes into Gain
     * @param buffer_ptr Pointer to beginning of Gain bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        std::memcpy(&m_stage_2, ptr, sizeof(m_stage_2));
        ptr += sizeof(m_stage_2);
        std::memcpy(&m_stage_1, ptr, sizeof(m_stage_1));
    }

private:
    int16_t m_stage_2{ 0 }; //!< stage_2 gain subfield 0/31 
    int16_t m_stage_1{ 0 }; //!< stage_1 gain subfield 0/15 

}; // end class Gain

/**
 * @class DeviceIdentifier
 * @brief Device Identifier Field (9.10.1)
 */
class DeviceIdentifier
{
public:
    /**
     * @brief Returns the Manufacturer OUI subfield value
     * @return Manufacturer OUI subfield value
     * 
     * Manufacturer OUI subfield is 24 bits long at bit position 23 in word 1
     * See VITA 49.2 Figure 9.10.1-1
     */
    uint32_t manufacturer_oui() const noexcept
    {
        return m_manufacturer_oui.get();
    }

    /**
     * @brief Sets the Manufacturer OUI subfield value
     * @param value Manufacturer OUI subfield value to set
     * 
     * Manufacturer OUI subfield is 24 bits long at bit position 23 in word 1
     * See VITA 49.2 Figure 9.10.1-1
     */
    void manufacturer_oui(uint32_t value) noexcept
    {
        m_manufacturer_oui.set(value);
    }

    /**
     * @brief Returns the Device Code subfield value
     * @return Device Code subfield value
     * 
     * Device Code subfield is 16 bits long at bit position 15 in word 2
     * See VITA 49.2 Figure 9.10.1-1
     */
    uint16_t device_code() const noexcept
    {
        return vrtgen::swap::from_be(m_device_code);
    }

    /**
     * @brief Sets the Device Code subfield value
     * @param value Device Code subfield value to set
     * 
     * Device Code subfield is 16 bits long at bit position 15 in word 2
     * See VITA 49.2 Figure 9.10.1-1
     */
    void device_code(uint16_t value) noexcept
    {
        m_device_code = vrtgen::swap::to_be(value);
    }

    /**
     * @brief Returns the number of DeviceIdentifier bytes
     * @return Number of DeviceIdentifier bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return sizeof(m_reserved_0) +
               m_manufacturer_oui.size() +
               sizeof(m_reserved_1) +
               sizeof(m_device_code);
    }

    /**
     * @brief Pack DeviceIdentifier as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add DeviceIdentifier bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        std::memcpy(buffer_ptr, &m_reserved_0, sizeof(m_reserved_0));
        buffer_ptr += sizeof(m_reserved_0);
        m_manufacturer_oui.pack_into(buffer_ptr);
        buffer_ptr += m_manufacturer_oui.size();
        std::memcpy(buffer_ptr, &m_reserved_1, sizeof(m_reserved_1));
        buffer_ptr += sizeof(m_reserved_1);
        std::memcpy(buffer_ptr, &m_device_code, sizeof(m_device_code));
    }

    /**
     * @brief Unpack buffer bytes into DeviceIdentifier
     * @param buffer_ptr Pointer to beginning of DeviceIdentifier bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        ptr += sizeof(m_reserved_0);
        m_manufacturer_oui.unpack_from(ptr);
        ptr += m_manufacturer_oui.size();
        ptr += sizeof(m_reserved_1);
        std::memcpy(&m_device_code, ptr, sizeof(m_device_code));
    }

private:
    /*
     * manufacturer_oui 0/23 
     * device_code 1/15 
     */
    uint8_t m_reserved_0{ 0 };
    vrtgen::OUI m_manufacturer_oui;
    uint16_t m_reserved_1{ 0 };
    uint16_t m_device_code{ 0 };

}; // end class DeviceIdentifier

/**
 * @class StateEventIndicators
 * @brief State and Event Indicator Field (9.10.8)
 */
class StateEventIndicators
{
public:
    /**
     * @brief Returns the Calibrated Time Enable flag
     * @return true if Calibrated Time Indicator is enabled, otherwise false
     * 
     * Calibrated Time Enable is 1 bit long at bit position 31
     * See VITA 49.2 Table 9.10.8-1
     */
    bool calibrated_time_enable() const noexcept
    {
        return m_packed_0.get(m_calibrated_time_enable_tag);
    }

    /**
     * @brief Sets the Calibrated Time Enable flag
     * @param value Calibrated Time Enable flag value to set
     * 
     * Calibrated Time Enable is 1 bit long at bit position 31
     * See VITA 49.2 Table 9.10.8-1
     */
    void calibrated_time_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_calibrated_time_enable_tag);
    }

    /**
     * @brief Returns the Valid Data Enable flag
     * @return true if Valid Data Indicator is enabled, otherwise false
     * 
     * Valid Data Enable is 1 bit long at bit position 30
     * See VITA 49.2 Table 9.10.8-1
     */
    bool valid_data_enable() const noexcept
    {
        return m_packed_0.get(m_valid_data_enable_tag);
    }

    /**
     * @brief Sets the Valid Data Enable flag
     * @param value Valid Data Enable flag value to set
     * 
     * Valid Data Enable is 1 bit long at bit position 30
     * See VITA 49.2 Table 9.10.8-1
     */
    void valid_data_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_valid_data_enable_tag);
    }

    /**
     * @brief Returns the Reference Lock Enable flag
     * @return true if Reference Lock Indicator is enabled, otherwise false
     * 
     * Reference Lock Enable is 1 bit long at bit position 29
     * See VITA 49.2 Table 9.10.8-1
     */
    bool reference_lock_enable() const noexcept
    {
        return m_packed_0.get(m_reference_lock_enable_tag);
    }

    /**
     * @brief Sets the Reference Lock Enable flag
     * @param value Reference Lock Enable flag value to set
     * 
     * Reference Lock Enable is 1 bit long at bit position 29
     * See VITA 49.2 Table 9.10.8-1
     */
    void reference_lock_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_reference_lock_enable_tag);
    }

    /**
     * @brief Returns the AGC/MGC Enable flag
     * @return true if AGC/MGC Indicator is enabled, otherwise false
     * 
     * AGC/MGC Enable is 1 bit long at bit position 28
     * See VITA 49.2 Table 9.10.8-1
     */
    bool agc_mgc_enable() const noexcept
    {
        return m_packed_0.get(m_agc_mgc_enable_tag);
    }

    /**
     * @brief Sets the AGC/MGC Enable flag
     * @param value AGC/MGC Enable flag value to set
     * 
     * AGC/MGC Enable is 1 bit long at bit position 28
     * See VITA 49.2 Table 9.10.8-1
     */
    void agc_mgc_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_agc_mgc_enable_tag);
    }

    /**
     * @brief Returns the Detected Signal Enable flag
     * @return true if Detected Signal Indicator is enabled, otherwise false
     * 
     * Detected Signal Enable is 1 bit long at bit position 27
     * See VITA 49.2 Table 9.10.8-1
     */
    bool detected_signal_enable() const noexcept
    {
        return m_packed_0.get(m_detected_signal_enable_tag);
    }

    /**
     * @brief Sets the Detected Signal Enable flag
     * @param value Detected Signal Enable flag value to set
     * 
     * Detected Signal Enable is 1 bit long at bit position 27
     * See VITA 49.2 Table 9.10.8-1
     */
    void detected_signal_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_detected_signal_enable_tag);
    }

    /**
     * @brief Returns the Spectral Inversion Enable flag
     * @return true if Spectral Inversion Indicator is enabled, otherwise false
     * 
     * Spectral Inversion Enable is 1 bit long at bit position 26
     * See VITA 49.2 Table 9.10.8-1
     */
    bool spectral_inversion_enable() const noexcept
    {
        return m_packed_0.get(m_spectral_inversion_enable_tag);
    }

    /**
     * @brief Sets the Spectral Inversion Enable flag
     * @param value Spectral Inversion Enable flag value to set
     * 
     * Spectral Inversion Enable is 1 bit long at bit position 26
     * See VITA 49.2 Table 9.10.8-1
     */
    void spectral_inversion_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_spectral_inversion_enable_tag);
    }

    /**
     * @brief Returns the Over-Range Enable flag
     * @return true if Over-Range Indicator is enabled, otherwise false
     * 
     * Over-Range Enable is 1 bit long at bit position 25
     * See VITA 49.2 Table 9.10.8-1
     */
    bool over_range_enable() const noexcept
    {
        return m_packed_0.get(m_over_range_enable_tag);
    }

    /**
     * @brief Sets the Over-Range Enable flag
     * @param value Over-Range Enable flag value to set
     * 
     * Over-Range Enable is 1 bit long at bit position 25
     * See VITA 49.2 Table 9.10.8-1
     */
    void over_range_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_over_range_enable_tag);
    }

    /**
     * @brief Returns the Sample Loss Enable flag
     * @return true if Sample Loss Indicator is enabled, otherwise false
     * 
     * Sample Loss Enable is 1 bit long at bit position 24
     * See VITA 49.2 Table 9.10.8-1
     */
    bool sample_loss_enable() const noexcept
    {
        return m_packed_0.get(m_sample_loss_enable_tag);
    }

    /**
     * @brief Sets the Sample Loss Enable flag
     * @param value Sample Loss Enable flag value to set
     * 
     * Sample Loss Enable is 1 bit long at bit position 24
     * See VITA 49.2 Table 9.10.8-1
     */
    void sample_loss_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_sample_loss_enable_tag);
    }

    /**
     * @brief Returns the Calibrated Time Indicator flag
     * @return Calibrated Time Indicator
     * 
     * Calibrated Time Indicator is 1 bit long at bit position 19
     * See VITA 49.2 Table 9.10.8-1
     */
    bool calibrated_time() const noexcept
    {
        return m_packed_0.get(m_calibrated_time_tag);
    }

    /**
     * @brief Sets the Calibrated Time Indicator flag
     * @param value Calibrated Time Indicator flag value to set
     * 
     * Calibrated Time Indicator is 1 bit long at bit position 19
     * See VITA 49.2 Table 9.10.8-1
     */
    void calibrated_time(bool value) noexcept
    {
        m_packed_0.set(value, m_calibrated_time_tag);
    }

    /**
     * @brief Returns the Valid Data Indicator flag
     * @return Valid Data Indicator
     * 
     * Valid Data Indicator is 1 bit long at bit position 18
     * See VITA 49.2 Table 9.10.8-1
     */
    bool valid_data() const noexcept
    {
        return m_packed_0.get(m_valid_data_tag);
    }

    /**
     * @brief Sets the Valid Data Indicator flag
     * @param value Valid Data Indicator flag value to set
     * 
     * Valid Data Indicator is 1 bit long at bit position 18
     * See VITA 49.2 Table 9.10.8-1
     */
    void valid_data(bool value) noexcept
    {
        m_packed_0.set(value, m_valid_data_tag);
    }

    /**
     * @brief Returns the Reference Lock Indicator flag
     * @return Reference Lock Indicator
     * 
     * Reference Lock Indicator is 1 bit long at bit position 17
     * See VITA 49.2 Table 9.10.8-1
     */
    bool reference_lock() const noexcept
    {
        return m_packed_0.get(m_reference_lock_tag);
    }

    /**
     * @brief Sets the Reference Lock Indicator flag
     * @param value Reference Lock Indicator flag value to set
     * 
     * Reference Lock Indicator is 1 bit long at bit position 17
     * See VITA 49.2 Table 9.10.8-1
     */
    void reference_lock(bool value) noexcept
    {
        m_packed_0.set(value, m_reference_lock_tag);
    }

    /**
     * @brief Returns the AGC/MGC Indicator flag
     * @return AGC/MGC Indicator
     * 
     * AGC/MGC Indicator is 1 bit long at bit position 16
     * See VITA 49.2 Table 9.10.8-1
     */
    bool agc_mgc() const noexcept
    {
        return m_packed_0.get(m_agc_mgc_tag);
    }

    /**
     * @brief Sets the AGC/MGC Indicator flag
     * @param value AGC/MGC Indicator flag value to set
     * 
     * AGC/MGC Indicator is 1 bit long at bit position 16
     * See VITA 49.2 Table 9.10.8-1
     */
    void agc_mgc(bool value) noexcept
    {
        m_packed_0.set(value, m_agc_mgc_tag);
    }

    /**
     * @brief Returns the Detected Signal Indicator flag
     * @return Detected Signal Indicator
     * 
     * Detected Signal Indicator is 1 bit long at bit position 15
     * See VITA 49.2 Table 9.10.8-1
     */
    bool detected_signal() const noexcept
    {
        return m_packed_0.get(m_detected_signal_tag);
    }

    /**
     * @brief Sets the Detected Signal Indicator flag
     * @param value Detected Signal Indicator flag value to set
     * 
     * Detected Signal Indicator is 1 bit long at bit position 15
     * See VITA 49.2 Table 9.10.8-1
     */
    void detected_signal(bool value) noexcept
    {
        m_packed_0.set(value, m_detected_signal_tag);
    }

    /**
     * @brief Returns the Spectral Inversion Indicator flag
     * @return Spectral Inversion Indicator
     * 
     * Spectral Inversion Indicator is 1 bit long at bit position 14
     * See VITA 49.2 Table 9.10.8-1
     */
    bool spectral_inversion() const noexcept
    {
        return m_packed_0.get(m_spectral_inversion_tag);
    }

    /**
     * @brief Sets the Spectral Inversion Indicator flag
     * @param value Spectral Inversion Indicator flag value to set
     * 
     * Spectral Inversion Indicator is 1 bit long at bit position 14
     * See VITA 49.2 Table 9.10.8-1
     */
    void spectral_inversion(bool value) noexcept
    {
        m_packed_0.set(value, m_spectral_inversion_tag);
    }

    /**
     * @brief Returns the Over-Range Indicator flag
     * @return Over-Range Indicator
     * 
     * Over-Range Indicator is 1 bit long at bit position 13
     * See VITA 49.2 Table 9.10.8-1
     */
    bool over_range() const noexcept
    {
        return m_packed_0.get(m_over_range_tag);
    }

    /**
     * @brief Sets the Over-Range Indicator flag
     * @param value Over-Range Indicator flag value to set
     * 
     * Over-Range Indicator is 1 bit long at bit position 13
     * See VITA 49.2 Table 9.10.8-1
     */
    void over_range(bool value) noexcept
    {
        m_packed_0.set(value, m_over_range_tag);
    }

    /**
     * @brief Returns the Sample Loss Indicator flag
     * @return Sample Loss Indicator
     * 
     * Sample Loss Indicator is 1 bit long at bit position 12
     * See VITA 49.2 Table 9.10.8-1
     */
    bool sample_loss() const noexcept
    {
        return m_packed_0.get(m_sample_loss_tag);
    }

    /**
     * @brief Sets the Sample Loss Indicator flag
     * @param value Sample Loss Indicator flag value to set
     * 
     * Sample Loss Indicator is 1 bit long at bit position 12
     * See VITA 49.2 Table 9.10.8-1
     */
    void sample_loss(bool value) noexcept
    {
        m_packed_0.set(value, m_sample_loss_tag);
    }

    /**
     * @brief Returns the number of StateEventIndicators bytes
     * @return Number of StateEventIndicators bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return m_packed_0.size();
    }

    /**
     * @brief Pack StateEventIndicators as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add StateEventIndicators bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed_0.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into StateEventIndicators
     * @param buffer_ptr Pointer to beginning of StateEventIndicators bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        m_packed_0.unpack_from(buffer_ptr);
    }

private:
    /**
     * calibrated_time_enable 0/31
     * valid_data_enable 0/30
     * reference_lock_enable 0/29
     * agc_mgc_enable 0/28
     * detected_signal_enable 0/27
     * spectral_inversion_enable 0/26
     * over_range_enable 0/25
     * sample_loss_enable 0/24
     * calibrated_time 0/19
     * valid_data 0/18
     * reference_lock 0/17
     * agc_mgc 0/16
     * detected_signal 0/15
     * spectral_inversion 0/14
     * over_range 0/13
     * sample_loss 0/12
     * associated_context_packets_count_enable 0/7
     * associated_context_packets_count 0/6
     */
    vrtgen::packed_tag<bool,31,1> m_calibrated_time_enable_tag;
    vrtgen::packed_tag<bool,30,1> m_valid_data_enable_tag;
    vrtgen::packed_tag<bool,29,1> m_reference_lock_enable_tag;
    vrtgen::packed_tag<bool,28,1> m_agc_mgc_enable_tag;
    vrtgen::packed_tag<bool,27,1> m_detected_signal_enable_tag;
    vrtgen::packed_tag<bool,26,1> m_spectral_inversion_enable_tag;
    vrtgen::packed_tag<bool,25,1> m_over_range_enable_tag;
    vrtgen::packed_tag<bool,24,1> m_sample_loss_enable_tag;
    vrtgen::packed_tag<bool,19,1> m_calibrated_time_tag;
    vrtgen::packed_tag<bool,18,1> m_valid_data_tag;
    vrtgen::packed_tag<bool,17,1> m_reference_lock_tag;
    vrtgen::packed_tag<bool,16,1> m_agc_mgc_tag;
    vrtgen::packed_tag<bool,15,1> m_detected_signal_tag;
    vrtgen::packed_tag<bool,14,1> m_spectral_inversion_tag;
    vrtgen::packed_tag<bool,13,1> m_over_range_tag;
    vrtgen::packed_tag<bool,12,1> m_sample_loss_tag;
    vrtgen::packed<uint32_t> m_packed_0;

}; // end class StateEventIndicators

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
     * See VITA 49.2 Figure 9.4.5-1
     */
    TSI tsi() const noexcept
    {
        return m_packed_0.get(m_tsi_tag);
    }

    /**
     * @brief Sets the TimeStamp-Integer code subfield value
     * @param value TimeStamp-Integer code subfield value to set
     * 
     * TimeStamp-Integer code subfield is 2 bits long at bit position 27 in word 1
     * See VITA 49.2 Figure 9.4.5-1
     */
    void tsi(TSI value) noexcept
    {
        m_packed_0.set(value, m_tsi_tag);
    }

    /**
     * @brief Returns the TimeStamp-Fractional code subfield value
     * @return TimeStamp-Fractional code subfield value
     * 
     * TimeStamp-Fractional code subfield is 2 bits long at bit position 25 in word 1
     * See VITA 49.2 Figure 9.4.5-1
     */
    TSF tsf() const noexcept
    {
        return m_packed_0.get(m_tsf_tag);
    }

    /**
     * @brief Sets the TimeStamp-Fractional code subfield value
     * @param value TimeStamp-Fractional code subfield value to set
     * 
     * TimeStamp-Fractional code subfield is 2 bits long at bit position 25 in word 1
     * See VITA 49.2 Figure 9.4.5-1
     */
    void tsf(TSF value) noexcept
    {
        m_packed_0.set(value, m_tsf_tag);
    }

    /**
     * @brief Returns the Manufacturer OUI subfield value
     * @return Manufacturer OUI subfield value
     * 
     * Manufacturer OUI subfield is 24 bits long at bit position 23 in word 1
     * See VITA 49.2 Figure 9.4.5-1
     */
    uint32_t manufacturer_oui() const noexcept
    {
        return m_manufacturer_oui.get();
    }

    /**
     * @brief Sets the Manufacturer OUI subfield value
     * @param value Manufacturer OUI subfield value to set
     * 
     * Manufacturer OUI subfield is 24 bits long at bit position 23 in word 1
     * See VITA 49.2 Figure 9.4.5-1
     */
    void manufacturer_oui(uint32_t value) noexcept
    {
        m_manufacturer_oui.set(value);
    }

    /**
     * @brief Returns the Integer Timestamp subfield value
     * @return Integer Timestamp subfield value
     * 
     * Integer Timestamp subfield is 32 bits long at bit position 31 in word 2
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
     * See VITA 49.2 Figure 9.4.5-1
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
        return m_packed_0.size() +
               m_manufacturer_oui.size() +
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
        m_packed_0.pack_into(buffer_ptr);
        buffer_ptr += m_packed_0.size();
        m_manufacturer_oui.pack_into(buffer_ptr);
        buffer_ptr += m_manufacturer_oui.size();
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
        m_packed_0.unpack_from(ptr);
        ptr += m_packed_0.size();
        m_manufacturer_oui.unpack_from(ptr);
        ptr += m_manufacturer_oui.size();
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
    /**
     * TSI 0/27
     * TSF 0/25
     */
    vrtgen::packed_tag<TSI,3,2> m_tsi_tag;
    vrtgen::packed_tag<TSF,1,2> m_tsf_tag;
    vrtgen::packed<uint8_t> m_packed_0;
    vrtgen::OUI m_manufacturer_oui{ 0 };
    uint32_t m_integer_timestamp{ vrtgen::swap::to_be(static_cast<uint32_t>(0xFFFFFFFF)) };
    uint64_t m_fractional_timestamp{ vrtgen::swap::to_be(static_cast<uint64_t>(0xFFFFFFFFFFFFFFFF)) };
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
     * See VITA 49.2 Figure 9.4.5-1
     */
    TSI tsi() const noexcept
    {
        return m_packed_0.get(m_tsi_tag);
    }

    /**
     * @brief Sets the TimeStamp-Integer code subfield value
     * @param value TimeStamp-Integer code subfield value to set
     * 
     * TimeStamp-Integer code subfield is 2 bits long at bit position 27 in word 1
     * See VITA 49.2 Figure 9.4.5-1
     */
    void tsi(TSI value) noexcept
    {
        m_packed_0.set(value, m_tsi_tag);
    }

    /**
     * @brief Returns the TimeStamp-Fractional code subfield value
     * @return TimeStamp-Fractional code subfield value
     * 
     * TimeStamp-Fractional code subfield is 2 bits long at bit position 25 in word 1
     * See VITA 49.2 Figure 9.4.5-1
     */
    TSF tsf() const noexcept
    {
        return m_packed_0.get(m_tsf_tag);
    }

    /**
     * @brief Sets the TimeStamp-Fractional code subfield value
     * @param value TimeStamp-Fractional code subfield value to set
     * 
     * TimeStamp-Fractional code subfield is 2 bits long at bit position 25 in word 1
     * See VITA 49.2 Figure 9.4.5-1
     */
    void tsf(TSF value) noexcept
    {
        m_packed_0.set(value, m_tsf_tag);
    }

    /**
     * @brief Returns the Manufacturer OUI subfield value
     * @return Manufacturer OUI subfield value
     * 
     * Manufacturer OUI subfield is 24 bits long at bit position 23 in word 1
     * See VITA 49.2 Figure 9.10.1-1
     */
    uint32_t manufacturer_oui() const noexcept
    {
        return m_manufacturer_oui.get();
    }

    void manufacturer_oui(uint32_t value) noexcept
    {
        m_manufacturer_oui.set(value);
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
        return m_packed_0.size() +
               m_manufacturer_oui.size() +
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
        m_packed_0.pack_into(buffer_ptr);
        buffer_ptr += m_packed_0.size();
        m_manufacturer_oui.pack_into(buffer_ptr);
        buffer_ptr += m_manufacturer_oui.size();
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
        m_packed_0.unpack_from(ptr);
        ptr += m_packed_0.size();
        m_manufacturer_oui.unpack_from(ptr);
        ptr += m_manufacturer_oui.size();
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
    /**
     * TSI 0/27
     * TSF 0/25
     */
    vrtgen::packed_tag<TSI,3,2> m_tsi_tag;
    vrtgen::packed_tag<TSF,1,2> m_tsf_tag;
    vrtgen::packed<uint8_t> m_packed_0;
    vrtgen::OUI m_manufacturer_oui{ 0 };
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
 * @class PayloadFormat
 * @brief Signal Data Packet Payload Format Field (9.13.3)
 */
class PayloadFormat
{
public:
    PackingMethod packing_method() const noexcept
    {
        return m_packed_0.get(m_packing_method_tag);
    }

    void packing_method(PackingMethod value) noexcept
    {
        m_packed_0.set(value, m_packing_method_tag);
    }

    DataSampleType real_complex_type() const noexcept
    {
        return m_packed_0.get(m_real_complex_type_tag);
    }

    void real_complex_type(DataSampleType value) noexcept
    {
        m_packed_0.set(value, m_real_complex_type_tag);
    }

    DataItemFormat data_item_format() const noexcept
    {
        return m_packed_0.get(m_data_item_format_tag);
    }

    void data_item_format(DataItemFormat value) noexcept
    {
        m_packed_0.set(value, m_data_item_format_tag);
    }

    bool repeat_indicator() const noexcept
    {
        return m_packed_0.get(m_repeat_indicator_tag);
    }

    void repeat_indicator(bool value) noexcept
    {
        m_packed_0.set(value, m_repeat_indicator_tag);
    }

    uint8_t event_tag_size() const noexcept
    {
        return m_packed_0.get(m_event_tag_size_tag);
    }

    void event_tag_size(uint8_t value) noexcept
    {
        m_packed_0.set(value, m_event_tag_size_tag);
    }

    uint8_t channel_tag_size() const noexcept
    {
        return m_packed_0.get(m_channel_tag_size_tag);
    }

    void channel_tag_size(uint8_t value) noexcept
    {
        m_packed_0.set(value, m_channel_tag_size_tag);
    }

    uint8_t data_item_fraction_size() const noexcept
    {
        return m_packed_0.get(m_data_item_fraction_size_tag);
    }

    void data_item_fraction_size(uint8_t value) noexcept
    {
        m_packed_0.set(value, m_data_item_fraction_size_tag);
    }

    uint8_t item_packing_field_size() const noexcept
    {
        return m_packed_0.get(m_item_packing_field_size_tag) + 1;
    }

    void item_packing_field_size(uint8_t value) noexcept
    {
        m_packed_0.set(value - 1, m_item_packing_field_size_tag);
    }

    uint8_t data_item_size() const noexcept
    {
        return m_packed_0.get(m_data_item_size_tag) + 1;
    }

    void data_item_size(uint8_t value) noexcept
    {
        m_packed_0.set(value - 1, m_data_item_size_tag);
    }

    uint16_t repeat_count() const noexcept
    {
        return vrtgen::swap::from_be(m_repeat_count + 1);
    }

    void repeat_count(uint16_t value) noexcept
    {
        m_repeat_count = vrtgen::swap::to_be(value - 1);
    }

    uint16_t vector_size() const noexcept
    {
        return vrtgen::swap::from_be(m_vector_size + 1);
    }

    void vector_size(uint16_t value) noexcept
    {
        m_vector_size = vrtgen::swap::to_be(value - 1);
    }

    /**
     * @brief Returns the number of PayloadFormat bytes
     * @return Number of PayloadFormat bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return m_packed_0.size() +
               sizeof(m_repeat_count) +
               sizeof(m_vector_size);
    }

    /**
     * @brief Pack PayloadFormat as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add PayloadFormat bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed_0.pack_into(buffer_ptr);
        buffer_ptr += m_packed_0.size();
        std::memcpy(buffer_ptr, &m_repeat_count, sizeof(m_repeat_count));
        buffer_ptr += sizeof(m_repeat_count);
        std::memcpy(buffer_ptr, &m_vector_size, sizeof(m_vector_size));
    }

    /**
     * @brief Unpack buffer bytes into PayloadFormat
     * @param buffer_ptr Pointer to beginning of PayloadFormat bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto* ptr = buffer_ptr;
        m_packed_0.unpack_from(ptr);
        ptr += m_packed_0.size();
        std::memcpy(&m_repeat_count, ptr, sizeof(m_repeat_count));
        ptr += sizeof(m_repeat_count);
        std::memcpy(&m_vector_size, ptr, sizeof(m_vector_size));
    }

private:
    /**
     * packing_method 0/31
     * real_complex_type 0/30
     * data_item_format 0/28
     * repeat_indicator 0/23
     * event_tag_size 0/22
     * channel_tag_size 0/19
     * data_item_fraction_size 0/15
     * item_packing_field_size 0/11
     * data_item_size 0/5
     */
    vrtgen::packed_tag<PackingMethod,31,1> m_packing_method_tag;
    vrtgen::packed_tag<DataSampleType,30,2> m_real_complex_type_tag;
    vrtgen::packed_tag<DataItemFormat,28,5> m_data_item_format_tag;
    vrtgen::packed_tag<bool,23,1> m_repeat_indicator_tag;
    vrtgen::packed_tag<uint8_t,22,3> m_event_tag_size_tag;
    vrtgen::packed_tag<uint8_t,19,4> m_channel_tag_size_tag;
    vrtgen::packed_tag<uint8_t,15,4> m_data_item_fraction_size_tag;
    vrtgen::packed_tag<uint8_t,11,6> m_item_packing_field_size_tag;
    vrtgen::packed_tag<uint8_t,5,6> m_data_item_size_tag;
    vrtgen::packed<uint32_t> m_packed_0;
    uint16_t m_repeat_count{ 0 };
    uint16_t m_vector_size{ 0 };

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
        return m_packed_0.get(m_source_list_size_tag);
    }

    void source_list_size(uint16_t value) noexcept
    {
        m_packed_0.set(value, m_source_list_size_tag);
    }

    uint16_t system_list_size() const noexcept
    {
        return m_packed_0.get(m_system_list_size_tag);
    }

    void system_list_size(uint16_t value) noexcept
    {
        m_packed_0.set(value, m_system_list_size_tag);
    }

    uint16_t vector_component_list_size() const noexcept
    {
        return m_packed_1.get(m_vector_component_list_size_tag);
    }

    void vector_component_list_size(uint16_t value) noexcept
    {
        m_packed_1.set(value, m_vector_component_list_size_tag);
    }

    bool async_channel_tag_list_enable() const noexcept
    {
        return m_packed_1.get(m_async_channel_tag_list_enable_tag);
    }

    void async_channel_tag_list_enable(bool value) noexcept
    {
        m_packed_1.set(value, m_async_channel_tag_list_enable_tag);
    }

    uint16_t async_channel_list_size() const noexcept
    {
        return m_packed_1.get(m_async_channel_list_size_tag);
    }

    void async_channel_list_size(uint16_t value) noexcept
    {
        m_packed_1.set(value, m_async_channel_list_size_tag);
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
        retval += m_packed_0.size();
        retval += m_packed_1.size();
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
        m_packed_0.pack_into(buffer_ptr);
        buffer_ptr += m_packed_0.size();
        m_packed_1.pack_into(buffer_ptr);
        buffer_ptr += m_packed_1.size();
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
        m_packed_0.unpack_from(ptr);
        ptr += m_packed_0.size();
        m_packed_1.unpack_from(ptr);
        ptr += m_packed_1.size();
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
    /**
     * source_list_size 0/24
     * system_list_size 0/8
     */
    vrtgen::packed_tag<uint16_t,24,9> m_source_list_size_tag;
    vrtgen::packed_tag<uint16_t,8,9> m_system_list_size_tag;
    vrtgen::packed<uint32_t> m_packed_0;
    /**
     * vector_component_list_size 1/31
     * async_channel_tag_list_enable 1/15
     * async_channel_list_size 1/14
     */
    vrtgen::packed_tag<uint16_t,31,16> m_vector_component_list_size_tag;
    vrtgen::packed_tag<bool,15,1> m_async_channel_tag_list_enable_tag;
    vrtgen::packed_tag<uint16_t,14,15> m_async_channel_list_size_tag;
    vrtgen::packed<uint32_t> m_packed_1;
    std::vector<uint32_t> m_source_list;
    std::vector<uint32_t> m_system_list;
    std::vector<uint32_t> m_vector_component_list;
    std::vector<uint32_t> m_async_channel_list;
    std::vector<uint32_t> m_async_channel_tag_list;

}; // end class ContextAssociationLists

} // end namespace vrtgen::packing

#endif // _VRTGEN_PACKING_CIF0_HPP
