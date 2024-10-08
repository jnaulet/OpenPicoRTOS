#include "clock-stm32l4x.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct RCC {
    volatile uint32_t RCC_CR;
    volatile uint32_t RCC_ICSCR;
    volatile uint32_t RCC_CFGR;
    volatile uint32_t RCC_PLLCFGR;
    volatile uint32_t RCC_PLLSAI1CFGR;
    uint32_t RESERVED0;
    volatile uint32_t RCC_CIER;
    volatile uint32_t RCC_CIFR;
    volatile uint32_t RCC_CICR;
    uint32_t RESERVED1;
    volatile uint32_t RCC_AHB1RSTR;
    volatile uint32_t RCC_AHB2RSTR;
    volatile uint32_t RCC_AHB3RSTR;
    uint32_t RESERVED2;
    volatile uint32_t RCC_APB1RSTR1;
    volatile uint32_t RCC_APB1RSTR2;
    volatile uint32_t RCC_APB2RSTR;
    uint32_t RESERVED3;
    volatile uint32_t RCC_AHB1ENR;
    volatile uint32_t RCC_AHB2ENR;
    volatile uint32_t RCC_AHB3ENR;
    uint32_t RESERVED4;
    volatile uint32_t RCC_APB1ENR1;
    volatile uint32_t RCC_APB1ENR2;
    volatile uint32_t RCC_APB2ENR;
    uint32_t RESERVED5;
    volatile uint32_t RCC_AHB1SMENR;
    volatile uint32_t RCC_AHB2SMENR;
    volatile uint32_t RCC_AHB3SMENR;
    uint32_t RESERVED6;
    volatile uint32_t RCC_APB1SMENR1;
    volatile uint32_t RCC_APB1SMENR2;
    volatile uint32_t RCC_APB2SMENR;
    uint32_t RESERVED7;
    volatile uint32_t RCC_CCIPR;
    uint32_t RESERVED8;
    volatile uint32_t RCC_BDCR;
    volatile uint32_t RCC_CSR;
    volatile uint32_t RCC_CRRCR;
    volatile uint32_t RCC_CCIPR2;
};

#define RCC_CR_PLLSAI1RDY  (1 << 27)
#define RCC_CR_PLLSAI1ON   (1 << 26)
#define RCC_CR_PLLRDY      (1 << 25)
#define RCC_CR_PLLON       (1 << 24)
#define RCC_CR_CSSON       (1 << 19)
#define RCC_CR_HSEBYP      (1 << 18)
#define RCC_CR_HSERDY      (1 << 17)
#define RCC_CR_HSEON       (1 << 16)
#define RCC_CR_HSIASFS     (1 << 11)
#define RCC_CR_HSIRDY      (1 << 10)
#define RCC_CR_HSIKERON    (1 << 9)
#define RCC_CR_HSION       (1 << 8)
#define RCC_CR_MSIRANGE_M  0xfu
#define RCC_CR_MSIRANGE(x) (((x) & RCC_CR_MSIRANGE_M) << 4)
#define RCC_CR_MSIRDY      (1 << 1)
#define RCC_CR_MSION       (1 << 0)

#define RCC_CFGR_MCOPRE_M  0x7u
#define RCC_CFGR_MCOPRE(x) (((x) & RCC_CFGR_MCO2PRE_M) << 28)
#define RCC_CFGR_MCOSEL_M  0xfu
#define RCC_CFGR_MCOSEL(x) (((x) & RCC_CFGR_MCO2PRE_M) << 24)
#define RCC_CFGR_STOPWUCK  (1 << 15)
#define RCC_CFGR_PPRE2_M   0x7u
#define RCC_CFGR_PPRE2(x)  (((x) & RCC_CFGR_PPRE2_M) << 11)
#define RCC_CFGR_PPRE1_M   0x7u
#define RCC_CFGR_PPRE1(x)  (((x) & RCC_CFGR_PPRE1_M) << 8)
#define RCC_CFGR_HPRE_M    0xfu
#define RCC_CFGR_HPRE(x)   (((x) & RCC_CFGR_HPRE_M) << 4)
#define RCC_CFGR_SWS_M     0x3u
#define RCC_CFGR_SWS(x)    (((x) & RCC_CFGR_SWS_M) << 2)
#define RCC_CFGR_SW_M      0x3u
#define RCC_CFGR_SW(x)     ((x) & RCC_CFGR_SW_M)

