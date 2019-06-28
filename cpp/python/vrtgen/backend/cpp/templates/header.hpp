//% from "packet.hpp" import define_packet
#pragma once

#include <vrtgen/vrtgen.hpp>

//% filter namespace(namespace)
//%     for packet in packets
/*%         if not loop.first %*/
 
/*%         endif %*/
{{define_packet(packet)}}
//%     endfor
//% endfilter
