#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <generated/autoconf.h>

/* ASM */
/*@external@*/ extern /*@temp@*/
picoRTOS_stack_t *arch_save_first_context(picoRTOS_stack_t *sp,
                                          arch_entry_point_fn fn,
                                          /*@null@*/ void *priv);

/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);

/* 8051 is one of the rare CPUs that can switch contexts without an interrupt,
 * so this is directly defined in assembly language */
/*@external@*/ extern void arch_syscall(picoRTOS_syscall_t syscall, void *priv);

/* SFRs */
#ifndef S_SPLINT_S
__sfr __at(ADDR_CKCON)    CKCON;
__sfr __at(ADDR_T2CON)    T2CON;
__sfr __at(ADDR_RCAP2L)   RCAP2L;
__sfr __at(ADDR_RCAP2H)   RCAP2H;
__sfr __at(ADDR_TL2)      TL2;
__sfr __at(ADDR_TH2)      TH2;
__sfr __at(ADDR_xIE_wET2) xIE;
#else
static unsigned char CKCON;
static unsigned char T2CON;
static unsigned char RCAP2L;
static unsigned char RCAP2H;
static unsigned char TL2;
static unsigned char TH2;
static unsigned char xIE;
#endif

#define CKCON_T2M (1 << 5)

#define T2CON_TF2   (1 << 7)
#define T2CON_EXF2  (1 << 6)
#define T2CON_RCLK  (1 << 5)
#define T2CON_TCLK  (1 << 4)
#define T2CON_EXEN2 (1 << 3)
#define T2CON_TR2   (1 << 2)
#define T2CON_CT2   (1 << 1)
#define T2CON_CPRL2 (1 << 0)

/* CLOCK */
static unsigned long sysclk_hz = DEVICE_DEFAULT_SYSCLK_HZ;

/* SETUP */
static void timer_setup(void)
{
    unsigned long value = 0x10000ul -
                          ((sysclk_hz / (unsigned long)CONFIG_TICK_HZ) >> 2);

    /* init counters */
    TL2 = (unsigned char)value;
    TH2 = (unsigned char)(value >> 8);
    RCAP2L = (unsigned char)value;
    RCAP2H = (unsigned char)(value >> 8);

    CKCON |= CKCON_T2M;                 /* div by 4 */
    T2CON = (unsigned char)T2CON_TR2;   /* start, auto-reload */

    /* enable timer2 interrupt */
    xIE |= xIE_ET2;
}

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
    ASM(" clr ea");

    /* TIMER2 */
    timer_setup();
}

void arch_suspend(void)
{
    /* disable tick */
    ASM(" clr ea");
}

void arch_resume(void)
{
    /* enable tick */
    ASM(" setb ea");
}

picoRTOS_stack_t *arch_prepare_stack(picoRTOS_stack_t *stack,
                                     size_t stack_count,
                                     arch_entry_point_fn fn,
                                     void *priv)
{
    arch_assert_void(stack_count >= (size_t)ARCH_MIN_STACK_COUNT);

    /* 8051 has an incrementing stack */
    return arch_save_first_context(stack, fn, priv);
}

/* cppcheck-suppress constParameter */
void arch_idle(void *null)
{
    arch_assert_void(null == NULL);

    for (;;)
        /* ASM(" mov pcon, 0x31"); */
        ASM(" nop");
}

/* ATOMIC OPS EMULATION */

picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                        picoRTOS_atomic_t old,
                                        picoRTOS_atomic_t val)
{
    ASM(" push ie");
    ASM(" clr ea");

    if (*var == old) {
        *var = val;
        val = old;
    }

    ASM(" pop ie");
    return val;
}

picoRTOS_atomic_t arch_test_and_set(picoRTOS_atomic_t *ptr)
{
    return arch_compare_and_swap(ptr, (picoRTOS_atomic_t)0,
                                 (picoRTOS_atomic_t)1);
}

/* INTERRUPTS MANAGEMENT */

/*@external@*/
extern struct {
    arch_isr_fn fn;
    /*@temp@*/ /*@null@*/ void *priv;
} ISR_TABLE[DEVICE_INTERRUPT_VECTOR_COUNT];

void arch_register_interrupt(picoRTOS_irq_t irq, arch_isr_fn fn, void *priv)
{
    arch_assert_void(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

    ISR_TABLE[irq].fn = fn;
    ISR_TABLE[irq].priv = priv;
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert_void(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);
    /* TBD */
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert_void(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);
    /* TBD */
}

/* STATS */

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    picoRTOS_cycles_t t2 = (picoRTOS_cycles_t)TH2 << 8 | (picoRTOS_cycles_t)TL2;
    picoRTOS_cycles_t rcap2 = (picoRTOS_cycles_t)RCAP2H << 8 | (picoRTOS_cycles_t)RCAP2L;

    if (counter == ARCH_COUNTER_CURRENT)
        return t2 - rcap2;

    if (counter == ARCH_COUNTER_SINCE) {
        unsigned int elapsed = t2 - rcap2;
        picoRTOS_tick_t period = (picoRTOS_cycles_t)0xffffu - rcap2 + 1;

        if (t > period) return period + 1;                  /* 1st tick */
        if (elapsed > t) return (period - elapsed) + t;     /* overflow */
        /* normal */
        return t - elapsed;
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

/* CLOCK */

void arch_set_clock_frequency(unsigned long freq)
{
    arch_assert_void(freq != 0);
    sysclk_hz = freq;
}

void arch_delay_us(unsigned long n)
{
    arch_assert_void(n != 0);

    /* 4 cycles per instruction in modern 8051s */
    unsigned long ncycles = (n * (sysclk_hz / 1000000ul)) >> 2;

    while (ncycles-- != 0)
        ASM(" nop");
}
