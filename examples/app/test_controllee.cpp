#include <iostream>

#include "RDCInformationControllee.hpp"

int main()
{
    // Create a controllee instance
    RDCInformationControllee controllee;

    // Bind this server for incoming vrt messages
    std::string ip = "127.0.0.1";
    unsigned short port = 5000;
    vrtgen::socket::endpoint::udp::v4 listen_endpoint(ip, port);
    if (!controllee.bind(listen_endpoint)) {
        std::cerr << "Failed to bind controllee socket" << std::endl;
        return ~0;
    }
    std::cout << "Successfully bound controllee socket to: " << listen_endpoint.to_string() << std::endl;

    // Listen
    controllee.vrt_listen();

    // Wait for user to stop the program
    std::cout << "Press ENTER to terminate the program" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    return 0;
}
