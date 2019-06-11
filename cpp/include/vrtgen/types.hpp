#ifndef _VRTGEN_TYPES_HPP
#define _VRTGEN_TYPES_HPP

#include <cstddef>
#include <algorithm>

#include <inttypes.h>

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

    inline uint16_t swap16(uint16_t value)
    {
        return (value << 8) | (value >> 8);
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
