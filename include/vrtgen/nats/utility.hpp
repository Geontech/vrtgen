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
#include <chrono>
#include <stdexcept>

#include "client.hpp"

namespace vrtgen {

template <class CtrlT, class ...AckT>
auto send_packet(vrtgen::nats::client& client, const std::string& subject, CtrlT& packet, AckT&... acks)
{
    // Send packet
    client.publish(subject, packet.data(), client.inbox());
    // Receive acks on unique reply subject
    const auto recv_ack = [&client](auto& ack)
    {
        if (!ack.has_value()) { return; }
        using namespace std::chrono_literals;
        using ack_t = typename std::remove_reference_t<decltype(ack)>::value_type;
        if (auto msg = client.next_inbox_msg(2s)) {
            if (auto match_err = ack_t::match(msg->data())) {
                throw std::runtime_error("incorrect acknowledgement type: " + match_err.value());
            }
            ack = std::move(ack_t{ msg->data() });
        } else {
            throw std::runtime_error("timed out waiting for acknowledgement packet");
        }
    };
    (recv_ack(acks), ...);
}

} // end namespace vrtgen