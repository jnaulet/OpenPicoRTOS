#include "clock-nxp_siu.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct SIU {
    uint32_t RESERVED0;
    volatile uint32_t MIDR;
    uint32_t RESERVED1;
    volatile uint32_t RSR;
    volatile uint32_t SRCR;
    volatile uint32_t EISR;
    volatile uint32_t DIRER;
    volatile uint32_t DIRSR;
    volatile uint32_t OSR;
    volatile uint32_t ORER;
    volatile uint32_t IREER;
    volatile uint32_t IFEER;
    volatile uint32_t IDFR;
    volatile uint32_t IFIR;
    uint32_t RESERVED2[2];
    volatile uint16_t PCR[512];
    uint32_t RESERVED3[112];
    volatile uint8_t GPDO[512];
    volatile uint8_t GPDIL[256];
    uint32_t RESERVED4;
    volatile uint32_t EIISR;
    volatile uint32_t DISR;
    uint32_t RESERVED5;
    volatile uint32_t ISEL4;
    volatile uint32_t ISEL5;
    volatile uint32_t ISEL6;
    volatile uint32_t ISEL7;
    volatile uint32_t ISEL8;
    volatile uint32_t ISEL9;
    volatile uint32_t DECFIL1;
    volatile uint32_t DECFIL2;
    volatile uint32_t DECFIL3;
    volatile uint32_t DECFIL4;
    volatile uint32_t DECFIL5;
    volatile uint32_t REACTSR;
    volatile uint32_t SDGATE_SEL;
    volatile uint32_t SDETPUA;
    volatile uint32_t SDETPUB;
    volatile uint32_t SDETPUC;
    volatile uint32_t TBG_CR_A[6];
    volatile uint32_t TBG_CR_B[6];
    uint32_t RESERVED6;
    volatile uint32_t ECCR;
    uint32_t RESERVED7[6];
    volatile uint32_t SYSDIV;
    volatile uint32_t HLT1;
    volatile uint32_t HLTACK1;
    volatile uint32_t RSTVEC0;
    volatile uint32_t RSTVEC1;
    volatile uint32_t C0PID;
    volatile uint32_t C1PID;
    volatile uint32_t HLT2;
    volatile uint32_t HLTACK2;
    volatile uint32_t SDCLKCFG;
    volatile uint32_t LFCLKCFG;
    volatile uint32_t PSCLKCFG;
    volatile uint32_t RCR;
    volatile uint32_t LOCKSTEP;
    volatile uint32_t PCSER;
    volatile uint32_t PCSIFR;
    volatile uint32_t FECCR;
    volatile uint32_t ECCEIR;
    uint32_t RESERVED8[134];
    volatile uint32_t PGPDO[16];
    volatile uint32_t PGPDI[16];
    volatile uint32_t MPGPDO[32];
    struct {
        volatile uint32_t DSPIH;
        volatile uint32_t DSPIL;
    } DSPIn[4];
    uint32_t RESERVED9[8];
    volatile uint32_t ETPUBA;
    volatile uint32_t EMIOSA;
    volatile uint32_t DSPIAHLA;
    uint32_t RESERVED10;
    volatile uint32_t ETPUAB;
    volatile uint32_t EMIOSB;
    volatile uint32_t DSPIBHLB;
    uint32_t RESERVED11;
    volatile uint32_t ETPUAC;
    volatile uint32_t EMIOSC;
    volatile uint32_t DSPICHLC;
    volatile uint32_t ETPUBC;
    volatile uint32_t ETPUBD;
    volatile uint32_t EMIOSD;
    volatile uint32_t DSPIDHLD;
    uint32_t RESERVED12[33];
    volatile uint8_t GPDI[512];
    uint32_t RESERVED13[128];
    volatile uint32_t IMUX0;
    volatile uint32_t IMUX1;
    volatile uint32_t IMUX2;
    volatile uint32_t IMUX3;
    volatile uint32_t IMUX4;
    volatile uint32_t IMUX5;
    uint32_t RESERVED14;
    volatile uint32_t IMUX7;
    uint32_t RESERVED15[2];
    volatile uint32_t IMUX10;
    uint32_t RESERVED16;
    volatile uint32_t IMUX12;
};

#define RSR_XOSC (1 << 7)

#define PCSIFR_PCSMS_M      0x3u
#define PCSIFR_PCSMS(x)     (((x) & PCSIFR_PCSMS_M) << 1)
#define PCSIFR_PCSI         (1 << 0)

