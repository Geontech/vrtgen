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

/**
 * @file vrtgen/packing/header.hpp
 * @brief Packing structures for VRT Header types.
 */

#ifndef _VRTGEN_PACKING_HEADER_HPP
#define _VRTGEN_PACKING_HEADER_HPP

#include <vrtgen/types/packed.hpp>
#include "enums.hpp"

namespace vrtgen::packing {

/**
 * @class Header
 * @brief Base class for VRT Packet Header (5.1.1) types
 */
class Header
{
public:
    /**
     * @brief Returns the VRT Packet Type
     * @return VRT Packet Type
     * 
     * VRT Packet Type is 4 bits long at bit position 31
     * See VITA 49.2 Figure 5.1.1-1 and Table 5.1.1-1
     */
    PacketType packet_type() const noexcept
    {
        return m_packed_0.get(m_packet_type_tag);
    }

    /**
     * @brief Sets the VRT Packet Type
     * @param value Packet Type value to set
     * 
     * VRT Packet Type is 4 bits long at bit position 31
     * See VITA 49.2 Figure 5.1.1-1 and Table 5.1.1-1
     */
    void packet_type(PacketType value) noexcept
    {
        m_packed_0.set(value, m_packet_type_tag);
    }

    /**
     * @brief Returns the Class ID Enable flag
     * @return true if Class ID is enabled, otherwise false
     * 
     * Class ID Enable is 1 bit long at bit position 27
     * See VITA 49.2 Figure 5.1.1-1
     */
    bool class_id_enable() const noexcept
    {
        return m_packed_0.get(m_class_id_enable_tag);
    }

    /**
     * @brief Sets the Class ID Enable flag
     * @param value Class ID Enable flag value to set
     * 
     * Class ID Enable is 1 bit long at bit position 27
     * See VITA 49.2 Figure 5.1.1-1
     */
    void class_id_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_class_id_enable_tag);
    }

    /**
     * @brief Returns the TimeStamp-Integer (TSI) Code
     * @return TimeStamp-Integer (TSI) Code
     * 
     * TimeStamp-Integer (TSI) Code is 2 bits long at bit position 23
     * See VITA 49.2 Figure 5.1.1-1 and Table 5.1.1-2
     */
    TSI tsi() const noexcept
    {
        return m_packed_0.get(m_tsi_tag);
    }

    /**
     * @brief Sets the TimeStamp-Integer (TSI) Code
     * @param value TimeStamp-Integer (TSI) Code value to set
     * 
     * TimeStamp-Integer (TSI) Code is 2 bits long at bit position 23
     * See VITA 49.2 Figure 5.1.1-1 and Table 5.1.1-2
     */
    void tsi(TSI value) noexcept
    {
        m_packed_0.set(value, m_tsi_tag);
    }

    /**
     * @brief Returns the TimeStamp-Fractional (TSF) Code
     * @return TimeStamp-Fractional (TSF) Code
     * 
     * TimeStamp-Fractional (TSF) Code is 2 bits long at bit position 21
     * See VITA 49.2 Figure 5.1.1-1 and Table 5.1.1-3
     */
    TSF tsf() const noexcept
    {
        return m_packed_0.get(m_tsf_tag);
    }

    /**
     * @brief Sets the TimeStamp-Fractional (TSF) Code
     * @param value TimeStamp-Fractional (TSF) Code value to set
     * 
     * TimeStamp-Fractional (TSF) Code is 2 bits long at bit position 21
     * See VITA 49.2 Figure 5.1.1-1 and Table 5.1.1-3
     */
    void tsf(TSF value) noexcept
    {
        m_packed_0.set(value, m_tsf_tag);
    }

    /**
     * @brief Returns the Packet Count
     * @return Packet Count
     * 
     * Packet Count is 4 bits long at bit position 19
     * See VITA 49.2 Figure 5.1.1-1
     */
    uint8_t packet_count() const noexcept
    {
        return m_packed_0.get(m_packet_count_tag);
    }

    /**
     * @brief Sets the Packet Count
     * @param value Packet Count value to set
     * 
     * Packet Count is 4 bits long at bit position 19
     * See VITA 49.2 Figure 5.1.1-1
     */
    void packet_count(uint8_t value) noexcept
    {
        m_packed_0.set(value, m_packet_count_tag);
    }

    /**
     * @brief Returns the Packet Size
     * @return Packet Size
     * 
     * Packet Size is 16 bits long at bit position 15
     * See VITA 49.2 Figure 5.1.1-1
     */
    uint16_t packet_size() const noexcept
    {
        return m_packed_0.get(m_packet_size_tag);
    }

    /**
     * @brief Sets the Packet Size
     * @param value Packet Size value to set
     * 
     * Packet Size is 16 bits long at bit position 15
     * See VITA 49.2 Figure 5.1.1-1
     */
    void packet_size(uint16_t value) noexcept
    {
        m_packed_0.set(value, m_packet_size_tag);
    }

    /**
     * @brief Returns the number of Header bytes
     * @return Number of Header bytes
     */
    constexpr std::size_t size() const noexcept
    {
        return m_packed_0.size();
    }

    /**
     * @brief Pack Header as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add Header bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed_0.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into Header
     * @param buffer_ptr Pointer to beginning of Header bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        m_packed_0.unpack_from(buffer_ptr);
    }

protected:
    vrtgen::packed<uint32_t> m_packed_0; //!< Packed Header bits structure

private:
    /**
     * packet_type 0/31
     * class_id_enable 0/31
     * TSI 0/31
     * TSF 0/31
     * packet_count 0/31
     * packet_size 0/31
     */
    vrtgen::packed_tag<PacketType,31,4> m_packet_type_tag;
    vrtgen::packed_tag<bool,27,1> m_class_id_enable_tag;
    vrtgen::packed_tag<TSI,23,2> m_tsi_tag;
    vrtgen::packed_tag<TSF,21,2> m_tsf_tag;
    vrtgen::packed_tag<uint8_t,19,4> m_packet_count_tag;
    vrtgen::packed_tag<uint16_t,15,16> m_packet_size_tag;

}; // end class Header

