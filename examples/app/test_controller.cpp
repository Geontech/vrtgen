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

#include <iostream>
#include <chrono>

#include "ExampleInfoController.hpp"

void handle_example_context(ExampleContext& packet)
{
    std::cout << "----- Received context packet -----" << std::endl;
    if (packet.has_bandwidth()) { 
        std::cout << "  Context bandwidth is: " << packet.bandwidth() << std::endl;
    }
    if (packet.has_rf_ref_frequency()) {
        std::cout << "  Context frequency is: " << packet.rf_ref_frequency() << std::endl;
    }
    if (packet.has_sample_rate()) { 
        std::cout << "  Context sample rate is: " << packet.sample_rate() << std::endl;
    }
    if (packet.has_gain()) {
        std::cout << "  Context gain is: " << packet.gain().stage_1() << std::endl;
    }
    std::cout << "-----------------------------------\n" << std::endl;
}

void handle_signal_data(SignalData& packet)
{
    std::cout << "----- Received data packet -----" << std::endl;
    std::cout << "  Trailer Valid Data: " << packet.valid_data() << std::endl;
    auto data = packet.payload();
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
        unsigned short controllee_cmd_port = 5000;
        unsigned short controller_cmd_port = 5001;
        unsigned short controller_data_ctxt_port = 5002;

        // Create a client instance
        ExampleInfoController controller({ ip, controller_cmd_port });

        // Point socket to radio endpoint
        controller.controllee_endpoint({ ip, controllee_cmd_port });

        // Setup data/context recv socket
        controller.data_ctxt_src_endpoint({ ip, controller_data_ctxt_port });

        // Send control packets
        std::cout << ">>> Sending control packet..." << std::endl;
        ExampleControl ctrl_packet;
        ctrl_packet.stream_id(0x1234);
        ctrl_packet.rf_ref_frequency(100e6);
        ctrl_packet.bandwidth(1e6);
        ctrl_packet.sample_rate(2e6);
        auto ackx = controller.send_example_control(ctrl_packet);
        std::cout << ">>> Received acknowledgement packet..." << std::endl;

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
        ackx = controller.send_example_control(ctrl_packet);
        std::cout << ">>> Received acknowledgement packet..." << std::endl;
        ctrl_packet.reset_discrete_io_32();
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        
        std::cout << ">>> Changing frequency to: 200e6" << std::endl;
        ctrl_packet.rf_ref_frequency(200e6);
        ackx = controller.send_example_control(ctrl_packet);
        std::cout << ">>> Received acknowledgement packet..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        std::cout << ">>> Setting sample rate to: 4e6" << std::endl;
        ctrl_packet.sample_rate(4e6);
        ackx = controller.send_example_control(ctrl_packet);
        std::cout << ">>> Received acknowledgement packet..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        std::cout << ">>> Setting gain to: 2" << std::endl;
        vrtgen::packing::Gain gain;
        gain.stage_1(2.);
        ctrl_packet.gain(gain);
        ackx = controller.send_example_control(ctrl_packet);
        std::cout << ">>> Received acknowledgement packet..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Stop data receive
        std::cout << ">>> Disabling data stream..." << std::endl;
        controller.disable_receive();
        discrete_io_32.stream_enable(false);
        ctrl_packet.discrete_io_32(discrete_io_32);   
        ackx = controller.send_example_control(ctrl_packet);
        std::cout << ">>> Received acknowledgement packet..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::cout << "Done!" << std::endl;
    } catch(const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return ~0;
    }

    return 0;
}
