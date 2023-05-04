# Contributing to OpenPicoRTOS

## Table of contents

  1. [Adding a new device](#adding-a-new-device)
  2. [Modifying an existing device](#modifying-an-existing-device)
  1. [Adding a new port](#adding-a-new-port)
  2. [Modifying and existing port](#modifying-an-existing-port)
  3. [Adding a driver](#adding-a-driver)
  4. [Modifying an existing driver](#modifying-an-existing-driver)
  5. [General advice](#general-advice)
  6. [Conclusion](#conclusion)

## Adding a new device

To add a new device to OpenPicoRTOS, you must first create a directory in staging/devices.
The directory should come in the form of *manufacturer*/*microcontroller*.

This directory will contain at least 4 files:

  * Makefile.in
  * picoRTOS_device.h
  * startup.S / startup.asm
  * linker file(s)

You can add a *device*.cmake on top if you think cmake is more relevant for your system.

### Makefile.in

This file contains everything you want to be automatically included in the user application build.
Here is an example template:

```make
ARCH_DIR := $(PICORTOS_DIR)/staging/arch/<arch>/<cpu>
DEVICE_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

DEFINE := <your mandatory defines>

INCLUDE := -I$(PICORTOS_DIR) -I$(PICORTOS_DIR)/arch/include
INCLUDE += -I$(DEVICE_DIR) -I$(ARCH_DIR)

SRC_C := $(PICORTOS_DIR)/picoRTOS.c
SRC_C += $(ARCH_DIR)/picoRTOS_port.c

SRC_S := $(DEVICE_DIR)/startup.S
SRC_S += $(ARCH_DIR)/picoRTOS_portasm.S
```

### picoRTOS_device.h

This file contains the device description, mostly the peripherals addresses & irqs.   
Addresses should always start with ADDR\_ & irqs with IRQ\_

Another important keyword is DEVICE\_INTERRUPT\_VECTOR\_COUNT, that must match the numbers of
interrupt vectors available to picoRTOS\_register\_irq()

On top of that you can add specific keywords used by some drivers, as some IPs can be the same
with minor differences between mcus of the same family.
It should take the form of *DRIVER_NAME*\_*PARAMETER*, ex:

```c
#define FLASH_PIC32MX_ADDR_START 0x1d000000
#define FLASH_PIC32MX_PAGE_SIZE  16384
#define FLASH_PIC32MX_ROW_SIZE   2048
```

No code is allowed in this file, only defines.

### startup.S

This is the boot sequence of your device. You can mostly copy-paste it from devices that look the
same. Some helper macros are available for most supported architectures in the devices/lib
subdirectory.

At some point, re-factorization might be required as a lot of startup files will end-up looking
the same but the structure of the folder (with its Makefile.in dependency management file) will
make the transition easy.

If possible use tabs or spaces in this assembly file, but not both.

### Linker files

You will have to provide linker files for every mcu, usually one that allows to run from RAM and
one that executes in place.
Some mcus might have different variants with different amounts of RAM & flash, it could be nice to
add them too if you can.

## Modifying an existing device

Not much to say here, just keep track of the dependencies you modify or add in Makefile.in.

Use tabs or spaces depending on the original file layout & test if the system still works.

## Adding a new port

You need at least 3 files in staging/arch/*family*/*cpu* to make a port of OpenPicoRTOS to a
new architecture. SMP ports require at least 2 more:

  * picoRTOS_types.h
  * picoRTOS_port.c
  * picoRTOS_portasm.S / picoRTOS_portasm.asm
  * picoRTOS-SMP_port.c (SMP)
  * picoRTOS-SMP_portasm.S / picoRTOS-SMP_portasm.asm (SMP)

### picoRTOS_types.h

This file contains the basic types picoRTOS need to run the scheduler.
Here's an example:

```c
typedef unsigned int picoRTOS_stack_t;
typedef unsigned long picoRTOS_tick_t;
typedef unsigned int picoRTOS_priority_t;
typedef unsigned int picoRTOS_pid_t;
typedef unsigned int picoRTOS_atomic_t;
typedef unsigned int picoRTOS_irq_t;
typedef unsigned long picoRTOS_cycles_t;
```

If you prefer using stdint types instead of ansi c ones, this is perfectly fine.

On top of that, you need to define the initial an minimal stack count (used by the default idle task):

```c
#define ARCH_INTIAL_STACK_COUNT 36
#define ARCH_MIN_STACK_COUNT (ARCH_INTIAL_STACK_COUNT + 2)
```

This is the place you define the ASM macro & arch_break(), too:

```c
/* splint cannot check inline assembly */
#ifdef S_SPLINT_S
# define ASM(x) {}
#else
# define ASM(x) asm (x)
# define arch_break() ({ ASM("break"); })
#endif
```

### picoRTOS_port.c

This file contains the basic interface for your port and should not mix c and assembly language,
at all (this is a requirement). You can use the ASM macro as long as it is side effect free
(no parameters or modifications of variables allowed).

The interfaces you need to implement are available in arch/include/picoRTOS_port.h.
You might want to implement some of them directly in assembly language, like arch_syscall() to
avoid any complaints about unused arguments by the static analyzers.

Look at the existing code for examples.

### picoRTOS_portasm.S / picoRTOS_portasm.asm

This file contains the meat of you port. This is where you will add macros like
SAVE_CONTEXT & RESTORE_CONTEXT & all the logic about the system tick & syscalls.

This is something that is probably so system-specific that it cannot be explained in a few lines, but
basically, this is where you put your interrupt service routines for tick and syscall.

This is a good place to put a function to create the first context on your stacks with
arch_save_first_context and to launch the scheduling with arch_start_first_task.

Ideally you want to switch back to the system stack in SAVE_CONTEXT & back to the task stack in
RESTORE_CONTEXT, to avoid any potential stack overflow.

### picoRTOS-SMP_port.c

This file must provide the interface for the SMP support on your chip, which usually means a way
to communicate through an interrupt between the cores.

You can find more information about this in the existing ports and in arch/include/picoRTOS-SMP_port.h

This file is more or less an add-on to picoRTOS_port.c, unless it's impossible.   
This should only contain interfaces that add on top of the existing port, not a complete re-write.

### picoRTOS-SMP_portasm.S

This file usually contains a few specific routines, like the auxiliary cores startup routines and
the interrupt service routine for arch_propagate_tick().

This is highly architecture-dependent, but a good piece of advice would be to use the auxiliary core
stack if you need to pass arguments between cores at startup instead of global variables.

## Modifying and existing port

This is much simpler than the previous one. If you see any kind of improvement to be made, just alter
the existing files & try on an existing demo.

## Adding a driver

The most difficult part of developing a new driver is to find a proper name.
Some IPs are used and reused by manufacturers, some are not, some have no official name, some have
the same but are, in fact, different. This is a mess.

You are going to make mistakes, there are probably already a few naming mistakes in the existing
drivers. This is ok, as long as the drivers build with no errors and work. Naming can wait.

For the rest, the drivers structure is the following:

  * Init functions
  * Runtime functions

### Init functions

These init functions are designed to be called during the initialization of a board. They are not
tied to any abstraction layer.   
This way you can add as many functions (init, setup) or parameters your peripheral requires
without having to worry about any structural limitations.

### Runtime functions

These functions are a little bit more defined than the init function. You have to follow the HAL
rules for your type of peripheral (you can find the interfaces in drivers/include), unless you
don't care about portability.

These HAL functions are meant to be called by upper layers and make OpenPicoRTOS easy to port
on different boards (cheap POC, eval board & final board, for example).

Of course there are some limitations due to the fact that this HAL is fully static, as function
pointers are forbidden (see requirements), you cannot have two peripherals of the same type but
different IPs at the same time on the system, currently. Any idea regarding how to solve this issue
is welcome.

## Modifying an existing driver

Not much here again, just ensure your driver builds and passes the static analysis by running
splint & cppcheck on your code.

## General advice

Every time you make a change on a C file, don't forget to run the static analysis.
You code must pass splint & cppcheck to be validated.

If it's a port, add it to the test/static_analysis makefiles and run:

```
make splint
make cppcheck
```

If it's an alteration of the scheduler, run the unit tests on top of that.

Once you're happy with it, go to the root of the project and run:

```
make uncrustify
```

Before any attempt to commit or push.

You code must build error-free in the staging dir & provide a working demo to be moved to the
root tree.

## Conclusion

I understand these guidelines might seem extremely severe, potentially starving this
project from talents, but they are here to garantee error-free software.

This is extremely important in safety critical applications for which this RTOS
has been designed for in the first place.
