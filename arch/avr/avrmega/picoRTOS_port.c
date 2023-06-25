#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

/* ASM */
/*@external@*/ extern /*@temp@*/
picoRTOS_stack_t *arch_save_first_context(picoRTOS_stack_t *sp,
                                          picoRTOS_task_fn_t fn,
                                          void *priv);

/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_SYSTICK(void *priv);

/* AVR is one of the rare CPUs that can switch contexts without an interrupt,
 * so this is directly defined in assembly language */
/*@external@*/ extern void arch_syscall(picoRTOS_syscall_t syscall, void *priv);

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
#define OCR (PICORTOS_CYCLES_PER_TICK - 1)

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

picoRTOS_stack_t *arch_prepare_stack(struct picoRTOS_task *task)
{
    /* AVRs have a decrementing stack */
    return arch_save_first_context(task->stack + task->stack_count,
                                   task->fn, task->priv);
}

/* cppcheck-suppress constParameter */
void __attribute__((weak)) arch_idle(void *null)
{
    if (!picoRTOS_assert_fatal(null == NULL)) return;

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
    picoRTOS_isr_fn fn;
    /*@temp@*/ /*@null@*/ void *priv;
} ISR_TABLE[DEVICE_INTERRUPT_VECTOR_COUNT];

void arch_register_interrupt(picoRTOS_irq_t irq, picoRTOS_isr_fn fn, void *priv)
{
    if (!picoRTOS_assert_fatal(irq > (picoRTOS_irq_t)1)) return;
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)(DEVICE_INTERRUPT_VECTOR_COUNT + 2))) return;

    ISR_TABLE[irq - 2].fn = fn;
    ISR_TABLE[irq - 2].priv = priv;
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT))
        return;

    /* no effect */
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT))
        return;

    /* no effect */
}

/* STATS */

picoRTOS_cycles_t arch_counter(void)
{
    return (picoRTOS_cycles_t)*AVR_TCNTn *
           (picoRTOS_cycles_t)PRESCALER;
}
