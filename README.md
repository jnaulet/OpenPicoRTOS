# OpenPicoRTOS [![Latest Release](https://img.shields.io/github/release-date/jnaulet/OpenPicoRTOS)](https://img.shields.io) [![Commits since](https://img.shields.io/github/commits-since/jnaulet/OpenPicoRTOS/latest/v1.9.x)](https://img.shields.io)

Very small, safe, lightning fast, yet portable preemptive RTOS with SMP support.

## Quick Presentation

picoRTOS is a small hard RTOS with as little overhead as humanly possible.

## Table of contents

  1. [Book of requirements](#book-of-requirements)
  1. [How to use](#how-to-use)
  1. [API Documentation](#api-documentation)
  1. [Supported architectures](#supported-architectures)
  1. [Featured devices](#featured-devices)
  1. [Working principle](#working-principle)
  1. [Inter-processus communication](#inter-processus-communication)
  1. [Shared priorities](#shared-priorities)
  1. [Interrupt management](#interrupt-management)
  1. [Staging tree](#staging-tree)
  1. [Featured demos](#featured-demos)

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
  - Less than 400 lines of code

More information here: https://github.com/jnaulet/OpenPicoRTOS/blob/main/etc/Requirements.md

## How to use

### Step1: Preparation of your project

To create a project compatible with OpenPicoRTOS you first need to add a specific kind of Makefile to your project.   
You can find a very basic template here: https://github.com/jnaulet/OpenPicoRTOS/blob/v1.9.x/etc/Makefile.template

This offers you access to a new set of targets for the OpenPicoRTOS build system directly in your project directory:

```
Cleaning targets:
  clean           - Remove most generated files but keep the config and
                    enough build support to build external modules
  mrproper        - Remove all generated files + config + various backup files
  distclean       - mrproper + remove editor backup and patch files

Configuration targets:
  config          - Update current config utilising a line-oriented program
  nconfig         - Update current config utilising a ncurses menu based program
  menuconfig      - Update current config utilising a menu based program
  xconfig         - Update current config utilising a QT based front-end
  gconfig         - Update current config utilising a GTK based front-end
  oldconfig       - Update current config utilising a provided .config as base
  localmodconfig  - Update current config disabling modules not loaded
  localyesconfig  - Update current config converting local mods to core
  silentoldconfig - Same as oldconfig, but quietly, additionally update deps
  defconfig       - New config with default from ARCH supplied defconfig
  savedefconfig   - Save current config as ./defconfig (minimal config)
  allnoconfig     - New config where all options are answered with no
  allyesconfig    - New config where all options are accepted with yes
  allmodconfig    - New config selecting modules when possible
  alldefconfig    - New config with all symbols set to default
  randconfig      - New config with random answer to all options
  listnewconfig   - List new options
  oldnoconfig     - Same as silentoldconfig but set new symbols to n (unset)

Other generic targets:
  all             - Build target

Test targets:
  check           - Run static analysis on all basic platforms + unit tests

Static analysis targets:
  splint          - Run splint -checks on the source code
  cppcheck        - Run cppcheck --enable=all on the source code
  staticcheck     - Run both previous checks

  make V=0|1 [targets] 0 => quiet build (default), 1 => verbose build

```

### Step2: configuration of your project

Simply type:

    # make menuconfig

And configure your project according to your needs.
You can find a lot of examples in the demo directory.

### Step3: Compilation of your project

    # make

### Step4: Static analysis of your project

    # make staticcheck

or, alternatively:

    # make splint
    # make cppcheck

## API Documentation

HTML documentation of the complete API is available in the documentation directory and
at the following address: https://jnaulet.github.io/OpenPicoRTOS

## Supported architectures

### Single core

 - ARM Cortex-M0+
 - ARM Cortex-M3
 - ARM Cortex-M4/F
 - ARM Cortex-M7
 - Atmel ATMega (AVR5+)
 - Atmel TinyAVR (0+) Series
 - MIPS M51xx / PIC32Mx
 - RISC-V RV32IMAC
 - PowerPC e200z4 (staging)
 - PowerPC e200z7 (staging)
 - TI C2000 / c28x

### Multi-core SMP
 - PowerPC e200z4 SMP (staging)
 - PowerPC e200z7 SMP (staging)
 - RP2040 SMP

### Simulation

 - POSIX threads / Linux
 - WIN32 threads / Windows

## Featured devices

 - Atmel ATMega2560
 - Atmel ATMega328P
 - Atmel ATMega32u4
 - Atmel ATSAM3X8E
 - Atmel ATSAMD5x/E5x
 - Atmel ATtiny817
 - Atmel ATtiny1607
 - GigaDevice GD32VF103
 - Microchip PIC32MZ-EF
 - Raspberry Pico RP2040
 - Renesas RA4M1
 - STMicro STM32H743/750
 - Texas Instruments TMS320F2837xD

## Working principle

On every new cycle (tick), picoRTOS stops the execution of the current task and runs the highest
priority task available.   
A few criterias make a task available for scheduling, it has to be:
  - Ready (aka not sleeping/busy)
  - The tick modulo has to match the task sub-priority (see shared priorities)
  - In case of SMP, the task core mask has to match the current running core

Any syscall (picoRTOS_schedule, picoRTOS_sleep or picoRTOS_sleep_until) will allow picoRTOS to run
the next highest priority task available until it reaches idle or a new tick occurs and the cycle
starts over.

Task execution order goes from highest (0) to lowest (CONFIG_TASK_COUNT - 1) priority.

Example:

    tick: prio0 -> prio1 -> prio2 -> ... (towards idle)   
    tick: prio0 -> prio1 -> prio2 -> ... (towards idle)
    ...

No memory management is offered, everything is static, which makes the static analyzer's
job much easier for critical applications.

## Inter-processus communication

The following IPCs are provided:

 - futexes (require arch_test_and_set)
 - re-entrant mutexes (require arch_compare_and_swap)
 - conditions (require mutexes)
 - queues (requires futexes)

## Shared priorities

Version 1.7 introduces shared priorities for tasks, a.k.a round-robin scheduling.

When several tasks share the same priority, the order of execution (highest to lowest
priority) doesn't change, but these tasks will be executed alternatively, on a tick
modulo basis.

Example with 2 tasks (B & C) sharing a priority of 1:

    tick 0: taskA (prio 0) -> taskB (prio 1) -> taskD (prio 2) -> ...    
    tick 1: taskA (prio 0) -> taskC (prio 1) -> taskD (prio 2) -> ...  
    tick 2: taskA (prio 0) -> taskB (prio 1) -> taskD (prio 2) -> ...  
    tick 3: taskA (prio 0) -> taskC (prio 1) -> taskD (prio 2) -> ...  
    ...

## Interrupt management

Version 1.5 introduces contextual interrupt management as an experimental feature.

All architectures are supported (at least partially) at the moment.

This feature should be used with care, as interrupts tend to destroy the real-time part in
"RTOS".

## Staging tree

Version 1.8 introduces the staging tree.

This tree contains code that builds & fits the book of requirements but could
not be tested for one reason or the other.

This code should be considered highly experimental until validation.

## Featured demos

Basic demo code is provided for the following boards:
  - Adafruit ItsyBitsy M4 Express
  - Arduino Due
  - Arduino Mega2560
  - Arduino Uno
  - Arduino Uno R4 Minima
  - ATtiny817-Xplained Mini
  - ATtiny1607 Curiosity Nano
  - Curiosity 2.0 PIC32 MZ EF
  - NXP Devkit MPC5748G (dual-core SMP) (staging)
  - NXP MPC5775E-EVB (dual-core SMP) (staging)
  - Sipeed Longan Nano
  - DevEBox STM32H7xx_M (staging)
  - Texas Instruments Launchxl-f28379d
  - Raspberry Pi Pico (single core & SMP)

A portability demo is available in demo/amigaball-lcd and works with the
follwing boards (+0.96" LCD):
  - Adafruit ItsyBitsy M4 Express
  - Sipeed Longan Nano
  - Texas Instruments Launchxl-f28379d
  - Raspberry Pi Pico SMP
  - DevEBox STM32H7xx_M / STM32H743VIT6
