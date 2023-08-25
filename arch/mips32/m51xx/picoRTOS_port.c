#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>

/* ASM */

/*@external@*/ extern /*@temp@*/
picoRTOS_stack_t *arch_save_first_context(picoRTOS_stack_t *sp,
                                          arch_entry_point_fn fn,
                                          /*@null@*/ void *priv);

/*@external@*/ extern void arch_hit_invalidate_d(uint32_t addr);
/*@external@*/ extern void arch_hit_writeback_inv_d(uint32_t addr);

/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_syscall(picoRTOS_syscall_t syscall, void *priv);
/*@external@*/ extern picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                                              picoRTOS_atomic_t old,
                                                              picoRTOS_atomic_t val);

/* stats */
/*@external@*/ extern picoRTOS_cycles_t arch_counter(void);

/* intc */
/*@external@*/ extern void arch_init_intc(void);
/*@external@*/ extern void arch_ack_interrupt(picoRTOS_irq_t irq);
/*@external@*/ extern picoRTOS_irq_t arch_get_interrupt(void);

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
    ASM("di");
    arch_init_intc();
}

void arch_suspend(void)
{
    /* disable interrupts */
    ASM("di");
}

void arch_resume(void)
{
    /* enable interrupts */
    ASM("ei");
}

picoRTOS_stack_t *arch_prepare_stack(picoRTOS_stack_t *stack,
                                     size_t stack_count,
                                     arch_entry_point_fn fn,
                                     void *priv)
{
    /* MPIS32s have a decrementing stack */
    return arch_save_first_context(stack + stack_count, fn, priv);
}

/* cppcheck-suppress constParameter */
void __attribute__((weak)) arch_idle(void *null)
{
    arch_assert_void(null == NULL);

    for (;;)
        ASM("wait");
}

/* ATOMIC OPS */

picoRTOS_atomic_t arch_test_and_set(picoRTOS_atomic_t *ptr)
{
    return arch_compare_and_swap(ptr, 0, (picoRTOS_atomic_t)1);
}

/* CACHE */

/* cppcheck-suppress [unusedFunction,unmatchedSuppression] */
void arch_invalidate_dcache(void *addr, size_t n)
{
    arch_assert_void(n > 0);

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    uint32_t base = (uint32_t)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        arch_hit_invalidate_d(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("ehb");
}

/* cppcheck-suppress [unusedFunction,unmatchedSuppression] */
void arch_flush_dcache(void *addr, size_t n)
{
    arch_assert_void(n > 0);

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    uint32_t base = (uint32_t)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        arch_hit_writeback_inv_d(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("ehb");
}
