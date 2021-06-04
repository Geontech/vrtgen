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

//% from "struct.hpp" import define_struct

//% macro define_constructor(packet)
//% set fields = []
//% for cif in packet.cifs if cif.enabled
//%     for field in cif.fields if field.member
//%         do fields.append(field)
//%     endfor
//% endfor
{{packet.name}}() :
//% for field in packet.prologue.fields if field.member
    {{field.member.identifier}}({{field.value}}){{',' if not loop.last or fields}}
//% endfor
//% for field in fields
//%     if packet.is_reqs
    {{field.member.identifier}}Enabled({{'false' if field.optional else 'true'}}){{',' if not loop.last}}
//%     else
    {{field.member.identifier}}({{field.value}}){{',' if not loop.last}}
//%     endif
//% endfor
{
}
//% endmacro

//% macro define_getter(field)
{{field.member.datatype}} get{{field.identifier}}() const
{
//%     if field.optional or 'UUID' in field.member.type
    return {{field.member.identifier}}.get();
//%     else
    return {{field.member.identifier}};
//%     endif
}
//% endmacro

//% macro define_setter(field)
void set{{field.identifier}}(const {{field.member.datatype}}& value)
{
//%     if field.optional or 'UUID' in field.member.type
    {{field.member.identifier}}.set(value);
//%     else
    {{field.member.identifier}} = value;
//%     endif
}
//% endmacro

//% macro define_has(field)
bool has{{field.identifier}}() const
{
    return static_cast<bool>({{field.member.identifier}});
}
//% endmacro

//% macro define_clear(field)
void clear{{field.identifier}}()
{
    {{field.member.identifier}}.clear();
}
//% endmacro

//% macro create_field_functions(packet)
//% set fields = []
//% for field in packet.prologue.fields if field.member
/**
 * {{field.identifier}} getter/setter
 */
{{define_getter(field) | trim}}

{{define_setter(field) | trim}}

//% endfor
//% for cif in packet.cifs if cif.enabled
//%     for field in cif.fields if field.member
//%         if packet.is_reqs
/**
 * {{field.identifier}} enabled getter/setter
 */
bool is{{field.identifier}}Enabled() const
{
    return {{field.member.identifier}}Enabled;
}

void set{{field.identifier}}Enabled(const bool value)
{
    {{field.member.identifier}}Enabled = value;
}

//%         else
//%             if field.optional
{{define_has(field) | trim}}

{{define_clear(field) | trim}}

//%             endif
{{define_getter(field) | trim}}

{{define_setter(field) | trim}}

//%         endif
//%     endfor
//% endfor
//% if packet.is_data
const uint8_t* getPayload() const
{
    return m_payload.data();
}

size_t getPayloadSize() const
{
    return m_payload.size();
}

void setPayload(const void* data, size_t len)
{
    m_payload.resize(len);
    std::memcpy(m_payload.data(), data, len);
}

//% endif
//% endmacro

//% macro define_members(packet)
//% set fields = []
//% for field in packet.prologue.fields if field.member
{{field.member.type}} {{field.member.identifier}};
//% endfor
//% for cif in packet.cifs if cif.enabled
//%     for field in cif.fields if field.member
//%         if packet.is_reqs
bool {{field.member.identifier}}Enabled;
//%         else
{{field.member.type}} {{field.member.identifier}};
//%         endif
//%     endfor
//% endfor
//% if packet.is_data
std::vector<uint8_t> m_payload;
//% endif
//% endmacro

//% macro define_packet(packet)
/**
//% if not packet.doc
 * class {{packet.name}}
//% else
//%     for line in packet.doc
 * {{line}}
//%     endfor
//% endif
 */
class {{packet.name}}
{
public:
    using helper = {{packet.namespace}}::packing::{{packet.helper}};

    {{define_constructor(packet) | indent(4) | trim}}

    {{create_field_functions(packet) | indent(4) | trim}}

private:
    friend helper;

    {{define_members(packet) | indent(4)}}
}; // end class {{packet.name}}
//%- endmacro
