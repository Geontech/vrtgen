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

#ifndef VRTGEN_SOCKET_SOCKET_ENDPOINT_HPP
#define VRTGEN_SOCKET_SOCKET_ENDPOINT_HPP

#include <arpa/inet.h>

#include "socket_address.hpp"

namespace vrtgen {
namespace socket {

template <int domain_type, int socket_type>
class socket_endpoint
{
    using address_type = socket_address<domain_type, socket_type>;

public:
    socket_endpoint() : 
        socket_endpoint("", 0)
    {

    }

    explicit socket_endpoint(const unsigned short port_num) :
        socket_endpoint("", port_num)
    {
    }

    socket_endpoint(const std::string& ip_addr, const unsigned short port_num) :
        m_address(ip_addr), m_port(port_num)
    {
        memset(&m_sockaddr, 0, sizeof(m_sockaddr));
        m_sockaddr.sin_family = domain_type;
        m_sockaddr.sin_port = htons(m_port);
        m_sockaddr.sin_addr.s_addr = m_address.address().s_addr;
        m_socklen = sizeof(m_sockaddr);
    }

    virtual ~socket_endpoint() = default;

    address_type& address() noexcept
    {
        return m_address;
    }

    const address_type& address() const noexcept
    {
        return m_address;
    }

    void address(const std::string& address) noexcept
    {
        m_address = address_type(address);
    }

    unsigned short port() const noexcept
    {
	return ntohs(m_sockaddr.sin_port);
    }

    void port(const unsigned short port) noexcept
    {
        m_sockaddr.sin_port = htons(port);
    }

    sockaddr_in& sockaddr() noexcept
    {
        return m_sockaddr;
    }

    const sockaddr_in& sockaddr() const noexcept
    {
        return m_sockaddr;
    }

    socklen_t& socklen() noexcept
    {
        return m_socklen;
    }

    const socklen_t& socklen() const noexcept
    {
        return m_socklen;
    }

    std::string to_string() const noexcept
    {
        std::string retval;
        retval.resize(address_type::address_len);
        auto addr = inet_ntop(domain_type, &m_sockaddr.sin_addr, &retval.front(), address_type::address_len);
        if (addr != nullptr) {
            retval += std::string(":") + std::to_string(ntohs(m_sockaddr.sin_port));
            return retval;
        }
        return {};
    }

private:
    address_type m_address;
    unsigned short m_port;
    sockaddr_in m_sockaddr;
    socklen_t m_socklen;

}; // end class udp

namespace endpoint {

namespace udp {

using v4 = socket::socket_endpoint<AF_INET, SOCK_DGRAM>;
using v6 = socket::socket_endpoint<AF_INET6, SOCK_DGRAM>;

} // end namepsace udp

namespace tcp {

using v4 = socket::socket_endpoint<AF_INET, SOCK_STREAM>;
using v6 = socket::socket_endpoint<AF_INET6, SOCK_STREAM>;

} // end namespace tcp

} // end namespace endpoint

} // end namespace socket
} // end namespace vrt

# endif // VRT_SOCKETS_SOCKET_ENDPOINT_HPP
