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
        return m_packed.get<31>();
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
        m_packed.set<31>(value);
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
        return m_packed.get<30>();
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
        m_packed.set<30>(value);
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
        return m_packed.get<29>();
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
        m_packed.set<29>(value);
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
        return m_packed.get<28>();
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
        m_packed.set<28>(value);
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
        return m_packed.get<27>();
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
        m_packed.set<27>(value);
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
        return m_packed.get<26>();
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
        m_packed.set<26>(value);
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
        return m_packed.get<25>();
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
        m_packed.set<25>(value);
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
        return m_packed.get<24>();
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
        m_packed.set<24>(value);
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
        return m_packed.get<19>();
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
        m_packed.set<19>(value);
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
        return m_packed.get<18>();
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
        m_packed.set<18>(value);
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
        return m_packed.get<17>();
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
        m_packed.set<17>(value);
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
        return m_packed.get<16>();
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
        m_packed.set<16>(value);
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
        return m_packed.get<15>();
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
        m_packed.set<15>(value);
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
        return m_packed.get<14>();
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
        m_packed.set<14>(value);
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
        return m_packed.get<13>();
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
        m_packed.set<13>(value);
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
        return m_packed.get<12>();
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
        m_packed.set<12>(value);
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
        return m_packed.get<7>();
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
        m_packed.set<7>(value);
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
        return m_packed.get<6,7,uint8_t>();
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
        m_packed.set<6,7>(value);
    }

    /**
     * @brief Returns the number of Trailer bytes
     * @return Number of Trailer bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return m_packed.size();
    }

    /**
     * @brief Pack Trailer as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add Trailer bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into Trailer
     * @param buffer_ptr Pointer to beginning of Trailer bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        m_packed.unpack_from(buffer_ptr);
    }

private:
    vrtgen::packed<uint32_t> m_packed;

}; // end class Trailer

} // end namespace vrtgen::packing

#endif // _VRTGEN_PACKING_TRAILER_HPP
