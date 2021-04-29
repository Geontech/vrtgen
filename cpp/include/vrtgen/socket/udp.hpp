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
#include "socket_endpoint.hpp"

namespace vrtgen {
namespace socket {

/**
 * @class datagram_socket
 * @brief UDP socket class
 * 
 * Socket class for datagram network communications.
 */
template <int domain_type>
class datagram_socket : public socket_base
{
public:
    using endpoint_type = socket_endpoint<domain_type, SOCK_DGRAM>;

    /**
     * @brief Constructor
     * @throw std::runtime_error Failed to create socket
     */
    datagram_socket() :
        socket_base(domain_type, SOCK_DGRAM)
    {
    }

    /**
     * @brief Destructor
     */
    virtual ~datagram_socket() = default;

    /**
     * @brief Set the socket destination endpoint
     * @param dst Desired destination endpoint
     */
    void dst(const endpoint_type& dst)
    {
        m_dst = dst;
    }

    /**
     * @brief Get the socket destination endpoint
     * @return Reference to the destination endpoint
     */
    endpoint_type& dst() noexcept
    {
        return m_dst;
    }

    /**
     * @brief Get the socket destination endpoint
     * @return Constant reference to the destination endpoint
     */
    const endpoint_type& dst() const noexcept
    {
        return m_dst;
    }

    /**
     * @brief Set the socket source endpoint
     * @param src Desired source endpoint
     */
    void src(const endpoint_type& src)
    {
        m_src = src;
    }
    
    /**
     * @brief Get the socket source endpoint
     * @return Reference to the source endpoint
     */
    endpoint_type& src() noexcept
    {
        return m_src;
    }

    /**
     * @brief Get the socket source endpoint
     * @return Constant reference to the source endpoint
     */
    const endpoint_type& src() const noexcept
    {
        return m_src;
    }

    /**
     * @brief Bind the socket to an endpoint.
     *        This function also sets the socket's source endpoint to [in] endpoint.
     * @param endpoint Desired endpoint to bind socket
     * @return true on successful bind, otherwise false
     */
    bool bind(const endpoint_type& endpoint)
    {
        auto res = ::bind(m_socket, (const sockaddr*)&endpoint.sockaddr(), endpoint.socklen());
        if (res < 0) {
            return false;
        }
        m_src = endpoint;
        return true;
    }

    /**
     * @brief Send a message on the socket
     * @param data Pointer to start of message data
     * @param len Size of message
     * @param endpoint Destination endpoint for the message
     * @return Number of characters sent on success, otherwise -1 for error
     */
    ssize_t send_to(const void* data, const size_t len, const endpoint_type& endpoint)
    {
        return sendto(m_socket, data, len, 0, (sockaddr*)&endpoint.sockaddr(), endpoint.socklen());
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
        return recvfrom(m_socket, data, len, 0, (sockaddr*)&endpoint.sockaddr(), &endpoint.socklen());
    }

private:
    endpoint_type m_src; /** Socket's source endpoint */
    endpoint_type m_dst; /** Socket's destination endpoint */

}; // end class udp

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
