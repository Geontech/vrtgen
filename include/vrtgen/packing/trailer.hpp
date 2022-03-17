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
#ifndef _VRTGEN_PACKING_TRAILER_HPP
#define _VRTGEN_PACKING_TRAILER_HPP

#include <vrtgen/types.hpp>
#include <vrtgen/packing/enums.hpp>
#include <vector>
#include <type_traits>

namespace vrtgen::packing {

/**
 * @class Trailer
 * @brief Signal Data Packet trailer (5.1.6)
 */
class Trailer
{
public:
    /**
     * @brief Returns the Calibrated Time Enable flag
     * @return true if Calibrated Time Indicator is enabled, otherwise false
     * 
     * Calibrated Time Enable is 1 bit long at bit position 31
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
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
     * See VITA 49.2 Table 5.1.6-1
     */
    void sample_loss(bool value) noexcept
    {
        m_packed_0.set(value, m_sample_loss_tag);
    }

    /**
     * @brief Returns the Associated Context Packets Count Enable flag
     * @return true if Associated Context Packets Count is enabled, otherwise false
     * 
     * Associated Context Packets Count Enable is 1 bit long at bit position 7
     * See VITA 49.2 Table 5.1.6-1
     */
    bool associated_context_packets_count_enable() const noexcept
    {
        return m_packed_0.get(m_associated_context_packets_count_enable_tag);
    }

    /**
     * @brief Sets the Associated Context Packets Count Enable flag
     * @param value Associated Context Packets Count Enable flag value to set
     * 
     * Associated Context Packets Count Enable is 1 bit long at bit position 7
     * See VITA 49.2 Table 5.1.6-1
     */
    void associated_context_packets_count_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_associated_context_packets_count_enable_tag);
    }

    /**
     * @brief Returns the Associated Context Packets Count subfield value
     * @return true if Associated Context Packets Count is enabled, otherwise false
     * 
     * Associated Context Packets Enable is 7 bits long at bit position 6
     * See VITA 49.2 Table 5.1.6-1
     */
    uint8_t associated_context_packets_count() const noexcept
    {
        return m_packed_0.get(m_associated_context_packets_count_tag);
    }

    /**
     * @brief Sets the Associated Context Packets Count subfield value
     * @param value Associated Context Packets Count subfield value to set
     * 
     * Associated Context Packets Count is 7 bits long at bit position 6
     * See VITA 49.2 Table 5.1.6-1
     */
    void associated_context_packets_count(uint8_t value) noexcept
    {
        m_packed_0.set(value, m_associated_context_packets_count_tag);
    }

    /**
     * @brief Returns the number of Trailer bytes
     * @return Number of Trailer bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return m_packed_0.size();
    }

    /**
     * @brief Pack Trailer as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add Trailer bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed_0.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into Trailer
     * @param buffer_ptr Pointer to beginning of Trailer bytes in the buffer
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
    vrtgen::packed_tag<bool,7,1> m_associated_context_packets_count_enable_tag;
    vrtgen::packed_tag<uint8_t,6,7> m_associated_context_packets_count_tag;
    vrtgen::packed<uint32_t> m_packed_0;

}; // end class Trailer

} // end namespace vrtgen::packing

#endif // _VRTGEN_PACKING_TRAILER_HPP
