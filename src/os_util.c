/**
 * @file os_util.c
 */

#include "inc/os_util.h"

#include <stdint.h>

extern void* os_memcpy(void* dest, const void* src, uint32_t len)
{
    uint8_t*       d = (uint8_t*)dest;
    const uint8_t* s = (uint8_t*)src;

    while (len--)
    {
        *d++ = *s++;
    }

    return dest;
}
