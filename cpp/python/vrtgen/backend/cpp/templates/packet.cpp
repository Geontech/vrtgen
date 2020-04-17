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
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
#*/

//% macro pack_subfield(field, subfield)
{{field.attr}}->{{subfield.src.setter}}(packet.{{subfield.getter}}());
//% endmacro

//% macro pack_struct(field)
{{field.type}}* {{field.attr}} = buffer.insert<{{field.type}}>();
//%     for subfield in field.fields
//%         if subfield.value is defined
{{field.attr}}->{{subfield.src.setter}}({{subfield.value}});
//%         elif subfield.optional
if (packet.{{subfield.check}}()) {
    {{field.attr}}->{{subfield.src.enable.setter}}(true);
    {{pack_subfield(field, subfield)}}
}
//%         else
{{pack_subfield(field, subfield)}}
//%         endif
//%     endfor
//% endmacro

//% macro pack_field(field)
//%     if field.struct
{{pack_struct(field)}}
//%     else
buffer.insert<{{field.type}}>(packet.get{{field.name}}());
//%     endif
//% endmacro

//% macro unpack_field(field)
//%     if field.struct
const {{field.type}}* {{field.attr}} = buffer.next<{{field.type}}>();
{{unpack_struct(field)}}
//%     else
packet.set{{field.name}}(buffer.get<{{field.type}}>());
//%     endif
//% endmacro

//% macro unpack_subfield(field, subfield)
packet.{{subfield.setter}}({{field.attr}}->{{subfield.src.getter}}());
//% endmacro

//% macro unpack_struct(field)
//%     for subfield in field.fields
//%         if subfield.value is defined
::validate({{field.attr}}->{{subfield.src.getter}}(), {{subfield.value}}, "invalid subfield {{subfield.title}}");
//%         elif subfield.optional
if ({{field.attr}}->{{subfield.src.enable.getter}}()) {
    {{unpack_subfield(field, subfield)}}
}
//%         else
{{unpack_subfield(field, subfield)}}
//%         endif
//%     endfor
//% endmacro

//% macro packet_impl(packet)
using {{namespace}}::packing::{{packet.helper}};

bool {{packet.helper}}::match(const void* ptr, size_t bufsize)
{
    vrtgen::InputBuffer buffer(ptr, bufsize);
    const {{packet.header.type}}* header = reinterpret_cast<const {{packet.header.type}}*>(buffer.getHeader());
//% for field in packet.header.fields
    if (header->{{field.getter}}() != {{field.value}}) {
        return false;
    }
//% endfor
//% if packet.class_id
//%     set field = packet.class_id
//%     for subfield in field.fields if subfield.value:
    if (buffer.get{{field.name}}()->get{{subfield.name}}() != {{subfield.value}}) {
        return false;
    }
//%     endfor
//% endif
    return true;
}

//# Suppress warnings by only naming the argument if there are optional fields
//% if packet.is_variable_length
//%     set varname = 'packet'
//% else
//%     set varname = '/*unused*/'
//% endif
size_t {{packet.helper}}::bytes_required(const {{packet.name}}& {{varname}})
{
    size_t bytes = sizeof({{packet.header.type}});
//% for field in packet.prologue
    bytes += sizeof({{field.type}});
//% endfor
//% for cif in packet.cifs if cif.enabled
    bytes += sizeof({{cif.header}});
//% endfor
//% for field in packet.fields
//%     if field.optional
    if (packet.has{{field.name}}()) {
        bytes += sizeof({{field.type}});
    }
//%     else
    bytes += sizeof({{field.type}});
//%     endif
//% endfor
    return bytes;
}

void {{packet.helper}}::pack(const {{packet.name}}& packet, void* ptr, size_t bufsize)
{
    vrtgen::OutputBuffer buffer(ptr, bufsize);
    {{packet.header.type}}* header = buffer.insert<{{packet.header.type}}>();
//% for field in packet.header.fields
    header->{{field.setter}}({{field.value}});
//% endfor

//% for field in packet.prologue
    {{pack_field(field) | indent(4) | trim}}
//% endfor
//% for cif in packet.cifs if cif.enabled
//%     if cif.number != 0
    cif_0->setCIF{{cif.number}}Enable(true);
//%     endif
//%     if not packet.fields
    buffer.insert<{{cif.header}}>();
//%     else
    {{cif.header}}* cif_{{cif.number}} = buffer.insert<{{cif.header}}>();
//%     endif
//% endfor
//% for field in packet.fields
//%     if field.optional
    if (packet.has{{field.name}}()) {
        cif_{{field.cif}}->set{{field.name}}Enabled(true);
        {{pack_field(field)}}
    }
//%     else
    cif_{{field.cif}}->set{{field.name}}Enabled(true);
    {{pack_field(field) | indent(4) | trim}}
//%     endif
//% endfor
    header->setPacketSize(buffer.size() / 4);
}

void {{packet.helper}}::unpack({{packet.name}}& packet, const void* ptr, size_t bufsize)
{
    vrtgen::InputBuffer buffer(ptr, bufsize);
    const {{packet.header.type}}* header = reinterpret_cast<const {{packet.header.type}}*>(buffer.getHeader());
//% for field in packet.header.fields
    ::validate(header->{{field.getter}}(), {{field.value}}, "invalid header field {{field.title}}");
//% endfor

//% for field in packet.prologue
//%     if field.struct
    const {{field.type}}* {{field.attr}} = buffer.get{{field.name}}();
    {{unpack_struct(field) | indent(4) | trim}}
//%     else
    packet.set{{field.name}}(buffer.get{{field.name}}());
//%     endif
//% endfor
//% for cif in packet.cifs if cif.enabled
    const {{cif.header}}* cif_{{cif.number}} = buffer.getCIF{{cif.number}}();
    ::validate(cif_{{cif.number}}, "CIF{{cif.number}} missing");
//% endfor
//% for field in packet.fields
//%     set cifvar = "cif_{}".format(field.cif)
    if (!{{cifvar}}->is{{field.name}}Enabled()) {
//%     if field.optional
        packet.clear{{field.name}}();
//%     else
        // ERROR
//%     endif
    } else {
        {{unpack_field(field) | indent(8) | trim}}
    }
//% endfor
}
//% endmacro

#include "{{header}}"

#include <stdexcept>

namespace {
    static inline void validate(bool actual, bool expected, const char* msg)
    {
        if (actual != expected) {
            throw std::runtime_error(msg);
        }
    }

    static inline void validate(bool cond, const char* msg)
    {
        validate(cond, true, msg);
    }
}

//% for packet in packets
{{packet_impl(packet)}}
//% endfor
