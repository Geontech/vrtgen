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

#include "catch/catch.hpp"
#include "vrtgen/socket/udp.hpp"

TEST_CASE("UDP v4 Unicast", "[socket]")
{
    using socket_type = vrtgen::socket::udp::v4;
    using endpoint_type = typename socket_type::endpoint_type;

    SECTION("Address") {
        endpoint_type endpoint;
        CHECK(endpoint.address().s_addr == INADDR_ANY);
        endpoint.address("18.52.86.120"); // = 0x12345678
        CHECK(endpoint.address().s_addr == 0x78563412); // network order
        endpoint.address("120.86.52.18"); // = 0x78563412
        CHECK(endpoint.address().s_addr == 0x12345678); // network order
    }

    SECTION("Port") {
        endpoint_type endpoint;
        CHECK(endpoint.port() == 0);
        endpoint.port(1234);
        CHECK(endpoint.port() == 1234);
        endpoint.port(4321);
        CHECK(endpoint.port() == 4321);
    }

    SECTION("Endpoint To String") {
        endpoint_type endpoint("127.0.0.1", 1234);
        CHECK(endpoint.to_string().compare("127.0.0.1:1234") == 0);

        endpoint.address("10.11.12.13");
        CHECK(endpoint.to_string().compare("10.11.12.13:1234") == 0);

        endpoint.port(5678);
        CHECK(endpoint.to_string().compare("10.11.12.13:5678") == 0);
    }

    SECTION("datagram_socket") {
        socket_type socket;
        CHECK(socket.src().address().s_addr == INADDR_ANY);
        CHECK(socket.src().port() == 0);
        CHECK(socket.src().to_string().compare("0.0.0.0:0") == 0);
        CHECK(socket.dst().address().s_addr == INADDR_ANY);
        CHECK(socket.dst().port() == 0);
        CHECK(socket.dst().to_string().compare("0.0.0.0:0") == 0);

        endpoint_type src_endpoint("127.0.0.1", 1234); 
        CHECK(socket.bind(src_endpoint));
        CHECK(socket.src().address().s_addr == 0x0100007f);
        CHECK(socket.src().port() == 1234);
        CHECK(socket.src().to_string().compare("127.0.0.1:1234") == 0);

        endpoint_type dst_endpoint("127.0.0.1", 5678); 
        socket.dst(dst_endpoint);
        CHECK(socket.dst().address().s_addr == 0x0100007f);
        CHECK(socket.dst().port() == 5678);
        CHECK(socket.dst().to_string().compare("127.0.0.1:5678") == 0);
    }
}