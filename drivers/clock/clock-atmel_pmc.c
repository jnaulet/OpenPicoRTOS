#include "clock-atmel_pmc.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct CLOCK_ATMEL_PMC {
    volatile uint32_t PMC_SCER;
    volatile uint32_t PMC_SCDR;
    volatile uint32_t PMC_SCSR;
    uint32_t RESERVED0;
    volatile uint32_t PMC_PCER0;
    volatile uint32_t PMC_PCDR0;
    volatile uint32_t PMC_PCSR0;
    volatile uint32_t CKGR_UCKR;
    volatile uint32_t CKGR_MOR;
    volatile uint32_t CKGR_MCFR;
    volatile uint32_t CKGR_PLLAR;
    uint32_t RESERVED1;
    volatile uint32_t PMC_MCKR;
    uint32_t RESERVED2;
    volatile uint32_t PMC_USB;
    uint32_t RESERVED3;
    volatile uint32_t PMC_PCK[CLOCK_ATMEL_PMC_PCK_COUNT];
    volatile uint32_t PMC_IER;
    volatile uint32_t PMC_IDR;
    volatile uint32_t PMC_SR;
    volatile uint32_t PMC_IMR;
    volatile uint32_t PMC_FSMR;
    volatile uint32_t PMC_FSPR;
    volatile uint32_t PMC_FOCR;
    uint32_t RESERVED5[26];
    volatile uint32_t PMC_WPMR;
    volatile uint32_t PMC_WPSR;
    uint32_t RESERVED6[5];
    volatile uint32_t PMC_PCER1;
    volatile uint32_t PMC_PCDR1;
    volatile uint32_t PMC_PCSR1;
    volatile uint32_t PMC_PCR;
};

#define PMC_SCER_PCK7   (1u << 15)
#define PMC_SCER_PCK6   (1u << 14)
#define PMC_SCER_PCK5   (1u << 13)
#define PMC_SCER_PCK4   (1u << 12)
#define PMC_SCER_PCK3   (1u << 11)
#define PMC_SCER_PCK2   (1u << 10)
#define PMC_SCER_PCK1   (1u << 9)
#define PMC_SCER_PCK0   (1u << 8)
#define PMC_SCER_USBCLK (1u << 5)

#define CKGR_UCKR_UPLLCOUNT_M  0xfu
#define CKGR_UCKR_UPLLCOUNT(x) (((x) & CKGR_UCKR_UPLLCOUNT_M) << 20)
#define CKGR_UCKR_UPLLEN       (1 << 16)

#define CKGR_MOR_XT32KFME    (1 << 26)
#define CKGR_MOR_CFDEN       (1 << 25)
#define CKGR_MOR_MOSCSEL     (1 << 24)
#define CKGR_MOR_KEY_M       0xffu
#define CKGR_MOR_KEY(x)      (((x) & CKGR_MOR_KEY_M) << 16)
#define CKGR_MOR_MOSCXTST_M  0xffu
#define CKGR_MOR_MOSCXTST(x) (((x) & CKGR_MOR_MOSCXTST_M) << 8)
#define CKGR_MOR_MOSCRCF_M   0x7u
#define CKGR_MOR_MOSCRCF(x)  (((x) & CKGR_MOR_MOSCRCF_M) << 4)
#define CKGR_MOR_MOSCREN     (1 << 3)
#define CKGR_MOR_WAITMODE    (1 << 2)
#define CKGR_MOR_MOSCXTBY    (1 << 1)
#define CKGR_MOR_MOSCXTEN    (1 << 0)

#define CKGR_PLLAR_ONE          (1 << 29)
#define CKGR_PLLAR_MULA_M       0x7ffu
#define CKGR_PLLAR_MULA(x)      (((x) & CKGR_PLLAR_MULA_M) << 16)
#define CKGR_PLLAR_PLLACOUNT_M  0x3fu
#define CKGR_PLLAR_PLLACOUNT(x) (((x) & CKGR_PLLAR_PLLACOUNT_M) << 8)
#define CKGR_PLLAR_DIVA_M       0xffu
#define CKGR_PLLAR_DIVA(x)      ((x) & CKGR_PLLAR_DIVA_M)

