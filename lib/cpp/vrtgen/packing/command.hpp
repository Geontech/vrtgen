/*
 * Copyright (C) 2024 Geon Technologies, LLC
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

#include <sstream>

#include <vrtgen/types/packed.hpp>
#include "enums.hpp"
#include "indicator_fields.hpp"

namespace vrtgen::packing {

/**
 * @class ControlAcknowledgeMode
 * @brief Control/Acknowledge Mode (CAM) Field (8.2.1)
 */
class ControlAcknowledgeMode
{
public:
    /**
     * @brief Returns the Controllee Enable flag
     * @return true if Controllee ID is enabled, otherwise false
     * 
     * Controllee Enable is 1 bit long at bit position 31
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    bool controllee_enable() const noexcept
    {
        return m_packed.get<31>();
    }

    /**
     * @brief Sets the Controllee Enable flag
     * @param value Controllee Enable flag value to set
     * 
     * Controllee Enable is 1 bit long at bit position 31
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    void controllee_enable(bool value) noexcept
    {
        m_packed.set<31>(value);
    }

    /**
     * @brief Returns the Controllee Identifier Format
     * @return Controllee Identifier Format
     * 
     * Controllee Identifier Format is 1 bit long at bit position 30
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    IdentifierFormat controllee_format() const noexcept
    {
        return IdentifierFormat{ m_packed.get<30>() };
    }

    /**
     * @brief Sets the Controllee Identifier Format
     * @return Controllee Identifier Format value to set
     * 
     * Controllee Identifier Format is 1 bit long at bit position 30
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    void controllee_format(IdentifierFormat value) noexcept
    {
        m_packed.set<30>(static_cast<bool>(value));
    }

    /**
     * @brief Returns the Controller Enable flag
     * @return true if Controller ID is enabled, otherwise false
     * 
     * Controller Enable is 1 bit long at bit position 29
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    bool controller_enable() const noexcept
    {
        return m_packed.get<29>();
    }

    /**
     * @brief Sets the Controller Enable flag
     * @param value Controller Enable flag value to set
     * 
     * Controller Enable is 1 bit long at bit position 29
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    void controller_enable(bool value) noexcept
    {
        m_packed.set<29>(value);
    }

    /**
     * @brief Returns the Controller Identifier Format
     * @return Controller Identifier Format
     * 
     * Controller Identifier Format is 1 bit long at bit position 28
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    IdentifierFormat controller_format() const noexcept
    {
        return IdentifierFormat{ m_packed.get<28>() };
    }

    /**
     * @brief Sets the Controller Identifier Format
     * @return Controller Identifier Format value to set
     * 
     * Controller Identifier Format is 1 bit long at bit position 28
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    void controller_format(IdentifierFormat value) noexcept
    {
        m_packed.set<28>(static_cast<bool>(value));
    }

    /**
     * @brief Returns the Partial Packet Permitted flag
     * @return true if partial packet execution is allowed, otherwise false
     * 
     * Partial Packet Permitted is 1 bit long at bit position 27
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    bool permit_partial() const noexcept
    {
        return m_packed.get<27>();
    }

    /**
     * @brief Sets the Partial Packet Permitted flag
     * @param value Partial Packet Permitted flag value to set
     * 
     * Partial Packet Permitted is 1 bit long at bit position 27
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    void permit_partial(bool value) noexcept
    {
        m_packed.set<27>(value);
    }

    /**
     * @brief Returns the Permit Warnings flag
     * @return true if execution of fields with warnings is allowed, otherwise false
     * 
     * Permit Warnings is 1 bit long at bit position 26
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    bool permit_warnings() const noexcept
    {
        return m_packed.get<26>();
    }

    /**
     * @brief Sets the Permit Warnings flag
     * @param value Permit Warnings flag value to set
     * 
     * Permit Warnings is 1 bit long at bit position 26
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    void permit_warnings(bool value) noexcept
    {
        m_packed.set<26>(value);
    }

    /**
     * @brief Returns the Permit Errors flag
     * @return true if execution of fields with errors is allowed, otherwise false
     * 
     * Permit Errors is 1 bit long at bit position 25
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    bool permit_errors() const noexcept
    {
        return m_packed.get<25>();
    }

    /**
     * @brief Sets the Permit Errors flag
     * @param value Permit Errors flag value to set
     * 
     * Permit Errors is 1 bit long at bit position 25
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    void permit_errors(bool value) noexcept
    {
        m_packed.set<25>(value);
    }

    /**
     * @brief Returns the Control Action Mode
     * @return Control Action Mode
     * 
     * Control Action Mode is 2 bits long at bit position 24
     * See VITA 49.2-2017 Table 8.3.1.3-1
     */
    ActionMode action_mode() const noexcept
    {
        return m_packed.get<24,2,ActionMode>();
    }

