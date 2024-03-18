#include "clock-stm32f401x.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct RCC {
    volatile uint32_t RCC_CR;
    volatile uint32_t RCC_PLLCFGR;
    volatile uint32_t RCC_CFGR;
    volatile uint32_t RCC_CIR;
    volatile uint32_t RCC_AHB1RSTR;
    volatile uint32_t RCC_AHB2RSTR;
    uint32_t RESERVED0[2];
    volatile uint32_t RCC_APB1RSTR;
    volatile uint32_t RCC_APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t RCC_AHB1ENR;
    volatile uint32_t RCC_AHB2ENR;
    uint32_t RESERVED2[2];
    volatile uint32_t RCC_APB1ENR;
    volatile uint32_t RCC_APB2ENR;
    uint32_t RESERVED3[2];
    volatile uint32_t RCC_AHB1LPENR;
    volatile uint32_t RCC_AHB2LPENR;
    uint32_t RESERVED4[2];
    volatile uint32_t RCC_APB1LPENR;
    volatile uint32_t RCC_APB2LPENR;
    uint32_t RESERVED5[2];
    volatile uint32_t RCC_BDCR;
    volatile uint32_t RCC_CSR;
    uint32_t RESERVED6[2];
    volatile uint32_t RCC_SSCGR;
    volatile uint32_t RCC_PLLI2SCFGR;
    uint32_t RESERVED7;
    volatile uint32_t RCC_DCKCFGR;
};

#define RCC_CR_PLLI2SRDY  (1 << 27)
#define RCC_CR_PLLI2SON   (1 << 26)
#define RCC_CR_PLLRDY     (1 << 25)
#define RCC_CR_PLLON      (1 << 24)
#define RCC_CR_CSSON      (1 << 19)
#define RCC_CR_HSEBYP     (1 << 18)
#define RCC_CR_HSERDY     (1 << 17)
#define RCC_CR_HSEON      (1 << 16)
#define RCC_CR_HSICAL_M   0xffu
#define RCC_CR_HSICAL(x)  (((x) & RCC_CR_HSICAL_M) << 8)
#define RCC_CR_HSITRIM_M  0x1fu
#define RCC_CR_HSITRIM(x) (((x) & RCC_CR_HSITRIM_M) << 3)
#define RCC_CR_HSIRDY     (1 << 1)
#define RCC_CR_HSION      (1 << 0)

#define RCC_PLLCFGR_PLLQ_M  0xfu
#define RCC_PLLCFGR_PLLQ(x) (((x) & RCC_PLLCFGR_PLLQ_M) << 24)
#define RCC_PLLCFGR_PLLSRC  (1 << 22)
#define RCC_PLLCFGR_PLLP_M  0x3u
#define RCC_PLLCFGR_PLLP(x) (((x) & RCC_PLLCFGR_PLLP_M) << 16)
#define RCC_PLLCFGR_PLLN_M  0x1ffu
#define RCC_PLLCFGR_PLLN(x) (((x) & RCC_PLLCFGR_PLLN_M) << 6)
#define RCC_PLLCFGR_PLLM_M  0x1fu
#define RCC_PLLCFGR_PLLM(x) ((x) & RCC_PLLCFGR_PLLM_M)

#define RCC_CFGR_MCO2_M     0x3u
#define RCC_CFGR_MCO2(x)    (((x) & RCC_CFGR_MCO2_M) << 30)
#define RCC_CFGR_MCO2PRE_M  0x7u
#define RCC_CFGR_MCO2PRE(x) (((x) & RCC_CFGR_MCO2PRE_M) << 27)
#define RCC_CFGR_MCO1PRE_M  0x7u
#define RCC_CFGR_MCO1PRE(x) (((x) & RCC_CFGR_MCO2PRE_M) << 24)
#define RCC_CFGR_I2SSRC     (1 << 23)
#define RCC_CFGR_MCO1_M     0x3u
#define RCC_CFGR_MCO1(x)    (((x) & RCC_CFGR_MCO1_M) << 21)
#define RCC_CFGR_RTCPRE_M   0x1fu
#define RCC_CFGR_RTCPRE(x)  (((x) & RCC_CFGR_RTCPRE_M) << 16)
#define RCC_CFGR_PPRE2_M    0x7u
#define RCC_CFGR_PPRE2(x)   (((x) & RCC_CFGR_PPRE2_M) << 13)
#define RCC_CFGR_PPRE1_M    0x7u
#define RCC_CFGR_PPRE1(x)   (((x) & RCC_CFGR_PPRE1_M) << 10)
#define RCC_CFGR_HPRE_M     0xfu
#define RCC_CFGR_HPRE(x)    (((x) & RCC_CFGR_HPRE_M) << 4)
#define RCC_CFGR_SWS_M      0x3u
#define RCC_CFGR_SWS(x)     (((x) & RCC_CFGR_SWS_M) << 2)
#define RCC_CFGR_SW_M       0x3u
#define RCC_CFGR_SW(x)      ((x) & RCC_CFGR_SW_M)

