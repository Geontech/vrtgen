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

#include <optional>

#include <vrtgen/types.hpp>
#include "indicator_fields.hpp"
#include "enums.hpp"

namespace vrtgen::packing {

/**
 * @class StateEventIndicators
 * @brief State and Event Indicator Field (VITA 49.2-2017 Section 9.10.8)
 */
class StateEventIndicators
{
public:
    /**
     * @brief Returns the Calibrated Time Indicator flag
     * @return Calibrated Time Indicator
     * 
     * Calibrated Time Indicator is 1 bit long at bit position 19
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto calibrated_time() const noexcept -> std::optional<bool>
    {
        if (calibrated_time_enable()) {
            return m_packed.get<19>();
        }
        return std::nullopt;
    }

    /**
     * @brief Sets the Calibrated Time Indicator flag
     * @param value Calibrated Time Indicator flag value to set
     * 
     * Calibrated Time Indicator is 1 bit long at bit position 19
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto calibrated_time(bool value) noexcept -> void
    {
        calibrated_time_enable(true);
        m_packed.set<19>(value);
    }

    /**
     * @brief Reset Calibrated Time enable and indicator values to false
     */
    auto reset_calibrated_time() -> void
    {
        calibrated_time(false);
        calibrated_time_enable(false);
    }

    /**
     * @brief Returns the Valid Data Indicator flag
     * @return Valid Data Indicator
     * 
     * Valid Data Indicator is 1 bit long at bit position 18
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto valid_data() const noexcept -> std::optional<bool>
    {
        if (valid_data_enable()) {
            return m_packed.get<18>();
        }
        return std::nullopt;
    }

    /**
     * @brief Sets the Valid Data Indicator flag
     * @param value Valid Data Indicator flag value to set
     * 
     * Valid Data Indicator is 1 bit long at bit position 18
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    void valid_data(bool value) noexcept
    {
        valid_data_enable(true);
        m_packed.set<18>(value);
    }

    /**
     * @brief Reset Valid Data enable and indicator values to false
     */
    auto reset_valid_data() -> void
    {
        valid_data(false);
        valid_data_enable(false);
    }

    /**
     * @brief Returns the Reference Lock Indicator flag
     * @return Reference Lock Indicator
     * 
     * Reference Lock Indicator is 1 bit long at bit position 17
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto reference_lock() const noexcept -> std::optional<bool>
    {
        if (reference_lock_enable()) {
            return m_packed.get<17>();
        }
        return std::nullopt;
    }

    /**
     * @brief Sets the Reference Lock Indicator flag
     * @param value Reference Lock Indicator flag value to set
     * 
     * Reference Lock Indicator is 1 bit long at bit position 17
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto reference_lock(bool value) noexcept -> void
    {
        reference_lock_enable(true);
        m_packed.set<17>(value);
    }

    /**
     * @brief Reset Reference Lock enable and indicator values to false
     */
    auto reset_reference_lock() -> void
    {
        reference_lock(false);
        reference_lock_enable(false);
    }

    /**
     * @brief Returns the AGC/MGC Indicator flag
     * @return AGC/MGC Indicator
     * 
     * AGC/MGC Indicator is 1 bit long at bit position 16
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto agc_mgc() const noexcept -> std::optional<bool>
    {
        if (agc_mgc_enable()) {
            return m_packed.get<16>();
        }
        return std::nullopt;
    }

    /**
     * @brief Sets the AGC/MGC Indicator flag
     * @param value AGC/MGC Indicator flag value to set
     * 
     * AGC/MGC Indicator is 1 bit long at bit position 16
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto agc_mgc(bool value) noexcept -> void
    {
        agc_mgc_enable(true);
        m_packed.set<16>(value);
    }

    /**
     * @brief Reset AGC/MGC enable and indicator values to false
     */
    auto reset_agc_mgc() -> void
    {
        agc_mgc(false);
        agc_mgc_enable(false);
    }

