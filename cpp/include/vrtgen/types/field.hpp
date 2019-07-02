#pragma once

namespace vrtgen {
    namespace detail {
        template <unsigned bytes>
        struct pad_traits;

        template<>
        struct pad_traits<2>
        {
            typedef unsigned short pad_type;
        };

        template <unsigned bytes>
        struct field_base
        {
            field_base() :
                m_reserved(0)
            {
            }

            typename pad_traits<bytes>::pad_type m_reserved;
        };

        template<>
        struct field_base<0>
        {
        };
    }

    template <typename T>
    struct field : private detail::field_base<sizeof(T) % 4>
    {
    public:
        typedef T packed_type;  
        typedef typename T::value_type value_type;

        explicit field(value_type value) :
            m_value(value)
        {
        }

        value_type get() const
        {
            return m_value.get();
        }

        void set(value_type value)
        {
            m_value.set(value);
        }

    private:
        packed_type m_value;
    };
}