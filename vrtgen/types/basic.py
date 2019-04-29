class IntegerType:
    def __init_subclass__(cls, bits, **kwds):
        super().__init_subclass__(**kwds)
        cls.bits = bits

    @classmethod
    def create(cls, bits):
        name = 'Integer{:d}'.format(bits)
        return type(name, (cls,), {}, bits=bits)

Integer64 = IntegerType.create(64)
Integer32 = IntegerType.create(32)
Integer24 = IntegerType.create(24)
Integer16 = IntegerType.create(16)
Integer8 = IntegerType.create(8)

class FixedPointType:
    def __init_subclass__(cls, bits, radix, **kwds):
        super().__init_subclass__(**kwds)
        cls.bits = bits
        cls.radix = radix

    @classmethod
    def create(cls, bits, radix):
        name = 'FixedPoint{:d}_{:d}'.format(bits, radix)
        return type(name, (cls,), {}, bits=bits, radix=radix)

FixedPoint64_20 = FixedPointType.create(64, 20)
FixedPoint32_16 = FixedPointType.create(32, 16)
FixedPoint16_7 = FixedPointType.create(16, 7)
FixedPoint16_13 = FixedPointType.create(16, 13)

Identifier32 = Integer32
Identifier16 = Integer16

StreamIdentifier = Integer32

class OUI(IntegerType, bits=24):
    pass
