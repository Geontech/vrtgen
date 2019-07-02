#ifndef _VRTGEN_TYPES_HPP
#define _VRTGEN_TYPES_HPP

#include "types/swap.hpp"
#include "types/field.hpp"
#include "types/fixed.hpp"
#include "types/packed.hpp"
#include "types/optional.hpp"

#include <inttypes.h>

namespace vrtgen {
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

    typedef uint32_t StreamIdentifier;
    typedef uint32_t GenericIdentifier32;
    typedef uint16_t GenericIdentifier16;
}

#endif // _VRTGEN_TYPES_HPP
