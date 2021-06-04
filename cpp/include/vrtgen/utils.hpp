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

#include <array>
#include <future>

using message_buffer = std::array<char, 65536>;

namespace vrtgen {

template <class SockT, class T, class AckT>
void send_packet(SockT& socket, const T& packet, AckT& ack)
{
    message_buffer message;
    size_t length = T::helper::bytes_required(packet);
    T::helper::pack(packet, message.data(), message.size());
    socket.send_to(message.data(), length, socket.dst());
    std::future<ssize_t> recv_from_fut = std::async(std::launch::async, [&socket, &message]{
        return socket.receive_from(const_cast<message_buffer::value_type*>(message.data()),
                                        message.size(), socket.dst());
    });
    auto status = recv_from_fut.wait_for(std::chrono::seconds(2));
    if (status == std::future_status::timeout) {
        throw std::runtime_error("timed out waiting for acknowledgement");;
    }
    auto reply_length = recv_from_fut.get();
    if (!AckT::helper::match(message.data(), reply_length)) {
        throw std::runtime_error("incorrect acknowledgement type");
    }
    AckT::helper::unpack(ack, message.data(), reply_length);
}

} // end namespace vrtgen