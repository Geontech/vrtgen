/*
 * Copyright (C) 2023 Geon Technologies, LLC
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

#include <iostream>
#include <chrono>

#include "Controller.hpp"

using namespace example::nats::packets;

std::string to_string(const vrtgen::packing::WarningErrorFields& warn_err, bool verbose=false)
{
    std::ostringstream oss;
    oss << std::boolalpha;
    if (warn_err.field_not_executed() || verbose) {
        oss << "  - Field Not Executed : " << warn_err.field_not_executed() << "\n";
    }
    if (warn_err.device_failure() || verbose) {
        oss << "  - Device Failure : " << warn_err.device_failure() << "\n";
    }
    if (warn_err.erroneous_field() || verbose) {
        oss << "  - Erroneous Field : " << warn_err.erroneous_field() << "\n";
    }
    if (warn_err.parameter_out_of_range() || verbose) {
        oss << "  - Parameter Out-of-Range : " << warn_err.parameter_out_of_range() << "\n";
    }
    if (warn_err.parameter_unsupported_precision() || verbose) {
        oss << "  - Parameter Unsupported Precision : " << warn_err.parameter_unsupported_precision() << "\n";
    }
    if (warn_err.field_value_invalid() || verbose) {
        oss << "  - Field Value Invalid : " << warn_err.field_value_invalid() << "\n";
    }
    if (warn_err.timestamp_problem() || verbose) {
        oss << "  - Timestamp Problem : " << warn_err.timestamp_problem() << "\n";
    }
    if (warn_err.hazardous_power_levels() || verbose) {
        oss << "  - Hazardous Power Levels : " << warn_err.hazardous_power_levels() << "\n";
    }
    return oss.str();
}

void handle_example_context(ExampleContext& packet)
{
    std::cout << "----- Received context packet -----" << std::endl;
    if (packet.bandwidth().has_value()) { 
        std::cout << "  Context bandwidth is: " << packet.bandwidth().value() << std::endl;
    }
    if (packet.rf_ref_frequency().has_value()) {
        std::cout << "  Context frequency is: " << packet.rf_ref_frequency().value() << std::endl;
    }
    if (packet.sample_rate().has_value()) { 
        std::cout << "  Context sample rate is: " << packet.sample_rate().value() << std::endl;
    }
    if (packet.gain().has_value()) {
        std::cout << "  Context gain is: " << packet.gain().value().stage_1() << std::endl;
    }
    std::cout << "-----------------------------------\n" << std::endl;
}

void handle_signal_data(SignalData& packet)
{
    std::cout << "----- Received data packet -----" << std::endl;
    if (packet.trailer().valid_data().has_value()) {
        std::cout << "  Trailer Valid Data: " << packet.trailer().valid_data().value() << std::endl;
    }
    auto data = packet.payload().data();
    std::cout << "  Got " << packet.payload_size() << " bytes" << std::endl;

    // Verify data
    std::vector<uint16_t> ramp(packet.payload_size() / sizeof(float));
    auto got_ramp = true;
    for (size_t i=0; i<ramp.size(); ++i) {
        auto data_val = *(reinterpret_cast<const uint16_t*>(data));
        if (data_val != (i + 1)) {
            std::cout << "  Ramp failed at index: " << i
                      << " (" << data_val << " vs " << i/*(i+1)*/ << ")" << std::endl;
            got_ramp = false;
            break;
        }
        data += sizeof(uint16_t);
    }

    if (got_ramp) {
        std::cout << "  Data ramp is valid." << std::endl;
    } else {
        std::cout << "  Failed to get data ramp." << std::endl;
    }
    std::cout << "--------------------------------\n" << std::endl;
}

