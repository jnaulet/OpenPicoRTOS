#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <generated/autoconf.h>

/* SYSTICK */
#define SYSTICK_CSR ((volatile unsigned long*)0xe000e010)
#define SYSTICK_RVR ((volatile unsigned long*)0xe000e014)
#define SYSTICK_CVR ((volatile unsigned long*)0xe000e018)

/* NVIC */
#define NVIC_ISER  ((volatile unsigned long*)0xe000e100)

/* SCB */
#define SCB_SHPR2 ((volatile unsigned long*)0xe000ed1c)
#define SCB_SHPR3 ((volatile unsigned long*)0xe000ed20)
#define SCB_SHCSR ((volatile unsigned long*)0xe000ed24)
#define SCB_CFSR  ((volatile unsigned long*)0xe000ed28)

#define SHCSR_USGFAULTENA (1 << 18)
#define SHCSR_BUSFAULTENA (1 << 17)
#define SHCSR_MEMFAULTENA (1 << 16)

/* VTOR */
#define VTOR ((volatile unsigned long*)0xe000ed08)

/* ASM */
/*@external@*/ extern /*@temp@*/
picoRTOS_stack_t *arch_save_first_context(picoRTOS_stack_t *sp,
                                          arch_entry_point_fn fn,
                                          /*@null@*/ void *priv);

/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_syscall(syscall_t syscall, void *priv);
/*@external@*/ extern picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                                              picoRTOS_atomic_t old,
                                                              picoRTOS_atomic_t val);

/* CLOCK */
static int sysclk_hz = DEVICE_DEFAULT_SYSCLK_HZ;

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
    ASM("cpsid i");

    /* set SYSTICK & SVC to max priority (no preempt) */
    *SCB_SHPR2 &= ~(0xffu << 24);
    *SCB_SHPR3 &= ~(0xffu << 24);
    /* enable all faults */
    *SCB_SHCSR |= (0x7u << 16);

    /* SYSTICK */
    *SYSTICK_CVR = 0;                                                   /* reset */
    *SYSTICK_RVR = (unsigned long)((sysclk_hz / CONFIG_TICK_HZ) - 1);   /* period */
    *SYSTICK_CSR = 0x7ul;                                               /* systick */
}

void arch_suspend(void)
{
    *SYSTICK_CSR &= ~0x1ul; /* stop systick */
}

void arch_resume(void)
{
    *SYSTICK_CSR |= 0x1ul;  /* restart systick */
}

picoRTOS_stack_t *arch_prepare_stack(picoRTOS_stack_t *stack,
                                     size_t stack_count,
                                     arch_entry_point_fn fn,
                                     void *priv)
{
    /* ARMs have a decrementing stack */
    return arch_save_first_context(stack + stack_count, fn, priv);
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

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)*SYSTICK_CVR;

    if (counter == ARCH_COUNTER_SINCE) {
        picoRTOS_cycles_t rvr = (picoRTOS_cycles_t)*SYSTICK_RVR;
        picoRTOS_cycles_t cvr = (picoRTOS_cycles_t)*SYSTICK_CVR;

        /* several cases here */
        if (t > rvr) return rvr + 1;            /* only used on first tick */
        if (cvr > t) return (rvr - cvr) + t;    /* rollover */
        /* normal */
        return t - cvr;
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
        ASM("");
}
