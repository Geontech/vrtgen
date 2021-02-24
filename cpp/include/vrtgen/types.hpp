/*
 * Copyright (C) 2019 Geon Technologies, LLC
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

#ifndef _VRTGEN_TYPES_HPP
#define _VRTGEN_TYPES_HPP

#include "types/swap.hpp"
#include "types/field.hpp"
#include "types/fixed.hpp"
#include "types/packed.hpp"
#include "types/optional.hpp"
#include "types/size.hpp"

#include <inttypes.h>
#include <bitset>
#include <functional>

namespace vrtgen {
    class OUI
    {
    public:
        typedef uint32_t int_type;

        int_type get() const
        {
            return ((m_value[0] << 16) | (m_value[1] << 8) | m_value[2]);
        }

        void set(int_type value)
        {
            m_value[0] = value >> 16;
            m_value[1] = value >> 8;
            m_value[2] = value;
        }
    private:
        uint8_t m_value[3];
    };

    class UUID
    {
        using value_type = std::bitset<128>;
    public:
        UUID() = default;
        explicit UUID(const std::string& value) :
            m_value(to_binstring(value))
        {
        }
        explicit UUID(const value_type& value) :
            m_value(value.to_string())
        {
        }

        std::string get() const
        {
            return to_uuidstring();
        }

        void set(const std::string& value)
        {
            m_value = value_type(to_binstring(value));
        }

        value_type& value()
        {
            return m_value;
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
            for (size_t i=0; i<m_value.size(); i+=4) {
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
    };

    typedef uint32_t StreamIdentifier;
    typedef uint32_t GenericIdentifier32;
    typedef uint16_t GenericIdentifier16;
    typedef uint32_t MessageIdentifier;
}

#endif // _VRTGEN_TYPES_HPP
