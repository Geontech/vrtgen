#pragma once

#include <utility>

namespace vrtgen {
    template <typename T>
    struct optional {
    public:
        typedef T value_type;

        optional() :
            m_value(nullptr)
        {
        }

        ~optional()
        {
            clear();
        }

        explicit optional(const value_type& value) :
            m_value(new value_type(value))
        {            
        }
 
        explicit operator bool () const
        {
            return (m_value != nullptr);
        }

        const value_type& get() const
        {
            return *m_value;
        }

        void set(const value_type& value)
        {
            optional other(value);
            swap(other);
        }

        void clear()
        {
            delete m_value;
            m_value = nullptr;
        }

        void swap(optional& other)
        {
            std::swap(m_value, other.m_value);
        }
    private:
        value_type* m_value;
    };
}