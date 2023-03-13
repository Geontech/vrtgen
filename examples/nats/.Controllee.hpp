#pragma once

#include <thread>
#include <atomic>

#include <Controllee_base.hpp>

#include <signal_data.hpp>
#include <example_context.hpp>

namespace example::nats::controllee {

class Controllee : public Controllee_base
{
public:
    /**
     * @brief Endpoint constructor
     * @param nats_url NATS server url to connect to
     * @param subject NATS subject to be used for command messages
     */
    Controllee(const std::string& nats_url, const std::string& subject) :
        Controllee_base(nats_url, subject)
    {
    }

    /**
     * @brief Deconstructor
     */
    ~Controllee() override
    {
        if (m_streaming) {
            stop_stream();
        }
    }

    auto execute_example_control(ExampleControl& packet) -> ExampleControlAckVX override
    {
        auto ack = ExampleControlAckVX{};
        std::cout << "Received example control packet" << std::endl;
        if (packet.bandwidth().has_value()) {
            std::cout << "  - Bandwidth       : " << packet.bandwidth().value() << std::endl;
            if (packet.bandwidth().value() > 1e6) {
                // Just for warning demonstration purposes
                vrtgen::packing::WarningErrorFields warn;
                warn.field_not_executed(true);
                warn.parameter_out_of_range(true);
                ack.bandwidth_warnings(warn);
            } else {
                m_bandwidth = packet.bandwidth().value();
            }
        }
        if (packet.rf_ref_frequency().has_value()) {
            std::cout << "  - RF Ref Frequency: " << packet.rf_ref_frequency().value() << std::endl;
            m_rf_freq = packet.rf_ref_frequency().value();
        }
        if (packet.sample_rate().has_value()) {
            std::cout << "  - Sample Rate     : " << packet.sample_rate().value() << std::endl;
            m_sample_rate = packet.sample_rate().value();
        }
        if (packet.gain().has_value()) {
            std::cout << "  - Gain            : " << packet.gain().value().stage_1() << std::endl;
            m_gain = packet.gain().value().stage_1();
        }
        if (packet.discrete_io_32().has_value()) {
            if (packet.discrete_io_32().value().stream_enable_enable()) {
                if (packet.discrete_io_32().value().stream_enable()) {
                    start_stream();
                } else {
                    stop_stream();
                }
            }
        }
        return ack;
    }

private:
    std::atomic_bool m_streaming{ false };
    std::thread m_thread;
    double m_bandwidth{ 0 };
    double m_rf_freq{ 0 };
    double m_sample_rate{ 0 };
    float m_gain{ 0 };

    auto start_stream() -> void
    {
        if (!m_streaming) {
            std::cout << "Starting data stream..." << std::endl;
            m_data_ctxt_send_socket.bind({ "127.0.0.1", 5003 });
            m_streaming = true;
            m_thread = std::thread(&Controllee::thread_func, this);
        }
    }

    auto stop_stream() -> void
    {
        std::cout << "Stopping data stream..." << std::endl;
        m_streaming = false;
        if (m_thread.joinable()) {
            m_thread.join();
        }
        m_data_ctxt_send_socket.close();
    }

    void thread_func()
    {
        while (m_streaming) {
            std::array<uint16_t, 1000> data;
            for (unsigned i=0; i<data.size(); ++i) {
                data[i] = i + 1;
            }
            example::nats::packets::SignalData data_packet;
            data_packet.stream_id(0x4321);
            data_packet.payload({ reinterpret_cast<uint8_t*>(data.data()), data.size() * sizeof(uint16_t) });
            data_packet.trailer().valid_data(true);
            auto packed_data = data_packet.data();
            std::cout << "Sending data packet..." << std::endl;
            m_data_ctxt_send_socket.send_to(packed_data.data(), packed_data.size(), { "127.0.0.1", 5002 });
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
            example::nats::packets::ExampleContext ctxt_packet;
            ctxt_packet.stream_id(0x4321);
            ctxt_packet.bandwidth(m_bandwidth);
            ctxt_packet.rf_ref_frequency(m_rf_freq);
            ctxt_packet.sample_rate(m_sample_rate);
            vrtgen::packing::Gain gain;
            gain.stage_1(m_gain);
            ctxt_packet.gain(gain);
            packed_data = ctxt_packet.data();
            std::cout << "Sending context packet..." << std::endl;
            m_data_ctxt_send_socket.send_to(packed_data.data(), packed_data.size(), { "127.0.0.1", 5002 });
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
        }
    }

}; // end class Controllee

} // end namespace example::nats::controllee