#define RCC_PLLCFGR_PLLPDIV_M  0x1fu
#define RCC_PLLCFGR_PLLPDIV(x) (((x) & RCC_PLLCFGR_PLLPDIV_M) << 27)
#define RCC_PLLCFGR_PLLR_M     0x3u
#define RCC_PLLCFGR_PLLR(x)    (((x) & RCC_PLLCFGR_PLLR_M) << 25)
#define RCC_PLLCFGR_PLLREN     (1 << 24)
#define RCC_PLLCFGR_PLLQ_M     0x3u
#define RCC_PLLCFGR_PLLQ(x)    (((x) & RCC_PLLCFGR_PLLQ_M) << 21)
#define RCC_PLLCFGR_PLLQEN     (1 << 20)
#define RCC_PLLCFGR_PLLP       (1 << 17)
#define RCC_PLLCFGR_PLLPEN     (1 << 16)
#define RCC_PLLCFGR_PLLN_M     0x7fu
#define RCC_PLLCFGR_PLLN(x)    (((x) & RCC_PLLCFGR_PLLN_M) << 8)
#define RCC_PLLCFGR_PLLM_M     0x7u
#define RCC_PLLCFGR_PLLM(x)    (((x) & RCC_PLLCFGR_PLLM_M) << 4)
#define RCC_PLLCFGR_PLLSRC_M   0x3u
#define RCC_PLLCFGR_PLLSRC(x)  ((x) & RCC_PLLCFGR_PLLSRC_M)

#define RCC_CIxR_HSI48RDYx   (1 << 10)
#define RCC_CIxR_LSECSSx     (1 << 9)
#define RCC_CIxR_CSSx        (1 << 8)
#define RCC_CIxR_PLLSAI1RDYx (1 << 6)
#define RCC_CIxR_PLLRDYx     (1 << 5)
#define RCC_CIxR_HSERDYx     (1 << 4)
#define RCC_CIxR_HSIRDYx     (1 << 3)
#define RCC_CIxR_MSIRDYx     (1 << 2)
#define RCC_CIxR_LSERDYx     (1 << 1)
#define RCC_CIxR_LSIRDYx     (1 << 0)

#define RCC_BDCR_LSCOSEL   (1 << 25)
#define RCC_BDCR_LSCOEN    (1 << 24)
#define RCC_BDCR_BDRST     (1 << 16)
#define RCC_BDCR_RTCEN     (1 << 15)
#define RCC_BDCR_RTCSEL_M  0x3u
#define RCC_BDCR_RTCSEL(x) (((x) & RCC_BDCR_RTCSEL_M) << 8)
#define RCC_BDCR_LSESYSDIS (1 << 7)
#define RCC_BDCR_LSECSSD   (1 << 6)
#define RCC_BDCR_LSECSSON  (1 << 5)
#define RCC_BDCR_LSEDRV_M  0x3u
#define RCC_BDCR_LSEDRV(x) (((x) & RCC_BDCR_LSEDRV_M) << 3)
#define RCC_BDCR_LSEBYP    (1 << 2)
#define RCC_BDCR_LSERDY    (1 << 1)
#define RCC_BDCR_LSEON     (1 << 0)

#define RCC_CSR_LPWRRSTF    (1 << 31u)
#define RCC_CSR_WWDGRSTF    (1 << 30)
#define RCC_CSR_IWDGRSTF    (1 << 29)
#define RCC_CSR_SFTRSTF     (1 << 28)
#define RCC_CSR_BORRSTF     (1 << 27)
#define RCC_CSR_PINRSTF     (1 << 26)
#define RCC_CSR_OBLRSTF     (1 << 25)
#define RCC_CSR_FWRSTF      (1 << 24)
#define RCC_CSR_RMVF        (1 << 23)
#define RCC_CSR_MSIRANGE_M  0xfu
#define RCC_CSR_MSIRANGE(x) (((x) & RCC_CSR_MSIRANGE_M) << 8)
#define RCC_CSR_LSIPREDIV   (1 << 4)
#define RCC_CSR_LSIRDY      (1 << 1)
#define RCC_CSR_LSION       (1 << 0)

#define RCC_CRRCR_HSI48CAL_M  0x1ffu
#define RCC_CRRCR_HSI48CAL(x) (((x) & RCC_CRRCR_HSI48CAL_M) << 7)
#define RCC_CRRCR_HSI48RDY    (1 << 1)
#define RCC_CRRCR_HSI48ON     (1 << 0)