    /**
     * @brief Sets the Control Action Mode
     * @param value Control Action Mode value to set
     * 
     * Control Action Mode is 2 bits long at bit position 24
     * See VITA 49.2-2017 Table 8.3.1.3-1
     */
    void action_mode(ActionMode value) noexcept
    {
        m_packed.set<24,2>(value);
    }

    /**
     * @brief Returns the Negative Acknowledge flag
     * @return true if NACK is enabled, otherwise false
     * 
     * Negative Acknowledge is 1 bit long at bit position 22
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    bool nack_only() const noexcept
    {
        return m_packed.get<22>();
    }

    /**
     * @brief Sets the Negative Acknowledge flag
     * @param value Negative Acknowledge flag value to set
     * 
     * Negative Acknowledge is 1 bit long at bit position 22
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    void nack_only(bool value) noexcept
    {
        m_packed.set<22>(value);
    }

    /**
     * @brief Returns the Timestamp Control Mode
     * @return Timestamp Control Mode
     * 
     * Timestamp Control Mode is 3 bits long at bit position 14
     * See VITA 49.2-2017 Table 8.3.1.3-1 and Table 8.3.1.7-1
     */
    TimestampControlMode timing_control() const noexcept
    {
        return m_packed.get<14,3,TimestampControlMode>();
    }

    /**
     * @brief Sets the Timestamp Control Mode
     * @param value Timestamp Control Mode value to set
     * 
     * Timestamp Control Mode is 3 bits long at bit position 14
     * See VITA 49.2-2017 Table 8.3.1.3-1 and Table 8.3.1.7-1
     */
    void timing_control(TimestampControlMode value) noexcept
    {
        m_packed.set<14,3>(value);
    }

    /**
     * @brief Returns the number of ControlAcknowledgeMode bytes
     * @return Number of ControlAcknowledgeMode bytes
     */
    constexpr size_t size() const noexcept
    {
        return m_packed.size();
    }

    /**
     * @brief Pack ControlAcknowledgeMode as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add ControlAcknowledgeMode bytes
     */
    inline void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into ControlAcknowledgeMode
     * @param buffer_ptr Pointer to beginning of ControlAcknowledgeMode bytes in the buffer
     */
    inline void unpack_from(const uint8_t* buffer_ptr)
    {
        // auto* ptr = buffer_ptr;
        m_packed.unpack_from(buffer_ptr);
    }

protected:
    vrtgen::packed<uint32_t> m_packed;

}; // end class ControlAcknowledgeMode

/**
 * @class ControlCAM
 * @brief Control Packet Control/Acknowledge Mode (CAM) Field (8.3.1)
 */
