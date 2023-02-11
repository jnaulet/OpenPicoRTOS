#include "clock-same5x.h"
#include "picoRTOS.h"

#include <stdint.h>

#include "picoRTOS_device.h"

struct GCLK_SAME5X {
    volatile uint32_t CTRLA;
    volatile uint32_t SYNCBUSY;
    uint32_t RESERVED0[6];
    volatile uint32_t GENCTRLn[CLOCK_SAME5X_GCLK_COUNT];
    uint32_t RESERVED1[12];
    volatile uint32_t PCHCTRLn[CLOCK_SAME5X_PCH_COUNT];
};

#define CTRLA_SWRST (1 << 0)

#define SYNCBUSY_GENCTRL(x) (1 << ((x) + 2))
#define SYNCBUSY_SWRST      (1 << 0)

#define GENCTRLn_DIV_M    0xffffu
#define GENCTRLn_DIV(x)   (((x) & GENCTRLn_DIV_M) << 16)
#define GENCTRLn_RUNSTDBY (1 << 13)
#define GENCTRLn_DIVSEL   (1 << 12)
#define GENCTRLn_OE       (1 << 11)
#define GENCTRLn_OOV      (1 << 10)
#define GENCTRLn_IDC      (1 << 9)
#define GENCTRLn_GENEN    (1 << 8)
#define GENCTRLn_SRC_M    0xfu
#define GENCTRLn_SRC(x)   ((x) & GENCTRLn_SRC_M)

#define PCHCTRLn_WRTLOCK (1 << 7)
#define PCHCTRLn_CHEN    (1 << 6)
#define PCHCTRLn_GEN_M   0xfu
#define PCHCTRLn_GEN(x)  ((x) & PCHCTRLn_GEN_M)

struct MCLK_SAME5X {
    uint8_t RESERVED0;
    volatile uint8_t INTENCLR;
    volatile uint8_t INTENSET;
    volatile uint8_t INTFLAG;
    volatile uint8_t HSDIV;
    volatile uint8_t CPUDIV;
    uint8_t RESERVED1[10];
    volatile uint32_t AHBMASK;
    volatile uint32_t APBAMASK;
    volatile uint32_t APBBMASK;
    volatile uint32_t APBCMASK;
    volatile uint32_t APBDMASK;
};

#define INTn_CKRDY (1 << 0)

struct OSCCTRL_SAME5X {
    volatile uint32_t EVCTRL;
    volatile uint32_t INTENCLR;
    volatile uint32_t INTENSET;
    volatile uint32_t INTFLAG;
    volatile uint32_t STATUS;
    volatile uint32_t XOSCCTRLn[2];
    volatile uint32_t DFLLCTRLA;
    volatile uint32_t DFLLCTRLB;
    volatile uint32_t DFLLVAL;
    volatile uint32_t DFLLMUL;
    volatile uint32_t DFLLSYNC;
    struct {
        volatile uint32_t CTRLA;
        volatile uint32_t RATIO;
        volatile uint32_t CTRLB;
        volatile uint32_t SYNCBUSY;
        volatile uint32_t STATUS;
    } DPLLn[2];
};

#define STATUS_DPLL1LDRTO (1 << 27)
#define STATUS_DPLL1TO    (1 << 26)
#define STATUS_DPLL1LCKF  (1 << 25)
#define STATUS_DPLL1LCKR  (1 << 24)
#define STATUS_DPLL0LDRTO (1 << 19)
#define STATUS_DPLL0TO    (1 << 18)
#define STATUS_DPLL0LCKF  (1 << 17)
#define STATUS_DPLL0LCKR  (1 << 16)
#define STATUS_DFLLRCS    (1 << 12)
#define STATUS_DFLLCKC    (1 << 11)
#define STATUS_DFLLCKF    (1 << 10)
#define STATUS_DFLLOOB    (1 << 9)
#define STATUS_DFLLRDY    (1 << 8)
#define STATUS_XOSCCKSW1  (1 << 5)
#define STATUS_XOSCCKSW0  (1 << 4)
#define STATUS_XOSCFAIL1  (1 << 3)
#define STATUS_XOSCFAIL0  (1 << 2)
#define STATUS_XOSCRDY1   (1 << 1)
#define STATUS_XOSCRDY0   (1 << 0)

