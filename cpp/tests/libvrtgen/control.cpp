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

#include "catch/catch.hpp"

#include "vrtgen/enums.hpp"
#include "vrtgen/packing/control.hpp"

#include "bytes.hpp"

TEST_CASE("ControlAcknowledgeMode setters", "[command]")
{
    using vrtgen::packing::ControlAcknowledgeMode;
    bytes data = {0, 0, 0, 0};
    ControlAcknowledgeMode& cam = *reinterpret_cast<ControlAcknowledgeMode*>(data.data());

    SECTION("Controllee Enable") {
        // Set a bit pattern that allows checking that the correct bits are
        // cleared
        data[0] = 0xFF;
        cam.setControlleeEnabled(false);
        CHECK(data == bytes{0x7F, 0x00, 0x00, 0x00});
    }

    SECTION("Controllee Identifier Format") {
        cam.setControlleeIdentifierFormat(vrtgen::IdentifierFormat::UUID);
        CHECK(data == bytes{0x40, 0x00, 0x00, 0x00});    
    }

    SECTION("Controller Enabled") {
        cam.setControllerEnabled(true);
        CHECK(data == bytes{0x20, 0x00, 0x00, 0x00});
    }

    SECTION("Controller Identifier Format") {
        // Set a bit pattern that allows checking that the correct bits are
        // cleared
        data[0] = 0xFF;
        cam.setControllerIdentifierFormat(vrtgen::IdentifierFormat::WORD);
        CHECK(data == bytes{0xEF, 0x00, 0x00, 0x00});
    }

    SECTION("Action") {
        cam.setActionMode(vrtgen::ActionMode::EXECUTE);
        CHECK(data == bytes{0x01, 0x00, 0x00, 0x00});
        cam.setActionMode(vrtgen::ActionMode::DRY_RUN);
        CHECK(data == bytes{0x00, 0x80, 0x00, 0x00});
    }

    SECTION("Timing Control Acknowledgement") {
        cam.setTimingControlAcknowledgement(vrtgen::TimestampControlMode::EARLY);
        CHECK(data == bytes{0x00, 0x00, 0x30, 0x00});
        cam.setTimingControlAcknowledgement(vrtgen::TimestampControlMode::EARLY_LATE);
        CHECK(data == bytes{0x00, 0x00, 0x40, 0x00});
    }
}

TEST_CASE("ControlAcknowledgeMode getters", "[command]")
{
    using vrtgen::packing::ControlAcknowledgeMode;
    bytes data = {0, 0, 0, 0};
    ControlAcknowledgeMode& cam = *reinterpret_cast<ControlAcknowledgeMode*>(data.data());

    SECTION("Controllee Enable") {
        data[0] = 0x80;
        CHECK(cam.isControlleeEnabled());
    }

    SECTION("Controllee Identifier Format") {
        data = {0xBF, 0xFF, 0xFF, 0xFF};
        CHECK(cam.getControlleeIdentifierFormat() == vrtgen::IdentifierFormat::WORD);
    }

    SECTION("Controller Enabled") {
        data = {0xDF, 0xFF, 0xFF, 0xFF};
        CHECK_FALSE(cam.isControllerEnabled());
    }

    SECTION("Controller Identifier Format") {
        // Set a bit pattern that allows checking that the correct bits are
        // cleared
        data[0] = 0x10;
        CHECK(cam.getControllerIdentifierFormat() == vrtgen::IdentifierFormat::UUID);
    }

    SECTION("Action") {
        data[0] = 0x01;
        CHECK(cam.getActionMode() == vrtgen::ActionMode::EXECUTE);
        data = {0xFE, 0x7F, 0xFF, 0xFF};
        CHECK(cam.getActionMode() == vrtgen::ActionMode::NO_ACTION);
    }

    SECTION("Timing Control Acknowledgement") {
        data[2] = 0x10;
        CHECK(cam.getTimingControlAcknowledgement() == vrtgen::TimestampControlMode::DEVICE);
        data = {0xFF, 0xFF, 0x8F, 0xFF};
        CHECK(cam.getTimingControlAcknowledgement() == vrtgen::TimestampControlMode::IGNORE);
    }
}
