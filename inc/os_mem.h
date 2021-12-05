#pragma once

#include "os_defs.h"

#define MEMORY_BLOCK_32  32
#define MEMORY_BLOCK_64  64
#define MEMORY_BLOCK_128 128
#define MEMORY_BLOCK_256 256

typedef uint32_t BlockSize_t;

/**
 * @brief Get a key to a block of pre-allocated memory
 *
 * @param key used to access the memory block useing OSMemoryBlockGet
 * @param size number of bytes to get (see BlockSize_t)
 * @param status OS_MEMORY_SUCCESS if able to find a free block, OS_MEMORY_BLOCK_FULL otherwise
 * @return uint8_t* pointer to block of memory
 */
extern uint8_t* OSMemoryBlockNew(uint16_t* key, BlockSize_t size, OSStatus_t* status);

/**
 * @brief Gets a pointer to the block of memory encoded in the key
 *
 * @param key
 * @return uint8_t* pointer to the block of memory
 */
extern uint8_t* OSMemoryBlockGet(uint16_t key);

/**
 * @brief Frees a block of memory so that it can be used again.
 *
 * @param key
 * @return OSStatus_t OS_SUCCESS
 */
extern OSStatus_t OSMemoryFreeBlock(uint16_t key);
