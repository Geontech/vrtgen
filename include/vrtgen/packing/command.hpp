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
#ifndef _VRTGEN_PACKING_COMMAND_HPP
#define _VRTGEN_PACKING_COMMAND_HPP

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
     * See VITA 49.2 Table 8.3.1-1
     */
    bool controllee_enable() const noexcept
    {
        return m_packed_0.get(m_controllee_enable_tag);
    }

    /**
     * @brief Sets the Controllee Enable flag
     * @param value Controllee Enable flag value to set
     * 
     * Controllee Enable is 1 bit long at bit position 31
     * See VITA 49.2 Table 8.3.1-1
     */
    void controllee_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_controllee_enable_tag);
    }

    /**
     * @brief Returns the Controllee Identifier Format
     * @return Controllee Identifier Format
     * 
     * Controllee Identifier Format is 1 bit long at bit position 30
     * See VITA 49.2 Table 8.3.1-1
     */
    IdentifierFormat controllee_format() const noexcept
    {
        return IdentifierFormat{ m_packed_0.get(m_controllee_format_tag) };
    }

    /**
     * @brief Sets the Controllee Identifier Format
     * @return Controllee Identifier Format value to set
     * 
     * Controllee Identifier Format is 1 bit long at bit position 30
     * See VITA 49.2 Table 8.3.1-1
     */
    void controllee_format(IdentifierFormat value) noexcept
    {
        m_packed_0.set(static_cast<bool>(value), m_controllee_format_tag);
    }

    /**
     * @brief Returns the Controller Enable flag
     * @return true if Controller ID is enabled, otherwise false
     * 
     * Controller Enable is 1 bit long at bit position 29
     * See VITA 49.2 Table 8.3.1-1
     */
    bool controller_enable() const noexcept
    {
        return m_packed_0.get(m_controller_enable_tag);
    }

    /**
     * @brief Sets the Controller Enable flag
     * @param value Controller Enable flag value to set
     * 
     * Controller Enable is 1 bit long at bit position 29
     * See VITA 49.2 Table 8.3.1-1
     */
    void controller_enable(bool value) noexcept
    {
        m_packed_0.set(value, m_controller_enable_tag);
    }

    /**
     * @brief Returns the Controller Identifier Format
     * @return Controller Identifier Format
     * 
     * Controller Identifier Format is 1 bit long at bit position 28
     * See VITA 49.2 Table 8.3.1-1
     */
    IdentifierFormat controller_format() const noexcept
    {
        return IdentifierFormat{ m_packed_0.get(m_controller_format_tag) };
    }

    /**
     * @brief Sets the Controller Identifier Format
     * @return Controller Identifier Format value to set
     * 
     * Controller Identifier Format is 1 bit long at bit position 28
     * See VITA 49.2 Table 8.3.1-1
     */
    void controller_format(IdentifierFormat value) noexcept
    {
        m_packed_0.set(static_cast<bool>(value), m_controller_format_tag);
    }

    /**
     * @brief Returns the Partial Packet Permitted flag
     * @return true if partial packet execution is allowed, otherwise false
     * 
     * Partial Packet Permitted is 1 bit long at bit position 27
     * See VITA 49.2 Table 8.3.1-1
     */
    bool permit_partial() const noexcept
    {
        return m_packed_0.get(m_permit_partial_tag);
    }

    /**
     * @brief Sets the Partial Packet Permitted flag
     * @param value Partial Packet Permitted flag value to set
     * 
     * Partial Packet Permitted is 1 bit long at bit position 27
     * See VITA 49.2 Table 8.3.1-1
     */
    void permit_partial(bool value) noexcept
    {
        m_packed_0.set(value, m_permit_partial_tag);
    }

    /**
     * @brief Returns the Permit Warnings flag
     * @return true if execution of fields with warnings is allowed, otherwise false
     * 
     * Permit Warnings is 1 bit long at bit position 26
     * See VITA 49.2 Table 8.3.1-1
     */
    bool permit_warnings() const noexcept
    {
        return m_packed_0.get(m_permit_warnings_tag);
    }

    /**
     * @brief Sets the Permit Warnings flag
     * @param value Permit Warnings flag value to set
     * 
     * Permit Warnings is 1 bit long at bit position 26
     * See VITA 49.2 Table 8.3.1-1
     */
    void permit_warnings(bool value) noexcept
    {
        m_packed_0.set(value, m_permit_warnings_tag);
    }

    /**
     * @brief Returns the Permit Errors flag
     * @return true if execution of fields with errors is allowed, otherwise false
     * 
     * Permit Errors is 1 bit long at bit position 25
     * See VITA 49.2 Table 8.3.1-1
     */
    bool permit_errors() const noexcept
    {
        return m_packed_0.get(m_permit_errors_tag);
    }

    /**
     * @brief Sets the Permit Errors flag
     * @param value Permit Errors flag value to set
     * 
     * Permit Errors is 1 bit long at bit position 25
     * See VITA 49.2 Table 8.3.1-1
     */
    void permit_errors(bool value) noexcept
    {
        m_packed_0.set(value, m_permit_errors_tag);
    }

    /**
     * @brief Returns the Control Action Mode
     * @return Control Action Mode
     * 
     * Control Action Mode is 2 bits long at bit position 24
     * See VITA 49.2 Table 8.3.1.3-1
     */
    ActionMode action_mode() const noexcept
    {
        return m_packed_0.get(m_action_mode_tag);
    }

    /**
     * @brief Sets the Control Action Mode
     * @param value Control Action Mode value to set
     * 
     * Control Action Mode is 2 bits long at bit position 24
     * See VITA 49.2 Table 8.3.1.3-1
     */
    void action_mode(ActionMode value) noexcept
    {
        m_packed_0.set(value, m_action_mode_tag);
    }

    /**
     * @brief Returns the Negative Acknowledge flag
     * @return true if NACK is enabled, otherwise false
     * 
     * Negative Acknowledge is 1 bit long at bit position 22
     * See VITA 49.2 Table 8.3.1-1
     */
    bool nack_only() const noexcept
    {
        return m_packed_0.get(m_nack_only_tag);
    }

    /**
     * @brief Sets the Negative Acknowledge flag
     * @param value Negative Acknowledge flag value to set
     * 
     * Negative Acknowledge is 1 bit long at bit position 22
     * See VITA 49.2 Table 8.3.1-1
     */
    void nack_only(bool value) noexcept
    {
        m_packed_0.set(value, m_nack_only_tag);
    }

    /**
     * @brief Returns the Timestamp Control Mode
     * @return Timestamp Control Mode
     * 
     * Timestamp Control Mode is 3 bits long at bit position 14
     * See VITA 49.2 Table 8.3.1.3-1 and Table 8.3.1.7-1
     */
    TimestampControlMode timing_control() const noexcept
    {
        return m_packed_0.get(m_timing_control_tag);
    }

    /**
     * @brief Sets the Timestamp Control Mode
     * @param value Timestamp Control Mode value to set
     * 
     * Timestamp Control Mode is 3 bits long at bit position 14
     * See VITA 49.2 Table 8.3.1.3-1 and Table 8.3.1.7-1
     */
    void timing_control(TimestampControlMode value) noexcept
    {
        m_packed_0.set(value, m_timing_control_tag);
    }

    /**
     * @brief Returns the number of ControlAcknowledgeMode bytes
     * @return Number of ControlAcknowledgeMode bytes
     */
    constexpr size_t size() const noexcept
    {
        return m_packed_0.size();
    }

    /**
     * @brief Pack ControlAcknowledgeMode as bytes into the buffer
     * @param buffer_ptr Pointer to buffer location to add ControlAcknowledgeMode bytes
     */
    void pack_into(uint8_t* buffer_ptr) const
    {
        m_packed_0.pack_into(buffer_ptr);
    }

    /**
     * @brief Unpack buffer bytes into ControlAcknowledgeMode
     * @param buffer_ptr Pointer to beginning of ControlAcknowledgeMode bytes in the buffer
     */
    void unpack_from(const uint8_t* buffer_ptr)
    {
        // auto* ptr = buffer_ptr;
        m_packed_0.unpack_from(buffer_ptr);
    }

