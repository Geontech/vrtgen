/*
 * Copyright (C) 2021 Geon Technologies, LLC
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

#ifndef _VRTGEN_BUFFER_HPP
#define _VRTGEN_BUFFER_HPP

#include <concepts>
#include <vector>
#include <cstring>
#include <cstdint>
#include <vrtgen/packing.hpp>

namespace vrtgen {

/**
 * @class Buffer
 * @brief Class to handle packing packet fields into a byte vector
 * 
 * This class is used by the packet packing routine to convert packet
 * classes into a byte vector to be sent over the wire.
 */
class Buffer
{
public:
    using value_type = uint8_t;
    using size_type = std::size_t;

    /**
     * @brief Constructor
     * @param buffer Existing data to be modified
     * @param bytes Length of @param buffer in bytes
     * 
     * Construct a mutable Buffer from @param buffer. This class is used
     * for packing VRT packet classes.
     */
    Buffer(value_type* buffer, size_type bytes) :
        m_begin(buffer), m_end(m_begin + bytes)
    {
    }

    /**
     * @brief Insert data into the buffer
     * @param data Data to be added to the buffer
     * @param len Length of @param data
     */
    void insert(const value_type* data, size_type len)
    {
        auto* ptr = m_begin + m_pos;
        std::copy(data, data + len, ptr);
        m_pos += len;
    }

    template <std::integral T>
    void insert(T value)
    {
        auto* ptr = m_begin + m_pos;
        std::memcpy(ptr, &value, sizeof(T));
        m_pos += sizeof(T);
    }

    template <typename T> requires (!std::integral<T>)
    T* insert(const T& value)
    {
        auto* ptr = m_begin + m_pos;
        if constexpr (std::is_same_v<T, vrtgen::packing::ContextAssociationLists>) {
            const_cast<T&>(value).pack_into(ptr);
        } else {
            value.pack_into(ptr);
        }
        m_pos += value.size();
        return reinterpret_cast<T*>(ptr);
    }

    void update_packet_size()
    {
        auto packet_size = vrtgen::swap::to_be(static_cast<uint16_t>((m_pos + 3) / 4));
        std::memcpy(m_begin + 2, &packet_size, sizeof(packet_size));
    }

    constexpr size_type size() const noexcept
    {
        return m_end - m_begin;
    }

    constexpr size_type pos() const noexcept
    {
        return m_pos;
    }

private:
    value_type* m_begin{ nullptr };
    value_type* m_end{ nullptr };
    size_type m_pos{ 0 };
};

/**
 * @class OverlayBuffer
 * @brief Class to handle translating received bytes into a packet
 * 
 * This class is used by the packet matching/unpacking routines to interpret
 * received bytes into the desired packet class.
 */
class OverlayBuffer
{
public:
    using value_type = uint8_t;
    using size_type = std::size_t;

    /**
     * @brief Constructor
     * @param buffer Existing data to be read
     * @param bytes Length of @param buffer in bytes
     * 
     * Construct a read-only OverlayBuffer from @param buffer. This class is used
     * for matching and unpacking received bytes.
     */
    OverlayBuffer(const void* buffer, size_type bytes) :
        m_begin(reinterpret_cast<const value_type*>(buffer)), m_end(m_begin + bytes)
    {
    }

    const value_type* next(size_type num_bytes) noexcept
    {
        const value_type* ptr = m_begin + m_pos;
        m_pos += num_bytes;
        return ptr;
    }

    template <std::integral T>
    const T next()
    {
        T value{ 0 };
        std::memcpy(&value, m_begin + m_pos, sizeof(T));
        m_pos += sizeof(T);
        return value;
    }

    template <typename T> requires (!std::integral<T>)
    const T next()
    {
        T value;
        value.unpack_from(m_begin + m_pos);
        m_pos += value.size();
        return value;
    }

    constexpr size_type size() const noexcept
    {
        return m_end - m_begin;
    }

    constexpr size_type pos() const noexcept
    {
        return m_pos;
    }

private:
    const value_type* m_begin{ nullptr };
    const value_type* m_end{ nullptr };
    size_type m_pos{ 0 };
};

} // end namespace vrtgen

#endif // _VRTGEN_BUFFER_HPP
