#include "clock-sam3x_pmc.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct CLOCK_SAM3X_PMC {
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
    volatile uint32_t PMC_PCK0;
    volatile uint32_t PMC_PCK1;
    volatile uint32_t PMC_PCK2;
    uint32_t RESERVED4[5];
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

#define CKGR_MOR_MOSCSEL     (1 << 24)
#define CKGR_MOR_KEY_M       0xffu
#define CKGR_MOR_KEY(x)      (((x) & CKGR_MOR_KEY_M) << 16)
#define CKGR_MOR_MOSCXTST_M  0xffu
#define CKGR_MOR_MOSCXTST(x) (((x) & CKGR_MOR_MOSCXTST_M) << 8)
#define CKGR_MOR_MOSCREN     (1 << 3)
#define CKGR_MOR_MOSCTXEN    (1 << 0)

#define CKGR_PLLAR_ONE          (1 << 29)
#define CKGR_PLLAR_MULA_M       0x7ffu
#define CKGR_PLLAR_MULA(x)      (((x) & CKGR_PLLAR_MULA_M) << 16)
#define CKGR_PLLAR_PLLACOUNT_M  0x3fu
#define CKGR_PLLAR_PLLACOUNT(x) (((x) & CKGR_PLLAR_PLLACOUNT_M) << 8)
#define CKGR_PLLAR_DIVA_M       0xffu
#define CKGR_PLLAR_DIVA(x)      ((x) & CKGR_PLLAR_DIVA_M)

#define PMC_MCKR_PRES_M  0x7u
#define PMC_MCKR_PRES(x) (((x) & PMC_MCKR_PRES_M) << 4)
#define PMC_MCKR_CSS_M   0x3u
#define PMC_MCKR_CSS(x)  ((x) & PMC_MCKR_CSS_M)

#define PMC_SR_MOSCSELS (1 << 16)
#define PMC_SR_OSCSELS  (1 << 7)
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
static struct CLOCK_SAM3X_PMC *PMC = (struct CLOCK_SAM3X_PMC*)ADDR_PMC;

static struct {
    /* internals */
    clock_freq_t moscx;
    /* clock_freq_t mainck; */
    clock_freq_t pllack;
    /* clock_freq_t upllck; */
    /* exported */
    clock_freq_t slck;
    clock_freq_t mck;
    /* periph */
    clock_freq_t periph[DEVICE_PID_COUNT];
} clocks;

static void clock_sam3x_pmc_init_flash(void)
{
    *(volatile uint32_t*)ADDR_EEFC0 = (uint32_t)(1 << 10);
    *(volatile uint32_t*)ADDR_EEFC1 = (uint32_t)(1 << 10);
}

