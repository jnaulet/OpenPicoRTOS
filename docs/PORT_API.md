# picoRTOS Port API

This document describes the API a port **MUST** use and/or provide in order to be able to run picoRTOS on a
new architecture.

    1. [Single core](#single-core)
    1. [SMP extensions](#smp-extensions)

## Single core


## provided by picoRTOS

---
picoRTOS_stack_t \***picoRTOS_syscall**(**picoRTOS_stack_t** \*<ins>sp</ins>
**syscall_t** <ins>syscall</ins>, **void** \*<ins>priv</ins>):
> Executes a syscall
### NOTES
> This **MUST** be called from a syscall interrupt or equivalent & provide
> the calling task's <ins>sp</ins> as the first parameter.
>
> On memory-protected systems, you have to call this with a SYSCALL_SEGFAULT
> <ins>syscall</ins> and provide a non-null <ins>priv</ins> parameter when there's
> an access or privilege violation.
>
> Usually, this is the address of the instruction or data that lead to the exception.
### RETURN
> This call returns the stack you have to switch to (context restore)

---
picoRTOS_stack_t \***picoRTOS_tick**(**picoRTOS_stack_t** *<ins>sp</ins>);
> Increments the tick & starts a new cycle
### NOTES
> This **MUST** be called from your main tick timer interrupt and provide
> the current task's <ins>sp</ins> as a single parameter.
### RETURN
> This call will return the next task sp to restore.

---
**arch_assert**(<ins>predicate</ins>, <ins>or_else</ins>);
> Asserts <ins>predicate</ins>, execute <ins>or_else</ins> if it fails
### NOTES
> This macro is very similar to `picoRTOS_assert()` but can be used in your
> ports without creating a dependency issue.
>
> Remark: <ins>predicate</ins> **MUST** be side-effect free, this will be
> enforced by the static analysis.

---
**arch_assert_void**(<ins>predicate</ins>);
> Asserts <ins>predicate</ins>
### NOTES
> This macro is very similar to `picoRTOS_assert_void()` but can be used in
> your ports without creating a dependency issue.
>
> If the macro NDEBUG is defined at the moment <ins><picoRTOS_port.h></ins>
> is included, this macro generates no code.
>
> Remark: <ins>predicate</ins> **MUST** be side-effect free, this will be
> enforced by the static analysis.


## Provided by port

---
void **arch_init**(void);
> Architecture/port main initialization function
### NOTES
> This function will be called by picoRTOS in `picoRTOS_start()`.
>
> Ports **MUST** provide this function in order to setup the tick timer,
> interrupts, and so on.

---
void **arch_suspend**(void);
> Suspends the scheduling
### NOTES
> This function will be called by `picoRTOS_syscall()`

---
void **arch_resume**(void);
> Resumes the scheduling
### NOTES
> This function will be called by `picoRTOS_syscall()`

---
picoRTOS_stack_t \***arch_prepare_stack**(**picoRTOS_stack_t** \*<ins>stack</ins>,
**size_t** <ins>stack_count</ins>, **arch_entry_point_fn** <ins>fn</ins>,
**void** \*<ins>priv</ins>);
> Prepares a task's stack for context restoration.
### NOTES
> This function is used by `picoRTOS_add_task()` to prepare the stack
> for context restoration.<br>
> The return stack structure must match your `RESTORE_CONTEXT` procedure
> (see <<ins>picoRTOS_portasm.S</ins>> for more information).
### RETURN
> This call **MUST** return a pointer to the first element of the newly
> prepared stack.

---
void **arch_start_first_stack**(**picoRTOS_stack_t** \*<ins>sp</ins>);
> Starts the first task on the system (usually *idle*) and
> bootstraps the scheduler.
### NOTES
> This function is called by `picoRTOS_start()` and will restore
> the previously prepared <ins>sp</ins>.
>
> Additionnaly, you might want to start the timer here.

---
void **arch_syscall**(**syscall_t** syscall, **void** \*<ins>priv</ins>);
> Port syscall function
### NOTES
> Every [UNPRIVILEGED API](UNPRIVILEGED_API.md) call found in
> <<ins>picoRTOS_u.c</ins>> will trigger a syscall to hand things
> over to the kernel.
>
> On memory-protected systems, this call **SHOULD** trigger a
> privilege escalation (user -> supervisor).

---
void **arch_idle**(void);
> Idle task function
### NOTES
> This function **MUST** provide an infinite loop that puts the CPU in IDLE mode
>
> This function will be called by `arch_start_first_task()` after ther first
> context restoration.


## Atomic operations

---
picoRTOS_atomic_t **arch_test_and_set**(**picoRTOS_atomic_t** \*<ins>ptr</ins>);
> Atomic test and set operation
### NOTES
> if *<ins>ptr</ins> value is 0, its value is changed to 1 & the function returns 0.<br>
> if *<ins>ptr</ins> value is 1, the call does nothing & returns 1;

---
picoRTOS_atomic_t **arch_compare_and_swap**(**picoRTOS_atomic_t** \*<ins>var</ins>,
**picoRTOS_atomic_t** <ins>old</ins>, **picoRTOS_atomic_t** <ins>val</ins>);
> Atomic compare and swap operation
### NOTES
> If *<ins>var</ins> & <ins>old</ins> are equal, *<ins>var</ins> is set to <ins>val</ins>
> and the call returns <ins>old</ins>.<br>
> If <ins>var</ins> & <ins>old</ins> are different, the call returns <ins>val</ins>.


## Interrupts

---
void **arch_register_interrupt**(**picoRTOS_irq_t** <ins>irq</ins>,
**arch_isr_fn** <ins>fn</ins>, **void** \*<ins>priv</ins>);
> Registers an interrupt on the system
### NOTES
> Make sure that when <ins>irq</ins> is asserted, <ins>fn</ins> is
> called with <ins>priv</ins> as a parameter.

---
void **arch_enable_interrupt**(**picoRTOS_irq_t** <ins>irq</ins>);
> Enables an <ins>irq</ins>
### NOTES
> This function will be called by the relevant `picoRTOS_syscall()`

---
void **arch_disable_interrupt**(**picoRTOS_irq_t** <ins>irq</ins>);
> Disables an <ins>irq</ins>
### NOTES
> This function will be called by the relevant `picoRTOS_syscall()`


## Statistics

---
picoRTOS_cycles_t **arch_counter**(**arch_counter_t** <ins>counter</ins>,
**picoRTOS_cycles_t** <ins>t</ins>);
> Provides the current cpu counter value (optional)
### NOTES
> picoRTOS will use this function to maintain tasks statistics in the kernel.<br>
> picoRTOS-lite doesn't use it at all.
>
> <ins>counter</ins> can be:
> - ARCH_COUNTER_CURRENT: returns the current counter value regardless of <ins>t</ins>
> - ARCH_COUNTER_SINCE: computes & returns the elapsed time between <ins>t</ins> and now.


## Cache operations (*only if `CONFIG_CACHE`*)

---
void **arch_invalidate_dcache**(const **void** \*<ins>addr</ins>,
**size_t** <ins>n<ins>);
> Invalidates one or more lines of cache, from <ins>addr</ins> to
> <ins>addr + n</ins>

---
void **arch_flush_dcache**(const **void** \*<ins>addr</ins>,
**size_t** <ins>n<ins>);
> Flushes one or more lines of cache to RAM, from <ins>addr</ins> to
> <ins>addr + n</ins>


Clock operations

---
void **arch_set_clock_frequency**(**unsigned long** <ins>freq</ins>);
> Sets the current CPU input frequency (in hz)
### NOTES
> This function is **NOT** called from picoRTOS itself, but **SHOULD**
> be called from any clock driver to ensure kernel & hardware clock(s)
> have the same parameters.

---
void **arch_delay_us**(**unsigned long** <ins>n</ins>);
> Busy waits for at least <ins>n</ins> microseconds


## MPU Operations (*only if `CONFIG_MPU`*)

---
void **arch_mpu_init**(void);
> Initializes the Memory Protection Unit
### NOTES
> This function is called during `picoRTOS_init()`

---
void **arch_mpu_add_region**(**int** <ins>pid</ins>, const **void** \*<ins>addr</ins>,
**size_t** <ins>n</ins>, **mpu_mode_t** <ins>mode</ins>);
> Adds a memory region to the MPU
### NOTES
> if <ins>pid</ins> is PID_KERNEL, the region is not linked to a task, but directly
> to the kernel itself.<br>
> <ins>mode</ins> is a mask, not an enum that can cumulate the following values:
>> MM_NON_CACHEABLE: the region is cache-inhibited<br>
>> MM_PRIVILIEGED: the region is only accessible to the kernel<br>
>> MM_READ: the region is readable<br>
>> MM_WRITE: the region is writable<br>
>> MM_EXECUTE: instructions can be fetched from this region

---
void **arch_mpu_restore_regions**(**int** <ins>pid</ins>);
> Restores MPU regions for the task identified by <ins>pid</ins>
### NOTES
> This will be called by picoRTOS before any context restoration

---
void **arch_mpu_enable**(void);
> Enables the MPU*
### NOTES
> Called by `picoRTOS_start()`


## SMP extensions

---
void **arch_smp_init**(void);
> SMP port initialization function
### NOTES
> This function is the SMP version of `arch_init()`.<br>
> It is called by <<ins>picoRTOS-SMP.c</ins>> `picoRTOS_start()`.
>
> <ins>Remark:</ins> `arch_init()` won't be automatically called.

---
void **arch_core_init**(**picoRTOS_core_t** <ins>core</ins>,
**picoRTOS_stack_t** \*<ins>sp</ins>);
> Auxiliary <ins>core</ins> startup function
### NOTES
> This function will be called by `picoRTOS_start()` for every auxiliary
> core you want ot start & will provide the corresponding idle stack/context
> in <ins>sp</ins>.

---
picoRTOS_core_t **arch_core**(void);
> Provides the current core index

---
void **arch_spin_lock**(void);
> Acquires the SMP spinlock
### NOTES
> On SMP architectures, the cores need a sync mechanism.<br>
> picoRTOS uses a simple spinlock, preferably a hardware one.
>
> Remark: this **MUST** make CONFIG_DEADLOCK_COUNT attempts at acquiring the lock
> and declare deadlock if it fails (`fatal()`)

---
void **arch_spin_unlock**(void);
> Unlocks the SMP spinlock


Interrupt management

---
void **arch_smp_register_interrupt**(**picoRTOS_irq_t** <ins>irq</ins>,
**arch_isr_fn** <ins>fn</ins>, **void** \*<ins>priv</ins>,
**picoRTOS_mask_t** <ins>core_mask</ins>);
> Registers an interrupt on the system & attach it to the core(s)
> matching the <ins>core_mask</ins>
### NOTES
> Make sure that when <ins>irq</ins> is asserted, <ins>fn</ins> is
> called with <ins>priv</ins> as a parameter.
>
> <ins>core_mask</ins> is a bitfield where every core has it own bit,
> for example, (1 << 0) for core0, (1 << 1) for core1, etc

