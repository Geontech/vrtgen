from v49gen.model.packets import *

import sys

class IndentedWriter:
    def __init__(self, stream, indent=4):
        self._stream = stream
        self._indent = indent
        self._prefix = ''

    def write(self, text):
        if self._prefix:
            self._stream.write(self._prefix)
        self._stream.write(text)
        self._stream.write('\n')

    def push(self):
        self._prefix += ' ' * self._indent

    def pop(self):
        self._prefix = self._prefix[:-self._indent]

def docstring_to_comment(doc):
    if not doc:
        return
    yield '/**'
    if doc.startswith('\n'):
        doc = doc[1:]
    indent = 0
    while doc[indent].isspace():
        indent += 1
    for line in doc.rstrip().split('\n'):
        yield ' * ' + line[indent:]
    yield ' */'

def generate_enum(stream, enum, name=None, format=None):
    stream.write('')
    if name is None:
        name = enum.__name__
    stream.write("namespace {} {{".format(name))
    stream.push()
    for line in docstring_to_comment(enum.__doc__):
        stream.write(line)
    stream.write("enum Code {")
    stream.push()
    if format is None:
        format = "{}"
    fmt = "{} = " + format + ","
    for item in enum:
        stream.write(fmt.format(item.name, item.value))
    stream.pop()
    stream.write("};")
    stream.pop()
    stream.write("}")

def name_to_identifier(name):
    identifier = ''
    for ch in name:
        if ch.isalnum():
            identifier += ch
    return identifier

def generate_struct(stream, structdef, name=None):
    stream.write('')
    if name is None:
        name = structdef.__name__
    stream.write('struct {} {{'.format(name))
    stream.push()
    stream.write('uint32_t word;')
    stream.write('};')
    for attr, field in structdef.get_field_descriptors():
        identifier = name_to_identifier(field.name)
        bit = field.enable_bit
        stream.write('')

        stream.write("bool get{}() const".format(identifier));
        stream.write('{')
        stream.push()
        stream.write('return GET_BIT(word, {});'.format(bit))
        stream.pop()
        stream.write('}')

        stream.write('')
        stream.write("void set{}(bool enable)".format(identifier));
        stream.write('{')
        stream.push()
        stream.write('SET_BIT(word, {}, enable);'.format(bit))
        stream.pop()
        stream.write('}')
    stream.pop()
    stream.write('};')

def main():
    with open('packing.hpp', 'w') as fp:
        stream = IndentedWriter(fp)
        stream.write('#ifndef _VRTGEN_PACKING_HPP_')
        stream.write('#define _VRTGEN_PACKING_HPP_')
        stream.write('')
        stream.write('#include <inttypes.h>')
        stream.write('')
        stream.write('#define SET_BIT(x,bit,enable)')
        stream.write('#define GET_BIT(x,bit) false')
        stream.write('namespace vrtgen {')
        stream.push()
        generate_enum(stream, PacketType, format="0b{:04b}")
        generate_enum(stream, TSI, format="0b{:02b}")
        generate_enum(stream, TSF, format="0b{:02b}")
        generate_enum(stream, SSI, format="0b{:02b}")
        stream.write('namespace packing {')
        stream.push()
        generate_struct(stream, CIF0, name='CIF0Prologue')
        generate_struct(stream, CIF1, name='CIF1Prologue')
        stream.pop()
        stream.write('}')
        stream.pop()
        stream.write('}')
        stream.write('#endif // _VRTGEN_PACKING_HPP_')

if __name__ == '__main__':
    main()