#define RCC_CIR_CSSC        (1 << 23)
#define RCC_CIR_PLLI2SRDYC  (1 << 21)
#define RCC_CIR_PLLRDYC     (1 << 20)
#define RCC_CIR_HSERDYC     (1 << 19)
#define RCC_CIR_HSIRDYC     (1 << 18)
#define RCC_CIR_LSERDYC     (1 << 17)
#define RCC_CIR_LSIRDYC     (1 << 16)
#define RCC_CIR_PLLI2SRDYIE (1 << 13)
#define RCC_CIR_PLLRDYIE    (1 << 12)
#define RCC_CIR_HSERDYIE    (1 << 11)
#define RCC_CIR_HSIRDYIE    (1 << 10)
#define RCC_CIR_LSERDYIE    (1 << 9)
#define RCC_CIR_LSIRDYIE    (1 << 8)
#define RCC_CIR_CSSF        (1 << 7)
#define RCC_CIR_PLLI2SRDYF  (1 << 5)
#define RCC_CIR_PLLRDYF     (1 << 4)
#define RCC_CIR_HSERDYF     (1 << 3)
#define RCC_CIR_HSIRDYF     (1 << 2)
#define RCC_CIR_LSERDYF     (1 << 1)
#define RCC_CIR_LSIRDYF     (1 << 0)

#define RCC_BDCR_BDRST     (1 << 16)
#define RCC_BDCR_RTCEN     (1 << 15)
#define RCC_BDCR_RTCSEL_M  0x3u
#define RCC_BDCR_RTCSEL(x) (((x) & RCC_BDCR_RTCSEL_M) << 8)
#define RCC_BDCR_LSEBYP    (1 << 2)
#define RCC_BDCR_LSERDY    (1 << 1)
#define RCC_BDCR_LSEON     (1 << 0)

#define RCC_CSR_LPWRRSTF (1 << 31u)
#define RCC_CSR_WWDGRSTF (1 << 30)
#define RCC_CSR_IWDGRSTF (1 << 29)
#define RCC_CSR_SFTRSTF  (1 << 28)
#define RCC_CSR_PORRSTF  (1 << 27)
#define RCC_CSR_PINRSTF  (1 << 26)
#define RCC_CSR_RMVF     (1 << 24)
#define RCC_CSR_LSIRDY   (1 << 1)
#define RCC_CSR_LSION    (1 << 0)

#define RCC_SSCGR_SSCGEN     (1u << 31)
#define RCC_SSCGR_SPREADSEL  (1 << 30)
#define RCC_SSCGR_INCSTEP_M  0x7ffu
#define RCC_SSCGR_INCSTEP(x) (((x) & RCC_SSCGR_INCSTEP_M) << 13)
#define RCC_SSCGR_MODPER_M   0xfffu
#define RCC_SSCGR_MODPER(x)  ((x) & RCC_SSCGR_MODPER_M)

#define RCC_PLLI2SCFGR_PLLI2SR_M  0x7u
#define RCC_PLLI2SCFGR_PLLI2SR(x) (((x) & RCC_PLLI2SCFGR_PLLI2SR_M) << 28)
#define RCC_PLLI2SCFGR_PLLI2SN_M  0x1ffu
#define RCC_PLLI2SCFGR_PLLI2SN(x) (((x) & RCC_PLLI2SCFGR_PLLI2SN_M) << 6)

#define RCC_DCKCFGR_TIMPRE (1 << 24)

struct FLASH {
    volatile uint32_t FLASH_ACR;
    /* ... */
};

#define FLASH_ACR_DCRST      (1 << 12)
#define FLASH_ACR_ICRST      (1 << 11)
#define FLASH_ACR_DCEN       (1 << 10)
#define FLASH_ACR_ICEN       (1 << 9)
#define FLASH_ACR_PRFTEN     (1 << 8)
#define FLASH_ACR_LATENCY_M  0xfu
#define FLASH_ACR_LATENCY(x) ((x) & FLASH_ACR_LATENCY_M)

