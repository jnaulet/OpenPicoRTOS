#include "clock-rp2040.h"
#include "picoRTOS.h"

#ifndef CONFIG_DEADLOCK_COUNT
# error CONFIG_DEADLOCK_COUNT must be configured for this driver
#endif

#include <stdint.h>

/* CLK */

struct CLK {
    struct {
        volatile uint32_t CTRL;
        volatile uint32_t DIV;
        volatile uint32_t SELECTED;
    } CLK_GPOUTn[4];
    volatile uint32_t CLK_REF_CTRL;
    volatile uint32_t CLK_REF_DIV;
    volatile uint32_t CLK_REF_SELECTED;
    volatile uint32_t CLK_SYS_CTRL;
    volatile uint32_t CLK_SYS_DIV;
    volatile uint32_t CLK_SYS_SELECTED;
    volatile uint32_t CLK_PERI_CTRL;
    uint32_t RESERVED0;
    volatile uint32_t CLK_PERI_SELECTED;
    volatile uint32_t CLK_USB_CTRL;
    volatile uint32_t CLK_USB_DIV;
    volatile uint32_t CLK_USB_SELECTED;
    volatile uint32_t CLK_ADC_CTRL;
    volatile uint32_t CLK_ADC_DIV;
    volatile uint32_t CLK_ADC_SELECTED;
    volatile uint32_t CLK_RTC_CTRL;
    volatile uint32_t CLK_RTC_DIV;
    volatile uint32_t CLK_RTC_SELECTED;
    volatile uint32_t CLK_SYS_RESUS_CTRL;
    volatile uint32_t CLK_SYS_RESUS_STATUS;
    volatile uint32_t FC0_REF_KHZ;
    volatile uint32_t FC0_MIN_KHZ;
    volatile uint32_t FC0_MAX_KHZ;
    volatile uint32_t FC0_DELAY;
    volatile uint32_t FC0_INTERVAL;
    volatile uint32_t FC0_SRC;
    volatile uint32_t FC0_STATUS;
    volatile uint32_t FC0_RESULT;
    volatile uint32_t WAKE_EN0;
    volatile uint32_t WAKE_EN1;
    volatile uint32_t SLEEP_EN0;
    volatile uint32_t SLEEP_EN1;
    volatile uint32_t ENABLED0;
    volatile uint32_t ENABLED1;
    volatile uint32_t INTR;
    volatile uint32_t INTE;
    volatile uint32_t INTF;
    volatile uint32_t INTS;
};

#define CLK_GPOUTn_CTRL_NUDGE     (1 << 20)
#define CLK_GPOUTn_CTRL_PHASE_M   0x3u
#define CLK_GPOUTn_CTRL_PHASE(x)  (((x) & CLK_GPOUTn_CTRL_PHASE_M) << 16)
#define CLK_GPOUTn_CTRL_DC50      (1 << 12)
#define CLK_GPOUTn_CTRL_ENABLE    (1 << 11)
#define CLK_GPOUTn_CTRL_KILL      (1 << 10)
#define CLK_GPOUTn_CTRL_AUXSRC_M  0xfu
#define CLK_GPOUTn_CTRL_AUXSRC(x) (((x) & CLK_GPOUTn_CTRL_AUXSRC_M) << 5)

#define CLK_GPOUTn_DIV_INT_M   0xffffffu
#define CLK_GPOUTn_DIV_INT(x)  (((x) & CLK_GPOUTn_DIV_INT_M) << 8)
#define CLK_GPOUTn_DIV_FRAC_M  0xffu
#define CLK_GPOUTn_DIV_FRAC(x) ((x) & CLK_GPOUTn_DIV_FRAC_M)

#define CLK_REF_CTRL_AUXSRC_M  0x3u
#define CLK_REF_CTRL_AUXSRC(x) (((x) & CLK_REF_CTRL_AUXSRC_M) << 5)
#define CLK_REF_CTRL_SRC_M     0x3u
#define CLK_REF_CTRL_SRC(x)    ((x) & CLK_REF_CTRL_SRC_M)

