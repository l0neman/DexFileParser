#include "Leb128.h"

#include <iostream>

#ifndef _HEY_I_AM_RUNNING_STATIC_ANALYZER
#define __builtin_expect(e,c) (e)
#endif

#define LIKELY( exp )    (__builtin_expect( (exp) != 0, true ))
#define UNLINKELY( exp ) (__builtin_expect( (exp) != 0, false ))

uint32_t Leb128::decode_signed_leb128(uint8_t const** data)
{
    auto ptr = *data;
    int32_t result = *(ptr++);
    if (result <= 0x7f)
    {
        result = (result << 25) >> 25;
    }
    else
    {
        int cur = *(ptr++);
        result = (result & 0x7f) | ((cur & 0x7f) << 7);
        if (cur <= 0x7f)
        {
            result = (result << 18) >> 18;
        }
        else
        {
            cur = *(ptr++);
            result |= (cur & 0x7f) << 14;
            if (cur <= 0x7f)
            {
                result = (result << 11) >> 11;
            }
            else
            {
                cur = *(ptr++);
                result |= (cur & 0x7f) << 21;
                if (cur <= 0x7f)
                {
                    result = (result << 4) >> 4;
                }
                else
                {
                    // Note: We don't check to see if cur is out of range here,
                    // meaning we tolerate garbage in the four high-order bits.
                    cur = *(ptr++);
                    result |= cur << 28;
                }
            }
        }
    }

    *data = ptr;
    return result;
}

uint32_t Leb128::decode_unsigned_leb128(uint8_t const** data)
{
    auto ptr = *data;
    int result = *(ptr++);
    if (LIKELY(result > 0x7f))
    {
        int cur = *(ptr++);
        result = (result & 0x7f) | ((cur & 0x7f) << 7);
        if (cur > 0x7f)
        {
            cur = *(ptr++);
            result |= (cur & 0x7f) << 14;
            if (cur > 0x7f)
            {
                cur = *(ptr++);
                result |= (cur & 0x7f) << 21;
                if (cur > 0x7f)
                {
                    // Note: We don't check to see if cur is out of range here,
                    // meaning we tolerate garbage in the four high-order bits.
                    cur = *(ptr++);
                    result |= cur << 28;
                }
            }
        }
    }

    *data = ptr;
    return static_cast<uint32_t>(result);
}

uint32_t Leb128::decode_unsigned_leb128_p1(uint8_t const** data)
{
    return decode_unsigned_leb128(data) - 1;
}

uint32_t Leb128::unsigned_leb128_size(uint32_t data)
{
    auto count = 0;
    do
    {
        data >>= 7;
        count++;
    } while (data != 0);

    return count;
}

uint32_t __inline builtin_clz(uint32_t value) {
    unsigned long leading_zero = 0;
    if (_BitScanReverse(&leading_zero, value))
        return 31 - leading_zero;
    return 32;
}

uint32_t __inline builtin_clzll(uint64_t value) {
    unsigned long leading_zero = 0;
    if (_BitScanReverse64(&leading_zero, value))
        return 63 - leading_zero;
    return 64;
}

template < typename T>
  static constexpr int CLZ(T x) {
      return (sizeof(T) == sizeof(uint32_t)) ?
      builtin_clz(x) : builtin_clzll(x);
}

int32_t Leb128::signed_leb128_size(uint32_t data)
{
    data = data ^ (data >> 31);
    uint32_t x = 1 + 6 + 32 - CLZ(data | 1U);
    return (x * 37) >> 8;
}