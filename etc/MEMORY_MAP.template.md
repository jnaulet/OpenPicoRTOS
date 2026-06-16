# picoRTOS Memory Map
## Memory organisation

```
               PRIVILEGED               │                      UNPRIVILEGED                        
                                        │                                                          
                                        │  ┌──────────┐    ┌─────────────────────┐                 
          ┌───────────────────┐         │  │          │    │       TASK#0        │                 
          │      KERNEL       │         │  │ USER API │◄───┤┌───────────────────┐│                 
          │ ┌───────────────┐ │         │  │          │    ││       STACK       ││                 
          │ │ MAIN STACK(S) │ │         │  └────┬─────┘    │├───────────────────┤│  R/W            
          │ └───────────────┘ │         │       │   ▲      ││  CLAIMED MEMORY   │├────────┐        
          │ ┌───────────────┐ │         │       │   │      │├───────────────────┤│        │        
          │ │   SCHEDULER   │◄┼───── SYSCALL ◄──┘   │      ││CLAIMED PERIPHERALS││        ▼        
          │ └───────────────┘ │         │           │      │└───────────────────┘│ ┌─────────────┐ 
          │ ┌───────────────┐ │         │           │      └─────────────────────┘ │             │ 
          │ │  INTERRUPTS   │ │         │           │      ┌─────────────────────┐ │ SHARED DATA │ 
          │ └───────────────┘ │         │           │      │       TASK#1        │ │             │ 
          │ ┌─────┐   ┌─────┐ │         │           └──────┤┌───────────────────┐│ └─────────────┘ 
          │ │ MPU │   │TIMER│ │         │                  ││       STACK       ││        ▲        
          │ └─────┘   └─────┘ │         │                  │├───────────────────┤│  R/W   │        
          └───────────────────┘         │                  ││  CLAIMED MEMORY   │├────────┘        
                                        │                  │├───────────────────┤│                 
                                        │                  ││CLAIMED PERIPHERALS││                 
                                        │                  │└───────────────────┘│                 
                                        │                  └─────────────────────┘                 
                                        │                            ...                           
                                                                                                   
```

## Sections

A software built with picoRTOS follows the following structure:

```
                                  ┌──────────────┐                          
                                  │    .init     │ ┌────────────────────┐   
          ┌─────────────────────┐ ├──────────────┤ │ Privileged .text   │   
          │  Privileged .data   │ │    .text     │/├────────────────────┤   
          ├─────────────────────┤\├──────────────┤\│       .text        │   
          │ Privileged stack(s) │/│    .data     │ └────────────────────┘   
          ├─────────────────────┤ ├──────────────┤ ┌───────────────────────┐
          │   Privileged .bss   │ │    .bss      │/│ Unprivileged stack(s) │
          ├─────────────────────┤ └──────────────┘\├───────────────────────┤
          │       .data         │                  │         .bss          │
          ├─────────────────────┤                  └───────────────────────┘
          │ Unprivileged .data  │                                           
          └─────────────────────┘                                           
```

Linker files in `arch/*/include/sections.ld` provide even more precise information.

## Description

A picoRTOS program only uses 4 main sections :

### .init

Here sits the startup code, typically the <<ins>startup.S</ins>> file
you can find in the relevant `arch/*/mach-*` directory.

### .text

This is the main code section. It is divided into 2 sections:

#### Privileged .text

This is where the picoRTOS kernel code sits & runs.

If `CONFIG_MPU`, picoRTOS will configure the MPU to ensure this code can only
be executed in supervisor/privileged mode.

Permissions are PRX (Privileged Read-only eXecute).

#### .text

This is where the rest of the code (drivers & tasks) sit & run.

If `CONFIG_MPU`, picoRTOS will configure the MPU to ensure this code can
be executed in user/unprivileged mode.

Permissions are URX (Unprivileged Read-only eXecute).

#### Beyond .text

In certain applications you might want to ensure only a certain task can
execute a certain section of code.

In order to achieve that you need to add custom sections to your linker file &
use `picoRTOS_mpu_add_region()` with the relevant permissions in your tasks
to ensure air-tight memory protection.

### .data

This is the main data section, where initialized data sits.

#### Privileged .data

This is where the picoRTOS kernel data sits.

If `CONFIG_MPU`, picoRTOS will configure the MPU to ensure this data can only
be accessed in supervisor/privileged mode.

Permissions are PRW (Privileged Read Write).

#### Privileged stack(s)

This sections contains the main stack(s) for single-core & SMP.<br>
picoRTOS switches to these stacks in privileged mode.

If `CONFIG_MPU`, picoRTOS will configure the MPU to ensure this data can only
be accessed in supervisor/privileged mode.

Permissions are PRW (Privileged Read Write).

#### Privileged .bss

This is the privileged .bss section, where kernel zero-initialized data sits.

If `CONFIG_MPU`, picoRTOS will configure the MPU to ensure this data can only
be accessed in supervisor/privileged mode.

Permissions are PRW (Privileged Read Write).

#### .data

This is the usual .data section.

If `CONFIG_MPU`, nothing in this section is available to anyone, except if
explicitly added with `picoRTOS_mpu_add_region()`

No permissions are set.

#### Unprivileged .data

No actual data should sit here unless explicitly marked with `UNPRIVILEGED_DATA`
(see Keywords for more info).

If `CONFIG_MPU`, picoRTOS will configure the MPU to ensure this data can
be accessed by everyone (shared memory).<br>
You should really be careful with this option & only put IPCs like mutexes
& conditions in this section.

Permissions are URW (Unprivileged Read Write).

### .bss

#### Unprivileged stack(s)

This section contains the idle task(s) stacks, provided by the kernel but
accessible to them in user/unprivileged mode.

If `CONFIG_MPU`, picoRTOS will configure the MPU to ensure this data can only
be accessed by the idle task.

Permissions are URW (Unprivileged Read Write).

#### .bss

This is the usual .bss section, where zero-initialized data sits.

If `CONFIG_MPU`, nothing in this section is available to anyone, except if
explicitly added with `picoRTOS_mpu_add_region()`

No permissions are set.

## Keywords (C)

These sections are accessible using keywords in the code if needed.

!extractmd.pl arch/include/picoRTOS_core.h

## Architecture-specific

TBD
