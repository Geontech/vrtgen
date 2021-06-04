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
//% macro pack_subfield(field, subfield)
{{field.attr}}->{{subfield.src.setter}}(packet.get{{field.identifier}}().{{subfield.src.getter}}());
//% endmacro

//% macro pack_struct(field)
{{field.type}}* {{field.attr}} = buffer.insert<{{field.type}}>();
//%     for subfield in field.subfields
//%         if subfield.value is defined
{{field.attr}}->{{subfield.src.setter}}({{subfield.value}});
//%         elif subfield.optional
if (packet.get{{field.identifier}}().{{subfield.src.enable.getter}}()) {
    {{field.attr}}->{{subfield.src.enable.setter}}(true);
    {{pack_subfield(field, subfield) | trim}}
}
//%         else
{{pack_subfield(field, subfield) | trim}}
//%         endif
//%     endfor
//% endmacro

//% macro pack_field(field)
//%     if field.struct
{{pack_struct(field) | trim}}
//%     else
buffer.insert<{{field.type}}>(packet.get{{field.identifier}}());
//%     endif
//% endmacro

//% macro unpack_field(field)
//%     if field.struct
const {{field.type}}* {{field.attr}} = buffer.next<{{field.type}}>();
packet.set{{field.identifier}}(*{{field.attr}});
//%     else
//%         if field.identifier == 'ContextFieldChangeIndicator'
packet.set{{field.identifier}}(true);
//%         else
packet.set{{field.identifier}}(buffer.get<{{field.type}}>());
//%         endif
//%     endif
//% endmacro

//% macro unpack_subfield(field, subfield)
packet.{{subfield.setter}}({{field.attr}}->{{subfield.src.getter}}());
//% endmacro

//% macro unpack_struct(field)
//%     for subfield in field.subfields
//%         if subfield.value is defined
::validate({{field.attr}}->{{subfield.src.getter}}(), {{subfield.value}}, "invalid subfield {{subfield.title}}");
//%         elif subfield.optional
if ({{field.attr}}->{{subfield.src.enable.getter}}()) {
    {{unpack_subfield(field, subfield) | trim}}
}
//%         else
{{unpack_subfield(field, subfield) | trim}}
//%         endif
//%     endfor
//% endmacro

//% macro pack_cam(cam)
{{cam.type}}* {{cam.attr}} = buffer.insert<{{cam.type}}>();
//% for field in cam.fields
{{cam.attr}}->{{field.setter}}({{field.value}});
//% endfor
//% endmacro

//% macro unpack_cam(cam)
const {{cam.type}}* {{cam.attr}} = buffer.get{{cam.name}}();
//% for field in cam.fields
::validate({{cam.attr}}->{{field.getter}}(), {{field.value}}, "invalid CAM field {{field.title}}");
//% endfor
//% endmacro

//% macro pack_prologue(packet)
//% for field in packet.prologue.fields if not field.post_cam
{{pack_field(field) | trim}}
//% endfor
//% if packet.cam
{{pack_cam(packet.cam) | trim}}
//% endif
//% for field in packet.prologue.fields if field.post_cam
//%     if 'UUID' in field.member.type
{{field.type}}* {{field.identifier.lower()}} = buffer.insert<{{field.type}}>();
{{field.identifier.lower()}}->set({{field.member.type}}(packet.get{{field.identifier}}()).value());
//%     else
{{pack_field(field) | trim}}
//%     endif
//% endfor
//% endmacro

//% macro unpack_prologue(packet)
//% for field in packet.prologue.fields if not field.post_cam
//%     if field.struct
const {{field.type}}* {{field.attr}} = buffer.get{{field.identifier}}();
{{unpack_struct(field) | trim}}
//%     else
packet.set{{field.identifier}}(buffer.get{{field.identifier}}());
//%     endif
//% endfor
//% if packet.cam
{{unpack_cam(packet.cam) | trim}}
//% endif
//% for field in packet.prologue.fields if field.post_cam
//%     if field.struct
const {{field.type}}* {{field.attr}} = buffer.get{{field.identifier}}();
{{unpack_struct(field) | trim}}
//%     elif 'UUID' in field.member.type
//%         set identifier = field.identifier[:-2] + 'UU' + field.identifier[-2:]
packet.set{{field.identifier}}(buffer.get{{identifier}}());
//%     else
packet.set{{field.identifier}}(buffer.get{{field.identifier}}());
//%     endif
//% endfor
//% endmacro

//% macro define_match(packet)
//% set prologue = packet.prologue
bool {{packet.helper}}::match(const void* ptr, size_t bufsize)
{
    vrtgen::InputBuffer buffer(ptr, bufsize);
    const {{packet.header.type}}* header = reinterpret_cast<const {{packet.header.type}}*>(buffer.getHeader());
//% for field in packet.header.fields
    if (header->{{field.getter}}() != {{field.value}}) {
        return false;
    }
//% endfor
//% if packet.cam
    const {{packet.cam.type}}* {{packet.cam.attr}} = buffer.get{{packet.cam.name}}();
//%     for field in packet.cam.fields
    if ({{packet.cam.attr}}->{{field.getter}}() != {{field.value}}) {
        return false;
    }
//%     endfor
//% endif
    return true;
}
//% endmacro

