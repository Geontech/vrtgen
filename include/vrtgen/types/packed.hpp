/*
 * Copyright (C) 2022 Geon Technologies, LLC
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

#ifndef _VRTGEN_TYPES_PACKED_HPP
#define _VRTGEN_TYPES_PACKED_HPP

#include <inttypes.h>
#include <cstring>
#include <concepts>

#include "fixed.hpp"
#include "swap.hpp"

namespace vrtgen {
namespace detail {

template <std::size_t P, std::size_t B>
constexpr bool valid_position = P < B;

/**
 * @class field_traits
 * @brief Bitwise traits related to a packed subfield
 * @tparam T Value of packed integer
 * @tparam P Bit position
 * @tparam B Number of bits
 */
template <typename T, std::size_t pos, std::size_t bits>
struct field_traits
{
    static constexpr T shift{ pos - bits + 1 };
    static constexpr T mask{ ((static_cast<T>(1) << bits) - 1) << shift };
}; // end struct field_traits

} // end namespace detail


template <std::unsigned_integral T>
struct packed
{
public:
    using value_type = T;

    template <std::size_t Pos, std::size_t Bits=1, typename Tout=bool>
    requires (detail::valid_position<Pos, packed::BITS>)
    inline Tout get() const
    {
        using traits = detail::field_traits<T,Pos,Bits>;
        // Fetch the stored bits in host order, then extract the field  bits
        return static_cast<Tout>((vrtgen::swap::from_be(m_value) & traits::mask) >> traits::shift);
    }

    template <std::size_t Pos, std::size_t Bits=1, typename Tin>
    requires (detail::valid_position<Pos, packed::BITS>)
    inline void set(Tin value)
    {
        using traits = detail::field_traits<T,Pos,Bits>;
        // Fetch the current value in host order, then mask off the field bits
        value_type old_value = vrtgen::swap::from_be(m_value) & ~traits::mask;
        value_type field_value = static_cast<value_type>(value) << traits::shift;
        // Combine the new value with the existing bits, then store back in network order
        m_value = vrtgen::swap::to_be(static_cast<value_type>(old_value | field_value));
    }

    /**
     * @brief Checks if any of the packed bits are set to true
     * @return true if any of the bits are set to true, otherwise false
     */
    bool any() const noexcept
    {
        return !none();
    }

    /**
     * @brief Checks if none of the packed bits are set to true
     * @return true if none of the bits are set to true, otherwise false
     */
    bool none() const noexcept
    {
        return m_value == 0;
    }

    /**
     * @brief Returns the number of packed bytes
     * @return Number of packed bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return sizeof(value_type);
    }

    /**
     * @brief Pack packed value as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add packed value bytes
     */
    inline void pack_into(uint8_t* buffer_ptr) const
    {
        std::memcpy(buffer_ptr, &m_value, sizeof(value_type));
    }

    /**
     * @brief Unpack buffer bytes into packed value
     * @param buffer_ptr Pointer to beginning of packed value bytes in the buffer
     */
    inline void unpack_from(const uint8_t* buffer_ptr)
    {
        std::memcpy(&m_value, buffer_ptr, sizeof(value_type));
    }

private:
    static constexpr std::size_t BITS = sizeof(value_type) * 8;

    value_type m_value{ 0 };

}; // end class packed

/**
 * @class Packed32_Fixed16R
 * @brief Common 32-bit field Structure with 2 16-bit fixed-point subfields
 * @tparam R Radix point of subfields
 */
template <std::size_t R> 
class Packed32_Fixed16R
{
public:
    /**
     * @brief Returns the number of field bytes
     * @return Number of field bytes
     */
    constexpr size_t size() const noexcept
    {
        return m_packed.size();
    }

    /**
     * @brief Pack field as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add field bytes
     */
    inline void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into field
     * @param buffer_ptr Pointer to beginning of field bytes in the buffer
     */
    inline void unpack_from(const uint8_t* buffer_ptr)
    {
        m_packed.unpack_from(buffer_ptr);
    }

protected:
    /**
     * @brief Returns the subfield value at bit position 31
     * @return Subfield value at bit position 31
     */
    double subfield_31() const noexcept
    {
        return vrtgen::fixed::to_fp<16,R>(m_packed.get<31,16,int16_t>());
    }

    /**
     * @brief Sets the subfield value at bit position 31
     * @param value Subfield value to set
     */
    void subfield_31(double value) noexcept
    {
        m_packed.set<31,16>(vrtgen::fixed::to_int<16,R>(value));
    }

    /**
     * @brief Returns the subfield value at bit position 15
     * @return Subfield value at bit position 15
     */
    double subfield_15() const noexcept
    {
        return vrtgen::fixed::to_fp<16,R>(m_packed.get<15,16,int16_t>());
    }

    /**
     * @brief Sets the subfield value at bit position 15
     * @param value Subfield value to set
     */
    void subfield_15(double value) noexcept
    {
        m_packed.set<15,16,uint16_t>(vrtgen::fixed::to_int<16,R>(value));
    }

    vrtgen::packed<uint32_t> m_packed;

}; // end class Packed32_Fixed16R

} // end namespace vrtgen

#endif // _VRTGEN_TYPES_PACKED_HPP
