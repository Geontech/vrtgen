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

#ifndef _VRTGEN_TYPES_OUI_HPP
#define _VRTGEN_TYPES_OUI_HPP

#include <array>
#include <sstream>
#include <cstring>
#include <cstdint>
#include <cctype>
#include <algorithm>

namespace vrtgen {

/**
 * @class OUI
 * @brief Organizationally Unique Identifier (OUI)
 */
class OUI
{
    static constexpr std::size_t SIZE{ 3 };
public:
    using value_type = std::array<uint8_t, SIZE>;
    using int_type = uint32_t;

    /**
     * @brief Default constructor
     */
    OUI() = default;

    /**
     * @brief Constructor
     * @param value Integer value to initialize from
     */
    explicit OUI(const int_type value)
    {
        set(value);
    }

    /**
     * @brief Returns the underlying value as an integer
     * @return Integer representation of the underlying value
     */
    constexpr int_type get() const noexcept
    {
        return ((m_value[0] << 16) | (m_value[1] << 8) | m_value[2]);
    }

    /**
     * @brief Sets the underlying value
     * @param value Integer value to set
     */
    void set(int_type value)
    {
        m_value[0] = value >> 16;
        m_value[1] = value >> 8;
        m_value[2] = value;
    }

    /**
     * @brief Returns the OUI as a string
     * @return OUI in it's string representation
     * 
     * Return format is AB-CD-EF
     */
    std::string to_string() const noexcept
    {
        std::stringstream ss;
        ss << std::hex
           << static_cast<int>(m_value[0]) << "-"
           << static_cast<int>(m_value[1]) << "-"
           << static_cast<int>(m_value[2]);
        auto retval = ss.str();
        std::transform(
            retval.begin(), retval.end(), retval.begin(),
            [](auto c){ return std::toupper(c); }
        );
        return retval;
    }

    /**
     * @brief Returns the number of OUI bytes
     * @return Number of OUI bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return m_value.size();
    }

    /**
     * @brief Pack OUI as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add OUI bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        std::memcpy(buffer_ptr, &m_value, sizeof(m_value));
    }

    /**
     * @brief Unpack buffer bytes into OUI
     * @param buffer_ptr Pointer to beginning of OUI bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        std::memcpy(&m_value, buffer_ptr, sizeof(m_value));
    }

private:
    value_type m_value { { 0 } };

}; // end class OUI

} // end namespace vrtgen

#endif // _VRTGEN_TYPES_OUI_HPP
