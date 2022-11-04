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
#include <vrtgen/packing/state_event_indicators.hpp>
#include <vector>
#include <type_traits>

namespace vrtgen::packing {

/**
 * @class Trailer
 * @brief Signal Data Packet trailer (5.1.6)
 */
class Trailer : public StateEventIndicators
{
public:

    /**
     * @brief Returns the Sample Frame Enable flag
     * @return true if Sample Frame Indicator is enabled, otherwise false
     * 
     * Sample Frame Enable is 2 bits long at bit position 23
     * See VITA 49.2 Table 5.1.6-1
     */
    bool sample_frame_enable() const noexcept
    {
        uint8_t sample_enable = m_packed.get<23, 2, uint8_t>();
        if (sample_enable == 0b11) {
            return true;
        }
        return false;
    }

    /**
     * @brief Sets the Sample Frame Enable flag
     * @param value Sample Frame Enable flag value to set
     * 
     * Sample Frame Enable is 2 bits long at bit position 23
     * See VITA 49.2 Table 5.1.6-1
     */
    void sample_frame_enable(bool value) noexcept
    {
        if (value) {
            m_packed.set<23, 2>(0b11);
        }
        else {
            m_packed.set<23, 2>(0b00);
        }
    }

    /**
     * @brief Returns the User Defined Indicator flag
     * @return User Defined Indicator
     * 
     * User Defined Indicator is 2 bits long at bit position 11
     * See VITA 49.2 Table 5.1.6-1
     */
    vrtgen::packing::SSI sample_frame() const noexcept {
        return m_packed.get<11, 2, vrtgen::packing::SSI>();
    }

    /**
     * @brief Sets the User Defined Indicator flag
     * @param value User Defined Indicator flag value to set
     * 
     * User Defined Indicator is 2 bits long at bit position 11
     * See VITA 49.2 Table 5.1.6-1
     */
    void sample_frame(const vrtgen::packing::SSI value) noexcept 
    {
        m_packed.set<11, 2, vrtgen::packing::SSI>(value);
    }

    /**
     * @brief Returns the Associated Context Packets Count Enable flag
     * @return true if Associated Context Packets Count is enabled, otherwise false
     * 
     * Associated Context Packets Count Enable is 1 bit long at bit position 7
     * See VITA 49.2 Table 5.1.6-1
     */
    bool associated_context_packet_count_enable() const noexcept
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
    void associated_context_packet_count_enable(bool value) noexcept
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
    uint8_t associated_context_packet_count() const noexcept
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
    void associated_context_packet_count(uint8_t value) noexcept
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

}; // end class Trailer

} // end namespace vrtgen::packing

#endif // _VRTGEN_PACKING_TRAILER_HPP
