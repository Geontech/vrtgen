#pragma once

#include <cstddef>
#include <new>

#include <vrtgen/types/swap.hpp>

namespace vrtgen {
    namespace detail {
        template <class T>
        struct overlay_buffer
        {
        public:
            template <typename Tin>
            overlay_buffer(Tin* buffer, size_t bytes) :
                m_start(reinterpret_cast<T*>(buffer)),
                m_end(m_start + bytes),
                m_pos(m_start)
            {
            }

            template <typename Tout>
            Tout* next()
            {
                Tout* ptr = reinterpret_cast<Tout*>(m_pos);
                m_pos += sizeof(Tout);
                return ptr;
            }

            size_t getpos() const
            {
                return m_pos - m_start;
            }

        private:
            T* const m_start;
            T* const m_end;
            T* m_pos;
        };
    }
    class OutputBuffer
    {
    public:
        OutputBuffer(void* buffer, size_t bytes) :
            m_buf(buffer, bytes)
        {
        }

        template <typename T, typename ...Args>
        T* insert(Args... args)
        {
            T* ptr = m_buf.next<T>();
            return new (ptr) T(args...);
        }

        template <typename T>
        void put(const typename T::value_type& value)
        {
            m_buf.next<T>()->set(value);
        }

        size_t size() const
        {
            return m_buf.getpos();
        }

    private:
        detail::overlay_buffer<char> m_buf;
    };

    class InputBuffer
    {
    public:
        InputBuffer(const void* buffer, size_t bytes) :
            m_buf(buffer, bytes)
        {
        }

        template <typename T>
        const T* next()
        {
            return m_buf.next<const T>();
        }

        template <typename T>
        typename T::value_type get()
        {
            return next<T>()->get();
        }

    private:
        detail::overlay_buffer<const char> m_buf;
    };
}