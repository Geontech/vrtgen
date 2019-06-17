#ifndef _VRTGEN_PACKED_HPP
#define _VRTGEN_PACKED_HPP

#include <inttypes.h>
#include "utils/macros.hpp"

namespace vrtgen {
    namespace detail {
        template <typename T>
        inline size_t adjust_pointer(T*& ptr, size_t pos)
        {
            ptr += 3 - (pos/8);
            return (7 - (pos & 0x7));
        }

        inline uint32_t bitmask(uint8_t nbits)
        {
            return (1 << nbits) - 1;
        }

        inline void set_int(uint8_t* data, size_t offset, size_t bits, uint32_t value)
        {
            // Shift the value up to the MSB
            value = value << (32 - bits);

            // Unaligned start bit
            if (offset) {
                size_t nbits = std::min(8 - offset, bits);
                const size_t shift = 8 - (offset + nbits);
                const uint8_t mask = detail::bitmask(nbits) << shift;
                uint8_t src = value >> (32-nbits);
                value <<= nbits;
                *data = ((*data) & ~mask) | (src << shift);
                ++data;
                bits -= nbits;
            }

            // Byte-aligned case, store 8 bits at a time
            for (size_t ii = 0; ii < (bits/8); ++ii, ++data) {
                // Store the current most significant byte and move the next byte
                // up
                *data = (value >> 24);
                value <<= 8;
            }

            // Less than a full byte (but aligned to the byte's MSB)
            size_t remain = bits & 0x7;
            if (remain) {
                const size_t shift = 8 - remain;
                const uint8_t mask = detail::bitmask(shift);
                uint8_t src = value >> (32-remain);
                *data = ((*data) & mask) | (src << shift);
            }
        }

        inline uint32_t get_int(uint32_t word, size_t pos, size_t bits)
        {
            const uint8_t* data = reinterpret_cast<const uint8_t*>(&word);
            size_t bit_offset = detail::adjust_pointer(data, pos);
            const size_t nbytes = (bit_offset + bits + 7)/8;
            uint32_t value = 0;
            for (size_t ii = 0; ii < nbytes; ++ii) {
                value = (value << 8) | data[ii];
            }
            const size_t shift = (nbytes * 8) - (bits + bit_offset);
            return (value >> shift) & detail::bitmask(bits);
        }
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
            return static_cast<T>(GET_BIT32(m_value, pos));
        }

        template <typename T, unsigned pos>
        inline void set(T value, packed_tag<T,pos,1>)
        {
            SET_BIT32(m_value, pos, value);
        }

        template <typename T, unsigned pos, unsigned bits>
        inline T get(packed_tag<T,pos,bits>) const
        {
            return static_cast<T>(vrtgen::detail::get_int(m_value, pos, bits));
        }

        template <typename T, unsigned pos, unsigned bits>
        inline void set(T value, packed_tag<T,pos,bits>)
        {
            uint8_t* data = reinterpret_cast<uint8_t*>(&m_value);
            size_t bit_offset = detail::adjust_pointer(data, pos);
            detail::set_int(data, bit_offset, bits, value);
        }

    private:
        value_type m_value;
    };
}

#endif // _VRTGEN_PACKED_HPP
