#include "clock-mc9s08ptxx.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct ICS {
    volatile uint8_t ICS_C1;
    volatile uint8_t ICS_C2;
    volatile uint8_t ICS_C3;
    volatile uint8_t ICS_C4;
    volatile uint8_t ICS_S;
    uint8_t RESERVED0;
    volatile uint8_t ICS_OSCSC;
};

#define ICS_C1_CLKS_M   0x3u
#define ICS_C1_CLKS(x)  (((x) & ICS_C1_CLKS_M) << 6)
#define ICS_C1_RDIV_M   0x7u
#define ICS_C1_RDIV(x)  (((x) & ICS_C1_RDIV_M) << 3)
#define ICS_C1_IREFS    (1 << 2)
#define ICS_C1_IRCLKEN  (1 << 1)
#define ICS_C1_IREFSTEN (1 << 0)

#define ICS_C2_BDIV_M  0x7u
#define ICS_C2_BDIV(x) (((x) & ICS_C2_BDIV_M) << 5)
#define ICS_C2_LP      (1 << 4)

#define ICS_C3_SCTRIM_M  0xffu
#define ICS_C3_SCTRIM(x) ((x) & ICS_C3_SCTRIM_M)

#define ICS_C4_LOLIE   (1 << 7)
#define ICS_C4_CME     (1 << 5)
#define ICS_C4_SCFTRIM (1 << 0)

#define ICS_S_LOLS     (1 << 7)
#define ICS_S_LOCK     (1 << 6)
#define ICS_S_IREFST   (1 << 4)
#define ICS_S_CLKST_M  0x3u
#define ICS_S_CLKST(x) (((x) & ICS_S_CLKST_M) << 2)

#define ICS_OSCSC_OSCEN   (1 << 7)
#define ICS_OSCSC_OSCSTEN (1 << 5)
#define ICS_OSCSC_OSCOS   (1 << 4)
#define ICS_OSCSC_RANGE   (1 << 2)
#define ICS_OSCSC_HGO     (1 << 1)
#define ICS_OSCSC_OSCINIT (1 << 0)

struct SCG {
    volatile uint8_t SCG_C1;
    volatile uint8_t SCG_C2;
    volatile uint8_t SCG_C3;
    volatile uint8_t SCG_C4;
};

/* instances */
static struct ICS *ICS = (struct ICS*)ADDR_ICS;
/* static struct SCG *SCG = (struct SCG*)ADDR_SCG; */

static struct {
    clock_freq_t icsir;
    clock_freq_t bus;
    clock_freq_t ext;
} clocks;

#define FREQ_31K25HZ   31250
#define FREQ_39K0625HZ 39062

#define SCTRIM_31K25HZ   0x90
#define SCTRIM_39K0625HZ 0x50

static int setup_sctrim(unsigned freq)
{
    picoRTOS_assert(freq <= (unsigned)FREQ_39K0625HZ, return -EINVAL);
    picoRTOS_assert(freq >= (unsigned)FREQ_31K25HZ, return -EINVAL);

    unsigned sctrim = ((unsigned)SCTRIM_39K0625HZ +
                       (((unsigned)FREQ_39K0625HZ - freq) / 122u));

    ICS->ICS_C3 = (uint8_t)ICS_C3_SCTRIM(sctrim);
    ICS->ICS_C4 &= ~ICS_C4_SCFTRIM;

    clocks.icsir = (clock_freq_t)freq;
    return 0;
}

static int fei(struct clock_settings *settings)
{
#define DIV512(x) ((x) >> 9)

    int res;
    volatile uint8_t ICS_C1 = ICS->ICS_C1;

    /* select internal ref clock */
    ICS_C1 &= ~ICS_C1_CLKS(ICS_C1_CLKS_M);
    ICS_C1 |= ICS_C1_IREFS;

    ICS->ICS_C1 = ICS_C1;
    ICS->ICS_C2 = (uint8_t)0;

    /* TODO: BDIV !!! */

    /* setup frequency */
    if ((res = setup_sctrim((unsigned)DIV512(settings->freq))) < 0)
        return res;

    clocks.bus = (clock_freq_t)settings->freq;
    arch_set_clock_frequency(settings->freq);
    return 0;
}

