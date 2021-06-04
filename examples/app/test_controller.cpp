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

#include "RDCInformationController.hpp"

void handleTunerInfo(TunerInfo& packet)
{
    std::cout << "Received context packet..." << std::endl;
    if (packet.hasSampleRate()) { 
        std::cout << "New sample rate is: " << packet.getSampleRate() << std::endl;
    }
    if (packet.hasRFReferenceFrequency()) {
        std::cout << "New frequency is: " << packet.getRFReferenceFrequency() << std::endl;
    }
}

void handleSignalData(SignalData& packet)
{
    auto data = packet.getPayload();
    std::cout << "Got data " << packet.getPayloadSize() << " bytes" << std::endl;

    // Verify data
    std::vector<float> ramp(packet.getPayloadSize() / sizeof(float));
    auto got_ramp = true;
    for (size_t i=0; i<ramp.size(); ++i) {
        auto data_val = *(reinterpret_cast<const float*>(data));
        if (data_val != (i + 1)) {
            std::cout << "Ramp failed at index: " << i
                      << " (" << data_val << " vs " << (i+1) << ")" << std::endl;
            got_ramp = false;
            break;
        }
        data += sizeof(float);
    }

    if (got_ramp) {
        std::cout << "Data ramp is valid." << std::endl;
    } else {
        std::cout << "Failed to get data ramp." << std::endl;
    }
}

int main()
{
    std::string ip = "127.0.0.1";
    unsigned short port = 5000;
    vrtgen::socket::endpoint::udp::v4 controllee_endpoint(ip, port);
    vrtgen::socket::endpoint::udp::v4 controller_endpoint(ip, port+1000);

    // Create a client instance
    RDCInformationController controller(controller_endpoint);

    // Point socket to radio endpoint
    controller.setControlleeEndpoint(controllee_endpoint);

    // Make API function calls
    double freq = 50;
    // Command functions
    std::cout << "Setting RF Reference Frequency to " << freq << "..." << std::endl;
    controller.setRFReferenceFrequency(freq);
    
    std::cout << "Getting RF Reference Frequency..." << std::endl;
    auto get_freq = controller.getRFReferenceFrequency();

    // Register data listener
    std::cout << "Registering data and context listeners..." << std::endl;
    controller.registerSignalDataListener(handleSignalData);
    controller.registerTunerInfoListener(handleTunerInfo);

    // Start data receive
    std::cout << "Enabling data stream..." << std::endl;
    controller.enableReceive();
    ::structs::configuretuner::DiscreteIO32 discrete_io_32;
    discrete_io_32.setStreamEnableEnabled(true);
    discrete_io_32.setStreamEnable(true);
    controller.setDiscreteIO32(discrete_io_32);
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    
    std::cout << "Changing frequency to: " << 100 << std::endl;
    controller.setRFReferenceFrequency(100);
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    
    std::cout << "Setting sample rate to: " << 20000 << std::endl;
    controller.setSampleRate(20000.);
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    
    std::cout << "Setting gain to: " << 2 << std::endl;
    vrtgen::packing::Gain gain;
    gain.setStage1(2.);
    controller.setGain(gain);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Stop data receive
    std::cout << "Disabling data stream..." << std::endl;
    controller.disableReceive();
    discrete_io_32.setStreamEnable(false);
    controller.setDiscreteIO32(discrete_io_32);   

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "Done!" << std::endl;

    return 0;
}