    /**
     * @brief Returns the Detected Signal Indicator flag
     * @return Detected Signal Indicator
     * 
     * Detected Signal Indicator is 1 bit long at bit position 15
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto detected_signal() const noexcept -> std::optional<bool>
    {
        if (detected_signal_enable()) {
            return m_packed.get<15>();
        }
        return std::nullopt;
    }

    /**
     * @brief Sets the Detected Signal Indicator flag
     * @param value Detected Signal Indicator flag value to set
     * 
     * Detected Signal Indicator is 1 bit long at bit position 15
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto detected_signal(bool value) noexcept -> void
    {
        detected_signal_enable(true);
        m_packed.set<15>(value);
    }

    /**
     * @brief Reset Detected Signal enable and indicator values to false
     */
    auto reset_detected_signal() -> void
    {
        detected_signal(false);
        detected_signal_enable(false);
    }

    /**
     * @brief Returns the Spectral Inversion Indicator flag
     * @return Spectral Inversion Indicator
     * 
     * Spectral Inversion Indicator is 1 bit long at bit position 14
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto spectral_inversion() const noexcept -> std::optional<bool>
    {
        if (spectral_inversion_enable()) {
            return m_packed.get<14>();
        }
        return std::nullopt;
    }

    /**
     * @brief Sets the Spectral Inversion Indicator flag
     * @param value Spectral Inversion Indicator flag value to set
     * 
     * Spectral Inversion Indicator is 1 bit long at bit position 14
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto spectral_inversion(bool value) noexcept -> void
    {
        spectral_inversion_enable(true);
        m_packed.set<14>(value);
    }

    /**
     * @brief Reset Spectral Inversion enable and indicator values to false
     */
    auto reset_spectral_inversion() -> void
    {
        spectral_inversion(false);
        spectral_inversion_enable(false);
    }

    /**
     * @brief Returns the Over-Range Indicator flag
     * @return Over-Range Indicator
     * 
     * Over-Range Indicator is 1 bit long at bit position 13
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto over_range() const noexcept -> std::optional<bool>
    {
        if (over_range_enable()) {
            return m_packed.get<13>();
        }
        return std::nullopt;
    }

    /**
     * @brief Sets the Over-Range Indicator flag
     * @param value Over-Range Indicator flag value to set
     * 
     * Over-Range Indicator is 1 bit long at bit position 13
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto over_range(bool value) noexcept -> void
    {
        over_range_enable(true);
        m_packed.set<13>(value);
    }

    /**
     * @brief Reset Over-Range enable and indicator values to false
     */
    auto reset_over_range() noexcept -> void
    {
        over_range(false);
        over_range_enable(false);
    }

    /**
     * @brief Returns the Sample Loss Indicator flag
     * @return Sample Loss Indicator
     * 
     * Sample Loss Indicator is 1 bit long at bit position 12
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto sample_loss() const noexcept -> std::optional<bool>
    {
        if (sample_loss_enable()) {
            return m_packed.get<12>();
        }
        return std::nullopt;
    }

    /**
     * @brief Sets the Sample Loss Indicator flag
     * @param value Sample Loss Indicator flag value to set
     * 
     * Sample Loss Indicator is 1 bit long at bit position 12
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto sample_loss(bool value) noexcept -> void
    {
        sample_loss_enable(true);
        m_packed.set<12>(value);
    }

    /**
     * @brief Reset Sample Loss enable and indicator values to false
     */
    auto reset_sample_loss() noexcept -> void
    {
        sample_loss(false);
        sample_loss_enable(false);
    }

