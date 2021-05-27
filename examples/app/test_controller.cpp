#include <iostream>
#include <chrono>

#include "RDCInformationController.hpp"

double sample_rate = 10000.;

void handleTunerInfo(TunerInfo& packet)
{
    if (packet.hasSampleRate()) { 
        sample_rate = packet.getSampleRate();
    }
}

void handleSignalData(SignalData& packet)
{
    auto data = packet.getRawData();
    std::vector<float> t(packet.getRawDataSize() / sizeof(float));
    std::vector<float> x(t.size());

    for (unsigned i=0; i<x.size(); ++i) {
        t[i] = i/sample_rate;
        x[i] = *(reinterpret_cast<const float*>(data));
        data += sizeof(float);
    }

    // plot data
}

int main()
{
    std::string ip = "127.0.0.1";
    unsigned short port = 5000;
    vrtgen::socket::endpoint::udp::v4 server_endpoint(ip, port);
    vrtgen::socket::endpoint::udp::v4 my_endpoint(ip, port+1000);

    // Create a client instance
    RDCInformationController controller(my_endpoint);

    // Point socket to radio endpoint
    controller.setControlleeEndpoint(server_endpoint);

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
