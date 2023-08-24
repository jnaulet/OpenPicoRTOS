#include "clock-gd32vf103.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#define IRC8M_FREQ  8000000
#define PLLMAX_FREQ 108000000
#define IRC40K_FREQ 40000

/* Bus max values */
#define AHB_MAX  108000000
#define APB1_MAX 54000000
#define APB2_MAX AHB_MAX
#define ADC_MAX  14000000

struct RCU {
    volatile uint32_t RCU_CTL;
    volatile uint32_t RCU_CFG0;
    volatile uint32_t RCU_INT;
    volatile uint32_t RCU_APB2RST;
    volatile uint32_t RCU_APB1RST;
    volatile uint32_t RCU_AHBEN;
    volatile uint32_t RCU_APB2EN;
    volatile uint32_t RCU_APB1EN;
    volatile uint32_t RCU_BDCTL;
    volatile uint32_t RCU_RSTSCK;
    volatile uint32_t RCU_AHBRST;
    volatile uint32_t RCU_CFG1;
    uint32_t RESERVED0;
    volatile uint32_t RCU_DSV;
};

#define RCU_CTL_PLL2STB       (1 << 29)
#define RCU_CTL_PLL2EN        (1 << 28)
#define RCU_CTL_PLL1STB       (1 << 27)
#define RCU_CTL_PLL1EN        (1 << 26)
#define RCU_CTL_PLLSTB        (1 << 25)
#define RCU_CTL_PLLEN         (1 << 24)
#define RCU_CTL_CKMEN         (1 << 29)
#define RCU_CTL_HXTALBPS      (1 << 18)
#define RCU_CTL_HXTALSTB      (1 << 17)
#define RCU_CTL_HXTALEN       (1 << 16)
#define RCU_CTL_IRC8MCALIB_M  0xffu
#define RCU_CTL_IRC8MCALIB(x) (((x) & RCU_CTL_IRC8MCALIB_M) << 8)
#define RCU_CTL_IRC8MADJ_M    0x1fu
#define RCU_CTL_IRC8MADJ(x)   (((x) & RCU_CTL_IRC8MADJ_M) << 3)
#define RCU_CTL_IRC8MSTB      (1 << 1)
#define RCU_CTL_IRC8MEN       (1 << 0)

#define RCU_CFG0_PLLMF4_M      0x1u
#define RCU_CFG0_PLLMF4(x)     (((x) & RCU_CFG0_PLLMF4_M) << 29)
#define RCU_CFG0_ADCPSC2       (1 << 28)
#define RCU_CFG0_CKOUT0SEL_M   0xfu
#define RCU_CFG0_CKOUT0SEL(x)  (((x) & RCU_CFG0_CKOUT0SEL_M) << 24)
#define RCU_CFG0_USBFSPSC_M    0x3u
#define RCU_CFG0_USBFSPSC(x)   (((x) & RCU_CFG0_USBFSPSC_M) << 22)
#define RCU_CFG0_PLLMF_M       0xfu
#define RCU_CFG0_PLLMF(x)      (((x) & RCU_CFG0_PLLMF_M) << 18)
#define RCU_CFG0_PREDV0_LSB_M  0x1u
#define RCU_CFG0_PREDV0_LSB(x) (((x) & RCU_CFG0_PREDV0_LSB_M) << 17)
#define RCU_CFG0_PLLSEL        (1 << 16)
#define RCU_CFG0_ADCPSC_M      0x3u
#define RCU_CFG0_ADCPSC(x)     (((x) & RCU_CFG0_ADCPSC_M) << 14)
#define RCU_CFG0_APB2PSC_M     0x7u
#define RCU_CFG0_APB2PSC(x)    (((x) & RCU_CFG0_APB2PSC_M) << 11)
#define RCU_CFG0_APB1PSC_M     0x7u
#define RCU_CFG0_APB1PSC(x)    (((x) & RCU_CFG0_APB1PSC_M) << 8)
#define RCU_CFG0_AHBPSC_M      0xfu
#define RCU_CFG0_AHBPSC(x)     (((x) & RCU_CFG0_AHBPSC_M) << 4)
#define RCU_CFG0_SCSS_M        0x3u
#define RCU_CFG0_SCSS(x)       (((x) & RCU_CFG0_SCSS_M) << 2)
#define RCU_CFG0_SCS_M         0x3u
#define RCU_CFG0_SCS(x)        ((x) & RCU_CFG0_SCS_M)

