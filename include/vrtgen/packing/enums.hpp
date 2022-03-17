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

#ifndef _VRTGEN_PACKING_ENUMS_HPP
#define _VRTGEN_PACKING_ENUMS_HPP

#include <cstdint>

namespace vrtgen::packing {

/**
 * @enum PacketType
 * @brief Packet Type field in the VRT Packet Header
 * 
 * Constants for the 4-bit Packet Type field in the VRT Packet Header.
 * Refer to VITA 49.2 Table 5.1.1-1.
 */
enum class PacketType : uint8_t
{
    SIGNAL_DATA = 0b0000, //!< Signal Data Packet without Stream Identifier
    SIGNAL_DATA_STREAM_ID = 0b0001, //!< Signal Data Packet with Stream Identifier
    EXTENSION_DATA = 0b0010, //!< Extension Data Packet without Stream Identifier
    EXTENSION_DATA_STREAM_ID = 0b0011, //!< Extension Data Packet with Stream Identifier
    CONTEXT = 0b0100, //!< Context Packet
    EXTENSION_CONTEXT = 0b0101, //!< Extension Context packet
    COMMAND = 0b0110, //!< Command Packet
    EXTENSION_COMMAND = 0b0111, //!< Extension Command Packet
    // Remaining values reserved for future VRT Packet Types
    // RESERVED_8 = 0b1000, //!< Reserved for future VRT Packet types
    // RESERVED_9 = 0b1001, //!< Reserved for future VRT Packet types
    // RESERVED_10 = 0b1010, //!< Reserved for future VRT Packet types
    // RESERVED_11 = 0b1011, //!< Reserved for future VRT Packet types
    // RESERVED_12 = 0b1100, //!< Reserved for future VRT Packet types
    // RESERVED_13 = 0b1101, //!< Reserved for future VRT Packet types
    // RESERVED_14 = 0b1110, //!< Reserved for future VRT Packet types
    // RESERVED_15 = 0b1111, //!< Reserved for future VRT Packet types
}; // end enum class PacketType


/**
 * @enum TSI
 * @brief TimeStamp-Integer (TSI) field in the VRT Packet Header
 * 
 * Constants for the 2-bit TimeStamp-Integer (TSI) field in the VRT Packet Header.
 * Refer to VITA 49.2 Table 5.1.1-2.
 */
enum class TSI : uint8_t
{
    NONE = 0b00, //!< No Integer-seconds Timestamp field included
    UTC = 0b01, //!< UTC
    GPS = 0b10, //!< GPS time
    OTHER = 0b11, //!< Other, must be documented
}; // end enum class TSI

/**
 * @enum TSF
 * @brief TimeStamp-Fractional (TSF) field in the VRT Packet Header
 * 
 * Constants for the 2-bit TimeStamp-Fractional (TSF) field in the VRT Packet Header.
 * Refer to VITA 49.2 Table 5.1.1-3.
 */
enum class TSF : uint8_t
{
    NONE = 0b00, //!< No Fractional-seconds Timestamp field included
    SAMPLE_COUNT = 0b01, //!< Sample Count Timestamp
    REAL_TIME = 0b10, //!< Real-Time (Picoseconds) Timestamp
    FREE_RUNNING = 0b11, //!< Free Running Count Timestamp
}; // end enum class TSI

/**
 * @enum TSM
 * @brief Timestamp Mode (TSM) field in the VRT Context Packet Header specific bits
 * 
 * Constants for the 1-bit Timestamp Mode (TSM) field in the VRT Packet Header.
 * Refer to VITA 49.2 Table 5.1.1.1-1, rules 7.1.1-3, 7.1.1-4, 7.1.1-5.
 */
enum class TSM : bool
{
    FINE = 0b0, //!< Timestamp being used to convey precise timing of events
    COARSE = 0b1, //!< Timestamp being used to convey general timing of events
}; // end enum class TSM

/**
 * @enum IdentifierFormat
 * @brief Controllee/Controller Identifier Format field in the ControlAcknowledgeMode
 * 
 * Constants for the 1-bit IdentifierFormat field in the ControlAcknowledgeMode.
 * Refer to VITA 49.2 Table 8.3.1-1.
 */
enum class IdentifierFormat
{
    WORD = 0b0, //!< 32-bit word
    UUID = 0b1, //!< 128-bit UUID
}; // end enum class IdentifierFormat

/**
 * @enum ActionMode
 * @brief Control Action Mode field in the ControlAcknowledgeMode
 * 
 * Constants for the 2-bit Control Action Mode field in the ControlAcknowledgeMode.
 * Refer to VITA 49.2 Table 8.3.1.3-1.
 */
enum class ActionMode
{
    NO_ACTION = 0b00, //!< Controllee takes no action
    DRY_RUN = 0b01, //!< Controllee does not implement controls but sends Ack packets as though it had
    EXECUTE = 0b10, //!< Controllee implements command and sends Ack packet(s)
    // RESERVED = 0x3, //!< Reserved
}; // end enum class ActionMode

/**
 * @enum TimestampControlMode
 * @brief Timestamp Control Mode field in the ControlAcknowledgeMode
 * 
 * Constants for the 3-bit Timestamp Control Mode field in the ControlAcknowledgeMode.
 * Refer to VITA 49.2 Table 8.3.1.3-1 and Table 8.3.1.7-1.
 */
enum class TimestampControlMode
{
    IGNORE = 0b000, //!< Execute(d) with no timestamp constraints
    DEVICE = 0b001, //!< Execute(d) within device timing precision window
    LATE = 0b010, //!< Execute(d) within device timing precision window,
                //!< or after if within application timing window
    EARLY = 0b011, //!< Execute(d) within device timing precision window,
                 //!< or before if within application timing window
    EARLY_LATE = 0b100, //!< Execute(d) within application timing execution
    // RESERVED_5 = 0b101, //!< Reserved
    // RESERVED_6 = 0b110, //!< Reserved
    TIMING_ISSUES = 0b111, //!< Did not execute some controls at specified time (Ack only)
}; // end enum class TimestampControlMode

enum class AGCMode
{
    /**
     * AGC/MGC Indicator:
     * MGC (0) - Manual Gain Control
     * AGC (1) - Automatic Gain Control active
     */
    MGC = 0x0,
    AGC = 0x1,
}; // end enum class AGCMode

enum class DataItemFormat
{
    /**
     * Data Item Format codes:
     * SIGNED_FIXED                  (00000) - Signed Fixed-Point
     * SIGNED_VRT_1                  (00001) - Signed VRT, 1-bit exponent
     * SIGNED_VRT_2                  (00001) - Signed VRT, 2-bit exponent
     * SIGNED_VRT_3                  (00001) - Signed VRT, 3-bit exponent
     * SIGNED_VRT_4                  (00001) - Signed VRT, 4-bit exponent
     * SIGNED_VRT_5                  (00001) - Signed VRT, 5-bit exponent
     * SIGNED_VRT_6                  (00001) - Signed VRT, 6-bit exponent
     * SIGNED_FIXED_NON_NORMALIZED   (00111) - Signed Fixed-Point Non-Normalized
     * (01000) - Reserved
     * (01001) - Reserved
     * (01010) - Reserved
     * (01011) - Reserved
     * (01100) - Reserved
     * IEEE754_HALF_PRECISION        (01101) - IEEE-754 Half-Precision Floating-Point
     * IEEE754_SINGLE_PRECISION      (01110) - IEEE-754 Single-Precision Floating-Point
     * IEEE754_DOUBLE_PRECISION      (01111) - IEEE-754 Double-Precision Floating-Point
     * UNSIGNED_FIXED                (10000) - Unsigned Fixed-Point
     * UNSIGNED_VRT_1                (10001) - Unsigned VRT, 1-bit exponent
     * UNSIGNED_VRT_2                (10001) - Unsigned VRT, 2-bit exponent
     * UNSIGNED_VRT_3                (10001) - Unsigned VRT, 3-bit exponent
     * UNSIGNED_VRT_4                (10001) - Unsigned VRT, 4-bit exponent
     * UNSIGNED_VRT_5                (10001) - Unsigned VRT, 5-bit exponent
     * UNSIGNED_VRT_6                (10001) - Unsigned VRT, 6-bit exponent
     * UNSIGNED_FIXED_NON_NORMALIZED (10111) - Unsigned Fixed-Point Non-Normalized
     * (11000) - Reserved
     * (11001) - Reserved
     * (11010) - Reserved
     * (11011) - Reserved
     * (11100) - Reserved
     * (11101) - Reserved
     * (11110) - Reserved
     * (11111) - Reserved
     */
    SIGNED_FIXED = 0x00,
    SIGNED_VRT_1 = 0x01,
    SIGNED_VRT_2 = 0x02,
    SIGNED_VRT_3 = 0x03,
    SIGNED_VRT_4 = 0x04,
    SIGNED_VRT_5 = 0x05,
    SIGNED_VRT_6 = 0x06,
    SIGNED_FIXED_NON_NORMALIZED = 0x07,
    RESERVED_8 = 0x08,
    RESERVED_9 = 0x09,
    RESERVED_10 = 0x0a,
    RESERVED_11 = 0x0b,
    RESERVED_12 = 0x0c,
    IEEE754_HALF_PRECISION = 0x0d,
    IEEE754_SINGLE_PRECISION = 0x0e,
    IEEE754_DOUBLE_PRECISION = 0x0f,
    UNSIGNED_FIXED = 0x10,
    UNSIGNED_VRT_1 = 0x11,
    UNSIGNED_VRT_2 = 0x12,
    UNSIGNED_VRT_3 = 0x13,
    UNSIGNED_VRT_4 = 0x14,
    UNSIGNED_VRT_5 = 0x15,
    UNSIGNED_VRT_6 = 0x16,
    UNSIGNED_FIXED_NON_NORMALIZED = 0x17,
    RESERVED_24 = 0x18,
    RESERVED_25 = 0x19,
    RESERVED_26 = 0x1a,
    RESERVED_27 = 0x1b,
    RESERVED_28 = 0x1c,
    RESERVED_29 = 0x1d,
    RESERVED_30 = 0x1e,
    RESERVED_31 = 0x1f,
}; // end enum class DataItemFormat

enum class DataSampleType
{
    /**
     * Data Sample real/complex type:
     * REAL              (00) - Real
     * COMPLEX_CARTESIAN (01) - Complex, Cartesian
     * COMPLEX_POLAR     (10) - Complex, Polar
     * (11) - Reserved
     */
    REAL = 0x0,
    COMPLEX_CARTESIAN = 0x1,
    COMPLEX_POLAR = 0x2,
    RESERVED = 0x3,
}; // end enum class DataSampleType

enum class EntrySize
{
    /**
     * Index List Entry Size Subfield Coding (Table 9.3.2-1):
     * EIGHT_BIT       (1) - 8-bit fields packed into 32 bits
     * SIXTEEN_BIT     (1) - 16-bit fields packed into 32 bits
     * THIRTY_TWO_BIT  (1) - 32-bit fields
     */
    EIGHT_BIT = 0x1,
    SIXTEEN_BIT = 0x2,
    THIRTY_TWO_BIT = 0x4,
}; // end enum class EntrySize

enum class PackingMethod
{
    /**
     * Data Format Packing Method flag:
     * PROCESSING_EFFICIENT (0) - Items are padded as necessary such that they
     * do not span 32-bit word boundaries
     * LINK_EFFICIENT       (1) - No padding of items
     */
    PROCESSING_EFFICIENT = 0x0,
    LINK_EFFICIENT = 0x1,
}; // end enum class PackingMethod

enum class SSI
{
    /**
     * Start/Stop of Sample Frame Indication (SSI) Bits:
     * SINGLE (00) - Sample Frames are not applicable to data packets, or the
     * entire Sample Frame is contained in a single data packet
     * FIRST  (01) - First data packet of current Sample Frame
     * MIDDLE (10) - Middle packet or packets of Sample Frame: "continuation"
     * indicator
     * FINAL  (11) - Final data packet of current Sample Frame
     */
    SINGLE = 0x0,
    FIRST = 0x1,
    MIDDLE = 0x2,
    FINAL = 0x3,
}; // end enum class SSI

enum class V49StandardCompliance
{
    /**
     * V49 Standard and Specification Compliance Number (Table 9.10.3-1)
     */
    V49_0 = 0x00000001,
    V49_1 = 0x00000002,
    V49_A = 0x00000003,
    V49_2 = 0x00000004,
}; // end enum class V49StandardCompliance

} // end namespace vrtgen::packing

#endif // _VRTGEN_PACKING_ENUMS_HPP