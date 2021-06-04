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
