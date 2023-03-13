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
#include <atomic>
#include <csignal>

#include "Controllee.hpp"

using namespace example::nats::controllee;

namespace
{
    std::atomic_bool running{ true };
}
 
void signal_handler(int signal)
{
    running = false;
}

int main()
{
    std::signal(SIGINT, signal_handler);

    // Create a controllee instance
    Controllee controllee{ NATS_DEFAULT_URL, "example_controllee" };
    std::cout << "Successfully connected controllee to: " << NATS_DEFAULT_URL << std::endl;

    // Listen
    controllee.vrt_listen();

    // Wait for user to stop the program
    while (running) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }

    return 0;
}