#define DFLLCTRLA_ONDEMAND (1 << 7)
#define DFLLCTRLA_RUNSTDBY (1 << 6)
#define DFLLCTRLA_ENABLE   (1 << 1)

#define DFLLCTRLB_WAITLOCK (1 << 7)
#define DFLLCTRLB_BPLCKC   (1 << 6)
#define DFLLCTRLB_QLDIS    (1 << 5)
#define DFLLCTRLB_CCDIS    (1 << 4)
#define DFLLCTRLB_USBCRM   (1 << 3)
#define DFLLCTRLB_LLAW     (1 << 2)
#define DFLLCTRLB_STABLE   (1 << 1)
#define DFLLCTRLB_MODE     (1 << 0)

#define DFLLVAL_DIFF_M    0xffffu
#define DFLLVAL_DIFF(x)   (((x) & DFLLVAL_DIFF_M) << 16)
#define DFLLVAL_COARSE_M  0x3fu
#define DFLLVAL_COARSE(x) (((x) & DFLLVAL_COARSE_M) << 10)
#define DFLLVAL_FINE_M    0xffu
#define DFLLVAL_FINE(x)   ((x) & DFLLVAL_FINE_M)

#define DFLLMUL_CSTEP_M  0x3fu
#define DFLLMUL_CSTEP(x) (((x) & DFLLMUL_CSTEP_M) << 26)
#define DFLLMUL_FSTEP_M  0xffu
#define DFLLMUL_FSTEP(x) (((x) & DFLLMUL_FSTEP_M) << 16)
#define DFLLMUL_MUL_M    0xffffu
#define DFLLMUL_MUL(x)   ((x) & DFLLMUL_MUL_M)

#define DFLLSYNC_DFLLMUL   (1 << 4)
#define DFLLSYNC_DFLLVAL   (1 << 3)
#define DFLLSYNC_DFLLCTRLB (1 << 2)
#define DFLLSYNC_ENABLE    (1 << 1)

#define DPLLn_CTRLA_ONDEMAND (1 << 7)
#define DPLLn_CTRLA_RUNSTDBY (1 << 6)
#define DPLLn_CTRLA_ENABLE   (1 << 1)

#define DPLLn_RATIO_LDRFRAC_M  0x1fu
#define DPLLn_RATIO_LDRFRAC(x) (((x) & DPLLn_RATIO_LDRFRAC_M) << 16)
#define DPLLn_RATIO_LDR_M      0x1fffu
#define DPLLn_RATIO_LDR(x)     ((x) & DPLLn_RATIO_LDR_M)

#define DPLLn_CTRLB_DIV_M        0x7fu
#define DPLLn_CTRLB_DIV(x)       (((x) & DPLLn_CTRLB_DIV_M) << 16)
#define DPLLn_CTRLB_DCOEN        (1 << 15)
#define DPLLn_CTRLB_DCOFILTER_M  0x7u
#define DPLLn_CTRLB_DCOFILTER(x) (((x) & DPLLn_CTRLB_DCOFILTER_M) << 12)
#define DPLLn_CTRLB_LBYPASS      (1 << 11)
#define DPLLn_CTRLB_LTIME_M      0x7u
#define DPLLn_CTRLB_LTIME(x)     (((x) & DPLLn_CTRLB_LTIME_M) << 8)
#define DPLLn_CTRLB_REFCLK_M     0x7u
#define DPLLn_CTRLB_REFCLK(x)    (((x) & DPLLn_CTRLB_REFCLK_M) << 5)
#define DPLLn_CTRLB_WUF          (1 << 4)
#define DPLLn_CTRLB_FILTER_M     0xfu
#define DPLLn_CTRLB_FILTER(x)    ((x) & DPLLn_CTRLB_FILTER_M)

#define DPLLn_SYNCBUSY_DPLLRATIO (1 << 2)
#define DPLLn_SYNCBUSY_ENABLE    (1 << 1)

#define DPLLn_STATUS_CLKRDY (1 << 1)
#define DPLLn_STATUS_LOCK   (1 << 0)

