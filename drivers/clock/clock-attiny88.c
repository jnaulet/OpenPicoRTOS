#include "clock-attiny88.h"
#include "picoRTOS.h"

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    picoRTOS_assert(clkid < CLOCK_ATTINY88_COUNT,
                    return (clock_freq_t)-EINVAL);

    /* basically */
    return (clock_freq_t)CONFIG_SYSCLK_HZ;
}
