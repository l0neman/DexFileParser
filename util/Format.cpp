#include "Format.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

// TODO: 念中野割 0。

char const * Format::binary_8bit(uint8_t n)
{
    char buffer[8 + 1];
    memset(buffer, 0, 8 + 1);
    itoa(n, buffer, 2);
    return buffer;
}

char const* Format::binary_16bit(uint16_t n)
{
    char buffer[16 + 1];
    memset(buffer, 0, 16 + 1);
    itoa(n, buffer, 2);
    return buffer;
}

char const* Format::binary_32bit(uint32_t n)
{
    char buffer[32 + 1];
    memset(buffer, 0, 32 + 1);
    itoa(n, buffer, 2);
    return buffer;
}

char const* Format::binary_64bit(uint64_t n)
{
    char buffer[64 + 1];
    memset(buffer, 0, 64 + 1);
    itoa(n, buffer, 2);
    return buffer;
}
 