struct OSC32KCTRL_SAME5X {
    volatile uint32_t INTENCLR;
    volatile uint32_t INTENSET;
    volatile uint32_t INTFLAG;
    volatile uint32_t STATUS;
    volatile uint32_t RTCCTRL;
    volatile uint16_t XOSC32K;
    volatile uint8_t CFDCTRL;
    volatile uint8_t EVCTRL;
    uint32_t RESERVED0;
    volatile uint32_t OSCULP32K;
};

#define INTn_XOSC32KFAIL (1 << 2)
#define INTn_XOSC32KRDY  (1 << 0)

#define STATUS_XOSC32KSW   (1 << 3)
#define STATUS_XOSC32KFAIL (1 << 2)
#define STATUS_XOSC32KRDY  (1 << 0)

#define RTCCTRL_RTCSEL_M  0x7u
#define RTCCTRL_RTCSEL(x) ((x) & RTCCTRL_RTCSEL_M)

#define XOSC32K_CGM_M      0x3u
#define XOSC32K_CGM(x)     (((x) & XOSC32K_CGM_M) << 13)
#define XOSC32K_WRTLOCK    (1 << 12)
#define XOSC32K_STARTUP_M  0x7u
#define XOSC32K_STARTUP(x) (((x) & XOSC32K_STARTUP_M) << 8)
#define XOSC32K_ONDEMAND   (1 << 7)
#define XOSC32K_RUNSTDBY   (1 << 6)
#define XOSC32K_EN1K       (1 << 4)
#define XOSC32K_EN32K      (1 << 3)
#define XOSC32K_XTALEN     (1 << 2)
#define XOSC32K_ENABLE     (1 << 1)

#define CFDCTRL_CFDPRESC (1 << 2)
#define CFDCTRL_SWBACK   (1 << 1)
#define CFDCTRL_CFDEN    (1 << 0)

#define C99_EVCTRL_CFDEO (1 << 0)

#define OSCULP32K_WRTLOCK (1 << 15)
#define OSCULP32K_EN1K    (1 << 2)
#define OSCULP32K_EN32K   (1 << 1)

/* globals */
static struct GCLK_SAME5X *GCLK = (struct GCLK_SAME5X*)ADDR_GCLK;
static struct MCLK_SAME5X *MCLK = (struct MCLK_SAME5X*)ADDR_MCLK;
static struct OSCCTRL_SAME5X *OSCCTRL = (struct OSCCTRL_SAME5X*)ADDR_OSCCTRL;
/* static struct OSC32KCTRL_SAME5X *OSC32KCTRL = (struct OSC32KCTRL_SAME5X*)ADDR_OSC32KCTRL; */

#define DFLL48M_FREQ   48000000
#define OSC32K_FREQ    32768
#define OSC32K_1K_FREQ 1024

#define CLOCK_SAME5X_DEADLOCK_COUNT 1000000

static struct {
    clock_freq_t gclk[CLOCK_SAME5X_GCLK_COUNT];
    clock_freq_t pch[CLOCK_SAME5X_PCH_COUNT];
    clock_freq_t dpll[CLOCK_SAME5X_DPLL_COUNT];
} clocks;

