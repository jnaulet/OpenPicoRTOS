#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <generated/autoconf.h>

/* ASM */
/*@external@*/ extern /*@temp@*/
picoRTOS_stack_t *arch_save_first_context(picoRTOS_stack_t *sp,
                                          arch_entry_point_fn fn,
                                          /*@null@*/ void *priv);

/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_SYSTICK(void *priv);

/* AVR is one of the rare CPUs that can switch contexts without an interrupt,
 * so this is directly defined in assembly language */
/*@external@*/ extern void arch_syscall(syscall_t syscall, void *priv);

#define IRQ_SYSTICK IRQ_TIMER0_COMPA

/* systick registers (8bit TC0) */
#define AVR_TIMSKn ((volatile unsigned char*)ADDR_TIMSK0)
#define AVR_TCCRnA ((volatile unsigned char*)(ADDR_TC0 + 0))
#define AVR_TCCRnB ((volatile unsigned char*)(ADDR_TC0 + 1))
#define AVR_TCNTn  ((volatile unsigned char*)(ADDR_TC0 + 2))
#define AVR_OCRnA  ((volatile unsigned char*)(ADDR_TC0 + 3))

/* SETUP */
static void timer_setup(void)
{
#define OCR ((CONFIG_SYSCLK_HZ / CONFIG_TICK_HZ) - 1)

    *AVR_TCCRnA = (unsigned char)(1 << 1); /* CTC mode */
    *AVR_TCCRnB = (unsigned char)0;
    *AVR_TCNTn = (unsigned char)0;
    /* output compare A match interrupt enable */
    *AVR_TIMSKn |= (unsigned char)(1u << 1);

    /* prescaler computation */
#if OCR < 0x100
# define PRESCALER 1
    *AVR_TCCRnB |= (unsigned char)1; /* no prescaler */
#elif OCR < 0x800
# define PRESCALER 8
    *AVR_TCCRnB |= (unsigned char)2; /* div8 prescaler */
#elif OCR < 0x4000
# define PRESCALER 64
    *AVR_TCCRnB |= (unsigned char)3; /* div64 prescaler */
#elif OCR < 0x10000
# define PRESCALER 256
    *AVR_TCCRnB |= (unsigned char)4; /* div256 prescaler */
#elif OCR < 0x40000
# define PRESCALER 1024
    *AVR_TCCRnB |= (unsigned char)5; /* div1024 prescaler */
#else
# error Cannot compute systick
#endif

    *AVR_OCRnA = (unsigned char)(OCR / PRESCALER);
}

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
    ASM("cli");

    arch_register_interrupt((picoRTOS_irq_t)IRQ_SYSTICK,
                            arch_SYSTICK, NULL);

    /* TIMER5 */
    timer_setup();
}

void arch_suspend(void)
{
    /* disable tick */
    ASM("cli");
}

void arch_resume(void)
{
    /* enable tick */
    ASM("sei");
}

picoRTOS_stack_t *arch_prepare_stack(picoRTOS_stack_t *stack,
                                     size_t stack_count,
                                     arch_entry_point_fn fn,
                                     void *priv)
{
    /* AVRs have a decrementing stack */
    return arch_save_first_context(stack + stack_count, fn, priv);
}

/* cppcheck-suppress constParameter */
void __attribute__((weak)) arch_idle(void *null)
{
    arch_assert_void(null == NULL);

    for (;;)
        ASM("sleep");
}

/* ATOMIC OPS EMULATION */

picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                        picoRTOS_atomic_t old,
                                        picoRTOS_atomic_t val)
{
    ASM("cli");

    if (*var == old) {
        *var = val;
        val = old;
    }

    ASM("sei");
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
    arch_assert_void(irq > (picoRTOS_irq_t)1);
    arch_assert_void(irq < (picoRTOS_irq_t)(DEVICE_INTERRUPT_VECTOR_COUNT + 2));

    ISR_TABLE[irq - 2].fn = fn;
    ISR_TABLE[irq - 2].priv = priv;
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert_void(irq > (picoRTOS_irq_t)1);
    arch_assert_void(irq < (picoRTOS_irq_t)(DEVICE_INTERRUPT_VECTOR_COUNT + 2));
    /* no effect */
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert_void(irq > (picoRTOS_irq_t)1);
    arch_assert_void(irq < (picoRTOS_irq_t)(DEVICE_INTERRUPT_VECTOR_COUNT + 2));
    /* no effect */
}

/* STATS */

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)*AVR_TCNTn;

    if (counter == ARCH_COUNTER_SINCE) {
        picoRTOS_cycles_t ocr = (picoRTOS_cycles_t)*AVR_OCRnA;
        picoRTOS_cycles_t tcnt = (picoRTOS_cycles_t)*AVR_TCNTn;

        if (t >= ocr) return ocr;           /* only used on first tick */
        if (tcnt < t) return ocr - t;       /* rollover */
        /* normal */
        return tcnt - t;
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
}

void arch_delay_us(unsigned long n)
{
    arch_assert_void(n != 0);

    unsigned long ncycles = n * ((unsigned long)CONFIG_SYSCLK_HZ / 1000000ul);

    while (ncycles-- != 0)
        ASM("nop");
}
