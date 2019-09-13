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
