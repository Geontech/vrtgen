#include <iostream>

#include "RDCInformationControllee.hpp"

int main()
{
    // Create a controllee instance
    RDCInformationControllee controllee;

    // BIND THIS SERVER FOR INCOMING VRT MESSAGES
    std::string ip = "127.0.0.1";
    unsigned short port = 5000;
    vrtgen::socket::endpoint::udp::v4 listen_endpoint(ip, port);
    if (!controllee.bind(listen_endpoint)) {
        std::cerr << "Failed to bind controllee socket" << std::endl;
        return ~0;
    }
    std::cout << "Successfully bound controllee socket to: " << listen_endpoint.to_string() << std::endl;

    // LISTEN
    controllee.vrt_listen();

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
