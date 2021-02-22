/*#
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
#*/
//% macro handle_query_callback(packet)
//% for cif in packet.cifs if cif.enabled
//%     for field in cif.fields if field.member
virtual {{field.member.datatype}} get{{field.identifier}}() = 0;
//%     endfor
//% endfor
//% endmacro

//% macro handle_configure_callback(packet)
//% for cif in packet.cifs if cif.enabled
//%     for field in cif.fields if field.member
//%         if 'DiscreteIO' in field.identifier
virtual void set{{field.identifier}}(const {{field.member.datatype}}& value,
                             const endpoint_type& controllerDataEndpoint) = 0;
//%         else
virtual void set{{field.identifier}}(const {{field.member.datatype}}& value) = 0;
//%         endif
//%     endfor
//% endfor
//% endmacro

//% macro handle_data_callback(packet)
virtual void send{{packet.name}}({{packet.name}}& packet) = 0;
//% endmacro

//% macro handle_query(packet)
if (packing::{{packet.helper}}::match(m_message.data(), recv_length)) {
    {{packet.name}} packet;
    packing::{{packet.helper}}::unpack(packet, m_message.data(), recv_length);
    {{packet.ack.name}} ack;
//% for cif in packet.cifs if cif.enabled
//%     for field in cif.fields if field.member
//%         if field.optional
//%             if packet.is_reqs
    if (packet.is{{field.identifier}}Enabled()) {
//%             else
    if (packet.has{{field.identifier}}()) {
//%             endif
        ack.set{{field.identifier}}(this->get{{field.identifier}}());
    }
//%         else
    ack.set{{field.identifier}}(this->get{{field.identifier}}());
//%         endif
//%     endfor
//% endfor
    ack.setMessageID(packet.getMessageID());
    ack.setStreamIdentifier(packet.getStreamIdentifier());
    packing::{{packet.ack.helper}}::pack(ack, m_message.data(), m_message.size());
    size_t reply_length = packing::{{packet.ack.helper}}::bytes_required(ack);
    m_socket.send_to(m_message.data(), reply_length, endpoint);
}{{'\n'}}
//% endmacro

//% macro handle_configure(packet)
if (packing::{{packet.helper}}::match(m_message.data(), recv_length)) {
    {{packet.name}} packet;
    packing::{{packet.helper}}::unpack(packet, m_message.data(), recv_length);
    endpoint_type data_endpoint(endpoint.address().to_string(), endpoint.port()+1);
//% for cif in packet.cifs if cif.enabled
//%     for field in cif.fields if field.member
//%         if field.optional
    if (packet.has{{field.identifier}}()) {
//%             if 'DiscreteIO' in field.identifier
        this->set{{field.identifier}}(packet.get{{field.identifier}}(), data_endpoint);
//%             else
        this->set{{field.identifier}}(packet.get{{field.identifier}}());
//%             endif
    }
//%         else
//%             if 'DiscreteIO' in field.identifier
    this->set{{field.identifier}}(packet.get{{field.identifier}}(), data_endpoint);
//%             else
    this->set{{field.identifier}}(packet.get{{field.identifier}}());
//%             endif
//%         endif
//%     endfor
//% endfor
    {{packet.ack.name}} ack;
    ack.setMessageID(packet.getMessageID());
    ack.setStreamIdentifier(packet.getStreamIdentifier());
    packing::{{packet.ack.helper}}::pack(ack, m_message.data(), m_message.size());
    size_t reply_length = packing::{{packet.ack.helper}}::bytes_required(ack);
    m_socket.send_to(m_message.data(), reply_length, endpoint);
}{{'\n'}}
//% endmacro

//% macro handle_data(packet)
if (packing::{{packet.helper}}::match(m_message.data(), recv_length)) {
    {{packet.name}} packet;
    packing::{{packet.helper}}::unpack(packet, m_message.data(), recv_length);
}{{'\n'}}
//% endmacro

//% macro define_controllee_base(class_name)
#ifndef {{class_name.upper()}}_HPP
#define {{class_name.upper()}}_HPP

#include <array>
#include <thread>
#include <atomic>
#include <map>
#include <vrtgen/socket/udp.hpp>

#include "{{header}}"

class {{class_name}}
{
    using socket_type = vrtgen::socket::udp::v4;
    using message_buffer = std::array<char, 65536>;
protected:
    using endpoint_type = typename socket_type::endpoint_type;

public:
    {{class_name}}() = default;
    virtual ~{{class_name}}()
    {
        m_listening = false;
        if (m_recv_thread.joinable()) {
            m_recv_thread.join();
        }
    }

    socket_type& socket() noexcept
    {
        return m_socket;
    }

    bool bind(const endpoint_type& endpoint)
    {
        if (m_socket.bind(endpoint)) {
            return true;
        }
        return false;
    }

    void vrt_listen()
    {
        if (!m_listening) {
            m_listening = true;
            m_recv_thread = std::thread(&{{class_name}}::m_listener_func, this);
        }
    }

//% for packet in packets
//%     if packet.is_reqs
    {{handle_query_callback(packet) | indent(4) | trim}}{{'\n'}}
//%     elif packet.is_reqx
    {{handle_configure_callback(packet) | indent(4) | trim}}{{'\n'}}    
//%     endif
//% endfor

private:
    socket_type m_socket;
    std::thread m_recv_thread;
    std::atomic_bool m_listening;
    message_buffer m_message;

    void m_listener_func()
    {
        while(m_listening) {
            endpoint_type endpoint;
            auto recv_length = m_socket.receive_from(m_message.data(), m_message.size(), endpoint);

//% for packet in packets
//%     if packet.is_reqs
            {{handle_query(packet) | indent(12) | trim}}
//%     elif packet.is_reqx
            {{handle_configure(packet) | indent(12) | trim}}
//%     elif packet.is_data
            {{handle_data(packet) | indent(12) | trim}}
//%     endif
//% endfor
        }
    }

}; // end class {{class_name}}

#endif // {{class_name.upper()}}_HPP
//% endmacro
{{define_controllee_base(information_class.controllee_base_name)}}