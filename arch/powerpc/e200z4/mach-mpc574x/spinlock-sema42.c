#include "picoRTOS_device.h"
#include "picoRTOS-SMP_port.h"

#include <stdint.h>
#include <generated/autoconf.h>

struct SEMA42 {
    volatile uint8_t GATE[16];
    uint32_t RESERVED0[12];
    volatile uint16_t RSTGT;
};

#define RSTGT_W_RSTGDP_M  0xffu
#define RSTGT_W_RSTGDP(x) (((x) & RSTGT_W_RSTGDP_M) << 8)

static struct SEMA42 *SEMA42 = (struct SEMA42*)ADDR_SEMA42;

/* ASM */
/*@external@*/ extern picoRTOS_core_t arch_core(void);

void arch_spinlock_init(void)
{
    /* reset spinlock0 */
    SEMA42->RSTGT = (uint16_t)RSTGT_W_RSTGDP(0xe2);
    SEMA42->RSTGT = (uint16_t)RSTGT_W_RSTGDP(0x1d);
}

void arch_spin_lock(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;
    picoRTOS_core_t core = arch_core() + 1;

    while (deadlock-- != 0) {
        SEMA42->GATE[0] = (uint8_t)core;
        if (SEMA42->GATE[0] == (uint8_t)core)
            break;
    }

    /* deadlock */
    arch_assert_void(deadlock != -1);
}

void arch_spin_unlock(void)
{
    SEMA42->GATE[0] = 0;
}
