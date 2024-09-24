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
/*@external@*/ extern void arch_syscall(syscall_t syscall, void *priv);
/*@external@*/ extern picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                                              picoRTOS_atomic_t old,
                                                              picoRTOS_atomic_t val);

/* stats */
/*@external@*/ extern unsigned long arch_cp0_count(void);

/* intc */
/*@external@*/ extern void arch_init_intc(void);
/*@external@*/ extern void arch_ack_interrupt(picoRTOS_irq_t irq);
/*@external@*/ extern picoRTOS_irq_t arch_get_interrupt(void);

/* CLOCK */
/*@external@*/ extern unsigned long timer_core_period;
static unsigned long timer_core_hz = DEVICE_DEFAULT_SYSCLK_HZ;

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
    ASM("di");
    arch_init_intc();

    /* setup timer */
    timer_core_period = timer_core_hz / (unsigned long)CONFIG_TICK_HZ;
    timer_core_period /= 2ul; /* on MIPS counter only increments every other clock */
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

void __attribute__((weak)) arch_idle(const void *null)
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

/* STATS */

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)arch_cp0_count();

    if (counter == ARCH_COUNTER_SINCE)
        return (picoRTOS_cycles_t)arch_cp0_count() - t;

    arch_assert_void(false);
    return 0;
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

/* CLOCK */

void arch_set_clock_frequency(unsigned long freq)
{
    arch_assert_void(freq != 0);

    timer_core_hz = freq;
    timer_core_period = (freq / (unsigned long)CONFIG_TICK_HZ) / 2ul;
}

void arch_delay_us(unsigned long n)
{
    arch_assert_void(n != 0);

    unsigned long ncycles = n * (timer_core_hz / 1000000ul);

    while (ncycles-- != 0)
        ASM("nop");
}
