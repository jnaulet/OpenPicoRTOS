#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#define INTC_INTCON ((volatile unsigned long*)(ADDR_INTC + 0x0))
#define INTC_IFSn   ((volatile unsigned long*)(ADDR_INTC + 0x40))
#define INTC_IECn   ((volatile unsigned long*)(ADDR_INTC + 0xc0))
#define INTC_IPCn   ((volatile unsigned long*)(ADDR_INTC + 0x140))
#define INTC_OFFn   ((volatile unsigned long*)(ADDR_INTC + 0x540))

/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_syscall(picoRTOS_syscall_t syscall, void *priv);
/*@external@*/ extern picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                                              picoRTOS_atomic_t old,
                                                              picoRTOS_atomic_t val);

/* interrupts */
/*@external@*/ extern void arch_CORE_TIMER(void);
/*@external@*/ extern void arch_SYSCALL(void);

/* stats */
/*@external@*/ extern picoRTOS_cycles_t arch_counter(void);

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
    ASM("di");

    /* register vector table & timer interrupt */
    INTC_OFFn[0] = (unsigned long)arch_CORE_TIMER;
    INTC_OFFn[1] = (unsigned long)arch_SYSCALL;

    /* set multi-vector mode */
    *INTC_INTCON |= (1ul << 12);

    /* enable interrupts */
    INTC_IFSn[0] &= ~0x3;
    INTC_IECn[0] |= 0x3;
    INTC_IPCn[0] |= 0x505;
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
    picoRTOS_stack_t *sp = task->stack + task->stack_count;

    sp -= ARCH_INITIAL_STACK_COUNT;

    sp[2] = (picoRTOS_stack_t)task->priv;           /* A0 */
    sp[25] = (picoRTOS_stack_t)picoRTOS_start;      /* RA */
    sp[28] = (picoRTOS_stack_t)task->fn;            /* EPC */

    return sp;
}

void arch_idle(void *null)
{
    picoRTOS_assert_fatal(null == NULL);

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

/*@external@*/
extern void arch_EIC_handler(void);

void arch_register_interrupt(picoRTOS_irq_t irq, picoRTOS_isr_fn fn, void *priv)
{
    picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

    ISR_TABLE[irq].fn = fn;
    ISR_TABLE[irq].priv = priv;
    INTC_OFFn[irq] = (unsigned long)arch_EIC_handler;
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

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
    picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

    /* find the correct IEC offset */
    size_t IEC_index = (size_t)(irq >> 5);
    size_t IEC_bit = (size_t)(0x1f & irq);

    /* Beware: indexes have to * 4 cause of microchip clear/set/invert structure
     * TODO: find a way to make this more generic and portable */
    INTC_IECn[IEC_index << 2] &= ~(1 << IEC_bit);
}