#define CLK_REF_DIV_INT_M  0x3u
#define CLK_REF_DIV_INT(x) (((x) & CLK_REF_DIV_INT_M) << 8)

#define CLK_SYS_CTRL_AUXSRC_M  0x7u
#define CLK_SYS_CTRL_AUXSRC(x) (((x) & CLK_SYS_CTRL_AUXSRC_M) << 5)
#define CLK_SYS_CTRL_SRC       (1 << 0)

#define CLK_SYS_DIV_INT_M   0xffffffu
#define CLK_SYS_DIV_INT(x)  (((x) & CLK_SYS_DIV_INT_M) << 8)
#define CLK_SYS_DIV_FRAC_M  0xffu
#define CLK_SYS_DIV_FRAC(x) ((x) & CLK_SYS_DIV_FRAC_M)

#define CLK_PERI_CTRL_ENABLE    (1 << 11)
#define CLK_PERI_CTRL_AUXSRC_M  0x7u
#define CLK_PERI_CTRL_AUXSRC(x) (((x) & CLK_PERI_CTRL_AUXSRC_M) << 5)

#define CLK_USB_CTRL_ENABLE    (1 << 11)
#define CLK_USB_CTRL_AUXSRC_M  0x7u
#define CLK_USB_CTRL_AUXSRC(x) (((x) & CLK_USB_CTRL_AUXSRC_M) << 5)

#define CLK_ADC_CTRL_ENABLE    (1 << 11)
#define CLK_ADC_CTRL_AUXSRC_M  0x7u
#define CLK_ADC_CTRL_AUXSRC(x) (((x) & CLK_ADC_CTRL_AUXSRC_M) << 5)

#define CLK_RTC_CTRL_ENABLE    (1 << 11)
#define CLK_RTC_CTRL_AUXSRC_M  0x7u
#define CLK_RTC_CTRL_AUXSRC(x) (((x) & CLK_RTC_CTRL_AUXSRC_M) << 5)

#define CLK_RTC_DIV_INT_M   0xffffffu
#define CLK_RTC_DIV_INT(x)  (((x) & CLK_RTC_DIV_INT_M) << 8)
#define CLK_RTC_DIV_FRAC_M  0xffu
#define CLK_RTC_DIV_FRAC(x) ((x) & CLK_RTC_DIV_FRAC_M)

/* XOSC */

struct XOSC {
    volatile uint32_t CTRL;
    volatile uint32_t STATUS;
    volatile uint32_t DORMANT;
    volatile uint32_t STARTUP;
    uint32_t RESERVED0[4];
    volatile uint32_t COUNT;
};

#define XOSC_CTRL_ENABLE_M      0xfffu
#define XOSC_CTRL_ENABLE(x)     (((x) & XOSC_CTRL_ENABLE_M) << 12)
#define XOSC_CTRL_FREQ_RANGE_M  0xfffu
#define XOSC_CTRL_FREQ_RANGE(x) ((x) & XOSC_CTRL_FREQ_RANGE_M)

#define XOSC_STATUS_STABLE   (1 << 31)
#define XOSC_STATUS_BADWRITE (1 << 24)

#define XOSC_STARTUP_X4       (1 << 20)
#define XOSC_STARTUP_DELAY_M  0x1fffu
#define XOSC_STARTUP_DELAY(x) ((x) & XOSC_STARTUP_DELAY_M)

#define XOSC_COUNT_COUNT_M  0xffu
#define XOSC_COUNT_COUNT(x) ((x) & XOSC_COUNT_COUNT_M)

/* PLL */

struct PLL {
    volatile uint32_t CS;
    volatile uint32_t PWR;
    volatile uint32_t FBDIV_INT;
    volatile uint32_t PRIM;
};