/**
 * @class DataHeader
 * @brief VRT Packet Header with Data Packet-Specific Indicator Bits (5.1.1.1)
 */
class DataHeader : public Header
{
public:
    /**
     * @brief Returns the Trailer Included flag
     * @return true if Trailer is included in the packet, otherwise false
     * 
     * Trailer Included is 1 bit long at bit position 26
     * See VITA 49.2 Figure 6.1-2 and Table 5.1.1.1-1
     */
    bool trailer_included() const noexcept
    {
        return m_packed_0.get(m_trailer_included_tag);
    }

    /**
     * @brief Sets the Trailer Included flag
     * @param value Trailer Included flag value to set
     * 
     * Trailer Included is 1 bit long at bit position 26
     * See VITA 49.2 Figure 6.1-2 and Table 5.1.1.1-1
     */
    void trailer_included(bool value) noexcept
    {
        m_packed_0.set(value, m_trailer_included_tag);
    }

    /**
     * @brief Returns the Not a V49.0 Packet Indicator (Nd0) flag
     * @return true if packet is not V49.0 compliant, otherwise false
     * 
     * Not a V49.0 Packet Indicator is 1 bit long at bit position 25
     * See VITA 49.2 Figure 6.1-2 and Table 5.1.1.1-1
     */
    bool not_v49d0() const noexcept
    {
        return m_packed_0.get(m_not_v49d0_tag);
    }

    /**
     * @brief Sets the Not a V49.0 Packet Indicator (Nd0) flag
     * @param value Not a V49.0 Packet Indicator (Nd0) flag value to set
     * 
     * Not a V49.0 Packet Indicator (Nd0) is 1 bit long at bit position 25
     * See VITA 49.2 Figure 6.1-2 and Table 5.1.1.1-1
     */
    void not_v49d0(bool value) noexcept
    {
        m_packed_0.set(value, m_not_v49d0_tag);
    }

    /**
     * @brief Returns the Signal Spectrum or Signal Time Data flag
     * @return true if Signal Spectrum or Signal Time Data is enabled, otherwise false
     * 
     * Signal Spectrum or Signal Time Data is 1 bit long at bit position 24
     * See VITA 49.2 Figure 6.1-2 and Table 5.1.1.1-1
     */
    bool spectrum_or_time() const noexcept
    {
        return m_packed_0.get(m_spectrum_or_time_tag);
    }

    /**
     * @brief Sets the Signal Spectrum or Signal Time Data flag
     * @param value Signal Spectrum or Signal Time Data flag value to set
     * 
     * Signal Spectrum or Signal Time Data is 1 bit long at bit position 24
     * See VITA 49.2 Figure 6.1-2 and Table 5.1.1.1-1
     */
    void spectrum_or_time(bool value) noexcept
    {
        m_packed_0.set(value, m_spectrum_or_time_tag);
    }

private:
    /**
     * trailer_included 0/26
     * not_v49d0 0/25
     * spectrum_or_time 0/24
     */
    vrtgen::packed_tag<bool,26,1> m_trailer_included_tag;
    vrtgen::packed_tag<bool,25,1> m_not_v49d0_tag;
    vrtgen::packed_tag<bool,24,1> m_spectrum_or_time_tag;

}; // end class DataHeader

