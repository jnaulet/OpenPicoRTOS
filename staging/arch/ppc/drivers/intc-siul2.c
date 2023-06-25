#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#ifndef CONFIG_SMP_CORES
# define CONFIG_SMP_CORES 1
#endif

struct INTC {
    volatile uint32_t BCR;
    volatile uint32_t MPROT;
    uint32_t RESERVED0[2];
    volatile uint32_t CPR[3];
    uint32_t RESERVED1;
    volatile uint32_t IACKR[3];
    uint32_t RESERVED2;
    volatile uint32_t EOIR[3];
    uint32_t RESERVED3;
    volatile uint8_t SSCIR[24];
    uint32_t RESERVED4[2];
    volatile uint16_t PSR[DEVICE_INTERRUPT_VECTOR_COUNT];
};

#define SSCIR_SET (1 << 1)
#define SSCIR_CLR (1 << 0)

#define PSR_PRC_SELN0 (1 << 15)
#define PSR_PRC_SELN1 (1 << 14)
#define PSR_PRC_SELN2 (1 << 13)
#define PSR_PRIN_M    0xfu
#define PSR_PRIN(x)   ((x) & PSR_PRIN_M)

static struct INTC *INTC = (struct INTC*)ADDR_INTC;

/* ASM */
/*@external@*/ extern picoRTOS_core_t arch_core(void);

/*@external@*/ extern unsigned long __intc_vectors_start__[];
/*@external@*/ extern unsigned long ISR_TABLE_priv[DEVICE_INTERRUPT_VECTOR_COUNT];

void arch_intc_init(void)
{
    size_t n = (size_t)CONFIG_SMP_CORES;

    INTC->BCR = 0;

    while (n-- != 0) {
        INTC->CPR[n] = 0;
        INTC->IACKR[n] = (uint32_t)__intc_vectors_start__;
    }
}

void arch_register_interrupt(picoRTOS_irq_t irq, picoRTOS_isr_fn fn, void *priv)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT))
        return;

    __intc_vectors_start__[irq] = (unsigned long)fn;
    ISR_TABLE_priv[irq] = (unsigned long)priv;
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT))
        return;

    /* only support for priority 1 yet */
    int prc_sel = 0x8000 >> arch_core();

    INTC->PSR[irq] = (uint16_t)(prc_sel | PSR_PRIN(1));
}


void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT))
        return;

    /* de-select core */
    int prc_sel = 0x8000 >> arch_core();

    INTC->PSR[irq] &= ~(uint16_t)prc_sel;
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
void arch_ack_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT))
        return;

    INTC->EOIR[arch_core()] = 0;
}

/* SMP */

#ifdef INTC_SIUL2_SMP

void arch_propagate_tick(void)
{
    INTC->SSCIR[0] = (uint8_t)SSCIR_SET;
}

void arch_acknowledge_tick(void)
{
    INTC->SSCIR[0] = (uint8_t)SSCIR_CLR;
}

static uint16_t prc_sel_from_mask(picoRTOS_mask_t core_mask)
{
    if (!picoRTOS_assert_fatal(core_mask != 0)) return 0;

    uint16_t prc_sel = 0;

    prc_sel |= ((0x1 & core_mask) != 0 ? PSR_PRC_SELN0 : 0);
    prc_sel |= ((0x2 & core_mask) != 0 ? PSR_PRC_SELN1 : 0);
    prc_sel |= ((0x4 & core_mask) != 0 ? PSR_PRC_SELN2 : 0);

    return prc_sel;
}

void arch_smp_enable_interrupt(picoRTOS_irq_t irq,
                               picoRTOS_mask_t core_mask)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;
    if (!picoRTOS_assert_fatal(core_mask != 0)) return;

    /* force to prio 1 */
    INTC->PSR[irq] = prc_sel_from_mask(core_mask) | (uint16_t)PSR_PRIN(1);
}

void arch_smp_disable_interrupt(picoRTOS_irq_t irq,
                                picoRTOS_mask_t core_mask)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;
    if (!picoRTOS_assert_fatal(core_mask != 0)) return;

    /* de-select core(s) */
    INTC->PSR[irq] &= ~prc_sel_from_mask(core_mask);
}

#endif
