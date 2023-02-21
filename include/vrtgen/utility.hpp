/*
 * Copyright (C) 2023 Geon Technologies, LLC
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

#pragma once

#include <string>
#include <stdexcept>
#include <array>
#include <future>
#include <sstream>
#include <utility>
#include <span>
#include <vrtgen/socket.hpp>

namespace vrtgen {

template <typename Ta, typename Te>
inline void validate(Ta actual, Te expected, const std::string& msg)
{
    if (actual != static_cast<Ta>(expected)) {
        throw std::runtime_error(msg);
    }
}

inline void validate(bool cond, const std::string& msg)
{
    validate(cond, true, msg);
}

template <typename T>
inline bool in_range(const T value, const std::pair<T,T>& min_max)
{
    auto min = min_max.first;
    auto max = min_max.second;
    if (value < min || value > max) {
        std::ostringstream err_msg;
        err_msg << "Attempting to set value " << value
                << " outside of valid range "
                << "[" << min << ", " << max << "]";
        throw std::invalid_argument(err_msg.str());
    }
    return true;
}

template <class SockT, class CtrlT, class ...AckT>
requires (std::same_as<SockT, socket::udp::v4>)
void send_packet(SockT& socket, const CtrlT& packet, AckT&... acks)
{
    auto packed_data = CtrlT::helper::pack(packet);
    socket.send_to(packed_data.data(), packed_data.size(), socket.dst());
    const auto recv_ack = [&socket](auto& ack)
    {
        std::array<uint8_t, 65536> message{ 0 };
        std::future<ssize_t> recv_from_fut = std::async(std::launch::async, [&socket, &message]
        {
            return socket.receive_from(message.data(), message.size(), socket.dst());
        });
        auto status = recv_from_fut.wait_for(std::chrono::seconds(2));
        if (status == std::future_status::timeout) {
            throw std::runtime_error("timed out waiting for acknowledgement: " + ack.name());
        }
        auto reply_length = recv_from_fut.get();
        using ack_helper = typename std::remove_reference_t<decltype(ack)>::helper;
        if (auto match_err = ack_helper::match(message.data(), reply_length)) {
            throw std::runtime_error("incorrect acknowledgement type: " + match_err.value());
        }
        ack_helper::unpack(ack, message.data(), reply_length);
    };
    (recv_ack(acks), ...);
}

template <class SockT, class CtrlT, class ...AckT>
requires (std::same_as<SockT, socket::tcp::v4>)
auto send_packet(SockT& socket, CtrlT& packet, AckT&... acks)
{
    auto packed_data = packet.data();
    socket.write_some(packed_data.data(), packed_data.size());
    auto message = std::array<uint8_t, 65536>{ 0 };
    auto reply_length = 0;
    auto read_idx = std::size_t{};
    const auto recv_ack = [&message, &socket, &reply_length, &read_idx](auto& ack)
    {
        if (!ack.has_value()) { return; }
        using ack_t = typename std::remove_reference_t<decltype(ack)>::value_type;
        if (reply_length > 0) {
            auto span = std::span<const uint8_t>{ message.data()+read_idx, static_cast<std::size_t>(reply_length) };
            if (auto match_err = ack_t::match(span)) {
                throw std::runtime_error("incorrect acknowledgement type: " + match_err.value());
            }
            ack = std::move(ack_t{ span });
            reply_length -= ack->size();
            read_idx += ack->size();
            return;
        }
        auto read_fut = std::async(std::launch::async, [&socket, &message]
        {
            return socket.read_some(message.data(), message.size());
        });
        using namespace std::chrono_literals;
        if (auto status = read_fut.wait_for(2s); status == std::future_status::timeout) {
            throw std::runtime_error("timed out waiting for acknowledgement packet");
        }
        reply_length = read_fut.get();
        if (auto match_err = ack_t::match(message)) {
            throw std::runtime_error("incorrect acknowledgement type: " + match_err.value());
        }
        ack = std::move(ack_t{ { message.data(), static_cast<std::size_t>(reply_length) } });
        reply_length -= ack->size();
        read_idx = ack->size();
    };
    (recv_ack(acks), ...);
}

} // end namespace vrtgen
