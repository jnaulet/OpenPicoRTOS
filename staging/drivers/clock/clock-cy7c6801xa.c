#include "clock-cy7c6801xa.h"
#include "picoRTOS_device.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"

#ifndef S_SPLINT_S
volatile __xdata __at(0xe600) unsigned char CPUCS;
#else
static unsigned char CPUCS;
#endif

#define CPUCS_CLKSPD_M  0x3u
#define CPUCS_CLKSPD(x) (((x) & CPUCS_CLKSPD_M) << 3)

static struct {
    clock_freq_t cpu;
} clocks;

int clock_cy7c6801xa_init(struct clock_settings *settings)
{
    picoRTOS_assert(settings->clkspd < CLOCK_CY7C6801XA_CLKSPD_COUNT, return -EINVAL);

    /* zero register */
    CPUCS &= ~CPUCS_CLKSPD(CPUCS_CLKSPD_M);

    switch (settings->clkspd) {
    case CLOCK_CY7C6801XA_CLKSPD_12MHZ:
        clocks.cpu = (clock_freq_t)12000000ul;
        break;

    case CLOCK_CY7C6801XA_CLKSPD_24MHZ:
        CPUCS |= CPUCS_CLKSPD(1);
        clocks.cpu = (clock_freq_t)24000000ul;
        break;

    case CLOCK_CY7C6801XA_CLKSPD_48MHZ:
        CPUCS |= CPUCS_CLKSPD(2);
        clocks.cpu = (clock_freq_t)48000000ul;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    arch_set_clock_frequency((unsigned long)clocks.cpu);
    return 0;
}


clock_freq_t clock_get_freq(clock_id_t clkid)
{
    picoRTOS_assert(clkid != CLOCK_CY7C6801XA_CPU,
                    return (clock_freq_t)-EINVAL);

    switch (clkid) {
    case CLOCK_CY7C6801XA_CPU: return clocks.cpu;
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return (clock_freq_t)-EIO;
}