#define RCU_CFG1_PREDV0_M  0xfu
#define RCU_CFG1_PREDV0(x) ((x) & RCU_CFG1_PREDV0_M)

/* static clock system */
static struct RCU *RCU = (struct RCU*)ADDR_RCU;

static struct {
    clock_freq_t hxtal;
    clock_freq_t ck_sys;
    clock_freq_t ahb;
    clock_freq_t apb1;
    clock_freq_t apb2;
    clock_freq_t adc;
} clocks;

static int switch_system_clock(clock_gd32vf103_ck_sys_t ck_sys)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;
    uint32_t rcu_cfg0 = RCU->RCU_CFG0;

    rcu_cfg0 &= ~RCU_CFG0_SCS(RCU_CFG0_SCS_M);
    rcu_cfg0 |= RCU_CFG0_SCS(ck_sys);
    RCU->RCU_CFG0 = rcu_cfg0;

    while (deadlock-- != 0)
        if ((RCU->RCU_CFG0 & RCU_CFG0_SCSS(RCU_CFG0_SCSS_M)) ==
            RCU_CFG0_SCSS(ck_sys))
            break;

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int setup_hxtal_busywait(unsigned long freq)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* start xtal */
    RCU->RCU_CTL |= RCU_CTL_HXTALEN;

    while (deadlock-- != 0)
        if ((RCU->RCU_CTL & RCU_CTL_HXTALSTB) != 0)
            break;

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    clocks.hxtal = (clock_freq_t)freq;
    return 0;
}

static int setup_pll(clock_gd32vf103_pllsel_t pllsel,
                     unsigned long freq)
{
    picoRTOS_assert(pllsel < CLOCK_GD32VF103_PLLSEL_COUNT, return -EINVAL);
    picoRTOS_assert(freq > 0, return -EINVAL);
    picoRTOS_assert(freq <= (unsigned long)PLLMAX_FREQ, return -EINVAL);

    unsigned long input;
    unsigned long pllmf;
    unsigned long predv0;

    uint32_t rcu_cfg0 = RCU->RCU_CFG0;
    uint32_t rcu_cfg1 = RCU->RCU_CFG1;

    /* pllsel */
    if (pllsel == CLOCK_GD32VF103_PLLSEL_IRC8M) {
        input = (unsigned long)IRC8M_FREQ;
        rcu_cfg0 &= ~RCU_CFG0_PLLSEL;
    }else{
        input = (unsigned long)clocks.hxtal;
        rcu_cfg0 |= RCU_CFG0_PLLSEL;
    }

    /* brute-force */
    for (pllmf = 2ul; pllmf <= 32ul; pllmf++)
        for (predv0 = 1ul; predv0 <= 16ul; predv0++) {
            unsigned long f = input / predv0 * pllmf;
            if (f == freq) {
                /* found freq, adjust parameters */
                rcu_cfg1 &= ~RCU_CFG1_PREDV0(RCU_CFG1_PREDV0_M);
                rcu_cfg0 &= ~(RCU_CFG0_PLLMF4(RCU_CFG0_PLLMF4_M) |
                              RCU_CFG0_PLLMF(RCU_CFG0_PLLMF_M) |
                              RCU_CFG0_PREDV0_LSB(RCU_CFG0_PREDV0_LSB_M));
                /* set values */
                rcu_cfg1 |= RCU_CFG1_PREDV0(predv0 - 1ul);
                rcu_cfg0 |= (RCU_CFG0_PLLMF4((pllmf - 1ul) >> 4) |
                             RCU_CFG0_PLLMF(pllmf - 1ul));

                RCU->RCU_CFG0 = rcu_cfg0;
                RCU->RCU_CFG1 = rcu_cfg1;
                clocks.ck_sys = (clock_freq_t)freq;
                return 0;
            }
        }

    picoRTOS_break();
    /*@notreached@*/ return -EINVAL;
}

