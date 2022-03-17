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

#include "catch.hpp"
#include "bytes.hpp"

#include "vrtgen/packing/command.hpp"

using namespace vrtgen::packing;

TEST_CASE("Control ControlAcknowledgeMode", "[command]")
{
    ControlCAM cam;
    ControlCAM unpack_cam;
    bytes packed_bytes{ 0xFF, 0xFF, 0xFF, 0xFF };

    SECTION("Zero on construction")
    {
        cam.pack_into(packed_bytes.data());
        CHECK(packed_bytes == bytes{ 0, 0, 0, 0 });
    }

    SECTION("Controllee Enable")
    {
        // Verify zero on construction
        CHECK(cam.controllee_enable() == false);
        // Setter
        cam.controllee_enable(true);
        // Getter check set value
        CHECK(cam.controllee_enable() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x80);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.controllee_enable() == true);
    }

    SECTION("Controllee Format")
    {
        // Verify zero on construction
        CHECK(cam.controllee_format() == IdentifierFormat::WORD);
        // Setter
        cam.controllee_format(IdentifierFormat::UUID);
        // Getter check set value
        CHECK(cam.controllee_format() == IdentifierFormat::UUID);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x40);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.controllee_format() == IdentifierFormat::UUID);
    }

    SECTION("Controller Enable")
    {
        // Verify zero on construction
        CHECK(cam.controller_enable() == false);
        // Setter
        cam.controller_enable(true);
        // Getter check set value
        CHECK(cam.controller_enable() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x20);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.controller_enable() == true);
    }

    SECTION("Controller Format")
    {
        // Verify zero on construction
        CHECK(cam.controller_format() == IdentifierFormat::WORD);
        // Setter
        cam.controller_format(IdentifierFormat::UUID);
        // Getter check set value
        CHECK(cam.controller_format() == IdentifierFormat::UUID);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x10);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.controller_format() == IdentifierFormat::UUID);
    }

    SECTION("Partial Packet Permitted")
    {
        // Verify zero on construction
        CHECK(cam.permit_partial() == false);
        // Setter
        cam.permit_partial(true);
        // Getter check set value
        CHECK(cam.permit_partial() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x08);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.permit_partial() == true);
    }

    SECTION("Permit Warnings")
    {
        // Verify zero on construction
        CHECK(cam.permit_warnings() == false);
        // Setter
        cam.permit_warnings(true);
        // Getter check set value
        CHECK(cam.permit_warnings() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x04);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.permit_warnings() == true);
    }

    SECTION("Permit Errors")
    {
        // Verify zero on construction
        CHECK(cam.permit_errors() == false);
        // Setter
        cam.permit_errors(true);
        // Getter check set value
        CHECK(cam.permit_errors() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x02);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.permit_errors() == true);
    }

    SECTION("Action Mode")
    {
        // Verify zero on construction
        CHECK(cam.action_mode() == ActionMode::NO_ACTION);
        // Execute
        // Setter
        cam.action_mode(ActionMode::EXECUTE);
        // Getter check set value
        CHECK(cam.action_mode() == ActionMode::EXECUTE);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[0] == 0x01);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.action_mode() == ActionMode::EXECUTE);
        // Dry Run
        // Setter
        cam.action_mode(ActionMode::DRY_RUN);
        // Getter check set value
        CHECK(cam.action_mode() == ActionMode::DRY_RUN);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x80);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.action_mode() == ActionMode::DRY_RUN);
    }

    SECTION("Not-Ack Only")
    {
        // Verify zero on construction
        CHECK(cam.nack_only() == false);
        // Setter
        cam.nack_only(true);
        // Getter check set value
        CHECK(cam.nack_only() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x40);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.nack_only() == true);
    }

    SECTION("Request Validation")
    {
        // Verify zero on construction
        CHECK(cam.req_v() == false);
        // Setter
        cam.req_v(true);
        // Getter check set value
        CHECK(cam.req_v() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x10);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.req_v() == true);
    }

    SECTION("Request Execution")
    {
        // Verify zero on construction
        CHECK(cam.req_x() == false);
        // Setter
        cam.req_x(true);
        // Getter check set value
        CHECK(cam.req_x() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x08);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.req_x() == true);
    }

    SECTION("Request Query-State")
    {
        // Verify zero on construction
        CHECK(cam.req_s() == false);
        // Setter
        cam.req_s(true);
        // Getter check set value
        CHECK(cam.req_s() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x04);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.req_s() == true);
    }

    SECTION("Request Warnings")
    {
        // Verify zero on construction
        CHECK(cam.req_w() == false);
        // Setter
        cam.req_w(true);
        // Getter check set value
        CHECK(cam.req_w() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x02);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.req_w() == true);
    }

    SECTION("Request Errors")
    {
        // Verify zero on construction
        CHECK(cam.req_er() == false);
        // Setter
        cam.req_er(true);
        // Getter check set value
        CHECK(cam.req_er() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x01);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.req_er() == true);
    }

    SECTION("Timestamp Control Mode")
    {
        // Verify zero on construction
        CHECK(cam.timing_control() == TimestampControlMode::IGNORE);
        // EARLY_LATE
        // Setter
        cam.timing_control(TimestampControlMode::EARLY_LATE);
        // Getter check set value
        CHECK(cam.timing_control() == TimestampControlMode::EARLY_LATE);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x40);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.timing_control() == TimestampControlMode::EARLY_LATE);
        // EARLY
        // Setter
        cam.timing_control(TimestampControlMode::EARLY);
        // Getter check set value
        CHECK(cam.timing_control() == TimestampControlMode::EARLY);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x30);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.timing_control() == TimestampControlMode::EARLY);
        // LATE
        // Setter
        cam.timing_control(TimestampControlMode::LATE);
        // Getter check set value
        CHECK(cam.timing_control() == TimestampControlMode::LATE);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x20);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.timing_control() == TimestampControlMode::LATE);
        // DEVICE
        // Setter
        cam.timing_control(TimestampControlMode::DEVICE);
        // Getter check set value
        CHECK(cam.timing_control() == TimestampControlMode::DEVICE);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x10);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.timing_control() == TimestampControlMode::DEVICE);
    }
}

