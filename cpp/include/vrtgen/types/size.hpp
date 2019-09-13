#pragma once

#include "swap.hpp"

namespace vrtgen {
    namespace detail {
        struct size_converter
        {
            typedef size_t size_type;

            static inline size_type store(size_type value)
            {
                return value - 1;
            }

            static inline size_type load(size_type value)
            {
                return value + 1;
            }
        };
    }

    template <typename T>
    class nonzero_size
    {
    public:
        typedef T value_type;
        typedef detail::size_converter converter_type;

        nonzero_size() :
            m_value()
        {
        }

        explicit nonzero_size(value_type value) :
            m_value(converter_type::store(value))
        {
        }

        inline value_type get() const
        {
            return converter_type::load(m_value.get());
        }

        inline void set(value_type value)
        {
            m_value.set(converter_type::store(value));
        }

    private:
        big_endian<T> m_value;
    };
}