#define PLL_CS_LOCK      (1 << 31)
#define PLL_CS_BYPASS    (1 << 8)
#define PLL_CS_REFDIV_M  0x1fu
#define PLL_CS_REFDIV(x) ((x) & PLL_CS_REFDIV_M)

#define PLL_PWR_VCOPD     (1 << 5)
#define PLL_PWR_POSTDIVPD (1 << 3)
#define PLL_PWR_DSMPD     (1 << 2)
#define PLL_PWR_PD        (1 << 0)

#define PLL_FBDIV_INT_M  0x7fu
#define PLL_FBDIV_INT(x) ((x) & PLL_FBDIV_INT_M)

#define PLL_PRIM_POSTDIV1_M  0x7u
#define PLL_PRIM_POSTDIV1(x) (((x) & PLL_PRIM_POSTDIV1_M) << 16)
#define PLL_PRIM_POSTDIV2_M  0x7u
#define PLL_PRIM_POSTDIV2(x) (((x) & PLL_PRIM_POSTDIV2_M) << 12)

/* "globals" */
static struct CLK *CLK = (struct CLK*)0x40008000;
static struct XOSC *XOSC = (struct XOSC*)0x40024000;
static struct PLL *PLL_SYS = (struct PLL*)0x40028000;
static struct PLL *PLL_USB = (struct PLL*)0x4002c000;

static struct {
    /* plls */
    clock_freq_t xosc;
    clock_freq_t pll_sys;
    clock_freq_t pll_usb;
    /* exported clocks */
    clock_freq_t sys;
    clock_freq_t ref;
    clock_freq_t peri;
    clock_freq_t usb;
    clock_freq_t adc;
    clock_freq_t rtc;
    clock_freq_t gpout[4];
} clocks;

static int xosc_config(unsigned long hz)
{
#define XOSC_DELAY         47
#define XOSC_ENABLE        0xfab
#define XOSC_FREQ_RANGE    0xaa0

    if (!picoRTOS_assert(hz <= 15000000ul)) return -EINVAL;

    int deadlock = CONFIG_DEADLOCK_COUNT;

    XOSC->STARTUP = (uint32_t)XOSC_STARTUP_DELAY(XOSC_DELAY);
    XOSC->CTRL = (uint32_t)(XOSC_CTRL_ENABLE(XOSC_ENABLE) |
                            XOSC_CTRL_FREQ_RANGE(XOSC_FREQ_RANGE));

    /* wait */
    while ((XOSC->STATUS & XOSC_STATUS_STABLE) == 0 &&
           deadlock-- != 0) {
    }

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    clocks.xosc = (clock_freq_t)hz;
    return 0;
}

static int pll_setup(struct PLL *pll, int refdiv, int fbdiv,
                     int postdiv1, int postdiv2)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* if locked, disable */
    if ((pll->CS & PLL_CS_LOCK) != 0)
        pll->PWR |= (PLL_PWR_VCOPD | PLL_PWR_PD | PLL_PWR_POSTDIVPD);

    /* set dividers */
    pll->CS = (uint32_t)PLL_CS_REFDIV(refdiv);
    pll->FBDIV_INT = (uint32_t)PLL_FBDIV_INT(fbdiv);

    /* turn on pll */
    pll->PWR &= ~(PLL_PWR_VCOPD | PLL_PWR_PD);

    /* wait pll lock */
    while ((pll->CS & PLL_CS_LOCK) == 0 &&
           deadlock-- != 0) {
    }

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    /* post dividers */
    pll->PRIM = (uint32_t)(PLL_PRIM_POSTDIV1(postdiv1) |
                           PLL_PRIM_POSTDIV2(postdiv2));

    /* turn on post dividers */
    pll->PWR &= ~PLL_PWR_POSTDIVPD;

    return 0;
}