TEST_CASE("Acknowledge ControlAcknowledgeMode", "[command]")
{
    AcknowledgeCAM cam;
    AcknowledgeCAM unpack_cam;
    bytes packed_bytes{ 0xFF, 0xFF, 0xFF, 0xFF };

    SECTION("Zero on construction")
    {
        cam.pack_into(packed_bytes.data());
        CHECK(packed_bytes == bytes{ 0, 0, 0, 0 });
    }

    SECTION("Acknowledge Validation")
    {
        // Verify zero on construction
        CHECK(cam.ack_v() == false);
        // Setter
        cam.ack_v(true);
        // Getter check set value
        CHECK(cam.ack_v() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x10);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.ack_v() == true);
    }

    SECTION("Acknowledge Execution")
    {
        // Verify zero on construction
        CHECK(cam.ack_x() == false);
        // Setter
        cam.ack_x(true);
        // Getter check set value
        CHECK(cam.ack_x() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x08);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.ack_x() == true);
    }

    SECTION("Acknowledge Query-State")
    {
        // Verify zero on construction
        CHECK(cam.ack_s() == false);
        // Setter
        cam.ack_s(true);
        // Getter check set value
        CHECK(cam.ack_s() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x04);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.ack_s() == true);
    }

    SECTION("Acknowledge Warnings")
    {
        // Verify zero on construction
        CHECK(cam.ack_w() == false);
        // Setter
        cam.ack_w(true);
        // Getter check set value
        CHECK(cam.ack_w() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x02);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.ack_w() == true);
    }

    SECTION("Acknowledge Errors")
    {
        // Verify zero on construction
        CHECK(cam.ack_er() == false);
        // Setter
        cam.ack_er(true);
        // Getter check set value
        CHECK(cam.ack_er() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[1] == 0x01);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.ack_er() == true);
    }

    SECTION("Partial Action")
    {
        // Verify zero on construction
        CHECK(cam.partial_action() == false);
        // Setter
        cam.partial_action(true);
        // Getter check set value
        CHECK(cam.partial_action() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x08);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.partial_action() == true);
    }

    SECTION("Partial Action")
    {
        // Verify zero on construction
        CHECK(cam.scheduled_or_executed() == false);
        // Setter
        cam.scheduled_or_executed(true);
        // Getter check set value
        CHECK(cam.scheduled_or_executed() == true);
        // Pack
        cam.pack_into(packed_bytes.data());
        // Verify packed bits
        CHECK(packed_bytes[2] == 0x04);
        // Unpack
        unpack_cam.unpack_from(packed_bytes.data());
        // Verify unpacked value
        CHECK(unpack_cam.scheduled_or_executed() == true);
    }
}