int main()
{
    try {
        std::string ip = "127.0.0.1";
        unsigned short controller_data_ctxt_port = 5002;

        // Create a client instance
        example::nats::Controller controller(NATS_DEFAULT_URL);

        // Point socket to radio endpoint
        controller.controllee_endpoint("example_controllee");
        std::cout << "Using controllee at " << controller.controllee_endpoint() << std::endl;

        // Setup data/context recv socket
        controller.data_ctxt_src_endpoint({ ip, controller_data_ctxt_port });
        std::cout << "Receiving data and context packets at " << controller.data_ctxt_src_endpoint().to_string() << std::endl;

        // Send control packets
        std::cout << ">>> Sending control packet..." << std::endl;
        ExampleControl ctrl_packet;
        ctrl_packet.req_x(true);
        ctrl_packet.stream_id(0x1234);
        ctrl_packet.rf_ref_frequency(100e6);
        ctrl_packet.bandwidth(1e6);
        ctrl_packet.sample_rate(2e6);
        if (auto [ackx] = controller.send_example_control(ctrl_packet); ackx.has_value()) {
            std::cout << ">>> Received execution acknowledgement packet..." << std::endl;
            if (auto bw_warn = ackx->bandwidth_warnings()) {
                std::cout << " - Bandwidth field reported warnings:" << std::endl;
                std::cout << to_string(bw_warn.value());
            }
        }

        // Register data listener
        std::cout << ">>> Registering data and context listeners..." << std::endl;
        controller.register_signal_data_listener(handle_signal_data);
        controller.register_example_context_listener(handle_example_context);

        // Start data receive
        std::cout << ">>> Enabling data stream..." << std::endl;
        controller.enable_receive();
        example_control::structs::DiscreteIO32 discrete_io_32;
        discrete_io_32.stream_enable_enable(true);
        discrete_io_32.stream_enable(true);
        ctrl_packet.discrete_io_32(discrete_io_32);
        if (auto [ackx] = controller.send_example_control(ctrl_packet); ackx.has_value()) {
            std::cout << ">>> Received execution acknowledgement packet..." << std::endl;
            if (auto bw_warn = ackx->bandwidth_warnings()) {
                std::cout << " - Bandwidth field reported warnings:" << std::endl;
                std::cout << to_string(bw_warn.value());
            }
        }
        ctrl_packet.reset_discrete_io_32();
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        
        std::cout << ">>> Changing frequency to: 200e6" << std::endl;
        ctrl_packet.rf_ref_frequency(200e6);
        if (auto [ackx] = controller.send_example_control(ctrl_packet); ackx.has_value()) {
            std::cout << ">>> Received execution acknowledgement packet..." << std::endl;
            if (auto bw_warn = ackx->bandwidth_warnings()) {
                std::cout << " - Bandwidth field reported warnings:" << std::endl;
                std::cout << to_string(bw_warn.value());
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        std::cout << ">>> Setting sample rate to: 4e6" << std::endl;
        ctrl_packet.sample_rate(4e6);
        if (auto [ackx] = controller.send_example_control(ctrl_packet); ackx.has_value()) {
            std::cout << ">>> Received execution acknowledgement packet..." << std::endl;
            if (auto bw_warn = ackx->bandwidth_warnings()) {
                std::cout << " - Bandwidth field reported warnings:" << std::endl;
                std::cout << to_string(bw_warn.value());
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        std::cout << ">>> Setting gain to: 2" << std::endl;
        vrtgen::packing::Gain gain;
        gain.stage_1(2.);
        ctrl_packet.gain() = gain;
        if (auto [ackx] = controller.send_example_control(ctrl_packet); ackx.has_value()) {
            std::cout << ">>> Received execution acknowledgement packet..." << std::endl;
            if (auto bw_warn = ackx->bandwidth_warnings()) {
                std::cout << " - Bandwidth field reported warnings:" << std::endl;
                std::cout << to_string(bw_warn.value());
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << ">>> Setting invalid bandwidth to: 200000" << std::endl;
        ctrl_packet.bandwidth(2e6);
        if (auto [ackx] = controller.send_example_control(ctrl_packet); ackx.has_value()) {
            std::cout << ">>> Received execution acknowledgement packet..." << std::endl;
            if (auto bw_warn = ackx->bandwidth_warnings()) {
                std::cout << " - Bandwidth field reported warnings:" << std::endl;
                std::cout << to_string(bw_warn.value());
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // Stop data receive
        std::cout << ">>> Disabling data stream..." << std::endl;
        controller.disable_receive();
        discrete_io_32.stream_enable(false);
        ctrl_packet.discrete_io_32() = discrete_io_32;
        if (auto [ackx] = controller.send_example_control(ctrl_packet); ackx.has_value()) {
            std::cout << ">>> Received acknowledgement packet..." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::cout << "Done!" << std::endl;
    } catch(const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return ~0;
    }

    return 0;
}
