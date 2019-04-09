#ifndef _VRTGEN_TYPES_HPP
#define _VRTGEN_TYPES_HPP

#include <cstddef>
#include <inttypes.h>

namespace vrtgen {
    namespace detail {
        static inline size_t adjust_pointer(const uint8_t*& ptr, size_t pos)
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
    }
}

#endif // _VRTGEN_TYPES_HPP
