//% set guard = "_VRTGEN_PACKING_" + name + "_HPP_"
#ifndef ${guard}
#define ${guard}

#include <inttypes.h>

#define SET_BIT(x,bit,v)
#define GET_BIT(x,bit) false

namespace vrtgen {
    namespace packing {
        struct ${name} {
            uint32_t word;
/*{% for field in fields %}*/

            bool get${field.name}() const {
                return GET_BIT(word, ${field.bit});
            }

            void set${field.name}(bool enable) {
                SET_BIT(word, ${field.bit}, enable);
            }
/*{% endfor %}*/
        };
    }
}

#endif // ${guard}