class ControlCAM : public ControlAcknowledgeMode
{
public:
    /**
     * @brief Returns the Request Validation Acknowledge flag
     * @return true if Validation Acknowledge is requested, otherwise false
     * 
     * Request Validation Acknowledge is 1 bit long at bit position 20
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    bool req_v() const noexcept
    {
        return m_packed.get<20>();
    }

    /**
     * @brief Sets the Request Validation Acknowledge flag
     * @param value Request Validation Acknowledge flag value to set
     * 
     * Request Validation Acknowledge is 1 bit long at bit position 20
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    void req_v(bool value) noexcept
    {
        m_packed.set<20>(value);
    }

    /**
     * @brief Returns the Request Execution Acknowledge flag
     * @return true if Execution Acknowledge is requested, otherwise false
     * 
     * Request Execution Acknowledge is 1 bit long at bit position 19
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    bool req_x() const noexcept
    {
        return m_packed.get<19>();
    }

    /**
     * @brief Sets the Request Execution Acknowledge flag
     * @param value Request Execution Acknowledge flag value to set
     * 
     * Request Execution Acknowledge is 1 bit long at bit position 19
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    void req_x(bool value) noexcept
    {
        m_packed.set<19>(value);
    }

    /**
     * @brief Returns the Request Query-State Acknowledge flag
     * @return true if Query-State Acknowledge is requested, otherwise false
     * 
     * Request Query-State Acknowledge is 1 bit long at bit position 18
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    bool req_s() const noexcept
    {
        return m_packed.get<18>();
    }

    /**
     * @brief Sets the Request Query-State Acknowledge flag
     * @param value Request Query-State Acknowledge flag value to set
     * 
     * Request Query-State Acknowledge is 1 bit long at bit position 18
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    void req_s(bool value) noexcept
    {
        m_packed.set<18>(value);
    }

    /**
     * @brief Returns the Request Warnings flag
     * @return true if warnings are requested, otherwise false
     * 
     * Request Warnings is 1 bit long at bit position 17
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    bool req_w() const noexcept
    {
        return m_packed.get<17>();
    }

    /**
     * @brief Sets the Request Warnings flag
     * @param value Request Warnings flag value to set
     * 
     * Request Warnings is 1 bit long at bit position 17
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    void req_w(bool value) noexcept
    {
        m_packed.set<17>(value);
    }

    /**
     * @brief Returns the Request Errors flag
     * @return true if errors are requested, otherwise false
     * 
     * Request Errors is 1 bit long at bit position 16
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    bool req_er() const noexcept
    {
        return m_packed.get<16>();
    }

    /**
     * @brief Sets the Request Errors flag
     * @param value Request Errors flag value to set
     * 
     * Request Errors is 1 bit long at bit position 16
     * See VITA 49.2-2017 Table 8.3.1-1
     */
    void req_er(bool value) noexcept
    {
        m_packed.set<16>(value);
    }

}; // end class ControlCAM

/**
 * @class AcknowledgeCAM
 * @brief Acknowledge Packet Control/Acknowledge Mode Field (8.4.1)
 */
class AcknowledgeCAM : public ControlAcknowledgeMode
{
public:
    /**
     * @brief Returns the Validation Acknowledge flag
     * @return true if packet is a Validation Acknowledge, otherwise false
     * 
     * Validation Acknowledge is 1 bit long at bit position 20
     * See VITA 49.2-2017 Table 8.4.1-1
     */
    bool ack_v() const noexcept
    {
        return m_packed.get<20>();
    }

    /**
     * @brief Sets the Validation Acknowledge flag
     * @param value Validation Acknowledge flag value to set
     * 
     * Validation Acknowledge is 1 bit long at bit position 20
     * See VITA 49.2-2017 Table 8.4.1-1
     */
    void ack_v(bool value) noexcept
    {
        m_packed.set<20>(value);
    }

    /**
     * @brief Returns the Execution Acknowledge flag
     * @return true if packet is a Execution Acknowledge, otherwise false
     * 
     * Execution Acknowledge is 1 bit long at bit position 19
     * See VITA 49.2-2017 Table 8.4.1-1
     */
    bool ack_x() const noexcept
    {
        return m_packed.get<19>();
    }

    /**
     * @brief Sets the Execution Acknowledge flag
     * @param value Execution Acknowledge flag value to set
     * 
     * Execution Acknowledge is 1 bit long at bit position 19
     * See VITA 49.2-2017 Table 8.4.1-1
     */
    void ack_x(bool value) noexcept
    {
        m_packed.set<19>(value);
    }

    /**
     * @brief Returns the Query-State Acknowledge flag
     * @return true if packet is a Query-State Acknowledge, otherwise false
     * 
     * Query-State Acknowledge is 1 bit long at bit position 18
     * See VITA 49.2-2017 Table 8.4.1-1
     */
    bool ack_s() const noexcept
    {
        return m_packed.get<18>();
    }

    /**
     * @brief Sets the Query-State Acknowledge flag
     * @param value Query-State Acknowledge flag value to set
     * 
     * Query-State Acknowledge is 1 bit long at bit position 18
     * See VITA 49.2-2017 Table 8.4.1-1
     */
    void ack_s(bool value) noexcept
    {
        m_packed.set<18>(value);
    }

    /**
     * @brief Returns the Warnings Acknowledge flag
     * @return true if warnings were generated, otherwise false
     * 
     * Warnings Acknowledge is 1 bit long at bit position 17
     * See VITA 49.2-2017 Table 8.4.1-1
     */
    bool ack_w() const noexcept
    {
        return m_packed.get<17>();
    }

