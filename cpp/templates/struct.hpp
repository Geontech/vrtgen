//% macro define_struct(struct)
/**
//% for line in struct.doc
 * ${line}
//% endfor
 */
struct ${struct.name} {
    vrtgen::word_t word[${struct.words}];
/*{% for field in struct.fields %}*/

    /**
     * ${field.getter.doc}.
     */
    ${field.type} ${field.getter.name}() const
    {
//% if field.type == 'bool'
        return GET_BIT32(word[${field.word}], ${field.offset});
//% else
        return (${field.type}) vrtgen::get_int(word[${field.word}], ${field.offset}, ${field.bits});
//% endif
    }

    /**
     * ${field.setter.doc}.
     */
    void ${field.setter.name}(${field.type} value)
    {
//% if field.type == 'bool'
        SET_BIT32(word[${field.word}], ${field.offset}, value);
//% else
        vrtgen::set_int(word[${field.word}], ${field.offset}, ${field.bits}, value);
//% endif
    }
/*{% endfor %}*/
};
//% endmacro
//% set guard = '_VRTGEN_PACKING_' + name.upper() + '_HPP_'
#ifndef ${guard}
#define ${guard}

#include <vrtgen/types.hpp>
#include <vrtgen/enums.hpp>
#include <vrtgen/utils/macros.hpp>

namespace vrtgen {
    namespace packing {
/*{% for struct in structs %}*/
/*{% if not loop.first %}*/

/*{% endif %}*/
        ${define_struct(struct)|indent(8)}
/*{% endfor %}*/
    }
}

#endif // ${guard}