/* Function: clock_same5x_gclk_reset
 * Resets the SAME5x clock system
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_same5x_gclk_reset(void)
{
    int deadlock = CLOCK_SAME5X_DEADLOCK_COUNT;

    GCLK->CTRLA = (uint32_t)SYNCBUSY_SWRST;

    while (deadlock-- != 0)
        if ((GCLK->SYNCBUSY & SYNCBUSY_SWRST) == 0)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

/* Function: clock_same5x_gclk_generator_setup
 * Configures a SAME5x GCLK generator
 *
 * Parameters:
 *  index - The GCLK generator index
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_same5x_gclk_generator_setup(size_t index, struct clock_same5x_gclk_settings *settings)
{
    if (!picoRTOS_assert(index < (size_t)CLOCK_SAME5X_GCLK_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->src < CLOCK_SAME5X_GCLK_SRC_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->div < 65536ul)) return -EINVAL;

    clock_freq_t freq;

    switch (settings->src) {
    /* case CLOCK_SAME5X_GCLK_SRC_XOSC0: */
    /* case CLOCK_SAME5X_GCLK_SRC_XOSC1: */
    /* case CLOCK_SAME5X_GCLK_SRC_GCLK_IN: */
    case CLOCK_SAME5X_GCLK_SRC_GCLK_GEN1: freq = clocks.gclk[1]; break;
    case CLOCK_SAME5X_GCLK_SRC_OSCULP32K: /*@fallthrough@*/
    case CLOCK_SAME5X_GCLK_SRC_XOSC32K: freq = (clock_freq_t)OSC32K_FREQ; break;
    case CLOCK_SAME5X_GCLK_SRC_DFLL: freq = (clock_freq_t)DFLL48M_FREQ; break;
    case CLOCK_SAME5X_GCLK_SRC_DPLL0: freq = clocks.dpll[0]; break;
    case CLOCK_SAME5X_GCLK_SRC_DPLL1: freq = clocks.dpll[1]; break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EINVAL;
    }

    GCLK->GENCTRLn[index] &= ~(GENCTRLn_DIV(GENCTRLn_DIV_M) | GENCTRLn_SRC(GENCTRLn_SRC_M));
    GCLK->GENCTRLn[index] |= GENCTRLn_DIV(settings->div) | GENCTRLn_SRC(settings->src);
    clocks.gclk[index] = freq / (clock_freq_t)settings->div;

    return 0;
}

static int gclk_generator_sync_busywait(size_t index)
{
    int deadlock = CLOCK_SAME5X_DEADLOCK_COUNT;

    while (deadlock-- != 0)
        if ((GCLK->SYNCBUSY & SYNCBUSY_GENCTRL(index)) == 0)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

/* Function: clock_same5x_gclk_generator_enable
 * Enables a SAME5x GCLK generator
 *
 * Parameters:
 *  index - The GCLK generator index
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_same5x_gclk_generator_enable(size_t index)
{
    if (!picoRTOS_assert(index < (size_t)CLOCK_SAME5X_GCLK_COUNT)) return -EINVAL;

    GCLK->GENCTRLn[index] |= GENCTRLn_GENEN;
    return gclk_generator_sync_busywait(index);
}

/* Function: clock_same5x_gclk_generator_disable
 * Disables a SAME5x GCLK generator
 *
 * Parameters:
 *  index - The GCLK generator index
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_same5x_gclk_generator_disable(size_t index)
{
    if (!picoRTOS_assert(index < (size_t)CLOCK_SAME5X_GCLK_COUNT)) return -EINVAL;

    GCLK->GENCTRLn[index] &= ~GENCTRLn_GENEN;
    return 0;
}

static int dfll_sync_busywait(uint32_t mask)
{
    int deadlock = CLOCK_SAME5X_DEADLOCK_COUNT;

    while (deadlock-- != 0)
        if ((OSCCTRL->DFLLSYNC & mask) == 0)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

/* Function: clock_same5x_dfll_setup
 * Configures the SAME5x DFLL clock
 *
 * Parameters:
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_same5x_dfll_setup(struct clock_same5x_dfll_settings *settings)
{
    /* TODO: check param boundaries */

    int res;

    OSCCTRL->DFLLCTRLA = (uint32_t)0;
    OSCCTRL->DFLLMUL = (uint32_t)(DFLLMUL_CSTEP(settings->cstep) |
                                  DFLLMUL_FSTEP(settings->fstep) |
                                  DFLLMUL_MUL(settings->mul));

    if ((res = dfll_sync_busywait((uint32_t)DFLLSYNC_DFLLMUL)) < 0)
        return res;

    OSCCTRL->DFLLCTRLB = 0;
    if ((res = dfll_sync_busywait((uint32_t)DFLLSYNC_DFLLCTRLB)) < 0)
        return res;

    return 0;
}

