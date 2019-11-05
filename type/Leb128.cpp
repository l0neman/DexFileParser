#include "Leb128.h"

#ifndef _HEY_I_AM_RUNNING_STATIC_ANALYZER
#define __builtin_expect(e,c) (e)
#endif

#define LIKELY( exp )    (__builtin_expect( (exp) != 0, true ))
#define UNLINKELY( exp ) (__builtin_expect( (exp) != 0, false ))

uint32_t Leb128::decode_signed_leb128(uint8_t const** data)
{
    const uint8_t* ptr = *data;
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
    const uint8_t* ptr = *data;
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