#define PMC_MCKR_UPLLDIV2 (1 << 13)
#define PMC_MCKR_MDIV_M   0x3u
#define PMC_MCKR_MDIV(x)  (((x) & PMC_MCKR_MDIV_M) << 8)
#define PMC_MCKR_PRES_M   0x7u
#define PMC_MCKR_PRES(x)  (((x) & PMC_MCKR_PRES_M) << 4)
#define PMC_MCKR_CSS_M    0x3u
#define PMC_MCKR_CSS(x)   ((x) & PMC_MCKR_CSS_M)

#define PMC_PCKx_PRES_M  0xffu
#define PMC_PCKx_PRES(x) (((x) & PMC_PCKx_PRES_M) << 4)
#define PMC_PCKx_CSS_M   0x7u
#define PMC_PCKx_CSS(x)  ((x) & PMC_PCKx_CSS_M)

#define PMC_SR_MOSCSELS (1 << 16)
#define PMC_SR_OSCSELS  (1 << 7)
#define PMC_SR_LOCKU    (1 << 6)
#define PMC_SR_MCKRDY   (1 << 3)
#define PMC_SR_LOCKA    (1 << 1)
#define PMC_SR_MOSCXTS  (1 << 0)

#define PMC_WPMR_WPKEY_M  0xffffffu
#define PMC_WPMR_WPKEY(x) (((x) & PMC_WPMR_WPKEY_M) << 8)
#define PMC_WPMR_WPEN     (1 << 0)

#define PMC_PCR_EN     (1 << 28)
#define PCM_PCR_DIV_M  0x3u
#define PCM_PCR_DIV(x) (((x) & PCM_PCR_DIV_M) << 16)
#define PMC_PCR_CMD    (1 << 12)
#define PMC_PCR_PID_M  0x3fu
#define PMC_PCR_PID(x) ((x) & PMC_PCR_PID_M)

/* only one instance */
static struct CLOCK_ATMEL_PMC *PMC = (struct CLOCK_ATMEL_PMC*)ADDR_PMC;

static struct {
    /* internals */
    clock_freq_t mainck;
    clock_freq_t pllack;
    clock_freq_t upllckdiv;
    /* exported */
    clock_freq_t slck;
    clock_freq_t hclk;
    clock_freq_t mck;
    /* pck */
    clock_freq_t pck[CLOCK_ATMEL_PMC_PCK_COUNT];
} clocks;

#define UPLLCK_HZ 480000000ul

static int slck_probe(void)
{
    if ((PMC->PMC_SR & PMC_SR_OSCSELS) == 0)
        clocks.slck = (clock_freq_t)32000;
    else
        clocks.slck = (clock_freq_t)32768;

    return 0;
}

