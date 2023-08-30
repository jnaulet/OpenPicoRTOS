#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

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
static struct STM *STM0 = (struct STM*)ADDR_STM0;
static uint32_t timer_stm_period = 0;

/* cppcheck-suppress constParameter */
static void Timer_Handler(void *priv)
{
    ASM("e_add16i 1, 1, -4");
    ASM("se_mflr 0");
    ASM("e_stw 0, 0 (1)");

    arch_assert_void(priv == NULL);

    ASM("mfsprg 3, 0");         /* load task pointer from sprg0 */
    ASM("e_bl picoRTOS_tick");  /* call tick */
    ASM("mtsprg 0, 3");         /* store returned task stack pointer */

    /* reset flag & prepare next tick */
    STM0->CH[3].CIRn = (uint32_t)CIRn_CIF;
    STM0->CH[3].CMPn += timer_stm_period;

    ASM("se_lwz 0, 0 (1)");
    ASM("se_mtlr 0");
    ASM("e_add16i 1, 1, 4");
}

void arch_timer_init(int period)
{
    arch_assert_void(period > 0);

    /* STM0 source clock is FS80 (half-speed) */
    timer_stm_period = (uint32_t)period >> 1;

    /* enable STM0 */
    STM0->CR = (uint32_t)(CR_FRZ | CR_TEN);
    /* setup period & start */
    STM0->CH[3].CMPn = (uint32_t)timer_stm_period;
    STM0->CH[3].CCRn = (uint32_t)CCRn_CEN;

    /* register interrupt */
    arch_register_interrupt((picoRTOS_irq_t)IRQ_STM0_CIR3, Timer_Handler, NULL);
    arch_enable_interrupt((picoRTOS_irq_t)IRQ_STM0_CIR3);
}

/* STAT OPS */

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)STM0->CNT;

    if (counter == ARCH_COUNTER_SINCE)
        return (picoRTOS_cycles_t)STM0->CNT - t;

    arch_assert_void(false);
    return 0;
}
