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
#pragma once

#include <cstddef>

#include <vrtgen/vrtgen.hpp>

//% filter namespace(namespace)
//%     for packet in packets
/*%         if not loop.first %*/
 
/*%         endif %*/
{{define_packet(packet)}}
//%     endfor
//% endfilter
