#include "clock-ch32v003.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct RCC {
    volatile uint32_t CTLR;
    volatile uint32_t CFGR0;
    volatile uint32_t INTR;
    volatile uint32_t APB2PRSTR;
    volatile uint32_t APB1PRSTR;
    volatile uint32_t AHBPCENR;
    volatile uint32_t APB2PCENR;
    volatile uint32_t APB1PCENR;
    uint32_t RESERVED0;
    volatile uint32_t RSTSCKR;
};

#define CTLR_PLLRDY     (1 << 25)
#define CTLR_PLLON      (1 << 24)
#define CTLR_CSSON      (1 << 19)
#define CTLR_HSEBYP     (1 << 18)
#define CTLR_HSERDY     (1 << 17)
#define CTLR_HSEON      (1 << 16)
#define CTLR_HSICAL_M   0xffu
#define CTLR_HSICAL(x)  (((x) & CTLR_HSICAL_M) << 8)
#define CTLR_HSITRIM_M  0x1fu
#define CTLR_HSITRIM(x) (((x) & CTLR_HSITRIM_M) << 3)
#define CTLR_HSIRDY     (1 << 1)
#define CTLR_HSION      (1 << 0)

#define CFGR0_MCO_M     0x7u
#define CFGR0_MCO(x)    (((x) & CFGR0_MCO_M) << 24)
#define CFGR0_PLLSRC    (1 << 16)
#define CFGR0_ADCPRE_M  0x1fu
#define CFGR0_ADCPRE(x) (((x) & CFGR0_ADCPRE_M) << 11)
#define CFGR0_HPRE_M    0xfu
#define CFGR0_HPRE(x)   (((x) & CFGR0_HPRE_M) << 4)
#define CFGR0_SWS_M     0x3u
#define CFGR0_SWS(x)    (((x) & CFGR0_SWS_M) << 2)
#define CFGR0_SW_M      0x3u
#define CFGR0_SW(x)     ((x) & CFGR0_SW_M)

/* instanciate */
static struct RCC *RCC = (struct RCC*)ADDR_RCC;

#define LSI_FREQ 1280000
#define HSI_FREQ 24000000

static struct {
    clock_freq_t hse;
    clock_freq_t pll;
    clock_freq_t sysclk;
    clock_freq_t hclk;
    clock_freq_t adcclk;
} clocks;

