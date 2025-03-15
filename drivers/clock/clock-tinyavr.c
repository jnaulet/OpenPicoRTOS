#include "clock-tinyavr.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#define OSCULP32K_HZ 32768
#define CCP_IOREG 0xd8

struct CLKCTRL {
    volatile uint8_t MCLKCTRLA;
    volatile uint8_t MCLKCTRLB;
    volatile uint8_t MCLKLOCK;
    volatile uint8_t MCLKSTATUS;
    uint8_t RESERVED0[12];
    volatile uint8_t OSC20MCTRLA;
    volatile uint8_t OSC20MCALIBA;
    volatile uint8_t OSC20MCALIBB;
    uint8_t RESERVED1[5];
    volatile uint8_t OSC32KCTRLA;
    uint8_t RESERVED2[3];
    volatile uint8_t XOSC32KCTRLA;
};

#define MCLKCTRLA_CLKOUT    (1 << 7)
#define MCLKCTRLA_CLKSEL_M  0x3u
#define MCLKCTRLA_CLKSEL(x) ((x) & MCLKCTRLA_CLKSEL_M)

#define MCLKCTRLB_PDIV_M  0xfu
#define MCLKCTRLB_PDIV(x) (((x) & MCLKCTRLB_PDIV_M) << 1)
#define MCLKCTRLB_PEN     (1 << 0)

/* Global instances */
static struct CLKCTRL *CLKCTRL = (struct CLKCTRL*)ADDR_CLKCTRL;
static volatile uint8_t *OSCCFG = (volatile uint8_t*)(ADDR_FUSES + 0x2);
static volatile uint8_t *CCP = (volatile uint8_t*)(ADDR_CPU + 0x4);

static struct {
    clock_freq_t freqsel;
    clock_freq_t clk_main;
    clock_freq_t per_div;
} clocks;

static int parse_osccfg(void)
{
    int freqsel = 0x3 & *OSCCFG;

    if (freqsel == 0x1) {
        clocks.freqsel = (clock_freq_t)16000000;
        return 0;
    }

    if (freqsel == 0x2) {
        clocks.freqsel = (clock_freq_t)20000000;
        return 0;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int setup_osc20m(void)
{
    /* unprotect & set */
    *CCP = (uint8_t)CCP_IOREG;
    CLKCTRL->MCLKCTRLA = (uint8_t)MCLKCTRLA_CLKSEL(0);

    clocks.clk_main = clocks.freqsel;
    return 0;
}

static int setup_osculp32k(void)
{
    /* unprotect & set */
    *CCP = (uint8_t)CCP_IOREG;
    CLKCTRL->MCLKCTRLA = (uint8_t)MCLKCTRLA_CLKSEL(1);

    clocks.clk_main = (clock_freq_t)OSCULP32K_HZ;
    return 0;
}

static int setup_prescaler(clock_tinyavr_pdiv_t pdiv)
{
    picoRTOS_assert(pdiv < CLOCK_TINYAVR_PDIV_COUNT, return -EINVAL);

    switch (pdiv) {
    case CLOCK_TINYAVR_PDIV_2: clocks.per_div = (clock_freq_t)2; break;
    case CLOCK_TINYAVR_PDIV_4: clocks.per_div = (clock_freq_t)4; break;
    case CLOCK_TINYAVR_PDIV_8: clocks.per_div = (clock_freq_t)8; break;
    case CLOCK_TINYAVR_PDIV_16: clocks.per_div = (clock_freq_t)16; break;
    case CLOCK_TINYAVR_PDIV_32: clocks.per_div = (clock_freq_t)32; break;
    case CLOCK_TINYAVR_PDIV_64: clocks.per_div = (clock_freq_t)64; break;
    case CLOCK_TINYAVR_PDIV_6: clocks.per_div = (clock_freq_t)6; break;
    case CLOCK_TINYAVR_PDIV_10: clocks.per_div = (clock_freq_t)10; break;
    case CLOCK_TINYAVR_PDIV_12: clocks.per_div = (clock_freq_t)12; break;
    case CLOCK_TINYAVR_PDIV_24: clocks.per_div = (clock_freq_t)24; break;
    case CLOCK_TINYAVR_PDIV_48: clocks.per_div = (clock_freq_t)48; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    /* unprotect & set */
    *CCP = (uint8_t)CCP_IOREG;
    CLKCTRL->MCLKCTRLB = (uint8_t)(MCLKCTRLB_PDIV(pdiv) | MCLKCTRLB_PEN);
    return 0;
}

/* Function: clock_tinyavr_init
 * Intializes the tinyAVR 1-series clock system
 *
 * Parameters:
 *  settings - The clocks settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_tinyavr_init(const struct clock_settings *settings)
{
    int res;

    /* FREQ SEL */
    if (parse_osccfg() < 0)
        return -EIO;

    /* CLKSEL */
    switch (settings->clksel) {
    case CLOCK_TINYAVR_CLKSEL_OSC20M: res = setup_osc20m(); break;
    case CLOCK_TINYAVR_CLKSEL_OSCULP32K: res = setup_osculp32k(); break;
    case CLOCK_TINYAVR_CLKSEL_XOSC32K:  /*@fallthrough@*/
    case CLOCK_TINYAVR_CLKSEL_EXTCLK:   /*@fallthrough@*/
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    if (res < 0)
        return res;

    /* CLKOUT */
    if (settings->clkout_enable) {
        *CCP = (uint8_t)CCP_IOREG;
        CLKCTRL->MCLKCTRLA |= MCLKCTRLA_CLKOUT;
    }

    /* PEN/PDIV */
    if (settings->prescaler_enable) {
        if ((res = setup_prescaler(settings->prescaler_div)) < 0)
            return res;
    }else{
        *CCP = (uint8_t)CCP_IOREG;
        CLKCTRL->MCLKCTRLB = (uint8_t)0;
        clocks.per_div = (clock_freq_t)1;
    }

    /* LOCK */
    if (settings->lock_enable) {
        *CCP = (uint8_t)CCP_IOREG;
        CLKCTRL->MCLKLOCK = (uint8_t)1;
    }

    return 0;
}

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    picoRTOS_assert(clkid < (clock_id_t)CLOCK_TINYAVR_COUNT,
                    return (clock_freq_t)-EINVAL);

    switch (clkid) {
    case CLOCK_TINYAVR_CLK_CPU: return clocks.clk_main / clocks.per_div;
    case CLOCK_TINYAVR_CLK_RTC: return (clock_freq_t)OSCULP32K_HZ;
    case CLOCK_TINYAVR_CLK_WDT: return (clock_freq_t)(OSCULP32K_HZ >> 5);
    case CLOCK_TINYAVR_CLK_TCD: return clocks.freqsel;
    default: break;
    }

    /* error */
    picoRTOS_break();
    /*@notreached@*/
    return (clock_freq_t)-EINVAL;
}
