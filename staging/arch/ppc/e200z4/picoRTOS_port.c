#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#define INTC_BCR   ((volatile uint32_t*)(ADDR_INTC + 0))
#define INTC_CPR   ((volatile uint32_t*)(ADDR_INTC + 0x10))
#define INTC_IACKR ((volatile uint32_t*)(ADDR_INTC + 0x20))
#define INTC_EOIR  ((volatile uint32_t*)(ADDR_INTC + 0x30))
#define INTC_PSR   ((volatile uint16_t*)(ADDR_INTC + 0x60))

#define INTC_PSR_PRIN_M  0xfu
#define INTC_PSR_PRIN(x) ((x) & INTC_PSR_PRIN_M)

#define DEC_VALUE PICORTOS_CYCLES_PER_TICK

/* ASM */
/*@external@*/ extern /*@temp@*/
picoRTOS_stack_t *arch_save_first_context(picoRTOS_stack_t *sp,
                                          picoRTOS_task_fn_t fn,
                                          void *priv);

/*@external@*/ extern picoRTOS_cycles_t arch_DEC(void);
/*@external@*/ extern picoRTOS_core_t arch_core(void);
/*@external@*/ extern void arch_dcbi(uint32_t addr);
/*@external@*/ extern void arch_dcbf(uint32_t addr);
/*@external@*/ extern void arch_dec_init(uint32_t value);
/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_syscall(picoRTOS_syscall_t syscall, void *priv);
/*@external@*/ extern picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                                              picoRTOS_atomic_t old,
                                                              picoRTOS_atomic_t val);

/* INTC */
/*@external@*/ extern unsigned long __intc_vectors_start__[];

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
    ASM("wrteei 0");

    /* reset intc */
    *INTC_BCR = 0;
    *INTC_CPR = 0;
    *INTC_IACKR = (uint32_t)__intc_vectors_start__;

    /* init periodic timer */
    arch_dec_init((uint32_t)DEC_VALUE);
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

picoRTOS_stack_t *arch_prepare_stack(struct picoRTOS_task *task)
{
    /* PPCs have a decrementing stack */
    picoRTOS_stack_t *stack_top = task->stack + task->stack_count;
    picoRTOS_stack_t *sp = arch_save_first_context(stack_top, task->fn, task->priv);

    arch_flush_dcache(sp, (size_t)stack_top - (size_t)sp);
    return sp;
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

/* INTERRUPT OPS */

/*@external@*/
extern unsigned long ISR_TABLE_priv[DEVICE_INTERRUPT_VECTOR_COUNT];

void arch_register_interrupt(picoRTOS_irq_t irq, picoRTOS_isr_fn fn, void *priv)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT))
        return;

    __intc_vectors_start__[irq] = (unsigned long)fn;
    ISR_TABLE_priv[irq] = (unsigned long)priv;
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT))
        return;

    /* only support for priority 1 yet */
    int prc_sel = 0x8000 >> arch_core();

    INTC_PSR[irq] = (uint16_t)(prc_sel | INTC_PSR_PRIN(1));
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT))
        return;

    /* de-select core */
    int prc_sel = 0x8000 >> arch_core();

    INTC_PSR[irq] &= ~(uint16_t)prc_sel;
}

/* STAT OPS */

picoRTOS_cycles_t arch_counter(void)
{
    return (picoRTOS_cycles_t)DEC_VALUE - arch_DEC();
}

/* CACHE OPS */

#ifdef DEVICE_HAS_SOFTWARE_CACHE_COHERENCY
void arch_invalidate_dcache(void *addr, size_t n)
{
    if (!picoRTOS_assert_fatal(n > 0)) return;

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    uint32_t base = (uint32_t)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        arch_dcbi(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("mbar");
}

void arch_flush_dcache(void *addr, size_t n)
{
    if (!picoRTOS_assert_fatal(n > 0)) return;

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    uint32_t base = (uint32_t)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        arch_dcbf(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("mbar");
}

#else

void arch_invalidate_dcache(/*@unused@*/ void *addr __attribute__((unused)),
                            /*@unused@*/ size_t n __attribute__((unused)))
{
    ASM("mbar");
}

void arch_flush_dcache(/*@unused@*/ void *addr __attribute__((unused)),
                       /*@unused@*/ size_t n __attribute__((unused)))
{
    ASM("mbar");
}
#endif
