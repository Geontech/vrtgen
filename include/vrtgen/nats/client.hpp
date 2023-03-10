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
#include <array>
#include <cstdint>
#include <chrono>
#include <memory>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <uuid/uuid.h>
#include <nats/nats.h>

#include "message.hpp"

namespace vrtgen::nats {

class client
{
    static constexpr auto BLOCKING = std::chrono::milliseconds{ INTMAX_MAX };
    using message_ptr = std::unique_ptr<message>;

public:
    /**
     * @brief Default constructor
     */
    client() = default;

    /**
     * @brief URL constructor
     */
    explicit client(const std::string& url)
    {
        natsConnection_ConnectTo(&m_connection, url.c_str());
    }
    
    /**
     * @brief Destructor
     */
    ~client()
    {
        m_done = true;
        stop();
        if (m_sub != nullptr) {
            natsSubscription_Destroy(m_sub);
        }
        if (m_connection != nullptr) {
            natsConnection_Destroy(m_connection);
        }
    }

    /**
     * @brief Returns whether or not the client is in a connected state
     * @return true if client is connected, otherwise false 
     */
    auto is_connected() const -> bool
    {
        return natsConnection_Status(m_connection) == NATS_CONN_STATUS_CONNECTED;
    }

    /**
     * @brief Connect to a NATS server
     * @param url The URL of the NATS server
     */
    auto connect(const std::string& url) -> natsStatus
    {
        return natsConnection_ConnectTo(&m_connection, url.c_str());
    }

    /**
     * @brief Return the NATS server url
     * @return The URL of the NATS server
     */
    auto url(const std::string& url) -> std::string
    {
        std::string retval;
        std::array<char, 256> url_str;
        auto res = natsConnection_GetConnectedUrl(m_connection, url_str.data(), sizeof(url_str));
        if (res == NATS_OK) {
            retval = std::string{ url_str.begin(), url_str.end() };
        }
        return retval;
    }

    /**
     * @brief Publish data bytes on the subject
     * @param subject Name of the subject data is sent to
     * @param data Data bytes to be sent
     */
    auto publish(const std::string& subject, std::span<const uint8_t> data, const std::string& reply="") -> natsStatus
    {
        if (!reply.empty()) {
            return natsConnection_PublishRequest(m_connection, subject.c_str(), reply.c_str(), data.data(), data.size());
        }
        return natsConnection_Publish(m_connection, subject.c_str(), data.data(), data.size());
    }

    /**
     * @brief Create a subscription to a subject
     * @param create_sync If true, create a synchronous subscription, otherwise create an
     *                    asynchronous subscription
     * @return Return status of the subscription 
     */
    auto subscribe(const std::string& subject, const bool create_sync=false) -> natsStatus
    {
        if (subject.empty()) {
            return NATS_INVALID_SUBJECT;
        }
        m_is_sync = create_sync;
        auto res = NATS_OK;
        if (create_sync) {
            res = natsConnection_SubscribeSync(&m_sub, m_connection, subject.c_str());
            if (res == NATS_OK) {
                start();
            }
        } else {
            res = natsConnection_Subscribe(
                &m_sub,
                m_connection,
                subject.c_str(),
                &client::on_msg,
                this
            );
        }
        return res;
    }

    /**
     * @brief Create a subscription to a queue group
     * @param queue_group The name of the queue group
     * @param create_sync If true, create a synchronous subscription, otherwise create an
     *                    asynchronous subscription
     * @return Return status of the subscription
     */
    auto queue_subscribe(const std::string& subject, const std::string& queue_group, const bool create_sync=false) -> natsStatus
    {
        if (subject.empty()) {
            return NATS_INVALID_SUBJECT;
        }
        if (queue_group.empty()) {
            return NATS_INVALID_QUEUE_NAME;
        }
        m_is_sync = create_sync;
        auto res = NATS_OK;
        if (create_sync) {
            res = natsConnection_QueueSubscribeSync(
                &m_sub, m_connection,
                subject.c_str(),
                queue_group.c_str()
            );
            if (res == NATS_OK) {
                start();
            }
        } else {
            res = natsConnection_QueueSubscribe(
                &m_sub,
                m_connection,
                subject.c_str(),
                queue_group.c_str(),
                &client::on_msg,
                this
            );
        }
        return res;
    }

    /**
     * @brief Return the subject name of the subscription
     * @return The name of the subject, empty if not subscribed
     */
    auto subject() const -> std::string
    {
        if (m_sub == nullptr) {
            return {};
        }
        if (auto res = natsSubscription_GetSubject(m_sub); res != nullptr) {
            return { res };
        }
    }

