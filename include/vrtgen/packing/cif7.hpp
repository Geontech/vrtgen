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
#ifndef _VRTGEN_PACKING_CIF7_HPP
#define _VRTGEN_PACKING_CIF7_HPP

#include <vrtgen/types.hpp>
#include "indicator_fields.hpp"

namespace vrtgen::packing {

/**
 * @class CIF7
 * @brief Context/Command Indicator Field 7 (9.1)
 */
class CIF7 : public IndicatorField7 {}; // end class CIF7

/**
 * @class Belief
 * @brief Belief field (9.12)
 */
class Belief 
{
public:
    /**
     * @brief Returns the Belief Percent subfield value
     * @return Belief Percent subfield value
     * 
     * Belief Percent is 8 bits long at bit position 7
     * See VITA 49.2 Figure 9.12-3
     */
    int belief_percent() const noexcept
    {
        return m_packed.get<7,8,uint8_t>();
    }

    /**
     * @brief Sets the Belief Percent subfield value
     * @param value Belief Percent subfield value
     * 
     * Belief Percent is 8 bits long at bit position 7
     * See VITA 49.2 Figure 9.12-3
     */
    void belief_percent(int value) noexcept
    {
        m_packed.set<7,8>(value);
    }

    /**
     * @brief Returns the number of Belief bytes
     * @return Number of Belief bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return m_packed.size();
    }

    /**
     * @brief Pack Belief as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add Belief bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into Belief
     * @param buffer_ptr Pointer to beginning of Belief bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        m_packed.unpack_from(buffer_ptr);
    }

private:
    vrtgen::packed<uint32_t> m_packed;

}; // end class Belief

/**
 * @class Probability
 * @brief Probability (9.12)
 */
class Probability
{
public:
    /**
     * @brief Returns the Probability Function subfield value
     * @return Probability Function subfield value
     * 
     * Probability Function subfield is 8 bits long at bit position 15
     * with a value of 0 meaning Uniform Distribution, 1 meaning Normal
     * Distribution, and 2-255 are user defined
     * See VITA 49.2 Rule 9.12-9 and 9.12-10
     */
    uint8_t probability_function() const noexcept
    {
        return m_packed.get<15,8,uint8_t>();
    }

    /**
     * @brief Sets the Probability Function subfield value
     * @return Probability Function subfield value
     * 
     * Probability Function subfield is 8 bits long at bit position 15
     * with a value of 0 meaning Uniform Distribution, 1 meaning Normal
     * Distribution, and 2-255 are user defined
     * See VITA 49.2 Rule 9.12-9 and 9.12-10
     */
    void probability_function(uint8_t value) noexcept
    {
        m_packed.set<15,8>(value);
    }

    /**
     * @brief Returns the Probability Percent subfield value
     * @return Probability Percent subfield value
     * 
     * Probability Percent subfield is 8 bits long at bit position 7
     * with a value of 1-255 representing a X/255 percentage
     * See VITA 49.2 Figure 9.12-9
     */
    uint8_t probability_percent() const noexcept
    {
        return m_packed.get<7,8,uint8_t>();
    }

    /**
     * @brief Sets the Probability Percent subfield value
     * @param value Probability Percent subfield value
     * 
     * Probability Percent subfield is 8 bits long at bit position 7
     * with a value of 1-255 representing a X/255 percentage
     * See VITA 49.2 Figure 9.12-9
     */
    void probability_percent(uint8_t value) noexcept
    {
        m_packed.set<7,8>(value);
    }

    /**
     * @brief Returns the number of Probability bytes
     * @return Number of Probability bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return m_packed.size();
    }

    /**
     * @brief Pack Probability as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add Probability bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into Gain
     * @param buffer_ptr Pointer to beginning of Gain bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        m_packed.unpack_from(buffer_ptr);
    }

private:
    vrtgen::packed<uint32_t> m_packed;

}; // end class Probability


} // end namespace vrtgen::packing

#endif // _VRTGEN_PACKING_CIF7_HPP
