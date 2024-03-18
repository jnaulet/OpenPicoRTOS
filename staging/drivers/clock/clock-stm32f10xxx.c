#include "clock-stm32f10xxx.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct RCC {
    volatile uint32_t RCC_CR;
    volatile uint32_t RCC_CFGR;
    volatile uint32_t RCC_CIR;
    volatile uint32_t RCC_APB2RSTR;
    volatile uint32_t RCC_APB1RSTR;
    volatile uint32_t RCC_AHBENR;
    volatile uint32_t RCC_APB2ENR;
    volatile uint32_t RCC_APB1ENR;
    volatile uint32_t RCC_BDCR;
    volatile uint32_t RCC_CSR;
    volatile uint32_t RCC_AHBRSTR;
    volatile uint32_t RCC_CFGR2;
};

#define RCC_CR_PLL3RDY    (1 << 29)
#define RCC_CR_PLL3ON     (1 << 28)
#define RCC_CR_PLL2RDY    (1 << 27)
#define RCC_CR_PLL2ON     (1 << 26)
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
#define RCC_CR_HSION      (1 << 0

#define RCC_CFGR_MCO_M     0xfu
#define RCC_CFGR_MCO(x)    (((x) & RCC_CFGR_MCO_M) << 24)
#define RCC_CFGR_OTGFSPRE  (1 << 22)
#define RCC_CFGR_USBPRE    (1 << 22)
#define RCC_CFGR_PLLMUL_M  0xfu
#define RCC_CFGR_PLLMUL(x) (((x) & RCC_CFGR_PLLMUL_M) << 18)
#define RCC_CFGR_PLLXTPRE  (1 << 17)
#define RCC_CFGR_PLLSRC    (1 << 16)
#define RCC_CFGR_ADCPRE_M  0x3u
#define RCC_CFGR_ADCPRE(x) (((x) & RCC_CFGR_ADCPRE_M) << 14)
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

#define RCC_CIR_CSSC      (1 << 23)
#define RCC_CIR_PLL3RDYC  (1 << 22)
#define RCC_CIR_PLL2RDYC  (1 << 21)
#define RCC_CIR_PLLRDYC   (1 << 20)
#define RCC_CIR_HSERDYC   (1 << 19)
#define RCC_CIR_HSIRDYC   (1 << 18)
#define RCC_CIR_LSERDYC   (1 << 17)
#define RCC_CIR_LSIRDYC   (1 << 16)
#define RCC_CIR_PLL3RDYIE (1 << 14)
#define RCC_CIR_PLL2RDYIE (1 << 13)
#define RCC_CIR_PLLRDYIE  (1 << 12)
#define RCC_CIR_HSERDYIE  (1 << 11)
#define RCC_CIR_HSIRDYIE  (1 << 10)
#define RCC_CIR_LSERDYIE  (1 << 9)
#define RCC_CIR_LSIRDYIE  (1 << 8)
#define RCC_CIR_CSSF      (1 << 7)
#define RCC_CIR_PLL3RDYF  (1 << 6)
#define RCC_CIR_PLL2RDYF  (1 << 5)
#define RCC_CIR_PLLRDYF   (1 << 4)
#define RCC_CIR_HSERDYF   (1 << 3)
#define RCC_CIR_HSIRDYF   (1 << 2)
#define RCC_CIR_LSERDYF   (1 << 1)
#define RCC_CIR_LSIRDYF   (1 << 0)

#define RCC_APB2RSTR_USART1RST (1 << 14)
#define RCC_APB2RSTR_SPI1RST   (1 << 12)
#define RCC_APB2RSTR_TIM1RST   (1 << 11)
#define RCC_APB2RSTR_ADC2RST   (1 << 10)
#define RCC_APB2RSTR_ADC1RST   (1 << 9)
#define RCC_APB2RSTR_IOPERST   (1 << 6)
#define RCC_APB2RSTR_IOPDRST   (1 << 5)
#define RCC_APB2RSTR_IOPCRST   (1 << 4)
#define RCC_APB2RSTR_IOPBRST   (1 << 3)
#define RCC_APB2RSTR_IOPARST   (1 << 2)
#define RCC_APB2RSTR_AFIORST   (1 << 0)