protected:
    vrtgen::packed<uint32_t> m_packed_0;

private:
    /**
     * controllee_enable 0/31
     * controllee_format 0/30
     * controller_enable 0/29
     * controller_format 0/28
     * permit_partial 0/27
     * permit_warnings 0/26
     * permit_errors 0/25
     * action_mode 0/24
     * nack_only 0/22
     * timing_control 0/14
     */
    vrtgen::packed_tag<bool,31,1> m_controllee_enable_tag;
    vrtgen::packed_tag<bool,30,1> m_controllee_format_tag;
    vrtgen::packed_tag<bool,29,1> m_controller_enable_tag;
    vrtgen::packed_tag<bool,28,1> m_controller_format_tag;
    vrtgen::packed_tag<bool,27,1> m_permit_partial_tag;
    vrtgen::packed_tag<bool,26,1> m_permit_warnings_tag;
    vrtgen::packed_tag<bool,25,1> m_permit_errors_tag;
    vrtgen::packed_tag<ActionMode,24,2> m_action_mode_tag;
    vrtgen::packed_tag<bool,22,1> m_nack_only_tag;
    vrtgen::packed_tag<TimestampControlMode,14,3> m_timing_control_tag;

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
     * See VITA 49.2 Table 8.3.1-1
     */
    bool req_v() const noexcept
    {
        return m_packed_0.get(m_req_v_tag);
    }

    /**
     * @brief Sets the Request Validation Acknowledge flag
     * @param value Request Validation Acknowledge flag value to set
     * 
     * Request Validation Acknowledge is 1 bit long at bit position 20
     * See VITA 49.2 Table 8.3.1-1
     */
    void req_v(bool value) noexcept
    {
        m_packed_0.set(value, m_req_v_tag);
    }

    /**
     * @brief Returns the Request Execution Acknowledge flag
     * @return true if Execution Acknowledge is requested, otherwise false
     * 
     * Request Execution Acknowledge is 1 bit long at bit position 19
     * See VITA 49.2 Table 8.3.1-1
     */
    bool req_x() const noexcept
    {
        return m_packed_0.get(m_req_x_tag);
    }

    /**
     * @brief Sets the Request Execution Acknowledge flag
     * @param value Request Execution Acknowledge flag value to set
     * 
     * Request Execution Acknowledge is 1 bit long at bit position 19
     * See VITA 49.2 Table 8.3.1-1
     */
    void req_x(bool value) noexcept
    {
        m_packed_0.set(value, m_req_x_tag);
    }

    /**
     * @brief Returns the Request Query-State Acknowledge flag
     * @return true if Query-State Acknowledge is requested, otherwise false
     * 
     * Request Query-State Acknowledge is 1 bit long at bit position 18
     * See VITA 49.2 Table 8.3.1-1
     */
    bool req_s() const noexcept
    {
        return m_packed_0.get(m_req_s_tag);
    }

    /**
     * @brief Sets the Request Query-State Acknowledge flag
     * @param value Request Query-State Acknowledge flag value to set
     * 
     * Request Query-State Acknowledge is 1 bit long at bit position 18
     * See VITA 49.2 Table 8.3.1-1
     */
    void req_s(bool value) noexcept
    {
        m_packed_0.set(value, m_req_s_tag);
    }

    /**
     * @brief Returns the Request Warnings flag
     * @return true if warnings are requested, otherwise false
     * 
     * Request Warnings is 1 bit long at bit position 17
     * See VITA 49.2 Table 8.3.1-1
     */
    bool req_w() const noexcept
    {
        return m_packed_0.get(m_req_w_tag);
    }

    /**
     * @brief Sets the Request Warnings flag
     * @param value Request Warnings flag value to set
     * 
     * Request Warnings is 1 bit long at bit position 17
     * See VITA 49.2 Table 8.3.1-1
     */
    void req_w(bool value) noexcept
    {
        m_packed_0.set(value, m_req_w_tag);
    }

    /**
     * @brief Returns the Request Errors flag
     * @return true if errors are requested, otherwise false
     * 
     * Request Errors is 1 bit long at bit position 16
     * See VITA 49.2 Table 8.3.1-1
     */
    bool req_er() const noexcept
    {
        return m_packed_0.get(m_req_er_tag);
    }

    /**
     * @brief Sets the Request Errors flag
     * @param value Request Errors flag value to set
     * 
     * Request Errors is 1 bit long at bit position 16
     * See VITA 49.2 Table 8.3.1-1
     */
    void req_er(bool value) noexcept
    {
        m_packed_0.set(value, m_req_er_tag);
    }

