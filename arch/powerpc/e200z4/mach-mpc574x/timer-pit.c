#include "picoRTOS_device.h"

#include "picoRTOS_port.h"
#include "picoRTOS-SMP_port.h"

#include <stdint.h>
#include <generated/autoconf.h>

#define PIT_PIT_COUNT 16
#define PIT_INDEX     3 /* FIXME: arbitrary */
#define PIT_IRQ       (IRQ_PIT0 + PIT_INDEX)

struct PIT {
    volatile uint32_t MCR;
    uint32_t RESERVED0[55];
    volatile uint32_t LTMR64H;
    volatile uint32_t LTMR64L;
    uint32_t RESERVED1[2];
    volatile uint32_t RTI_LDVAL;
    volatile uint32_t RTI_CVAL;
    volatile uint32_t RTI_TCTRL;
    volatile uint32_t RTI_TFLG;
    struct {
        volatile uint32_t LDVALn;
        volatile uint32_t CVALn;
        volatile uint32_t TCTRLn;
        volatile uint32_t TFLGn;
    } PIT[PIT_PIT_COUNT];
};

#define MCR_MDIS_RTI (1 << 2)
#define MCR_MDIS     (1 << 1)
#define MCR_FRZ      (1 << 0)

#define TCTRLn_CHN (1 << 2)
#define TCTRLn_TIE (1 << 1)
#define TCTRLn_TEN (1 << 0)

/* instance */
static struct PIT *PIT = (struct PIT*)ADDR_PIT; // NOLINT

static void Timer_Handler(void)
{
    ASM("se_mflr %r0");
    ASM("e_stwu %r0, -4(%r1)");

    ASM("mfsprg %r3, 0");       /* load task pointer from sprg0 */
    ASM("e_bl picoRTOS_tick");  /* call tick */
    ASM("mtsprg 0, %r3");       /* store returned task stack pointer */

    ASM("e_lwz %r0, 0(%r1)");
    ASM("e_add16i %r1, %r1, 4");
    ASM("se_mtlr %r0");

    /* reset flag */
    if (PIT->PIT[PIT_INDEX].TFLGn != 0)
        PIT->PIT[PIT_INDEX].TFLGn = (uint32_t)1;
}

void arch_timer_init(int period)
{
    arch_assert_void(period > 0);

    /* enable PIT */
    PIT->MCR = (uint32_t)0;
    PIT->MCR = (uint32_t)(MCR_FRZ | MCR_MDIS_RTI);
    /* setup period & start */
    PIT->PIT[PIT_INDEX].LDVALn = (uint32_t)((PIT_FREQ_HZ / CONFIG_TICK_HZ) - 1);
    PIT->PIT[PIT_INDEX].TCTRLn = (uint32_t)(TCTRLn_TIE | TCTRLn_TEN);

    /* register interrupt */
    arch_register_interrupt((picoRTOS_irq_t)PIT_IRQ, (arch_isr_fn)Timer_Handler, NULL);
#ifndef CONFIG_SMP
    arch_enable_interrupt((picoRTOS_irq_t)PIT_IRQ);
#else
    arch_smp_enable_interrupt((picoRTOS_irq_t)PIT_IRQ,
                              (picoRTOS_mask_t)(1 << CONFIG_CORE_COUNT) - 1);
#endif
    /* mpu */
    arch_mpu_add_region(PID_KERNEL, PIT, sizeof(*PIT),
                        MM_PRW | MM_NON_CACHEABLE);
}

/* STAT OPS */

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)PIT->PIT[PIT_INDEX].CVALn;

    if (counter == ARCH_COUNTER_SINCE) {
        picoRTOS_cycles_t cval = (picoRTOS_cycles_t)PIT->PIT[PIT_INDEX].CVALn;
        picoRTOS_cycles_t ldval = (picoRTOS_cycles_t)PIT->PIT[PIT_INDEX].LDVALn;

        if (t > ldval) return ldval + 1;            /* only used on first tick */
        if (cval > t) return (ldval - cval) + t;    /* rollover */
        /* normal */
        return t - cval;
    }

    arch_assert_void(false);
    return 0;
}
