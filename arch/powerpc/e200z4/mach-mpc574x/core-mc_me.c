#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>
#include <generated/autoconf.h>

#ifndef CONFIG_DEADLOCK_COUNT
# error CONFIG_DEADLOCK_COUNT is not defined
#endif

struct CORE_MC_ME {
    volatile uint32_t GS;
    volatile uint32_t MCTL;
    volatile uint32_t ME;
    volatile uint32_t IS;
    volatile uint32_t IM;
    volatile uint32_t IMTS;
    volatile uint32_t DMTS;
    uint32_t RESERVED0;
    volatile uint32_t RESET_MC;
    uint32_t RESERVED1;
    volatile uint32_t SAFE_MC;
    volatile uint32_t DRUN_MC;
    volatile uint32_t RUN0_MC;
    volatile uint32_t RUN1_MC;
    volatile uint32_t RUN2_MC;
    volatile uint32_t RUN3_MC;
    uint32_t RESERVED2[2];
    volatile uint32_t STOP_MC;
    uint32_t RESERVED3[2];
    volatile uint32_t STANDBY_MC;
    uint32_t RESERVED4[2];
    volatile uint32_t PSn[4];
    uint32_t RESERVED5[4];
    volatile uint32_t RUN_PC[8];
    volatile uint32_t LP_PC[8];
    volatile uint8_t PCTLn[106];
    uint16_t RESERVED6[75];
    volatile uint32_t CS;
    uint16_t RESERVED7;
    volatile uint16_t CCTL[3];
    uint32_t RESERVED8[6];
    volatile uint32_t CADDR[3];
};

#define GS_S_MTRANS (1 << 27)

#define MCTL_TARGET_MODE_M  0xfu
#define MCTL_TARGET_MODE(x) (((x) & MCTL_TARGET_MODE_M) << 28)
#define MCTL_KEY_M          0xffffu
#define MCTL_KEY(x)         ((x) & MCTL_KEY_M)

#define CCTL_RUN3  (1 << 7)
#define CCTL_RUN2  (1 << 6)
#define CCTL_RUN1  (1 << 5)
#define CCTL_RUN0  (1 << 4)
#define CCTL_DRUN  (1 << 3)
#define CCTL_SAFE  (1 << 2)
#define CCTL_RESET (1 << 0)

#define CADDR_RMC (1 << 0)

static struct CORE_MC_ME *MC_ME = (struct CORE_MC_ME*)ADDR_MC_ME;

/* ASM */
/*@external@*/ extern void arch_core_start(void);

static void mode_transition_not_active_busywait(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while ((MC_ME->GS & GS_S_MTRANS) != 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    arch_assert_void(deadlock != -1);
}

void arch_core_run(picoRTOS_core_t core)
{
    arch_assert_void(core < (picoRTOS_core_t)CONFIG_SMP_CORES);

    uint32_t mctl = MC_ME->MCTL;

    /* zero key, keep target mode */
    mctl &= ~MCTL_KEY(MCTL_KEY_M);

    /* run in all modes & start */
    MC_ME->CADDR[core] = (uint32_t)arch_core_start | CADDR_RMC;
    MC_ME->CCTL[core] = (uint16_t)(CCTL_RUN3 | CCTL_RUN2 | CCTL_RUN1 |
                                   CCTL_RUN0 | CCTL_DRUN | CCTL_SAFE);

    /* trigger start */
    MC_ME->MCTL = (uint32_t)(mctl | MCTL_KEY(0x5af0));
    MC_ME->MCTL = (uint32_t)(mctl | MCTL_KEY(0xa50f));

    /* wait for transition complete */
    mode_transition_not_active_busywait();
}
