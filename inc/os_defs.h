#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef OS_TRACE_ENABLED
    #define DEBUG_PRINT_IS_QUEUE  1
    #define DEBUG_PRINT_IS_HANDLE 0
#endif

#define OS_BASEPRI 0x3F

//! OS return codes
#define OS_SUCCESS           0
#define OS_INVALID_ARGUMENT  1
#define OS_MEMORY_BLOCK_FULL 2
#define OS_ERROR             3

#define OS_MESSAGE_MAX_SIZE 20
#define OS_EVENT_LOG_MSG_ID 999

// clang-format off
#define ENABLE_INTERRUPTS() __asm volatile ("cpsie i" ::: "memory");
#define DISABLE_INTERRUPTS() __asm volatile ("cpsid i" ::: "memory");

/**
 * @brief see ARM errata 838869, a store immediate with offset at the end of an ISR could
 *  lead to incorrect interrupt handling. DSB is like a "flush" for pending data that needs
 *  to be written in the write buffer. ARM considers this problem rare but could happen
 */
#define ERRATUM() __asm volatile("dsb" ::: "memory")

// clang-format on

#ifndef UNUSED
    #define UNUSED(X) (void)(X)
#endif

//! compile time assertion
#define STATIC_ASSERT(X)                                                                           \
    do                                                                                             \
    {                                                                                              \
        extern int __attribute__((error("assertion failure: '" #X "' not true")))                  \
        compile_time_check();                                                                      \
        int tmp = ((X) ? 0 : compile_time_check());                                                \
        UNUSED(tmp);                                                                               \
    } while (false);

/**
 * @brief Macro to be called upon entering an ISR
 *
 * More of a temporary placeholder for future development right now
 *
 */
#define OS_ISR_ENTER(os)                                                                           \
    {                                                                                              \
    }

/**
 * @brief Macro to be called upon exiting an ISR
 *
 * Sets PendSV bit if an AO has been queued during ISR handling
 *
 */
#define OS_ISR_EXIT(os)                                                                            \
    {                                                                                              \
        DISABLE_INTERRUPTS();                                                                      \
        if (0U != Schedule())                                                                      \
        {                                                                                          \
            *((uint32_t*)(0xE000ED04U)) = (1U << 28U);                                             \
        }                                                                                          \
        ENABLE_INTERRUPTS();                                                                       \
        ERRATUM();                                                                                 \
    }

typedef uint8_t OSStatus_t;

//! see os.h
typedef struct OS_s OS_t;

//! see os.h
typedef struct OSCallbacksCfg_s OSCallbacksCfg_t;

//! see os_msg.h
typedef struct MessageQueue_s MessageQueue_t;

//! see os_msg.h
typedef struct MessageGeneric_s     MessageGeneric_t;
typedef struct Message_s            Message_t;
typedef struct DataMessage_s        DataMessage_t;
typedef struct MemoryBlockMessage_s MemoryBlockMessage_t;

//! see os.h
typedef struct ActiveObject_s ActiveObject_t;

//! see os.h
typedef struct TimedEventSimple_s TimedEventSimple_t;

/**
 * @brief Event handler. Must run to completion. No blocking allowed!! Should run quickly
 *
 */
typedef void (*EventHandler_f)(Message_t*);
