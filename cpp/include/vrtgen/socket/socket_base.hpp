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

#ifndef VRTGEN_SOCKET_SOCKET_BASE_HPP
#define VRTGEN_SOCKET_SOCKET_BASE_HPP

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>

namespace vrtgen {
namespace socket {

/**
 * @class socket_base
 * @brief Socket base class
 * 
 * Base class for socket communications.
 */
class socket_base
{
    static constexpr auto INVALID_SOCKET = -1;
    static constexpr auto STANDARD_PROTOCOL = 0;

public:
    /**
     * @brief Destructor.
     *        Closes the socket.
     */
    virtual ~socket_base()
    {
        close();
    }

    /**
     * @brief Close the socket
     */
    void close()
    {
        ::close(m_socket);
    }

    /**
     * @brief Determine whether the socket is open
     * @return true if open, otherwise false
     */
    bool is_open() const noexcept
    {
        return m_socket != INVALID_SOCKET;
    }

    /**
     * @brief Disables sends or receives on the socket
     * @param what The operations that will no longer be allowed
     * @throw std::runtime_error Thrown on failure
     */
    void shutdown(const int what)
    {
        ::shutdown(m_socket, what);
    }

protected:
    /**
     * @brief Constructor
     * @param domain Communication domain
     * @param type Type of socket to be created
     * @param protocol Protocol to be used; default 0 is standard protocol
     * @throw std::runtime_error Failed to create socket
     */
    socket_base(const int domain, const int type, const int protocol=0) :
        m_domain(domain), m_type(type), m_protocol(protocol)
    {
        if ((m_socket = ::socket(m_domain, m_type, m_protocol)) < 0) {
            throw std::runtime_error("Failed to create socket descriptor");
        }
    }
    
    int m_socket{ INVALID_SOCKET }; /**< Socket file descriptor */
    int m_domain; /**< Communication domain */
    int m_type; /**< Socket type */
    int m_protocol{ STANDARD_PROTOCOL }; /**< Socket protocol */
    
}; // end class socket_base

} // end namespace socket
} // end namsepace vrtgen

#endif // VRTGEN_SOCKET_SOCKET_BASE_HPP