//% macro define_bytes_required(packet)
//# Suppress warnings by only naming the argument if there are optional fields
//% if packet.is_variable_length and not packet.is_reqs
//%     set varname = 'packet'
//% else
//%     set varname = '/*unused*/'
//% endif
size_t {{packet.helper}}::bytes_required(const {{packet.name}}& {{varname}})
{
    size_t bytes = sizeof({{packet.header.type}});
//% for field in packet.prologue.fields
    bytes += sizeof({{field.type}});
//% endfor
//% if packet.cam
    bytes += sizeof({{packet.cam.type}});
//% endif
//% for cif in packet.cifs if cif.enabled
    bytes += sizeof({{cif.header}});
//%     if not packet.is_reqs
//%         for field in cif.fields 
//%             if field.identifier == 'ContextFieldChangeIndicator'
//%                 do continue
//%             elif field.optional
    if (packet.has{{field.identifier}}()) {
        bytes += sizeof({{field.type}});
    }
//%             else
    bytes += sizeof({{field.type}});
//%             endif
//%         endfor
//%     endif
//% endfor
    return bytes;
}
//% endmacro

//% macro define_pack(packet)
void {{packet.helper}}::pack(const {{packet.name}}& packet, void* ptr, size_t bufsize)
{
    vrtgen::OutputBuffer buffer(ptr, bufsize);
    {{packet.header.type}}* header = buffer.insert<{{packet.header.type}}>();
//% for field in packet.header.fields
    header->{{field.setter}}({{field.value}});
//% endfor
    {{pack_prologue(packet) | indent(4) | trim}}
//% set has_mult_cif = []
//% for cif in packet.cifs if cif.enabled
//%     if cif.number != 0
//%         do has_mult_cif.append(true)
//%     endif
//% endfor
//% for cif in packet.cifs if cif.enabled
//%     if cif.number == 0
//%         if has_mult_cif or cif.fields
    {{cif.header}}* cif_{{cif.number}} = buffer.insert<{{cif.header}}>();
//%         else
    buffer.insert<{{cif.header}}>();
//%         endif
//%     else
    cif_0->setCIF{{cif.number}}Enable(true);
    {{cif.header}}* cif_{{cif.number}} = buffer.insert<{{cif.header}}>();
//%     endif
//% endfor
//% for cif in packet.cifs if cif.enabled
//%     for field in cif.fields
//%         if field.optional
//%             if packet.is_reqs
    cif_{{cif.number}}->set{{field.identifier}}Enabled(packet.is{{field.identifier}}Enabled());
//%             else
    if (packet.has{{field.identifier}}()) {
//%                 if field.identifier == 'ContextFieldChangeIndicator'
        cif_{{cif.number}}->set{{field.identifier}}(packet.get{{field.identifier}}());
//%                 else
        cif_{{cif.number}}->set{{field.identifier}}Enabled(true);
        {{pack_field(field) | indent(8) | trim}}
//%                 endif
    }
//%             endif
//%         else
    cif_{{cif.number}}->set{{field.identifier}}Enabled(true);
    {{pack_field(field) | indent(4) | trim}}
//%         endif
//%     endfor
//% endfor 
//% if packet.is_data
    std::memcpy(buffer.get(), packet.getPayload(), packet.getPayloadSize());
//% endif
    header->setPacketSize(buffer.size() / 4);
}
//% endmacro

//% macro define_unpack(packet)
void {{packet.helper}}::unpack({{packet.name}}& packet, const void* ptr, std::size_t bufsize)
{
    vrtgen::InputBuffer buffer(ptr, bufsize);
    const {{packet.header.type}}* header = reinterpret_cast<const {{packet.header.type}}*>(buffer.getHeader());
//% for field in packet.header.fields
    ::validate(header->{{field.getter}}(), {{field.value}}, "invalid header field {{field.title}}");
//% endfor
    {{unpack_prologue(packet) | indent(4) | trim}}
//% for cif in packet.cifs if cif.enabled
//%     set cifvar = 'cif_{}'.format(cif.number)
    const {{cif.header}}* {{cifvar}} = buffer.getCIF{{cif.number}}();
    ::validate({{cifvar}}, "CIF{{cif.number}} missing");
//%     for field in cif.fields if field.member
//%         if packet.is_reqs
    packet.set{{field.identifier}}Enabled({{cifvar}}->is{{field.identifier}}Enabled());
//%         else
//%             if field.identifier == 'ContextFieldChangeIndicator'
    if (!{{cifvar}}->get{{field.identifier}}()) {
//%             else
    if (!{{cifvar}}->is{{field.identifier}}Enabled()) {
//%             endif
//%             if field.optional
        packet.clear{{field.identifier}}();
//%             else
        // ERROR
//%             endif
    } else {
        {{unpack_field(field) | indent(8) | trim}}
    }
//%         endif   
//%     endfor
//% endfor
//% if packet.is_data
    packet.setPayload(static_cast<const uint8_t*>(ptr)+buffer.size(), bufsize-buffer.size());
//% endif
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
using {{namespace}}::packing::{{packet.helper}};

{{define_match(packet) | trim}}

{{define_bytes_required(packet) | trim}}

{{define_pack(packet) | trim}}

{{define_unpack(packet) | trim}}
//% endfor