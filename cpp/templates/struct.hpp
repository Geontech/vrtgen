//% set guard = '_VRTGEN_PACKING_' + name.upper() + '_HPP_'
#ifndef ${guard}
#define ${guard}

#include <vrtgen/types.hpp>
#include <vrtgen/enums.hpp>
#include <vrtgen/utils/macros.hpp>

namespace vrtgen {
    namespace packing {
        struct ${name} {
            vrtgen::word_t word;
/*{% for field in fields %}*/

            /**
             * ${field.getter.doc}.
             */
            ${field.type} ${field.getter.name}() const
            {
//% if field.type == 'bool'
                return GET_BIT32(word, ${field.position});
//% else
                return (${field.type}) vrtgen::get_int(word, ${field.position}, ${field.bits});
//% endif
            }

            /**
             * ${field.setter.doc}.
             */
            void ${field.setter.name}(${field.type} value)
            {
//% if field.type == 'bool'
                SET_BIT32(word, ${field.position}, value);
//% else
                vrtgen::set_int(word, ${field.position}, ${field.bits}, value);
//% endif
            }
/*{% endfor %}*/
        };
    }
}

#endif // ${guard}
