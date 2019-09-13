#ifndef _VRTGEN_TYPES_SWAP_HPP
#define _VRTGEN_TYPES_SWAP_HPP

#include <inttypes.h>

namespace vrtgen {
    inline uint16_t swap16(uint16_t value)
    {
        return (value << 8) | (value >> 8);
    }

    inline uint32_t swap24(uint32_t value)
    {
        return (((value & 0xFF) << 16) |
                (value & (0xFF << 8)) |
                ((value >> 16) & 0xFF));
    }

    inline uint32_t swap32(uint32_t value)
    {
        return ((value << 24) |
                ((value << 8) & (0xFF << 16)) |
                ((value >> 8) & (0xFF << 8)) |
                (value >> 24));
    }

    inline uint64_t swap64(uint64_t value)
    {
        const uint64_t mask = 0xFF;
        return ((value << 56) |
                ((value << 40) & (mask << 48)) |
                ((value << 24) & (mask << 40)) |
                ((value << 8) & (mask << 32)) |
                ((value >> 8) & (mask << 24)) |
                ((value >> 24) & (mask << 16)) |
                ((value >> 40) & (mask << 8)) |
                (value >> 56));
    }

    namespace detail {
        template <unsigned int bytes>
        struct byte_swap;

        template <>
        struct byte_swap<1>
        {
            typedef uint8_t int_type;
            static inline int_type swap(int_type value)
            {
                return value;
            }
        };

        template <>
        struct byte_swap<2>
        {
            typedef uint16_t int_type;
            static inline int_type swap(int_type value)
            {
                return swap16(value);
            }
        };

        template <>
        struct byte_swap<3>
        {
            typedef uint32_t int_type;
            static inline int_type swap(int_type value)
            {
                return swap24(value);
            }
        };

        template <>
        struct byte_swap<4>
        {
            typedef uint32_t int_type;
            static inline int_type swap(int_type value)
            {
                return swap32(value);
            }
        };

        template <>
        struct byte_swap<8>
        {
            typedef uint64_t int_type;
            static inline int_type swap(int_type value)
            {
                return swap64(value);
            }
        };
    }

    template <typename T>
    struct big_endian
    {
    public:
        typedef T value_type;
        typedef detail::byte_swap<sizeof(T)> swap_type;

        big_endian() :
            m_value(0)
        {
        }

        explicit big_endian(value_type value) :
            m_value(swap_type::swap(value))
        {
        }

        inline value_type get() const
        {
            return swap_type::swap(m_value);
        }

        inline void set(value_type value)
        {
            m_value = swap_type::swap(value);
        }

    private:
        value_type m_value;
    };
}

#endif // _VRTGEN_TYPES_SWAP_HPP
