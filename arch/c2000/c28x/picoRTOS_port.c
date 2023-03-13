#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

/*
 * Based on IvanZuy's previous work on FreeRTOS, check it out at:
 * https://github.com/IvanZuy/freertos_c28x
 */

struct C28X_CPUTIMER {
    volatile unsigned long TIM;
    volatile unsigned long PRD;
    volatile unsigned long TCR;
    volatile unsigned long TPR;
};

static struct C28X_CPUTIMER *CPUTIMER2 =
    (struct C28X_CPUTIMER*)ADDR_CPUTIMER2;

#define CPUTIMER2_PRD_VALUE (PICORTOS_CYCLES_PER_TICK - 1)

/* PIE */
struct PIEIxRn {
    volatile unsigned int IER;
    volatile unsigned int IFR;
};

#define PIECTRL      ((volatile unsigned int*)(ADDR_PIECTRL + 0x0))
#define PIEIxRn      ((volatile struct PIEIxRn*)(ADDR_PIECTRL + 0x2))
#define PIEVECTTABLE ((volatile unsigned long*)ADDR_PIEVECTTABLE)

/* ASM */
/*@external@*/ extern /*@temp@*/
picoRTOS_stack_t *arch_save_first_context(picoRTOS_stack_t *sp,
                                          picoRTOS_task_fn_t fn,
                                          void *priv);

/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_syscall(picoRTOS_syscall_t syscall, void *priv);
/*@external@*/ extern picoRTOS_atomic_t arch_test_and_set(picoRTOS_atomic_t *ptr);

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
    ASM(" setc INTM");

    /* periodic timer */
    CPUTIMER2->PRD = (unsigned long)CPUTIMER2_PRD_VALUE;
    CPUTIMER2->TPR = 0;                 /* no prescaler */
    CPUTIMER2->TCR |= 0x30;             /* reload */
    CPUTIMER2->TCR &= ~0xc000;          /* stop */

    /* interrupt enable + start */
    CPUTIMER2->TCR |= 0x4000;
    CPUTIMER2->TCR &= ~0x10;

    /* enable PIE */
    *PIECTRL |= 0x1;
}

void arch_suspend(void)
{
    /* disable interrupts */
    ASM(" setc INTM");
}

void arch_resume(void)
{
    /* enable interrupts */
    ASM(" clrc INTM");
}

picoRTOS_stack_t *arch_prepare_stack(struct picoRTOS_task *task)
{
    /* c28x has an incrementing stack */
    return arch_save_first_context(task->stack, task->fn, task->priv);
}

/* cppcheck-suppress constParameter */
void __attribute__((weak)) arch_idle(void *null)
{
    if (!picoRTOS_assert_fatal(null == NULL)) return;

    for (;;)
        ASM(" idle");
}

/* ATOMIC ops */

picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                        picoRTOS_atomic_t old,
                                        picoRTOS_atomic_t val)
{
    /* Warning: this is a stretch */
    static picoRTOS_atomic_t lock = 0;
    picoRTOS_atomic_t res = val;

    if (arch_test_and_set(&lock) == 0) {
        res = *var;
        if (res == old)
            *var = val;

        lock = 0;
    }

    return res;
}

/* ARCH: INTERRUPTS */

/*@external@*/
extern struct {
    picoRTOS_isr_fn fn;
    /*@temp@*/ /*@null@*/ void *priv;
} ISR_TABLE[DEVICE_INTERRUPT_VECTOR_COUNT];

void arch_register_interrupt(picoRTOS_irq_t irq,
                             picoRTOS_isr_fn fn,
                             void *priv)
{
    if (!picoRTOS_assert_fatal(irq >= (picoRTOS_irq_t)IRQ_INT1_1)) return;
    if (picoRTOS_assert_fatal(irq <= (picoRTOS_irq_t)IRQ_INT12_16)) return;

    ISR_TABLE[irq - IRQ_INT1_1].fn = fn;
    ISR_TABLE[irq - IRQ_INT1_1].priv = priv;
}

static void arch_set_irq_status(picoRTOS_irq_t irq, bool enable)
{
    if (!picoRTOS_assert_fatal(irq >= (picoRTOS_irq_t)IRQ_INT1_1)) return;
    if (!picoRTOS_assert_fatal(irq <= (picoRTOS_irq_t)IRQ_INT12_16)) return;

    size_t bit;
    size_t index;

    if (irq < (picoRTOS_irq_t)IRQ_INT1_9) {
        /* for IRQ_INTn_[0-8] */
        irq -= (picoRTOS_irq_t)IRQ_INT1_9;
        bit = (size_t)(0x7u & irq);
        index = (size_t)(irq >> 3);
    }else{
        /* for IRQ_INTn_[9-16] */
        bit = (size_t)(0x7u & irq) + (size_t)8;
        index = (size_t)((0x7fu & irq) >> 3);
    }

    ASM(" eallow");
    if (enable) PIEIxRn[index].IER |= (1u << bit);
    else PIEIxRn[index].IER &= ~(1u << bit);
    ASM(" edis");
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq >= (picoRTOS_irq_t)IRQ_INT1_1)) return;
    if (!picoRTOS_assert_fatal(irq <= (picoRTOS_irq_t)IRQ_INT12_16)) return;

    arch_set_irq_status(irq, true);
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq >= (picoRTOS_irq_t)IRQ_INT1_1)) return;
    if (!picoRTOS_assert_fatal(irq <= (picoRTOS_irq_t)IRQ_INT12_16)) return;

    arch_set_irq_status(irq, false);
}

picoRTOS_cycles_t arch_counter(void)
{
    return (picoRTOS_cycles_t)CPUTIMER2_PRD_VALUE -
           (picoRTOS_cycles_t)CPUTIMER2->TIM;
}
