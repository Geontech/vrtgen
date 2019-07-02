//% macro define_packet(packet)
namespace packing {
    struct {{packet.helper}};
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
    friend struct {{packet.namespace}}::packing::{{packet.helper}};

//% for field in packet.members
    {{field.member.type}} {{field.member.identifier}};
//% endfor
};

namespace packing {
    struct {{packet.helper}} {
        static size_t bytes_required(const {{packet.name}}& packet);

//% for cif in packet.cifs[1:] if cif.optional
        /**
         * Checks whether @a packet has any CIF{{cif.number}} fields set.
         */
        static bool has_cif{{cif.number}}(const {{packet.name}}& packet);

//% endfor
//% for cif in packet.cifs[1:]
        static const vrtgen::packing::{{cif.header}}* get_cif{{cif.number}}(vrtgen::InputBuffer& buffer, const vrtgen::packing::CIF0Enables* cif0);

//% endfor
        static void pack(const {{packet.name}}& packet, void* ptr, size_t bufsize);

        static void unpack({{packet.name}}& packet, const void* ptr, size_t bufsize);
    };
}
//%- endmacro