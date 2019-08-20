#pragma once

#include "swap.hpp"

namespace vrtgen {
    template <typename T>
    class nonzero_size
    {
    public:
        typedef T value_type;

        nonzero_size() :
            m_value()
        {
        }

        explicit nonzero_size(value_type value) :
            m_value(value - 1)
        {
        }

        inline value_type get() const
        {
            return m_value.get() + 1;
        }

        inline void set(value_type value)
        {
            m_value.set(value - 1);
        }

    private:
        big_endian<T> m_value;
    };
}
