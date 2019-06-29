//% macro define_enum(enum)
namespace {{enum.name}} {
    /**
//% for line in enum.doc
     * {{line}}
//% endfor
     */
    enum Code {
//% for item in enum['values']
        {{item.name}} = {{enum.format(item.value)}},
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
