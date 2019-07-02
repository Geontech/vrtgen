//% macro packet_impl(packet)
using {{namespace}}::packing::{{packet.helper}};

size_t {{packet.helper}}::bytes_required(const {{packet.name}}& packet)
{
    size_t bytes = sizeof({{packet.header.type}});
//% if packet.has_stream_id
    bytes += sizeof(vrtgen::StreamIdentifier);
//% endif            
//% if packet.class_id
    bytes += sizeof(vrtgen::packing::ClassIdentifier);
//% endif
//% if packet.has_integer_timestamp
    bytes += sizeof(vrtgen::packing::IntegerTimestamp);
//% endif
//% if packet.has_fractional_timestamp
    bytes += sizeof(vrtgen::packing::FractionalTimestamp);
//% endif
//% for cif in packet.cifs
//%     if cif.optional
    if (has_cif{{cif.number}}(packet)) {
        bytes += sizeof(vrtgen::packing::{{cif.header}});
    }
//%     else
    bytes += sizeof(vrtgen::packing::{{cif.header}});
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
const vrtgen::packing::{{cif.header}}* {{packet.helper}}::get_cif{{cif.number}}(vrtgen::InputBuffer& buffer, const vrtgen::packing::CIF0Enables* cif0)
{
    if (cif0->getCIF{{cif.number}}Enable()) {
        return buffer.next<vrtgen::packing::{{cif.header}}>();
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

//% if packet.has_stream_id
    buffer.put<vrtgen::packing::StreamIdentifier>(packet.getStreamIdentifier());
//% endif
//% if packet.class_id
    vrtgen::packing::ClassIdentifier* class_id = buffer.insert<vrtgen::packing::ClassIdentifier>();
//%     for field in packet.class_id
//%         if field.value
    class_id->set{{field.name}}({{field.value}});
//%         else
    class_id->set{{field.name}}(packet.get{{field.name}}());
//% endif
//%     endfor
//% endif
//% if packet.has_integer_timestamp
    buffer.put<vrtgen::packing::IntegerTimestamp>(packet.getIntegerTimestamp());
//% endif
//% if packet.has_fractional_timestamp
    buffer.put<vrtgen::packing::FractionalTimestamp>(packet.getFractionalTimestamp());
//% endif
//% for cif in packet.cifs if cif.enabled
    vrtgen::packing::{{cif.header}}* cif_{{cif.number}} = nullptr;
//%     if cif.optional
    if (has_cif{{cif.number}}(packet)) {
        cif_0->setCIF{{cif.number}}Enable(true);
        cif_{{cif.number}} = buffer.insert<vrtgen::packing::{{cif.header}}>();
    }
//%     else
    cif_{{cif.number}} = buffer.insert<vrtgen::packing::{{cif.header}}>();
//%     endif
//% endfor
//% for field in packet.fields
//%     if field.optional
    if (packet.has{{field.name}}()) {
        cif_{{field.cif}}->set{{field.name}}Enabled(true);
        buffer.put<{{field.type}}>(packet.get{{field.name}}());
    }
//%     else
    cif_{{field.cif}}->set{{field.name}}Enabled(true);
    buffer.put<{{field.type}}>(packet.get{{field.name}}());
//%     endif
//% endfor
    header->setPacketSize(buffer.size() / 4);
}

void {{packet.helper}}::unpack({{packet.name}}& packet, const void* ptr, size_t bufsize)
{
    vrtgen::InputBuffer buffer(ptr, bufsize);
    const {{packet.header.type}}* header = buffer.next<{{packet.header.type}}>();
//% for field in packet.header.fields
    if (header->{{field.getter}}() != {{field.value}}) {
        // ERROR
    }
//% endfor

//% if packet.has_stream_id
    packet.setStreamIdentifier(buffer.get<vrtgen::packing::StreamIdentifier>());
//% endif
//% if packet.class_id
    const vrtgen::packing::ClassIdentifier* class_id = buffer.next<vrtgen::packing::ClassIdentifier>();
//%     for field in packet.class_id
//%         if field.value
    if (class_id->get{{field.name}}() != {{field.value}}) {
        // ERROR
    }
//%         else
    packet.set{{field.name}}(class_id->get{{field.name}}());
//%         endif
//%     endfor
//% endif
//% if packet.has_integer_timestamp
    packet.setIntegerTimestamp(buffer.get<vrtgen::packing::IntegerTimestamp>());
//% endif
//% if packet.has_fractional_timestamp
    packet.setFractionalTimestamp(buffer.get<vrtgen::packing::FractionalTimestamp>());
//% endif
//% for cif in packet.cifs if cif.enabled
//%     if cif.number == 0
    const vrtgen::packing::{{cif.header}}* cif_{{cif.number}} = buffer.next<vrtgen::packing::{{cif.header}}>();
//%     else
    const vrtgen::packing::{{cif.header}}* cif_{{cif.number}} = get_cif{{cif.number}}(buffer, cif_0);
//%         if not cif.optional
    if (!cif_{{cif.number}}) {
        // ERROR
    }
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
        packet.set{{field.name}}(buffer.get<{{field.type}}>());
    }
//% endfor
}
//% endmacro
#include "{{header}}"

//% for packet in packets
{{packet_impl(packet)}}
//% endfor