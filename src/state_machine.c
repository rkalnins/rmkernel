#include "state_machine.h"

extern void StateMachineInit(StateMachine_t* sm, Command_t* start)
{
    sm->start = start;
    sm->current = start;
}

extern void StateMachineStart(StateMachine_t* sm, void* instance_data)
{
    if (!sm || !sm->current)
    {
        return;
    }

    sm->current->on_Start(sm->current, instance_data);

    // instant commands hanve no steps involved
    // immediately go to next state and start
    if (COMMAND_ON_END_INSTANT == sm->current->end_behavior)
    {
        sm->current = sm->current->next;
        StateMachineStart(sm, instance_data);
    }
}

extern bool StateMachineStep(StateMachine_t* sm, Message_t* msg, void* instance_data)
{
    bool is_done = true;
    if (sm->current->on_Message)
    {
        // handle current message
        is_done = sm->current->on_Message(sm->current, msg, instance_data);
    }

    if (is_done)
    {
        if (sm->current->on_End)
        {
            sm->current->on_End(sm->current, instance_data);
        }

        // go to next state
        sm->current = sm->current->next;

        // if the next command is NULL, we've reached the end of this state machine
        if (sm->current)
        {
            StateMachineStart(sm, instance_data);
        }
        else
        {
            // true if current command is done
            return true;
        }
    }

    // false if current command is not done (sequential states)
    return false;
}