static int enable_pll_busywait(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    RCU->RCU_CTL |= RCU_CTL_PLLEN;
    while (deadlock-- != 0)
        if ((RCU->RCU_CTL & RCU_CTL_PLLSTB) != 0)
            break;

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int setup_ahb_div(unsigned long ahb_div)
{
    picoRTOS_assert(ahb_div > 0, return -EINVAL);

    uint32_t rcu_cfg0 = RCU->RCU_CFG0;
    clock_freq_t ck_ahb = clocks.ck_sys / (clock_freq_t)ahb_div;

    picoRTOS_assert(ck_ahb <= (clock_freq_t)AHB_MAX,
                    return -EIO);

    /* reset field */
    rcu_cfg0 &= ~RCU_CFG0_AHBPSC(RCU_CFG0_AHBPSC_M);

    switch (ahb_div) {
    case 1: break;
    case 2: rcu_cfg0 |= RCU_CFG0_AHBPSC(0x8); break;
    case 4: rcu_cfg0 |= RCU_CFG0_AHBPSC(0x9); break;
    case 8: rcu_cfg0 |= RCU_CFG0_AHBPSC(0xa); break;
    case 16: rcu_cfg0 |= RCU_CFG0_AHBPSC(0xb); break;
    case 64: rcu_cfg0 |= RCU_CFG0_AHBPSC(0xc); break;
    case 128: rcu_cfg0 |= RCU_CFG0_AHBPSC(0xd); break;
    case 256: rcu_cfg0 |= RCU_CFG0_AHBPSC(0xe); break;
    case 512: rcu_cfg0 |= RCU_CFG0_AHBPSC(0xf); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    clocks.ahb = ck_ahb;
    RCU->RCU_CFG0 = rcu_cfg0;

    return 0;
}

static int setup_apb1_div(unsigned long apb1_div)
{
    picoRTOS_assert(apb1_div > 0, return -EINVAL);

    uint32_t rcu_cfg0 = RCU->RCU_CFG0;
    clock_freq_t ck_apb1 = clocks.ahb / (clock_freq_t)apb1_div;

    picoRTOS_assert(ck_apb1 <= (clock_freq_t)APB1_MAX,
                    return -EIO);

    /* reset field */
    rcu_cfg0 &= ~RCU_CFG0_APB1PSC(RCU_CFG0_APB1PSC_M);

    switch (apb1_div) {
    case 1: break;
    case 2: rcu_cfg0 |= RCU_CFG0_APB1PSC(0x4); break;
    case 4: rcu_cfg0 |= RCU_CFG0_APB1PSC(0x5); break;
    case 8: rcu_cfg0 |= RCU_CFG0_APB1PSC(0x6); break;
    case 16: rcu_cfg0 |= RCU_CFG0_APB1PSC(0x7); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    clocks.apb1 = ck_apb1;
    RCU->RCU_CFG0 = rcu_cfg0;

    return 0;
}

static int setup_apb2_div(unsigned long apb2_div)
{
    picoRTOS_assert(apb2_div > 0, return -EINVAL);

    uint32_t rcu_cfg0 = RCU->RCU_CFG0;
    clock_freq_t ck_apb2 = clocks.ahb / (clock_freq_t)apb2_div;

    picoRTOS_assert(ck_apb2 <= (clock_freq_t)APB2_MAX,
                    return -EIO);

    /* reset field */
    rcu_cfg0 &= ~RCU_CFG0_APB2PSC(RCU_CFG0_APB2PSC_M);

    switch (apb2_div) {
    case 1: break;
    case 2: rcu_cfg0 |= RCU_CFG0_APB2PSC(0x4); break;
    case 4: rcu_cfg0 |= RCU_CFG0_APB2PSC(0x5); break;
    case 8: rcu_cfg0 |= RCU_CFG0_APB2PSC(0x6); break;
    case 16: rcu_cfg0 |= RCU_CFG0_APB2PSC(0x7); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    clocks.apb2 = ck_apb2;
    RCU->RCU_CFG0 = rcu_cfg0;

    return 0;
}

static int setup_adc_div(unsigned long adc_div)
{
    picoRTOS_assert(adc_div > 0, return -EINVAL);

    uint32_t rcu_cfg0 = RCU->RCU_CFG0;
    clock_freq_t ck_adc = clocks.apb2 / (clock_freq_t)adc_div;

    picoRTOS_assert(ck_adc <= (clock_freq_t)ADC_MAX,
                    return -EIO);

    /* reset field */
    rcu_cfg0 &= ~(RCU_CFG0_ADCPSC(RCU_CFG0_ADCPSC_M) | RCU_CFG0_ADCPSC2);

    switch (adc_div) {
    case 2: break;
    case 4: rcu_cfg0 |= RCU_CFG0_ADCPSC(0x1); break;
    case 6: rcu_cfg0 |= RCU_CFG0_ADCPSC(0x2); break;
    case 8: rcu_cfg0 |= RCU_CFG0_ADCPSC(0x3); break;
    case 12:
        rcu_cfg0 |= RCU_CFG0_ADCPSC2;
        rcu_cfg0 |= RCU_CFG0_ADCPSC(0x6);
        break;

    case 16:
        rcu_cfg0 |= RCU_CFG0_ADCPSC2;
        rcu_cfg0 |= RCU_CFG0_ADCPSC(0x7);
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    clocks.adc = ck_adc;
    RCU->RCU_CFG0 = rcu_cfg0;

    return 0;
}

/* Function: clock_gd32vf103_init
 * Initialises the GD32VF103 clock system
 *
 * Parameters:
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_gd32vf103_init(struct clock_settings *settings)
{
    picoRTOS_assert(settings->ck_sys < CLOCK_GD32VF103_CK_SYS_COUNT,
                    return -EINVAL);

    int res;

    /* first, set crystal freq */
    if (settings->hxtal != 0 &&
        (res = setup_hxtal_busywait(settings->hxtal)) < 0)
        return res;

    switch (settings->ck_sys) {
    case CLOCK_GD32VF103_CK_SYS_IRC8M:
        clocks.ck_sys = (clock_freq_t)IRC8M_FREQ;
        break;

    case CLOCK_GD32VF103_CK_SYS_HXTAL:
        clocks.ck_sys = clocks.hxtal;
        break;

    case CLOCK_GD32VF103_CK_SYS_PLL:
        if ((res = setup_pll(settings->pllsel, settings->pll)) < 0 ||
            (res = enable_pll_busywait()) < 0)
            return res;

        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    /* switch to selected clock */
    if ((res = switch_system_clock(settings->ck_sys)) < 0)
        return res;

    /* dividers */
    if ((res = setup_ahb_div(settings->ahb_div)) < 0 ||
        (res = setup_apb1_div(settings->apb1_div)) < 0 ||
        (res = setup_apb2_div(settings->apb2_div)) < 0 ||
        (res = setup_adc_div(settings->adc_div)) < 0)
        return res;

    return 0;
}

/* Function: clock_gd32vf103_enable
 * Enables a particular clock
 *
 * Parameters:
 *  clk - The clock identifier
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_gd32vf103_enable(clock_gd32vf103_clk_t clk)
{
    picoRTOS_assert(clk < CLOCK_GD32VF103_CLK_COUNT, return -EINVAL);

    size_t bus = (size_t)(clk >> 5);
    size_t bit = (size_t)(0x1f & clk);

    switch (bus) {
    /* AHB */
    case 0: RCU->RCU_AHBEN |= (1u << bit); break;
    /* APB1 */
    case 1: RCU->RCU_APB1EN |= (1u << bit); break;
    /* APB2 */
    case 2: RCU->RCU_APB2EN |= (1u << bit); break;
    /* ? */
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

/* Function: clock_gd32vf103_disable
 * Disables a particular clock
 *
 * Parameters:
 *  clk - The clock identifier
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_gd32vf103_disable(clock_gd32vf103_clk_t clk)
{
    picoRTOS_assert(clk < CLOCK_GD32VF103_CLK_COUNT, return -EINVAL);

    size_t bus = (size_t)(clk >> 5);
    size_t bit = (size_t)(0x1f & clk);

    switch (bus) {
    /* AHB */
    case 0: RCU->RCU_AHBEN &= ~(1u << bit); break;
    /* APB1 */
    case 1: RCU->RCU_APB1EN &= ~(1u << bit); break;
    /* APB2 */
    case 2: RCU->RCU_APB2EN &= ~(1u << bit); break;
    /* ? */
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

/* hooks */

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    picoRTOS_assert(clkid < (clock_id_t)CLOCK_GD32VF103_COUNT,
                    return (clock_freq_t)-EINVAL);

    switch (clkid) {
    case CLOCK_GD32VF103_CK_SYS: return clocks.ck_sys;
    case CLOCK_GD32VF103_AHB: return clocks.ahb;
    case CLOCK_GD32VF103_APB1: return clocks.apb1;
    case CLOCK_GD32VF103_APB2: return clocks.apb2;
    case CLOCK_GD32VF103_ADC: return clocks.adc;
    case CLOCK_GD32VF103_CK_FWDGT: return (clock_freq_t)IRC40K_FREQ;
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return (clock_freq_t)-EIO;
}
