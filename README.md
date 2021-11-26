# Realtime Micro Kernel 

## Features

- Active Objects
  - Message queues
  - Variable sized, custom messages
- Periodic and single timed events
- Memory pools

## Supported Platforms

Tested and developed on STM32 platforms using `stm32-cmake`

- ARM Cortex-M4 (STM32L4R5ZI, STM32F401RE)

### UART

- `VddIO2` must be enabled for LPUART to work on STM32L4R5. Enable `PWR` clock beforehand.

### Clocks, Timing

- `SysTick_Handler` runs at lower interrupt priority for `rmkernel`. However, STM32 HAL expects to hook into the 1ms tick. Therefore, we need an alternative to `SysTick_Handler` for the STM32 HAL. The solution was to sacrifice `TIM2` to the HAL and configure it as a 1ms clock to drive the millisecond-precision OSTime and HAL time. Can hook onto the `__weak`ly defined `HAL_IncTick`, `HAL_InitTick`, and `HAL_GetTick` to make this happen.
