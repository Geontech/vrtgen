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

#ifndef VRTGEN_SOCKET_TCP_HPP
#define VRTGEN_SOCKET_TCP_HPP

#include <iostream>

#include "socket_base.hpp"

namespace vrtgen {
namespace socket {

/**
 * @class stream_socket
 * @brief TCP socket class
 * 
 * Socket class for stream network communications.
 */
template <int domain>
class stream_socket : public socket_base<domain, SOCK_STREAM>
{
    using base_type = socket_base<domain, SOCK_STREAM>;

public:
    using endpoint_type = typename base_type::endpoint_type;

    /**
     * @brief Constructor
     * @throw std::runtime_error Failed to create socket
     */
    explicit stream_socket() :
        base_type()
    {
    }

    /**
     * @brief Destructor
     */
    virtual ~stream_socket() = default;

    /**
     * @brief Listen for connections on the socket
     * @param backlog Maximum number of pending connections allowed
     * @return true on successful connection, otherwise false
     */
    bool listen(const int backlog=SOMAXCONN)
    {
        auto res = ::listen(this->m_socket, backlog);
        if (res < 0) {
            return false;
        }
        return true;
    }

    /**
     * @brief Accept a connection on the socket
     * @param endpoint Destination endpoint to connect to socket
     * @return true on successful connection, otherwise false
     */
    bool accept()
    {
        endpoint_type endpoint;
        auto res = ::accept(this->m_socket, (sockaddr*)&endpoint.sockaddr(), &endpoint.socklen());
        if (res < 0) {
            return false;
        }
        m_connected_socket = res;
        this->m_dst = endpoint;
        return true;
    }

    /**
     * @brief Write data to the socket
     * @param data Pointer to start of message data
     * @param len Size of message
     * @return Number of bytes written on success, otherwise -1 for error
     */
    ssize_t write_some(const void* data, const size_t len)
    {
        if (m_is_connected()) {
            return write(m_connected_socket, data, len);
        }
        return write(this->m_socket, data, len);
    }

    /**
     * @brief Read data off the socket
     * @param data Pointer to start of buffer where received message data will be written
     * @param len Size of data buffer
     * @return Number of bytes read on success, otherwise -1 for error
     */
    ssize_t read_some(void* data, const size_t len)
    {
        if (m_is_connected()) {
            return read(m_connected_socket, data, len);
        }
        return read(this->m_socket, data, len);
    }

private:
    int m_connected_socket = this->INVALID_SOCKET;

    bool m_is_connected() const noexcept
    {
        return m_connected_socket != this->INVALID_SOCKET;
    }

}; // end class stream_socket

namespace tcp {

/**
 * @typedef TCP IPv4 socket
 */
using v4 = socket::stream_socket<AF_INET>;
/**
 * @typedef TCP IPv6 socket
 */
using v6 = socket::stream_socket<AF_INET6>;

} // end namespace tcp

} // end namespace socket
} // end namespace vrt

# endif // VRTGEN_SOCKET_TCP_HPP
