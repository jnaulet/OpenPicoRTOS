#ifndef PICORTOS_PORT_H
#define PICORTOS_PORT_H

#include <stddef.h>
#include <stdbool.h>

#include "picoRTOS_types.h"
#include <generated/autoconf.h>

/**§
 * ## provided by picoRTOS
 */

typedef enum {
    /* task-related */
    SYSCALL_SEGFAULT    = 0,    /* W */
    SYSCALL_SLEEP       = 1,    /* W */
    SYSCALL_SLEEP_UNTIL = 2,    /* RW */
    SYSCALL_GETPID      = 3,    /* R */
    SYSCALL_MPU         = 4,    /* W */
    SYSCALL_KILL        = 5,    /* W */
    /* OS-related */
    SYSCALL_RUN         = 6,    /* W */
    SYSCALL_GETTICK     = 7,    /* R */
    SYSCALL_CACHEOP     = 8,    /* W */
    SYSCALL_IRQOP       = 9,    /* W */
    SYSCALL_COUNT
} syscall_t;

struct syscall_irqop {
    picoRTOS_irq_t irq;
    bool enable;
};

struct syscall_cacheop {
    bool invalidate;
    bool flush;
    /*@temp@*/ const void *addr;
    size_t n;
};

struct syscall_sleep_until {
    picoRTOS_tick_t ref;
    picoRTOS_tick_t period;
};

struct syscall_mpu {
    /*@temp@*/ const void *addr;
    size_t n;
    unsigned mode;
};

typedef unsigned mpu_mode_t;

/**
 * picoRTOS_stack_t \***picoRTOS_syscall**(**picoRTOS_stack_t** \*<ins>sp</ins>
 * **syscall_t** <ins>syscall</ins>, **void** \*<ins>priv</ins>):
 * > Executes a syscall
 * ### NOTES
 * > This **MUST** be called from a syscall interrupt or equivalent & provide
 * > the calling task's <ins>sp</ins> as the first parameter.
 * >
 * > On memory-protected systems, you have to call this with a SYSCALL_SEGFAULT
 * > <ins>syscall</ins> and provide a non-null <ins>priv</ins> parameter when there's
 * > an access or privilege violation.
 * >
 * > Usually, this is the address of the instruction or data that lead to the exception.
 * ### RETURN
 * > This call returns the stack you have to switch to (context restore)
 */
extern /*@exposed@*/
picoRTOS_stack_t *picoRTOS_syscall(picoRTOS_stack_t *sp,
                                   syscall_t syscall,
                                   /*@null@*/ void *priv);

/**
 * picoRTOS_stack_t \***picoRTOS_tick**(**picoRTOS_stack_t** *<ins>sp</ins>);
 * > Increments the tick & starts a new cycle
 * ### NOTES
 * > This **MUST** be called from your main tick timer interrupt and provide
 * > the current task's <ins>sp</ins> as a single parameter.
 * ### RETURN
 * > This call will return the next task sp to restore.
 */
extern /*@exposed@*/
picoRTOS_stack_t *picoRTOS_tick(picoRTOS_stack_t *sp);

typedef void (*arch_entry_point_fn)(void*);     /* entry point */
typedef void (*arch_isr_fn)(void*);             /* interrupt service routine */

#if !defined(NDEBUG)

/**
 * **arch_assert**(<ins>predicate</ins>, <ins>or_else</ins>);
 * > Asserts <ins>predicate</ins>, execute <ins>or_else</ins> if it fails
 * ### NOTES
 * > This macro is very similar to `picoRTOS_assert()` but can be used in your
 * > ports without creating a dependency issue.
 * >
 * > Remark: <ins>predicate</ins> **MUST** be side-effect free, this will be
 * > enforced by the static analysis.
 */
# define arch_assert(x, or_else)                \
    if (!(x)) {                                 \
        picoRTOS_core_sef(x);                   \
        arch_break(); /*@notreached@*/          \
        { or_else; }                            \
    }

