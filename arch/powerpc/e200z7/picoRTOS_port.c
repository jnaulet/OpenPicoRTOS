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
/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_syscall(picoRTOS_syscall_t syscall, void *priv);
/*@external@*/ extern picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                                              picoRTOS_atomic_t old,
                                                              picoRTOS_atomic_t val);

/* DRIVERS */
/*@external@*/ extern void arch_intc_init(void);

/* SPRS */
/*@external@*/ extern uint32_t arch_mfdec(void);
/*@external@*/ extern uint32_t arch_mfdecar(void);
/*@external@*/ extern uint32_t arch_mftcr(void);
/*@external@*/ extern uint32_t arch_mfhid0(void);
/*@external@*/ extern void arch_mtdec(uint32_t value);
/*@external@*/ extern void arch_mtdecar(uint32_t value);
/*@external@*/ extern void arch_mttcr(uint32_t value);
/*@external@*/ extern void arch_mthid0(uint32_t value);

#define HID0_TBEN       (1 << 14)
#define HID0_SEL_TBLCLK (1 << 13)

#define TCR_DIE (1 << 26)
#define TCR_ARE (1 << 22)

/* CLOCK */
static int sysclk_hz = DEVICE_DEFAULT_SYSCLK_HZ;

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
    ASM("wrteei 0");

    /* reset intc */
    arch_intc_init();

    /* setup decrementer */
    arch_mtdecar((uint32_t)(sysclk_hz / CONFIG_TICK_HZ) - 1);               /* reload value */
    arch_mthid0(arch_mfhid0() & (uint32_t) ~(HID0_TBEN | HID0_SEL_TBLCLK)); /* clock cpu */
    arch_mttcr(arch_mftcr() | (uint32_t)(TCR_DIE | TCR_ARE));               /* auto-reload */
    arch_mtdec(arch_mfdecar());                                             /* initial value */
}

void arch_suspend(void)
{
    /* disable interrupts */
    ASM("wrteei 0");
}

void arch_resume(void)
{
    /* enable interrupts */
    ASM("wrteei 1");
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

/* STAT OPS */

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)arch_mfdec();

    if (counter == ARCH_COUNTER_SINCE) {
        picoRTOS_cycles_t dec = (picoRTOS_cycles_t)arch_mfdec();
        picoRTOS_cycles_t decar = (picoRTOS_cycles_t)arch_mfdecar();

        if (t > decar) return decar + 1;        /* only used on first tick */
        if (dec > t) return (decar - dec) + t;  /* rollover */
        /* normal */
        return t - dec;
    }

    arch_assert_void(false);
    return 0;
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

/* cppcheck-suppress [unusedFunction,unmatchedSuppression] */
void arch_invalidate_dcache(void *addr, size_t n)
{
    arch_assert_void(n > 0);

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    uint32_t base = (uint32_t)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        arch_dcbi(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("mbar");
}

/* cppcheck-suppress [unusedFunction,unmatchedSuppression] */
void arch_flush_dcache(void *addr, size_t n)
{
    arch_assert_void(n > 0);

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    uint32_t base = (uint32_t)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        arch_dcbf(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("mbar");
}
