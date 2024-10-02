#include "clock-stm8.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct CLK {
    volatile uint8_t ICKR;
    volatile uint8_t ECKR;
    uint8_t RESERVED0;
    volatile uint8_t CMSR;
    volatile uint8_t SWR;
    volatile uint8_t SWCR;
    volatile uint8_t CKDIVR;
    volatile uint8_t PCKENR1;
    volatile uint8_t CSSR;
    volatile uint8_t CCOR;
    volatile uint8_t PCKENR2;
    uint8_t RESERVED1;
    volatile uint8_t HSITRIMR;
    volatile uint8_t SWIMCCR;
};

#define ICKR_REGAH  (1 << 5)
#define ICKR_LSIRDY (1 << 4)
#define ICKR_LSIEN  (1 << 3)
#define ICKR_FHW    (1 << 2)
#define ICKR_HSIRDY (1 << 1)
#define ICKR_HSIEN  (1 << 0)

#define C99_ECKR_HSERDY (1 << 1)
#define C99_ECKR_HSEEN  (1 << 0)

#define CKM_SWI_HSI 0xe1
#define CKM_SWI_LSI 0xd2
#define CKM_SWI_HSE 0xb4

#define SWR_SWI_M  0xffu
#define SWR_SWI(x) ((x) & SWR_SWI_M)

#define SWCR_SWIF  (1 << 3)
#define SWCR_SWIEN (1 << 2)
#define SWCR_SWEN  (1 << 1)
#define SWCR_SWBSY (1 << 0)

#define CKDIVR_HSIDIV_M 0x3u
#define CKDIVR_HSIDIV(x) (((x) & CKDIVR_HSIDIV_M) << 3)
#define CKDIVR_CPUDIV_M  0x7u
#define CKDIVR_CPUDIV(x) ((x) & CKDIVR_CPUDIV_M)

#define CSSR_CSSD   (1 << 3)
#define CSSR_CSSDIE (1 << 2)
#define CSSR_AUX    (1 << 1)
#define CSSR_CSSEN  (1 << 0)

#define CCOR_CCOBSY    (1 << 6)
#define CCOR_CCORDY    (1 << 5)
#define CCOR_CCOSEL_M  0xfu
#define CCOR_CCOSEL(x) (((x) & CCOR_CCOSEL_M) << 1)
#define CCOR_CCOEN     (1 << 0)

#define HSITRIM_HSITRIM_M  0xfu
#define HSITRIM_HSITRIM(x) ((x) & HSITRIM_HSITRIM_M)

#define SWIMCCR_SWIMCLK (1 << 0)

/* global */
static struct CLK *CLK = (struct CLK*)ADDR_CLK;

#define HSI_FREQ_HZ 16000000ul
#define LSI_FREQ_HZ 128000ul

static struct {
    clock_freq_t master;
    clock_freq_t cpu;
} clocks;

static int switch_to_hsi(clock_stm8_hsidiv_t hsidiv)
{
    picoRTOS_assert(hsidiv < CLOCK_STM8_HSIDIV_COUNT, return -EINVAL);

    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* enable hsi */
    CLK->ICKR |= ICKR_HSIEN;
    while ((CLK->ICKR & ICKR_HSIRDY) == 0 &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    deadlock = CONFIG_DEADLOCK_COUNT;

    /* switch clock */
    CLK->SWR = (uint8_t)SWR_SWI(CKM_SWI_HSI);
    while (CLK->CMSR != (uint8_t)CKM_SWI_HSI &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    /* set divider */
    CLK->CKDIVR &= ~CKDIVR_HSIDIV(CKDIVR_HSIDIV_M);
    CLK->CKDIVR |= CKDIVR_HSIDIV(hsidiv);

    clocks.master = (clock_freq_t)(HSI_FREQ_HZ / (1ul << (unsigned)hsidiv));
    return 0;
}

static int switch_to_hse(unsigned long hse)
{
    picoRTOS_assert(hse >= 1000000ul, return -EINVAL);
    picoRTOS_assert(hse <= 24000000ul, return -EINVAL);

    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* enable hse */
    CLK->ECKR |= C99_ECKR_HSEEN;
    while ((CLK->ECKR & C99_ECKR_HSERDY) == 0 &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    deadlock = CONFIG_DEADLOCK_COUNT;

    /* switch clock */
    CLK->SWR = (uint8_t)SWR_SWI(CKM_SWI_HSE);
    while (CLK->CMSR != (uint8_t)CKM_SWI_HSE &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    clocks.master = (clock_freq_t)hse;
    return 0;
}

static int switch_to_lsi(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* enable LSI */
    CLK->ICKR |= ICKR_LSIEN;
    while ((CLK->ICKR & ICKR_LSIRDY) == 0 &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    deadlock = CONFIG_DEADLOCK_COUNT;

    /* switch clock */
    CLK->SWR = (uint8_t)SWR_SWI(CKM_SWI_LSI);
    while (CLK->CMSR != (uint8_t)CKM_SWI_LSI &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    clocks.master = (clock_freq_t)LSI_FREQ_HZ;
    return 0;
}

int clock_stm8_init(struct clock_settings *settings)
{
    picoRTOS_assert(settings->ckm < CLOCK_STM8_CKM_COUNT, return -EINVAL);
    picoRTOS_assert(settings->cpudiv < CLOCK_STM8_CPUDIV_COUNT, return -EINVAL);

    int res = 0;

    switch (settings->ckm) {
    case CLOCK_STM8_CKM_HSI:
        res = switch_to_hsi(settings->hsidiv);
        break;

    case CLOCK_STM8_CKM_HSE:
        res = switch_to_hse(settings->hse);
        break;

    case CLOCK_STM8_CKM_LSI:
        res = switch_to_lsi();
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    if (res < 0)
        return res;

    /* set cpu div */
    CLK->CKDIVR &= ~CKDIVR_CPUDIV(CKDIVR_CPUDIV_M);
    CLK->CKDIVR |= CKDIVR_CPUDIV(settings->cpudiv);

    clocks.cpu = clocks.master / (clock_freq_t)(1ul << (unsigned)settings->cpudiv);
    arch_set_clock_frequency((unsigned long)clocks.cpu);

    return 0;
}

int clock_stm8_pck_enable(clock_pcken_t pck)
{
    picoRTOS_assert(pck < CLOCK_PCKEN_COUNT, return -EINVAL);

    if (pck <= CLOCK_PCKEN17) CLK->PCKENR1 |= (1 << (unsigned)pck);
    else CLK->PCKENR2 |= (1 << ((unsigned)pck & 0x7u));

    return 0;
}

int clock_stm8_pck_disable(clock_pcken_t pck)
{
    picoRTOS_assert(pck < CLOCK_PCKEN_COUNT, return -EINVAL);

    if (pck <= CLOCK_PCKEN17) CLK->PCKENR1 &= ~(1 << (unsigned)pck);
    else CLK->PCKENR2 &= ~(1 << ((unsigned)pck & 0x7u));

    return 0;
}

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    picoRTOS_assert(clkid <= CLOCK_STM8_LSI, return (clock_freq_t)-EINVAL);

    switch (clkid) {
    case CLOCK_STM8_CPU: return clocks.cpu;
    case CLOCK_STM8_MASTER: return clocks.master;
    case CLOCK_STM8_LSI: return (clock_freq_t)LSI_FREQ_HZ;
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return (clock_freq_t)-EIO;
}
