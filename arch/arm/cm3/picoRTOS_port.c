#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

/* SYSTICK */
#define SYSTICK_CSR ((volatile unsigned long*)0xe000e010)
#define SYSTICK_RVR ((volatile unsigned long*)0xe000e014)
#define SYSTICK_CVR ((volatile unsigned long*)0xe000e018)

#define SYSTICK_RVR_VALUE (PICORTOS_CYCLES_PER_TICK - 1)

/* NVIC */
#define NVIC_ISER         ((volatile unsigned long*)0xe000e100)
#define NVIC_ICSR         ((volatile unsigned long*)0xe000ed04)
#define NVIC_SHPR3        ((volatile unsigned long*)0xe000ed20)

/* VTOR */
#define VTOR ((volatile unsigned long*)0xe000ed08)

/* ASM */
/*@external@*/ extern void arch_SYSTICK(void);
/*@external@*/ extern void arch_PENDSV(void);
/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_syscall(picoRTOS_syscall_t syscall, void *priv);
/*@external@*/ extern picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                                              picoRTOS_atomic_t old,
                                                              picoRTOS_atomic_t val);

/* see cm0+ port for more info */
#ifdef DEVICE_MOVE_VTABLE_TO_RAM
/* vector table */
#define VTABLE_COUNT (16 + DEVICE_INTERRUPT_VECTOR_COUNT)
static unsigned long VTABLE[VTABLE_COUNT] __attribute__((aligned(128)));

static void move_vtable_to_ram(void)
{
    int n = VTABLE_COUNT;
    unsigned long *old_VTABLE = (unsigned long*)*VTOR;

    while (n-- != 0)
        VTABLE[n] = old_VTABLE[n];

    *VTOR = (unsigned long)VTABLE;
}
#endif

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
    ASM("cpsid i");

    /* INTERRUPTS */
#ifdef DEVICE_MOVE_VTABLE_TO_RAM
    move_vtable_to_ram();
#else
    unsigned long *VTABLE = (unsigned long*)*VTOR;
#endif
    VTABLE[14] = (unsigned long)arch_PENDSV;
    VTABLE[15] = (unsigned long)arch_SYSTICK;

    /* set SYSTICK & PENDSV to min priority */
    *NVIC_SHPR3 |= 0xffff0000ul;

    /* SYSTICK */
    *SYSTICK_CSR &= ~1ul;                                       /* stop systick */
    *SYSTICK_CVR = 0ul;                                         /* reset */
    *SYSTICK_RVR = (unsigned long)SYSTICK_RVR_VALUE;            /* set period */
    *SYSTICK_CSR |= 0x7ul;                                      /* enable interrupt & start */
}

void arch_suspend(void)
{
    /* disable interrupts */
    ASM("cpsid i");
}

void arch_resume(void)
{
    /* enable interrupts */
    ASM("cpsie i");
}

picoRTOS_stack_t *arch_prepare_stack(struct picoRTOS_task *task)
{
    /* ARMs have a decrementing stack */
    picoRTOS_stack_t *sp = task->stack + task->stack_count;

    /* ARM v6 reference manual, section B1.5.6 */
    sp -= ARCH_INITIAL_STACK_COUNT;

    sp[15] = (picoRTOS_stack_t)0x1000000;       /* xspr */
    sp[14] = (picoRTOS_stack_t)task->fn;        /* return address */
    sp[13] = (picoRTOS_stack_t)picoRTOS_start;  /* lr (r14) */
    /* sp[12-9] = r12, r3, r2 and r1 and */
    sp[8] = (picoRTOS_stack_t)task->priv;       /* r0 */
    /* sp[7-0] = r11..r4 */

    return sp;
}

/* cppcheck-suppress constParameter */
void __attribute__((weak)) arch_idle(void *null)
{
    if (!picoRTOS_assert_fatal(null == NULL)) return;

    for (;;)
        ASM("wfe");
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

/*@external@*/ extern void arch_NVIC_handler(void);

void arch_register_interrupt(picoRTOS_irq_t irq, picoRTOS_isr_fn fn, void *priv)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT))
        return;

#ifndef DEVICE_MOVE_VTABLE_TO_RAM
    unsigned long *VTABLE = (unsigned long*)*VTOR;
#endif

    VTABLE[16 + irq] = (unsigned long)arch_NVIC_handler;
    ISR_TABLE[irq].fn = fn;
    ISR_TABLE[irq].priv = priv;
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT))
        return;

    NVIC_ISER[irq >> 5] |= (1ul << (0x1fu & irq));
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT))
        return;

    NVIC_ISER[irq >> 5] &= ~(1ul << (0x1fu & irq));
}

/* STATS */

picoRTOS_cycles_t arch_counter(void)
{
    return (picoRTOS_cycles_t)SYSTICK_RVR_VALUE -
           (picoRTOS_cycles_t)*SYSTICK_CVR;
}
