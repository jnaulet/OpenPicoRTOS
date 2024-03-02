#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

/*@external@*/ extern /*@temp@*/
picoRTOS_stack_t *arch_save_first_context(picoRTOS_stack_t *sp,
                                          arch_entry_point_fn fn,
                                          /*@null@*/ void *priv);

/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_syscall(syscall_t syscall, void *priv);
/*@external@*/ extern void arch_timer_init(void);

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    arch_timer_init();
}

void arch_suspend(void)
{
    /* disable interrupts */
    ASM("csrci mstatus, 0x8"); /* ~MIE */
}

void arch_resume(void)
{
    /* enable interrupts */
    ASM("csrsi mstatus, 0x8"); /* MIE */
}

picoRTOS_stack_t *arch_prepare_stack(picoRTOS_stack_t *stack,
                                     size_t stack_count,
                                     arch_entry_point_fn fn,
                                     void *priv)
{
    /* RISC-Vs have a decrementing stack */
    return arch_save_first_context(stack + stack_count, fn, priv);
}

/* cppcheck-suppress constParameter */
void __attribute__((weak)) arch_idle(void *null)
{
    arch_assert_void(null == NULL);

    for (;;)
        ASM("wfi");
}

/* ATOMIC OPS (EMULATION) */

picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                        picoRTOS_atomic_t old,
                                        picoRTOS_atomic_t val)
{
    arch_suspend();

    if (*var == old) {
        *var = val;
        val = old;
    }

    arch_resume();
    return val;
}

picoRTOS_atomic_t arch_test_and_set(picoRTOS_atomic_t *ptr)
{
    return arch_compare_and_swap(ptr, (picoRTOS_atomic_t)0,
                                 (picoRTOS_atomic_t)1);
}

/* CACHES */

void arch_invalidate_dcache(/*@unused@*/ void *addr, /*@unused@*/ size_t n)
{
    /*@i@*/ (void)addr;
    /*@i@*/ (void)n;
}

void arch_flush_dcache(/*@unused@*/ void *addr, /*@unused@*/ size_t n)
{
    /*@i@*/ (void)addr;
    /*@i@*/ (void)n;
}