    /**
     * @brief Sets the Warnings Acknowledge flag
     * @param value Warnings Acknowledge flag value to set
     * 
     * Warnings Acknowledge is 1 bit long at bit position 17
     * See VITA 49.2-2017 Table 8.4.1-1
     */
    void ack_w(bool value) noexcept
    {
        m_packed.set<17>(value);
    }

    /**
     * @brief Returns the Errors Acknowledge flag
     * @return true if errors were generated, otherwise false
     * 
     * Errors Acknowledge is 1 bit long at bit position 16
     * See VITA 49.2-2017 Table 8.4.1-1
     */
    bool ack_er() const noexcept
    {
        return m_packed.get<16>();
    }

    /**
     * @brief Sets the Errors Acknowledge flag
     * @param value Errors Acknowledge flag value to set
     * 
     * Errors Acknowledge is 1 bit long at bit position 16
     * See VITA 49.2-2017 Table 8.4.1-1
     */
    void ack_er(bool value) noexcept
    {
        m_packed.set<16>(value);
    }

    /**
     * @brief Returns the Partial Action flag
     * @return true if action partially executed, false if action fully executed
     * 
     * Partial Action is 1 bit long at bit position 11
     * See VITA 49.2-2017 Table 8.4.1-1
     */
    bool partial_action() const noexcept
    {
        return m_packed.get<11>();
    }

    /**
     * @brief Sets the Partial Action flag
     * @param value Partial Action flag value to set
     * 
     * Partial Action is 1 bit long at bit position 11
     * See VITA 49.2-2017 Table 8.4.1-1
     */
    void partial_action(bool value) noexcept
    {
        m_packed.set<11>(value);
    }

    /**
     * @brief Returns the Action Scheduled or Executed flag
     * @return true if all fields can be scheduled for execution in timestamp, otherwise false
     * 
     * Action Scheduled or Executed is 1 bit long at bit position 10
     * See VITA 49.2-2017 Table 8.4.1-1
     */
    bool scheduled_or_executed() const noexcept
    {
        return m_packed.get<10>();
    }

    /**
     * @brief Sets the Action Scheduled or Executed flag
     * @param value Action Scheduled or Executed flag value to set
     * 
     * Action Scheduled or Executed is 1 bit long at bit position 10
     * See VITA 49.2-2017 Table 8.4.1-1
     */
    void scheduled_or_executed(bool value) noexcept
    {
        m_packed.set<10>(value);
    }

}; // end class AcknowledgeCAM

/**
 * @class WIF0
 * @brief Warning Indicator Field 0
 */
