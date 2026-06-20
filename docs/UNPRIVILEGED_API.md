# picoRTOS unprivileged API

This document describes the picoRTOS unprivileged API, which are the functions that can be
safely called whithin a task.

On memory-protected systems picoRTOS enforces this safety & will kill any tasks that
attempts to make a privileged call.

All these functions are available to picoRTOS, picoRTOS-SMP & picoRTOS-lite.

## Macros & shortcuts


### SCHEDULER MANAGEMENT

---
**picoRTOS_schedule**();
> Puts the current task to sleep until next tick.<br>
> Strictly equivalent to `picoRTOS_sleep(1)`

---
**picoRTOS_postpone**();
> Puts the current task back in the scheduler's FIFO [(more info)](SCHEDULERS.md)<br>
> Strictly equivalent to `picoRTOS_sleep(0)`

---
**picoRTOS_suspend**();
> Suspends the scheduling. Typical use is critical sections.<br>
> Strictly equivalent to `picoRTOS_run(false)`

---
**picoRTOS_suspend**();
> Resumes the scheduling. Typical use is critical sections.<br>
> Strictly equivalent to `picoRTOS_run(true)`


### TIME MANAGEMENT

---
**PICORTOS_DELAY_SEC**(<ins>x</ins>);
> Converts <ins>x</ins> seconds in picoRTOS_tick_t

---
**PICORTOS_DELAY_MSEC**(<ins>x</ins>);
> Converts <ins>x</ins> milliseconds in picoRTOS_tick_t

---
**PICORTOS_DELAY_MSEC**(<ins>x</ins>);
> Converts <ins>x</ins> microseconds in picoRTOS_tick_t

---
**PICORTOS_DELAY_ELAPSED**(<ins>ref</ins>, <ins>x</ins>);
> Checks if delay <ins>ref</ins> + <ins>x</ins> has elapsed


### INTERRUPT MANAGEMENT

---
**picoRTOS_enable_interrupt**(<ins>irq</ins>);
> Enables an interrupt on the system.<br>
> Strictly equivalent to `picoRTOS_set_interrupt(irq, true)`

---
**picoRTOS_disable_interrupt**(<ins>irq</ins>);
> Disables an interrupt on the system.<br>
> Strictly equivalent to `picoRTOS_set_interrupt(irq, false)`


### ASSERTS

---
**picoRTOS_dbgbreak**();
> Throws a debug exception, ignored if -DNDEBUG

---
**picoRTOS_assert**(<ins>x</ins>, <ins>or_else</ins>);
> If predicate <ins>x</ins> is true, do nothing.<br>
> If predicate <ins>x</ins> is false, execute <ins>or_else</ins>.

>[!NOTE]
> This will throw a debug exception unless NDEBUG is set

---
**picoRTOS_assert**(<ins>x</ins>, <ins>or_else</ins>);
> If predicate <ins>x</ins> is true, do nothing.<br>
> If predicate <ins>x</ins> is false, throw a debug exeption, unless
> NDEBUG is set.


## API

---
void **picoRTOS_run**(**bool** <ins>run</ins>);
> Suspend/resume the scheduling according to the value of <ins>run</ins>
### NOTES
> `picoRTOS_suspend()` is equivalent to `picoRTOS_run(false)`.<br>
> `picoRTOS_resume()` is equivalent to `picoRTOS_run(true)`.
>
> Typical use is critical sections

---
void **picoRTOS_sleep**(**picoRTOS_tick_t** <ins>delay</ins>);
> Puts the current task to sleep for <ins>delay</ins> ticks
### NOTES
> If <ins>delay</ins> is 1, the current task is put to sleep until
> the next tick, this is equivalent to `picoRTOS_schedule()`
>
> If <ins>delay</ins> is 0, the current task is put to sleep and
> set to be woken-up after the lowest priority task run and
> before the next tick (FIFO scheduling), this is the same
> as `picoRTOS_postpone()`

---
void **picoRTOS_sleep_until**(**picoRTOS_tick_t** \*<ins>ref</ins>,
**picoRTOS_tick_t** <ins>period</ins>);
> Puts the current task to sleep until *<ins>ref</ins> + <ins>period</ins> is elapsed.
### NOTES
> <ins>ref</ins> is a pointer to a reference time that will be overwritten with
> *<ins>ref</ins> + <ins>period</ins> after the call is completed.
>
> Example:
```c
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for(;;){
        my_periodic_function();
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
```
### ADDITIONAL INFO
> If the deadline is already elaspped (the caller is late), the function
> returns immediately and doesn't update *<ins>ref</ins>.
>
> After CONFIG_DEADLOCK_COUNT unsuccessful attempts, picoRTOS will kill
> the calling task with error code `FDEADLOCK`

---
void **picoRTOS_kill**(**int** <ins>errnum</ins>);
> Kills the current task with error code <ins>errnum</ins>

---
picoRTOS_pid_t **picoRTOS_self**(void);
> Returns the current task's identitifer

---
picoRTOS_tick_t **picoRTOS_get_tick**(void);
> Returns the current system tick

---
void **picoRTOS_invalidate_dcache**(const **void** \*<ins>addr</ins>, **size_t** <ins>n</ins>);
> Invalidates the data cache from <ins>addr</ins> to <ins>addr</ins> + <ins>n</ins>

---
void **picoRTOS_flush_dcache**(const **void** \*<ins>addr</ins>, **size_t** <ins>n</ins>);
> Forces data cache write to RAM, from <ins>addr</ins> to <ins>addr</ins> + <ins>n</ins>

---
void **picoRTOS_mpu_add_region**(const **void** \*<ins>addr</ins>, **size_t** <ins>n</ins>,
**unsigned** <ins>mode</ins>);
> Adds a memory region (from <ins>addr</ins> to <ins>addr</ins> + <ins>n</ins>) to the MPU.
>
> <ins>mode</ins> is described in [picoRTOS port API](PORT_API.md).<br>
> <ins>Remark:</ins> this forces us to `#include "picoRTOS_port.h"` wherever we
> use the MPU calls, which is NOT very elegant.
### NOTES
> The region is added to the context of the calling task and is NOT available to any other
> taks unless the make the same call.
>
> This function is only useful is your system uses a Memory Protection Unit
> supported by picoRTOS.

---
void **picoRTOS_set_interrupt**(**picoRTOS_irq_t** <ins>irq</ins>, **bool** <ins>active</ins>);
> Enables/disables an interrupt on the system according to the value of <ins>active</ins>
### NOTES
> `picoRTOS_enable_interrupt(irq)` is the same as `picoRTOS_set_interrupt(irq, true)`
>, and<br>
> `picoRTOS_disable_interrupt(irq)` is the same as `picoRTOS_set_interrupt(irq, false)`

