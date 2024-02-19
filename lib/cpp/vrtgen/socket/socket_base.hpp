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
#include <sys/time.h>
#include <sstream>

#include "socket_endpoint.hpp"

namespace vrtgen::socket {

/**
 * @class socket_base
 * @brief Socket base class
 * 
 * Base class for socket communications.
 */
template <int domain_type, int socket_type>
class socket_base
{
public:
    using endpoint_type = socket_endpoint<domain_type, socket_type>;

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
     * @brief Bind the socket to an endpoint.
     *        This function also sets the socket's source endpoint to [in] endpoint.
     * @param endpoint Desired endpoint to bind socket
     * @return true on successful bind, otherwise false
     */
    bool bind(const endpoint_type& endpoint)
    {
        auto res = ::bind(m_socket, (const sockaddr*)&endpoint.sockaddr(), endpoint.socklen());
        if (res < 0) {
            auto err_val = strerror(errno);
            std::ostringstream msg;
            msg << "Failed to bind to: " << endpoint.to_string()
                << " with errno: " << err_val;
            std::cout << msg.str() << std::endl;

            return false;
        }
        m_src = endpoint;
        return true;
    }

    /**
     * @brief Initiate a connection on a socket
     * @param endpoint Destination endpoint to connect to socket
     * @return true on successful connection, otherwise false
     */
    bool connect(const endpoint_type& endpoint)
    {
        auto res = ::connect(m_socket, (const sockaddr*)&endpoint.sockaddr(), endpoint.socklen());
        if (res < 0) {
            auto err_val = strerror(errno);
            std::ostringstream msg;
            msg << "Failed to connect to: " << endpoint.to_string()
                << " with errno: " << err_val;
            std::cout << msg.str() << std::endl;

            return false;
        }
        m_dst = endpoint;
        return true;
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

    /**
     * @brief Get the native socket representation
     * @return The native socket file descriptor
     */
    int native_handle() const noexcept
    {
        return m_socket;
    }

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
     * @brief Set receive timeout for socket
     * @param timeout The desired socket receive timeout (in seconds)
     */
    void timeout(const size_t timeout)
    {
        struct timeval tv;
        tv.tv_sec = timeout;
        tv.tv_usec = 0;
        setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
    }

protected:
    static constexpr int INVALID_SOCKET = -1;
    static constexpr int STANDARD_PROTOCOL = 0;
    static constexpr auto TIMEOUT = 3; /**< Default socket timeout in seconds */

    /**
     * @brief Constructor
     * @param domain Communication domain
     * @param type Type of socket to be created
     * @param protocol Protocol to be used; default 0 is standard protocol
     * @throw std::runtime_error Failed to create socket
     */
    socket_base() :
        m_domain(domain_type),
        m_type(socket_type),
        m_protocol(STANDARD_PROTOCOL)
    {
        if ((m_socket = ::socket(m_domain, m_type, m_protocol)) < 0) {
            throw std::runtime_error(std::string("Failed to create socket descriptor: ") + strerror(errno));
        }
        this->timeout(TIMEOUT);
    }
    
    int m_socket{ INVALID_SOCKET }; /**< Socket file descriptor */
    int m_domain; /**< Communication domain */
    int m_type; /**< Socket type */
    int m_protocol{ STANDARD_PROTOCOL }; /**< Socket protocol */
    endpoint_type m_src; /** Socket's source endpoint */
    endpoint_type m_dst; /** Socket's destination endpoint */

}; // end class socket_base

} // end namespace vrtgen::socket

#endif // VRTGEN_SOCKET_SOCKET_BASE_HPP