static int mainck_setup(const struct clock_settings *settings)
{
#define MOSCXTST_DEFAULT 64 /* FIXME: arbitrary */
    picoRTOS_assert(settings->mainck < CLOCK_ATMEL_PMC_MAINCK_COUNT, return -EINVAL);

    int deadlock = CONFIG_DEADLOCK_COUNT;
    uint32_t mor = (uint32_t)(CKGR_MOR_KEY(0x37) | CKGR_MOR_MOSCREN);

    if (settings->mainck == CLOCK_ATMEL_PMC_MAINCK_RC) {
        /* main rc oscillator, set freq to mosc */
        switch (settings->mosc) {
        case 4000000ul: mor |= CKGR_MOR_MOSCRCF(0); break;
        case 8000000ul: mor |= CKGR_MOR_MOSCRCF(1); break;
        case 12000000ul: mor |= CKGR_MOR_MOSCRCF(2); break;
        default: picoRTOS_assert(false, return -EINVAL); break;
        }
    }else{
        /* main crystal oscillator */
        picoRTOS_assert(settings->mosc > 0, return -EINVAL);

        mor |= CKGR_MOR_MOSCXTEN;                       /* enable main crystal */
        mor |= CKGR_MOR_MOSCXTST(MOSCXTST_DEFAULT);     /* startup time */
        /* write to register */
        PMC->CKGR_MOR = mor;
        while ((PMC->PMC_SR & PMC_SR_MOSCXTS) == 0 &&
               deadlock-- != 0) arch_delay_us(10ul);

        mor |= CKGR_MOR_MOSCSEL;    /* select */
        mor &= ~CKGR_MOR_MOSCREN;   /* de-activate rc */
    }

    /* write to register */
    PMC->CKGR_MOR = mor;
    while ((PMC->PMC_SR & PMC_SR_MOSCSELS) == 0 &&
           deadlock-- != 0) arch_delay_us(10ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    clocks.mainck = (clock_freq_t)settings->mosc;
    return 0;
#undef MOSCXTST_DEFAULT
}

static int plla_setup(const struct clock_settings *settings)
{
#define PLLACOUNT_DEFAULT 0x3f
    picoRTOS_assert((clock_freq_t)(settings->plla / 2ul) >= clocks.mainck,
                    return -EINVAL);

    unsigned long diva = 0;
    unsigned long mula = 0;

    for (diva = 1ul; diva <= (unsigned long)CKGR_PLLAR_DIVA_M; diva++) {
        /* try to find the best match */
        mula = settings->plla * diva / (unsigned long)clocks.mainck;
        if (((mula * (unsigned long)clocks.mainck) / diva) == settings->plla) break;
    }

    picoRTOS_assert(diva < (unsigned long)(CKGR_PLLAR_DIVA_M + 1u),
                    return -EINVAL);
    picoRTOS_assert(mula < (unsigned long)(CKGR_PLLAR_MULA_M + 1u),
                    return -EINVAL);

    int deadlock = CONFIG_DEADLOCK_COUNT;
    uint32_t pllar = (uint32_t)CKGR_PLLAR_ONE;

    pllar |= CKGR_PLLAR_PLLACOUNT(PLLACOUNT_DEFAULT);
    pllar |= CKGR_PLLAR_MULA(mula - 1ul);
    pllar |= CKGR_PLLAR_DIVA(diva);

    /* set pll config */
    PMC->CKGR_PLLAR = pllar;

    /* wait for lock bit */
    while ((PMC->PMC_SR & PMC_SR_LOCKA) == 0 && deadlock-- != 0)
        arch_delay_us(10ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    clocks.pllack = (clock_freq_t)settings->plla;
    return 0;
#undef PLLACOUNT_DEFAULT
#undef MULA_COUNT
}

static int upll_setup(const struct clock_settings *settings)
{
    picoRTOS_assert(settings->upll < CLOCK_ATMEL_PMC_UPLL_COUNT,
                    return -EINVAL);

    int deadlock = CONFIG_DEADLOCK_COUNT;

    if (settings->upll == CLOCK_ATMEL_PMC_UPLL_DISABLE) {
        PMC->CKGR_UCKR &= ~CKGR_UCKR_UPLLEN;
        return 0;
    }

    /* uplldiv2 */
    if (settings->upll == CLOCK_ATMEL_PMC_UPLL_DIV2) {
        PMC->PMC_MCKR |= PMC_MCKR_UPLLDIV2;
        clocks.upllckdiv = (clock_freq_t)(UPLLCK_HZ / 2ul);
    }else{
        PMC->PMC_MCKR &= ~PMC_MCKR_UPLLDIV2;
        clocks.upllckdiv = (clock_freq_t)UPLLCK_HZ;
    }

    /* Enable PLL */
    PMC->CKGR_UCKR |= CKGR_UCKR_UPLLEN;

    /* wait for lock bit */
    while ((PMC->PMC_SR & PMC_SR_LOCKU) == 0 &&
           deadlock-- != 0) arch_delay_us(10ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int mckr_pres(unsigned long prescaler)
{
    switch (prescaler) {
    case 1ul: return 0;
    case 2ul: return 1;
    case 3ul: return 7;
    case 4ul: return 2;
    case 8ul: return 3;
    case 16ul: return 4;
    case 32ul: return 5;
    case 64ul: return 6;
    default: break;
    }

    picoRTOS_assert_void(false);
    return -EINVAL;
}

static int mckr_mdiv(unsigned long mdiv)
{
    switch (mdiv) {
    case 1: return 0;
    case 2: return 1;
    case 3: return 3;
    case 4: return 2;
    default: break;
    }

    picoRTOS_assert_void(false);
    return -EINVAL;
}

static int hclk_mck_setup(const struct clock_settings *settings)
{
    picoRTOS_assert(settings->mckr_css < CLOCK_ATMEL_PMC_MCKR_CSS_COUNT,
                    return -EINVAL);

    int pres;
    int mdiv;
    unsigned long freq = 0;
    uint32_t pmc_mckr = PMC->PMC_MCKR;
    int deadlock = CONFIG_DEADLOCK_COUNT;

    if ((pres = mckr_pres(settings->mckr_prescaler)) < 0) return pres;
    if ((mdiv = mckr_mdiv(settings->mckr_mdiv)) < 0) return mdiv;

    /* a. Program PMC_MCKR.PRES */
    pmc_mckr &= ~PMC_MCKR_PRES(PMC_MCKR_PRES_M);
    pmc_mckr |= PMC_MCKR_PRES(pres);
    PMC->PMC_MCKR = pmc_mckr;
    /* b. Wait for PMC_SR.MCKRDY to be set */
    while ((PMC->PMC_SR & PMC_SR_MCKRDY) == 0 && deadlock-- != 0)
        arch_delay_us(10ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    /* c. Program PMC_MCKR.MDIV */
    pmc_mckr &= ~PMC_MCKR_MDIV(PMC_MCKR_MDIV_M);
    pmc_mckr |= PMC_MCKR_MDIV(mdiv);
    PMC->PMC_MCKR = pmc_mckr;
    /* d. Wait for PMC_SR.MCKRDY to be set */
    while ((PMC->PMC_SR & PMC_SR_MCKRDY) == 0 && deadlock-- != 0)
        arch_delay_us(10ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    /* e. Program PMC_MCKR.CSS */
    pmc_mckr &= ~PMC_MCKR_CSS(PMC_MCKR_CSS_M);
    pmc_mckr |= PMC_MCKR_CSS(settings->mckr_css);
    PMC->PMC_MCKR = pmc_mckr;
    /* f. Wait for PMC_SR.MCKRDY to be set */
    while ((PMC->PMC_SR & PMC_SR_MCKRDY) == 0 && deadlock-- != 0)
        arch_delay_us(10ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    switch (settings->mckr_css) {
    case CLOCK_ATMEL_PMC_MCKR_CSS_SLCK: freq = (unsigned long)clocks.slck; break;
    case CLOCK_ATMEL_PMC_MCKR_CSS_MAINCK: freq = (unsigned long)clocks.mainck; break;
    case CLOCK_ATMEL_PMC_MCKR_CSS_PLLACK: freq = (unsigned long)clocks.pllack; break;
    case CLOCK_ATMEL_PMC_MCKR_CSS_UPLLCKDIV: freq = (unsigned long)clocks.upllckdiv; break;
    default: picoRTOS_assert(false, return -EIO);
    }

    clocks.mck = (clock_freq_t)(freq / (settings->mckr_prescaler * settings->mckr_mdiv));
    clocks.hclk = (clock_freq_t)(freq / settings->mckr_prescaler);
    arch_set_clock_frequency((unsigned long)clocks.hclk);

    return 0;
}

/* Function: clock_atmel_pmc_init
 * Initialises the ATMEL clock system
 *
 * Parameters:
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_atmel_pmc_init(const struct clock_settings *settings)
{
    int res;

    /* ensure write protect is disabled */
    PMC->PMC_WPMR = (uint32_t)PMC_WPMR_WPKEY(0x504d43);

    (void)slck_probe();                                     /* slck */
    if ((res = mainck_setup(settings)) < 0) return res;     /* mainck */
    if (settings->plla != 0 &&
        (res = plla_setup(settings)) < 0) return res;       /* plla */
    if ((res = upll_setup(settings)) < 0) return res;       /* upll */
    if ((res = hclk_mck_setup(settings)) < 0) return res;   /* hclk, mck */

    return 0;
}

/* Function: clock_atmel_pmc_periph_clk_enable
 * Enables a particular peripheral clock
 *
 * Parameters:
 *  pid - The peripheral id
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_atmel_pmc_periph_clk_enable(unsigned pid)
{
    picoRTOS_assert(pid < (unsigned)DEVICE_PID_COUNT, return -EINVAL);

    PMC->PMC_PCR = (uint32_t)(PMC_PCR_EN | PMC_PCR_CMD | PMC_PCR_PID(pid));
    return 0;
}

/* Function: clock_atmel_pmc_periph_clk_disable
 * Disable a particular peripheral clock
 *
 * Parameters:
 *  pid - The peripheral id
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_atmel_pmc_periph_clk_disable(unsigned pid)
{
    picoRTOS_assert(pid < (unsigned)DEVICE_PID_COUNT, return -EINVAL);

    PMC->PMC_PCR = (uint32_t)(PMC_PCR_CMD | PMC_PCR_PID(pid));
    return 0;
}

/* Function: clock_atmel_pmc_pck_enable
 * Enables a particular programmable clock
 *
 * Parameters:
 *  pck - The PCK index
 *  prescaler - The PCK prescaler (1-256)
 *  css - The PCK source clock
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_atmel_pmc_pck_enable(size_t pck, unsigned prescaler, clock_atmel_pmc_pckx_css_t css)
{
    picoRTOS_assert(pck < (size_t)CLOCK_ATMEL_PMC_PCK_COUNT, return -EINVAL);
    picoRTOS_assert(prescaler > 0, return -EINVAL);
    picoRTOS_assert(prescaler <= (PMC_PCKx_PRES_M + 1u), return -EINVAL);
    picoRTOS_assert(css < CLOCK_ATMEL_PMC_PCKR_CSS_COUNT, return -EINVAL);

    clock_freq_t freq = 0;

    switch (css) {
    case CLOCK_ATMEL_PMC_PCKR_CSS_SLCK: freq = clocks.slck; break;
    case CLOCK_ATMEL_PMC_PCKR_CSS_MAINCK: freq = clocks.mainck; break;
    case CLOCK_ATMEL_PMC_PCKR_CSS_PLLACK: freq = clocks.pllack; break;
    case CLOCK_ATMEL_PMC_PCKR_CSS_UPLLCKDIV: freq = clocks.upllckdiv; break;
    case CLOCK_ATMEL_PMC_PCKR_CSS_MCK: freq = clocks.mck; break;
    case CLOCK_ATMEL_PMC_PCKR_CSS_AUDIOCK: /*@fallthrough@*/
    default: picoRTOS_assert(false, return -EINVAL);
    }

    PMC->PMC_PCK[pck] = (uint32_t)(PMC_PCKx_PRES(prescaler - 1) | PMC_PCKx_CSS(css));
    PMC->PMC_SCER |= (PMC_SCER_PCK0 << pck);

    clocks.pck[pck] = freq / (clock_freq_t)prescaler;
    return 0;
}

/* Function: clock_atmel_pmc_pck_disable
 * Disables a particular programmable clock
 *
 * Parameters:
 *  pck - The PCK index
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_atmel_pmc_pck_disable(size_t pck)
{
    picoRTOS_assert(pck < (size_t)CLOCK_ATMEL_PMC_PCK_COUNT, return -EINVAL);
    PMC->PMC_SCER &= ~(PMC_SCER_PCK0 << pck);
    return 0;
}

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    switch (clkid) {
    case CLOCK_ATMEL_PMC_SCLK: return clocks.slck;
    case CLOCK_ATMEL_PMC_HCLK: return clocks.hclk;
    case CLOCK_ATMEL_PMC_SYSTICK: return clocks.hclk / (clock_freq_t)2;
    case CLOCK_ATMEL_USB_HS:    /*@fallthrough@*/
    case CLOCK_ATMEL_USB_FS: break;
    case CLOCK_ATMEL_PMC_FCLK:  /*@fallthrough@*/
    case CLOCK_ATMEL_PMC_MCK: return clocks.mck;
    default:
        if (clkid >= (clock_id_t)CLOCK_ATMEL_PMC_PCK_OFFSET &&
            clkid < (clock_id_t)(CLOCK_ATMEL_PMC_PCK_OFFSET + CLOCK_ATMEL_PMC_PCK_COUNT))
            return clocks.pck[(int)clkid - CLOCK_ATMEL_PMC_PCK_OFFSET];
        /* error */
        break;
    }

    picoRTOS_assert_void(false);
    return (clock_freq_t)-EINVAL;
}
