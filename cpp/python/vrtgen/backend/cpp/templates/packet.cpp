//% macro pack_field(field)
//%     if field.struct
{{field.type}}* {{field.attr}} = buffer.insert<{{field.type}}>();
//%         for subfield in field.fields
//%             if subfield.value
{{field.attr}}->set{{subfield.name}}({{subfield.value}});
//%             else
{{field.attr}}->set{{subfield.name}}(packet.get{{subfield.name}}());
//%             endif
//%         endfor
//%     else
buffer.put<{{field.type}}>(packet.get{{field.name}}());
//%     endif
//% endmacro

//% macro unpack_field(field)
//%     if field.struct:
const {{field.type}}* {{field.attr}} = buffer.next<{{field.type}}>();
//%         for subfield in field.fields
//%             if subfield.value
::validate({{field.attr}}->get{{subfield.name}}(), {{subfield.value}}, "invalid subfield {{subfield.title}}");
//%             else
packet.set{{subfield.name}}({{field.attr}}->get{{subfield.name}}());
//%             endif
//%         endfor
//%     else
packet.set{{field.name}}(buffer.get<{{field.type}}>());
//%     endif
//% endmacro

//% macro packet_impl(packet)
using {{namespace}}::packing::{{packet.helper}};

bool {{packet.helper}}::match(const void* ptr, size_t bufsize)
{
    vrtgen::InputBuffer buffer(ptr, bufsize);
    const {{packet.header.type}}* header = buffer.next<{{packet.header.type}}>();
//% for field in packet.header.fields
    if (header->{{field.getter}}() != {{field.value}}) {
        return false;
    }
//% endfor
    return true;
}

size_t {{packet.helper}}::bytes_required(const {{packet.name}}& packet)
{
    size_t bytes = sizeof({{packet.header.type}});
//% for field in packet.prologue
    bytes += sizeof({{field.type}});
//% endfor
//% for cif in packet.cifs
//%     if cif.optional
    if (has_cif{{cif.number}}(packet)) {
        bytes += sizeof({{cif.header}});
    }
//%     else
    bytes += sizeof({{cif.header}});
//%     endif
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

//% for cif in packet.cifs[1:] if cif.optional
/**
 * Checks whether @a packet has any CIF{{cif.number}} fields set.
 */
bool {{packet.helper}}::has_cif{{cif.number}}(const {{packet.name}}& packet)
{
//%     for field in packet.fields if field.cif == cif.number and field.optional
    if (packet.has{{field.name}}()) {
        return true;
    }
//%     endfor
    return false;
}

//% endfor
//% for cif in packet.cifs[1:]
const {{cif.header}}* {{packet.helper}}::get_cif{{cif.number}}(vrtgen::InputBuffer& buffer, const vrtgen::packing::CIF0Enables* cif0)
{
    if (cif0->getCIF{{cif.number}}Enable()) {
        return buffer.next<{{cif.header}}>();
    }
    return nullptr;
}

//% endfor
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
    {{cif.header}}* cif_{{cif.number}} = nullptr;
//%     if cif.optional
    if (has_cif{{cif.number}}(packet)) {
        cif_0->setCIF{{cif.number}}Enable(true);
        cif_{{cif.number}} = buffer.insert<{{cif.header}}>();
    }
//%     else
//%         if cif.number != 0
    cif_0->setCIF{{cif.number}}Enable(true);
//%         endif
    cif_{{cif.number}} = buffer.insert<{{cif.header}}>();
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
    const {{packet.header.type}}* header = buffer.next<{{packet.header.type}}>();
//% for field in packet.header.fields
    ::validate(header->{{field.getter}}(), {{field.value}}, "invalid header field {{field.title}}");
//% endfor

//% for field in packet.prologue
    {{unpack_field(field) | indent(4) | trim}}
//% endfor
//% for cif in packet.cifs if cif.enabled
//%     if cif.number == 0
    const {{cif.header}}* cif_{{cif.number}} = buffer.next<{{cif.header}}>();
//%     else
    const {{cif.header}}* cif_{{cif.number}} = get_cif{{cif.number}}(buffer, cif_0);
//%         if not cif.optional
    ::validate(cif_{{cif.number}}, "CIF{{cif.number}} missing");
//%         endif
//%     endif
//% endfor
//% for field in packet.fields
//%     set cifvar = "cif_{}".format(field.cif)
    if (!{{cifvar}} || !{{cifvar}}->is{{field.name}}Enabled()) {
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