//
class ${packet.name} {
public:
  ${packet.name}()
  {
  }

//% for field in packet.prologue.fields
    // ${field.name}
//% endfor

//% for field in packet.payload
  // ${field.name} getter
  ${field.type} get${field.identifier}() const
  {
/*{%     if field.optional %}*/
    return *${field.member.identifier};
/*{%     else %}*/
    return ${field.member.identifier};
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
    return ${field.member.identifier}.get();
  }

/*{%     endif %}*/
/*{% endfor %}*/
private:
//% for field in packet.payload
  ${field.member.type} ${field.member.identifier};
//% endfor
};