private:
    /**
     * req_v 0/20
     * req_x 0/19
     * req_s 0/18
     * req_w 0/17
     * req_er 0/16
     */
    vrtgen::packed_tag<bool,20,1> m_req_v_tag;
    vrtgen::packed_tag<bool,19,1> m_req_x_tag;
    vrtgen::packed_tag<bool,18,1> m_req_s_tag;
    vrtgen::packed_tag<bool,17,1> m_req_w_tag;
    vrtgen::packed_tag<bool,16,1> m_req_er_tag;

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
     * See VITA 49.2 Table 8.4.1-1
     */
    bool ack_v() const noexcept
    {
        return m_packed_0.get(m_ack_v_tag);
    }

    /**
     * @brief Sets the Validation Acknowledge flag
     * @param value Validation Acknowledge flag value to set
     * 
     * Validation Acknowledge is 1 bit long at bit position 20
     * See VITA 49.2 Table 8.4.1-1
     */
    void ack_v(bool value) noexcept
    {
        m_packed_0.set(value, m_ack_v_tag);
    }

    /**
     * @brief Returns the Execution Acknowledge flag
     * @return true if packet is a Execution Acknowledge, otherwise false
     * 
     * Execution Acknowledge is 1 bit long at bit position 19
     * See VITA 49.2 Table 8.4.1-1
     */
    bool ack_x() const noexcept
    {
        return m_packed_0.get(m_ack_x_tag);
    }

    /**
     * @brief Sets the Execution Acknowledge flag
     * @param value Execution Acknowledge flag value to set
     * 
     * Execution Acknowledge is 1 bit long at bit position 19
     * See VITA 49.2 Table 8.4.1-1
     */
    void ack_x(bool value) noexcept
    {
        m_packed_0.set(value, m_ack_x_tag);
    }

    /**
     * @brief Returns the Query-State Acknowledge flag
     * @return true if packet is a Query-State Acknowledge, otherwise false
     * 
     * Query-State Acknowledge is 1 bit long at bit position 18
     * See VITA 49.2 Table 8.4.1-1
     */
    bool ack_s() const noexcept
    {
        return m_packed_0.get(m_ack_s_tag);
    }

    /**
     * @brief Sets the Query-State Acknowledge flag
     * @param value Query-State Acknowledge flag value to set
     * 
     * Query-State Acknowledge is 1 bit long at bit position 18
     * See VITA 49.2 Table 8.4.1-1
     */
    void ack_s(bool value) noexcept
    {
        m_packed_0.set(value, m_ack_s_tag);
    }

    /**
     * @brief Returns the Warnings Acknowledge flag
     * @return true if warnings were generated, otherwise false
     * 
     * Warnings Acknowledge is 1 bit long at bit position 17
     * See VITA 49.2 Table 8.4.1-1
     */
    bool ack_w() const noexcept
    {
        return m_packed_0.get(m_ack_w_tag);
    }

    /**
     * @brief Sets the Warnings Acknowledge flag
     * @param value Warnings Acknowledge flag value to set
     * 
     * Warnings Acknowledge is 1 bit long at bit position 17
     * See VITA 49.2 Table 8.4.1-1
     */
    void ack_w(bool value) noexcept
    {
        m_packed_0.set(value, m_ack_w_tag);
    }

    /**
     * @brief Returns the Errors Acknowledge flag
     * @return true if errors were generated, otherwise false
     * 
     * Errors Acknowledge is 1 bit long at bit position 16
     * See VITA 49.2 Table 8.4.1-1
     */
    bool ack_er() const noexcept
    {
        return m_packed_0.get(m_ack_er_tag);
    }

    /**
     * @brief Sets the Errors Acknowledge flag
     * @param value Errors Acknowledge flag value to set
     * 
     * Errors Acknowledge is 1 bit long at bit position 16
     * See VITA 49.2 Table 8.4.1-1
     */
    void ack_er(bool value) noexcept
    {
        m_packed_0.set(value, m_ack_er_tag);
    }

    /**
     * @brief Returns the Partial Action flag
     * @return true if action partially executed, false if action fully executed
     * 
     * Partial Action is 1 bit long at bit position 11
     * See VITA 49.2 Table 8.4.1-1
     */
    bool partial_action() const noexcept
    {
        return m_packed_0.get(m_partial_action_tag);
    }

    /**
     * @brief Sets the Partial Action flag
     * @param value Partial Action flag value to set
     * 
     * Partial Action is 1 bit long at bit position 11
     * See VITA 49.2 Table 8.4.1-1
     */
    void partial_action(bool value) noexcept
    {
        m_packed_0.set(value, m_partial_action_tag);
    }

    /**
     * @brief Returns the Action Scheduled or Executed flag
     * @return true if all fields can be scheduled for execution in timestamp, otherwise false
     * 
     * Action Scheduled or Executed is 1 bit long at bit position 10
     * See VITA 49.2 Table 8.4.1-1
     */
    bool scheduled_or_executed() const noexcept
    {
        return m_packed_0.get(m_scheduled_or_executed_tag);
    }

    /**
     * @brief Sets the Action Scheduled or Executed flag
     * @param value Action Scheduled or Executed flag value to set
     * 
     * Action Scheduled or Executed is 1 bit long at bit position 10
     * See VITA 49.2 Table 8.4.1-1
     */
    void scheduled_or_executed(bool value) noexcept
    {
        m_packed_0.set(value, m_scheduled_or_executed_tag);
    }

