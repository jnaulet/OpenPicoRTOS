#include "picoRTOS-SMP.h"
#include "picoRTOS_device.h"
#include "picoRTOS-SMP_port.h"

#include <stdint.h>

#ifndef CONFIG_DEADLOCK_COUNT
# error CONFIG_DEADLOCK_COUNT is not defined
#endif

#define INTC_CPR    ((volatile uint32_t*)(ADDR_INTC + 0x10))
#define INTC_IACKR  ((volatile uint32_t*)(ADDR_INTC + 0x20))
#define INTC_SSCIR0 ((volatile uint8_t*)(ADDR_INTC + 0x40))
#define INTC_PSR    ((volatile uint16_t*)(ADDR_INTC + 0x60))

#define INTC_SSCIR0_SET (1 << 1)
#define INTC_SSCIR0_CLR (1 << 0)

#define INTC_PSR_PRC_SELN0 (1 << 15)
#define INTC_PSR_PRC_SELN1 (1 << 14)
#define INTC_PSR_PRC_SELN2 (1 << 13)

#define INTC_PSR_PRIN_M  0xfu
#define INTC_PSR_PRIN(x) ((x) & INTC_PSR_PRIN_M)

#define SEMA42_GATE0 ((volatile uint8_t*)(ADDR_SEMA42 + 0))
#define SEMA42_RSTGT ((volatile uint16_t*)(ADDR_SEMA42 + 0x40))

#define SEMA42_RSTGT_W_RSTGDP_M  0xffu
#define SEMA42_RSTGT_W_RSTGDP(x) (((x) & SEMA42_RSTGT_W_RSTGDP_M) << 8)

#define MC_ME_GS    ((volatile uint32_t*)(ADDR_MC_ME + 0))
#define MC_ME_MCTL  ((volatile uint32_t*)(ADDR_MC_ME + 0x4))
#define MC_ME_CCTL  ((volatile uint16_t*)(ADDR_MC_ME + 0x1c6))
#define MC_ME_CADDR ((volatile uint32_t*)(ADDR_MC_ME + 0x1e4))

#define MC_ME_GS_S_MTRANS (1 << 27)

#define MC_ME_MCTL_TARGET_MODE_M  0xfu
#define MC_ME_MCTL_TARGET_MODE(x) (((x) & MC_ME_MCTL_TARGET_MODE_M) << 24)
#define MC_ME_MCTL_KEY            0x5af0
#define MC_ME_MCTL_INVERTED_KEY   0xa50f

#define MC_ME_CCTL_RUN3  (1 << 7)
#define MC_ME_CCTL_RUN2  (1 << 6)
#define MC_ME_CCTL_RUN1  (1 << 5)
#define MC_ME_CCTL_RUN0  (1 << 4)
#define MC_ME_CCTL_DRUN  (1 << 3)
#define MC_ME_CCTL_SAFE  (1 << 2)
#define MC_ME_CCTL_RESET (1 << 0)

#define MC_ME_CADDR_RMC (1 << 0)

#define SIUL2_MIDR2 ((volatile uint32_t*)(ADDR_SIUL2 + 0x8))

/* ASM */
/*@external@*/ extern picoRTOS_core_t arch_core(void);
/*@external@*/ extern void arch_disable_dcache(void);
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
    *SEMA42_RSTGT = (uint16_t)SEMA42_RSTGT_W_RSTGDP(0xe2);
    *SEMA42_RSTGT = (uint16_t)SEMA42_RSTGT_W_RSTGDP(0x1d);

    /* init whole intc */
    smp_intc_init();

    /* disable dcache (no cache sync on e200z4) */
    arch_disable_dcache();
}

static void transition_complete_busywait(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while (deadlock-- != 0)
        if ((*MC_ME_GS & MC_ME_GS_S_MTRANS) == 0)
            break;

    if (!picoRTOS_assert_fatal(deadlock != -1))
        return;
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

    uint32_t mctl = *MC_ME_MCTL;

    /* zero key, keep target mode */
    mctl &= MC_ME_MCTL_TARGET_MODE(MC_ME_MCTL_TARGET_MODE_M);

    /* prepare core main stack */
    stack += (stack_count - (size_t)2);
    *stack = (picoRTOS_stack_t)sp;

    /* store in exchange ram */
    arch_core_sp[core - 1] = stack;

    /* enable propagated tick interrupt */
    arch_register_interrupt((picoRTOS_irq_t)IRQ_SSCIR0, arch_SSCIR0, NULL);
    arch_smp_enable_interrupt((picoRTOS_irq_t)IRQ_SSCIR0,
                              (picoRTOS_mask_t)1 << core);

    /* run in all modes & start */
    MC_ME_CADDR[core] = (uint32_t)arch_core_start | MC_ME_CADDR_RMC;
    MC_ME_CCTL[core] = (uint16_t)(MC_ME_CCTL_RUN3 | MC_ME_CCTL_RUN2 | MC_ME_CCTL_RUN1 |
                                  MC_ME_CCTL_RUN0 | MC_ME_CCTL_DRUN | MC_ME_CCTL_SAFE);

    /* trigger start */
    *MC_ME_MCTL = mctl | MC_ME_MCTL_KEY;
    *MC_ME_MCTL = mctl | MC_ME_MCTL_INVERTED_KEY;

    /* wait for transition complete */
    transition_complete_busywait();
}

void arch_spin_lock(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;
    picoRTOS_core_t core = arch_core() + 1;

    while (deadlock-- != 0) {
        *SEMA42_GATE0 = (uint8_t)core;
        if (*SEMA42_GATE0 == (uint8_t)core)
            break;
    }

    /* deadlock */
    if (!picoRTOS_assert_fatal(deadlock != -1))
        return;
}

void arch_spin_unlock(void)
{
    *SEMA42_GATE0 = 0;
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

static uint16_t prc_sel_from_mask(picoRTOS_mask_t core_mask)
{
    if (!picoRTOS_assert_fatal(core_mask != 0)) return 0;

    uint16_t prc_sel = 0;

    prc_sel |= ((0x1 & core_mask) != 0 ? INTC_PSR_PRC_SELN0 : 0);
    prc_sel |= ((0x2 & core_mask) != 0 ? INTC_PSR_PRC_SELN1 : 0);
    prc_sel |= ((0x4 & core_mask) != 0 ? INTC_PSR_PRC_SELN2 : 0);

    return prc_sel;
}

void arch_smp_enable_interrupt(picoRTOS_irq_t irq,
                               picoRTOS_mask_t core_mask)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;
    if (!picoRTOS_assert_fatal(core_mask != 0)) return;

    /* force to prio 1 */
    INTC_PSR[irq] = prc_sel_from_mask(core_mask) |
                    (uint16_t)INTC_PSR_PRIN(1);
}

void arch_smp_disable_interrupt(picoRTOS_irq_t irq,
                                picoRTOS_mask_t core_mask)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;
    if (!picoRTOS_assert_fatal(core_mask != 0)) return;

    /* de-select core(s) */
    INTC_PSR[irq] &= ~prc_sel_from_mask(core_mask);
}
