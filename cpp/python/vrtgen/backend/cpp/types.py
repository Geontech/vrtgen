def name_to_identifier(name):
    identifier = ''
    for ch in name:
        if ch.isalnum():
            identifier += ch
        elif ch in '.':
            identifier += '_'
    return identifier

def int_type(bits, signed):
    if bits > 32:
        ctype = 'int64_t'
    elif bits > 16:
        ctype = 'int32_t'
    elif bits > 8:
        ctype = 'int16_t'
    else:
        ctype = 'int8_t'
    if not signed:
        ctype = 'u' + ctype
    return ctype

def enum_type(datatype):
    name = name_to_identifier(datatype.__name__)
    return 'vrtgen::{}::Code'.format(name)

def float_type(bits):
    if bits >= 32:
        return 'double'
    else:
        return 'float'
