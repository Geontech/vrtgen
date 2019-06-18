#ifndef _VRTGEN_PACKED_HPP
#define _VRTGEN_PACKED_HPP

#include <inttypes.h>
#include "swap.hpp"

namespace vrtgen {
    namespace detail {
        template <unsigned pos>
        struct bit_traits
        {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            // Big-endian, bit position is pass-through.
            static constexpr unsigned offset = pos;
#else
            // Little-endian, swap the byte offset. This expression is expected to be used
            // with constant values for b, where the final value is calculated at compile
            // time.
            static constexpr unsigned offset = ((8*(3-(pos/8)))+(pos&0x7));
#endif
            static constexpr unsigned mask = 1 << bit_traits::offset;
        };
    }
    template <typename T, unsigned pos, unsigned bits>
    struct packed_tag
    {
    };

    template <typename IntT>
    struct packed
    {
    public:
        typedef IntT value_type;

        template <typename T, unsigned pos>
        inline T get(packed_tag<T,pos,1>) const
        {
            typedef detail::bit_traits<pos> traits;
            return static_cast<T>((m_value >> traits::offset) & 1);
        }

        template <typename T, unsigned pos>
        inline void set(T value, packed_tag<T,pos,1>)
        {
            typedef detail::bit_traits<pos> traits;
            m_value = (m_value & ~traits::mask) | (bool(value) << traits::offset);
        }

        template <typename T, unsigned pos, unsigned bits>
        inline T get(packed_tag<T,pos,bits>) const
        {
            constexpr unsigned offset = pos % ((sizeof(value_type) * 8));
            constexpr unsigned shift = offset - bits + 1;
            constexpr unsigned mask = (1 << bits) - 1;
            return static_cast<T>((swap_type::swap(m_value) >> shift) & mask);
        }

        template <typename T, unsigned pos, unsigned bits>
        inline void set(T value, packed_tag<T,pos,bits>)
        {
            constexpr unsigned offset = pos % ((sizeof(value_type) * 8));
            constexpr unsigned shift = offset - bits + 1;
            const unsigned mask = swap_type::swap(((1 << bits) - 1) << shift);
            m_value = (m_value & ~mask) | swap_type::swap(value << shift);
        }

    private:
        typedef detail::byte_swap<sizeof(value_type)> swap_type;

        value_type m_value;
    };
}

#endif // _VRTGEN_PACKED_HPP
