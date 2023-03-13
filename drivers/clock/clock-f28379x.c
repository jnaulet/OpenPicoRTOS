#include "clock-f28379x.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <errno.h>
#include <stdint.h>
#include <stddef.h>

#define PLL_LOCK_COUNT 5

struct CLK_CFG_REGS {
    volatile uint32_t CLKSEM;
    volatile uint32_t CLKCFGLOCK1;
    uint16_t RESERVED0[4];
    volatile uint32_t CLKSRCCTL1;
    volatile uint32_t CLKSRCCTL2;
    volatile uint32_t CLKSRCCTL3;
    volatile uint32_t SYSPLLCTL1;
    uint16_t RESERVED1[4];
    volatile uint32_t SYSPLLMULT;
    volatile uint32_t SYSPLLSTS;
    volatile uint32_t AUXPLLCTL1;
    uint16_t RESERVED2[4];
    volatile uint32_t AUXPLLMULT;
    volatile uint32_t AUXPLLSTS;
    volatile uint32_t SYSCLKDIVSEL;
    volatile uint32_t AUXCLKDIVSEL;
    volatile uint32_t PERCLKDIVSEL;
    volatile uint32_t XCLKOUTDIVSEL;
    volatile uint32_t CLBCLKCTL;
    volatile uint32_t LOSPCP;
    volatile uint32_t MCDCR;
    volatile uint32_t X1CNT;
    volatile uint32_t XTALCR;
    uint16_t RESERVED3[2];
    volatile uint32_t ETHERCATCLKCTL;
    volatile uint32_t CMCLKCTL;
};

#define CLKSRCCTL1_XTALOFF         (1 << 5)
#define CLKSRCCTL1_OSCCLKSRCSEL_M  0x3u
#define CLKSRCCTL1_OSCCLKSRCSEL(x) ((x) & CLKSRCCTL1_OSCCLKSRCSEL_M)

#define SYSPLLCTL1_PLLCLKEN (1 << 1)
#define SYSPLLCTL1_PLLEN    (1 << 0)

#define SYSPLLMULT_FMULT_M  0x3u
#define SYSPLLMULT_FMULT(x) (((x) & SYSPLLMULT_FMULT_M) << 8)
#define SYSPLLMULT_IMULT_M  0x7fu
#define SYSPLLMULT_IMULT(x) ((x) & SYSPLLMULT_IMULT_M)

#define SYSPLLSTS_LOCKS (1 << 0)

#define SYSCLKDIVSEL_PLLSYSCLKDIV_M  0x3fu
#define SYSCLKDIVSEL_PLLSYSCLKDIV(x) ((x) & SYSCLKDIVSEL_PLLSYSCLKDIV_M)

struct CPU_SYS_REGS {
    uint16_t RESERVED0[34];
    volatile uint32_t PCLKCRn[24];
};

/* registers */
static struct CLK_CFG_REGS *CLK_CFG_REGS =
    (struct CLK_CFG_REGS *)ADDR_CLKCFG;
static struct CPU_SYS_REGS *CPU_SYS_REGS =
    (struct CPU_SYS_REGS *)ADDR_CPUSYS;

#define INTOSCx_FREQ 10000000

static struct {
    clock_freq_t clksrc;
    clock_freq_t cpuclk;
} clocks;

static void configure_xtal_sel(unsigned long freq)
{
    ASM(" eallow");

    CLK_CFG_REGS->CLKSRCCTL1 &= ~CLKSRCCTL1_XTALOFF;    /* turn XTAL on */
    ASM(" mov AL, #300 \n rpt @AL || nop");             /* delay */

    /* XTAL as OSCCLKSRC */
    CLK_CFG_REGS->CLKSRCCTL1 &= ~CLKSRCCTL1_OSCCLKSRCSEL_M;
    CLK_CFG_REGS->CLKSRCCTL1 |= CLKSRCCTL1_OSCCLKSRCSEL(1);
    ASM(" mov AL, #300 \n rpt @AL || nop"); /* delay */

    ASM(" edis");

    /* remember */
    clocks.clksrc = (clock_freq_t)freq;
}