#define RCC_CCIPR2_I2C4SEL_M  0x3u
#define RCC_CCIPR2_I2C4SEL(x) ((x) & RCC_CCIPR2_I2C4SEL_M)

struct FLASH {
    volatile uint32_t FLASH_ACR;
    /* ... */
};

#define FLASH_ACR_SLEEP_PD   (1 << 14)
#define FLASH_ACR_RUN_PD     (1 << 13)
#define FLASH_ACR_DCRST      (1 << 12)
#define FLASH_ACR_ICRST      (1 << 11)
#define FLASH_ACR_DCEN       (1 << 10)
#define FLASH_ACR_ICEN       (1 << 9)
#define FLASH_ACR_PRFTEN     (1 << 8)
#define FLASH_ACR_LATENCY_M  0x7u
#define FLASH_ACR_LATENCY(x) ((x) & FLASH_ACR_LATENCY_M)

struct PWR {
    volatile uint32_t PWR_CR1;
    volatile uint32_t PWR_CR2;
    volatile uint32_t PWR_CR3;
    volatile uint32_t PWR_CR4;
    volatile uint32_t PWR_SR1;
    volatile uint32_t PWR_SR2;
    volatile uint32_t PWR_SCR;
    uint32_t RESERVED[2];
    volatile uint32_t PWR_PUCRA;
    volatile uint32_t PWR_PDCRA;
    /* ... */
};

#define PWR_CR1_LPR     (1 << 14)
#define PWR_CR1_VOS_M   0x3u
#define PWR_CR1_VOS(x)  (((x) & PWR_CR1_VOS_M) << 14)
#define PWR_CR1_LPMS_M  0x7u
#define PWR_CR1_LPMS(x) ((x) & PWR_CR1_LPMS_M)

#define PWR_CR2_USV    (1 << 10)
#define PWR_CR2_PVME4  (1 << 7)
#define PWR_CR2_PVME3  (1 << 6)
#define PWR_CR2_PVME1  (1 << 4)
#define PWR_CR2_PLS_M  0x7u
#define PWR_CR2_PLS(x) (((x) & PWR_CR_PLS_M) << 1)
#define PWR_CR2_PVDE   (1 << 0)

#define PWR_CR_CSBF   (1 << 3)
#define PWR_CR_CWUF   (1 << 2)
#define PWR_CR_PDDS   (1 << 1)
#define PWR_CR_LPDS   (1 << 0)

#define PWR_CSR_VOSRDY (1 << 14)
#define PWR_CSR_BRE    (1 << 9)
#define PWR_CSR_EWUP   (1 << 8)
#define PWR_CSR_BRR    (1 << 3)
#define PWR_CSR_PVDO   (1 << 2)
#define PWR_CSR_SBF    (1 << 1)
#define PWR_CSR_WUF    (1 << 0)

/* instances */
static struct RCC *RCC = (struct RCC*)ADDR_RCC;
static struct FLASH *FLASH = (struct FLASH*)ADDR_FLASH;
static struct PWR *PWR = (struct PWR*)ADDR_PWR;

#define LSI_FREQ 32000
#define LSE_FREQ 32768
#define HSI_FREQ 16000000
#define USB_FREQ 48000000

static struct {
    clock_freq_t pll_r;
    clock_freq_t pll_q;
    clock_freq_t sysclk;
    clock_freq_t ahb;
    clock_freq_t apb1;
    clock_freq_t apb2;
} clocks;