#define SYSDIV_LCK          (1u << 31)
#define SYSDIV_PLL0SEL      (1 << 28)
#define SYSDIV_PLL1SEL      (1 << 26)
#define SYSDIV_PERCLKSEL    (1 << 24)
#define SYSDIV_FMPERDIV_M   0x3u
#define SYSDIV_FMPERDIV(x)  (((x) & SYSDIV_FMPERDIV_M) << 20)
#define SYSDIV_PERDIV_M     0x3u
#define SYSDIV_PERDIV(x)    (((x) & SYSDIV_FMPERDIV_M) << 16)
#define SYSDIV_MCANSEL      (1 << 15)
#define SYSDIV_SYSCLKSEL_M  0x3u
#define SYSDIV_SYSCLKSEL(x) (((x) & SYSDIV_SYSCLKSEL_M) << 12)
#define SYSDIV_ETPUDIV      (1 << 8)
#define SYSDIV_SYSCLKDIV_M  0x7u
#define SYSDIV_SYSCLKDIV(x) (((x) & SYSDIV_SYSCLKDIV_M) << 2)
#define SYSDIV_PCSEN        (1 << 0)

#define C99_ECCR_ENGDIV_M  0xffu
#define C99_ECCR_ENGDIV(x) (((x) & C99_ECCR_ENGDIV_M) << 8)
#define C99_ECCR_ECCS      (1 << 7)
#define C99_ECCR_EBDF_M    0x3u
#define C99_ECCR_EBDF(x)   ((x) & C99_ECCR_EBDF_M)

#define SDCLKCFG_SDDIV_M  0x7fu
#define SDCLKCFG_SDDIV(x) ((x) & SDCLKCFG_SDDIV_M)

#define LFCLKCFG_LFCLKSEL_M  0x3u
#define LFCLKCFG_LFCLKSEL(x) (((x) & LFCLKCFG_LFCLKSEL_M) << 12)
#define LFCLKCFG_LFDIV_M     0x7fu
#define LFCLKCFG_LFDIV(x)    ((x) & LFCLKCFG_LFDIV_M)

#define PSCLKCFG_PSDIV_M    0xffu
#define PSCLKCFG_PSDIV(x)   (((x) & PSCLKCFG_PSDIV_M) << 16)
#define PSCLKCFG_PSDIV1M_M  0x3ffu
#define PSCLKCFG_PSDIV1M(x) ((x) & PSCLKCFG_PSDIV1M_M)

struct PLLDIG {
    volatile uint32_t PLL0CR;
    volatile uint32_t PLL0SR;
    volatile uint32_t PLL0DV;
    uint32_t RESERVED0[5];
    volatile uint32_t PLL1CR;
    volatile uint32_t PLL1SR;
    volatile uint32_t PLL1DV;
    volatile uint32_t PLL1FM;
    volatile uint32_t PLL1FD;
};

#define PLLnCR_CLKCFG_M   0x3u
#define PLLnCR_CLKCFG(x)  (((x) & PLLnCR_CLKCFG_M) << 8)

#define PLL0DV_RFDPHI1_M  0xfu
#define PLL0DV_RFDPHI1(x) (((x) & PLL0DV_RFDPHI1_M) << 27)
#define PLLnDV_RFDPHI_M   0x3fu
#define PLLnDV_RFDPHI(x)  (((x) & PLLnDV_RFDPHI_M) << 16)
#define PLL0DV_PREDIV_M   0x7u
#define PLL0DV_PREDIV(x)  (((x) & PLL0DV_PREDIV_M) << 12)
#define PLLnSR_LOCK       (1 << 2)
#define PLLnDV_MFD_M      0x7fu
#define PLLnDV_MFD(x)     ((x) & PLLnDV_MFD_M)

struct PCS_0 {
    uint32_t RESERVED0[448];
    volatile uint32_t SDUR;
    volatile uint32_t DIVC1;
    volatile uint32_t DIVE1;
    volatile uint32_t DIVS1;
    volatile uint32_t DIVC2;
    volatile uint32_t DIVE2;
    volatile uint32_t DIVS2;
    volatile uint32_t DIVC3;
    volatile uint32_t DIVE3;
    volatile uint32_t DIVS3;
};

#define DIVCn_RATE_M  0xffu
#define DIVCn_RATE(x) ((x) & DIVCn_RATE_M)
#define DIVCn_INIT_M  0xffffu
#define DIVCn_INIT(x) (((x) & DIVCn_INIT_M) << 16)