struct PWR {
    volatile uint32_t PWR_CR;
    volatile uint32_t PWR_CSR;
};

#define PWR_CR_VOS_M  0x3u
#define PWR_CR_VOS(x) (((x) & PWR_CR_VOS_M) << 14)
#define PWR_CR_ADCDC1 (1 << 13)
#define PWR_CR_MRLVDS (1 << 11)
#define PWR_CR_LPLVDS (1 << 10)
#define PWR_CR_FPDS   (1 << 9)
#define PWR_CR_DBP    (1 << 8)
#define PWR_CR_PLS_M  0x7u
#define PWR_CR_PLS(x) (((x) & PWR_CR_PLS_M) << 5)
#define PWR_CR_PVDE   (1 << 4)
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
static struct FLASH *FLASH = (struct FLASH*)ADDR_FIR;
static struct PWR *PWR = (struct PWR*)ADDR_PWR;

#define HSI_FREQ 16000000
#define LSE_FREQ 32768
#define LSI_FREQ 32000
#define USB_FREQ 48000000

static struct {
    clock_freq_t pll_p;
    clock_freq_t pll_q;
    clock_freq_t sysclk;
    clock_freq_t ahb;
    clock_freq_t apb1;
    clock_freq_t apb2;
} clocks;

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
#define VCO_MAX    432000000ul
#define VCO_IN_MAX 2000000ul
#define VCO_IN_MIN 1000000ul
#define PLL_P_MAX  84000000ul

    picoRTOS_assert(settings->pllsrc < CLOCK_STM32F401X_PLLSRC_COUNT, return -EINVAL);
    picoRTOS_assert(settings->pll_p <= PLL_P_MAX, return -EINVAL);

    unsigned long in;
    unsigned long m, n, p;

    /* disable */
    if (settings->pll_p == 0) {
        RCC->RCC_CR &= ~RCC_CR_PLLON;
        return 0;
    }

    /* turn on PWR & set scale 2 mode */
    (void)clock_stm32f401x_enable(CLOCK_STM32F401X_CLK_PWR);
    PWR->PWR_CR = (uint32_t)(PWR->PWR_CR & PWR_CR_VOS(PWR_CR_VOS_M) |
                             PWR_CR_VOS(2));

    /* input clk frequency */
    switch (settings->pllsrc) {
    case CLOCK_STM32F401X_PLLSRC_HSI:
        RCC->RCC_PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
        in = (unsigned long)HSI_FREQ;
        break;

    case CLOCK_STM32F401X_PLLSRC_HSE:
        picoRTOS_assert(settings->hse != 0, return -EINVAL);
        RCC->RCC_PLLCFGR |= RCC_PLLCFGR_PLLSRC;
        in = settings->hse;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    /* setup
     * vco = (pllsrc / M) * N
     * p = vco / P
     */

    /* vco input is 2Mhz ideally, down to 1Mhz */
    for (m = (in / VCO_IN_MAX); m <= (in / VCO_IN_MIN); m++) {
        /* 192 <= PLLN <= 432 */
        for (n = 192ul; n <= 423ul; n++) {

            unsigned long vco = (in / m) * n;
            if (vco > VCO_MAX) /*@innerbreak@*/ break;

            /* PLLP = 2, 4, 6, or 8 */
            for (p = 2ul; p <= 8ul; p += 2ul) {
                if ((vco / p) == settings->pll_p) {

                    int deadlock = CONFIG_DEADLOCK_COUNT;
                    unsigned long q = vco / (unsigned long)USB_FREQ;

                    /* setup */
                    RCC->RCC_PLLCFGR &= ~(RCC_PLLCFGR_PLLP(RCC_PLLCFGR_PLLP_M) |
                                          RCC_PLLCFGR_PLLQ(RCC_PLLCFGR_PLLQ_M) |
                                          RCC_PLLCFGR_PLLM(RCC_PLLCFGR_PLLM_M) |
                                          RCC_PLLCFGR_PLLN(RCC_PLLCFGR_PLLN_M));
                    /* P, M, N */
                    RCC->RCC_PLLCFGR |= RCC_PLLCFGR_PLLP((p >> 1) - 1);
                    RCC->RCC_PLLCFGR |= RCC_PLLCFGR_PLLQ(q);
                    RCC->RCC_PLLCFGR |= RCC_PLLCFGR_PLLM(m);
                    RCC->RCC_PLLCFGR |= RCC_PLLCFGR_PLLN(n);
                    /* turn on */
                    RCC->RCC_CR |= RCC_CR_PLLON;
                    /* wait */
                    while ((RCC->RCC_CR & RCC_CR_PLLRDY) == 0 && deadlock-- != 0) arch_delay_us(1ul);
                    picoRTOS_assert(deadlock != -1, return -EBUSY);
                    /* remember */
                    clocks.pll_p = (clock_freq_t)settings->pll_p;
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

/* Function: clock_stm32f401x_init
 * Initialises the stm32f401x clock system
 *
 * Parameters:
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_stm32f401x_init(struct clock_settings *settings)
{
    picoRTOS_assert(settings->sysclk < CLOCK_STM32F401X_SYSCLK_COUNT, return -EINVAL);
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
    case CLOCK_STM32F401X_SYSCLK_HSI:
        clocks.sysclk = (clock_freq_t)HSI_FREQ;
        break;

    case CLOCK_STM32F401X_SYSCLK_PLL:
        picoRTOS_assert(settings->pll_p != 0, return -EINVAL);
        clocks.sysclk = clocks.pll_p;
        break;

    case CLOCK_STM32F401X_SYSCLK_HSE:
        picoRTOS_assert(settings->hse != 0, return -EINVAL);
        clocks.sysclk = (clock_freq_t)settings->hse;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    /* flash */
    FLASH->FLASH_ACR &= ~FLASH_ACR_LATENCY(FLASH_ACR_LATENCY_M);
    if (clocks.sysclk <= (clock_freq_t)24000000) {
    }else if (clocks.sysclk <= (clock_freq_t)48000000)
        FLASH->FLASH_ACR |= FLASH_ACR_LATENCY(1);
    else
        FLASH->FLASH_ACR |= FLASH_ACR_LATENCY(2);

    /* prescalers */
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

int clock_stm32f401x_enable(clock_stm32f401x_clk_t clk)
{
    picoRTOS_assert(clk < CLOCK_STM32F401X_CLK_COUNT, return -EINVAL);

    int bus = (int)(clk >> 5);
    uint32_t mask = (uint32_t)(1 << (0x1fu & clk));

    switch (bus) {
    case 0: RCC->RCC_AHB1ENR |= mask; break;    /* AHB1 */
    case 1: RCC->RCC_AHB2ENR |= mask; break;    /* AHB2 */
    case 2: RCC->RCC_APB1ENR |= mask; break;    /* APB1 */
    case 3: RCC->RCC_APB2ENR |= mask; break;    /* APB2 */
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

int clock_stm32f401x_disable(clock_stm32f401x_clk_t clk)
{
    picoRTOS_assert(clk < CLOCK_STM32F401X_CLK_COUNT, return -EINVAL);

    int bus = (int)(clk >> 5);
    uint32_t mask = (uint32_t)(1 << (0x1fu & clk));

    switch (bus) {
    case 0: RCC->RCC_AHB1ENR &= ~mask; break;   /* AHB1 */
    case 1: RCC->RCC_AHB2ENR &= ~mask; break;   /* AHB2 */
    case 2: RCC->RCC_APB1ENR &= ~mask; break;   /* APB2 */
    case 3: RCC->RCC_APB2ENR &= ~mask; break;   /* APB1 */
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    switch (clkid) {
    case CLOCK_STM32F401X_SYSCLK: return clocks.sysclk;
    case CLOCK_STM32F401X_I2SCLK: /*@fallthrough@*/
    case CLOCK_STM32F401X_USBCLK: return (clock_freq_t)USB_FREQ;
    case CLOCK_STM32F401X_HCLK: return clocks.ahb;
    case CLOCK_STM32F401X_APB1: return clocks.apb1;
    case CLOCK_STM32F401X_APB2: return clocks.apb2;
    case CLOCK_STM32F401X_RTCCLK: return (clock_freq_t)LSE_FREQ;    /* FIXME */
    case CLOCK_STM32F401X_IWDGCLK: return (clock_freq_t)LSI_FREQ;
    default: break;
    }

    /* unknown */
    picoRTOS_break();
    /*@notreached@*/ return (clock_freq_t)-EIO;
}
