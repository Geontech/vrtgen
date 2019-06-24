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

        template <typename T, unsigned bits>
        struct field_traits_base
        {
            static inline T unpack(unsigned value)
            {
                return static_cast<T>(value);
            }

            static inline unsigned pack(T value)
            {
                return static_cast<unsigned>(value) & field_traits_base::mask;
            }

            static constexpr unsigned mask = (1 << bits) - 1;
        };

        template <typename T, unsigned bits>
        struct field_traits : public field_traits_base<T,bits>
        {
        };

        template <unsigned bits>
        struct field_traits<bool,bits> : public field_traits_base<bool,bits>
        {
            static inline unsigned pack(bool value)
            {
                return -1 & field_traits::mask;
            }
        };

        template <unsigned bits>
        struct field_traits<signed,bits> : public field_traits_base<signed,bits>
        {
            static inline signed unpack(unsigned value)
            {
                bool sign_bit = value & (1<<(bits-1));
                if (sign_bit) {
                    value |= ~field_traits::mask;
                }
                return value;
            }
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

        template <typename T, unsigned pos, unsigned bits>
        inline T get(packed_tag<T,pos,bits>) const
        {
            static_assert(pos < packed::BITS, "bit position exceeds size of packed value");
            typedef detail::field_traits<T,bits> traits;
            constexpr unsigned shift = pos - bits + 1;
            constexpr unsigned mask = (1 << bits) - 1;
            return traits::unpack((swap_type::swap(m_value) >> shift) & mask);
        }

        template <typename Tin, typename T, unsigned pos, unsigned bits>
        inline void set(Tin value, packed_tag<T,pos,bits>)
        {
            static_assert(pos < packed::BITS, "bit position exceeds size of packed value");
            typedef detail::field_traits<T,bits> traits;
            value_type field_value = traits::pack(value);
            constexpr unsigned shift = pos - bits + 1;
            const unsigned mask = swap_type::swap(((1 << bits) - 1) << shift);
            m_value = (m_value & ~mask) | swap_type::swap(field_value << shift);
        }

    private:
        static constexpr unsigned BITS = sizeof(value_type) * 8;

        typedef detail::byte_swap<sizeof(value_type)> swap_type;

        value_type m_value;
    };
}

#endif // _VRTGEN_TYPES_PACKED_HPP
