//% macro define_packet(packet)
namespace packing {
    class {{packet.name}}Helper;
}

/**
//% for line in packet.doc
 * {{line}}
//% endfor
 */
class {{packet.name}} {
public:
    {{packet.name}}() :
//% for field in packet.members if not field.optional
        {{field.member.identifier}}({{field.value}}){{"," if not loop.last}}
//% endfor
    {
    }

//% for field in packet.members
//%     if field.optional
    /**
     * Check {{field.name}} presence
     */
    bool has{{field.identifier}}() const
    {
        return static_cast<bool>({{field.member.identifier}});
    }

    /**
     * Clear {{field.name}} presence
     */
    void clear{{field.identifier}}()
    {
        {{field.member.identifier}}.clear();
    }

//%     endif
    /**
     * {{field.name}} getter
     */
    {{field.type}} get{{field.identifier}}() const
    {
//%     if field.optional
        return {{field.member.identifier}}.get();
//%     else
        return {{field.member.identifier}};
//%     endif
    }

    /**
     * {{field.name}} setter
     */
    void set{{field.identifier}}({{field.type}} value)
    {
//%     if field.optional
        {{field.member.identifier}}.set(value);
//%     else
        {{field.member.identifier}} = value;
//%     endif
    }

//% endfor
private:
    friend class {{packet.namespace}}::packing::{{packet.name}}Helper;

//% for field in packet.members
    {{field.member.type}} {{field.member.identifier}};
//% endfor
};

namespace packing {
    struct {{packet.name}}Helper {
        static size_t bytes_required(const {{packet.name}}& packet)
        {
            size_t bytes = sizeof(vrtgen::packing::Header);
//% if packet.has_stream_id
            bytes += sizeof(vrtgen::StreamIdentifier);
//% endif            
//% if packet.has_class_id
            bytes += sizeof(vrtgen::packing::ClassIdentifier);
//% endif
            return bytes;
        }

        static void pack(const {{packet.name}}& packet, void* ptr, size_t bufsize)
        {
            vrtgen::OutputBuffer buffer(ptr, bufsize);
            {{packet.header.type}}* header = buffer.insert<{{packet.header.type}}>();
//% for field in packet.header.fields
            header->{{field.setter}}({{field.value}});
//% endfor

//% if packet.has_stream_id
            buffer.put(packet.getStreamIdentifier());
//%endif
//% if packet.has_class_id
            buffer.insert<vrtgen::packing::ClassIdentifier>();
//% endif
//% if packet.has_integer_timestamp
            buffer.put(packet.getIntegerTimestamp());
//%endif
//% if packet.has_fractional_timestamp
            buffer.put(packet.getFractionalTimestamp());
//%endif
//% if packet.fields
            vrtgen::packing::CIF0Enables* cif_0 = buffer.insert<vrtgen::packing::CIF0Enables>();
//% endif
            header->setPacketSize(buffer.getpos() / 4);
        }

        static void unpack({{packet.name}}& packet, const void* ptr, size_t bufsize)
        {
            vrtgen::InputBuffer buffer(ptr, bufsize);
            const {{packet.header.type}}* header = buffer.next<{{packet.header.type}}>();
//% for field in packet.header.fields
            if (header->{{field.getter}}() != {{field.value}}) {
                // ERROR
            }
//% endfor

//% if packet.has_stream_id
            packet.setStreamIdentifier(buffer.get<vrtgen::StreamIdentifier>());
//%endif
//% if packet.has_class_id
            buffer.next<vrtgen::packing::ClassIdentifier>();
//% endif
//% if packet.has_integer_timestamp
            packet.setIntegerTimestamp(buffer.get<uint32_t>());
//%endif
//% if packet.has_fractional_timestamp
            packet.setFractionalTimestamp(buffer.get<uint64_t>());
//%endif
//%if packet.fields
            const vrtgen::packing::CIF0Enables* cif_0 = buffer.next<vrtgen::packing::CIF0Enables>();
//% endif
        }
    };
}
//%- endmacro