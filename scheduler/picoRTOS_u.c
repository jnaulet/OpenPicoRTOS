#include "picoRTOS.h"
#include "picoRTOS_port.h"

/**
 * void **picoRTOS_run**(**bool** <ins>run</ins>);
 * > Suspend/resume the scheduling according to the value of <ins>run</ins>
 * ### NOTES
 * > `picoRTOS_suspend()` is equivalent to `picoRTOS_run(false)`.<br>
 * > `picoRTOS_resume()` is equivalent to `picoRTOS_run(true)`.
 * >
 * > Typical use is critical sections
 */
void picoRTOS_run(bool run)
{
    arch_syscall(SYSCALL_RUN, &run);
}

/**
 * void **picoRTOS_sleep**(**picoRTOS_tick_t** <ins>delay</ins>);
 * > Puts the current task to sleep for <ins>delay</ins> ticks
 * ### NOTES
 * > If <ins>delay</ins> is 1, the current task is put to sleep until
 * > the next tick, this is equivalent to `picoRTOS_schedule()`
 * >
 * > If <ins>delay</ins> is 0, the current task is put to sleep and
 * > set to be woken-up after the lowest priority task run and
 * > before the next tick (FIFO scheduling), this is the same
 * > as `picoRTOS_postpone()`
 */
void picoRTOS_sleep(picoRTOS_tick_t delay)
{
    arch_syscall(SYSCALL_SLEEP, &delay);
}

/**
 * void **picoRTOS_sleep_until**(**picoRTOS_tick_t** \*<ins>ref</ins>,
 * **picoRTOS_tick_t** <ins>period</ins>);
 * > Puts the current task to sleep until *<ins>ref</ins> + <ins>period</ins> is elapsed.
 * ### NOTES
 * > <ins>ref</ins> is a pointer to a reference time that will be overwritten with
 * > *<ins>ref</ins> + <ins>period</ins> after the call is completed.
 * >
 * > Example:
 * ```c
 *     picoRTOS_tick_t ref = picoRTOS_get_tick();
 *
 *     for(;;){
 *         my_periodic_function();
 *         picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
 *     }
 * ```
 * ### ADDITIONAL INFO
 * > If the deadline is already elaspped (the caller is late), the function
 * > returns immediately and doesn't update *<ins>ref</ins>.
 * >
 * > After CONFIG_DEADLOCK_COUNT unsuccessful attempts, picoRTOS will kill
 * > the calling task with error code `FDEADLOCK`
 */
void picoRTOS_sleep_until(picoRTOS_tick_t *ref, picoRTOS_tick_t period)
{
    picoRTOS_assert(period > 0, return );
    struct syscall_sleep_until sc = { *ref, period };

    arch_syscall(SYSCALL_SLEEP_UNTIL, &sc);
    *ref = sc.ref; /* update ref */
}

/**
 * void **picoRTOS_kill**(**int** <ins>errnum</ins>);
 * > Kills the current task with error code <ins>errnum</ins>
 */
void picoRTOS_kill(int errnum)
{
    arch_syscall(SYSCALL_KILL, &errnum);
}

/**
 * picoRTOS_pid_t **picoRTOS_self**(void);
 * > Returns the current task's identitifer
 */
picoRTOS_pid_t picoRTOS_self(void)
{
    picoRTOS_pid_t pid = 0;

    arch_syscall(SYSCALL_GETPID, &pid);
    return pid;
}

/**
 * picoRTOS_tick_t **picoRTOS_get_tick**(void);
 * > Returns the current system tick
 */
picoRTOS_tick_t picoRTOS_get_tick(void)
{
    picoRTOS_tick_t tick = 0;

    arch_syscall(SYSCALL_GETTICK, &tick);
    return tick;
}

/**
 * void **picoRTOS_invalidate_dcache**(const **void** \*<ins>addr</ins>, **size_t** <ins>n</ins>);
 * > Invalidates the data cache from <ins>addr</ins> to <ins>addr</ins> + <ins>n</ins>
 */
void picoRTOS_invalidate_dcache(const void *addr, size_t n)
{
    picoRTOS_assert(n > 0, return );
    struct syscall_cacheop op = { true, false, addr, n };

    arch_syscall(SYSCALL_CACHEOP, &op);
}

/**
 * void **picoRTOS_flush_dcache**(const **void** \*<ins>addr</ins>, **size_t** <ins>n</ins>);
 * > Forces data cache write to RAM, from <ins>addr</ins> to <ins>addr</ins> + <ins>n</ins>
 */
void picoRTOS_flush_dcache(const void *addr, size_t n)
{
    picoRTOS_assert(n > 0, return );
    struct syscall_cacheop op = { false, true, addr, n };

    arch_syscall(SYSCALL_CACHEOP, &op);
}

/**
 * void **picoRTOS_mpu_add_region**(const **void** \*<ins>addr</ins>, **size_t** <ins>n</ins>,
 * **unsigned** <ins>mode</ins>);
 * > Adds a memory region (from <ins>addr</ins> to <ins>addr</ins> + <ins>n</ins>) to the MPU.
 * >
 * > <ins>mode</ins> is described in [picoRTOS port API](PORT_API.md).<br>
 * > <ins>Remark:</ins> this forces us to `#include "picoRTOS_port.h"` wherever we
 * > use the MPU calls, which is NOT very elegant.
 * ### NOTES
 * > The region is added to the context of the calling task and is NOT available to any other
 * > taks unless the make the same call.
 * >
 * > This function is only useful is your system uses a Memory Protection Unit
 * > supported by picoRTOS.
 */
void picoRTOS_mpu_add_region(const void *addr, size_t n, unsigned mode)
{
    picoRTOS_assert(n > 0, return );
    struct syscall_mpu mpu = { addr, n, mode };

    arch_syscall(SYSCALL_MPU, &mpu);
}

/**
 * void **picoRTOS_set_interrupt**(**picoRTOS_irq_t** <ins>irq</ins>, **bool** <ins>active</ins>);
 * > Enables/disables an interrupt on the system according to the value of <ins>active</ins>
 * ### NOTES
 * > `picoRTOS_enable_interrupt(irq)` is the same as `picoRTOS_set_interrupt(irq, true)`
 * >, and<br>
 * > `picoRTOS_disable_interrupt(irq)` is the same as `picoRTOS_set_interrupt(irq, false)`
 */
void picoRTOS_set_interrupt(picoRTOS_irq_t irq, bool active)
{
    struct syscall_irqop op = { irq, active };

    arch_syscall(SYSCALL_IRQOP, &op);
}