static int turn_hse_on(unsigned long hz)
{
    picoRTOS_assert(hz >= 4000000ul, return -EINVAL);
    picoRTOS_assert(hz <= 25000000ul, return -EINVAL);

    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* turn bit on */
    RCC->CTLR |= CTLR_HSEON;
    clocks.hse = (clock_freq_t)hz;

    /* wait for rdy */
    while ((RCC->CTLR & CTLR_HSERDY) == 0 &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int turn_pll_on(clock_ch32v003_pllsrc_t pllsrc)
{
    picoRTOS_assert(pllsrc < CLOCK_CH32V003_PLLSRC_COUNT, return -EINVAL);

    int deadlock = CONFIG_DEADLOCK_COUNT;

    switch (pllsrc) {
    case CLOCK_CH32V003_PLLSRC_HSI:
        RCC->CFGR0 &= ~CFGR0_PLLSRC;
        clocks.pll = (clock_freq_t)(HSI_FREQ * 2);
        break;

    case CLOCK_CH32V003_PLLSRC_HSE:
        RCC->CFGR0 |= CFGR0_PLLSRC;
        clocks.pll = clocks.hse * (clock_freq_t)2;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    /* turn PLL on */
    RCC->CTLR |= CTLR_PLLON;
    /* wait for rdy */
    while ((RCC->CTLR & CTLR_PLLRDY) == 0 &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int switch_to(clock_ch32v003_sysclk_t sysclk)
{
    picoRTOS_assert(sysclk < CLOCK_CH32V003_SYSCLK_COUNT, return -EINVAL);

    int deadlock = CONFIG_DEADLOCK_COUNT;
    uint32_t cfgr0 = (uint32_t)(RCC->CFGR0 & ~CFGR0_SW(CFGR0_SW_M));

    /* switch clock  */
    RCC->CFGR0 = (uint32_t)(cfgr0 | CFGR0_SW(sysclk));
    /* wait for transition complete */
    while ((RCC->CFGR0 & CFGR0_SWS(CFGR0_SWS_M)) != CFGR0_SWS(sysclk) &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    switch (sysclk) {
    case CLOCK_CH32V003_SYSCLK_HSI: clocks.sysclk = (clock_freq_t)HSI_FREQ; break;
    case CLOCK_CH32V003_SYSCLK_HSE: clocks.sysclk = clocks.hse; break;
    case CLOCK_CH32V003_SYSCLK_PLLCLK: clocks.sysclk = clocks.pll; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

static int div_hclk(unsigned long hclk_div)
{
    picoRTOS_assert(hclk_div > 0ul, return -EINVAL);
    picoRTOS_assert(hclk_div <= 256ul, return -EINVAL);

    uint32_t cfgr0 = (uint32_t)(RCC->CFGR0 & ~CFGR0_HPRE(CFGR0_HPRE_M));

    switch (hclk_div) {
    case 1: RCC->CFGR0 = (uint32_t)(cfgr0 | CFGR0_HPRE(0)); break;
    case 2: RCC->CFGR0 = (uint32_t)(cfgr0 | CFGR0_HPRE(1)); break;
    case 3: RCC->CFGR0 = (uint32_t)(cfgr0 | CFGR0_HPRE(2)); break;
    case 4: RCC->CFGR0 = (uint32_t)(cfgr0 | CFGR0_HPRE(3)); break;
    case 5: RCC->CFGR0 = (uint32_t)(cfgr0 | CFGR0_HPRE(4)); break;
    case 6: RCC->CFGR0 = (uint32_t)(cfgr0 | CFGR0_HPRE(5)); break;
    case 7: RCC->CFGR0 = (uint32_t)(cfgr0 | CFGR0_HPRE(6)); break;
    case 8: RCC->CFGR0 = (uint32_t)(cfgr0 | CFGR0_HPRE(7)); break;
    case 16: RCC->CFGR0 = (uint32_t)(cfgr0 | CFGR0_HPRE(11)); break;
    case 32: RCC->CFGR0 = (uint32_t)(cfgr0 | CFGR0_HPRE(12)); break;
    case 64: RCC->CFGR0 = (uint32_t)(cfgr0 | CFGR0_HPRE(13)); break;
    case 128: RCC->CFGR0 = (uint32_t)(cfgr0 | CFGR0_HPRE(14)); break;
    case 256: RCC->CFGR0 = (uint32_t)(cfgr0 | CFGR0_HPRE(15)); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    clocks.hclk = clocks.sysclk / (clock_freq_t)hclk_div;
    arch_set_clock_frequency((unsigned long)clocks.hclk);
    return 0;
}

int clock_ch32v003_init(const struct clock_settings *settings)
{
    picoRTOS_assert(settings->sysclk < CLOCK_CH32V003_SYSCLK_COUNT, return -EINVAL);
    picoRTOS_assert(settings->pllsrc < CLOCK_CH32V003_PLLSRC_COUNT, return -EINVAL);

    int res;

    /* default values */
    clocks.sysclk = (clock_freq_t)HSI_FREQ;

    /* hse must be on */
    if (settings->sysclk == CLOCK_CH32V003_SYSCLK_HSE ||
        settings->pllsrc == CLOCK_CH32V003_PLLSRC_HSE) {
        /* turn on */
        if ((res = turn_hse_on(settings->hse)) < 0)
            return res;
    }

    /* pll */
    if (settings->sysclk == CLOCK_CH32V003_SYSCLK_PLLCLK &&
        (res = turn_pll_on(settings->pllsrc)) < 0)
        return res;

    /* switch anyway */
    if ((res = switch_to(settings->sysclk)) < 0)
        return res;

    return div_hclk(settings->hclk_div);
}

int clock_ch32v003_enable(clock_ch32v003_clk_t clk)
{
    picoRTOS_assert(clk < CLOCK_CH32V003_CLK_COUNT, return -EINVAL);

    int bus = (int)(clk >> 5);
    uint32_t mask = (uint32_t)(1 << (0x1fu & clk));

    switch (bus) {
    case 0: RCC->AHBPCENR |= mask; break;       /* AHB */
    case 1: RCC->APB2PCENR |= mask; break;      /* APB2 */
    case 2: RCC->APB1PCENR |= mask; break;      /* APB1 */
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

int clock_ch32v003_disable(clock_ch32v003_clk_t clk)
{
    picoRTOS_assert(clk < CLOCK_CH32V003_CLK_COUNT, return -EINVAL);

    int bus = (int)(clk >> 5);
    uint32_t mask = (uint32_t)(1 << (0x1fu & clk));

    switch (bus) {
    case 0: RCC->AHBPCENR &= ~mask; break;      /* AHB */
    case 1: RCC->APB2PCENR &= ~mask; break;     /* APB2 */
    case 2: RCC->APB1PCENR &= ~mask; break;     /* APB1 */
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    switch (clkid) {
    case CLOCK_CH32V003_HCLK: return clocks.hclk;
    case CLOCK_CH32V003_IWDGCLK: return (clock_freq_t)LSI_FREQ;
    case CLOCK_CH32V003_WWDGCLK: return clocks.hclk / (clock_freq_t)4096;
    case CLOCK_CH32V003_ADCCLK: /*@fallthrough@*/
    default: break;
    }

    /* unknown */
    picoRTOS_break();
    /*@notreached@*/ return (clock_freq_t)-EIO;
}
