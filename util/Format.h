#ifndef FORMAT_H_
#define FORMAT_H_

#include <cstdint>

class Format
{
public:
    static char const* binary_8bit(uint8_t n);
    static char const* binary_16bit(uint16_t n);
    static char const* binary_32bit(uint32_t n);
    static char const* binary_64bit(uint64_t n);
};

#endif // !FORMAT_H_