#define RCC_APB1RSTR_DACRST    (1 << 29)
#define RCC_APB1RSTR_PWRRST    (1 << 28)
#define RCC_APB1RSTR_BKPRST    (1 << 27)
#define RCC_APB1RSTR_CAN2RST   (1 << 26)
#define RCC_APB1RSTR_CAN1RST   (1 << 25)
#define RCC_APB1RSTR_I2C2RST   (1 << 22)
#define RCC_APB1RSTR_I2C1RST   (1 << 21)
#define RCC_APB1RSTR_UART5RST  (1 << 20)
#define RCC_APB1RSTR_UART4RST  (1 << 19)
#define RCC_APB1RSTR_USART3RST (1 << 18)
#define RCC_APB1RSTR_USART2RST (1 << 17)
#define RCC_APB1RSTR_SPI3RST   (1 << 15)
#define RCC_APB1RSTR_SPI2RST   (1 << 14)
#define RCC_APB1RSTR_WWDGRST   (1 << 11)
#define RCC_APB1RSTR_TIM7RST   (1 << 5)
#define RCC_APB1RSTR_TIM6RST   (1 << 4)
#define RCC_APB1RSTR_TIM5RST   (1 << 3)
#define RCC_APB1RSTR_TIM4RST   (1 << 2)
#define RCC_APB1RSTR_TIM3RST   (1 << 1)
#define RCC_APB1RSTR_TIM2RST   (1 << 0)

#define RCC_AHBENR_SDIOEN     (1 << 10)
#define RCC_AHBENR_FSMCEN     (1 << 8)
#define RCC_AHBENR_CRCEN      (1 << 6)
#define RCC_AHBENR_FLITFEN    (1 << 4)
#define RCC_AHBENR_SRAMEN     (1 << 2)
#define RCC_AHBENR_DAM2EN     (1 << 1)
#define RCC_AHBENR_DMA1EN     (1 << 0)

#define RCC_APB2ENR_TIM11EN  (1 << 21)
#define RCC_APB2ENR_TIM10EN  (1 << 20)
#define RCC_APB2ENR_TIM9EN   (1 << 19)
#define RCC_APB2ENR_ADC3EN   (1 << 15)
#define RCC_APB2ENR_USART1EN (1 << 14)
#define RCC_APB2ENR_TIM8EN   (1 << 13)
#define RCC_APB2ENR_SPI1EN   (1 << 12)
#define RCC_APB2ENR_TIM1EN   (1 << 11)
#define RCC_APB2ENR_ADC2EN   (1 << 10)
#define RCC_APB2ENR_ADC1EN   (1 << 9)
#define RCC_APB2ENR_IOPGEN   (1 << 8)
#define RCC_APB2ENR_IOPFEN   (1 << 7)
#define RCC_APB2ENR_IOPEEN   (1 << 6)
#define RCC_APB2ENR_IOPDEN   (1 << 5)
#define RCC_APB2ENR_IOPCEN   (1 << 4)
#define RCC_APB2ENR_IOPBEN   (1 << 3)
#define RCC_APB2ENR_IOPAEN   (1 << 2)
#define RCC_APB2ENR_AFIOEN   (1 << 0)

#define RCC_APB1ENR_DACEN    (1 << 29)
#define RCC_APB1ENR_PWREN    (1 << 28)
#define RCC_APB1ENR_BKPEN    (1 << 27)
#define RCC_APB1ENR_CAN2EN   (1 << 26)
#define RCC_APB1ENR_CAN1EN   (1 << 25)
#define RCC_APB1ENR_I2C2EN   (1 << 22)
#define RCC_APB1ENR_I2C1EN   (1 << 21)
#define RCC_APB1ENR_UART5EN  (1 << 20)
#define RCC_APB1ENR_UART4EN  (1 << 19)
#define RCC_APB1ENR_USART3EN (1 << 18)
#define RCC_APB1ENR_USART2EN (1 << 17)
#define RCC_APB1ENR_SPI3EN   (1 << 15)
#define RCC_APB1ENR_SPI2EN   (1 << 14)
#define RCC_APB1ENR_WWDGEN   (1 << 11)
#define RCC_APB1ENR_TIM7EN   (1 << 5)
#define RCC_APB1ENR_TIM6EN   (1 << 4)
#define RCC_APB1ENR_TIM5EN   (1 << 3)
#define RCC_APB1ENR_TIM4EN   (1 << 2)
#define RCC_APB1ENR_TIM3EN   (1 << 1)
#define RCC_APB1ENR_TIM2EN   (1 << 0)

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

#define RCC_AHBRSTR_ETHMACRST (1 << 14)
#define RCC_AHBRSTR_OTGFSRST  (1 << 12)

#define RCC_CFGR2_I2S3SRC    (1 << 18)
#define RCC_CFGR2_I2S2SRC    (1 << 17)
#define RCC_CFGR2_PREDIV1SRC (1 << 16)
#define RCC_CFGR2_PLL3MUL_M  0xfu
#define RCC_CFGR2_PLL3MUL(x) (((x) & RCC_CFGR2_PLL3MUL_M) << 12)
#define RCC_CFGR2_PLL2MUL_M  0xfu
#define RCC_CFGR2_PLL2MUL(x) (((x) & RCC_CFGR2_PLL2MUL_M) << 8)
#define RCC_CFGR2_PREDIV2_M  0xfu
#define RCC_CFGR2_PREDIV2(x) (((x) & RCC_CFGR2_PREDIV2_M) << 4)
#define RCC_CFGR2_PREDIV1_M  0xfu
#define RCC_CFGR2_PREDIV1(x) ((x) & RCC_CFGR2_PREDIV1_M)