    /**
     * @brief Get the next message from the queue
     * @return Unique pointer to the next message from the queue
     */
    auto next_msg(std::chrono::milliseconds timeout=BLOCKING) -> message_ptr
    {
        using namespace std::chrono_literals;
        std::unique_lock<std::mutex> lock(m_msg_mtx);
        if (timeout == BLOCKING) {
            m_msg_cv.wait(lock, [this]{ return !m_msg_queue.empty() || (m_is_sync && !m_running) || m_done; });
        } else {
            m_msg_cv.wait_for(lock, timeout, [this]{ return !m_msg_queue.empty() || (m_is_sync && !m_running); });
        }
        if (!m_msg_queue.empty()) {
            auto retval = std::move(m_msg_queue.front());
            m_msg_queue.pop();
            return retval;
        }
        return { nullptr };
    }

    /**
     * @brief Get the reply inbox subject name
     * @return The name of the reply subject inbox
     */
    auto inbox() const -> std::string
    {
        if (m_reply_sub == nullptr) {
            return {};
        }
        if (auto res = natsSubscription_GetSubject(m_reply_sub); res != nullptr) {
            return { res };
        }
    }

    /**
     * @brief Create a new reply subscription inbox
     */
    auto new_inbox() -> natsStatus
    {
        if (m_reply_sub != nullptr) {
            natsSubscription_Destroy(m_reply_sub);
        }

        // Generate a unique subject
        uuid_t uuid;
        uuid_generate(uuid);
        std::string uuid_str("0", 37);
        uuid_unparse(uuid, uuid_str.data());

        return natsConnection_Subscribe(
            &m_reply_sub,
            m_connection,
            uuid_str.c_str(),
            &client::on_reply_msg,
            this
        );
    }

    /**
     * @brief Get the next message from the inbox queue
     * @return Unique pointer to the next message from the inbox queue
     */
    auto next_inbox_msg(std::chrono::milliseconds timeout=BLOCKING) -> message_ptr
    {
        using namespace std::chrono_literals;
        std::unique_lock<std::mutex> lock(m_msg_mtx);
        if (timeout == BLOCKING) {
            m_inbox_msg_cv.wait(lock, [this]{ return !m_inbox_msg_queue.empty() || (m_is_sync && !m_running) || m_done; });
        } else {
            m_inbox_msg_cv.wait_for(lock, timeout, [this]{ return !m_inbox_msg_queue.empty() || (m_is_sync && !m_running); });
        }
        if (!m_inbox_msg_queue.empty()) {
            auto retval = std::move(m_inbox_msg_queue.front());
            m_inbox_msg_queue.pop();
            return retval;
        }
        return { nullptr };
    }

private:
    natsConnection* m_connection{ nullptr };
    natsSubscription* m_sub{ nullptr };
    natsSubscription* m_reply_sub{ nullptr };
    // Sync
    bool m_is_sync{ false };
    std::atomic_bool m_running{ false };
    bool m_done{ false };
    std::thread m_thread;
    // Subscription
    std::queue<message_ptr> m_msg_queue;
    std::mutex m_msg_mtx;
    std::condition_variable m_msg_cv;
    // Reply subscription
    std::queue<message_ptr> m_inbox_msg_queue;
    std::mutex m_inbox_msg_mtx;
    std::condition_variable m_inbox_msg_cv;
    
    auto start() -> void
    {
        if (!m_running) {
            m_running = true;
            m_thread = std::thread(&client::thread_func, this);
        }
    }

    auto stop() -> void
    {
        m_running = false;
        m_msg_cv.notify_all();
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    auto thread_func() -> void
    {
        while (m_is_sync && m_running) {
            if (m_sub != nullptr) {
                natsMsg* msg{ nullptr };
                auto res = natsSubscription_NextMsg(&msg, m_sub, 1000);
                if (res == NATS_OK && msg != nullptr) {
                    add_msg(std::make_unique<message>(msg));
                }
            }
        }
    }

    auto add_msg(message_ptr&& data) -> void
    {
        {
            const auto lock = std::lock_guard<std::mutex>{ m_msg_mtx };
            m_msg_queue.push(std::move(data));
        }
        m_msg_cv.notify_one();
    }

    static
    auto on_msg([[maybe_unused]] natsConnection* conn,
                [[maybe_unused]] natsSubscription* sub,
                natsMsg* msg,
                void* ctx) -> void
    {
        if (auto self = static_cast<client*>(ctx); self != nullptr) {
            if (msg != nullptr) {
                self->add_msg(std::make_unique<message>(msg));
            }
        }
    }

    auto add_inbox_msg(message_ptr&& data) -> void
    {
        {
            const auto lock = std::lock_guard<std::mutex>{ m_inbox_msg_mtx };
            m_inbox_msg_queue.push(std::move(data));
        }
        m_inbox_msg_cv.notify_one();
    }

    static
    auto on_reply_msg([[maybe_unused]] natsConnection* conn,
                      [[maybe_unused]] natsSubscription* sub,
                      natsMsg* msg,
                      void* ctx) -> void
    {
        if (auto self = static_cast<client*>(ctx); self != nullptr) {
            if (msg != nullptr) {
                self->add_inbox_msg(std::make_unique<message>(msg));
            }
        }
    }

}; // end class client

} // end namespace vrtgen::nats
