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

    typedef uint32_t StreamIdentifier;
    typedef uint32_t GenericIdentifier32;
    typedef uint16_t GenericIdentifier16;
    typedef uint32_t MessageIdentifier;
}

#endif // _VRTGEN_TYPES_HPP
