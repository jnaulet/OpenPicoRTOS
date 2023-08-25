#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <generated/autoconf.h>

/* SYSTICK */
#define SYSTICK_CSR ((volatile unsigned long*)0xe000e010)
#define SYSTICK_RVR ((volatile unsigned long*)0xe000e014)
#define SYSTICK_CVR ((volatile unsigned long*)0xe000e018)

#define SYSTICK_RVR_VALUE ((CONFIG_SYSCLK_HZ / CONFIG_TICK_HZ) - 1)

/* NVIC */
#define NVIC_ISER         ((volatile unsigned long*)0xe000e100)
#define NVIC_SHPR3        ((volatile unsigned long*)0xe000ed20)

/* VTOR */
#define VTOR ((volatile unsigned long*)0xe000ed08)

/* CACHES */
#define DCIMVAC ((volatile unsigned long*)0xe000ef5c)
#define DCCMVAC ((volatile unsigned long*)0xe000ef68)

/* ASM */
/*@external@*/ extern /*@temp@*/
picoRTOS_stack_t *arch_save_first_context(picoRTOS_stack_t *sp,
                                          arch_entry_point_fn fn,
                                          /*@null@*/ void *priv);

/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_syscall(picoRTOS_syscall_t syscall, void *priv);
/*@external@*/ extern picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                                              picoRTOS_atomic_t old,
                                                              picoRTOS_atomic_t val);

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
    ASM("cpsid if");

    /* set SYSTICK & PENDSV to min priority */
    *NVIC_SHPR3 |= 0xffff0000ul;

    /* SYSTICK */
    *SYSTICK_CSR = 0x6ul;                                   /* stop systick */
    *SYSTICK_CVR = 0;                                       /* reset */
    *SYSTICK_RVR = (unsigned long)SYSTICK_RVR_VALUE;        /* set period */
}

void arch_suspend(void)
{
    /* disable interrupts */
    ASM("cpsid if");
}

void arch_resume(void)
{
    /* enable interrupts */
    ASM("cpsie if");
}

picoRTOS_stack_t *arch_prepare_stack(picoRTOS_stack_t *stack,
                                     size_t stack_count,
                                     arch_entry_point_fn fn,
                                     void *priv)
{
    /* ARMs have a decrementing stack */
    return arch_save_first_context(stack + stack_count, fn, priv);
}

/* cppcheck-suppress constParameter */
void __attribute__((weak)) arch_idle(void *null)
{
    arch_assert_void(null == NULL);

    for (;;)
        ASM("wfe");
}

/* ATOMIC OPS */

picoRTOS_atomic_t arch_test_and_set(picoRTOS_atomic_t *ptr)
{
    return arch_compare_and_swap(ptr, 0, (picoRTOS_atomic_t)1);
}

/* INTERRUPT MANAGEMENT */

/*@external@*/
extern struct {
    arch_isr_fn fn;
    /*@temp@*/ /*@null@*/ void *priv;
} ISR_TABLE[DEVICE_INTERRUPT_VECTOR_COUNT];

void arch_register_interrupt(picoRTOS_irq_t irq, arch_isr_fn fn, void *priv)
{
    arch_assert(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return );
    ISR_TABLE[irq].fn = fn;
    ISR_TABLE[irq].priv = priv;
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return );
    NVIC_ISER[irq >> 5] |= (1ul << (0x1fu & irq));
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return );
    NVIC_ISER[irq >> 5] &= ~(1ul << (0x1fu & irq));
}

/* STATS */

picoRTOS_cycles_t arch_counter(void)
{
    return (picoRTOS_cycles_t)SYSTICK_RVR_VALUE -
           (picoRTOS_cycles_t)*SYSTICK_CVR;
}

/* CACHE OPS */

/* cppcheck-suppress [unusedFunction,unmatchedSuppression] */
void arch_invalidate_dcache(void *addr, size_t n)
{
    arch_assert_void(n > 0);

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    unsigned long base = (unsigned long)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        *DCIMVAC = (unsigned long)(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("dsb");
}

/* cppcheck-suppress [unusedFunction,unmatchedSuppression] */
void arch_flush_dcache(void *addr, size_t n)
{
    arch_assert_void(n > 0);

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    unsigned long base = (unsigned long)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        *DCCMVAC = (unsigned long)(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("dsb");
}
