//
class ${packet.name} {
public:
  ${packet.name}() :
//% for field in packet.fields if field.value is defined and not field.const
    ${field.member.identifier}(${field.value})${"," if not loop.last}
//% endfor
  {
  }

//% for field in packet.fields
  // ${field.name} getter
  ${field.type} get${field.identifier}() const
  {
/*{%     if field.const %}*/
    return ${field.value};
/*{%     else %}*/
    return ${field.member.identifier}${".get()" if field.optional};
/*{%     endif %}*/
  }

/*{%     if not field.const %}*/
  // ${field.name} setter
  void set${field.identifier}(${field.type} value)
  {
    ${field.member.identifier} = value;
  }

/*{%     endif %}*/
/*{%     if field.optional %}*/
  bool has${field.identifier}() const
  {
    return ${field.member.identifier}.is_set();
  }

/*{%     endif %}*/
/*{% endfor %}*/
private:
//% for field in packet.fields if not field.const
  ${field.member.type} ${field.member.identifier};
//% endfor
};
