#include "clock-atmega2560.h"
#include "picoRTOS.h"

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    if (!picoRTOS_assert(clkid < CLOCK_ATMEGA2560_COUNT)) return (clock_freq_t)-EINVAL;

    /* basically */
    return (clock_freq_t)CONFIG_SYSCLK_HZ;
}