static int pll_lock_busywait(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while (deadlock-- != 0)
        if ((CLK_CFG_REGS->SYSPLLSTS & SYSPLLSTS_LOCKS) != 0)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

static int pll_setup(uint32_t imult, uint32_t fmult, uint32_t divsel)
{
    if (!picoRTOS_assert(imult <= (uint32_t)SYSPLLMULT_IMULT_M)) return -EINVAL;
    if (!picoRTOS_assert(fmult <= (uint32_t)SYSPLLMULT_FMULT_M)) return -EINVAL;
    if (!picoRTOS_assert(divsel <= (uint32_t)SYSCLKDIVSEL_PLLSYSCLKDIV_M)) return -EINVAL;

    int i;

    ASM(" eallow");

    /* ti: bypass pll and set dividers to 1 */
    CLK_CFG_REGS->SYSPLLCTL1 &= ~SYSPLLCTL1_PLLCLKEN;

    /* ti: delay of at least 120 OSCCLK cycles required post pll bypass */
    ASM(" rpt #120 || nop");
    CLK_CFG_REGS->SYSCLKDIVSEL &= ~SYSCLKDIVSEL_PLLSYSCLKDIV_M;

    /* ti: lock the pll 5 times at least */
    for (i = 0; i < PLL_LOCK_COUNT; i++) {

        int res;

        /* ti: turn off pll */
        CLK_CFG_REGS->SYSPLLCTL1 &= ~SYSPLLCTL1_PLLEN;
        ASM(" rpt #20 || nop");  /* delay */

        /* ti: write multiplier (pll turns on) */
        CLK_CFG_REGS->SYSPLLMULT = (uint32_t)(SYSPLLMULT_FMULT(fmult) |
                                              SYSPLLMULT_IMULT(imult));

        if ((res = pll_lock_busywait()) < 0) {
            ASM(" edis");
            return res;
        }
    }

    /* set divsel */
    CLK_CFG_REGS->SYSCLKDIVSEL = (uint32_t)SYSCLKDIVSEL_PLLSYSCLKDIV(divsel);

    /* enable PLLSYSCLK */
    CLK_CFG_REGS->SYSPLLCTL1 |= SYSPLLCTL1_PLLCLKEN;
    ASM(" rpt #20 || nop");  /* delay */

    ASM(" edis");
    return 0;
}

static int configure_pll(unsigned long freq)
{
    unsigned long imult;
    unsigned long divsel;

    /* find the right combination by brute force */
    for (divsel = 2ul; divsel <= (unsigned long)SYSCLKDIVSEL_PLLSYSCLKDIV_M; divsel++)
        for (imult = 1ul; imult <= (unsigned long)SYSPLLMULT_IMULT_M; imult++) {
            if (freq == ((unsigned long)clocks.clksrc * imult) / divsel) {
                clocks.cpuclk = (clock_freq_t)freq;
                return pll_setup((uint32_t)imult, (uint32_t)0, (uint32_t)(divsel - 1ul));
            }
        }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

/* Function: clock_f28379x_init
 * Initialises the f28379x clock system
 *
 * Parameters:
 *  settings - The desired lcock settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_f28379x_init(struct clock_settings *settings)
{
    if (!picoRTOS_assert(settings->clksrc < CLOCK_F28379X_CLKSRC_COUNT)) return -EINVAL;

    /* default is INTOSC2 */
    clocks.clksrc = (clock_freq_t)INTOSCx_FREQ;

    if (settings->clksrc == CLOCK_F28379X_CLKSRC_XTAL)
        configure_xtal_sel(settings->clksrc_freq); /* switch to xtal */

    /* set CPU clock to requested pll freq */
    return configure_pll(settings->pll);
}

/* Function: clock_f28379x_pclk_enable
 * Enables a peripheral clock
 *
 * Parameters:
 *  clk - The clock to enable
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_f28379x_pclk_enable(clock_f28379x_pclk_t pclk)
{
    if (!picoRTOS_assert(pclk < CLOCK_F28379X_PCLK_COUNT)) return -EINVAL;

    size_t index = (size_t)pclk >> 5;
    uint32_t mask = (uint32_t)(1ul << (0x1fu & pclk));

    ASM(" eallow");;
    CPU_SYS_REGS->PCLKCRn[index] |= mask;
    ASM(" edis");

    return 0;
}

/* Function: clock_f28379x_pclk_disable
 * Disables a peripheral clock
 *
 * Parameters:
 *  pclk - The clock to disable
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_f28379x_pclk_disable(clock_f28379x_pclk_t pclk)
{
    if (!picoRTOS_assert(pclk < CLOCK_F28379X_PCLK_COUNT)) return -EINVAL;

    size_t index = (size_t)pclk >> 5;
    uint32_t mask = (uint32_t)(1ul << (0x1fu & pclk));

    ASM(" eallow");
    CPU_SYS_REGS->PCLKCRn[index] &= ~mask;
    ASM(" edis");

    return 0;
}

/* hooks */

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    switch (clkid) {
    case CLOCK_F28379X_SYSCLK: return clocks.cpuclk;
    case CLOCK_F28379X_INTOSC2: /* @fallthrough@ */
    case CLOCK_F28379X_INTOSC1: return (clock_freq_t)INTOSCx_FREQ;
    case CLOCK_F28379X_LSPCLK: return clocks.cpuclk / (clock_freq_t)4;
    case CLOCK_F28379X_EPWMCLK: return clocks.cpuclk / (clock_freq_t)2;
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return (clock_freq_t)-EINVAL;
}
