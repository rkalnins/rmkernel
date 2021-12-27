#pragma once

#include "os_defs.h"

#define COMMAND_ON_END_INSTANT      0x0
#define COMMAND_ON_END_WAIT_FOR_END 0x1

/**
 * @brief Base command instance data
 */
typedef struct Command_s Command_t;

struct Command_s
{
    void (*on_Start)(Command_t* cmd, void* instance_data);
    bool (*on_Message)(Command_t* cmd, Message_t* msg, void* instance_data);
    void (*on_End)(Command_t* cmd, void* instance_data);
    uint32_t   end_behavior;
    Command_t* next;
};

/**
 * @brief State machine instance data
 */
typedef struct StateMachine_s
{
    Command_t* start;
    Command_t* current;
} StateMachine_t;

/**
 * @brief Initialize state machine instance data
 * 
 * @param sm State machine instance
 * @param start First command
 */
extern void StateMachineInit(StateMachine_t* sm, Command_t* start);

/**
 * @brief Start the state machine at the first commmand
 *
 * @param sm State machine instance
 * @param instance_data
 */
extern void StateMachineStart(StateMachine_t* sm, void* instance_data);

/**
 * @brief Step through the state machine using the give message
 *
 * @param sm State machine instance
 * @param msg Current message
 * @paramm instance_data
 */
extern bool StateMachineStep(StateMachine_t* sm, Message_t* msg, void* instance_data);