class WIF0 : public IndicatorField0
{
public:
    /**
     * @brief Returns the Warning Indicator Field 7 enable flag
     * @return true if packet contains Warning Indicator Field 7 word, otherwise false
     * 
     * Warning Indicator Field 7 Enable is 1 bit long at bit position 7
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    bool wif7_enable() const noexcept
    {
        return IndicatorField0::if7_enable();
    }

    /**
     * @brief Sets the Warning Indicator Field 7 enable flag
     * @param value Warning Indicator Field 7 enable flag value to set
     * 
     * Warning Indicator Field 7 Enable is 1 bit long at bit position 7
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    void wif7_enable(bool value) noexcept
    {
        IndicatorField0::if7_enable(value);
    }

    /**
     * @brief Returns the Warning Indicator Field 3 enable flag
     * @return true if packet contains Warning Indicator Field 3 word, otherwise false
     * 
     * Warning Indicator Field 3 Enable is 1 bit long at bit position 3
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    bool wif3_enable() const noexcept
    {
        return IndicatorField0::if3_enable();;
    }

    /**
     * @brief Sets the Warning Indicator Field 3 enable flag
     * @param value Warning Indicator Field 3 enable flag value to set
     * 
     * Warning Indicator Field 3 Enable is 1 bit long at bit position 3
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    void wif3_enable(bool value) noexcept
    {
        IndicatorField0::if3_enable(value);
    }

    /**
     * @brief Returns the Warning Indicator Field 2 enable flag
     * @return true if packet contains Warning Indicator Field 2 word, otherwise false
     * 
     * Warning Indicator Field 2 Enable is 1 bit long at bit position 2
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    bool wif2_enable() const noexcept
    {
        return IndicatorField0::if2_enable();
    }

    /**
     * @brief Sets the Warning Indicator Field 2 enable flag
     * @param value Warning Indicator Field 2 enable flag value to set
     * 
     * Warning Indicator Field 2 Enable is 1 bit long at bit position 2
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    void wif2_enable(bool value) noexcept
    {
        IndicatorField0::if2_enable(value);
    }

    /**
     * @brief Returns the Warning Indicator Field 1 enable flag
     * @return true if packet contains Warning Indicator Field 7 word, otherwise false
     * 
     * Warning Indicator Field 1 Enable is 1 bit long at bit position 1
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    bool wif1_enable() const noexcept
    {
        return IndicatorField0::if1_enable();
    }

    /**
     * @brief Sets the Warning Indicator Field 1 enable flag
     * @param value Warning Indicator Field 1 enable flag value to set
     * 
     * Warning Indicator Field 1 Enable is 1 bit long at bit position 1
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    void wif1_enable(bool value) noexcept
    {
        IndicatorField0::if1_enable(value);
    }

}; // end class WIF0

/**
 * @class EIF0
 * @brief Error Indicator Field 0
 */
class EIF0 : public IndicatorField0
{
public:
    /**
     * @brief Returns the Error Indicator Field 7 enable flag
     * @return true if packet contains Error Indicator Field 7 word, otherwise false
     * 
     * Error Indicator Field 7 Enable is 1 bit long at bit position 7
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    bool eif7_enable() const noexcept
    {
        return IndicatorField0::if7_enable();
    }

    /**
     * @brief Sets the Error Indicator Field 7 enable flag
     * @param value Error Indicator Field 7 enable flag value to set
     * 
     * Error Indicator Field 7 Enable is 1 bit long at bit position 7
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    void eif7_enable(bool value) noexcept
    {
        IndicatorField0::if7_enable(value);
    }

    /**
     * @brief Returns the Error Indicator Field 3 enable flag
     * @return true if packet contains Error Indicator Field 3 word, otherwise false
     * 
     * Error Indicator Field 3 Enable is 1 bit long at bit position 3
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    bool eif3_enable() const noexcept
    {
        return IndicatorField0::if3_enable();;
    }

    /**
     * @brief Sets the Error Indicator Field 3 enable flag
     * @param value Error Indicator Field 3 enable flag value to set
     * 
     * Error Indicator Field 3 Enable is 1 bit long at bit position 3
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    void eif3_enable(bool value) noexcept
    {
        IndicatorField0::if3_enable(value);
    }

    /**
     * @brief Returns the Error Indicator Field 2 enable flag
     * @return true if packet contains Error Indicator Field 2 word, otherwise false
     * 
     * Error Indicator Field 2 Enable is 1 bit long at bit position 2
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    bool eif2_enable() const noexcept
    {
        return IndicatorField0::if2_enable();
    }

    /**
     * @brief Sets the Error Indicator Field 2 enable flag
     * @param value Error Indicator Field 2 enable flag value to set
     * 
     * Error Indicator Field 2 Enable is 1 bit long at bit position 2
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    void eif2_enable(bool value) noexcept
    {
        IndicatorField0::if2_enable(value);
    }

    /**
     * @brief Returns the Error Indicator Field 1 enable flag
     * @return true if packet contains Error Indicator Field 7 word, otherwise false
     * 
     * Error Indicator Field 1 Enable is 1 bit long at bit position 1
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    bool eif1_enable() const noexcept
    {
        return IndicatorField0::if1_enable();
    }

    /**
     * @brief Sets the Error Indicator Field 1 enable flag
     * @param value Error Indicator Field 1 enable flag value to set
     * 
     * Error Indicator Field 1 Enable is 1 bit long at bit position 1
     * See VITA 49.2-2017 Figure 8.4.1.2-1
     */
    void eif1_enable(bool value) noexcept
    {
        IndicatorField0::if1_enable(value);
    }

}; // end class EIF0


/**
 * @class WEIF1
 * @brief Warning/Error Indicator Field 1
 */
class WEIF1 : public IndicatorField1
{
}; // end class WEIF1

/**
 * @class WEIF2
 * @brief Warning/Error Indicator Field 2
 */
class WEIF2 : public IndicatorField2
{
}; // end class WEIF2

/**
 * @class WEIF3
 * @brief Warning/Error Indicator Field 3
 */
class WEIF3 : public IndicatorField3
{
}; // end class WEIF1

/**
 * @class WEIF7
 * @brief Warning/Error Indicator Field 7
 */
class WEIF7 : public IndicatorField7
{
}; // end class WEIF1

/**
 * @class WarningErrorFields
 * @brief Warning and Error Response Fields (8.4.1.2.1)
 */
class WarningErrorFields : public IndicatorField
{
public:
    /**
     * @brief Returns the Field Not Executed flag
     * @return true if warning/error field is present, otherwise false
     * 
     * Field Not Executed is 1 bit long at bit position 31
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    bool field_not_executed() const noexcept
    {
        return m_packed.get<31>();
    }

    /**
     * @brief Sets the Field Not Executed flag
     * @param value Field Not Exectued flag value to set
     * 
     * Field Not Executed is 1 bit long at bit position 31
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    void field_not_executed(bool value) noexcept
    {
        m_packed.set<31>(value);
    }

    /**
     * @brief Returns the Device Failure flag
     * @return true if warning/error field is present, otherwise false
     * 
     * Device Failure is 1 bit long at bit position 30
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    bool device_failure() const noexcept
    {
        return m_packed.get<30>();
    }

    /**
     * @brief Sets the Device Failure flag
     * @param value Device Failure flag value to set
     * 
     * Device Failure is 1 bit long at bit position 30
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    void device_failure(bool value) noexcept
    {
        m_packed.set<30>(value);
    }

    /**
     * @brief Returns the Erroneous Field flag
     * @return true if warning/error field is present, otherwise false
     * 
     * Erroneous Field is 1 bit long at bit position 29
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    bool erroneous_field() const noexcept
    {
        return m_packed.get<29>();
    }

    /**
     * @brief Sets the Erroneous Field flag
     * @param value Erroneous Field flag value to set
     * 
     * Erroneous Field is 1 bit long at bit position 29
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    void erroneous_field(bool value) noexcept
    {
        m_packed.set<29>(value);
    }

    /**
     * @brief Returns the Parameter Out Of Range flag
     * @return true if warning/error field is present, otherwise false
     * 
     * Parameter Out Of Range is 1 bit long at bit position 28
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    bool parameter_out_of_range() const noexcept
    {
        return m_packed.get<28>();
    }

    /**
     * @brief Sets the Parameter Out Of Range flag
     * @param value Parameter Out Of Range flag value to set
     * 
     * Parameter Out Of Range is 1 bit long at bit position 28
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    void parameter_out_of_range(bool value) noexcept
    {
        m_packed.set<28>(value);
    }

    /**
     * @brief Returns the Parameter Unsupported Precision flag
     * @return true if warning/error field is present, otherwise false
     * 
     * Parameter Unsupported Precision is 1 bit long at bit position 27
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    bool parameter_unsupported_precision() const noexcept
    {
        return m_packed.get<27>();
    }

    /**
     * @brief Sets the Parameter Unsupported Precision flag
     * @param value Parameter Unsupported Precision flag value to set
     * 
     * Parameter Unsupported Precision is 1 bit long at bit position 27
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    void parameter_unsupported_precision(bool value) noexcept
    {
        m_packed.set<27>(value);
    }

    /**
     * @brief Returns the Field Value Invalid flag
     * @return true if warning/error field is present, otherwise false
     * 
     * Field Value Invalid is 1 bit long at bit position 26
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    bool field_value_invalid() const noexcept
    {
        return m_packed.get<26>();
    }

    /**
     * @brief Sets the Field Value Invalid flag
     * @param value Field Value Invalid flag value to set
     * 
     * Field Value Invalid is 1 bit long at bit position 26
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    void field_value_invalid(bool value) noexcept
    {
        m_packed.set<26>(value);
    }

    /**
     * @brief Returns the Timestamp Problem flag
     * @return true if warning/error field is present, otherwise false
     * 
     * Timestamp Problem is 1 bit long at bit position 25
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    bool timestamp_problem() const noexcept
    {
        return m_packed.get<25>();
    }

    /**
     * @brief Sets the Timestamp Problem flag
     * @param value Timestamp Problem flag value to set
     * 
     * Timestamp Problem is 1 bit long at bit position 25
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    void timestamp_problem(bool value) noexcept
    {
        m_packed.set<25>(value);
    }

    /**
     * @brief Returns the Hazardous Power Levels flag
     * @return true if warning/error field is present, otherwise false
     * 
     * Hazardous Power Levels is 1 bit long at bit position 24
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    bool hazardous_power_levels() const noexcept
    {
        return m_packed.get<24>();
    }

    /**
     * @brief Sets the Hazardous Power Levels flag
     * @param value Hazardous Power Levels flag value to set
     * 
     * Hazardous Power Levels is 1 bit long at bit position 24
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    void hazardous_power_levels(bool value) noexcept
    {
        m_packed.set<24>(value);
    }

    /**
     * @brief Returns the Distortion flag
     * @return true if warning/error field is present, otherwise false
     * 
     * Distortion is 1 bit long at bit position 23
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    bool distortion() const noexcept
    {
        return m_packed.get<23>();
    }

    /**
     * @brief Sets the Distortion flag
     * @param value Distortion flag value to set
     * 
     * Distortion is 1 bit long at bit position 23
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    void distortion(bool value) noexcept
    {
        m_packed.set<23>(value);
    }

    /**
     * @brief Returns the In-Band Power Compliance flag
     * @return true if warning/error field is present, otherwise false
     * 
     * In-Band Power Compliance is 1 bit long at bit position 22
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    bool in_band_power_compliance() const noexcept
    {
        return m_packed.get<22>();
    }

    /**
     * @brief Sets the In-Band Power Compliance flag
     * @param value In-Band Power Compliance flag value to set
     * 
     * In-Band Power Compliance is 1 bit long at bit position 22
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    void in_band_power_compliance(bool value) noexcept
    {
        m_packed.set<22>(value);
    }

    /**
     * @brief Returns the Out-of-Band Power Compliance flag
     * @return true if warning/error field is present, otherwise false
     * 
     * Out-of-Band Power Compliance is 1 bit long at bit position 21
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    bool out_of_band_power_compliance() const noexcept
    {
        return m_packed.get<21>();
    }

    /**
     * @brief Sets the Out-of-Band Power Compliance flag
     * @param value Out-of-Band Power Compliance flag value to set
     * 
     * Out-of-Band Power Compliance is 1 bit long at bit position 21
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    void out_of_band_power_compliance(bool value) noexcept
    {
        m_packed.set<21>(value);
    }

    /**
     * @brief Returns the Co-Site Interference flag
     * @return true if warning/error field is present, otherwise false
     * 
     * Co-Site Interference is 1 bit long at bit position 20
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    bool cosite_interference() const noexcept
    {
        return m_packed.get<20>();
    }

    /**
     * @brief Sets the Co-Site Interference flag
     * @param value Co-Site Interference flag value to set
     * 
     * Co-Site Interference is 1 bit long at bit position 20
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    void cosite_interference(bool value) noexcept
    {
        m_packed.set<20>(value);
    }

    /**
     * @brief Returns the Regional Interference flag
     * @return true if warning/error field is present, otherwise false
     * 
     * Regional Interference is 1 bit long at bit position 19
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    bool regional_interference() const noexcept
    {
        return m_packed.get<19>();
    }

    /**
     * @brief Sets the Regional Interference flag
     * @param value Regional Interference flag value to set
     * 
     * Regional Interference is 1 bit long at bit position 19
     * See VITA 49.2-2017 Table 8.4.1.2.1-1
     */
    void regional_interference(bool value) noexcept
    {
        m_packed.set<19>(value);
    }

