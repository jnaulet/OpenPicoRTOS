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
    ASM("se_mflr %r0");
    ASM("e_stwu %r0, -4(%r1)");

    ASM("mfsprg %r3, 0");       /* load task pointer from sprg0 */
    ASM("e_bl picoRTOS_tick");  /* call tick */
    ASM("mtsprg 0, %r3");       /* store returned task stack pointer */

    ASM("e_lwz %r0, 0(%r1)");
    ASM("e_add16i %r1, %r1, 4");
    ASM("se_mtlr %r0");

    /* reset flag & prepare next tick */
    if ((STMA->CH[0].CIRn & CIRn_CIF) != 0) {
        STMA->CH[0].CIRn = (uint32_t)CIRn_CIF;
        STMA->CH[0].CMPn += timer_stm_period;
    }
}

#define SYSDIV ((volatile uint32_t*)ADDR_SIU + 0X9a0)

#define SYSDIV_FMPERDIV_S 20
#define SYSDIV_FMPERDIV_M 0x3u

void arch_timer_init(int period)
{
    arch_assert_void(period > 0);

    /* STMA source clock is FM_PER_DIV, extract divider */
    size_t fmperdiv = (size_t)(SYSDIV_FMPERDIV_M & (*SYSDIV >> SYSDIV_FMPERDIV_S)) + 1;
    timer_stm_period = (uint32_t)period >> fmperdiv;

    /* enable STMA */
    STMA->CH[0].CMPn = (uint32_t)timer_stm_period;
    STMA->CH[0].CCRn = (uint32_t)CCRn_CEN;
    /* register interrupt */
    arch_register_interrupt((picoRTOS_irq_t)IRQ_STM_CIR0,
                            (arch_isr_fn)Timer_Handler, NULL);
    /* enable */
    arch_smp_enable_interrupt((picoRTOS_irq_t)IRQ_STM_CIR0,
                              (picoRTOS_mask_t)(1 << CONFIG_CORE_COUNT) - 1);
}

void arch_timer_start(void)
{
    STMA->CR = (uint32_t)CR_TEN;
}

void arch_timer_stop(void)
{
    STMA->CR = (uint32_t)CR_FRZ;
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
