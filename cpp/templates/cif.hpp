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

//% from "struct.hpp" import define_struct
#pragma once

#include <vrtgen/types.hpp>

namespace vrtgen {
    namespace packing {
//% for struct in structs
/*%     if not loop.first %*/

/*%     endif %*/
        {{define_struct(struct)|indent(8)}}
//% endfor
//% for typedef in typedefs
/*%     if loop.first %*/

/*%     endif %*/
        typedef {{typedef.type}} {{typedef.name}};
//% endfor
    }
}