    std::string to_string() const noexcept
    {
        std::ostringstream oss;
        if (field_not_executed()) {
            oss << "Field Not Executed : true\n";
        }
        if (device_failure()) {
            oss << "Device Failure : true\n";
        }
        if (erroneous_field()) {
            oss << "Erroneous Field : true\n";
        }
        if (parameter_out_of_range()) {
            oss << "Parameter Out of Range : true\n";
        }
        if (parameter_unsupported_precision()) {
            oss << "Parameter Unsupported Precision : true\n";
        }
        if (field_value_invalid()) {
            oss << "Field Value Invalid : true\n";
        }
        if (timestamp_problem()) {
            oss << "Timestamp Problem : true\n";
        }
        if (hazardous_power_levels()) {
            oss << "Hazardous Power Levels : true\n";
        }
        if (distortion()) {
            oss << "Distortion : true\n";
        }
        if (in_band_power_compliance()) {
            oss << "In-Band Power Compliance : true\n";
        }
        if (out_of_band_power_compliance()) {
            oss << "Out-of-Band Power Compliance : true\n";
        }
        if (cosite_interference()) {
            oss << "Cosite Interference : true\n";
        }
        if (regional_interference()) {
            oss << "Regional Interference : true\n";
        }
        return oss.str();
    }

}; // end class WarningErrorFields

} // end namespace vrtgen::packing
