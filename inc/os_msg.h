#ifndef OS_MSG_H
#define OS_MSG_H

#include "os_defs.h"

/**
 * @brief Queue status
 *
 */
typedef enum MessageQueueStatus_e
{
    MSG_Q_SUCCESS = 0,
    MSG_Q_ERROR,
    MSG_Q_FULL //!< no space left
} MessageQueueStatus_t;

struct MessageGeneric_s
{
    uint8_t placeholder[OS_MESSAGE_MAX_SIZE];
};

/**
 * @brief Message to be sent to an AO's queue
 *
 */
struct Message_s
{
    uint32_t id; //!< message id
    uint8_t msg_size; //<! length of message
};

struct DataMessage_s
{
    Message_t base;
    uint32_t timestamp; //!< message timestamp
    uint32_t data; //!< message data
};

struct MemoryBlockMessage_s
{
    Message_t base;
    uint16_t key;
    uint8_t size;
};

/**
 * @brief Queue for messages, each AO should have one
 *
 */
struct MessageQueue_s
{
    MessageGeneric_t* queue; //!< buffer
    volatile uint16_t head; //!< index
    volatile uint16_t tail; //!< index
    uint16_t size; //!< buffer size
    bool is_full;
};

/**
 * @brief
 *
 * @param q
 * @return true if queue is empty
 * @return false if queue has space
 */
extern bool MsgQueueIsEmpty(MessageQueue_t* q);

/**
 * @brief Creates message queue
 *
 * @param q
 * @param size
 * @param queue
 */
extern void MsgQueueCreate(MessageQueue_t* q, const uint16_t size, MessageGeneric_t* queue);

/**
 * @brief Adds message to queue
 *
 * @param dest
 * @param msg
 * @return MessageQueueStatus_t
 */
extern MessageQueueStatus_t MsgQueuePut(ActiveObject_t* dest, void* msg);

/**
 * @brief Gets the next message from the queue, ONLY BLOCKING FUNCTION IN OS
 *
 * @param ao
 * @return Message_t*
 */
extern void* MsgQueueGet(ActiveObject_t* ao);

#endif