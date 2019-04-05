//% set guard = "_VRTGEN_PACKING_" + name + "_HPP_"
#ifndef ${guard}
#define ${guard}

#include <vrtgen/types.hpp>
#include <vrtgen/utils/macros.hpp>

namespace vrtgen {
    namespace packing {
        struct ${name} {
            vrtgen::word_t word;
/*{% for field in fields %}*/

            bool get${field.name}Enable() const {
                return GET_BIT32(word, ${field.bit});
            }

            void set${field.name}Enable(bool enable) {
                SET_BIT32(word, ${field.bit}, enable);
            }
/*{% endfor %}*/
        };
    }
}

#endif // ${guard}
