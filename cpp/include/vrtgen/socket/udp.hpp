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

template <int domain_type>
class datagram_socket : public socket_base
{
public:
    using endpoint_type = socket_endpoint<domain_type, SOCK_DGRAM>;

    datagram_socket() : socket_base(domain_type, SOCK_DGRAM)
    {
    }

    virtual ~datagram_socket() = default;

    void endpoint(const endpoint_type& endpoint)
    {
        m_dest_endpoint = endpoint;
    }

    endpoint_type& endpoint() noexcept
    {
        return m_dest_endpoint;
    }

    const endpoint_type& endpoint() const noexcept
    {
        return m_dest_endpoint;
    }
    
    endpoint_type& src() noexcept
    {
        return m_src_endpoint;
    }

    const endpoint_type& src() const noexcept
    {
        return m_src_endpoint;
    }

    bool bind(const endpoint_type& endpoint)
    {
        auto res = ::bind(m_socket, (const sockaddr*)&endpoint.sockaddr(), endpoint.socklen());
        if (res < 0) {
            return false;
        }
        m_src_endpoint = endpoint;
        return true;
    }

    /**
     * @brief Connect the socket to the endpoint
     * @param endpoint The remote endpoint to which the socket will be connected
     * @return true on success, otherwise false
     */
    bool connect(const endpoint_type& endpoint)
    {
        m_connected = false;
        if (::connect(m_socket, (const sockaddr*)&endpoint.sockaddr(), endpoint.socklen()) < 0) {
            return m_connected;
        }
        return m_connected;
    }

    void send(const void* data, const std::size_t len)
    {
        if (m_connected) {
            write(m_socket, data, len);
        } else {
            std::cerr << "send() may only be used when the socket is connected" << std::endl;
        }
    }

    void send_to(const void* data, const std::size_t len)
    {
        sendto(m_socket, data, len, 0, nullptr, 0);
    }

    void send_to(const void* data, const std::size_t len, const endpoint_type& endpoint)
    {
        sendto(m_socket, data, len, 0, (sockaddr*)&endpoint.sockaddr(), endpoint.socklen());
    }

    std::size_t receive(void* data, const std::size_t len)
    {
        return read(m_socket, data, len);
    }

    std::size_t receive_from(void* data, const std::size_t len)
    {
        return recvfrom(m_socket, data, len, 0, nullptr, nullptr);
    }

    std::size_t receive_from(void* data, const std::size_t len, endpoint_type& endpoint)
    {
        return recvfrom(m_socket, data, len, 0, (sockaddr*)&endpoint.sockaddr(), &endpoint.socklen());
    }

private:
    endpoint_type m_src_endpoint;
    endpoint_type m_dest_endpoint;

}; // end class udp

namespace udp {

using v4 = socket::datagram_socket<AF_INET>;
using v6 = socket::datagram_socket<AF_INET6>;

} // end namespace udp

} // end namespace socket
} // end namespace vrt

# endif // VRTGEN_SOCKET_UDP_HPP