/**
 * **arch_assert_void**(<ins>predicate</ins>);
 * > Asserts <ins>predicate</ins>
 * ### NOTES
 * > This macro is very similar to `picoRTOS_assert_void()` but can be used in
 * > your ports without creating a dependency issue.
 * >
 * > If the macro NDEBUG is defined at the moment <ins><picoRTOS_port.h></ins>
 * > is included, this macro generates no code.
 * >
 * > Remark: <ins>predicate</ins> **MUST** be side-effect free, this will be
 * > enforced by the static analysis.
 */
# define arch_assert_void(x)     \
    if (!(x)) {                  \
        picoRTOS_core_sef(x);    \
        arch_break();            \
    }

#else

# define arch_assert(x, or_else) if (!(x)) { or_else; }
# define arch_assert_void(x)

#endif

/**§
 * ## Provided by port
 */

/**
 * void **arch_init**(void);
 * > Architecture/port main initialization function
 * ### NOTES
 * > This function will be called by picoRTOS in `picoRTOS_start()`.
 * >
 * > Ports **MUST** provide this function in order to setup the tick timer,
 * > interrupts, and so on.
 */
extern void arch_init(void);


/**
 * void **arch_suspend**(void);
 * > Suspends the scheduling
 * ### NOTES
 * > This function will be called by `picoRTOS_syscall()`
 */
extern void arch_suspend(void);

/**
 * void **arch_resume**(void);
 * > Resumes the scheduling
 * ### NOTES
 * > This function will be called by `picoRTOS_syscall()`
 */
extern void arch_resume(void);

/**
 * picoRTOS_stack_t \***arch_prepare_stack**(**picoRTOS_stack_t** \*<ins>stack</ins>,
 * **size_t** <ins>stack_count</ins>, **arch_entry_point_fn** <ins>fn</ins>,
 * **void** \*<ins>priv</ins>);
 * > Prepares a task's stack for context restoration.
 * ### NOTES
 * > This function is used by `picoRTOS_add_task()` to prepare the stack
 * > for context restoration.<br>
 * > The return stack structure must match your `RESTORE_CONTEXT` procedure
 * > (see <<ins>picoRTOS_portasm.S</ins>> for more information).
 * ### RETURN
 * > This call **MUST** return a pointer to the first element of the newly
 * > prepared stack.
 */
extern /*@temp@*/
picoRTOS_stack_t *arch_prepare_stack(/*@returned@*/ picoRTOS_stack_t *stack,
                                     size_t stack_count,
                                     arch_entry_point_fn fn,
                                     /*@null@*/ void *priv);

/**
 * void **arch_start_first_stack**(**picoRTOS_stack_t** \*<ins>sp</ins>);
 * > Starts the first task on the system (usually *idle*) and
 * > bootstraps the scheduler.
 * ### NOTES
 * > This function is called by `picoRTOS_start()` and will restore
 * > the previously prepared <ins>sp</ins>.
 * >
 * > Additionnaly, you might want to start the timer here.
 */
extern /*@noreturn@*/ void arch_start_first_task(picoRTOS_stack_t *sp);

/**
 * void **arch_syscall**(**syscall_t** syscall, **void** \*<ins>priv</ins>);
 * > Port syscall function
 * ### NOTES
 * > Every [UNPRIVILEGED API](UNPRIVILEGED_API.md) call found in
 * > <<ins>picoRTOS_u.c</ins>> will trigger a syscall to hand things
 * > over to the kernel.
 * >
 * > On memory-protected systems, this call **SHOULD** trigger a
 * > privilege escalation (user -> supervisor).
 */
extern void arch_syscall(syscall_t syscall, /*@null@*/ void *priv);

/**
 * void **arch_idle**(void);
 * > Idle task function
 * ### NOTES
 * > This function **MUST** provide an infinite loop that puts the CPU in IDLE mode
 * >
 * > This function will be called by `arch_start_first_task()` after ther first
 * > context restoration.
 */
extern /*@noreturn@*/ void arch_idle(void);

/**§
 * ## Atomic operations
 */