    /**
     * @brief Returns the number of StateEventIndicators bytes
     * @return Number of StateEventIndicators bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return m_packed.size();
    }

    /**
     * @brief Pack StateEventIndicators as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add StateEventIndicators bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into StateEventIndicators
     * @param buffer_ptr Pointer to beginning of StateEventIndicators bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        m_packed.unpack_from(buffer_ptr);
    }

protected:
    vrtgen::packed<uint32_t> m_packed;

private:
    /**
     * @brief Returns the Calibrated Time Enable flag
     * @return true if Calibrated Time Indicator is enabled, otherwise false
     * 
     * Calibrated Time Enable is 1 bit long at bit position 31
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto calibrated_time_enable() const noexcept -> bool
    {
        return m_packed.get<31>();
    }

    /**
     * @brief Sets the Calibrated Time Enable flag
     * @param value Calibrated Time Enable flag value to set
     * 
     * Calibrated Time Enable is 1 bit long at bit position 31
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto calibrated_time_enable(bool value) noexcept -> void
    {
        m_packed.set<31>(value);
    }

    /**
     * @brief Returns the Valid Data Enable flag
     * @return true if Valid Data Indicator is enabled, otherwise false
     * 
     * Valid Data Enable is 1 bit long at bit position 30
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto valid_data_enable() const noexcept -> bool
    {
        return m_packed.get<30>();
    }

    /**
     * @brief Sets the Valid Data Enable flag
     * @param value Valid Data Enable flag value to set
     * 
     * Valid Data Enable is 1 bit long at bit position 30
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto valid_data_enable(bool value) noexcept -> void
    {
        m_packed.set<30>(value);
    }

    /**
     * @brief Returns the Reference Lock Enable flag
     * @return true if Reference Lock Indicator is enabled, otherwise false
     * 
     * Reference Lock Enable is 1 bit long at bit position 29
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto reference_lock_enable() const noexcept -> bool
    {
        return m_packed.get<29>();
    }

    /**
     * @brief Sets the Reference Lock Enable flag
     * @param value Reference Lock Enable flag value to set
     * 
     * Reference Lock Enable is 1 bit long at bit position 29
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto reference_lock_enable(bool value) noexcept -> void
    {
        m_packed.set<29>(value);
    }

    /**
     * @brief Returns the AGC/MGC Enable flag
     * @return true if AGC/MGC Indicator is enabled, otherwise false
     * 
     * AGC/MGC Enable is 1 bit long at bit position 28
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto agc_mgc_enable() const noexcept -> bool
    {
        return m_packed.get<28>();
    }

    /**
     * @brief Sets the AGC/MGC Enable flag
     * @param value AGC/MGC Enable flag value to set
     * 
     * AGC/MGC Enable is 1 bit long at bit position 28
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto agc_mgc_enable(bool value) noexcept -> void
    {
        m_packed.set<28>(value);
    }

    /**
     * @brief Returns the Detected Signal Enable flag
     * @return true if Detected Signal Indicator is enabled, otherwise false
     * 
     * Detected Signal Enable is 1 bit long at bit position 27
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto detected_signal_enable() const noexcept -> bool
    {
        return m_packed.get<27>();
    }

    /**
     * @brief Sets the Detected Signal Enable flag
     * @param value Detected Signal Enable flag value to set
     * 
     * Detected Signal Enable is 1 bit long at bit position 27
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto detected_signal_enable(bool value) noexcept -> void
    {
        m_packed.set<27>(value);
    }

    /**
     * @brief Returns the Spectral Inversion Enable flag
     * @return true if Spectral Inversion Indicator is enabled, otherwise false
     * 
     * Spectral Inversion Enable is 1 bit long at bit position 26
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto spectral_inversion_enable() const noexcept -> bool
    {
        return m_packed.get<26>();
    }

    /**
     * @brief Sets the Spectral Inversion Enable flag
     * @param value Spectral Inversion Enable flag value to set
     * 
     * Spectral Inversion Enable is 1 bit long at bit position 26
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto spectral_inversion_enable(bool value) noexcept -> void
    {
        m_packed.set<26>(value);
    }

    /**
     * @brief Returns the Over-Range Enable flag
     * @return true if Over-Range Indicator is enabled, otherwise false
     * 
     * Over-Range Enable is 1 bit long at bit position 25
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto over_range_enable() const noexcept -> bool
    {
        return m_packed.get<25>();
    }

    /**
     * @brief Sets the Over-Range Enable flag
     * @param value Over-Range Enable flag value to set
     * 
     * Over-Range Enable is 1 bit long at bit position 25
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto over_range_enable(bool value) noexcept -> void
    {
        m_packed.set<25>(value);
    }

    /**
     * @brief Returns the Sample Loss Enable flag
     * @return true if Sample Loss Indicator is enabled, otherwise false
     * 
     * Sample Loss Enable is 1 bit long at bit position 24
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto sample_loss_enable() const noexcept -> bool
    {
        return m_packed.get<24>();
    }

    /**
     * @brief Sets the Sample Loss Enable flag
     * @param value Sample Loss Enable flag value to set
     * 
     * Sample Loss Enable is 1 bit long at bit position 24
     * See VITA 49.2-2017 Table 9.10.8-1
     */
    auto sample_loss_enable(bool value) noexcept -> void
    {
        m_packed.set<24>(value);
    }

}; // end class StateEventIndicators

} // end namespace vrtgen::packing
