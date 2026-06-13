#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>
#include <generated/autoconf.h>

/* ASM */
/*@external@*/ extern /*@temp@*/
picoRTOS_stack_t *arch_save_first_context(picoRTOS_stack_t *sp,
                                          arch_entry_point_fn fn,
                                          /*@null@*/ void *priv);

/*@external@*/ extern void arch_dcbi(uint32_t addr);
/*@external@*/ extern void arch_dcbf(uint32_t addr);
/*@external@*/ extern void arch_start_idle_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_syscall(syscall_t syscall, void *priv);
/*@external@*/ extern picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                                              picoRTOS_atomic_t old,
                                                              picoRTOS_atomic_t val);

/* DRIVERS */
/*@external@*/ extern void arch_intc_init(void);
/*@external@*/ extern void arch_timer_init(int period);
/*@external@*/ extern void arch_timer_start(void);
/*@external@*/ extern void arch_timer_stop(void);

/* CLOCK */
static int sysclk_hz = DEVICE_DEFAULT_SYSCLK_HZ;

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    ASM("wrteei 0");                                /* disable interrupts */
    arch_intc_init();                               /* reset intc */
    arch_timer_init(sysclk_hz / CONFIG_TICK_HZ);    /* timer */
}

void arch_suspend(void)
{
    arch_timer_stop();
}

void arch_resume(void)
{
    arch_timer_start();
}

picoRTOS_stack_t *arch_prepare_stack(picoRTOS_stack_t *stack,
                                     size_t stack_count,
                                     arch_entry_point_fn fn,
                                     void *priv)
{
    /* PPCs have a decrementing stack */
    picoRTOS_stack_t *stack_top = stack + stack_count;
    picoRTOS_stack_t *sp = arch_save_first_context(stack_top, fn, priv);

    arch_flush_dcache(sp, (size_t)stack_top - (size_t)sp);
    return sp;
}

void arch_start_first_task(picoRTOS_stack_t *sp)
{
    arch_timer_start();
    arch_start_idle_task(sp);
}

/* CLOCKS */

void arch_set_clock_frequency(unsigned long freq)
{
    arch_assert_void(freq != 0);
    sysclk_hz = (int)freq;
}

void arch_delay_us(unsigned long n)
{
    arch_assert_void(n != 0);

    unsigned long ncycles = n * ((unsigned long)sysclk_hz / 1000000ul);

    while (ncycles-- != 0)
        ASM("se_nop");
}

/* CACHE OPS */

#ifdef CONFIG_CACHE
void arch_invalidate_dcache(const void *addr, size_t n)
{
    arch_assert_void(n > 0);

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    uint32_t base = (uint32_t)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        arch_dcbi(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("mbar");
}

void arch_flush_dcache(const void *addr, size_t n)
{
    arch_assert_void(n > 0);

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    uint32_t base = (uint32_t)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        arch_dcbf(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("mbar");
}
#endif
