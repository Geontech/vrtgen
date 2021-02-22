/*#
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
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
#*/
//% from "packet.hpp" import define_packet
//% from "struct.hpp" import define_struct
#pragma once

#include <cstddef>
//% filter namespace(namespace)
//%     set data_packet = []
//%     for packet in packets if packet.is_data
//%         do data_packet.append(true)
//%     endfor
//%     if data_packet
#include <vector>
#include <cstring>

//%     endif
//% endfilter

#include <vrtgen/vrtgen.hpp>

namespace packing {
//% filter namespace(namespace)
//%     for packet in packets
    struct {{packet.helper}};
//%     endfor
//% endfilter
} // end namespace packing

//% filter namespace(namespace)
//%     set struct_packets = []
//%     for packet in packets
//%         if packet.structs and not packet.is_reqs
//%             do struct_packets.append(packet)
//%         endif
//%     endfor
//%     if struct_packets
namespace structs {
//%         for packet in struct_packets
namespace {{packet.name.lower()}} {
//%             for struct in packet.structs
{{define_struct(struct) | trim}}{{'\n' if not loop.last}}
//%             endfor
} // end namespace {{packet.name.lower()}}{{'\n' if not loop.last}}
//%         endfor
} // end namespace structs

//%     endif
//% endfilter

//% filter namespace(namespace)
//%     for packet in packets
{{define_packet(packet)}}

//%     endfor

/**
 * namespace packing
 */
namespace packing {

//%     for packet in packets
    struct {{packet.helper}}
    {
        static bool match(const void* ptr, size_t length);
        static size_t bytes_required(const {{packet.name}}& packet);
        static void pack(const {{packet.name}}& packet, void* ptr, size_t bufsize);
        static void unpack({{packet.name}}& packet, const void* ptr, size_t bufsize);
    }; // end struct {{packet.helper}}

//%     endfor
} // end namespace packing
//% endfilter