/**
 * picoRTOS_atomic_t **arch_test_and_set**(**picoRTOS_atomic_t** \*<ins>ptr</ins>);
 * > Atomic test and set operation
 * ### NOTES
 * > if *<ins>ptr</ins> value is 0, its value is changed to 1 & the function returns 0.<br>
 * > if *<ins>ptr</ins> value is 1, the call does nothing & returns 1;
 */
extern /*@unused@*/ picoRTOS_atomic_t arch_test_and_set(picoRTOS_atomic_t *ptr);

/**
 * picoRTOS_atomic_t **arch_compare_and_swap**(**picoRTOS_atomic_t** \*<ins>var</ins>,
 * **picoRTOS_atomic_t** <ins>old</ins>, **picoRTOS_atomic_t** <ins>val</ins>);
 * > Atomic compare and swap operation
 * ### NOTES
 * > If *<ins>var</ins> & <ins>old</ins> are equal, *<ins>var</ins> is set to <ins>val</ins>
 * > and the call returns <ins>old</ins>.<br>
 * > If <ins>var</ins> & <ins>old</ins> are different, the call returns <ins>val</ins>.
 */
extern /*@unused@*/ picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                                            picoRTOS_atomic_t old,
                                                            picoRTOS_atomic_t val);

/**§
 * ## Interrupts
 */

/**
 * void **arch_register_interrupt**(**picoRTOS_irq_t** <ins>irq</ins>,
 * **arch_isr_fn** <ins>fn</ins>, **void** \*<ins>priv</ins>);
 * > Registers an interrupt on the system
 * ### NOTES
 * > Make sure that when <ins>irq</ins> is asserted, <ins>fn</ins> is
 * > called with <ins>priv</ins> as a parameter.
 */
extern /*@unused@*/ void arch_register_interrupt(picoRTOS_irq_t irq,
                                                 arch_isr_fn fn,
                                                 /*@null@*/ void *priv);

/**
 * void **arch_enable_interrupt**(**picoRTOS_irq_t** <ins>irq</ins>);
 * > Enables an <ins>irq</ins>
 * ### NOTES
 * > This function will be called by the relevant `picoRTOS_syscall()`
 */
extern /*@unused@*/ void arch_enable_interrupt(picoRTOS_irq_t irq);

/**
 * void **arch_disable_interrupt**(**picoRTOS_irq_t** <ins>irq</ins>);
 * > Disables an <ins>irq</ins>
 * ### NOTES
 * > This function will be called by the relevant `picoRTOS_syscall()`
 */
extern /*@unused@*/ void arch_disable_interrupt(picoRTOS_irq_t irq);

/**§
 * ## Statistics
 */

typedef enum {
    ARCH_COUNTER_CURRENT,
    ARCH_COUNTER_SINCE,
    ARCH_COUNTER_COUNT
} arch_counter_t;

/**
 * picoRTOS_cycles_t **arch_counter**(**arch_counter_t** <ins>counter</ins>,
 * **picoRTOS_cycles_t** <ins>t</ins>);
 * > Provides the current cpu counter value (optional)
 * ### NOTES
 * > picoRTOS will use this function to maintain tasks statistics in the kernel.<br>
 * > picoRTOS-lite doesn't use it at all.
 * >
 * > <ins>counter</ins> can be:
 * > - ARCH_COUNTER_CURRENT: returns the current counter value regardless of <ins>t</ins>
 * > - ARCH_COUNTER_SINCE: computes & returns the elapsed time between <ins>t</ins> and now.
 */
extern /*@external@*/ picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t);

/**§
 * ## Cache operations (*only if `CONFIG_CACHE`*)
 */

#ifdef CONFIG_CACHE
/**
 * void **arch_invalidate_dcache**(const **void** \*<ins>addr</ins>,
 * **size_t** <ins>n<ins>);
 * > Invalidates one or more lines of cache, from <ins>addr</ins> to
 * > <ins>addr + n</ins>
 */
extern void arch_invalidate_dcache(const void *addr, size_t n);

