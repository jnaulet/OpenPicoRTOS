#include "picoRTOS_port.h"

#include <stdint.h>
#include <generated/autoconf.h>

/* SPRS */
/*@external@*/ extern uint32_t arch_mfdec(void);
/*@external@*/ extern uint32_t arch_mfdecar(void);
/*@external@*/ extern uint32_t arch_mftcr(void);
/*@external@*/ extern uint32_t arch_mfhid0(void);
/*@external@*/ extern void arch_mtdec(uint32_t value);
/*@external@*/ extern void arch_mtdecar(uint32_t value);
/*@external@*/ extern void arch_mttcr(uint32_t value);
/*@external@*/ extern void arch_mthid0(uint32_t value);

#define HID0_TBEN       (1 << 14)
#define HID0_SEL_TBLCLK (1 << 13)

#define TCR_DIE (1 << 26)
#define TCR_ARE (1 << 22)

void arch_timer_init(int period)
{
    /* setup decrementer */
    arch_mtdecar((uint32_t)period - 1);                                     /* reload value */
    arch_mthid0(arch_mfhid0() & (uint32_t) ~(HID0_TBEN | HID0_SEL_TBLCLK)); /* clock cpu */
    arch_mttcr(arch_mftcr() | (uint32_t)(TCR_DIE | TCR_ARE));               /* auto-reload */
    arch_mtdec(arch_mfdecar());
}

/* STAT OPS */

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)arch_mfdec();

    if (counter == ARCH_COUNTER_SINCE) {
        picoRTOS_cycles_t dec = (picoRTOS_cycles_t)arch_mfdec();
        picoRTOS_cycles_t decar = (picoRTOS_cycles_t)arch_mfdecar();

        if (t > decar) return decar + 1;        /* only used on first tick */
        if (dec > t) return (decar - dec) + t;  /* rollover */
        /* normal */
        return t - dec;
    }

    arch_assert_void(false);
    return 0;
}