static int pll_config(struct PLL *pll, unsigned long hz)
{
#define REFDIV_COUNT   64
#define FBDIV_MIN      16
#define FBDIV_COUNT    320
#define POSTDIVn_COUNT 8

    if (!picoRTOS_assert(hz > 0)) return -EINVAL;

    /* PLL SYS: 12 / 1 = 12MHz * 125 = 1500MHZ / 6 / 2 = 125MHz */
    /* PLL USB: 12 / 1 = 12MHz * 40 = 480 MHz / 5 / 2 = 48MHz */

    int refdiv;
    int fbdiv;
    int postdiv1;
    int postdiv2;

    for (refdiv = 1; refdiv < REFDIV_COUNT; refdiv++) {
        for (fbdiv = FBDIV_MIN; fbdiv < FBDIV_COUNT; fbdiv++) {
            for (postdiv1 = 1; postdiv1 < POSTDIVn_COUNT; postdiv1++) {
                for (postdiv2 = 1; postdiv2 < POSTDIVn_COUNT; postdiv2++) {
                    /* compute end frequency */
                    int pll_hz = (((int)clocks.xosc / refdiv) * fbdiv) / (postdiv1 * postdiv2);
                    if ((unsigned long)pll_hz == hz)
                        return pll_setup(pll, refdiv, fbdiv, postdiv1, postdiv2);
                }
            }
        }
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int pll_sys_config(unsigned long hz)
{
    if (!picoRTOS_assert(hz > 0)) return -EINVAL;

    int res;

    /* pico-sdk configured this one already */
    if ((res = pll_config(PLL_SYS, hz)) < 0)
        return res;

    clocks.pll_sys = (clock_freq_t)hz;
    return 0;
}

static int pll_usb_config(unsigned long hz)
{
    if (!picoRTOS_assert(hz > 0)) return -EINVAL;

    int res;

    /* pico-sdk configured this one already */
    if ((res = pll_config(PLL_USB, hz)) < 0)
        return res;

    clocks.pll_usb = (clock_freq_t)hz;
    return 0;
}

static int sys_config_ref(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* force pll as src */
    CLK->CLK_SYS_DIV = (uint32_t)CLK_SYS_DIV_INT(1);
    CLK->CLK_SYS_CTRL = (uint32_t)0;

    while ((CLK->CLK_SYS_SELECTED & (1 << 0)) == 0 &&
           deadlock-- != 0) {
    }

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

static int sys_config(unsigned long sys_div)
{
    if (!picoRTOS_assert(sys_div > 0)) return -EINVAL;

    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* force pll as src */
    CLK->CLK_SYS_DIV = (uint32_t)CLK_SYS_DIV_INT(sys_div);
    CLK->CLK_SYS_CTRL = (uint32_t)CLK_SYS_CTRL_AUXSRC(0);
    CLK->CLK_SYS_CTRL |= CLK_SYS_CTRL_SRC;

    while ((CLK->CLK_SYS_SELECTED & (1 << 1)) == 0 &&
           deadlock-- != 0) {
    }

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    clocks.sys = clocks.pll_sys / (clock_freq_t)sys_div;
    return 0;
}

static int ref_config(unsigned long ref_div)
{
#define REF_SRC 0x2 /* xosc */

    if (!picoRTOS_assert(ref_div > 0ul)) return -EINVAL;
    if (!picoRTOS_assert(ref_div < 4ul)) return -EINVAL;

    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* force xosc as clksrc */
    CLK->CLK_REF_DIV = (uint32_t)CLK_REF_DIV_INT(ref_div);
    CLK->CLK_REF_CTRL = (uint32_t)CLK_REF_CTRL_SRC(REF_SRC);

    /* poll */
    while ((CLK->CLK_REF_SELECTED & (1 << REF_SRC)) == 0 &&
           deadlock-- != 0) {
    }

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    clocks.ref = clocks.xosc / (clock_freq_t)ref_div;
    return 0;
}

static int gpout_config(size_t index, unsigned long gpout_div)
{
    if (!picoRTOS_assert(gpout_div > 0)) return -EINVAL;

    /* force clk_sys as auxsrc */
    CLK->CLK_GPOUTn[index].CTRL = (uint32_t)CLK_GPOUTn_CTRL_AUXSRC(0x6);
    CLK->CLK_GPOUTn[index].DIV = (uint32_t)(CLK_GPOUTn_DIV_INT(gpout_div) | CLK_GPOUTn_DIV_FRAC(0));

    clocks.gpout[index] = clocks.sys / (clock_freq_t)gpout_div;
    return 0;
}

static int peri_config(void)
{
    /* force clk_sys as auxsrc */
    CLK->CLK_PERI_CTRL = (uint32_t)CLK_PERI_CTRL_AUXSRC(0x0);

    clocks.peri = clocks.sys;
    return 0;
}

static int usb_config(void)
{
    /* force pll_usb as auxsrc */
    CLK->CLK_USB_CTRL = (uint32_t)CLK_USB_CTRL_AUXSRC(0);

    clocks.usb = clocks.pll_usb;
    return 0;
}

static int adc_config(void)
{
    /* force adc to pll_usb (48Mhz) */
    CLK->CLK_ADC_CTRL = (uint32_t)CLK_ADC_CTRL_AUXSRC(0);

    clocks.adc = clocks.pll_usb;
    return 0;
}

static int rtc_config(void)
{
#define RTC_FREQ 46875

    int div_int = (int)clocks.xosc / RTC_FREQ;
    int div_frac = (((int)clocks.xosc % RTC_FREQ) * 100) / RTC_FREQ;

    /* force rtc to xosc */
    CLK->CLK_RTC_CTRL = (uint32_t)CLK_RTC_CTRL_AUXSRC(0x3); /* xosc */
    CLK->CLK_RTC_DIV = (uint32_t)(CLK_RTC_DIV_INT(div_int) | CLK_RTC_DIV_FRAC(div_frac));

    clocks.rtc = (clock_freq_t)RTC_FREQ;
    return 0;
}

/* Function: clock_rp2040_init
 * Intializes the rp2040 clock system
 *
 * Parameters:
 *  settings - The clocks settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_rp2040_init(struct clock_settings *settings)
{
    if (!picoRTOS_assert(settings->xosc > 0)) return -EINVAL;

    int res;

    /* XOSC */
    if ((res = xosc_config(settings->xosc)) < 0)
        return res;

    /* CLK_REF */
    if ((res = ref_config(settings->ref_div)) < 0)
        return res;

    /* CLK_SYS reset */
    if ((res = sys_config_ref()) < 0)
        return res;

    /* PLLs. FIXME: will fail with pico-sdk as it's already configured */
    if ((res = pll_sys_config(settings->pll_sys)) < 0 ||
        (res = pll_usb_config(48000000ul)) < 0) /* MUST be 48Mhz according to doc */
        return res;

    /* CLK_SYS */
    if ((res = sys_config(settings->sys_div)) < 0)
        return res;

    /* CLK_GPOUT0-3: GPIO Muxing */
    if ((res = gpout_config((size_t)0, settings->clk_gpout0_div)) < 0 ||
        (res = gpout_config((size_t)1, settings->clk_gpout1_div)) < 0 ||
        (res = gpout_config((size_t)2, settings->clk_gpout2_div)) < 0 ||
        (res = gpout_config((size_t)3, settings->clk_gpout3_div)) < 0)
        return res;

    /* CLK_PERI, CLK_USB, CLK_ADC, CLK_RTC */
    if ((res = peri_config()) < 0 ||
        (res = usb_config()) < 0 ||
        (res = adc_config()) < 0 ||
        (res = rtc_config()) < 0)
        return res;

    return 0;
}

/* hooks */

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    if (!picoRTOS_assert(clkid <= CLOCK_RP2040_GPOUT3)) return (clock_freq_t)-EINVAL;

    switch (clkid) {
    case CLOCK_RP2040_SYS: return clocks.sys;
    case CLOCK_RP2040_REF: return clocks.ref;
    case CLOCK_RP2040_PERI: return clocks.peri;
    case CLOCK_RP2040_USB: return clocks.usb;
    case CLOCK_RP2040_ADC: return clocks.adc;
    case CLOCK_RP2040_RTC: return clocks.rtc;
    case CLOCK_RP2040_GPOUT0: return clocks.gpout[0];
    case CLOCK_RP2040_GPOUT1: return clocks.gpout[1];
    case CLOCK_RP2040_GPOUT2: return clocks.gpout[2];
    case CLOCK_RP2040_GPOUT3: return clocks.gpout[3];
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return (clock_freq_t)-EIO;
}

/* Function: clock_rp2040_enable
 * Enables a particular clock
 *
 * Parameters:
 *  clkid - The clock identifier
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_rp2040_enable(clock_id_t clkid)
{
    switch (clkid) {
    case CLOCK_RP2040_PERI: CLK->CLK_PERI_CTRL |= CLK_PERI_CTRL_ENABLE; break;
    case CLOCK_RP2040_USB: CLK->CLK_USB_CTRL |= CLK_USB_CTRL_ENABLE; break;
    case CLOCK_RP2040_ADC: CLK->CLK_ADC_CTRL |= CLK_ADC_CTRL_ENABLE; break;
    case CLOCK_RP2040_RTC: CLK->CLK_RTC_CTRL |= CLK_RTC_CTRL_ENABLE; break;
    case CLOCK_RP2040_GPOUT0: CLK->CLK_GPOUTn[0].CTRL |= CLK_GPOUTn_CTRL_ENABLE; break;
    case CLOCK_RP2040_GPOUT1: CLK->CLK_GPOUTn[1].CTRL |= CLK_GPOUTn_CTRL_ENABLE; break;
    case CLOCK_RP2040_GPOUT2: CLK->CLK_GPOUTn[2].CTRL |= CLK_GPOUTn_CTRL_ENABLE; break;
    case CLOCK_RP2040_GPOUT3: CLK->CLK_GPOUTn[3].CTRL |= CLK_GPOUTn_CTRL_ENABLE; break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    return 0;
}

/* Function: clock_rp2040_disable
 * Disables a particular clock
 *
 * Parameters:
 *  clkid - The clock identifier
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_rp2040_disable(clock_id_t clkid)
{
    switch (clkid) {
    case CLOCK_RP2040_PERI: CLK->CLK_PERI_CTRL &= ~CLK_PERI_CTRL_ENABLE; break;
    case CLOCK_RP2040_USB: CLK->CLK_USB_CTRL &= ~CLK_USB_CTRL_ENABLE; break;
    case CLOCK_RP2040_ADC: CLK->CLK_ADC_CTRL &= ~CLK_ADC_CTRL_ENABLE; break;
    case CLOCK_RP2040_RTC: CLK->CLK_RTC_CTRL &= ~CLK_RTC_CTRL_ENABLE; break;
    case CLOCK_RP2040_GPOUT0: CLK->CLK_GPOUTn[0].CTRL &= ~CLK_GPOUTn_CTRL_ENABLE; break;
    case CLOCK_RP2040_GPOUT1: CLK->CLK_GPOUTn[1].CTRL &= ~CLK_GPOUTn_CTRL_ENABLE; break;
    case CLOCK_RP2040_GPOUT2: CLK->CLK_GPOUTn[2].CTRL &= ~CLK_GPOUTn_CTRL_ENABLE; break;
    case CLOCK_RP2040_GPOUT3: CLK->CLK_GPOUTn[3].CTRL &= ~CLK_GPOUTn_CTRL_ENABLE; break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    return 0;
}
