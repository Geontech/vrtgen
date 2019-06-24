//% macro define_struct(struct)
/**
//% for line in struct.doc
 * ${line}
//% endfor
 */
struct ${struct.name} {
/*{% if struct.reserved %}*/
    ${struct.name}() :
/*{%    for member in struct.reserved %}*/
        ${member.name}(0)${',' if not loop.last}
/*{%    endfor %}*/
    {
    }

/*{% endif %}*/
/*{% for field in struct.fields %}*/
//%     set member = field.member
    /**
     * ${field.getter.doc}.
     */
    ${field.type} ${field.getter.name}() const
    {
//%     if field.tag
        return ${member.name}.get(${field.tag}());
//%     elif field.bits % 8 == 0
        return ${member.name}.get();
//%     else
        return ${member.name};
//%     endif
    }

    /**
     * ${field.setter.doc}.
     */
    void ${field.setter.name}(${field.type} value)
    {
//%     if field.tag
        ${member.name}.set(value, ${field.tag}());
//%     elif field.bits % 8 == 0
        ${member.name}.set(value);
//%     else
        ${member.name} = value;
//%     endif
    }

/*{% endfor %}*/
private:
/*{% for member in struct.members %}*/
    /**
//%      for line in member.doc
     * ${line}
//%      endfor
     */
//%      for tag in member.tags
    typedef ${tag.type} ${tag.name};
//%      endfor
    ${member.decl};
/*{%     if not loop.last %}*/

/*{%     endif %}*/
/*{% endfor %}*/
};
//% endmacro
//% set guard = '_VRTGEN_PACKING_' + name.upper() + '_HPP_'
#ifndef ${guard}
#define ${guard}

#include <vrtgen/types.hpp>
#include <vrtgen/enums.hpp>

namespace vrtgen {
    namespace packing {
/*{% for struct in structs %}*/
/*{%     if not loop.first %}*/

/*{%     endif %}*/
        ${define_struct(struct)|indent(8)}
/*{% endfor %}*/
    }
}

#endif // ${guard}
