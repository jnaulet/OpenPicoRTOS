#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#define TIMER_PERIOD (PICORTOS_CYCLES_PER_TICK - 1)

/* ASM */
/*@external@*/ extern void arch_SYSTICK(void*);
/*@external@*/ extern void arch_init_timer(unsigned long period);
/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_syscall(picoRTOS_syscall_t syscall, void *priv);
/*@external@*/ extern picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                                              picoRTOS_atomic_t old,
                                                              picoRTOS_atomic_t val);

/* stats */
/*@external@*/ extern picoRTOS_cycles_t arch_counter(void);

void arch_init(void)
{
    /* disable interrupts */
    ASM("rsil a2, 0");

    /* register timer interrupt */
    arch_register_interrupt((picoRTOS_irq_t)IRQ_TIMER0, arch_SYSTICK, NULL);
    arch_enable_interrupt((picoRTOS_irq_t)IRQ_TIMER0);

    /* start timer */
    arch_init_timer((unsigned long)TIMER_PERIOD);
}

void arch_suspend(void)
{
    ASM("rsil a2, 0");
}

void arch_resume(void)
{
    ASM("rsil a2, 1");
}

picoRTOS_stack_t *arch_prepare_stack(struct picoRTOS_task *task)
{
    /* LX6 have decrementing stack */
    picoRTOS_stack_t *sp = task->stack + task->stack_count;

    sp -= ARCH_INITIAL_STACK_COUNT;

    /* According to Xtensa ISA, this is user-defined */

    sp[15] = (picoRTOS_stack_t)task->fn;        /* return address (a0) */
    sp[14] = (picoRTOS_stack_t)task->priv;      /* param (a2) */

    return sp;
}

void arch_idle(void *null)
{
    picoRTOS_assert_fatal(null == NULL);

    for (;;)
        ASM("waiti 0");
}

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

/*@external@*/ extern unsigned long arch_intenable(unsigned long);

void arch_register_interrupt(picoRTOS_irq_t irq, picoRTOS_isr_fn fn, void *priv)
{
    picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

    ISR_TABLE[irq].fn = fn;
    ISR_TABLE[irq].priv = priv;
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

    unsigned long mask = arch_intenable(0);

    (void)arch_intenable(mask | (1 << irq));
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

    unsigned long mask = arch_intenable(0);

    (void)arch_intenable(mask & ~(1 << irq));
}
