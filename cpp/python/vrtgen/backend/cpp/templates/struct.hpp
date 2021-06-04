/*
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
 */

//% macro define_struct(struct)
/**
//% for line in struct.doc
 * {{line}}
//% endfor
 */
struct {{struct.name}} {
/*% if struct.reserved %*/
    {{struct.name}}() :
/*%    for member in struct.reserved %*/
        {{member.name}}(0){{',' if not loop.last}}
/*%    endfor %*/
    {
    }

/*% endif %*/
/*% for field in struct.fields %*/
//%     set member = field.member
    /**
     * {{field.getter.doc}}.
     */
    {{field.type}} {{field.getter.name}}() const
    {
//%     if field.tag
        return {{member.name}}.get({{field.tag}}());
//%     elif field.bits % 8 == 0
        return {{member.name}}.get();
//%     else
        return {{member.name}};
//%     endif
    }

    /**
     * {{field.setter.doc}}.
     */
    void {{field.setter.name}}({{field.type}} value)
    {
//%     if field.tag
        {{member.name}}.set(value, {{field.tag}}());
//%     elif field.bits % 8 == 0
        {{member.name}}.set(value);
//%     else
        {{member.name}} = value;
//%     endif
    }

/*% endfor %*/
    /**
     * Checks that all fields have valid values.
     * 
     * Returns the number of invalid fields.
     */
    int validate() const
    {
        int errors = 0;
/*% for field in struct.reserved %*/
        if ({{field.name}} != 0) {
            ++errors;
        }
/*% endfor %*/
        return errors;
    }

private:
/*% for member in struct.members %*/
    /**
//%      for line in member.doc
     * {{line}}
//%      endfor
     */
//%      for tag in member.tags
    typedef {{tag.type}} {{tag.name}};
//%      endfor
    {{member.decl}};
/*%     if not loop.last %*/

/*%     endif %*/
/*% endfor %*/
}; // end struct {{struct.name}}
//%- endmacro
#pragma once

#include <vrtgen/types.hpp>
#include <vrtgen/enums.hpp>

namespace vrtgen {
    namespace packing {
//% for struct in structs
/*%     if not loop.first %*/

/*%     endif %*/
        {{define_struct(struct)|indent(8)}}
//% endfor
    }
}