#define DIVEn_DIVE_M  0xffffu
#define DIVEn_DIVE(x) ((x) & DIVEn_DIVE_M)

#define DIVSn_DIVS_M  0xffffu
#define DIVSn_DIVS(x) ((x) & DIVSn_DIVS_M)

/* instances */
static struct SIU *SIU = (struct SIU*)ADDR_SIU;
static struct PLLDIG *PLLDIG = (struct PLLDIG*)ADDR_PLLDIG;
static struct PCS_0 *PCS_0 = (struct PCS_0*)ADDR_PCS_0;

static struct {
    /* pll */
    unsigned long pll0_phi;
    unsigned long pll0_phi1;
    unsigned long pll1_phi;
    unsigned long sysclk;
    /* exported */
    clock_freq_t xosc;
    clock_freq_t core_clk;
    clock_freq_t plat_clk;
    clock_freq_t per_clk;
    clock_freq_t fm_per_clk;
    clock_freq_t etpu_clk;
} clocks;

static int xosc_stable_clock_busywait(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while ((SIU->RSR & RSR_XOSC) == 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int pll0_lock_busywait(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while ((PLLDIG->PLL0SR & PLLnSR_LOCK) == 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int pll1_lock_busywait(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while ((PLLDIG->PLL1SR & PLLnSR_LOCK) != 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int setup_pll0(unsigned long freq,
                      unsigned long phi_div,
                      unsigned long phi1_div)
{
#define PLL0DV_MFD_COUNT    (PLLnDV_MFD_M + 1)
#define PLL0DV_PREDIV_COUNT (PLL0DV_PREDIV_M + 1)

    picoRTOS_assert(phi_div > 0, return -EINVAL);
    picoRTOS_assert(phi_div <= (unsigned long)PLLnDV_RFDPHI_M, return -EINVAL);
    picoRTOS_assert(phi1_div > 4ul, return -EINVAL);
    picoRTOS_assert(phi1_div <= (unsigned long)PLL0DV_RFDPHI1_M, return -EINVAL);

    unsigned long mfd;
    unsigned long prediv;

    /* force pll0sel to xosc. FIXME ? */
    SIU->SYSDIV &= ~SYSDIV_PLL0SEL;
    /* turn pll off */
    PLLDIG->PLL0CR &= ~PLLnCR_CLKCFG(PLLnCR_CLKCFG_M);
    PLLDIG->PLL0DV = (uint32_t)0;

    for (prediv = 1ul; prediv < (unsigned long)PLL0DV_PREDIV_COUNT; prediv++)
        for (mfd = 8ul; mfd < (unsigned long)PLL0DV_MFD_COUNT; mfd++) {
            unsigned long pll0 = ((unsigned long)clocks.xosc / prediv) * mfd;
            if (pll0 == freq) {
                PLLDIG->PLL0DV |= PLL0DV_PREDIV(prediv);
                PLLDIG->PLL0DV |= PLLnDV_MFD(mfd);
                PLLDIG->PLL0DV |= PLLnDV_RFDPHI(phi_div);
                PLLDIG->PLL0DV |= PLL0DV_RFDPHI1(phi1_div);
                /* set clock values */
                clocks.pll0_phi = pll0 / phi_div;
                clocks.pll0_phi1 = pll0 / phi1_div;
                /* turn pll on */
                PLLDIG->PLL0CR |= PLLnCR_CLKCFG(0x3);
                return pll0_lock_busywait();
            }
        }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int setup_pll1(unsigned long freq)
{
#define PLL1DV_MFD_COUNT    35
#define PLL1DV_RFDPHI_COUNT 64

    unsigned long mfd;
    unsigned long rfdphi;

    /* force pll1sel to pll0_phi1. FIXME ? */
    SIU->SYSDIV |= SYSDIV_PLL1SEL;
    /* turn pll off */
    PLLDIG->PLL1CR &= ~PLLnCR_CLKCFG(PLLnCR_CLKCFG_M);
    PLLDIG->PLL1DV = (uint32_t)0;

    for (mfd = 16ul; mfd < (unsigned long)PLL1DV_MFD_COUNT; mfd++)
        for (rfdphi = 1ul; rfdphi < (unsigned long)PLL1DV_RFDPHI_COUNT; rfdphi++) {
            unsigned long pll1_phi = clocks.pll0_phi1 * mfd / (rfdphi * 2ul);
            if (pll1_phi == freq) {
                PLLDIG->PLL1DV |= PLLnDV_MFD(mfd);
                PLLDIG->PLL1DV |= PLLnDV_RFDPHI(rfdphi);
                /* set clocks */
                clocks.pll1_phi = pll1_phi;
                /* turn pll on */
                PLLDIG->PLL1CR |= PLLnCR_CLKCFG(0x3);
                return pll1_lock_busywait();
            }
        }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int setup_sysclk(unsigned long sysclkdiv)
{
    uint32_t value = 0;

    switch (sysclkdiv) {
    case 1: value = (uint32_t)4; break;
    case 2: value = (uint32_t)0; break;
    case 4: value = (uint32_t)1; break;
    case 8: value = (uint32_t)2; break;
    case 16: value = (uint32_t)3; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    /* sysclksel is pll1 */
    SIU->SYSDIV &= ~SYSDIV_SYSCLKSEL(SYSDIV_SYSCLKSEL_M);
    SIU->SYSDIV |= SYSDIV_SYSCLKSEL(0x2);
    SIU->SYSDIV &= ~SYSDIV_SYSCLKDIV(SYSDIV_SYSCLKDIV_M);
    SIU->SYSDIV |= SYSDIV_SYSCLKDIV(value);

    clocks.sysclk = clocks.pll1_phi / sysclkdiv;
    clocks.core_clk = (clock_freq_t)(clocks.pll1_phi / sysclkdiv);
    clocks.plat_clk = (clock_freq_t)(clocks.sysclk / 2ul);

    /* set system clock */
    arch_set_clock_frequency((unsigned long)clocks.sysclk);

    return 0;
}

static int setup_per_clk(unsigned long perdiv)
{
    uint32_t value = 0;

    switch (perdiv) {
    case 2: value = (uint32_t)0; break;
    case 4: value = (uint32_t)1; break;
    case 8: value = (uint32_t)2; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    /* force perclk's input as pll0_phi. FIXME */
    SIU->SYSDIV |= SYSDIV_PERCLKSEL;
    SIU->SYSDIV &= ~SYSDIV_PERDIV(SYSDIV_PERDIV_M);
    SIU->SYSDIV |= SYSDIV_PERDIV(value);

    clocks.per_clk = (clock_freq_t)(clocks.pll0_phi / perdiv);
    return 0;
}

static int setup_fm_per_clk(unsigned long fmperdiv)
{
    uint32_t value = 0;

    switch (fmperdiv) {
    case 2: value = (uint32_t)0; break;
    case 4: value = (uint32_t)1; break;
    case 8: value = (uint32_t)2; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    SIU->SYSDIV &= ~SYSDIV_FMPERDIV(SYSDIV_FMPERDIV_M);
    SIU->SYSDIV |= SYSDIV_FMPERDIV(value);

    clocks.fm_per_clk = (clock_freq_t)(clocks.sysclk / fmperdiv);
    return 0;
}

static int setup_etpu_clk(unsigned long etpudiv)
{
    switch (etpudiv) {
    case 1: SIU->SYSDIV |= SYSDIV_ETPUDIV; break;
    case 2: SIU->SYSDIV &= ~SYSDIV_ETPUDIV; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    clocks.etpu_clk = (clock_freq_t)(clocks.sysclk / etpudiv);
    return 0;
}

static void setup_pcs(void)
{
#define DEFAULT_DIVC_RATE 12
#define DEFAULT_DIVC_INIT 624
#define DEFAULT_DIVE      16499
#define DEFAULT_DIVS      17535

    /* step duration is 16 irc clock cycles (FIXME) */
    PCS_0->SDUR = (uint32_t)16;

    /* div1 */
    PCS_0->DIVC1 = (uint32_t)DIVCn_RATE(DEFAULT_DIVC_RATE);
    PCS_0->DIVC1 |= DIVCn_INIT(DEFAULT_DIVC_INIT);
    PCS_0->DIVE1 = (uint32_t)DIVEn_DIVE(DEFAULT_DIVE);
    PCS_0->DIVS1 = (uint32_t)DIVSn_DIVS(DEFAULT_DIVS);
    /* div2 */
    PCS_0->DIVC2 = (uint32_t)DIVCn_RATE(DEFAULT_DIVC_RATE);
    PCS_0->DIVC2 |= DIVCn_INIT(DEFAULT_DIVC_INIT);
    PCS_0->DIVE2 = (uint32_t)DIVEn_DIVE(DEFAULT_DIVE);
    PCS_0->DIVS2 = (uint32_t)DIVSn_DIVS(DEFAULT_DIVS);
    /* div3 */
    PCS_0->DIVC3 = (uint32_t)DIVCn_RATE(DEFAULT_DIVC_RATE);
    PCS_0->DIVC3 |= DIVCn_INIT(DEFAULT_DIVC_INIT);
    PCS_0->DIVE3 = (uint32_t)DIVEn_DIVE(DEFAULT_DIVE);
    PCS_0->DIVS3 = (uint32_t)DIVSn_DIVS(DEFAULT_DIVS);
}

static int start_pcs(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    SIU->SYSDIV |= SYSDIV_PCSEN; /* enable pcs */

    while ((SIU->PCSIFR & PCSIFR_PCSI) == 0 &&
           (SIU->PCSIFR & PCSIFR_PCSMS(PCSIFR_PCSMS_M)) != PCSIFR_PCSMS(3) &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    /* ack */
    SIU->PCSIFR |= PCSIFR_PCSI;
    return 0;
}

/* Function: clock_nxp_siu_init
 * Inits the SIU clock subsystem
 *
 * Parameters:
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, <0 error code otherwise
 */
int clock_nxp_siu_init(struct clock_settings *settings)
{
    int res;

    /* run sysclk from irc */
    SIU->SYSDIV = (uint32_t)0;

    /* prepare clocks */
    setup_pcs();
    clocks.xosc = (clock_freq_t)settings->xosc;

    /* wait for xosc & setup plls */
    if ((res = xosc_stable_clock_busywait()) < 0 ||
        (res = setup_pll0(settings->pll0, settings->pll0_phi_div, settings->pll0_phi1_div)) < 0 ||
        (res = setup_pll1(settings->pll1)) < 0)
        return res;

    /* select clock dividers */
    if ((res = setup_per_clk(settings->perdiv)) < 0 ||
        (res = setup_sysclk(settings->sysclkdiv)) < 0 ||
        (res = setup_fm_per_clk(settings->fmperdiv)) < 0 ||
        (res = setup_etpu_clk(settings->etpudiv)) < 0)
        return res;

    /* turn on pcs */
    if ((res = start_pcs()) < 0)
        return res;

    /* FIXME: fixed values */

    SIU->SYSDIV &= ~SYSDIV_MCANSEL; /* MCANSEL = XOSC */

    /* eccr */
    SIU->ECCR = (uint32_t)C99_ECCR_ENGDIV(16); /* 1/32th */
    SIU->ECCR |= (C99_ECCR_EBDF(2) | C99_ECCR_ECCS);
    /* sdclk */
    SIU->SDCLKCFG = (uint32_t)SDCLKCFG_SDDIV(14); /* 1/15th */
    /* lfclk */
    SIU->LFCLKCFG = (uint32_t)LFCLKCFG_LFCLKSEL(0);
    SIU->LFCLKCFG |= LFCLKCFG_LFDIV(1);             /* half */
    /* psclk */
    SIU->PSCLKCFG = (uint32_t)PSCLKCFG_PSDIV(2);    /* 1/3rd */
    SIU->PSCLKCFG |= PSCLKCFG_PSDIV1M(239);         /* 1/240th */

    /* lock register */
    SIU->SYSDIV |= SYSDIV_LCK;

    return 0;
}

/* hooks */

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    picoRTOS_assert(clkid < (clock_id_t)CLOCK_NXP_SIU_COUNT,
                    return (clock_freq_t)-EINVAL);

    switch (clkid) {
    case CLOCK_NXP_SIU_XOSC: return clocks.xosc;
    case CLOCK_NXP_SIU_IRC:  return (clock_freq_t)16000000u;   /* 16mhz irc */
    case CLOCK_NXP_SIU_CORE_CLK: return clocks.core_clk;
    case CLOCK_NXP_SIU_PLAT_CLK: return clocks.plat_clk;
    case CLOCK_NXP_SIU_FM_PER_CLK: return clocks.fm_per_clk;
    case CLOCK_NXP_SIU_ETPU_CLK: return clocks.etpu_clk;
    case CLOCK_NXP_SIU_PER_CLK: return clocks.per_clk;
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return (clock_freq_t)-EIO;
}
