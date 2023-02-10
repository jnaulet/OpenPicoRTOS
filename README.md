# OpenPicoRTOS [![Latest Release](https://img.shields.io/github/release-date/jnaulet/OpenPicoRTOS)](https://img.shields.io) [![Commits since](https://img.shields.io/github/commits-since/jnaulet/OpenPicoRTOS/latest/v1.6.x)](https://img.shields.io)

Very small, lightning fast, yet portable preemptive RTOS with SMP suppport.

## Quick Presentation

picoRTOS is a teeny tiny RTOS with as little overhead as humanly possible.

## Book of requirements

OpenPicoRTOS has been designed with these requirements in mind:
  - Compliance with "The Power Of 10" from the NASA/JPL (https://en.wikipedia.org/wiki/The_Power_of_10:_Rules_for_Developing_Safety-Critical_Code)
  - Limited use of inline assembly:
    - Inline assembly should be side-effect free (no use or modification of variables)
    - Inline assembly can be safely removed from the static analysis
  - Smallest footprint possible
  - Fully static
  - Fully predictable
  - High portability
  - Lowest overhead possible
  - Support for SMP

More information here: https://github.com/jnaulet/OpenPicoRTOS/blob/main/etc/Requirements.md

## Documentation

HTML documentation of the complete API is available in the documentation directory and
at the following address: https://jnaulet.github.io/OpenPicoRTOS

## Supported architectures

### Single core

 - ARM Cortex-M0+
 - ARM Cortex-M3
 - AVR6
 - AVR5
 - TinyAVR 1-Series / AVRXMEGA3

### Multi-core SMP
 - RP2040 SMP

### Simulation

 - POSIX threads / Linux
 - WIN32 threads / Windows

## Featured devices

 - Atmel ATMega2560
 - Atmel ATMega328P
 - Atmel ATMega32u4
 - Atmel ATSAM3X8E
 - Raspberry Pico RP2040
 - Atmel ATtiny817

## Working principle

On every new schedule (tick) task 0 is executed first.
Any call to a sleeping function (picoRTOS_schedule, picoRTOS_sleep or
picoRTOS_sleep_until) will allow the scheduler to move to the next task until
it reaches idle or a new tick occurs and the cycle starts over.

To increase speed and predictability, every task is identified by its exclusive
level of priority, no round robin support is offered.

No memory management is offered, everything is static, which makes the static analyzer's
job much easier for critical applications.

## Advanced features

IPCs are available to architectures that support the correct associated atomic operations.
A small infringement has been made to the hard real time philosophy of the project by supporting
the CONFIG_ARCH_EMULATE_ATOMIC on platforms that don't support native atomic operations. This is
not recommended, though.

### The following IPCs are provided:

 - futexes (require arch_test_and_set)
 - re-entrant mutexes (require arch_compare_and_swap)
 - conditions (require mutexes)
 - queues (requires futexes)

## Interrupt management

Version 1.5 introduces contextual interrupt management as an experimental feature.

All architectures are supported (at least partially) at the moment.

This feature should be used with care, as interrupts tend to destroy the real-time part in
"RTOS".

## How to use

Copy the picoRTOS directory in your project.

Create a picoRTOSConfig.h file in the include path of your project.

Sample configs are available for every supported archs in arch/x/y/samples and in the
demo directory.

Then, add the relevant arch files to your build using the provided devices build files.

Example for ATMega2560:

    PICORTOS_DIR := ./picoRTOS
    -include $(PICORTOS_DIR)/devices/atmel/atmega2560/Makefile.in

IPCs are not in the build files by default, you need to add them manually to your build:

    INCLUDE += -I$(PICORTOS_DIR)/ipc
    SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_futex.c

---

Code-wise, using picoRTOS is quite straightforward :

    #include "picoRTOS.h"
    
    void main(void)
    {
        picoRTOS_init();
    
        struct picoRTOS_task task0;
        struct picoRTOS_task task1;
        static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
        static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
        ...
    
        picoRTOS_task_init(&task0, task0_main, &task0_context, stack0, CONFIG_DEFAULT_TASK_COUNT);
        picoRTOS_task_init(&task1, task1_main, &task1_context, stack1, CONFIG_DEFAULT_TASK_COUNT);
        ...
    
        picoRTOS_add_task(&task0, TASK0_PRIO);
        picoRTOS_add_task(&task1, TASK1_PRIO);
        ...
    
        picoRTOS_start();
    }

Hint: tasks are converted to internal structures in picoRTOS_add_task and can be local
but stacks need to be persistant (prefer static to globals to reduce scope).

## Demo

Some demo code is available under the demo directory for every supported architecture
so you can see for yourself if this software suits your needs.
