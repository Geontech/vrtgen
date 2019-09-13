#ifndef _VRTGEN_TYPES_FIXED_HPP
#define _VRTGEN_TYPES_FIXED_HPP

#include "swap.hpp"

#include <cmath>

namespace vrtgen {
    namespace detail {
        template <typename IntT, unsigned radix, typename FloatT=double>
        struct fixed_converter
        {
            typedef IntT int_type;
            typedef FloatT float_type;

            static inline int_type to_int(float_type value)
            {
                return static_cast<int_type>(std::round(value * SCALE));
            }

            static inline float_type from_int(int_type value)
            {
                return value / SCALE;
            }
        private:
            static constexpr float_type SCALE = (1 << radix);
        };
    }

    template <typename IntT, unsigned radix, typename FloatT=double>
    struct fixed
    {
    public:
        typedef IntT int_type;
        typedef FloatT float_type;
        typedef float_type value_type;
        typedef detail::fixed_converter<int_type,radix,float_type> converter_type;

        fixed() :
            m_value(0)
        {
        }

        explicit fixed(value_type value) :
            m_value(converter_type::to_int(value))
        {
        }

        inline float_type get() const
        {
            return converter_type::from_int(m_value.get());
        }

        inline void set(float_type value)
        {
            m_value.set(converter_type::to_int(value));
        }

    private:
        big_endian<int_type> m_value;
    };
}

#endif // _VRTGEN_TYPES_FIXED_HPP