private:
    /**
     * ack_v 0/20
     * ack_x 0/19
     * ack_s 0/18
     * ack_w 0/17
     * ack_er 0/16
     * partial_action 0/11
     * scheduled_or_executed 0/10
     */
    vrtgen::packed_tag<bool,20,1> m_ack_v_tag;
    vrtgen::packed_tag<bool,19,1> m_ack_x_tag;
    vrtgen::packed_tag<bool,18,1> m_ack_s_tag;
    vrtgen::packed_tag<bool,17,1> m_ack_w_tag;
    vrtgen::packed_tag<bool,16,1> m_ack_er_tag;
    vrtgen::packed_tag<bool,11,1> m_partial_action_tag;
    vrtgen::packed_tag<bool,10,1> m_scheduled_or_executed_tag;

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
     * See VITA 49.2 Figure 8.4.1.2-1
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
     * See VITA 49.2 Figure 8.4.1.2-1
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
     * See VITA 49.2 Figure 8.4.1.2-1
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
     * See VITA 49.2 Figure 8.4.1.2-1
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
     * See VITA 49.2 Figure 8.4.1.2-1
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
     * See VITA 49.2 Figure 8.4.1.2-1
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
     * See VITA 49.2 Figure 8.4.1.2-1
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
     * See VITA 49.2 Figure 8.4.1.2-1
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
     * See VITA 49.2 Figure 8.4.1.2-1
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
     * See VITA 49.2 Figure 8.4.1.2-1
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
     * See VITA 49.2 Figure 8.4.1.2-1
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
     * See VITA 49.2 Figure 8.4.1.2-1
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
     * See VITA 49.2 Figure 8.4.1.2-1
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
     * See VITA 49.2 Figure 8.4.1.2-1
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
     * See VITA 49.2 Figure 8.4.1.2-1
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
     * See VITA 49.2 Figure 8.4.1.2-1
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
    bool field_not_executed() const noexcept
    {
        return m_packed_0.get(m_field_not_executed_tag);
    }

    void field_not_executed(bool value) noexcept
    {
        m_packed_0.set(value, m_field_not_executed_tag);
    }

    bool device_failure() const noexcept
    {
        return m_packed_0.get(m_device_failure_tag);
    }

    void device_failure(bool value) noexcept
    {
        m_packed_0.set(value, m_device_failure_tag);
    }

    bool erroneuous_field() const noexcept
    {
        return m_packed_0.get(m_erroneuous_field_tag);
    }

    void erroneuous_field(bool value) noexcept
    {
        m_packed_0.set(value, m_erroneuous_field_tag);
    }

    bool parameter_out_of_range() const noexcept
    {
        return m_packed_0.get(m_parameter_out_of_range_tag);
    }

    void parameter_out_of_range(bool value) noexcept
    {
        m_packed_0.set(value, m_parameter_out_of_range_tag);
    }

    bool parameter_unsupported_precision() const noexcept
    {
        return m_packed_0.get(m_parameter_unsupported_precision_tag);
    }

    void parameter_unsupported_precision(bool value) noexcept
    {
        m_packed_0.set(value, m_parameter_unsupported_precision_tag);
    }

    bool field_value_invalid() const noexcept
    {
        return m_packed_0.get(m_field_value_invalid_tag);
    }

    void field_value_invalid(bool value) noexcept
    {
        m_packed_0.set(value, m_field_value_invalid_tag);
    }

    bool timestamp_problem() const noexcept
    {
        return m_packed_0.get(m_timestamp_problem_tag);
    }

    void timestamp_problem(bool value) noexcept
    {
        m_packed_0.set(value, m_timestamp_problem_tag);
    }

    bool hazardous_power_levels() const noexcept
    {
        return m_packed_0.get(m_hazardous_power_levels_tag);
    }

    void hazardous_power_levels(bool value) noexcept
    {
        m_packed_0.set(value, m_hazardous_power_levels_tag);
    }

    bool distortion() const noexcept
    {
        return m_packed_0.get(m_distortion_tag);
    }

    void distortion(bool value) noexcept
    {
        m_packed_0.set(value, m_distortion_tag);
    }

    bool in_band_power_compliance() const noexcept
    {
        return m_packed_0.get(m_in_band_power_compliance_tag);
    }

    void in_band_power_compliance(bool value) noexcept
    {
        m_packed_0.set(value, m_in_band_power_compliance_tag);
    }

    bool out_of_band_power_compliance() const noexcept
    {
        return m_packed_0.get(m_out_of_band_power_compliance_tag);
    }

    void out_of_band_power_compliance(bool value) noexcept
    {
        m_packed_0.set(value, m_out_of_band_power_compliance_tag);
    }

    bool cosite_interference() const noexcept
    {
        return m_packed_0.get(m_cosite_interference_tag);
    }

    void cosite_interference(bool value) noexcept
    {
        m_packed_0.set(value, m_cosite_interference_tag);
    }

    bool regional_interference() const noexcept
    {
        return m_packed_0.get(m_regional_interference_tag);
    }

    void regional_interference(bool value) noexcept
    {
        m_packed_0.set(value, m_regional_interference_tag);
    }

