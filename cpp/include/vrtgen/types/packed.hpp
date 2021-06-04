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

#ifndef _VRTGEN_TYPES_PACKED_HPP
#define _VRTGEN_TYPES_PACKED_HPP

#include <inttypes.h>
#include "swap.hpp"

namespace vrtgen {
    namespace detail {
        template <unsigned bytes, unsigned pos>
        struct bit_traits
        {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            // Big-endian, bit position is pass-through.
            static constexpr unsigned offset = pos;
#else
            // Little-endian, keep the  bit offset (lower 3 bits) but swap the
            // byte offset (upper bits).
            static constexpr unsigned offset = ((8*(bytes-1-(pos/8)))+(pos&0x7));
#endif
            static constexpr unsigned mask = 1 << bit_traits::offset;
        };

        template <unsigned pos, unsigned bits>
        struct field_traits
        {
            static constexpr unsigned shift = pos - bits + 1;
            static constexpr unsigned mask = ((1 << bits) - 1) << shift;
        };

        template <typename T, unsigned bits>
        struct field_packing_base
        {
            static inline T load(unsigned value)
            {
                return static_cast<T>(value);
            }

            static inline unsigned store(T value)
            {
                return static_cast<unsigned>(value) & field_packing_base::mask;
            }

            static constexpr unsigned mask = (1 << bits) - 1;
        };

        template <typename T, unsigned bits>
        struct field_packing : public field_packing_base<T,bits>
        {
        };

        template <unsigned bits>
        struct field_packing<bool,bits> : public field_packing_base<bool,bits>
        {
            static inline unsigned store(bool value)
            {
                // A little sleight-of-hand: value must be 0 or 1, so taking
                // the negative integer value will set all bits to be the same
                return (-value & field_packing::mask);
            }
        };

        template <unsigned bits>
        struct field_packing<signed,bits> : public field_packing_base<signed,bits>
        {
            static inline signed load(unsigned value)
            {
                // Extract sign bit in MSB and expand its value to all sign
                // bits in the output type.
                bool sign_bit = value & (1<<(bits-1));
                return (-sign_bit & ~field_packing::mask) | value;
            }
        };

        template <typename T, unsigned bits, typename Converter=void>
        struct field_converter;

        template <typename T, unsigned bits>
        struct field_converter<T,bits,void> : public field_packing<T,bits>
        {
        };

        template <typename T, unsigned bits, typename Converter>
        struct field_converter : public field_packing<T,bits>
        {
            static inline T store(unsigned value)
            {
                return field_packing<T,bits>::store(Converter::store(value));
            }

            static inline unsigned load(T value)
            {
                return Converter::load(field_packing<T,bits>::load(value));
            }
        };
    }

    template <typename Value, unsigned Position, unsigned Bits, typename Converter=void>
    struct packed_tag
    {
        typedef Value value_type;
        static constexpr unsigned pos = Position;
        static constexpr unsigned bits = Bits;
        typedef Converter converter_type;
    };

    template <typename IntT>
    struct packed
    {
    public:
        typedef IntT value_type;

        template <typename T, unsigned pos>
        inline T get(packed_tag<T,pos,1>) const
        {
            static_assert(pos < packed::BITS, "bit position exceeds size of packed value");
            typedef detail::bit_traits<sizeof(value_type),pos> traits;
            return static_cast<T>((m_value >> traits::offset) & 1);
        }

        template <typename Tin, typename T, unsigned pos>
        inline void set(Tin value, packed_tag<T,pos,1>)
        {
            static_assert(pos < packed::BITS, "bit position exceeds size of packed value");
            typedef detail::bit_traits<sizeof(value_type),pos> traits;
            m_value = (m_value & ~traits::mask) | (bool(value) << traits::offset);
        }

        template <typename T, unsigned pos, unsigned bits, typename Converter>
        inline T get(packed_tag<T,pos,bits,Converter>) const
        {
            static_assert(pos < packed::BITS, "bit position exceeds size of packed value");
            typedef detail::field_traits<pos,bits> traits;
            typedef detail::field_converter<T,bits,Converter> converter;
            // Fetch the stored bits in host order, then extract the field
            // bits
            value_type value = (swap_type::swap(m_value) & traits::mask) >> traits::shift;
            return converter::load(value);
        }

        template <typename Tin, typename T, unsigned pos, unsigned bits, typename Converter>
        inline void set(Tin value, packed_tag<T,pos,bits,Converter>)
        {
            static_assert(pos < packed::BITS, "bit position exceeds size of packed value");
            typedef detail::field_traits<pos,bits> traits;
            typedef detail::field_converter<T,bits,Converter> converter;
            // Fetch the current value in host order, then mask off the field
            // bits
            value_type old_value = swap_type::swap(m_value) & ~traits::mask;
            value_type field_value = converter::store(value) << traits::shift;
            // Combine the new value with the existing bits, then store back
            // in network order
            m_value = swap_type::swap(old_value | field_value);
        }

    private:
        static constexpr unsigned BITS = sizeof(value_type) * 8;

        typedef detail::byte_swap<sizeof(value_type)> swap_type;

        value_type m_value;
    };
}

#endif // _VRTGEN_TYPES_PACKED_HPP
