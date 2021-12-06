#pragma once

#include "os_defs.h"

#define COMMAND_ON_END_INSTANT      0x0
#define COMMAND_ON_END_WAIT_FOR_END 0x1

typedef struct Command_s Command_t;

struct Command_s
{
    void (*on_Start)(Command_t* cmd, void* instance_data);
    bool (*on_Message)(Command_t* cmd, Message_t* msg, void* instance_data);
    void (*on_End)(Command_t* cmd, void* instance_data);
    uint32_t   end_behavior;
    Command_t* next;
};

typedef struct StateMachine_s
{
    Command_t* start;
    Command_t* current;
} StateMachine_t;

extern void StateMachineInit(StateMachine_t* sm, Command_t* start);

extern void StateMachineStart(StateMachine_t* sm, void* instance_data);

extern bool StateMachineStep(StateMachine_t* sm, Message_t* msg, void* instance_data);
