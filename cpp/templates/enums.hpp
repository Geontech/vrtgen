#ifndef _VRTGEN_ENUMS_HPP_
#define _VRTGEN_ENUMS_HPP_

namespace vrtgen {
//% for enum in enums

    namespace ${enum.name} {
        /**
//% for line in enum.doc
         * ${line}
//% endfor
         */
        enum Code {
//% for item in enum['values']
            ${item.name} = ${enum.format(item.value)},
//% endfor
        };
    }
//% endfor
}

#endif // _VRTGEN_ENUMS_HPP_
