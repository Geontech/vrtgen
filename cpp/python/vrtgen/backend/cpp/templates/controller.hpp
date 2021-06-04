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
//% macro handle_query(packet)
{{packet.ack.name}} send{{packet.name}}({{packet.name}}& packet)
{
    packet.setMessageID(m_nextMessageID());
    {{packet.ack.name}} ack;
    vrtgen::send_packet(m_socket, packet, ack);
    return ack;
}

//% for cif in packet.cifs if cif.enabled
//%     for field in cif.fields if field.member
{{field.member.datatype}} get{{field.identifier}}()
{
    {{packet.name}} packet;
    packet.setMessageID(m_nextMessageID());
    packet.set{{field.identifier}}Enabled(true);
    {{packet.ack.name}} ack;
    vrtgen::send_packet(m_socket, packet, ack);
    return ack.get{{field.identifier}}();
}{{'\n'}}
//%     endfor
//% endfor
//% endmacro

//% macro handle_configure(packet)
{{packet.ack.name}} send{{packet.name}}({{packet.name}}& packet)
{
    packet.setMessageID(m_nextMessageID());
//% if packet.cam
//%     for field in packet.cam.fields
//%         if field.name == 'Controller' and field.value == 'true'
    packet.setControllerID(m_ControllerID);
//%         endif
//%     endfor
//% endif
    {{packet.ack.name}} ack;
    vrtgen::send_packet(m_socket, packet, ack);
    return ack;
}

//% for cif in packet.cifs if cif.enabled
//%     for field in cif.fields if field.member
void set{{field.identifier}}(const {{field.member.datatype}}& value)
{
    {{packet.name}} packet;
    packet.setMessageID(m_nextMessageID());
//% if packet.cam
//%     for field in packet.cam.fields
//%         if field.name == 'Controller' and field.value == 'true'
    packet.setControllerID(m_ControllerID);
//%         endif
//%     endfor
//% endif
    packet.set{{field.identifier}}(value);
    {{packet.ack.name}} ack;
    vrtgen::send_packet(m_socket, packet, ack);
}{{'\n'}}
//%     endfor
//% endfor
//% endmacro

//% macro handle_datactxt(packet)
void register{{packet.name}}Listener(const std::function<void({{packet.name}}&)>&& func)
{
    m_{{packet.name}}Listener = std::move(func);
}{{'\n'}}
//% endmacro

//% macro define_functions(packets, class_name)
//% set datactxt = []
//% for packet in packets
//%     if packet.is_reqs
{{handle_query(packet) | trim}}{{'\n'}}
//%     elif packet.is_reqx
{{handle_configure(packet) | trim}}{{'\n'}}
//%     elif packet.is_data or packet.is_context
{{handle_datactxt(packet) | trim}}{{'\n'}}
//%         do datactxt.append(packet)
//%     endif
//% endfor
//% if datactxt
void enableReceive()
{
    if (!m_receiving) {
        m_receiving = true;
        m_recv_thread = std::thread(&{{class_name}}::m_receiver_func, this);
    }
}

void disableReceive()
{
    m_receiving = false;
    if (m_recv_thread.joinable()) {
        m_recv_thread.join();
    }
}
//% endif
//% endmacro

//% macro define_datactxt_members(packets)
//% set datactxt_packets = []
//% set controller = []
//% for packet in packets
//%     for field in packet.prologue.fields
//%         if field.identifier == 'ControllerID' and not field.optional and not controller
{{field.member.type}} {{field.member.identifier}};
//%             do controller.append(true)
//%         endif
//%     endfor
//%     if packet.is_data or packet.is_context
//%         do datactxt_packets.append(packet.name)
//%     endif
//% endfor
//% if datactxt_packets
std::thread m_recv_thread;
std::atomic_bool m_receiving{ false };
//%     for packet in datactxt_packets
std::function<void({{packet}}&)> m_{{packet}}Listener;
//%     endfor
//% endif
//% endmacro

//% macro define_datactxt_member_funcs(packets, class_name)
//% set datactxt = []
//% for packet in packets
//%     if packet.is_data or packet.is_context
//%         do datactxt.append(packet)
//%     endif
//% endfor
//% if datactxt
void m_receiver_func()
{
    message_buffer message;
    while(m_receiving) {
        endpoint_type endpoint;
        auto recv_length = m_datactxt_socket.receive_from(message.data(), message.size(), endpoint);
        if (recv_length < 0) {
            continue;
        }

//%     for packet in datactxt
        if (packing::{{packet.helper}}::match(message.data(), recv_length)) {
            {{packet.name}} packet;
            packing::{{packet.helper}}::unpack(packet, message.data(), recv_length);
            if (m_{{packet.name}}Listener) {
                m_{{packet.name}}Listener(packet);
            }
        }
//%     endfor
    }
}
//% endif
//% endmacro

//% macro define_controller(class_name)
#ifndef {{class_name.upper()}}_HPP
#define {{class_name.upper()}}_HPP

#include <array>
//% for packet in packets
//%     if packet.is_data
#include <map>
#include <atomic>
#include <thread>
#include <memory>
#include <functional>
//%         break
//%     endif
//% endfor

#include <vrtgen/socket/udp.hpp>
#include "{{header}}"

class {{class_name}}
{
    using socket_type = vrtgen::socket::udp::v4;
    using endpoint_type = typename socket_type::endpoint_type;
    using message_buffer = std::array<char, 65536>;
//% for packet in packets
//%     if packet.is_data
    using atomic_bool_ptr = std::shared_ptr<std::atomic_bool>;

//%         break
//%     endif
//% endfor

public:
    {{class_name}}() = default;
    explicit {{class_name}}(const endpoint_type& endpoint)
    {
        m_socket.bind(endpoint);
        auto datactxt_endpoint = endpoint;
        datactxt_endpoint.port(endpoint.port()+1);
        m_datactxt_socket.bind(datactxt_endpoint);
    }

    void setControlleeEndpoint(const endpoint_type& endpoint)
    {
        m_socket.dst(endpoint);
    }

    const endpoint_type& getControlleeEndpoint() const
    {
        return m_socket.dst();
    }

    socket_type& socket() noexcept
    {
        return m_socket;
    }

    {{define_functions(packets, class_name) | indent(4) | trim}}

private:
    socket_type m_socket;
    socket_type m_datactxt_socket;
    vrtgen::MessageIdentifier m_messageID = 1;
    {{define_datactxt_members(packets) | indent(4) | trim}}

    vrtgen::MessageIdentifier m_nextMessageID()
    {
        return m_messageID++;
    }

    {{define_datactxt_member_funcs(packets, class_name) | indent(4) | trim}}

}; // end class {{class_name}}

#endif // {{class_name.upper()}}_HPP
//% endmacro
{{define_controller(information_class.controller_name)}}