struct FLASH {
    volatile uint32_t FLASH_ACR;
    /* ... */
};

#define FLASH_ACR_PRFTBS     (1 << 5)
#define FLASH_ACR_PRFTBE     (1 << 4)
#define FLASH_ACR_HLFCYA     (1 << 3)
#define FLASH_ACR_LATENCY_M  0x7u
#define FLASH_ACR_LATENCY(x) ((x) & FLASH_ACR_LATENCY_M)

/* instances */
static struct RCC *RCC = (struct RCC*)ADDR_RCC;
static struct FLASH *FLASH = (struct FLASH*)ADDR_FMI;

#define HSI_FREQ 8000000
#define LSE_FREQ 32768
#define LSI_FREQ 40000
#define USB_FREQ 48000000

static struct {
    clock_freq_t pll;
    clock_freq_t sysclk;
    clock_freq_t ahb;
    clock_freq_t apb1;
    clock_freq_t apb2;
    clock_freq_t adc;
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
#define PLL_MAX 72000000ul

    picoRTOS_assert(settings->pllsrc < CLOCK_STM32F10XXX_PLLSRC_COUNT, return -EINVAL);
    picoRTOS_assert(settings->pll <= PLL_MAX, return -EINVAL);

    unsigned long in;
    unsigned long pllmul;

    /* disable */
    if (settings->pll == 0) {
        RCC->RCC_CR &= ~RCC_CR_PLLON;
        return 0;
    }