/**
 * void **arch_flush_dcache**(const **void** \*<ins>addr</ins>,
 * **size_t** <ins>n<ins>);
 * > Flushes one or more lines of cache to RAM, from <ins>addr</ins> to
 * > <ins>addr + n</ins>
 */
extern void arch_flush_dcache(const void *addr, size_t n);
#else
# define arch_invalidate_dcache(x, y) /*@ignore@*/ do { (void)(x); (void)(y); } while(false) /*@end@*/
# define arch_flush_dcache(x, y)      /*@i@*/ (void)(x)
#endif /* CONFIG_CACHE */

/**§
 * Clock operations
 */

/**
 * void **arch_set_clock_frequency**(**unsigned long** <ins>freq</ins>);
 * > Sets the current CPU input frequency (in hz)
 * ### NOTES
 * > This function is **NOT** called from picoRTOS itself, but **SHOULD**
 * > be called from any clock driver to ensure kernel & hardware clock(s)
 * > have the same parameters.
 */
extern /*@external@*/ void arch_set_clock_frequency(unsigned long freq);

/**
 * void **arch_delay_us**(**unsigned long** <ins>n</ins>);
 *> Busy waits for at least <ins>n</ins> microseconds
 */
extern /*@external@*/ void arch_delay_us(unsigned long n);

/**§
 * ## MPU Operations (*only if `CONFIG_MPU`*)
 */

#define PID_KERNEL -1

#define MM_NON_CACHEABLE (1u << 4)
#define MM_PRIVILEGED    (1u << 3)
#define MM_READ          (1u << 2)
#define MM_WRITE         (1u << 1)
#define MM_EXECUTE       (1u << 0)

#define MM_PRX (MM_PRIVILEGED|MM_READ|MM_EXECUTE)
#define MM_PRW (MM_PRIVILEGED|MM_READ|MM_WRITE)
#define MM_PRO (MM_PRIVILEGED|MM_READ)
#define MM_URX (MM_READ|MM_EXECUTE)
#define MM_URW (MM_READ|MM_WRITE)
#define MM_URO (MM_READ)

#ifdef CONFIG_MPU
/**
 * void **arch_mpu_init**(void);
 * > Initializes the Memory Protection Unit
 * ### NOTES
 * > This function is called during `picoRTOS_init()`
 */
extern void arch_mpu_init(void);

/**
 * void **arch_mpu_add_region**(**int** <ins>pid</ins>, const **void** \*<ins>addr</ins>,
 * **size_t** <ins>n</ins>, **mpu_mode_t** <ins>mode</ins>);
 * > Adds a memory region to the MPU
 * ### NOTES
 * > if <ins>pid</ins> is PID_KERNEL, the region is not linked to a task, but directly
 * > to the kernel itself.<br>
 * > <ins>mode</ins> is a mask, not an enum that can cumulate the following values:
 * >> MM_NON_CACHEABLE: the region is cache-inhibited<br>
 * >> MM_PRIVILIEGED: the region is only accessible to the kernel<br>
 * >> MM_READ: the region is readable<br>
 * >> MM_WRITE: the region is writable<br>
 * >> MM_EXECUTE: instructions can be fetched from this region
 */
extern void arch_mpu_add_region(int pid, const void *addr, size_t n, mpu_mode_t mode);

/**
 * void **arch_mpu_restore_regions**(**int** <ins>pid</ins>);
 * > Restores MPU regions for the task identified by <ins>pid</ins>
 * ### NOTES
 * > This will be called by picoRTOS before any context restoration
 */
extern void arch_mpu_restore_regions(int pid);

/**
 * void **arch_mpu_enable**(void);
 * > Enables the MPU*
 * ### NOTES
 * > Called by `picoRTOS_start()`
 */
extern void arch_mpu_enable(void);

#else
# define arch_mpu_init()
# define arch_mpu_add_region(a, b, c, d) /*@ignore@*/ do { (void)(a); (void)(b); (void)(c); (void)(d); } while(false) /*@end@*/
# define arch_mpu_restore_regions(x)     /*@i@*/ (void)(x)
# define arch_mpu_enable()
#endif /* CONFIG_MPU */

#endif
