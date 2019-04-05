#ifndef _VRTGEN_PACKING_TRAILER_HPP_
#define _VRTGEN_PACKING_TRAILER_HPP_

#include <vrtgen/types.hpp>
#include <vrtgen/enums.hpp>
#include <vrtgen/utils/macros.hpp>

namespace vrtgen {
    namespace packing {
        struct Trailer {
            vrtgen::word_t word;
/*{% for field in enables %}*/

            /**
             * Gets enabled state for ${field.name}.
             */
            bool get${field.identifier}Enable() const
            {
                return GET_BIT32(word, ${field.enable_bit});
            }

            /**
             * Sets enabled state for ${field.name}.
             */
            void set${field.identifier}Enable(bool enable) {
                SET_BIT32(word, ${field.enable_bit}, enable);
            }
/*{% endfor %}*/
/*{% for field in fields %}*/

            /**
             * Gets current value of ${field.name}.
             */
            bool get${field.identifier}() const
            {
                return GET_BIT32(word, ${field.position});
            }

            /**
             * Sets current value of ${field.name}.
             */
            void set${field.identifier}(bool value)
            {
                SET_BIT32(word, ${field.position}, value);
            }
/*{% endfor %}*/

            vrtgen::SSI::Code getSampleFrame() const
            {
                return vrtgen::SSI::SINGLE;
            }

            void setSampleFrame(vrtgen::SSI::Code value)
            {
                // TODO
            }

            uint8_t getAssociatedContextPacketCount() const
            {
                return 0;
            }

            void setAssociatedContextPacketCount(uint8_t value)
            {
                // TODO
            }
        };
    }
}

#endif // _VRTGEN_PACKING_TRAILER_HPP_
