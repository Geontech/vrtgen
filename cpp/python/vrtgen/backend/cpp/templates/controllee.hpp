/*#
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
#*/
//% macro handle_query(packet)
//% for cif in packet.cifs if cif.enabled
//%     for field in cif.fields if field.member
virtual {{field.member.datatype}} get{{field.identifier}}()
{
    // AUTO-GENERATED FUNCTION STUB
    // IMPLEMENT HARDWARE-SPECIFIC FUNCTIONALITY HERE
    throw std::runtime_error("get{{field.identifier}} not implemented");
}{{'\n' if not loop.last}}
//%     endfor
//% endfor
//% endmacro

//% macro handle_configure(packet)
//% for cif in packet.cifs if cif.enabled
//%     for field in cif.fields if field.member
//%         if 'DiscreteIO' in field.identifier
virtual void set{{field.identifier}}(const {{field.member.datatype}}& value,
                             const endpoint_type& controllerDataEndpoint)
//%         else
virtual void set{{field.identifier}}(const {{field.member.datatype}}& value)
//%         endif
{
    // AUTO-GENERATED FUNCTION STUB
    // IMPLEMENT HARDWARE-SPECIFIC FUNCTIONALITY HERE
    throw std::runtime_error("set{{field.identifier}} not implemented");
}{{'\n' if not loop.last}}
//%     endfor
//% endfor
//% endmacro

//% macro handle_data(packet)
virtual void send{{packet.name}}({{packet.name}}& value)
{
    // AUTO-GENERATED FUNCTION STUB
    // IMPLEMENT HARDWARE-SPECIFIC FUNCTIONALITY HERE
    throw std::runtime_error("send{{packet.name}} not implemented");
}
//% endmacro

//% macro define_controllee(class_name, base_class_name)
#ifndef {{class_name.upper()}}_HPP
#define {{class_name.upper()}}_HPP

#include <stdexcept>

#include "{{base_class_name}}.hpp"

class {{class_name}} : public {{base_class_name}}
{

public:
    {{class_name}}() = default;
    virtual ~{{class_name}}() = default;

//% for packet in packets
//%     if packet.is_reqs
    {{handle_query(packet) | indent(4) | trim}}{{'\n'}}
//%     elif packet.is_reqx
    {{handle_configure(packet) | indent(4) | trim}}{{'\n'}}
//%     endif
//% endfor

}; // end class {{class_name}}

#endif // {{class_name.upper()}}_HPP
//% endmacro
{{define_controllee(information_class.controllee_name, information_class.controllee_base_name)}}