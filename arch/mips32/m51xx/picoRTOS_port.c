#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#define INTC_INTCON ((volatile uint32_t*)(ADDR_INTC + 0x0))
#define INTC_IFSn   ((volatile uint32_t*)(ADDR_INTC + 0x40))
#define INTC_IECn   ((volatile uint32_t*)(ADDR_INTC + 0xc0))
#define INTC_IPCn   ((volatile uint32_t*)(ADDR_INTC + 0x140))

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

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
    ASM("di");

    /* set multi-vector mode */
    *INTC_INTCON |= (1 << 12);

    /* enable interrupts */
    INTC_IFSn[0] &= ~0x1;
    INTC_IECn[0] |= 0x1;
    INTC_IPCn[0] |= 0x5;
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

/* INTERRUPT MANAGEMENT */

/*@external@*/
extern struct {
    picoRTOS_isr_fn fn;
    /*@temp@*/ /*@null@*/ void *priv;
} ISR_TABLE[DEVICE_INTERRUPT_VECTOR_COUNT];

void arch_register_interrupt(picoRTOS_irq_t irq, picoRTOS_isr_fn fn, void *priv)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;

    ISR_TABLE[irq].fn = fn;
    ISR_TABLE[irq].priv = priv;
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;

    /* find the correct IEC offset */
    size_t IEC_index = (size_t)(irq >> 5);
    size_t IEC_bit = (size_t)(0x1fu & irq);

    /* find the correct IPC offset */
    size_t IPC_index = (size_t)(irq >> 6);
    size_t IPC_shift = (size_t)((0x1u & irq) << 4);

    /* Beware: indexes have to * 4 cause of microchip clear/set/invert structure
     * TODO: find a way to make this more generic and portable */
    INTC_IFSn[IEC_index << 2] &= ~(1 << IEC_bit);
    INTC_IPCn[IPC_index << 2] |= (0x5 << IPC_shift);
    INTC_IECn[IEC_index << 2] |= (1 << IEC_bit);
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;

    /* find the correct IEC offset */
    size_t IEC_index = (size_t)(irq >> 5);
    size_t IEC_bit = (size_t)(0x1f & irq);

    /* Beware: indexes have to * 4 cause of microchip clear/set/invert structure
     * TODO: find a way to make this more generic and portable */
    INTC_IECn[IEC_index << 2] &= ~(1 << IEC_bit);
}

/* CACHE */

#ifdef DEVICE_HAS_SOFTWARE_CACHE_COHERENCY
void arch_invalidate_dcache(void *addr, size_t n)
{
    if (!picoRTOS_assert_fatal(n > 0)) return;

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    uint32_t base = (uint32_t)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        arch_hit_invalidate_d(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("ehb");
}

void arch_flush_dcache(void *addr, size_t n)
{
    if (!picoRTOS_assert_fatal(n > 0)) return;

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    uint32_t base = (uint32_t)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        arch_hit_writeback_inv_d(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("ehb");
}
#else
void arch_invalidate_dcache(/*@unused@*/ void *addr __attribute__((unused)),
                            /*@unused@*/ size_t n __attribute__((unused)))
{
}

void arch_flush_dcache(/*@unused@*/ void *addr __attribute__((unused)),
                       /*@unused@*/ size_t n __attribute__((unused)))
{
}
#endif
