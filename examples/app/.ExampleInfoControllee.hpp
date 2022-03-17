#ifndef EXAMPLEINFOCONTROLLEE_HPP
#define EXAMPLEINFOCONTROLLEE_HPP

#include <stdexcept>

#include "ExampleInfoControllee_base.hpp"

class ExampleInfoControllee : public ExampleInfoControllee_base
{

public:
    explicit ExampleInfoControllee(const cmd_endpoint_type& endpoint)
    {
        if (!_cmd_socket.bind(endpoint)) {
            throw std::runtime_error("Failed to bind socket during ExampleInfoControllee construction");
        }
    }
    virtual ~ExampleInfoControllee() = default;

    virtual void handle_example_control(const ExampleControl& packet)
    {
        std::cout << "Received example control packet" << std::endl;
        if (packet.has_bandwidth()) {
            std::cout << "  - Bandwidth       : " << packet.bandwidth() << std::endl;
            _bandwidth = packet.bandwidth();
        }
        if (packet.has_rf_ref_frequency()) {
            std::cout << "  - RF Ref Frequency: " << packet.rf_ref_frequency() << std::endl;
            _rf_freq = packet.rf_ref_frequency();
        }
        if (packet.has_sample_rate()) {
            std::cout << "  - Sample Rate     : " << packet.sample_rate() << std::endl;
            _sample_rate = packet.sample_rate();
        }
        if (packet.has_gain()) {
            std::cout << "  - Gain            : " << packet.gain().stage_1() << std::endl;
            _gain = packet.gain().stage_1();
        }
        if (packet.has_discrete_io_32()) {
            if (packet.discrete_io_32().stream_enable_enable()) {
                if (packet.discrete_io_32().stream_enable()) {
                    this->start_stream();
                } else {
                    this->stop_stream();
                }
            }
        }
        ExampleControlAck ack;
        ack.stream_id(packet.stream_id());
        ack.message_id(packet.message_id());
        auto packed_bytes = ExampleControlAck::helper::pack(ack);
        _cmd_socket.write_some(packed_bytes.data(), packed_bytes.size());
    }

    void start_stream()
    {
        if (!_streaming) {
            std::cout << "Starting data stream..." << std::endl;
            _data_ctxt_send_socket.bind({ "127.0.0.1", 5003 });
            _streaming = true;
            _thread = std::thread(&ExampleInfoControllee::_thread_func, this);
        }
    }

    void stop_stream()
    {
        std::cout << "Stopping data stream..." << std::endl;
        _streaming = false;
        if (_thread.joinable()) {
            _thread.join();
        }
        _data_ctxt_send_socket.close();
    }

private:
    std::atomic_bool _streaming{ false };
    std::thread _thread;
    double _bandwidth{ 0 };
    double _rf_freq{ 0 };
    double _sample_rate{ 0 };
    float _gain{ 0 };

    void _thread_func()
    {
        while (_streaming) {
            std::array<uint16_t, 1000> data;
            for (unsigned i=0; i<data.size(); ++i) {
                data[i] = i + 1;
            }
            SignalData data_packet;
            data_packet.stream_id(0x4321);
            data_packet.payload(data.data(), data.size() * sizeof(uint16_t));
            data_packet.valid_data(true);
            auto packed_data = packing::SignalDataHelper::pack(data_packet);
            std::cout << "Sending data packet..." << std::endl;
            _data_ctxt_send_socket.send_to(packed_data.data(), packed_data.size(), { "127.0.0.1", 5002 });
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
            ExampleContext ctxt_packet;
            ctxt_packet.stream_id(0x4321);
            ctxt_packet.bandwidth(_bandwidth);
            ctxt_packet.rf_ref_frequency(_rf_freq);
            ctxt_packet.sample_rate(_sample_rate);
            vrtgen::packing::Gain gain;
            gain.stage_1(_gain);
            ctxt_packet.gain(gain);
            packed_data = packing::ExampleContextHelper::pack(ctxt_packet);
            std::cout << "Sending context packet..." << std::endl;
            _data_ctxt_send_socket.send_to(packed_data.data(), packed_data.size(), { "127.0.0.1", 5002 });
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
        }
    }

}; // end class ExampleInfoControllee

#endif // EXAMPLEINFOCONTROLLEE_HPP