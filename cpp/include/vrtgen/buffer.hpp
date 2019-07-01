#pragma once

#include <cstddef>
#include <new>

#include <vrtgen/types/swap.hpp>

namespace vrtgen {
    class OutputBuffer
    {
    public:
        OutputBuffer(void* buffer, size_t bytes) :
            m_start(reinterpret_cast<char*>(buffer)),
            m_end(m_start + bytes),
            m_pos(m_start)
        {
        }

        template <typename T, typename ...Args>
        T* insert(Args... args)
        {
            T* ptr = new (m_pos) T(args...);
            m_pos += sizeof(T);
            return ptr;
        }

        template <typename T>
        void put(const typename T::value_type& value)
        {
            insert<T>()->set(value);
        }

        size_t getpos() const
        {
            return m_pos - m_start;
        }
    private:
        char* const m_start;
        char* const m_end;
        char* m_pos;
    };

    class InputBuffer
    {
    public:
        InputBuffer(const void* buffer, size_t bytes) :
            m_start(reinterpret_cast<const char*>(buffer)),
            m_end(m_start + bytes),
            m_pos(m_start)
        {
        }

        template <typename T>
        const T* next()
        {
            const T* ptr = reinterpret_cast<const T*>(m_pos);
            m_pos += sizeof(T);
            return ptr;
        }

        template <typename T>
        typename T::value_type get()
        {
            return next<T>()->get();
        }

    private:
        const char* const m_start;
        const char* const m_end;
        const char* m_pos;
    };
}