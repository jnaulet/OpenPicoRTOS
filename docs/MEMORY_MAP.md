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

---
**PRIVILEGED_TEXT**
> Puts the following code in the kernel privileged section `.ptext`
### NOTES
> If `CONFIG_MPU`, this section is only accessible through privilege
> escalation & no task can access & fetch instructions (SEGFAULT)<br>
> This is where the kernel code sits & you **SHOULDN'T** put
> anything here

---
**UNPRIVILEGED_TEXT**
> Puts the following code in the unprivileged section `.utext`
### NOTES
> If `CONFIG_MPU`, this section is accessible to everyoneto read or fetch
> instructions.<br>
> If you want better code protection, you **SHOULD** use custom sections
> & add them through `picoRTOS_mpu_add_region()`

---
**PRIVILEGED_DATA**
> Puts the following data in the kernel privileged section `.pdata`
### NOTES
> If `CONFIG_MPU`, this section is only accessible through privilege
> escalation & no task can access it (SEGFAULT)

---
**UNPRIVILEGED_DATA**
> Put the following data in the unprivileged section `.udata`
> ###NOTES
> Typical use:
```c
    static struct picoRTOS_mutex UNPRIVILEGED_DATA mutex = PICORTOS_MUTEX_INITIALIZER;
    static struct picoRTOS_cond UNPRIVILEGED_DATA cond = PICORTOS_COND_INITIALIZER;
```
> If `CONFIG_MPU`, this section is accessible to any task, regardless of
> privileges. This **SHOULD** be of very limited use (IPCs, typically)

---
**PRIVILEGED_STACK**
> Put the following data in the privileged data section `.pstack`
### NOTES
> You **SHOULDN'T** use this section, it is reserved for internal kernel
> use (main stacks for single-core & SMP).

---
**UNPRIVILEGED_STACK**
> Put the following data in the unprivileged data section `.ustack`
### NOTES
> You **SHOULDN'T** use this section, it is reserved for internal kernel
> use (idle stacks)

---
**PRIVILEGED_BSS**
> Put the following data in the privileged data section `.pbss`
### NOTES
> You **SHOULDN'T** use this section, it is reserved for internal kernel
> use (main stacks for single-core & SMP).

---
**UNPRIVILEGED_BSS**
> Put the following data in the unprivileged bss section `.bss`
### NOTES
> This section is ignored by picoRTOS & any access without prior
> use of `picoRTOS_mpu_add_region()` will lead to a SEGFAULT.
>
> A notable exception are the tasks' stacks, who sit there but
> are made R/W accessible to their owners automatically after
> `picoRTOS_start()`


## Architecture-specific

TBD
