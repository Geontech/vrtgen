#ifndef _VRTGEN_TYPES_HPP
#define _VRTGEN_TYPES_HPP

#include <cstddef>
#include <algorithm>

#include <inttypes.h>

#include "swap.hpp"
#include "fixed.hpp"

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

    inline void set_int(uint32_t& word, size_t pos, size_t bits, uint32_t value)
    {
        uint8_t* data = reinterpret_cast<uint8_t*>(&word);
        size_t bit_offset = detail::adjust_pointer(data, pos);
        detail::set_int(data, bit_offset, bits, value);
    }

    inline void set_int(uint16_t& hword, size_t pos, size_t bits, uint32_t value)
    {
        uint8_t* data = reinterpret_cast<uint8_t*>(&hword);
        size_t bit_offset = detail::adjust_pointer(data, pos);
        detail::set_int(data, bit_offset, bits, value);
    }

    inline void set_int(uint8_t& dest, size_t pos, size_t bits, uint32_t value)
    {
        uint8_t* data = &dest;
        size_t bit_offset = detail::adjust_pointer(data, pos);
        detail::set_int(data, bit_offset, bits, value);
    }

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
}

#endif // _VRTGEN_TYPES_HPP
