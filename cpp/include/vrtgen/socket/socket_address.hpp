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

#ifndef VRTGEN_SOCKET_SOCKET_ADDRESS_HPP
#define VRTGEN_SOCKET_SOCKET_ADDRESS_HPP

#include <netdb.h>
#include <sys/types.h>

namespace vrtgen {
namespace socket {

namespace address {
namespace detail {

template <int domain>
class address_traits;

template <>
class address_traits<AF_INET>
{
public:
    using address_type = in_addr;
    static constexpr auto address_len = INET_ADDRSTRLEN;
};

template <>
class address_traits<AF_INET6>
{
public:
    using address_type = in6_addr;
    static constexpr auto address_len = INET6_ADDRSTRLEN;
};

} // end namespace detail
} // end namespace address

template <int domain_type, int socket_type>
class socket_address
{
    using address_type = typename address::detail::address_traits<domain_type>::address_type;
    
public:
    static constexpr auto address_len = address::detail::address_traits<domain_type>::address_len;
    
    socket_address() = default;
    explicit socket_address(const std::string& address)
    {
        if (!address.empty()) {
            if (inet_pton(domain_type, address.c_str(), &m_address) <= 0) {
                throw std::runtime_error("Unable to create address " + address);
            }
        } else {
            m_address.s_addr = INADDR_ANY;
        }
    }

    virtual ~socket_address() = default;

    const address_type& address() const noexcept
    {
        return m_address;
    }

    void address(const address_type& address)
    {
        m_address = address;
    }

    std::string to_string() const noexcept
    {
        std::string retval;
        retval.resize(address_len);
        auto addr = inet_ntop(domain_type, &m_address, &retval.front(), address_len);
        if (addr != nullptr) {
            return retval;
        }
        return {};
    }

private:
    address_type m_address;

}; // end class socket_address

namespace address {
namespace udp {

using v4 = socket::socket_address<AF_INET, SOCK_DGRAM>;
using v6 = socket::socket_address<AF_INET6, SOCK_DGRAM>;

} // end namespace udp
} // end namespace address

} // end namespace socket
} // end namespace vrtgen

# endif // VRTGEN_SOCKET_SOCKET_ADDRESS_HPP
