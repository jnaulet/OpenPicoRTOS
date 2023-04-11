#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

/* ASM */
/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_SYSTICK(void *priv);

/* AVR is one of the rare CPUs that can switch contexts without an interrupt,
 * so this is directly defined in assembly language */
/*@external@*/ extern void arch_syscall(picoRTOS_syscall_t syscall, void *priv);

/* RTC registers */
#define RTC_CTRLA   ((volatile unsigned char*)(ADDR_RTC + 0x0))
#define RTC_INTCTRL ((volatile unsigned char*)(ADDR_RTC + 0x2))
#define RTC_CLKSEL  ((volatile unsigned char*)(ADDR_RTC + 0x7))
#define RTC_CNT     ((volatile unsigned int*)(ADDR_RTC + 0x8))
#define RTC_PER     ((volatile unsigned int*)(ADDR_RTC + 0xa))

/* SETUP */
static void rtc_setup(void)
{
    *RTC_INTCTRL = (unsigned char)0x1;  /* OVF */
    *RTC_CLKSEL = (unsigned char)0;     /* INT32K */
    *RTC_CNT = (unsigned char)0;

    /* set period & start */
    *RTC_PER = (unsigned int)(PICORTOS_CYCLES_PER_TICK - 1);
    *RTC_CTRLA = (unsigned char)0x1;
}

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
    ASM("cli");

    arch_register_interrupt((picoRTOS_irq_t)IRQ_RTC_CNT,
                            arch_SYSTICK, NULL);

    /* RTC/PIT */
    rtc_setup();
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
    /* stack is decrementing */
    picoRTOS_stack_t *sp = task->stack + task->stack_count;

    sp -= ARCH_INTIAL_STACK_COUNT;

    /* tinyAVR 1-series has 16-bit pc */
    sp[34] = (picoRTOS_stack_t)(unsigned int)task->fn;
    sp[33] = (picoRTOS_stack_t)((unsigned int)task->fn >> 8);

    sp[30] = (unsigned char)0x0;                                /* r0 */
    sp[29] = (picoRTOS_stack_t)(1u << 7);                       /* sreg (int enable) */

    sp[5] = (picoRTOS_stack_t)(unsigned int)task->priv;         /* r24 */
    sp[4] = (picoRTOS_stack_t)((unsigned int)task->priv >> 8);  /* r25 */

    return sp - 1;                                              /* pop pre-increments */
}

/* cppcheck-suppress constParameter */
void __attribute__((weak)) arch_idle(void *null)
{
    if (!picoRTOS_assert_fatal(null == NULL)) return;

    for (;;)
        ASM("sleep");
}

/* ATOMIC OPS EMULATION */
#ifdef CONFIG_ARCH_EMULATE_ATOMIC
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
#endif

/* INTERRUPTS MANAGEMENT */

/*@external@*/
extern struct {
    picoRTOS_isr_fn fn;
    /*@temp@*/ /*@null@*/ void *priv;
} ISR_TABLE[DEVICE_INTERRUPT_VECTOR_COUNT];

void arch_register_interrupt(picoRTOS_irq_t irq, picoRTOS_isr_fn fn, void *priv)
{
    if (!picoRTOS_assert_fatal(irq > (picoRTOS_irq_t)0)) return;
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)(DEVICE_INTERRUPT_VECTOR_COUNT + 1))) return;

    ISR_TABLE[irq - 1].fn = fn;
    ISR_TABLE[irq - 1].priv = priv;
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;
    /* no effect */
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;
    /* no effect */
}

/* STATS */

picoRTOS_cycles_t arch_counter(void)
{
    return (picoRTOS_cycles_t)*RTC_CNT;
}
