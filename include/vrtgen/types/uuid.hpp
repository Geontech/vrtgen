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

#ifndef _VRTGEN_TYPES_UUID_HPP
#define _VRTGEN_TYPES_UUID_HPP

#include <bitset>
#include <bit>
#include <functional>
#include <algorithm>
#include <cstring>

namespace vrtgen {

class UUID
{
    static constexpr std::size_t BYTES = 16;
    static constexpr std::size_t BITS = BYTES * 8;
public:
    using value_type = std::bitset<BITS>;

    /**
     * @brief Default constructor
     */
    UUID() = default;

    /**
     * @brief String constructor
     */
    explicit UUID(const std::string& value) :
        m_value(to_binstring(value))
    {
    }

    /**
     * @brief Returns the string representation of the UUID
     * @return UUID string
     * 
     * Return format is 8-4-4-4-12 hex characters e.g. 12345678-fedc-1234-abcd-123456789abc
     */
    std::string get() const
    {
        return to_uuidstring();
    }

    /**
     * @brief Sets the UUID from it's string representation
     * @param value UUID string to set
     * 
     * Format of value is 8-4-4-4-12 hex characters e.g. 12345678-fedc-1234-abcd-123456789abc
     */
    void set(const std::string& value)
    {
        m_value = value_type{ to_binstring(value) };
    }

    /**
     * @brief Returns the number of UUID bytes
     * @return Number of UUID bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return BYTES;
    }

    /**
     * @brief Pack UUID as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add UUID bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        auto ptr = reinterpret_cast<const uint8_t*>(&m_value);
        if constexpr (std::endian::native == std::endian::little) {
            std::reverse_copy(ptr, ptr + size(), buffer_ptr);
        } else {
            std::memcpy(buffer_ptr, ptr, size());
        }
    }

    /**
     * @brief Unpack buffer bytes into UUID
     * @param buffer_ptr Pointer to beginning of UUID bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        auto ptr = reinterpret_cast<uint8_t*>(&m_value);
        if constexpr (std::endian::native == std::endian::little) {
            std::reverse_copy(buffer_ptr, buffer_ptr + size(), ptr);
        } else {
            std::memcpy(ptr, buffer_ptr, size());
        }
    }

private:
    std::string to_binstring(const std::string& value) const
    {
        std::string retval;
        for (const auto& c : value) {
            switch (std::toupper(c)) {
                case '0': retval.append("0000"); break;
                case '1': retval.append("0001"); break;
                case '2': retval.append("0010"); break;
                case '3': retval.append("0011"); break;
                case '4': retval.append("0100"); break;
                case '5': retval.append("0101"); break;
                case '6': retval.append("0110"); break;
                case '7': retval.append("0111"); break;
                case '8': retval.append("1000"); break;
                case '9': retval.append("1001"); break;
                case 'A': retval.append("1010"); break;
                case 'B': retval.append("1011"); break;
                case 'C': retval.append("1100"); break;
                case 'D': retval.append("1101"); break;
                case 'E': retval.append("1110"); break;
                case 'F': retval.append("1111"); break;
                default: break;
            }
        }
        return retval;
    }

    std::string to_uuidstring() const
    {
        std::string retval;
        std::string value_str = m_value.to_string();
        for (std::size_t i=0; i<m_value.size(); i+=4) {
            switch(i) {
                case 32:
                case 48:
                case 64:
                case 80: retval.append("-"); break;
                default: break;
            }

            auto hex = std::bitset<4>(value_str.substr(i, i+4)).to_ulong();
            switch (hex) {
                case 10: retval.append("a"); break;
                case 11: retval.append("b"); break;
                case 12: retval.append("c"); break;
                case 13: retval.append("d"); break;
                case 14: retval.append("e"); break;
                case 15: retval.append("f"); break;
                default: retval.append(std::to_string(hex)); break;
            }
        }
        return retval;
    }

    value_type m_value;

}; // end class UUID

} // end namespace vrtgen

#endif // _VRTGEN_TYPES_UUID_HPP
