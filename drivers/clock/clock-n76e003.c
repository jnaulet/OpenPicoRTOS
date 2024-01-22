#include "clock-n76e003.h"
#include "picoRTOS_device.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"

#define TA_KEY     0xaa
#define TA_KEY_INV 0x55

#ifndef S_SPLINT_S
__sfr __at(ADDR_TA)    TA;
__sfr __at(ADDR_CKSWT) CKSWT;
__sfr __at(ADDR_CKEN)  CKEN;
__sfr __at(ADDR_CKDIV) CKDIV;
#else
static unsigned char TA;
static unsigned char CKSWT;
static unsigned char CKEN;
static unsigned char CKDIV;
#endif

#define CKSWT_HIRCST (1 << 5)
#define CKSWT_LIRCST (1 << 4)
#define CKSWT_ECLKST (1 << 3)
#define CKSWT_OSC_M  0x3u
#define CKSWT_OSC(x) (((x) & CKSWT_OSC_M) << 1)

#define CKEN_EXTEN_M  0x3u
#define CKEN_EXTEN(x) (((x) & CKEN_EXTEN_M) << 6)
#define CKEN_HIRCEN   (1 << 5)
#define CKEN_CKSWTF   (1 << 0)

#define CKCON_PWMCKS (1 << 6)
#define CKCON_T1M    (1 << 4)
#define CKCON_T0M    (1 << 3)
#define CKCON_CLOEN  (1 << 1)

#define FHIRC_FREQ_HZ 16000000ul
#define FLIRC_FREQ_HZ 10000ul

static struct {
    clock_freq_t fsys;
    clock_freq_t fosc;
    clock_freq_t feclk;
} clocks;

static int enabled_and_stable_busywait(unsigned char mask)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while (deadlock-- != 0)
        if ((CKSWT & mask) != (unsigned char)0)
            break;

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int ckswt_osc_fhirc(void)
{
    int res;

    /* enable */
    TA = (unsigned char)TA_KEY;
    TA = (unsigned char)TA_KEY_INV;
    CKEN = (unsigned char)CKEN_HIRCEN;

    /* check */
    if ((res = enabled_and_stable_busywait((unsigned char)CKSWT_HIRCST)) < 0)
        return res;

    /* switch */
    TA = (unsigned char)TA_KEY;
    TA = (unsigned char)TA_KEY_INV;
    CKSWT = (unsigned char)CKSWT_OSC(0);

    /* clock */
    clocks.fosc = (clock_freq_t)FHIRC_FREQ_HZ;

    return 0;
}

static int ckswt_osc_feclk(unsigned long hz)
{
    picoRTOS_assert(hz > 0, return -EINVAL);

    int res;

    /* enable */
    TA = (unsigned char)TA_KEY;
    TA = (unsigned char)TA_KEY_INV;
    CKEN = (unsigned char)CKEN_EXTEN(3);

    /* check */
    if ((res = enabled_and_stable_busywait((unsigned char)CKSWT_ECLKST)) < 0)
        return res;

    /* switch */
    TA = (unsigned char)TA_KEY;
    TA = (unsigned char)TA_KEY_INV;
    CKSWT = (unsigned char)CKSWT_OSC(1);

    /* clocks */
    clocks.fosc = (clock_freq_t)hz;

    return 0;
}

static int ckswt_osc_flirc(void)
{
    int res;

    /* check */
    if ((res = enabled_and_stable_busywait((unsigned char)CKSWT_LIRCST)) < 0)
        return res;

    /* switch */
    TA = (unsigned char)TA_KEY;
    TA = (unsigned char)TA_KEY_INV;
    CKSWT = (unsigned char)CKSWT_OSC(2);

    /* clock */
    clocks.fosc = (clock_freq_t)FLIRC_FREQ_HZ;

    return 0;
}

/* Function: clock_n76e003_init
 * Intializes the n76e003 clock system
 *
 * Parameters:
 *  settings - The clocks settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_n76e003_init(struct clock_settings *settings)
{
    picoRTOS_assert(settings->clkdiv > 0, return -EINVAL);
    picoRTOS_assert(settings->clkdiv == 1u || (settings->clkdiv & 0x1u) == 0, return -EINVAL);
    picoRTOS_assert(settings->clkdiv <= (unsigned int)CLOCK_N76E003_CKDIV_MAX, return -EINVAL);
    picoRTOS_assert(settings->fosc < CLOCK_N76E003_FOSC_COUNT, return -EINVAL);

    int res;

    switch (settings->fosc) {
    case CLOCK_N76E003_FOSC_FHIRC: res = ckswt_osc_fhirc(); break;
    case CLOCK_N76E003_FOSC_FECLK: res = ckswt_osc_feclk(settings->feclk); break;
    case CLOCK_N76E003_FOSC_FLIRC: res = ckswt_osc_flirc(); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    if (res < 0)
        return res;

    CKDIV = (unsigned char)(settings->clkdiv >> 1);

    clocks.feclk = (clock_freq_t)settings->feclk;
    clocks.fsys = clocks.fosc / (clock_freq_t)settings->clkdiv;

    arch_set_clock_frequency((unsigned long)clocks.fsys);
    return 0;
}

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    picoRTOS_assert(clkid <= CLOCK_N76E003_FLIRC,
                    return (clock_freq_t)-EINVAL);

    switch (clkid) {
    case CLOCK_N76E003_FSYS: return clocks.fsys;
    case CLOCK_N76E003_FOSC: return clocks.fosc;
    case CLOCK_N76E003_FECLK: return clocks.feclk;
    case CLOCK_N76E003_FHIRC: return (clock_freq_t)FHIRC_FREQ_HZ;
    case CLOCK_N76E003_FLIRC: return (clock_freq_t)FLIRC_FREQ_HZ;
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return (clock_freq_t)-EIO;
}