static int dfll_ready_busywait(void)
{
    int deadlock = CLOCK_SAME5X_DEADLOCK_COUNT;

    while (deadlock-- != 0)
        if ((OSCCTRL->STATUS & STATUS_DFLLRDY) != 0)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

/* Function: clock_same5x_dfll_enable
 * Enables the SAME5x DFLL clock
 *
 * Parameters:
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_same5x_dfll_enable(void)
{
    int res;

    OSCCTRL->DFLLCTRLA |= DFLLCTRLA_ENABLE;
    if ((res = dfll_sync_busywait((uint32_t)DFLLSYNC_ENABLE)) < 0)
        return res;

    /* ? */
    OSCCTRL->DFLLVAL = OSCCTRL->DFLLVAL;
    if ((res = dfll_sync_busywait((uint32_t)DFLLSYNC_DFLLVAL)) < 0)
        return res;

    OSCCTRL->DFLLCTRLB = (uint32_t)(DFLLCTRLB_WAITLOCK |
                                    DFLLCTRLB_CCDIS |
                                    DFLLCTRLB_USBCRM);

    return dfll_ready_busywait();
}

/* Function: clock_same5x_dfll_disable
 * Disables the SAME5x DFLL clock
 *
 * Parameters:
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_same5x_dfll_disable(void)
{
    OSCCTRL->DFLLCTRLA &= ~DFLLCTRLA_ENABLE;
    return 0;
}

static int dpll_sync_busywait(size_t index)
{
    if (!picoRTOS_assert(index < (size_t)CLOCK_SAME5X_DPLL_COUNT)) return -EINVAL;

    int deadlock = CLOCK_SAME5X_DEADLOCK_COUNT;

    while (deadlock-- != 0)
        if ((OSCCTRL->DPLLn[index].SYNCBUSY & DPLLn_SYNCBUSY_DPLLRATIO) == 0)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

static int dpll_lock_busywait(size_t index)
{
    if (!picoRTOS_assert(index < (size_t)CLOCK_SAME5X_DPLL_COUNT)) return -EINVAL;

    int deadlock = CLOCK_SAME5X_DEADLOCK_COUNT;

    while (deadlock-- != 0)
        if ((OSCCTRL->DPLLn[index].STATUS & DPLLn_STATUS_CLKRDY) != 0 &&
            (OSCCTRL->DPLLn[index].STATUS & DPLLn_STATUS_LOCK) != 0)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

/* Function: clock_same5x_dpll_setup
 * Configures a SAME5x DPLL
 *
 * Parameters:
 *  index - The DPLL index
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_same5x_dpll_setup(size_t index, struct clock_same5x_dpll_settings *settings)
{
    if (!picoRTOS_assert(index < (size_t)CLOCK_SAME5X_DPLL_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->refclk < CLOCK_SAME5X_DPLL_REFCLK_COUNT)) return -EINVAL;

    int res;
    clock_freq_t freq;
    clock_id_t clkid = CLOCK_SAME5X_FDPLL0;

    if (index == (size_t)1)
        clkid = CLOCK_SAME5X_FDPLL1;

    freq = clock_get_freq(clkid);
    if (!picoRTOS_assert(freq > 0))
        return -EIO;

    /* LDR + LDRFRAC */
    unsigned long ldr = (unsigned long)(settings->freq / freq);
    unsigned long ldrfrac = (unsigned long)(settings->freq % freq) * 32ul / (unsigned long)freq;

    /* refclk */
    OSCCTRL->DPLLn[index].CTRLB &= ~DPLLn_CTRLB_REFCLK(DPLLn_CTRLB_REFCLK_M);
    OSCCTRL->DPLLn[index].CTRLB |= (DPLLn_CTRLB_REFCLK(settings->refclk) |
                                    DPLLn_CTRLB_LBYPASS); /* SAMD51 bugfix */
    /* ratio */
    OSCCTRL->DPLLn[index].RATIO = (uint32_t)(DPLLn_RATIO_LDRFRAC(ldrfrac) |
                                             DPLLn_RATIO_LDR(ldr - 1));

    if ((res = dpll_sync_busywait(index)) < 0)
        return res;

    clocks.dpll[index] = (clock_freq_t)settings->freq;
    return 0;
}

