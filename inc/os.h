#ifndef OS_H
#define OS_H

#include "os_defs.h"
#include "os_util.h"

#include "os_mem.h"
#include "os_msg.h"

/**
 * @brief Macro to declare an AO with a message queue and message queue buffer
 *
 */
#define ACTIVE_OBJECT_EXTERN(name, queue_size)  \
    extern ActiveObject_t name;                 \
    extern MessageQueue_t name##_message_queue; \
    extern MessageGeneric_t name##_message_queue_buffer[queue_size];

#define ACTIVE_OBJECT_DECL(name, queue_size) \
    ActiveObject_t name;                     \
    MessageQueue_t name##_message_queue;     \
    MessageGeneric_t name##_message_queue_buffer[queue_size];

/**
 * @brief Macro to create a message queue and active object
 *
 */
#define AO_INIT(name, priority, handler, size)                                \
    MsgQueueCreate(&name##_message_queue, size, name##_message_queue_buffer); \
    ActiveObjectCreate(&name, priority, &name##_message_queue, handler);

/**
 * @brief State of the Active Object
 *
 */
typedef enum ActiveObjectState_e
{
    AO_ACTIVE = 0, //!< Currently in execution
    AO_READY, //!< In ready list
    AO_WAITING, //!< Not queued, not in ready list
} ActiveObjectState_t;

/**
 * @brief Configuration for callbacks provided to OS
 *
 */
struct OSCallbacksCfg_s
{
    void (*on_SysTick)(void); //!< Hooked to end of SysTick_Handler
    void (*on_Idle)(void); //!< Hooked to scheduler idle loop
    void (*on_Init)(void); //!< Hooked to end of KernelInit
};

/**
 * @brief OS instance data
 *
 */
struct OS_s
{
    uint32_t time; //!< current tick time, incremented with SysTick_Handler
    uint16_t current_prio; //!< current active AO priority
    uint8_t nesting; //!< scheduler nesting count, increment on ISR enter, decrement on exit
    void (*on_SysTick)(void); //!< Hooked to end of SysTick_Handler
    void (*on_Idle)(void); //!< Hooked to scheduler idle loop
    void (*on_Init)(void); //!< Hooked to end of KernelInit
};

/**
 * @brief Timed event types
 *
 */
typedef enum TimedEventType_e
{
    TIMED_EVENT_SINGLE_TYPE = 0, //!< run once
    TIMED_EVENT_PERIODIC_TYPE //!< run periodically
} TimedEventType_t;

/**
 * @brief Simple Timed Event instance.
 *        Timed events run either once or periodically and on execution,
 *        dispatch a message to destination AO
 *
 */
struct TimedEventSimple_s
{
    ActiveObject_t* dest; //!< active object receiving dispatched message
    void* message; //!< message to dispatch
    uint32_t period; //!< delay or period of dispatch (ms)
    uint32_t count; //!< current count since last dispatch
    TimedEventType_t type; //!< single or periodic
    TimedEventSimple_t* next; //!< next event in list
};

/**
 * @brief Active Object is an encapsulation for all data.
 *
 */
struct ActiveObject_s
{
    MessageQueue_t* msg_queue; //!< Incoming message queue
    ActiveObjectState_t state; //!< current state of AO
    EventHandler_f handler; //!< Event/message handler
    uint8_t priority; //!< task priority 0-255
    ActiveObject_t* next; //!< next AO in queue
    ActiveObject_t* prev; //!< prev AO in queue
};

/**
 * @brief Get system time (ms)
 *
 * @return uint32_t
 */
extern uint32_t OSGetTime();

/**
 * @brief Initializes the kernel, sets up instance with callbacks
 *
 * @param os Pointer to the OS instance
 * @param callback_cfg Pointer to the callbacks
 */
extern void KernelInit(OS_t* os, OSCallbacksCfg_t* callback_cfg);

/**
 * @brief Creates an Active Object
 *
 * @param ao
 * @param priority
 * @param queue
 * @param handler
 */
extern void ActiveObjectCreate(ActiveObject_t* ao, uint8_t priority, MessageQueue_t* queue,
                               EventHandler_f handler);

/**
 * @brief Start the scheduler, does not return.
 *
 * If nothing is currently running, enters idle loop
 *
 */
extern void SchedulerRun();

/**
 * @brief Determines if there is an AO to activate. Called on ISR exit to set PendSV
 *
 * @return int
 */
extern int Schedule();

/**
 * @brief Activates the first active object in queue
 *
 */
extern void SchedulerActivateAO();

/**
 * @brief Adds the AO to the queue
 *
 * @param ao
 */
extern void SchedulerAddReady(ActiveObject_t* ao);

/**
 * @brief Creates a simple time event
 *
 * @param event
 * @param dest
 * @param msg
 * @param period
 * @param type
 */
extern void TimedEventSimpleCreate(TimedEventSimple_t* event, ActiveObject_t* dest, void* msg,
                                   uint32_t period, TimedEventType_t type);

/**
 * @brief Schedules a timed event
 *
 * @param event
 */
extern void SchedulerAddTimedEvent(TimedEventSimple_t* event);

#endif