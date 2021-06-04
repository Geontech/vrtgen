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

#pragma once

namespace vrtgen {
    namespace detail {
        template <unsigned bytes>
        struct padded_base;

        template<>
        struct padded_base<2>
        {
            unsigned short m_reserved{0};
        };

        template <typename T, unsigned bytes>
        struct padded_type : private padded_base<bytes>, public T
        {
            using T::T;
        };

        template <typename T, unsigned bytes>
        struct field_type
        {
            typedef padded_type<T,bytes> type;
        };

        template <typename T>
        struct field_type<T,0>
        {
            typedef T type;
        };
    }

    template <typename T>
    struct field
    {
        typedef typename detail::field_type<T, sizeof(T)%4>::type type;
    };
}
