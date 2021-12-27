# Realtime Micro Kernel

See the University of Michigan Fall 2021 EECS 373 final project [WarehouseProject-EECS373/zumo-controller](https://github.com/WarehouseProject-EECS373/zumo-controller) for usage example.

In the example project, a bit of a different source structure is used, specifically

- See `src/os_port_arm_m4.c` for borrowed and slightly modified code from Quantum Leap's QP-nano (GPLv3)
- `src/app_defs.h` contains message definitions
- `src/main.c` contains active object declarations

Currently, `os_port_arm_m4.c` is `ports/arm-cortex-m4/port.c`.

## Features

- Active Objects
  - Message queues
  - Variable sized, custom messages
- Periodic and single timed events
- Memory pools
- Command-based hierarchical state machine framework
  - Commands
  - Instant commands

## Usage

### Active Objects

```cpp
// app_definitions.h: global definitions
#include <os.h>

#define OBJECT_QUEUE_SIZE 16
#define OBJECT_ID 0
#define OBJECT_PRIORITY 1

ACTIVE_OBJECT_EXTERN(example_object, OBJECT_QUEUE_SIZE)
```

```cpp
// main.c
#include "app_definitions.h"

ACTIVE_OBJECT_DECL(example_object, OBJECT_QUEUE_SIZE)

void ObjectEventHandler(Message_t *msg)
{  
}

int main()
{
    AO_INIT(example_object, OBJECT_PRIORITY, ObjectEventHandler, OBJECT_QUEUE_SIZE, OBJECT_ID);
}
```

### Custom Messages and Message Queues

```cpp
// app_definitions.h: global definitions

#define EXAMPLE_MSG_ID  0x100

typedef struct ExampleMessage_s
{
    Messasge_t base;
    uint32_t data;
} ExampleMessage_t;

```

```cpp
// isr_example.c
#include "app_definitions.h"
#include <os.h>
#include <os_msg.h>


void ExampleISR()
{
    STATIC_ASSERT(sizeof(ExampleMessage_t) <= OS_MESSAGE_MAX_SIZE);

    // send a message to example_object
    ExampleMessage_t msg;
    msg.base.id = EXAMPLE_MSG_ID;
    msg.base.msg_size = sizeof(ExampleMessage_t);
    msg.data = 543210;

    MsgQueuePut(&example_object, &msg);
}
```

```cpp
// example_object.c
#include "app_definitions.h"

void ObjectEventHandler(Message_t *msg)
{
    if ( EXAMPLE_MSG_ID == msg->id)
    {
        ExampleMessage_t *ex_msg = (ExampleMessage_t*)msg;
        // handle
    }
}
```

### Timed and Periodic Events

```cpp
#define DELAY_OR_PERIOD    500 // ms
#define EVENT_TYPE         TIMED_EVENT_SINGLE_TYPE // or TIMED_EVENT_PERIODIC_TYPE

TimedEventSimple_t event;
Message_t timed_event_msg = {.id = 0x101, .msg_size = sizeof(Message_t)};

TimedEventSimpleCreate(&event, &state_ctl_ao, &timed_event_msg, DELAY_OR_PERIOD, EVENT_TYPE);
SchedulerAddTimedEvent(&event);
```

### Memory Pools

Can be accessed using a 16-bit key

```cpp
// getting a memory block pointer
OSStatus_t status;
uint16_t key;

uint8_t* block_ptr = OSMemoryBlockNew(&key, MEMORY_BLOCK_32, &status); // _64, _128, _512 sizes available as well
```

```cpp
// getting block
uint8_t* buffer = OSMemoryBlockGet(key);

// use, make sure to free when done
OSMemoryFreeBlock(key);

```

### State Machine Framework

```cpp
#include "app_definitions.h"

#include <state_machine.h>

#include "cmd_a.h"
#include "cmd_b.h"
#include "cmd_c.h"

static StateMachine_t sm;

static CommandA_t cmd_a;
static CommandB_t cmd_b;
static CommandC_t cmd_c;

void Init()
{
    CmdAInit(&cmd_a, (Command_t*) cmd_b); // b follows a
    CmdAInit(&cmd_b, (Command_t*) cmd_c); // c follows b
    CmdAInit(&cmd_c, (Command_t*) NULL); // NULL pointer ends sequence

    StateMachineInit(&sm, (Command_t*) cmd_a); // starting with cmd_a
    StateMachineStart(&sm, NULL);
}

void EventHandler(Message_t *msg)
{
    StateMachineStep(&sm, msg, NULL);
}
```

## Supported Platforms

Tested and developed on STM32 platforms using [`ObKo/stm32-cmake`](https://github.com/ObKo/stm32-cmake)

- ARM Cortex-M4 (STM32L4R5ZI, STM32F401RE)

## STM32 Board Notes

### UART

- For STM32L4R5ZI `VddIO2` must be enabled for LPUART to work on STM32L4R5. Enable `PWR` clock beforehand.

### Clocks, Timing

- `SysTick_Handler` runs at lower interrupt priority for `rmkernel`. However, STM32 HAL expects to hook into the 1ms tick. Therefore, we need an alternative to `SysTick_Handler` for the STM32 HAL. The solution was to sacrifice `TIM2` to the HAL and configure it as a 1ms clock to drive the millisecond-precision OSTime and HAL time. Can hook onto the `__weak`ly defined `HAL_IncTick`, `HAL_InitTick`, and `HAL_GetTick` to make this happen. See [`WarehouseProject-EECS373/zumo-controller/src/rmk_hal_clock_cfg.c/h`](https://github.com/WarehouseProject-EECS373/zumo-controller/blob/main/src/rmk_hal_clock_cfg.c) for an implementation of this.
