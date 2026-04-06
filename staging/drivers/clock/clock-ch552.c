#include "clock-ch552.h"
#include "picoRTOS_device.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"

#ifndef S_SPLINT_S
__sfr __at(ADDR_CLOCK_CFG) CLOCK_CFG;
__sfr __at(ADDR_SAFE_MOD) SAFE_MOD;
#else
static unsigned char CLOCK_CFG;
static unsigned char SAFE_MOD;
#endif

#define CLOCK_CFG_bOSC_EN_INT        (1 << 7)
#define CLOCK_CFG_bOSC_EN_XT         (1 << 6)
#define CLOCK_CFG_bWDOG_IF_TO        (1 << 5)
#define CLOCK_CFG_bROM_CLK_FAST      (1 << 4)
#define CLOCK_CFG_bRST               (1 << 3)
#define CLOCK_CFG_MASK_SYS_CK_SEL_M  0x7u
#define CLOCK_CFG_MASK_SYS_CK_SEL(x) ((x) & CLOCK_CFG_MASK_SYS_CK_SEL_M)

static unsigned long clock_fsys = DEVICE_DEFAULT_SYSCLK_HZ;

int clock_init(const struct clock_settings *settings)
{
    picoRTOS_assert(settings->osc < CLOCK_CH552_OSC_COUNT, return -EINVAL);
    picoRTOS_assert(settings->sys_ck_sel < CLOCK_CH552_SYS_CK_SEL_COUNT, return -EINVAL);

    if (settings->osc == CLOCK_CH552_OSC_XT) {
        /* 1 enter safe mode */
        SAFE_MOD = (unsigned char)0x55;
        SAFE_MOD = (unsigned char)0xaa;
        /* 2 set bOSC_EN_XT to 1 with OR */
        CLOCK_CFG |= CLOCK_CFG_bOSC_EN_XT;
        /* 3 delay several milliseconds */
        arch_delay_us(10000ul); /* 10ms */
        /* 4 re-enter safe mode */
        SAFE_MOD = (unsigned char)0x55;
        SAFE_MOD = (unsigned char)0xaa;
        /* 5 reset bOSC_EN_INT with AND */
        CLOCK_CFG &= ~CLOCK_CFG_bOSC_EN_INT;
        /* 6 terminate safe mode */
        SAFE_MOD = (unsigned char)1;
    }

    unsigned char clock_cfg = CLOCK_CFG;

    if (settings->sys_ck_sel < CLOCK_CH552_SYS_CK_SEL_101)
        clock_cfg |= CLOCK_CFG_bROM_CLK_FAST;

    clock_cfg &= ~CLOCK_CFG_MASK_SYS_CK_SEL(CLOCK_CFG_MASK_SYS_CK_SEL_M);
    clock_cfg |= CLOCK_CFG_MASK_SYS_CK_SEL(settings->sys_ck_sel);

    /* 1 enter safe mode */
    SAFE_MOD = (unsigned char)0x55;
    SAFE_MOD = (unsigned char)0xaa;
    /* 2 write new value to CLOCK_XFG */
    CLOCK_CFG = clock_cfg;
    /* 3 terminate safe mode */
    SAFE_MOD = (unsigned char)1;

    switch (settings->sys_ck_sel) {
    case CLOCK_CH552_SYS_CK_SEL_000: clock_fsys = 187500ul; break;
    case CLOCK_CH552_SYS_CK_SEL_001: clock_fsys = 750000ul; break;
    case CLOCK_CH552_SYS_CK_SEL_010: clock_fsys = 3000000ul; break;
    case CLOCK_CH552_SYS_CK_SEL_011: clock_fsys = 6000000ul; break;
    case CLOCK_CH552_SYS_CK_SEL_100: clock_fsys = 12000000ul; break;
    case CLOCK_CH552_SYS_CK_SEL_101: clock_fsys = 16000000ul; break;
    case CLOCK_CH552_SYS_CK_SEL_110: clock_fsys = 24000000ul; break;
    default:
        picoRTOS_assert(false, return -EIO);
    }

    arch_set_clock_frequency(clock_fsys);
    return 0;
}

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    picoRTOS_assert(clkid != CLOCK_CH552_FSYS,
                    return (clock_freq_t)-EINVAL);

    return (clock_freq_t)clock_fsys;
}
