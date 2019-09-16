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

#pragma once

namespace vrtgen {
    namespace detail {
        template <unsigned bytes>
        struct pad_traits;

        template<>
        struct pad_traits<2>
        {
            typedef unsigned short pad_type;
        };

        template <unsigned bytes>
        struct field_base
        {
            field_base() :
                m_reserved(0)
            {
            }

            typename pad_traits<bytes>::pad_type m_reserved;
        };

        template<>
        struct field_base<0>
        {
        };
    }

    template <typename T>
    struct field : private detail::field_base<sizeof(T) % 4>
    {
    public:
        typedef T packed_type;  
        typedef typename T::value_type value_type;

        explicit field(value_type value) :
            m_value(value)
        {
        }

        value_type get() const
        {
            return m_value.get();
        }

        void set(value_type value)
        {
            m_value.set(value);
        }

    private:
        packed_type m_value;
    };
}