static int turn_hsi_on(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    RCC->RCC_CR |= RCC_CR_HSION;
    while ((RCC->RCC_CR & RCC_CR_HSIRDY) == 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int setup_hse(unsigned long hse)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* disabled */
    if (hse == 0)
        return 0;

    RCC->RCC_CR |= RCC_CR_HSEON;
    while ((RCC->RCC_CR & RCC_CR_HSERDY) == 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int setup_pll(struct clock_settings *settings)
{
#define VCO_MIN    64000000ul
#define VCO_MAX    344000000ul
#define VCO_IN_MAX 16000000ul
#define VCO_IN_MIN 4000000ul
#define PLL_R_MAX  80000000ul

    picoRTOS_assert(settings->pllsrc < CLOCK_STM32L4X_PLLSRC_COUNT, return -EINVAL);
    picoRTOS_assert(settings->pll_r <= PLL_R_MAX, return -EINVAL);

    unsigned long in;
    unsigned long m, n, r;

    /* disable */
    if (settings->pll_r == 0 ||
        settings->pllsrc == CLOCK_STM32L4X_PLLSRC_NONE) {
        RCC->RCC_CR &= ~RCC_CR_PLLON;
        return 0;
    }

    /* turn on PWR & set scale1 mode */
    (void)clock_stm32l4x_enable(CLOCK_STM32L4X_CLK_PWR);
    PWR->PWR_CR1 = (uint32_t)((PWR->PWR_CR1 & PWR_CR1_VOS(PWR_CR1_VOS_M)) |
                              PWR_CR1_VOS(1));

    /* input clk frequency */
    RCC->RCC_PLLCFGR &= ~RCC_PLLCFGR_PLLSRC(RCC_PLLCFGR_PLLSRC_M);

    switch (settings->pllsrc) {
    case CLOCK_STM32L4X_PLLSRC_MSI: /*@fallthrough@*/ /* FIXME */
    case CLOCK_STM32L4X_PLLSRC_HSI16:
        RCC->RCC_PLLCFGR |= RCC_PLLCFGR_PLLSRC(2);
        in = (unsigned long)HSI_FREQ;
        break;

    case CLOCK_STM32L4X_PLLSRC_HSE:
        picoRTOS_assert(settings->hse != 0, return -EINVAL);
        RCC->RCC_PLLCFGR |= RCC_PLLCFGR_PLLSRC(3);
        in = settings->hse;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    /* setup
     * vco = pllsrc * (N / M)
     * p = vco / P
     * q = vco / Q
     * r = vco / R
     */

    /* vco input is 4-16Mhz */
    for (m = (in / VCO_IN_MAX); m <= (in / VCO_IN_MIN); m++) {
        /* 8 <= PLLN <= 86 */
        for (n = 8ul; n <= 86ul; n++) {

            unsigned long vco = (in / m) * n;

            if (vco < VCO_MIN) /*@innerbreak@*/ break;
            if (vco > VCO_MAX) /*@innerbreak@*/ break;

            /* PLLR = 2, 4, 6, or 8 */
            for (r = 2ul; r <= 8ul; r += 2ul) {
                if ((vco / r) == settings->pll_r) {

                    int deadlock = CONFIG_DEADLOCK_COUNT;
                    unsigned long q = vco / (unsigned long)USB_FREQ;

                    /* setup */
                    RCC->RCC_PLLCFGR &= ~(RCC_PLLCFGR_PLLR(RCC_PLLCFGR_PLLR_M) |
                                          RCC_PLLCFGR_PLLQ(RCC_PLLCFGR_PLLQ_M) |
                                          RCC_PLLCFGR_PLLM(RCC_PLLCFGR_PLLM_M) |
                                          RCC_PLLCFGR_PLLN(RCC_PLLCFGR_PLLN_M));
                    /* R, Q, M, N */
                    RCC->RCC_PLLCFGR |= RCC_PLLCFGR_PLLR((r >> 1) - 1);
                    RCC->RCC_PLLCFGR |= RCC_PLLCFGR_PLLREN;
                    RCC->RCC_PLLCFGR |= RCC_PLLCFGR_PLLQ(q);
                    RCC->RCC_PLLCFGR |= RCC_PLLCFGR_PLLM(m - 1);
                    RCC->RCC_PLLCFGR |= RCC_PLLCFGR_PLLN(n);
                    /* turn on */
                    RCC->RCC_CR |= RCC_CR_PLLON;
                    /* wait */
                    while ((RCC->RCC_CR & RCC_CR_PLLRDY) == 0 && deadlock-- != 0) arch_delay_us(1ul);
                    picoRTOS_assert(deadlock != -1, return -EBUSY);
                    /* remember */
                    clocks.pll_r = (clock_freq_t)settings->pll_r;
                    clocks.pll_q = (clock_freq_t)(vco / q);
                    return 0;
                }
            }
        }
    }

    /* something went wrong */
    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static void setup_flash(void)
{
    /* latency computation */
    FLASH->FLASH_ACR &= ~FLASH_ACR_LATENCY(FLASH_ACR_LATENCY_M);

    if (clocks.sysclk <= (clock_freq_t)16000000)
        FLASH->FLASH_ACR |= FLASH_ACR_LATENCY(0);
    else if (clocks.sysclk <= (clock_freq_t)32000000)
        FLASH->FLASH_ACR |= FLASH_ACR_LATENCY(1);
    else if (clocks.sysclk <= (clock_freq_t)48000000)
        FLASH->FLASH_ACR |= FLASH_ACR_LATENCY(2);
    else if (clocks.sysclk <= (clock_freq_t)64000000)
        FLASH->FLASH_ACR |= FLASH_ACR_LATENCY(3);
    else
        FLASH->FLASH_ACR |= FLASH_ACR_LATENCY(4);
}

static int set_hpre(unsigned int prescaler)
{
    picoRTOS_assert(prescaler != 0, return -EINVAL);
    picoRTOS_assert(prescaler <= 512u, return -EINVAL);

    RCC->RCC_CFGR &= ~RCC_CFGR_HPRE(RCC_CFGR_HPRE_M);

    switch (prescaler) {
    case 1: RCC->RCC_CFGR |= RCC_CFGR_HPRE(0); break;
    case 2: RCC->RCC_CFGR |= RCC_CFGR_HPRE(8); break;
    case 4: RCC->RCC_CFGR |= RCC_CFGR_HPRE(9); break;
    case 8: RCC->RCC_CFGR |= RCC_CFGR_HPRE(10); break;
    case 16: RCC->RCC_CFGR |= RCC_CFGR_HPRE(11); break;
    case 64: RCC->RCC_CFGR |= RCC_CFGR_HPRE(12); break;
    case 128: RCC->RCC_CFGR |= RCC_CFGR_HPRE(13); break;
    case 256: RCC->RCC_CFGR |= RCC_CFGR_HPRE(14); break;
    case 512: RCC->RCC_CFGR |= RCC_CFGR_HPRE(15); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    clocks.ahb = clocks.sysclk / (clock_freq_t)prescaler;
    return 0;
}

static int set_ppre1(unsigned int prescaler)
{
    picoRTOS_assert(prescaler != 0, return -EINVAL);
    picoRTOS_assert(prescaler <= 16u, return -EINVAL);

    RCC->RCC_CFGR &= ~RCC_CFGR_PPRE1(RCC_CFGR_PPRE1_M);

    switch (prescaler) {
    case 1: RCC->RCC_CFGR |= RCC_CFGR_PPRE1(0); break;
    case 2: RCC->RCC_CFGR |= RCC_CFGR_PPRE1(4); break;
    case 4: RCC->RCC_CFGR |= RCC_CFGR_PPRE1(5); break;
    case 8: RCC->RCC_CFGR |= RCC_CFGR_PPRE1(6); break;
    case 16: RCC->RCC_CFGR |= RCC_CFGR_PPRE1(7); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    clocks.apb1 = clocks.sysclk / (clock_freq_t)prescaler;
    picoRTOS_assert(clocks.apb1 <= (clock_freq_t)42000000, return -EINVAL);
    return 0;
}

static int set_ppre2(unsigned int prescaler)
{
    picoRTOS_assert(prescaler != 0, return -EINVAL);
    picoRTOS_assert(prescaler <= 16u, return -EINVAL);

    RCC->RCC_CFGR &= ~RCC_CFGR_PPRE2(RCC_CFGR_PPRE2_M);

    switch (prescaler) {
    case 1: RCC->RCC_CFGR |= RCC_CFGR_PPRE2(0); break;
    case 2: RCC->RCC_CFGR |= RCC_CFGR_PPRE2(4); break;
    case 4: RCC->RCC_CFGR |= RCC_CFGR_PPRE2(5); break;
    case 8: RCC->RCC_CFGR |= RCC_CFGR_PPRE2(6); break;
    case 16: RCC->RCC_CFGR |= RCC_CFGR_PPRE2(7); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    clocks.apb2 = clocks.sysclk / (clock_freq_t)prescaler;
    return 0;
}

/* Function: clock_stm32l4x_init
 * Initialises the stm32l4x clock system
 *
 * Parameters:
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_stm32l4x_init(struct clock_settings *settings)
{
    picoRTOS_assert(settings->sysclk < CLOCK_STM32L4X_SYSCLK_COUNT, return -EINVAL);
    picoRTOS_assert(settings->ahb_prescaler != 0, return -EINVAL);
    picoRTOS_assert(settings->apb1_prescaler != 0, return -EINVAL);
    picoRTOS_assert(settings->apb2_prescaler != 0, return -EINVAL);

    int res;
    int deadlock = CONFIG_DEADLOCK_COUNT;

    if ((res = setup_hse(settings->hse)) < 0 ||
        (res = setup_pll(settings)) < 0)
        return res;

    /* prepare sysclk future values */
    switch (settings->sysclk) {
    case CLOCK_STM32L4X_SYSCLK_MSI:
        picoRTOS_break();
        /*@notreached@*/ return 0;

    case CLOCK_STM32L4X_SYSCLK_HSI16:
        if ((res = turn_hsi_on()) < 0) return res;
        clocks.sysclk = (clock_freq_t)HSI_FREQ;
        break;

    case CLOCK_STM32L4X_SYSCLK_PLL:
        picoRTOS_assert(settings->pll_r != 0, return -EINVAL);
        clocks.sysclk = clocks.pll_r;
        break;

    case CLOCK_STM32L4X_SYSCLK_HSE:
        picoRTOS_assert(settings->hse != 0, return -EINVAL);
        clocks.sysclk = (clock_freq_t)settings->hse;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    /* flash & prescalers */
    setup_flash();
    if ((res = set_hpre(settings->ahb_prescaler)) < 0 ||
        (res = set_ppre1(settings->apb1_prescaler)) < 0 ||
        (res = set_ppre2(settings->apb2_prescaler)) < 0)
        return res;

    /* switch clock */
    RCC->RCC_CFGR = (uint32_t)((RCC->RCC_CFGR & ~RCC_CFGR_SW(RCC_CFGR_SW_M)) |
                               RCC_CFGR_SW(settings->sysclk));
    /* wait */
    while ((RCC->RCC_CFGR & RCC_CFGR_SWS(RCC_CFGR_SWS_M)) != RCC_CFGR_SWS(settings->sysclk) &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    /* finally, setup arch */
    arch_set_clock_frequency((unsigned long)clocks.ahb);
    return 0;
}

int clock_stm32l4x_enable(clock_stm32l4x_clk_t clk)
{
    picoRTOS_assert(clk < CLOCK_STM32L4X_CLK_COUNT, return -EINVAL);

    int bus = (int)(clk >> 5);
    uint32_t mask = (uint32_t)(1 << (0x1fu & clk));

    switch (bus) {
    case 0: RCC->RCC_AHB1ENR |= mask; break;    /* AHB1 */
    case 1: RCC->RCC_AHB2ENR |= mask; break;    /* AHB2 */
    case 2: RCC->RCC_AHB3ENR |= mask; break;    /* AHB3 */
    case 3: RCC->RCC_APB1ENR1 |= mask; break;   /* APB1.1 */
    case 4: RCC->RCC_APB1ENR2 |= mask; break;   /* APB1.2 */
    case 5: RCC->RCC_APB2ENR |= mask; break;    /* APB2 */
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

int clock_stm32l4x_disable(clock_stm32l4x_clk_t clk)
{
    picoRTOS_assert(clk < CLOCK_STM32L4X_CLK_COUNT, return -EINVAL);

    int bus = (int)(clk >> 5);
    uint32_t mask = (uint32_t)(1 << (0x1fu & clk));

    switch (bus) {
    case 0: RCC->RCC_AHB1ENR &= ~mask; break;   /* AHB1 */
    case 1: RCC->RCC_AHB2ENR &= ~mask; break;   /* AHB2 */
    case 2: RCC->RCC_AHB3ENR &= ~mask; break;   /* AHB3 */
    case 3: RCC->RCC_APB1ENR1 &= ~mask; break;  /* APB1.1 */
    case 4: RCC->RCC_APB1ENR2 &= ~mask; break;  /* APB1.2 */
    case 5: RCC->RCC_APB2ENR &= ~mask; break;   /* APB2 */
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    switch (clkid) {
    case CLOCK_STM32L4X_SYSCLK: return clocks.sysclk;
    case CLOCK_STM32L4X_HCLK: return clocks.ahb;
    case CLOCK_STM32L4X_PCLK1: return clocks.apb1;
    case CLOCK_STM32L4X_PCLK2: return clocks.apb2;
    case CLOCK_STM32L4X_USB: return (clock_freq_t)USB_FREQ;
    case CLOCK_STM32L4X_IWDG: return (clock_freq_t)LSI_FREQ;
    case CLOCK_STM32L4X_RTC: return (clock_freq_t)LSE_FREQ;    /* FIXME */
    default: break;
    }

    /* unknown */
    picoRTOS_break();
    /*@notreached@*/ return (clock_freq_t)-EIO;
}
