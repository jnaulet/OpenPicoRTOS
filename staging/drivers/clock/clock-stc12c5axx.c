#include "clock-stc12c5axx.h"
#include "picoRTOS_device.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"

#ifndef S_SPLINT_S
__sfr __at(ADDR_AUXR) AUXR;
#else
static unsigned char AUXR;
#endif

#define AUXR_T1x12  (1 << 6)
#define AUXR_BRTx12 (1 << 2)

static struct {
    clock_freq_t t1;
    clock_freq_t brt;
} clocks;

#define SYSCLK_HZ (CONFIG_SYSCLK_HZ / CONFIG_SYSCLK_DIV)

int clock_stc12c5axx_init(struct clock_settings *settings)
{
    picoRTOS_assert(settings->t1 < CLOCK_STC12C5AXX_XCOUNT, return -EINVAL);
    picoRTOS_assert(settings->brt < CLOCK_STC12C5AXX_XCOUNT, return -EINVAL);

    if (settings->t1 == CLOCK_STC12C5AXX_X1) {
        AUXR &= ~AUXR_T1x12;
        clocks.t1 = (clock_freq_t)(SYSCLK_HZ / 12);
    }else{
        AUXR |= AUXR_T1x12;
        clocks.t1 = (clock_freq_t)SYSCLK_HZ;
    }

    if (settings->brt == CLOCK_STC12C5AXX_X1) {
        AUXR &= ~AUXR_BRTx12;
        clocks.brt = (clock_freq_t)(SYSCLK_HZ / 12);
    }else{
        AUXR |= AUXR_T1x12;
        clocks.brt = (clock_freq_t)SYSCLK_HZ;
    }

    return 0;
}

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    picoRTOS_assert(clkid <= CLOCK_STC12C5AXX_BRT,
                    return (clock_freq_t)-EINVAL);

    switch (clkid) {
    case CLOCK_STC12C5AXX_FSYS: return (clock_freq_t)SYSCLK_HZ;
    case CLOCK_STC12C5AXX_T1: return clocks.t1;
    case CLOCK_STC12C5AXX_BRT: return clocks.brt;
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return (clock_freq_t)-EIO;
}
