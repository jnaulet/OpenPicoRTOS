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

/* CACHES */
#ifdef ADDR_CMCC
# define CMCC_CTRL ((volatile unsigned long*)(ADDR_CMCC + 0x8))
#endif

/* ASM */
/*@external@*/ extern void arch_SYSTICK(void);
/*@external@*/ extern void arch_PENDSV(void);
/*@external@*/ extern void arch_enable_vfp(void);
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
    ASM("cpsid if");

    /* CACHES (Cortex M Cache Controller) */
#ifdef DEVICE_ENABLE_CMCC
    *CMCC_CTRL = 0x1ul;
#endif

    /* VFP */
    arch_enable_vfp();

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
    ASM("cpsid if");
}

void arch_resume(void)
{
    /* enable interrupts */
    ASM("cpsie if");
}

picoRTOS_stack_t *arch_prepare_stack(struct picoRTOS_task *task)
{
    /* ARMs have a decrementing stack */
    picoRTOS_stack_t *sp = task->stack + task->stack_count;

    /* ARM v6 reference manual, section B1.5.6 */
    sp -= ARCH_INITIAL_STACK_COUNT;

    /* sp[49] = reserved */
    /* sp[48] = fpscr */
    /* sp[47-32] = s15...s0 */

    sp[31] = (picoRTOS_stack_t)0x1000000;       /* xspr */
    sp[30] = (picoRTOS_stack_t)task->fn;        /* return address */
    sp[29] = (picoRTOS_stack_t)picoRTOS_start;  /* lr (r14) */
    /* sp[28-25] = r12, r3, r2 and r1 and */
    sp[24] = (picoRTOS_stack_t)task->priv;      /* r0 */

    /* sp[23-16] = r11..r4 */
    /* sp[15-0] = fpu s16...s31 */

    return sp;
}

void arch_idle(void *null)
{
    picoRTOS_assert_fatal(null == NULL);

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
    picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

#ifndef DEVICE_MOVE_VTABLE_TO_RAM
    unsigned long *VTABLE = (unsigned long*)*VTOR;
#endif

    VTABLE[16 + irq] = (unsigned long)arch_NVIC_handler;
    ISR_TABLE[irq].fn = fn;
    ISR_TABLE[irq].priv = priv;
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

    NVIC_ISER[irq >> 5] |= (1ul << (0x1fu & irq));
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT);

    NVIC_ISER[irq >> 5] &= ~(1ul << (0x1fu & irq));
}

/* STATS */

picoRTOS_cycles_t arch_counter(void)
{
    return (picoRTOS_cycles_t)SYSTICK_RVR_VALUE -
           (picoRTOS_cycles_t)*SYSTICK_CVR;
}