static int clock_sam3x_pmc_init_moscx(unsigned long hz)
{
#define CKGR_MOR_MOSCXTST_DEFAULT 8

    picoRTOS_assert(hz > 0, return -EINVAL);

    int deadlock = CONFIG_DEADLOCK_COUNT;
    uint32_t mor = (uint32_t)CKGR_MOR_KEY(0x37);

    mor |= CKGR_MOR_MOSCTXEN;                               /* enable main crystal */
    mor |= CKGR_MOR_MOSCSEL;                                /* select */
    mor |= CKGR_MOR_MOSCXTST(CKGR_MOR_MOSCXTST_DEFAULT);    /* startup time */

    /* write to register */
    PMC->CKGR_MOR = mor;
    while ((PMC->PMC_SR & PMC_SR_MOSCXTS) != 0 &&
           (PMC->PMC_SR & PMC_SR_MOSCSELS) == 0 &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    clocks.moscx = (clock_freq_t)hz;
    return 0;
}

static void clock_sam3x_pmc_init_slck(void)
{
    if ((PMC->PMC_SR & PMC_SR_OSCSELS) == 0)
        clocks.slck = (clock_freq_t)32000;
    else
        clocks.slck = (clock_freq_t)32768;
}

static int plla_setup(unsigned long mula)
{
#define CKGR_PLLAR_PLLACOUNT_DEFAULT 0x3f
#define CKGR_PLLAR_MULA_COUNT        2048ul

    picoRTOS_assert(mula < CKGR_PLLAR_MULA_COUNT, return -EINVAL);

    int deadlock = CONFIG_DEADLOCK_COUNT;
    uint32_t pllar = (uint32_t)CKGR_PLLAR_ONE;

    /* reset plla */
    PMC->CKGR_PLLAR = (uint32_t)CKGR_PLLAR_ONE;

    pllar |= CKGR_PLLAR_PLLACOUNT(CKGR_PLLAR_PLLACOUNT_DEFAULT);
    pllar |= CKGR_PLLAR_MULA(mula - 1ul);
    pllar |= CKGR_PLLAR_DIVA(1);

    /* set pll config */
    PMC->CKGR_PLLAR = pllar;

    /* wait for lock bit */
    while ((PMC->PMC_SR & PMC_SR_LOCKA) == 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int clock_sam3x_pmc_init_plla(unsigned long hz)
{
    picoRTOS_assert((clock_freq_t)(hz / 2ul) >= clocks.moscx,
                    return -EINVAL);

    unsigned long mula = (hz / (unsigned long)clocks.moscx);

    clocks.pllack = (clock_freq_t)hz;
    return plla_setup(mula);
}

static int clock_sam3x_pmc_init_mck(unsigned long mck_div)
{
#define MAIN_CLK 1
#define PLLA_CLK 2

    int pres;
    int deadlock = CONFIG_DEADLOCK_COUNT;

    switch (mck_div) {
    case 1ul: pres = 0; break;
    case 2ul: pres = 1; break;
    case 3ul: pres = 7; break;
    case 4ul: pres = 2; break;
    case 8ul: pres = 3; break;
    case 16ul: pres = 4; break;
    case 32ul: pres = 5; break;
    case 64ul: pres = 6; break;
    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EINVAL;
    }

    /* set prescaler */
    PMC->PMC_MCKR = (uint32_t)((PMC->PMC_MCKR & ~PMC_MCKR_PRES(PMC_MCKR_PRES_M)) |
                               PMC_MCKR_PRES(pres));

    while ((PMC->PMC_SR & PMC_SR_MCKRDY) == 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    /* switch to pll */
    PMC->PMC_MCKR = (uint32_t)(PMC_MCKR_PRES(pres) | PMC_MCKR_CSS(PLLA_CLK));
    while ((PMC->PMC_SR & PMC_SR_MCKRDY) == 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    clocks.mck = clocks.pllack / (clock_freq_t)mck_div;
    arch_set_clock_frequency((unsigned long)clocks.mck);

    return 0;
}

/* Function: clock_sam3x_pmc_init
 * Initialises the SAM3X clock system
 *
 * Parameters:
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_sam3x_pmc_init(struct clock_settings *settings)
{
    picoRTOS_assert(settings->moscx > 0, return -EINVAL);

    int res;

    /* ensure write protect is disabled */
    PMC->PMC_WPMR = (uint32_t)PMC_WPMR_WPKEY(0x504d43);

    /* flash */
    clock_sam3x_pmc_init_flash();
    /* slow clock */
    clock_sam3x_pmc_init_slck();

    /* main crystal oscillator */
    if ((res = clock_sam3x_pmc_init_moscx(settings->moscx)) < 0)
        return res;

    /* plla */
    if ((res = clock_sam3x_pmc_init_plla(settings->plla)) < 0)
        return res;

    /* master clock */
    if ((res = clock_sam3x_pmc_init_mck(settings->mck_div)) < 0)
        return res;

    return 0;
}

/* Function: clock_sam3x_pmc_enable
 * Enables a particular clock
 *
 * Parameters:
 *  clkid - The clock identifier
 *  periph_div - The clock divider to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_sam3x_pmc_enable(clock_id_t clkid, clock_sam3x_pmc_periph_div_t periph_div)
{
    picoRTOS_assert(periph_div < CLOCK_SAM3X_PMC_PERIPH_DIV_COUNT,
                    return -EINVAL);

    if (clkid > (clock_id_t)CLOCK_SAM3X_PMC_PERIPH_BASE) {
        size_t pid = (size_t)clkid - (size_t)CLOCK_SAM3X_PMC_PERIPH_BASE;

        picoRTOS_assert(pid < (size_t)DEVICE_PID_COUNT, return -EINVAL);

        PMC->PMC_PCR = (uint32_t)(PMC_PCR_EN | PMC_PCR_CMD |
                                  PCM_PCR_DIV(periph_div) |
                                  PMC_PCR_PID(pid));

        clocks.periph[pid] = (clock_freq_t)((int)clocks.mck / (1 << periph_div));
        return 0;
    }

    return 0;
}

/* Function: clock_sam3x_pmc_disable
 * Disable a particular clock
 *
 * Parameters:
 *  clkid - The clock id
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_sam3x_pmc_disable(clock_id_t clkid)
{
    if (clkid > (clock_id_t)CLOCK_SAM3X_PMC_PERIPH_BASE) {
        size_t pid = (size_t)clkid - (size_t)CLOCK_SAM3X_PMC_PERIPH_BASE;

        picoRTOS_assert(pid < (size_t)DEVICE_PID_COUNT, return -EINVAL);

        PMC->PMC_PCR = (uint32_t)(PMC_PCR_CMD | PMC_PCR_PID(pid));
        return 0;
    }

    return 0;
}

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    if (clkid > (clock_id_t)CLOCK_SAM3X_PMC_PERIPH_BASE) {
        size_t pid = (size_t)clkid - (size_t)CLOCK_SAM3X_PMC_PERIPH_BASE;

        picoRTOS_assert(pid < (size_t)DEVICE_PID_COUNT,
                        return (clock_freq_t)-EINVAL);

        return clocks.periph[pid];
    }

    if (clkid > (clock_id_t)CLOCK_SAM3X_PMC_PCK_BASE)
        return (clock_freq_t)-EINVAL;

    switch (clkid) {
    case CLOCK_SAM3X_PMC_SCLK: return clocks.slck;
    case CLOCK_SAM3X_PMC_HCLK:      /*@fallthrough@*/
    case CLOCK_SAM3X_PMC_FCLK:      /*@fallthrough@*/
    case CLOCK_SAM3X_PMC_MCK: return clocks.mck;
    case CLOCK_SAM3X_PMC_SYSTICK: return clocks.mck / (clock_freq_t)8;
    case CLOCK_SAM3X_USB_HS:        /*@fallthrough@*/
    case CLOCK_SAM3X_USB_FS:        /*@fallthrough@*/
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return (clock_freq_t)-EIO;
}
