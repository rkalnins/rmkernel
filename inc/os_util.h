#pragma once

#include <stdint.h>

/**
 * @brief Copies memory from source to destination
 *
 * @param dest
 * @param src
 * @param len
 * @return void*
 */
extern void* os_memcpy(void* dest, const void* src, uint32_t len);
