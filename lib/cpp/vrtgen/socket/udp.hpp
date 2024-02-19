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

#ifndef VRTGEN_SOCKET_UDP_HPP
#define VRTGEN_SOCKET_UDP_HPP

#include <iostream>

#include "socket_base.hpp"

namespace vrtgen {
namespace socket {

/**
 * @class datagram_socket
 * @brief UDP socket class
 * 
 * Socket class for datagram network communications.
 */
template <int domain>
class datagram_socket : public socket_base<domain, SOCK_DGRAM>
{
    using base_type = socket_base<domain, SOCK_DGRAM>;

public:
    using endpoint_type = typename base_type::endpoint_type;

    /**
     * @brief Constructor
     * @throw std::runtime_error Failed to create socket
     */
    explicit datagram_socket() :
        base_type()
    {
    }

    /**
     * @brief Destructor
     */
    virtual ~datagram_socket() = default;

    /**
     * @brief Send a message on the socket
     * @param data Pointer to start of message data
     * @param len Size of message
     * @param endpoint Destination endpoint for the message
     * @return Number of characters sent on success, otherwise -1 for error
     */
    ssize_t send_to(const void* data, const size_t len, const endpoint_type& endpoint)
    {
        return sendto(this->m_socket, data, len, 0, (sockaddr*)&endpoint.sockaddr(), endpoint.socklen());
    }

    /**
     * @brief Receive a message on the socket
     * @param data Pointer to start of buffer where received message data will be written
     * @param len Size of data buffer
     * @param endpoint Endpoint to be populated with source endpoint information
     * @return Number of bytes received on success, otherwise -1 for error
     */
    ssize_t receive_from(void* data, const size_t len, endpoint_type& endpoint)
    {
        return recvfrom(this->m_socket, data, len, 0, (sockaddr*)&endpoint.sockaddr(), &endpoint.socklen());
    }
    
}; // end class datagram_socket

namespace udp {

/**
 * @typedef UDP IPv4 socket
 */
using v4 = socket::datagram_socket<AF_INET>;
/**
 * @typedef UDP IPv6 socket
 */
using v6 = socket::datagram_socket<AF_INET6>;

} // end namespace udp

} // end namespace socket
} // end namespace vrt

# endif // VRTGEN_SOCKET_UDP_HPP
