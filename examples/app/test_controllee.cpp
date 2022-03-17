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

#include "ExampleInfoControllee.hpp"

int main()
{
    std::string ip = "127.0.0.1";
    unsigned short port = 5000;

    // Create a controllee instance
    ExampleInfoControllee controllee({ ip, port });
    std::cout << "Successfully bound controllee socket to: " << controllee.cmd_socket().src().to_string() << std::endl;

    // Enable TCP accept
    if (!controllee.cmd_socket().listen()) {
        std::cout << "Socket failed to listen. Exiting..." << std::endl;
        return 1;
    }
    std::cout << "Listening for connections..." << std::endl;
    controllee.cmd_socket().timeout(10);
    if (!controllee.cmd_socket().accept()) {
        std::cout << "Socket failed to accept incoming connections. Exiting..." << std::endl;
        return 1;
    }
    std::cout << "Accepted incoming connection..." << std::endl;

    // Listen
    controllee.vrt_listen();

    // Wait for user to stop the program
    std::cout << "Press ENTER to terminate the program" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    return 0;
}
