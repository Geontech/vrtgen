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

#ifndef _VRTGEN_TYPES_FIXED_HPP
#define _VRTGEN_TYPES_FIXED_HPP

#include "swap.hpp"

#include <cmath>

namespace vrtgen {
    namespace detail {
        template <typename IntT, unsigned radix, typename FloatT=double, unsigned prec=sizeof(double)>
        struct fixed_converter
        {
            typedef IntT int_type;
            typedef FloatT float_type;

            static inline int_type to_int(float_type value)
            {
                return static_cast<int_type>(std::round(value * SCALE));
            }

            static inline float_type from_int(int_type value)
            {
                float_type tmp = value / SCALE;
                int_type int_part = static_cast<int_type>(tmp);
                float_type frac_part = std::round(tmp*PREC) / PREC - int_part;
                return static_cast<float_type>(int_part) + frac_part;
            }
        private:
            static constexpr float_type SCALE = (1 << radix);
            static constexpr double PREC = std::pow(10,prec);
        };
    }

    template <typename IntT, unsigned radix, typename FloatT=double, unsigned prec=sizeof(double)>
    struct fixed
    {
    public:
        typedef IntT int_type;
        typedef FloatT float_type;
        typedef float_type value_type;
        typedef detail::fixed_converter<int_type,radix,float_type,prec> converter_type;

        fixed() :
            m_value(0)
        {
        }

        explicit fixed(value_type value) :
            m_value(converter_type::to_int(value))
        {
        }

        inline float_type get() const
        {
            return converter_type::from_int(m_value.get());
        }

        inline void set(float_type value)
        {
            m_value.set(converter_type::to_int(value));
        }

    private:
        big_endian<int_type> m_value;
    };
}

#endif // _VRTGEN_TYPES_FIXED_HPP
