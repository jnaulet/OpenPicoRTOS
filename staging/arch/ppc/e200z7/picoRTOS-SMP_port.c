#include "picoRTOS-SMP.h"
#include "picoRTOS_device.h"
#include "picoRTOS-SMP_port.h"

#include <stdint.h>

#ifndef CONFIG_DEADLOCK_COUNT
# error CONFIG_DEADLOCK_COUNT is not defined
#endif

#define INTC_CPR    ((volatile uint32_t*)(ADDR_INTC + 0x8))
#define INTC_IACKR  ((volatile uint32_t*)(ADDR_INTC + 0x10))
#define INTC_SSCIR0 ((volatile uint8_t*)(ADDR_INTC + 0x20))
#define INTC_PSR    ((volatile uint8_t*)(ADDR_INTC + 0x40))

#define INTC_SSCIR0_SET (1 << 1)
#define INTC_SSCIR0_CLR (1 << 0)

#define INTC_PSR_PRC_SELn_M  0x3u
#define INTC_PSR_PRC_SELn(x) (((x) & INTC_PSR_PRC_SELn_M) << 5)
#define INTC_PSR_PRC_PRIn_M  0xfu
#define INTC_PSR_PRC_PRIn(x) ((x) & INTC_PSR_PRC_PRIn_M)

#define SEMA4_GATE0 ((volatile uint8_t*)(ADDR_SEMA4 + 0))
#define SEMA4_RSTGT ((volatile uint16_t*)(ADDR_SEMA4 + 0x100))

#define SEMA42_RSTGT_W_RSTGDP_M  0xffu
#define SEMA42_RSTGT_W_RSTGDP(x) (((x) & SEMA42_RSTGT_W_RSTGDP_M) << 8)

#define SIU_HLT1    ((volatile uint32_t*)(ADDR_SIU + 0x9a4))
#define SIU_RSTVEC1 ((volatile uint32_t*)(ADDR_SIU + 0x9b0))

#define SIU_RSTVEC1_VLE (1 << 0)

/* ASM */
/*@external@*/ extern picoRTOS_core_t arch_core(void);
/*@external@*/ extern void arch_core_start(void);
/*@external@*/ extern void arch_SSCIR0(void *priv);

/* STACK */
/*@external@*/ extern picoRTOS_stack_t *arch_core_sp[CONFIG_SMP_CORES - 1];

/* INTC */
/*@external@*/ extern unsigned long __intc_vectors_start__[];

static void smp_intc_init(void)
{
    size_t n = (size_t)CONFIG_SMP_CORES;

    while (n-- != 0) {
        INTC_CPR[n] = 0;
        INTC_IACKR[n] = (uint32_t)__intc_vectors_start__;
    }
}

void arch_smp_init(void)
{
    /* single-core init first */
    arch_init();

    /* reset spinlock0 */
    *SEMA4_RSTGT = (uint16_t)SEMA42_RSTGT_W_RSTGDP(0xe2);
    *SEMA4_RSTGT = (uint16_t)SEMA42_RSTGT_W_RSTGDP(0x1d);

    /* init whole intc */
    smp_intc_init();
}

void arch_core_init(picoRTOS_core_t core,
                    picoRTOS_stack_t *stack,
                    size_t stack_count,
                    picoRTOS_stack_t *sp)
{
    if (!picoRTOS_assert_fatal(core > 0)) return;
    if (!picoRTOS_assert_fatal(core < (picoRTOS_core_t)CONFIG_SMP_CORES)) return;
    if (!picoRTOS_assert_fatal(stack != NULL)) return;
    if (!picoRTOS_assert_fatal(stack_count >= (size_t)ARCH_MIN_STACK_COUNT)) return;
    if (!picoRTOS_assert_fatal(sp != NULL)) return;

    /* prepare core main stack */
    stack += (stack_count - (size_t)2);
    *stack = (picoRTOS_stack_t)sp;

    /* store in exchange ram */
    arch_core_sp[core - 1] = stack;

    /* enable propagated tick interrupt */
    arch_register_interrupt((picoRTOS_irq_t)IRQ_SSCIR0, arch_SSCIR0, NULL);
    arch_smp_enable_interrupt((picoRTOS_irq_t)IRQ_SSCIR0,
                              (picoRTOS_mask_t)1 << core);

    /* start (vle mode) */
    *SIU_RSTVEC1 = (uint32_t)arch_core_start | SIU_RSTVEC1_VLE;
}

