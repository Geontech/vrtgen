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
#ifndef _VRTGEN_PACKING_PROLOGUE_HPP
#define _VRTGEN_PACKING_PROLOGUE_HPP

namespace vrtgen::packing {

/**
 * @class Class Identifier
 * @brief Class Identifier field (5.1.3)
 */
class ClassIdentifier
{
public:
    /**
     * @brief Returns the number of pad bits
     * @return Number of pad bits
     * 
     * Pad Bit Count is 5 bits long at bit position 31 in word 1
     * See VITA 49.2 Figure 5.1.3-1 and Rule 5.1.3-4
     */
    uint8_t pad_bits() const noexcept
    {
        return m_packed.get<63,5,uint8_t>();
    }

    /**
     * @brief Set the number of pad bits
     * @param value Number of pad bits to set
     * 
     * Pad Bit Count is 5 bits long at bit position 31 in word 1
     * See VITA 49.2 Figure 5.1.3-1 and Rule 5.1.3-4
     */
    void pad_bits(uint8_t value) noexcept
    {
        m_packed.set<63,5>(value);
    }

    /**
     * @brief Returns the Organizationally Unique Identifier (OUI)
     * @return Organizationally Unique Identifier (OUI)
     * 
     * Organizationally Unique Identifier (OUI) is 24 bits long at bit position 23 in word 1
     * See VITA 49.2 Figure 5.1.3-1 and Table 5.1.3-2
     */
    uint32_t oui() const noexcept
    {
        return m_packed.get<55,24,uint32_t>();
    }

    /**
     * @brief Sets the Organizationally Unique Identifier (OUI)
     * @param value Organizationally Unique Identifier (OUI) value to set
     * 
     * Organizationally Unique Identifier (OUI) is 24 bits long at bit position 23 in word 1
     * See VITA 49.2 Figure 5.1.3-1 and Table 5.1.3-2
     */
    void oui(uint32_t value) noexcept
    {
        m_packed.set<55,24>(value);
    }

    /**
     * @brief Returns the Information Class Code
     * @return Information Class Code
     * 
     * Information Class Code is 16 bits long at bit position 31 in word 2
     * See VITA 49.2 Figure 5.1.3-1
     */
    uint16_t information_code() const noexcept
    {
        return m_packed.get<31,16,uint16_t>();
    }

    /**
     * @brief Sets the Information Class Code
     * @param value Information Class Code to set
     * 
     * Information Class Code is 16 bits long at bit position 31 in word 2
     * See VITA 49.2 Figure 5.1.3-1
     */
    void information_code(uint16_t value) noexcept
    {
        m_packed.set<31,16>(value);
    }

    /**
     * @brief Returns the Packet Class Code
     * @return Packet Class Code
     * 
     * Packet Class Code is 16 bits long at bit position 15 in word 2
     * See VITA 49.2 Figure 5.1.3-1
     */
    uint16_t packet_code() const noexcept
    {
        return m_packed.get<15,16,uint16_t>();
    }

    /**
     * @brief Sets the Packet Class Code
     * @param value Packet Class Code to set
     * 
     * Packet Class Code is 16 bits long at bit position 15 in word 2
     * See VITA 49.2 Figure 5.1.3-1
     */
    void packet_code(uint16_t value) noexcept
    {
        m_packed.set<15,16>(value);
    }

    /**
     * @brief Returns the number of ClassIdentifier bytes
     * @return Number of ClassIdentifier bytes
     */
    constexpr size_t size() const noexcept
    {
        return m_packed.size();
    }

    /**
     * @brief Pack ClassIdentifier as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add ClassIdentifier bytes
     */
    inline void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into ClassIdentifier
     * @param buffer_ptr Pointer to beginning of ClassIdentifier bytes in the buffer
     */
    inline void unpack_from(const uint8_t* buffer_ptr)
    {
        m_packed.unpack_from(buffer_ptr);
    }

private:
    vrtgen::packed<uint64_t> m_packed;

}; // end class ClassIdentifier

} // end namespace vrtgen::packing

#endif // _VRTGEN_PACKING_PROLOGUE_HPP
