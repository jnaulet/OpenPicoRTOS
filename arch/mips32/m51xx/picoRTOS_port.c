#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>

/* ASM */

/*@external@*/ extern /*@temp@*/
picoRTOS_stack_t *arch_save_first_context(picoRTOS_stack_t *sp,
                                          picoRTOS_task_fn_t fn,
                                          void *priv);

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

picoRTOS_stack_t *arch_prepare_stack(struct picoRTOS_task *task)
{
    /* MPIS32s have a decrementing stack */
    return arch_save_first_context(task->stack + task->stack_count,
                                   task->fn, task->priv);
}

/* cppcheck-suppress constParameter */
void __attribute__((weak)) arch_idle(void *null)
{
    if (!picoRTOS_assert_fatal(null == NULL)) return;

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
    if (!picoRTOS_assert_fatal(n > 0)) return;

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    uint32_t base = (uint32_t)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        arch_hit_invalidate_d(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("ehb");
}

/* cppcheck-suppress [unusedFunction,unmatchedSuppression] */
void arch_flush_dcache(void *addr, size_t n)
{
    if (!picoRTOS_assert_fatal(n > 0)) return;

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    uint32_t base = (uint32_t)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        arch_hit_writeback_inv_d(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("ehb");
}
