//% macro define_packet(packet)
/**
//% for line in packet.doc
 * {{line}}
//% endfor
 */
class {{packet.name}} {
public:
    {{packet.name}}() :
//% for field in packet.fields if field.value is defined and not field.const
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
        return {{field.member.identifier}};
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
            return 0;
        }

        static void pack(const {{packet.name}}& packet, void* buffer, size_t bufsize)
        {
            vrtgen::packing::Header* header = reinterpret_cast<vrtgen::packing::Header*>(buffer);
            // TODO
        }

        static void unpack({{packet.name}}& packet, const uint8_t* buffer, size_t bufsize)
        {
            const vrtgen::packing::Header* header = reinterpret_cast<const vrtgen::packing::Header*>(buffer);
            // TODO
        }
    };
}
//%- endmacro