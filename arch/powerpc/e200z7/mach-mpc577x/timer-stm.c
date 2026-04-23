#include "picoRTOS_device.h"
#include "picoRTOS-SMP_port.h"

#include <stdint.h>
#include <generated/autoconf.h>

#define STM_CHANNEL_COUNT 4

struct STM {
    volatile uint32_t CR;
    volatile uint32_t CNT;
    uint32_t RESERVED0[2];
    struct {
        volatile uint32_t CCRn;
        volatile uint32_t CIRn;
        volatile uint32_t CMPn;
        uint32_t RESERVED1;
    } CH[STM_CHANNEL_COUNT];
};

#define CR_CPS_M  0xffu
#define CR_CPS(x) (((x) & CR_CPS_M) << 8)
#define CR_CSL    (1 << 4)
#define CR_FRZ    (1 << 1)
#define CR_TEN    (1 << 0)

#define CCRn_CEN (1 << 0)
#define CIRn_CIF (1 << 0)

/* instance */
static struct STM *STMA = (struct STM*)ADDR_STMA;
static uint32_t timer_stm_period
__attribute__((aligned(ARCH_L1_DCACHE_LINESIZE)));

static void Timer_Handler(void)
{
    ASM("mfsprg %r3, 0");       /* load task pointer from sprg0 */
    ASM("e_bl picoRTOS_tick");  /* call tick */
    ASM("mtsprg 0, %r3");       /* store returned task stack pointer */

    {
        int core = (int)arch_core();
        /* reset flag & prepare next tick */
        STMA->CH[core].CIRn = (uint32_t)CIRn_CIF;
        STMA->CH[core].CMPn += timer_stm_period;
    }
}

#define SYSDIV ((volatile uint32_t*)ADDR_SIU + 0X9a0)

#define SYSDIV_FMPERDIV_S 20
#define SYSDIV_FMPERDIV_M 0x3u

void arch_timer_init(int period)
{
    arch_assert_void(period > 0);

    size_t n = (size_t)CONFIG_SMP_CORES;
    /* STMA source clock is FM_PER_DIV, extract divider */
    size_t fmperdiv = (size_t)(SYSDIV_FMPERDIV_M & (*SYSDIV >> SYSDIV_FMPERDIV_S)) + 1;

    timer_stm_period = (uint32_t)period >> fmperdiv;
    arch_flush_dcache(&timer_stm_period, sizeof(timer_stm_period));

    /* enable STMA */
    STMA->CR = (uint32_t)(CR_FRZ | CR_TEN);
    while (n-- != 0) {
        /* setup period & start */
        STMA->CH[n].CMPn = (uint32_t)timer_stm_period;
        STMA->CH[n].CCRn = (uint32_t)CCRn_CEN;
        /* register interrupt */
        arch_register_interrupt((picoRTOS_irq_t)(IRQ_STM_CIR0 + n), (arch_isr_fn)Timer_Handler, NULL);
        arch_smp_enable_interrupt((picoRTOS_irq_t)(IRQ_STM_CIR0 + n), (picoRTOS_mask_t)(1u << n));
    }
}

/* STAT OPS */

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)STMA->CNT;

    if (counter == ARCH_COUNTER_SINCE)
        return (picoRTOS_cycles_t)STMA->CNT - t;

    arch_assert_void(false);
    return 0;
}
