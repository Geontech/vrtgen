#ifndef _VRTGEN_TYPES_HPP
#define _VRTGEN_TYPES_HPP

#include <cstddef>
#include <algorithm>

#include <inttypes.h>

namespace vrtgen {
    namespace detail {
        template <typename T>
        static inline size_t adjust_pointer(T*& ptr, size_t pos)
        {
            ptr += 3 - (pos/8);
            return (7 - (pos & 0x7));
        }

        static inline uint32_t bitmask(uint8_t nbits)
        {
            return (1 << nbits) - 1;
        }
    }

    typedef uint32_t word_t;

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

    void set_int(uint32_t& word, size_t pos, size_t bits, uint32_t value)
    {
        // Shift the value up to the MSB
        value = value << (32 - bits);

        uint8_t* data = reinterpret_cast<uint8_t*>(&word);
        size_t bit_offset = detail::adjust_pointer(data, pos);
        // Unaligned start bit
        if (bit_offset) {
            size_t nbits = std::min(8 - bit_offset, bits);
            const size_t shift = 8 - (bit_offset + nbits);
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

    /**
     * Placeholder class for fixed point types.
     * TODO: Implement class and/or replace with float conversion 
     */
    template <size_t bits, size_t radix>
    struct fixed
    {
        fixed(uint32_t)
        {
        }

        operator uint32_t () const
        {
            return 0;
        }
    };
}

#endif // _VRTGEN_TYPES_HPP
