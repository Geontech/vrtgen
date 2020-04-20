/*
 * Copyright (C) 2019 Geon Technologies, LLC
 *
 * This file is part of vrtgen.
 *
 * vrtgen is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * vrtgen is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

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

        optional(optional&& other) :
            m_value(other.m_value)
        {
            other.m_value = nullptr;
        }

        explicit optional(const value_type& value) :
            m_value(new value_type(value))
        {
        }

        ~optional()
        {
            clear();
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
