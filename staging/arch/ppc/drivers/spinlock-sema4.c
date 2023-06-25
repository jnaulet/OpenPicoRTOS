#include "picoRTOS-SMP.h"
#include "picoRTOS_device.h"

#include "picoRTOS-SMP_port.h"

#include <stdint.h>

struct SEMA4 {
    volatile uint8_t GATE[16];
    uint32_t RESERVED0[12];
    volatile uint16_t CP0INE;
    uint16_t RESERVED1[3];
    volatile uint16_t CP1INE;
    uint16_t RESERVED2[27];
    volatile uint16_t CP0INF;
    uint16_t RESERVED3[3];
    volatile uint16_t CP1INF;
    uint16_t RESERVED4[59];
    volatile uint16_t RSTGT;
    uint16_t RESERVED5;
    volatile uint16_t RSTNTF;
};

#define RSTGT_W_RSTGDP_M  0xffu
#define RSTGT_W_RSTGDP(x) (((x) & RSTGT_W_RSTGDP_M) << 8)

static struct SEMA4 *SEMA4 = (struct SEMA4*)ADDR_SEMA4;

/* ASM */
/*@external@*/ extern picoRTOS_core_t arch_core(void);

void arch_spinlock_init(void)
{
    /* reset spinlock0 */
    SEMA4->RSTGT = (uint16_t)RSTGT_W_RSTGDP(0xe2);
    SEMA4->RSTGT = (uint16_t)RSTGT_W_RSTGDP(0x1d);
}

void arch_spin_lock(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;
    picoRTOS_core_t core = arch_core() + 1;

    while (deadlock-- != 0) {
        SEMA4->GATE[0] = (uint8_t)core;
        if (SEMA4->GATE[0] == (uint8_t)core)
            break;
    }

    /* deadlock */
    if (!picoRTOS_assert_fatal(deadlock != -1))
        return;
}

void arch_spin_unlock(void)
{
    SEMA4->GATE[0] = 0;
}
