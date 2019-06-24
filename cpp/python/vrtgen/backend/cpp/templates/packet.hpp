//
class ${packet.name} {
public:
    ${packet.name}() :
//% for field in packet.fields if field.value is defined and not field.const
        ${field.member.identifier}(${field.value})${"," if not loop.last}
//% endfor
    {
    }

    //% for field in packet.members
    /**
     * ${field.name} getter
     */
    ${field.type} get${field.identifier}() const
    {
        return ${field.member};
    }

    /**
     * ${field.name} setter
     */
    void set${field.identifier}(${field.type} value)
    {
        ${field.member} = value;
    }

/*{% endfor %}*/
private:
    friend class ${packet.namespace}::packing::${packet.name}Helper;

//% for member in packet.members
    ${member.type} ${member.member};
//% endfor
};

namespace packing {
    struct ${packet.name}Helper {
        static size_t bytes_required(const ${packet.name}& packet)
        {
            return 0;
        }

        static void pack(const ${packet.name}& packet, void* buffer, size_t bufsize)
        {
            vrtgen::packing::Header* header = reinterpret_cast<vrtgen::packing::Header*>(buffer);
            // TODO
        }

        static void unpack(${packet.name}& packet, const uint8_t* buffer, size_t bufsize)
        {
            const vrtgen::packing::Header* header = reinterpret_cast<const vrtgen::packing::Header*>(buffer);
            // TODO
        }
    };
}