/* Function: clock_same5x_dpll_enable
 * Enables a SAME5x DPLL
 *
 * Parameters:
 *  index - The DPLL index
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_same5x_dpll_enable(size_t index)
{
    if (!picoRTOS_assert(index < (size_t)CLOCK_SAME5X_DPLL_COUNT)) return -EINVAL;

    OSCCTRL->DPLLn[index].CTRLA = (uint32_t)DPLLn_CTRLA_ENABLE;
    return dpll_lock_busywait(index);
}

/* Function: clock_same5x_dpll_disable
 * Disables a SAME5x DPLL
 *
 * Parameters:
 *  index - The DPLL index
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_same5x_dpll_disable(size_t index)
{
    if (!picoRTOS_assert(index < (size_t)CLOCK_SAME5X_DPLL_COUNT)) return -EINVAL;

    OSCCTRL->DPLLn[index].CTRLA &= ~DPLLn_CTRLA_ENABLE;
    return 0;
}

/* Function: clock_same5x_mclk_enable
 * Enables a SAME5x MCLK gate
 *
 * Parameters:
 *  mclk - The mclk to enable
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_same5x_mclk_enable(clock_same5x_mclk_t mclk)
{
    if (!picoRTOS_assert(mclk < CLOCK_SAME5X_MCLK_COUNT)) return -EINVAL;

    size_t index = (size_t)mclk >> 5;
    size_t shift = (size_t)0x1f & mclk;

    switch (index) {
    case 0: MCLK->AHBMASK |= (1 << shift); break;
    case 1: MCLK->APBAMASK |= (1 << shift); break;
    case 2: MCLK->APBBMASK |= (1 << shift); break;
    case 3: MCLK->APBCMASK |= (1 << shift); break;
    case 4: MCLK->APBDMASK |= (1 << shift); break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    return 0;
}

/* Function: clock_same5x_mclk_disable
 * Disables a SAME5x MCLK gate
 *
 * Parameters:
 *  mclk - The mclk to disable
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_same5x_mclk_disable(clock_same5x_mclk_t mclk)
{
    if (!picoRTOS_assert(mclk < CLOCK_SAME5X_MCLK_COUNT)) return -EINVAL;

    size_t index = (size_t)mclk >> 5;
    size_t shift = (size_t)0x1f & mclk;

    switch (index) {
    case 0: MCLK->AHBMASK &= ~(1 << shift); break;
    case 1: MCLK->APBAMASK &= ~(1 << shift); break;
    case 2: MCLK->APBBMASK &= ~(1 << shift); break;
    case 3: MCLK->APBCMASK &= ~(1 << shift); break;
    case 4: MCLK->APBDMASK &= ~(1 << shift); break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    return 0;
}

/* Function: clock_same5x_setup
 * Configures a SAME5x clock / attach a clock id to a gclk generator
 *
 * Parameters:
 *  clkid - The clock id to configure
 *  gclk_index - The GCLK generator to attach to
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_same5x_setup(clock_id_t clkid, size_t gclk_index)
{
    if (!picoRTOS_assert(clkid < (clock_id_t)CLOCK_SAME5X_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(gclk_index < (size_t)CLOCK_SAME5X_GCLK_COUNT)) return -EINVAL;

    GCLK->PCHCTRLn[clkid] &= ~PCHCTRLn_GEN(PCHCTRLn_GEN_M);
    GCLK->PCHCTRLn[clkid] |= PCHCTRLn_GEN(gclk_index);

    clocks.pch[clkid] = clocks.gclk[gclk_index];
    return 0;
}

/* Function: clock_same5x_enable
 * Enables a SAME5x clock
 *
 * Parameters:
 *  clkid - The clock id
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_same5x_enable(clock_id_t clkid)
{
    if (!picoRTOS_assert(clkid < (clock_id_t)CLOCK_SAME5X_COUNT)) return -EINVAL;

    GCLK->PCHCTRLn[clkid] |= PCHCTRLn_CHEN;
    return 0;
}

/* Function: clock_same5x_disable
 * Disables a SAME5x clock
 *
 * Parameters:
 *  clkid - The clock id
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_same5x_disable(clock_id_t clkid)
{
    if (!picoRTOS_assert(clkid < (clock_id_t)CLOCK_SAME5X_COUNT)) return -EINVAL;

    GCLK->PCHCTRLn[clkid] &= ~PCHCTRLn_CHEN;
    return 0;
}

/* hooks */

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    if (!picoRTOS_assert(clkid < (clock_id_t)CLOCK_SAME5X_COUNT)) return (clock_freq_t)-EINVAL;

    return clocks.pch[clkid];
}
