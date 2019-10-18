/*
 * Copyright (C) 2019 Geon Technologies, LLC
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

#include <cstddef>
#include <new>
#include <stdexcept>

#include <vrtgen/enums.hpp>
#include <vrtgen/types.hpp>
#include <vrtgen/packing.hpp>

namespace vrtgen {
    namespace detail {
        template <class T>
        struct overlay_buffer
        {
        public:
            template <typename Tin>
            overlay_buffer(Tin* buffer, size_t bytes) :
                m_start(reinterpret_cast<T*>(buffer)),
                m_end(m_start + bytes),
                m_pos(m_start)
            {
            }

            template <typename Tout>
            Tout* next()
            {
                Tout* ptr = reinterpret_cast<Tout*>(m_pos);
                m_pos += sizeof(Tout);
                return ptr;
            }

            size_t getpos() const
            {
                return m_pos - m_start;
            }

        private:
            T* const m_start;
            T* const m_end;
            T* m_pos;
        };
    }
    class OutputBuffer
    {
    public:
        OutputBuffer(void* buffer, size_t bytes) :
            m_buf(buffer, bytes)
        {
        }

        template <typename T, typename ...Args>
        T* insert(Args... args)
        {
            T* ptr = m_buf.next<T>();
            return new (ptr) T(args...);
        }

        size_t size() const
        {
            return m_buf.getpos();
        }

    private:
        detail::overlay_buffer<char> m_buf;
    };

    class InputBuffer
    {
    public:
        InputBuffer(const void* buffer, size_t bytes) :
            m_buf(buffer, bytes),
            m_Header(m_buf.next<const vrtgen::packing::Header>()),
            m_StreamIdentifier(nullptr),
            m_ClassIdentifier(nullptr),
            m_IntegerTimestamp(nullptr),
            m_FractionalTimestamp(nullptr),
            m_ControlAcknowledgeMode(nullptr),
            m_MessageID(nullptr),
            m_CIF0(nullptr),
            m_CIF1(nullptr)
        {
            vrtgen::PacketType::Code packet_type = m_Header->getPacketType();
            if (packet_type != vrtgen::PacketType::SIGNAL_DATA) {
                m_StreamIdentifier = m_buf.next<const vrtgen::packing::StreamIdentifier>();
            }
            if (m_Header->isClassIdentifierEnabled()) {
                m_ClassIdentifier = m_buf.next<const vrtgen::packing::ClassIdentifier>();
            }
            if (m_Header->getTSI() != vrtgen::TSI::NONE) {
                m_IntegerTimestamp = m_buf.next<const vrtgen::packing::IntegerTimestamp>();
            }
            if (m_Header->getTSF() != vrtgen::TSF::NONE) {
                m_FractionalTimestamp = m_buf.next<const vrtgen::packing::FractionalTimestamp>();
            }

            // Data packets will not have any CIF enables in their prologue
            switch (packet_type) {
                case vrtgen::PacketType::SIGNAL_DATA:
                case vrtgen::PacketType::SIGNAL_DATA_STREAM_ID:
                case vrtgen::PacketType::EXTENSION_DATA:
                case vrtgen::PacketType::EXTENSION_DATA_STREAM_ID:
                    return;
                case vrtgen::PacketType::COMMAND:
                case vrtgen::PacketType::EXTENSION_COMMAND:
                    m_ControlAcknowledgeMode = m_buf.next<const vrtgen::packing::ControlAcknowledgeMode>();
                    m_MessageID = m_buf.next<const vrtgen::packing::MessageID>();
                default:
                    break;
            }

            m_CIF0 = m_buf.next<const vrtgen::packing::CIF0Enables>();
            if (m_CIF0->getCIF1Enable()) {
                m_CIF1 = m_buf.next<const vrtgen::packing::CIF1Enables>();
            }
        }

        const vrtgen::packing::Header* getHeader() const
        {
            return m_Header;
        }

        bool hasStreamIdentifier() const
        {
            return m_StreamIdentifier;
        }

        vrtgen::StreamIdentifier getStreamIdentifier() const
        {
            if (!hasStreamIdentifier()) {
                throw std::runtime_error("missing Stream Identifier");
            }
            return m_StreamIdentifier->get();
        }

        bool hasClassIdentifier() const
        {
            return m_ClassIdentifier;
        }

        const vrtgen::packing::ClassIdentifier* getClassIdentifier() const
        {
            if (!hasClassIdentifier()) {
                throw std::runtime_error("missing Class Identifier");
            }
            return m_ClassIdentifier;
        }

        bool hasIntegerTimestamp() const
        {
            return m_IntegerTimestamp;
        }

        uint32_t getIntegerTimestamp() const
        {
            if (!hasIntegerTimestamp()) {
                throw std::runtime_error("missing Integer Timestamp");
            }
            return m_IntegerTimestamp->get();
        }

        bool hasFractionalTimestamp() const
        {
            return m_FractionalTimestamp;
        }

        uint64_t getFractionalTimestamp() const
        {
            if (!hasFractionalTimestamp()) {
                throw std::runtime_error("missing Fractional Timestamp");
            }
            return m_FractionalTimestamp->get();
        }

        const vrtgen::packing::ControlAcknowledgeMode* getControlAcknowledgeMode() const
        {
            return m_ControlAcknowledgeMode;
        }

        vrtgen::MessageIdentifier getMessageID() const
        {
            if (!m_MessageID) {
                throw std::runtime_error("no Message ID");
            }
            return m_MessageID->get();
        }

        const vrtgen::packing::CIF0Enables* getCIF0() const
        {
            return m_CIF0;
        }

        const vrtgen::packing::CIF1Enables* getCIF1() const
        {
            return m_CIF1;
        }

        template <typename T>
        const T* next()
        {
            return m_buf.next<const T>();
        }

        template <typename T>
        typename T::value_type get()
        {
            return next<T>()->get();
        }

    private:
        detail::overlay_buffer<const char> m_buf;
        const vrtgen::packing::Header* const m_Header;
        const vrtgen::packing::StreamIdentifier* m_StreamIdentifier;
        const vrtgen::packing::ClassIdentifier* m_ClassIdentifier;
        const vrtgen::packing::IntegerTimestamp* m_IntegerTimestamp;
        const vrtgen::packing::FractionalTimestamp* m_FractionalTimestamp;
        const vrtgen::packing::ControlAcknowledgeMode* m_ControlAcknowledgeMode;
        const vrtgen::packing::MessageID* m_MessageID;
        const vrtgen::packing::CIF0Enables* m_CIF0;
        const vrtgen::packing::CIF1Enables* m_CIF1;
    };
}
