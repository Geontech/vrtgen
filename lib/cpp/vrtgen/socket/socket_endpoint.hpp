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

#include <functional>
#include <algorithm>
#include <arpa/inet.h>

namespace vrtgen {
namespace socket {

namespace endpoint {
namespace detail {

template <int domain>
class endpoint_traits {};

/**
 * @class endpoint_traits<AF_INET>
 * @brief Specialized traits class for IPv4 endpoints
 */
template <>
class endpoint_traits<AF_INET>
{
public:
    using sockaddr_type = sockaddr_in;
    using address_type = in_addr;
    static constexpr auto address_len = INET_ADDRSTRLEN;
    /**
     * @brief Extract the IP address member from the given sockaddr object
     * @param sockaddr The sockaddr object to get IP address from
     * @return Reference to the IP address from the given sockaddr
     */
    static address_type& address(sockaddr_type& sockaddr)
    {
        return sockaddr.sin_addr;
    }
    /**
     * @brief Extract the IP address member from the given sockaddr object
     * @param sockaddr The sockaddr object to get IP address from
     * @return Constant reference to the IP address from the given sockaddr
     */
    static const address_type& address(const sockaddr_type& sockaddr)
    {
        return sockaddr.sin_addr;
    }
}; // end class endpoint_traits<AF_INET6>

/**
 * @class endpoint_traits<AF_INET6>
 * @brief Specialized traits class for IPv6 endpoints
 */
template <>
class endpoint_traits<AF_INET6>
{
public:
    using sockaddr_type = sockaddr_in6;
    using address_type = in6_addr;
    static constexpr auto address_len = INET6_ADDRSTRLEN;
    /**
     * @brief Extract the IP address member from the given sockaddr object
     * @param sockaddr The sockaddr object to get IP address from
     * @return Reference to the IP address from the given sockaddr
     */
    static address_type& address(sockaddr_type& sockaddr)
    {
        return sockaddr.sin6_addr;
    }
    /**
     * @brief Extract the IP address member from the given sockaddr object
     * @param sockaddr The sockaddr object to get IP address from
     * @return Constant reference to the IP address from the given sockaddr
     */
    static const address_type& address(const sockaddr_type& sockaddr)
    {
        return sockaddr.sin6_addr;
    }
}; // end class endpoint_traits<AF_INET6>

} // end namespace detail
} // end namespace endpoint

/**
 * @class socket_endpoint
 * @brief Socket endpoint for IPv4/6 sockets
 */
template <int domain_type, int socket_type>
class socket_endpoint
{    
public:
    using sockaddr_type = typename endpoint::detail::endpoint_traits<domain_type>::sockaddr_type;
    using address_type = typename endpoint::detail::endpoint_traits<domain_type>::address_type;

    /**
     * @brief Default constructor.
     *        Constructs an endpoint with empty IP address and port 0.
     */
    socket_endpoint() : 
        socket_endpoint("", 0)
    {
    }

    /**
     * @brief Constructor.
     *        Constructs an endpoint with empty IP address and given port number.
     * @param port_num Desired port number
     */
    explicit socket_endpoint(const in_port_t port_num) :
        socket_endpoint("", port_num)
    {
    }

    /**
     * @brief Constructor.
     *        Constructs an endpoint with given IP address and port number.
     * @param ip_addr Desired IP address
     * @param port_num Desired port number
     * @throw std::runtime_error Unable to create address from given string
     */
    socket_endpoint(const std::string& ip_addr, const in_port_t port_num)
    {
        memset(&m_sockaddr, 0, sizeof(m_sockaddr));
        m_sockaddr.sin_family = domain_type;
        m_sockaddr.sin_port = htons(port_num);
        if (!ip_addr.empty()) {
            if (inet_pton(domain_type, ip_addr.c_str(), &get_address(m_sockaddr)) <= 0) {
                throw std::runtime_error("Unable to create address " + ip_addr);
            }
        } else {
            memset(&get_address(m_sockaddr), 0x0, sizeof(address_type));
        }
        m_socklen = sizeof(sockaddr_type);
    }

    /**
     * @brief Destructor.
     */
    virtual ~socket_endpoint() = default;