static int fbi(struct clock_settings *settings)
{
    int res;
    volatile uint8_t ICS_C1 = ICS->ICS_C1;

    /* select internal ref clock */
    ICS_C1 &= ~ICS_C1_CLKS(ICS_C1_CLKS_M);
    ICS_C1 |= ICS_C1_CLKS(1);

    ICS->ICS_C1 = ICS_C1;
    ICS->ICS_C2 = (uint8_t)0;

    /* setup frequency */
    if ((res = setup_sctrim((unsigned)settings->freq)) < 0)
        return res;

    clocks.bus = (clock_freq_t)settings->freq;
    arch_set_clock_frequency(settings->freq);
    return 0;
}

static int oscinit_busywait(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* only high frequency range */
    ICS->ICS_OSCSC |= (ICS_OSCSC_OSCEN | ICS_OSCSC_OSCOS | ICS_OSCSC_RANGE);
    while ((ICS->ICS_OSCSC & ICS_OSCSC_OSCINIT) == 0 &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int fee(struct clock_settings *settings)
{
    /* only high frequency range */
    picoRTOS_assert(settings->xosc >= 4000000ul, return -EINVAL);
    picoRTOS_assert(settings->xosc <= 20000000ul, return -EINVAL);

    int res;
    uint8_t rdiv;
    unsigned long x = (settings->xosc >> 5);

    /* turn xosc on */
    if ((res = oscinit_busywait()) < 0)
        return res;

    for (rdiv = 0; rdiv < (uint8_t)6; rdiv++) {
        if (x <= (unsigned long)FREQ_39K0625HZ &&
            x >= (unsigned long)FREQ_31K25HZ) {
            /* set RDIV */
            volatile uint8_t ICS_C1 = ICS->ICS_C1;
            ICS_C1 &= ~ICS_C1_RDIV(ICS_C1_RDIV_M);
            ICS_C1 |= ICS_C1_RDIV(rdiv);
            /* select ext ref clock */
            ICS_C1 &= ~ICS_C1_CLKS(ICS_C1_CLKS_M);
            ICS_C1 &= ~ICS_C1_IREFS;
            /* force BDIV to 1. FIXME */
            ICS->ICS_C1 = ICS_C1;
            ICS->ICS_C2 = (uint8_t)0;
            /* setup clk */
            clocks.bus = (clock_freq_t)settings->freq;
            arch_set_clock_frequency(settings->freq);
            return 0;
        }

        /* next */
        x >>= 1;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int fbe(struct clock_settings *settings)
{
    volatile uint8_t ICS_C1 = ICS->ICS_C1;

    /* select oscillator */
    ICS_C1 &= ~ICS_C1_CLKS(ICS_C1_CLKS_M);
    ICS_C1 |= ICS_C1_CLKS(2);

    ICS->ICS_C1 = ICS_C1;
    ICS->ICS_C2 = (uint8_t)0;

    /* TODO: BDIV */

    arch_set_clock_frequency(settings->xosc);
    return 0;
}

int clock_mc9s08ptxx_init(struct clock_settings *settings)
{
    picoRTOS_assert(settings->mode < CLOCK_MC9S08PTXX_MODE_COUNT, return -EINVAL);

    /* remember */
    clocks.ext = (clock_freq_t)settings->xosc;

    switch (settings->mode) {
    case CLOCK_MC9S08PTXX_MODE_FEI: return fei(settings);
    case CLOCK_MC9S08PTXX_MODE_FBI: return fbi(settings);
    case CLOCK_MC9S08PTXX_MODE_FEE: return fee(settings);
    case CLOCK_MC9S08PTXX_MODE_FBE: return fbe(settings);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    switch (clkid) {
    case CLOCK_NXP_MC9S08PTXX_BUS: return clocks.bus;
    case CLOCK_NXP_MC9S08PTXX_LPO: return (clock_freq_t)1000;
    case CLOCK_NXP_MC9S08PTXX_32K: return (clock_freq_t)clocks.icsir;
    case CLOCK_NXP_MC9S08PTXX_EXT: return clocks.ext;
    default: break;
    }

    /* unknown */
    picoRTOS_break();
    /*@notreached@*/ return (clock_freq_t)-EIO;
}
