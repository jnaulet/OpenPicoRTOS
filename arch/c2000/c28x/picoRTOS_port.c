#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <generated/autoconf.h>

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

/* PIE */
struct PIEIxRn {
    volatile unsigned int IER;
    volatile unsigned int IFR;
};

#define PIEIxRn      ((volatile struct PIEIxRn*)(ADDR_PIECTRL + 0x2))
#define PIEVECTTABLE ((volatile unsigned long*)ADDR_PIEVECTTABLE)

/* ASM */
/*@external@*/ extern /*@temp@*/
picoRTOS_stack_t *arch_save_first_context(picoRTOS_stack_t *sp,
                                          arch_entry_point_fn fn,
                                          /*@null@*/ void *priv);

/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_syscall(syscall_t syscall, void *priv);
/*@external@*/ extern picoRTOS_atomic_t arch_test_and_set(picoRTOS_atomic_t *ptr);

/* CLOCK */
static unsigned long sysclk_hz = DEVICE_DEFAULT_SYSCLK_HZ;

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
    ASM(" setc INTM");

    /* periodic timer */
    CPUTIMER2->PRD = (unsigned long)((sysclk_hz / CONFIG_TICK_HZ) - 1);
    CPUTIMER2->TPR = 0;                 /* no prescaler */
    CPUTIMER2->TCR |= 0x30;             /* reload */
    CPUTIMER2->TCR &= ~0xc000;          /* stop */

    /* interrupt enable + start */
    CPUTIMER2->TCR |= 0x4000;
    CPUTIMER2->TCR &= ~0x10;
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

picoRTOS_stack_t *arch_prepare_stack(picoRTOS_stack_t *stack,
                                     size_t stack_count,
                                     arch_entry_point_fn fn,
                                     void *priv)
{
    arch_assert_void(stack_count >= (size_t)ARCH_MIN_STACK_COUNT);

    /* c28x has an incrementing stack */
    return arch_save_first_context(stack, fn, priv);
}

/* cppcheck-suppress constParameter */
void __attribute__((weak)) arch_idle(void *null)
{
    arch_assert_void(null == NULL);

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
    arch_isr_fn fn;
    /*@temp@*/ /*@null@*/ void *priv;
} ISR_TABLE[DEVICE_INTERRUPT_VECTOR_COUNT];

void arch_register_interrupt(picoRTOS_irq_t irq,
                             arch_isr_fn fn,
                             void *priv)
{
    arch_assert_void(irq >= (picoRTOS_irq_t)IRQ_INT1_1);
    arch_assert_void(irq <= (picoRTOS_irq_t)IRQ_INT12_16);

    ISR_TABLE[irq - IRQ_INT1_1].fn = fn;
    ISR_TABLE[irq - IRQ_INT1_1].priv = priv;
}

static void arch_set_irq_status(picoRTOS_irq_t irq, bool enable)
{
    arch_assert_void(irq >= (picoRTOS_irq_t)IRQ_INT1_1);
    arch_assert_void(irq <= (picoRTOS_irq_t)IRQ_INT12_16);

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
    arch_assert_void(irq >= (picoRTOS_irq_t)IRQ_INT1_1);
    arch_assert_void(irq <= (picoRTOS_irq_t)IRQ_INT12_16);

    arch_set_irq_status(irq, true);
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert_void(irq >= (picoRTOS_irq_t)IRQ_INT1_1);
    arch_assert_void(irq <= (picoRTOS_irq_t)IRQ_INT12_16);

    arch_set_irq_status(irq, false);
}

/* STATS */

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)CPUTIMER2->TIM;

    if (counter == ARCH_COUNTER_SINCE) {
        picoRTOS_cycles_t prd = (picoRTOS_cycles_t)CPUTIMER2->PRD;
        picoRTOS_cycles_t tim = (picoRTOS_cycles_t)CPUTIMER2->TIM;


        if (t > prd) return prd + 1;            /* 1st tick */
        if (tim > t) return (prd - tim) + t;    /* rollover */
        /* normal */
        return t - tim;
    }

    arch_assert_void(false);
    return 0;
}

/* CACHES */

void arch_invalidate_dcache(/*@unused@*/ void *addr, /*@unused@*/ size_t n)
{
    /*@i@*/ (void)addr;
    /*@i@*/ (void)n;
}

void arch_flush_dcache(/*@unused@*/ void *addr, /*@unused@*/ size_t n)
{
    /*@i@*/ (void)addr;
    /*@i@*/ (void)n;
}

/* CLOCKS */

void arch_set_clock_frequency(unsigned long freq)
{
    arch_assert_void(freq != 0);
    sysclk_hz = freq;
}

void arch_delay_us(unsigned long n)
{
    arch_assert_void(n != 0);

    unsigned long ncycles = n * (sysclk_hz / 1000000ul);

    while (ncycles-- != 0)
        ASM(" nop");
}
