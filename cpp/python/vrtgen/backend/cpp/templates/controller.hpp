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
//% set helper = 'packing::' + packet.helper
//% set ack_helper = 'packing::' + packet.ack.helper
{{packet.ack.name}} send{{packet.name}}({{packet.name}}& packet)
{
    packet.setMessageID(m_nextMessageID());
    {{packet.ack.name}} ack;
    m_send_packet<{{helper}}, {{ack_helper}}>(packet, ack);
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
    m_send_packet<{{helper}}, {{ack_helper}}>(packet, ack);
    return ack.get{{field.identifier}}();
}{{'\n'}}
//%     endfor
//% endfor
//% endmacro

//% macro handle_configure(packet)
//% set helper = 'packing::' + packet.helper
//% set ack_helper = 'packing::' + packet.ack.helper
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
    m_send_packet<{{helper}}, {{ack_helper}}>(packet, ack);
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
    m_send_packet<{{helper}}, {{ack_helper}}>(packet, ack);
}{{'\n'}}
//%     endfor
//% endfor
//% endmacro

//% macro handle_datactxt(packet)
void set{{packet.name}}Receive(const bool enable)
{
    auto receiver = m_receivers["{{packet.name}}"];
    if (receiver) {
        *receiver = enable;
        m_check_receivers(enable);
    }
}

void register{{packet.name}}Listener(std::function<void({{packet.name}}&)>&& func)
{
    m_{{packet.name}}Listener = std::move(func);
}{{'\n'}}
//% endmacro

//% macro define_functions(packets)
//% for packet in packets
//%     if packet.is_reqs
{{handle_query(packet) | trim}}{{'\n'}}
//%     elif packet.is_reqx
{{handle_configure(packet) | trim}}{{'\n'}}
//%     elif packet.is_data or packet.is_context
{{handle_datactxt(packet) | trim}}{{'\n'}}
//%     endif
//% endfor
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
std::map<std::string, atomic_bool_ptr> m_receivers {
//%     for packet in datactxt_packets
    {{'{'}}"{{packet}}", std::make_shared<std::atomic_bool>(false){{'}'}}{{',' if not loop.last}}
//%     endfor
};
//%     for packet in datactxt_packets
std::function<void({{packet}}&)> m_{{packet}}Listener;
//%     endfor
//% endif
//% endmacro

//% macro define_check_thread(class_name)
void m_check_receivers(const bool enable)
{
    if (enable) {
        if (!m_receiving) {
            m_receiving = true;
            m_recv_thread = std::thread(&{{class_name}}::m_receiver_func, this);
        }
    } else {
        bool is_receiving = false;
        for (const auto& receiver : m_receivers) {
            if (receiver.second && *(receiver.second)) {
                is_receiving = true;
                break;
            }
        }
        if (!is_receiving) {
            m_receiving = false;
            if (m_recv_thread.joinable()) {
                m_recv_thread.join();
            }
        }
    }
}
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

{{define_check_thread(class_name) | trim}}
//% endif
//% endmacro

//% macro define_packet_send_member_func()
template <class Thelper, class Tackhelper, class T, class Tack>
void m_send_packet(const T& packet, Tack& ack)
{
    message_buffer message;
    size_t length = Thelper::bytes_required(packet);
    Thelper::pack(packet, message.data(), message.size());
    m_socket.send_to(message.data(), length, m_socket.endpoint());
    std::future<size_t> reply_length = std::async(std::launch::async, [this, message]{
        return m_socket.receive_from(const_cast<message_buffer::value_type*>(message.data()),
                                     message.size(), m_socket.endpoint());
    });
    auto status = reply_length.wait_for(std::chrono::seconds(2));
    if (status == std::future_status::timeout) {
        throw std::runtime_error("timed out waiting for acknowledgement");;
    }
    if (!Tackhelper::match(message.data(), reply_length.get())) {
        throw std::runtime_error("incorrect acknowledgement type");
    }
    Tackhelper::unpack(ack, message.data(), reply_length.get());
}
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
#include <future>
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
        m_datactxt_socket.bind(endpoint_type(endpoint.address().to_string(), endpoint.port()+1));
    }

    /**
     * @brief Connect to an open socket
     * @param ip_addr The IP address of the scoket to connect to
     * @param port The port number where the socket is listening
     * @return true on success, otherwise false
     */
    bool connect(const endpoint_type& endpoint)
    {
        return m_socket.connect(endpoint);
    }

    /**
     * @brief Disables sends or receives on the socket
     * @param what The operations that will no longer be allowed
     * @throws std::runtime_error Thrown on failure
     */
    void shutdown(const int what)
    {
        m_socket.shutdown(what);
    }

    void setControlleeEndpoint(const endpoint_type& endpoint)
    {
        m_socket.endpoint(endpoint);
    }

    socket_type& socket() noexcept
    {
        return m_socket;
    }

    {{define_functions(packets) | indent(4) | trim}}

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

    {{define_packet_send_member_func() | indent(4) | trim}}

}; // end class {{class_name}}

#endif // {{class_name.upper()}}_HPP
//% endmacro
{{define_controller(information_class.controller_name)}}