#ifndef _VRTGEN_MACROS_HPP
#define _VRTGEN_MACROS_HPP

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
// Big-endian, bit position is pass-through.
#define _BIT_POS32(b) (b)
#else
// Little-endian, swap the byte offset. This expression is expected to be used
// with constant values for b, where the final value is calculated at compile
// time.
#define _BIT_POS32(b) ((8*(3-(b/8)))+(b&0x7))
#endif
#define GET_BIT32(w,b) ((w >> _BIT_POS(b)) & 1)
#define SET_BIT32(w,b) (w |= (1 << _BIT_POS(b)))

#endif // _VRTGEN_MACROS_HPP
