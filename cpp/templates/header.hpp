#ifndef _VRTGEN_PACKING_HEADER_HPP_
#define _VRTGEN_PACKING_HEADER_HPP_

#include <vrtgen/types.hpp>
#include <vrtgen/enums.hpp>
#include <vrtgen/utils/macros.hpp>

namespace vrtgen {
    namespace packing {
        struct Header {
            vrtgen::word_t word;
/*{% for field in fields %}*/

            /**
             * Get ${field.doc}.
             */
            ${field.type} get${field.identifier}() const
            {
//% if field.type == 'bool'
                return GET_BIT32(word, ${field.position});
//% else
                // TODO
                return (${field.type})0;
//% endif
            }

            /**
             * Set ${field.doc}.
             */
            void set${field.identifier}(${field.type} value)
            {
//% if field.type == 'bool'
                SET_BIT32(word, ${field.position}, value);
//% else
                // TODO
//% endif
            }
/*{% endfor %}*/
        };
    }
}

#endif // _VRTGEN_PACKING_HEADER_HPP_
