#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

#include <stdint.h>
#include <generated/autoconf.h>

#ifndef CONFIG_SMP_CORES
# define CONFIG_SMP_CORES 1
#endif

struct INTC {
    volatile uint32_t MCR;
    uint32_t RESERVED0;
    volatile uint32_t CPR[2];
    volatile uint32_t IACKR[2];
    volatile uint32_t EOIR[2];
    volatile uint8_t SSCIR[8];
    uint32_t RESERVED1[6];
    volatile uint8_t PSR[DEVICE_INTERRUPT_VECTOR_COUNT];
};

#define SSCIR_SET (1 << 1)
#define SSCIR_CLR (1 << 0)

#define PSR_PRC_SELn_M  0x3u
#define PSR_PRC_SELn(x) (((x) & PSR_PRC_SELn_M) << 6)
#define PSR_PRC_PRIn_M  0xfu
#define PSR_PRC_PRIn(x) ((x) & PSR_PRC_PRIn_M)

static struct INTC *INTC = (struct INTC*)ADDR_INTC;

/* ASM */
/*@external@*/ extern picoRTOS_core_t arch_core(void);

/*@external@*/ extern unsigned long __intc_vectors_start__[];
/*@external@*/ extern unsigned long ISR_TABLE_priv[DEVICE_INTERRUPT_VECTOR_COUNT];

void arch_intc_init(void)
{
    /* reset intc */
    size_t n = (size_t)CONFIG_SMP_CORES;

    INTC->MCR = 0;

    while (n-- != 0) {
        INTC->CPR[n] = 0;
        INTC->IACKR[n] = (uint32_t)__intc_vectors_start__;
    }
}

void arch_register_interrupt(picoRTOS_irq_t irq, arch_isr_fn fn, void *priv)
{
    arch_assert(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return );

    __intc_vectors_start__[irq] = (unsigned long)fn;
    ISR_TABLE_priv[irq] = (unsigned long)priv;
}

#define PRC_SELn_PROC_0 0x0
#define PRC_SELn_PROC_1 0x3

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return );

    uint8_t psr = (uint8_t)PSR_PRC_SELn(PRC_SELn_PROC_0);

    if (arch_core() != 0)
        psr = (uint8_t)PSR_PRC_SELn(PRC_SELn_PROC_1);

    /* only support for priority 1 yet */
    INTC->PSR[irq] = psr | (uint8_t)PSR_PRC_PRIn(1);
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    arch_assert(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return );

    /* disable by setting prio to 0 */
    INTC->PSR[irq] &= ~(uint8_t)PSR_PRC_PRIn(PSR_PRC_PRIn_M);
}

/* Beware:
 * These functions are called from the assembly only,
 * we need to setup the static analysis accordingly
 */

/*@external@*/
/* cppcheck-suppress [unusedFunction,unmatchedSuppression] */
uint32_t arch_get_interrupt(void)
{
    return INTC->IACKR[arch_core()];
}

/*@external@*/
/* cppcheck-suppress [unusedFunction,unmatchedSuppression] */
void arch_ack_interrupt(void)
{
    INTC->EOIR[arch_core()] = 0;
}

/* SMP */

#ifdef CONFIG_SMP
#include "picoRTOS-SMP_port.h"

void arch_propagate_tick(void)
{
    INTC->SSCIR[0] = (uint8_t)SSCIR_SET;
}

void arch_acknowledge_tick(void)
{
    INTC->SSCIR[0] = (uint8_t)SSCIR_CLR;
}

/* INTERRUPT OPS */

#define PRC_SELn_PROC_0        0x0
#define PRC_SELn_PROC_BOTH     0x1
#define PRC_SELn_PROC_1        0x3

void arch_smp_enable_interrupt(picoRTOS_irq_t irq,
                               picoRTOS_mask_t core_mask)
{
    arch_assert(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return );
    arch_assert(core_mask != 0, return );

    uint8_t psr = INTC->PSR[irq];
    int prc_seln = (int)(psr >> 6);
    int prin = (int)(psr & PSR_PRC_PRIn_M);

    if (prin == 0) {
        /* interrupt disabled */
        switch (core_mask) {
        case 0x1: psr = (uint8_t)PSR_PRC_SELn(PRC_SELn_PROC_0); break;
        case 0x2: psr = (uint8_t)PSR_PRC_SELn(PRC_SELn_PROC_1); break;
        default: psr = (uint8_t)PSR_PRC_SELn(PRC_SELn_PROC_BOTH); break;
        }
    }else{
        /* interrupt already enabled */
        switch (prc_seln) {
        case PRC_SELn_PROC_0:
            if ((core_mask & 0x2) != 0)
                psr = (uint8_t)PSR_PRC_SELn(PRC_SELn_PROC_BOTH);
            break;

        case PRC_SELn_PROC_BOTH:
            break;

        case PRC_SELn_PROC_1:
            if ((core_mask & 0x1) != 0)
                psr = (uint8_t)PSR_PRC_SELn(PRC_SELn_PROC_BOTH);
            break;

        default:
            arch_assert_void(false);
            break;
        }
    }

    /* only support for priority 1 yet */
    INTC->PSR[irq] = psr | (uint8_t)PSR_PRC_PRIn(1);
}

void arch_smp_disable_interrupt(picoRTOS_irq_t irq,
                                picoRTOS_mask_t core_mask)
{
    arch_assert(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return );
    arch_assert(core_mask != 0, return );

    uint8_t psr = INTC->PSR[irq];
    uint8_t prin = (uint8_t)PSR_PRC_PRIn(1);
    int prc_seln = (int)(psr >> 6);

    switch (prc_seln) {
    case PRC_SELn_PROC_0:
        if ((core_mask & 0x1) != 0)
            prin = 0; /* disable */
        break;

    case PRC_SELn_PROC_BOTH:
        if ((core_mask & 0x3) == (picoRTOS_mask_t)0x1)
            psr = (uint8_t)PSR_PRC_SELn(PRC_SELn_PROC_1);
        if ((core_mask & 0x3) == (picoRTOS_mask_t)0x2)
            psr = (uint8_t)PSR_PRC_SELn(PRC_SELn_PROC_0);
        if ((core_mask & 0x3) == (picoRTOS_mask_t)0x3)
            prin = 0; /* disable */
        break;

    case PRC_SELn_PROC_1:
        if ((core_mask & 0x2) != 0)
            prin = 0; /* disable */
        break;

    default:
        arch_assert_void(false);
        break;
    }

    /* only support for priority 1 yet */
    INTC->PSR[irq] = psr | prin;
}

#endif