    /* input clk frequency */
    switch (settings->pllsrc) {
    case CLOCK_STM32F10XXX_PLLSRC_HSI_DIV2:
        RCC->RCC_CFGR &= ~RCC_CFGR_PLLSRC;
        in = (unsigned long)HSI_FREQ >> 1;
        break;

    case CLOCK_STM32F10XXX_PLLSRC_HSE:
        picoRTOS_assert(settings->hse != 0, return -EINVAL);
        RCC->RCC_CFGR = (uint32_t)(RCC->RCC_CFGR & ~RCC_CFGR_PLLXTPRE) | RCC_CFGR_PLLSRC;
        in = settings->hse;
        break;

    case CLOCK_STM32F10XXX_PLLSRC_HSE_DIV2:
        picoRTOS_assert(settings->hse != 0, return -EINVAL);
        RCC->RCC_CFGR |= (RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE);
        in = settings->hse >> 1;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    /* setup */

    /* pll multiplier */
    for (pllmul = 2ul; pllmul <= 16ul; pllmul++) {
        /* found */
        if ((in * pllmul) == settings->pll) {

            int deadlock = CONFIG_DEADLOCK_COUNT;

            /* setup */
            RCC->RCC_CFGR &= ~RCC_CFGR_PLLMUL(RCC_CFGR_PLLMUL_M);
            RCC->RCC_CFGR |= RCC_CFGR_PLLMUL(pllmul - 2ul);
            /* turn on */
            RCC->RCC_CR |= RCC_CR_PLLON;
            /* wait */
            while ((RCC->RCC_CR & RCC_CR_PLLRDY) == 0 && deadlock-- != 0) arch_delay_us(1ul);
            picoRTOS_assert(deadlock != -1, return -EBUSY);
            /* remember */
            clocks.pll = (clock_freq_t)settings->pll;
            return 0;
        }
    }

    /* something went wrong */
    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int setup_usb(void)
{
    if (clocks.pll > (clock_freq_t)USB_FREQ)
        RCC->RCC_CFGR |= RCC_CFGR_USBPRE;

    return 0;
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
    picoRTOS_assert(clocks.apb1 <= (clock_freq_t)36000000, return -EINVAL);
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

static int set_adcpre(unsigned int prescaler)
{
    picoRTOS_assert(prescaler != 0, return -EINVAL);
    picoRTOS_assert(prescaler <= 8u, return -EINVAL);

    RCC->RCC_CFGR &= ~RCC_CFGR_ADCPRE(RCC_CFGR_ADCPRE_M);

    switch (prescaler) {
    case 2: RCC->RCC_CFGR |= RCC_CFGR_ADCPRE(0); break;
    case 4: RCC->RCC_CFGR |= RCC_CFGR_ADCPRE(1); break;
    case 8: RCC->RCC_CFGR |= RCC_CFGR_ADCPRE(2); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    clocks.adc = clocks.apb2 / (clock_freq_t)prescaler;
    return 0;
}

/* Function: clock_stm32f10xxx_init
 * Initialises the STM32f10xxx clock system
 *
 * Parameters:
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_stm32f10xxx_init(struct clock_settings *settings)
{
    picoRTOS_assert(settings->sysclk < CLOCK_STM32F10XXX_SYSCLK_COUNT, return -EINVAL);
    picoRTOS_assert(settings->ahb_prescaler != 0, return -EINVAL);
    picoRTOS_assert(settings->apb1_prescaler != 0, return -EINVAL);
    picoRTOS_assert(settings->apb2_prescaler != 0, return -EINVAL);
    picoRTOS_assert(settings->adc_prescaler > 1u, return -EINVAL);

    int res;
    int deadlock = CONFIG_DEADLOCK_COUNT;

    if ((res = setup_hse(settings->hse)) < 0 ||
        (res = setup_pll(settings)) < 0 ||
        (res = setup_usb()) < 0)
        return res;

    /* prepare sysclk future values */
    switch (settings->sysclk) {
    case CLOCK_STM32F10XXX_SYSCLK_HSI:
        clocks.sysclk = (clock_freq_t)HSI_FREQ;
        break;

    case CLOCK_STM32F10XXX_SYSCLK_PLL:
        picoRTOS_assert(settings->pll != 0, return -EINVAL);
        clocks.sysclk = clocks.pll;
        break;

    case CLOCK_STM32F10XXX_SYSCLK_HSE:
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
        (res = set_ppre2(settings->apb2_prescaler)) < 0 ||
        (res = set_adcpre(settings->adc_prescaler)) < 0)
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

int clock_stm32f10xxx_enable(clock_stm32f10xxx_clk_t clk)
{
    picoRTOS_assert(clk < CLOCK_STM32F10XXX_CLK_COUNT, return -EINVAL);

    int bus = (int)(clk >> 5);
    uint32_t mask = (uint32_t)(1 << (0x1fu & clk));

    switch (bus) {
    case 0: RCC->RCC_AHBENR |= mask; break;     /* AHB */
    case 1: RCC->RCC_APB2ENR |= mask; break;    /* APB2 */
    case 2: RCC->RCC_APB1ENR |= mask; break;    /* APB1 */
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

int clock_stm32f10xxx_disable(clock_stm32f10xxx_clk_t clk)
{
    picoRTOS_assert(clk < CLOCK_STM32F10XXX_CLK_COUNT, return -EINVAL);

    int bus = (int)(clk >> 5);
    uint32_t mask = (uint32_t)(1 << (0x1fu & clk));

    switch (bus) {
    case 0: RCC->RCC_AHBENR &= ~mask; break;    /* AHB */
    case 1: RCC->RCC_APB2ENR &= ~mask; break;   /* APB2 */
    case 2: RCC->RCC_APB1ENR &= ~mask; break;   /* APB1 */
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    switch (clkid) {
    case CLOCK_STM32F10XXX_I2S3CLK: /*@fallthrough@*/
    case CLOCK_STM32F10XXX_I2S2CLK: /*@fallthrough@*/
    case CLOCK_STM32F10XXX_SYSCLK: return clocks.sysclk;
    case CLOCK_STM32F10XXX_FLITFCLK: return (clock_freq_t)HSI_FREQ;
    case CLOCK_STM32F10XXX_USBCLK: return (clock_freq_t)USB_FREQ;
    case CLOCK_STM32F10XXX_SDIOCLK: /*@fallthrough@*/
    case CLOCK_STM32F10XXX_FSMCCLK: /*@fallthrough@*/
    case CLOCK_STM32F10XXX_HCLK:    /*@fallthrough@*/
    case CLOCK_STM32F10XXX_FCLK: return clocks.ahb;
    case CLOCK_STM32F10XXX_PCLK1: return clocks.apb1;
    case CLOCK_STM32F10XXX_TIMXCLK: return clocks.apb1 / 2;         /* FIXME */
    case CLOCK_STM32F10XXX_PCLK2:  return clocks.apb2;
    case CLOCK_STM32F10XXX_TIMxCLK: return clocks.apb1 / 2;         /* FIXME */
    case CLOCK_STM32F10XXX_ADCCLK: return clocks.adc;
    case CLOCK_STM32F10XXX_RTCCLK: return (clock_freq_t)LSE_FREQ;   /* FIXME */
    case CLOCK_STM32F10XXX_IWDGCLK: return (clock_freq_t)LSI_FREQ;
    default: break;
    }

    /* unknown */
    picoRTOS_break();
    /*@notreached@*/ return (clock_freq_t)-EIO;
}