private:
    /**
     * field_not_executed 0/31
     * device_failure 0/30
     * erroneuous_field 0/29
     * parameter_out_of_range 0/28
     * parameter_unsupported_precision 0/27
     * field_value_invalid 0/26
     * timestamp_problem 0/25
     * hazardous_power_levels 0/24
     * distortion 0/23
     * in_band_power_compliance 0/22
     * out_of_band_power_compliance 0/21
     * cosite_interference 0/20
     * regional_interference 0/19
     */
    vrtgen::packed_tag<bool,31,1> m_field_not_executed_tag;
    vrtgen::packed_tag<bool,30,1> m_device_failure_tag;
    vrtgen::packed_tag<bool,29,1> m_erroneuous_field_tag;
    vrtgen::packed_tag<bool,28,1> m_parameter_out_of_range_tag;
    vrtgen::packed_tag<bool,27,1> m_parameter_unsupported_precision_tag;
    vrtgen::packed_tag<bool,26,1> m_field_value_invalid_tag;
    vrtgen::packed_tag<bool,25,1> m_timestamp_problem_tag;
    vrtgen::packed_tag<bool,24,1> m_hazardous_power_levels_tag;
    vrtgen::packed_tag<bool,23,1> m_distortion_tag;
    vrtgen::packed_tag<bool,22,1> m_in_band_power_compliance_tag;
    vrtgen::packed_tag<bool,21,1> m_out_of_band_power_compliance_tag;
    vrtgen::packed_tag<bool,20,1> m_cosite_interference_tag;
    vrtgen::packed_tag<bool,19,1> m_regional_interference_tag;

}; // end class WarningErrorFields

} // end namespace vrtgen::packing

#endif // _VRTGEN_PACKING_COMMAND_HPP