void arch_spin_lock(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;
    picoRTOS_core_t core = arch_core() + 1;

    while (deadlock-- != 0) {
        *SEMA4_GATE0 = (uint8_t)core;
        if (*SEMA4_GATE0 == (uint8_t)core)
            break;
    }

    /* deadlock */
    if (!picoRTOS_assert_fatal(deadlock != -1))
        return;
}

void arch_spin_unlock(void)
{
    *SEMA4_GATE0 = 0;
}

void arch_propagate_tick(void)
{
    *INTC_SSCIR0 = (uint8_t)INTC_SSCIR0_SET;
}

void arch_acknowledge_tick(void)
{
    *INTC_SSCIR0 = (uint8_t)INTC_SSCIR0_CLR;
}

/* INTERRUPT OPS */

#define PRC_SELn_PROC_0        0x0
#define PRC_SELn_PROC_BOTH     0x1
#define PRC_SELn_PROC_1        0x3

void arch_smp_enable_interrupt(picoRTOS_irq_t irq,
                               picoRTOS_mask_t core_mask)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;
    if (!picoRTOS_assert_fatal(core_mask != 0)) return;

    uint8_t psr = INTC_PSR[irq];
    int prc_seln = (int)(psr >> 6);

    switch (prc_seln) {
    case PRC_SELn_PROC_0:
        if ((core_mask & 0x2) != 0)
            psr = (uint8_t)INTC_PSR_PRC_SELn(PRC_SELn_PROC_BOTH);
        break;

    case PRC_SELn_PROC_BOTH:
        break;

    case PRC_SELn_PROC_1:
        if ((core_mask & 0x1) != 0)
            psr = (uint8_t)INTC_PSR_PRC_SELn(PRC_SELn_PROC_BOTH);
        break;

    default:
        picoRTOS_assert_void_fatal(false);
        break;
    }

    /* only support for priority 1 yet */
    INTC_PSR[irq] = psr | (uint8_t)INTC_PSR_PRC_PRIn(1);
}

void arch_smp_disable_interrupt(picoRTOS_irq_t irq,
                                picoRTOS_mask_t core_mask)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;
    if (!picoRTOS_assert_fatal(core_mask != 0)) return;

    uint8_t psr = INTC_PSR[irq];
    uint8_t prin = (uint8_t)INTC_PSR_PRC_PRIn(1);
    int prc_seln = (int)(psr >> 6);

    switch (prc_seln) {
    case PRC_SELn_PROC_0:
        if ((core_mask & 0x1) != 0)
            prin = 0; /* disable */
        break;

    case PRC_SELn_PROC_BOTH:
        if ((core_mask & 0x3) == (picoRTOS_mask_t)0x1)
            psr = (uint8_t)INTC_PSR_PRC_SELn(PRC_SELn_PROC_1);
        if ((core_mask & 0x3) == (picoRTOS_mask_t)0x2)
            psr = (uint8_t)INTC_PSR_PRC_SELn(PRC_SELn_PROC_0);
        if ((core_mask & 0x3) == (picoRTOS_mask_t)0x3)
            prin = 0; /* disable */
        break;

    case PRC_SELn_PROC_1:
        if ((core_mask & 0x2) != 0)
            prin = 0; /* disable */
        break;

    default:
        picoRTOS_assert_void_fatal(false);
        break;
    }

    /* only support for priority 1 yet */
    INTC_PSR[irq] = psr | prin;
}