/**
 * @class ContextHeader
 * @brief VRT Packet Header with Context Packet-Specific Indicator Bits (5.1.1.1)
 */
class ContextHeader : public Header
{
public:
    /**
     * @brief Returns the Not a V49.0 Packet Indicator (Nd0) flag
     * @return true if packet is not V49.0 compliant, otherwise false
     * 
     * Not a V49.0 Packet Indicator is 1 bit long at bit position 25
     * See VITA 49.2 Figure 7.1.1-1 and Table 5.1.1.1-1
     */
    bool not_v49d0() const noexcept
    {
        return m_packed_0.get(m_not_v49d0_tag);
    }

    /**
     * @brief Sets the Not a V49.0 Packet Indicator (Nd0) flag
     * @param value Not a V49.0 Packet Indicator (Nd0) flag value to set
     * 
     * Not a V49.0 Packet Indicator (Nd0) is 1 bit long at bit position 25
     * See VITA 49.2 Figure 7.1.1-1 and Table 5.1.1.1-1
     */
    void not_v49d0(bool value) noexcept
    {
        m_packed_0.set(value, m_not_v49d0_tag);
    }

    /**
     * @brief Returns the TimeStamp Mode (TSM)
     * @return TimeStamp Mode (TSM)
     * 
     * TimeStamp Mode (TSM) is 1 bit long at bit position 24
     * See VITA 49.2 Figure 7.1.1-1 and Table 5.1.1.1-1
     */
    TSM tsm() const noexcept
    {
        return TSM{ m_packed_0.get(m_tsm_tag) };
    }

    /**
     * @brief Sets the TimeStamp Mode (TSM)
     * @param value TimeStamp Mode (TSM) value to set
     * 
     * TimeStamp Mode (TSM) is 1 bit long at bit position 24
     * See VITA 49.2 Figure 7.1.1-1 and Table 5.1.1.1-1
     */
    void tsm(TSM value) noexcept
    {
        m_packed_0.set(static_cast<bool>(value), m_tsm_tag);
    }

private:
    /**
     * not_v49d0 0/25
     * tsm 0/24
     */
    vrtgen::packed_tag<bool,25,1> m_not_v49d0_tag;
    vrtgen::packed_tag<bool,24,1> m_tsm_tag;

}; // end class ContextHeader

/**
 * @class CommandHeader
 * @brief VRT Packet Header with Command Packet-Specific Indicator Bits (5.1.1.1)
 */
class CommandHeader : public Header
{
public:
    /**
     * @brief Returns the Acknowledge Packet flag
     * @return true if packet is Acknowledge subtype, otherwise false
     * 
     * Acknowledge Packet is 1 bit long at bit position 26
     * See VITA 49.2 Figure 8.2-2 and Table 5.1.1.1-1
     */
    bool acknowledge_packet() const noexcept
    {
        return m_packed_0.get(m_acknowledge_packet_tag);
    }

    /**
     * @brief Sets the Acknowledge Packet flag
     * @param value Acknowledge Packet flag value to set
     * 
     * Acknowledge Packet is 1 bit long at bit position 26
     * See VITA 49.2 Figure 8.2-2 and Table 5.1.1.1-1
     */
    void acknowledge_packet(bool value) noexcept
    {
        m_packed_0.set(value, m_acknowledge_packet_tag);
    }

    /**
     * @brief Returns the Cancellation Packet flag
     * @return true if packet is Control-Cancellation subtype, otherwise false
     * 
     * Cancellation Packet is 1 bit long at bit position 24
     * See VITA 49.2 Figure 8.2-2 and Table 5.1.1.1-1
     */
    bool cancellation_packet() const noexcept
    {
        return m_packed_0.get(m_cancellation_packet_tag);
    }

     /**
     * @brief Returns the Cancellation Packet flag
     * @param value Cancellation Packet flag value to set
     * 
     * Cancellation Packet is 1 bit long at bit position 24
     * See VITA 49.2 Figure 8.2-2 and Table 5.1.1.1-1
     */
    void cancellation_packet(bool value) noexcept
    {
        m_packed_0.set(value, m_cancellation_packet_tag);
    }

private:
    /**
     * acknowledge_packet 0/26
     * cancellation_packet 0/24
     */
    vrtgen::packed_tag<bool,26,1> m_acknowledge_packet_tag;
    vrtgen::packed_tag<bool,24,1> m_cancellation_packet_tag;

}; // end class CommandHeader

} // end namespace vrtgen::packing

#endif // _VRTGEN_PACKING_HEADER_HPP
