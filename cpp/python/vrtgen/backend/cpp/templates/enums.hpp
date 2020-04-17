/*
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
 */

//% macro define_enum(enum)
namespace {{enum.name}} {
    /**
//% for line in enum.doc
     * {{line}}
//% endfor
     */
    enum Code {
//% for label, value in enum['values']
        {{label}} = {{value}},
//% endfor
    };
}
//%- endmacro
#pragma once

namespace vrtgen {
//% for enum in enums
/*% if not loop.first %*/

/*% endif %*/
    {{define_enum(enum)|indent(4)}}
//% endfor
}
