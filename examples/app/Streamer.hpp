#include <thread>
#include <atomic>
#include <array>
#include <vrtgen/socket/udp.hpp>
#include <cmath>
#include <chrono>

#include "rdc-information.hpp"

class Streamer
{
    using socket_type = vrtgen::socket::udp::v4;
    using endpoint_type = typename socket_type::endpoint_type;
    using message_buffer = std::array<uint8_t, 65536>;

public:
    Streamer() = default;
    ~Streamer()
    {
        m_streaming = false;
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    void setControllerDataEndpoint(const endpoint_type& controllerDataEndpoint)
    {
        m_socket.dst(controllerDataEndpoint);
    }

    void startStream()
    {
        std::cout << "Streamer start" << std::endl;
        m_streaming = true;
        m_thread = std::thread(&Streamer::m_thread_func, this);
    }

    void stopStream()
    {
        std::cout << "Streamer stop" << std::endl;
        m_streaming = false;
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }
    
    void setGain(const double gain)
    {
        m_amp = pow(10.0, gain/10.0);
        message_buffer message;
        TunerInfo packet;
        vrtgen::packing::Gain gainret;
        gainret.setStage1(gain);
        packet.setGain(gainret);
        size_t length = packing::TunerInfoHelper::bytes_required(packet);
        packing::TunerInfoHelper::pack(packet, message.data(), message.size());
        m_socket.send_to(message.data(), length, m_socket.dst());
    }

    void setFrequency(const double freq)
    {
        m_freq = freq;
        message_buffer message;
        TunerInfo packet;
        packet.setRFReferenceFrequency(m_freq);
        size_t length = packing::TunerInfoHelper::bytes_required(packet);
        packing::TunerInfoHelper::pack(packet, message.data(), message.size());
        m_socket.send_to(message.data(), length, m_socket.dst());
    }

    double getFrequency()
    {
        return m_freq;
    }

    void setSampleRate(const double rate)
    {
        m_rate = rate;
        message_buffer message;
        TunerInfo packet;
        packet.setSampleRate(m_rate);
        size_t length = packing::TunerInfoHelper::bytes_required(packet);
        packing::TunerInfoHelper::pack(packet, message.data(), message.size());
        m_socket.send_to(message.data(), length, m_socket.dst());
    }

    void setTransferLen(const int len)
    {
        m_xfer_len = len;
    }

private:
    double m_amp{1};
    double m_freq{50};
    double m_rate{10000};
    int m_xfer_len{1000};
    std::atomic_bool m_streaming;
    std::thread m_thread;
    socket_type m_socket;
    const double twopi = 2. * M_PI;

    void m_thread_func()
    {
        while (m_streaming) {
            std::array<float, 1000> data;
            for (unsigned i=0; i<data.size(); ++i) {
                data[i] = m_amp*std::sin(twopi*m_freq*(i/m_rate));
            }
            auto len = data.size() * sizeof(float);
            message_buffer message;
            SignalData packet;
            packet.setRawData(data.data(), len);
            size_t length = packing::SignalDataHelper::bytes_required(packet) +
                            packet.getRawDataSize();            
            packing::SignalDataHelper::pack(packet, message.data(), message.size());
            m_socket.send_to(message.data(), length, m_socket.dst());
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

}; // end class Streamer
