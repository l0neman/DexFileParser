#ifndef LEB128_H
#define LEB128_H

#include <cstdint>

class Leb128
{
public:
    static uint32_t decode_unsigned_leb128(uint8_t const** data);
    static uint32_t decode_unsigned_leb128_p1(uint8_t const** data);
    static uint32_t decode_signed_leb128(uint8_t const** data);
    static uint32_t unsigned_leb238_size(uint32_t data);
};

#endif // !LEB128_H
