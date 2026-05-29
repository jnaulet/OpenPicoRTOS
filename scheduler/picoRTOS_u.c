#include "picoRTOS.h"
#include "picoRTOS_port.h"

/* Group: picoRTOS scheduler unprivileged API */

/* Function: picoRTOS_run
 * Suspend/resumes the scheduling. Typical use is critical sections
 */
void picoRTOS_run(bool run)
{
    arch_syscall(SYSCALL_RUN, &run);
}

/* Function: picoRTOS_sleep
 * Puts the current task to sleep for the specified number of ticks
 *
 * Parameters:
 *  delay - A delay in picoRTOS_tick_t (ticks)
 *
 */
void picoRTOS_sleep(picoRTOS_tick_t delay)
{
    arch_syscall(SYSCALL_SLEEP, &delay);
}

/* Function: picoRTOS_sleep_until
 * Puts the current task to sleep until *ref + period is elapsed.
 *
 * Parameters:
 *  ref - A pointer to a reference time in ticks (will be overwritten)
 *  period - A period in ticks
 *
 * Example:
 * (start code)
 * picoRTOS_tick_t ref = picoRTOS_get_tick();
 *
 * for(;;){
 *   my_periodic_function();
 *   picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
 * }
 * (end)
 *
 * Remarks:
 * If the period is already elapsed (aka we're late), picoRTOS will throw a debug
 * exception, update *ref to current tick and continue anyway
 */
void picoRTOS_sleep_until(picoRTOS_tick_t *ref, picoRTOS_tick_t period)
{
    picoRTOS_assert(period > 0, return );
    struct syscall_sleep_until sc = { *ref, period };

    arch_syscall(SYSCALL_SLEEP_UNTIL, &sc);
    *ref = sc.ref; /* update ref */
}

/* Function: picoRTOS_kill
 * Kills the current task (suicide)
 */
void picoRTOS_kill(int errnum)
{
    arch_syscall(SYSCALL_KILL, &errnum);
}

/* Function: picoRTOS_self
 * Returns the current task's priority/identitifer
 */
picoRTOS_pid_t picoRTOS_self(void)
{
    picoRTOS_pid_t pid = 0;

    arch_syscall(SYSCALL_GETPID, &pid);
    return pid;
}

/* Function: picoRTOS_get_tick
 * Returns the current system tick/timer
 */
picoRTOS_tick_t picoRTOS_get_tick(void)
{
    picoRTOS_tick_t tick = 0;

    arch_syscall(SYSCALL_GETTICK, &tick);
    return tick;
}

/* Group: picoRTOS cache maintenance API */

/* Function: picoRTOS_invalidate_dcache
 * Invalidates the data cache by address(es)
 *
 * Parameters:
 *  addr - The base address to invalidate
 *  n    - The size of the data to invalidate (in bytes)
 */
void picoRTOS_invalidate_dcache(const void *addr, size_t n)
{
    picoRTOS_assert(n > 0, return );
    struct syscall_cacheop op = { true, false, addr, n };

    arch_syscall(SYSCALL_CACHEOP, &op);
}

/* Function: picoRTOS_flush_dcache
 * Flushes the data cache by address(es)
 *
 * Parameters:
 *  addr - The base address to flush
 *  n    - The size of the data to flush (in bytes)
 */
void picoRTOS_flush_dcache(const void *addr, size_t n)
{
    picoRTOS_assert(n > 0, return );
    struct syscall_cacheop op = { false, true, addr, n };

    arch_syscall(SYSCALL_CACHEOP, &op);
}

/* Group: picoRTOS MPU API */

/* Function: picoRTOS_mpu_add_region
 * Adds a region to the MPU
 *
 * Parameters:
 *  addr - The base address of the region
 *  n    - The size of the region (in bytes)
 *  mode - The region mode as a mask (see picoRTOS_port.h)
 */
void picoRTOS_mpu_add_region(const void *addr, size_t n, unsigned mode)
{
    picoRTOS_assert(n > 0, return );
    struct syscall_mpu mpu = { addr, n, mode };

    arch_syscall(SYSCALL_MPU, &mpu);
}