    /**
     * @brief Get the endpoint IP address
     * @return Reference to the endpoint's address
     */
    address_type& address() noexcept
    {
        return get_address(m_sockaddr);
    }

    /**
     * @brief Get the endpoint IP address
     * @return Constant reference to the endpoint's address
     */
    const address_type& address() const noexcept
    {
        return get_address(m_sockaddr);
    }

    /**
     * @brief Set the endpoint IP address
     * @param address Desired IP address
     */
    void address(const std::string& address) noexcept
    {
        if (!address.empty()) {
            if (inet_pton(domain_type, address.c_str(), &get_address(m_sockaddr)) <= 0) {
                std::cerr << "Unable to create address " <<  address << std::endl;
                memset(&get_address(m_sockaddr), 0x0, sizeof(address_type));
            }
        } else {
            memset(&get_address(m_sockaddr), 0x0, sizeof(address_type));
        }
    }

    /**
     * @brief Get the endpoint port number
     * @return Endpoint's port number
     */
    in_port_t port() const noexcept
    {
	    return ntohs(m_sockaddr.sin_port);
    }

    /**
     * @brief Set the endpoint port number
     * @param port Desired port number
     */
    void port(const in_port_t port) noexcept
    {
        m_sockaddr.sin_port = htons(port);
    }

    /**
     * @brief Get the endpoint sockaddr
     * @return Constant reference to the endpoint's sockaddr
     */
    sockaddr_type& sockaddr() noexcept
    {
        return m_sockaddr;
    }

    /**
     * @brief Get the endpoint sockaddr
     * @return Constant reference to the endpoint's address
     */
    const sockaddr_type& sockaddr() const noexcept
    {
        return m_sockaddr;
    }

    /**
     * @brief Get the length of endpoint's sockaddr
     * @return Reference to the length of endpoint's sockaddr
     */
    socklen_t& socklen() noexcept
    {
        return m_socklen;
    }

    /**
     * @brief Get the length of endpoint's sockaddr
     * @return Constant reference to the length of endpoint's sockaddr
     */
    const socklen_t& socklen() const noexcept
    {
        return m_socklen;
    }

    /**
     * @brief Get a string representation of this endpoint
     * @return String representation of this endpoint on success, otherwise empty string
     */
    std::string to_string() const noexcept
    {
        std::string retval;
        const auto addr_len = endpoint::detail::endpoint_traits<domain_type>::address_len;
        retval.resize(addr_len);
        auto addr = inet_ntop(domain_type, &get_address(m_sockaddr), &retval.front(), addr_len);
        if (addr != nullptr) {
            if (!retval.empty()) {
                retval.erase(std::remove(retval.begin(), retval.end(), 0), retval.end());
                retval += std::string(":");
                retval += std::to_string(this->port());
                return retval;
            }
        }
        return {};
    }

private:
    sockaddr_type m_sockaddr;
    socklen_t m_socklen;

    /**
     * @brief Extract the IP address member from the given sockaddr object
     * @param sockaddr The sockaddr object to get IP address from
     * @return Reference to the IP address from the given sockaddr
     */
    address_type& get_address(sockaddr_type& sockaddr)
    {
        return endpoint::detail::endpoint_traits<domain_type>::address(sockaddr);
    }

    /**
     * @brief Extract the IP address member from the given sockaddr object
     * @param sockaddr The sockaddr object to get IP address from
     * @return Constant eference to the IP address from the given sockaddr
     */
    const address_type& get_address(const sockaddr_type& sockaddr) const
    {
        return endpoint::detail::endpoint_traits<domain_type>::address(sockaddr);
    }

}; // end class socket_endpoint

namespace endpoint {
namespace udp {

/**
 * @typedef UDP IPv4 endpoint
 */
using v4 = socket::socket_endpoint<AF_INET, SOCK_DGRAM>;
/**
 * @typedef UDP IPv6 endpoint
 */
using v6 = socket::socket_endpoint<AF_INET6, SOCK_DGRAM>;

} // end namepsace udp
} // end namespace endpoint

} // end namespace socket
} // end namespace vrt

# endif // VRT_SOCKETS_SOCKET_ENDPOINT_HPP
