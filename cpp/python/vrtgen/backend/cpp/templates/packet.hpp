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

//% from "struct.hpp" import define_struct

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
//% for field in packet.members
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
//% for structdef in packet.structs
    {{define_struct(structdef)|indent(4)}}

/*% endfor %*/
    friend struct {{packet.namespace}}::packing::{{packet.helper}};

//% for field in packet.members
    {{field.member.type}} {{field.member.identifier}};
//% endfor
};

namespace packing {
    struct {{packet.helper}} {
        static bool match(const void* ptr, size_t length);

        static size_t bytes_required(const {{packet.name}}& packet);

        static void pack(const {{packet.name}}& packet, void* ptr, size_t bufsize);

        static void unpack({{packet.name}}& packet, const void* ptr, size_t bufsize);
    };
}
//%- endmacro
