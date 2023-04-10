#include "clock-mpc574xx.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

struct MC_CGM {
    uint32_t RESERVED0[96];
    volatile uint32_t CLKOUT1_SC;
    uint32_t RESERVED1;
    volatile uint32_t CLKOUT1_DC;
    uint32_t RESERVED2[13];
    volatile uint32_t AC5_SC;
    volatile uint32_t AC5_SS;
    uint32_t RESERVED3[14];
    volatile uint32_t SC_DC6;
    uint32_t RESERVED4[31];
    volatile uint32_t AC8_SC;
    volatile uint32_t AC8_SS;
    uint32_t RESERVED5[14];
    volatile uint32_t AC9_SC;
    volatile uint32_t AC9_SS;
    uint32_t RESERVED6[270];
    volatile uint8_t PCS_SDUR;
    uint8_t RESERVED7[3];
    volatile uint32_t PCS_DIVC1;
    volatile uint32_t PCS_DIVE1;
    volatile uint32_t PCS_DIVS1;
    volatile uint32_t PCS_DIVC2;
    volatile uint32_t PCS_DIVE2;
    volatile uint32_t PCS_DIVS2;
    uint32_t RESERVED8[44];
    volatile uint32_t DIV_UPD_ABRTD;
    volatile uint32_t SC_DIV_RC;
    volatile uint32_t DIV_UPD_TYPE;
    volatile uint32_t DIV_UPD_TRIG;
    volatile uint32_t DIV_UPD_STAT;
    uint32_t RESERVED9;
    volatile uint32_t SC_SS;
    volatile uint32_t SC_DC0;
    volatile uint32_t SC_DC1;
    volatile uint32_t SC_DC2;
    volatile uint32_t SC_DC3;
    volatile uint32_t SC_DC4;
    volatile uint32_t SC_DC5;
    uint32_t RESERVED10[16];
    volatile uint32_t AC2_SC;
    volatile uint32_t AC2_SS;
    uint32_t RESERVED11[7];
    volatile uint32_t AC3_SS;
    uint32_t RESERVED12[6];
    volatile uint32_t AC4_SC;
    volatile uint32_t AC4_SS;
    uint32_t RESERVED13[14];
    volatile uint32_t AC6_SC;
    volatile uint32_t AC6_SS;
    volatile uint32_t AC6_DC0;
};

#define SC_DCn_DE     (1u << 31)
#define SC_DCn_DIV_M  0x7u
#define SC_DCn_DIV(x) (((x) & SC_DCn_DIV_M) << 16)

#define AC5_SC_SELCTL (1 << 24)

struct PLLDIG {
    uint32_t RESERVED0[6];
    volatile uint32_t PLLCAL3;
    uint32_t RESERVED1;
    volatile uint32_t PLLCR;
    volatile uint32_t PLLSR;
    volatile uint32_t PLLDV;
    volatile uint32_t PLLFM;
    volatile uint32_t PLLFD;
    uint32_t RESERVED2;
    volatile uint32_t PLLCAL1;
};

#define PLLCAL3_MFDEN_M  0x7fu
#define PLLCAL3_MFDEN(x) (((x) & PLLCAL3_MFDEN_M) << 14)

#define PLLDV_RFDPHI1_M  0x3fu
#define PLLDV_RFDPHI1(x) (((x) & PLLDV_RFDPHI1_M) << 25)
#define PLLDV_RFDPHI_M   0x3fu
#define PLLDV_RFDPHI(x)  (((x) & PLLDV_RFDPHI_M) << 16)
#define PLLDV_PREDIV_M   0x7u
#define PLLDV_PREDIV(x)  (((x) & PLLDV_PREDIV_M) << 12)
#define PLLDV_MFD_M      0xffu
#define PLLDV_MFD(x)     ((x) & PLLDV_MFD_M)

#define PLLFD_SMDEN (1 << 30)

struct MC_ME {
    volatile uint32_t GS;
    volatile uint32_t MCTL;
    volatile uint32_t ME;
    volatile uint32_t IS;
    volatile uint32_t IM;
    volatile uint32_t IMTS;
    volatile uint32_t DMTS;
    uint32_t RESERVED0;
    volatile uint32_t RESET_MC;
    uint32_t RESERVED1;
    volatile uint32_t SAFE_MC;
    volatile uint32_t DRUN_MC;
    volatile uint32_t RUN0_MC;
    volatile uint32_t RUN1_MC;
    volatile uint32_t RUN2_MC;
    volatile uint32_t RUN3_MC;
    uint32_t RESERVED2[2];
    volatile uint32_t STOP_MC;
    uint32_t RESERVED3[2];
    volatile uint32_t STANDBY_MC;
    uint32_t RESERVED4[2];
    volatile uint32_t PS0;
    volatile uint32_t PS1;
    volatile uint32_t PS2;
    volatile uint32_t PS3;
    uint32_t RESERVED5[4];
    volatile uint32_t RUN_PC[8];
    volatile uint32_t LP_PC[8];
    volatile uint8_t PCTLn[106];
    uint8_t RESERVED6[150];
    volatile uint32_t CS;
    uint8_t RESERVED7[2];
    volatile uint16_t CCTL1;
    volatile uint16_t CCTL2;
    volatile uint16_t CCTL3;
    uint32_t RESERVED8[6];
    volatile uint32_t CADDR1;
    volatile uint32_t CADDR2;
    volatile uint32_t CADDR3;
};

#define GS_S_CURRENT_MODE_M  0xfu
#define GS_S_CURRENT_MODE(x) (((x) & GS_S_CURRENT_MODE_M) << 28)
#define GS_S_PLLON           (1 << 6)

#define MCTL_TARGET_MODE_M  0xfu
#define MCTL_TARGET_MODE(x) (((x) & MCTL_TARGET_MODE_M) << 28)
#define MCTL_KEY_M          0xffffu
#define MCTL_KEY(x)         ((x) & MCTL_KEY_M)

#define DRUN_MC_MVRON     (1 << 20)
#define DRUN_MC_FLAON_M   0x3u
#define DRUN_MC_FLAON(x)  (((x) & DRUN_MC_FLAON_M) << 16)
#define DRUN_MC_PLLON     (1 << 6)
#define DRUN_MC_FXOSCON   (1 << 5)
#define DRUN_MC_FIRCON    (1 << 4)
#define DRUN_MC_SYSCLK_M  0xfu
#define DRUN_MC_SYSCLK(x) ((x) & DRUN_MC_SYSCLK_M)

#define RUN_PC_DRUN (1 << 3)

#define PCTLn_RUN_CFG_M  0x7u
#define PCTLn_RUN_CFG(x) ((x) & PCTLn_RUN_CFG_M)

/* no need to instanciate these in the main program */
static struct MC_CGM *MC_CGM = (struct MC_CGM*)ADDR_MC_CGM;
static struct PLLDIG *PLLDIG = (struct PLLDIG*)ADDR_PLLDIG;
static struct MC_ME *MC_ME = (struct MC_ME*)ADDR_MC_ME;

static struct {
    clock_freq_t fmpll_clkin;
    clock_freq_t sysclk;
    clock_freq_t s160;
    clock_freq_t s80;
    clock_freq_t fs80;
    clock_freq_t s40;
    clock_freq_t f80;
    clock_freq_t f40;
    clock_freq_t f20;
} clocks;

static int stable_pll_busywait(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while (deadlock-- != 0)
        if ((MC_ME->GS & GS_S_PLLON) != 0)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

static int drun_mode_busywait(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;
    uint32_t mode = (uint32_t)GS_S_CURRENT_MODE(3);

    while (deadlock-- != 0)
        if ((uint32_t)(MC_ME->GS & GS_S_CURRENT_MODE_M) == mode)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

static int setup_fmpll_clkin(clock_mpc574xx_fmpll_clkin_t clkin, unsigned long fxosc)
{
    if (!picoRTOS_assert(clkin < CLOCK_MPC574XX_FMPLL_CLKIN_COUNT)) return -EINVAL;

    switch (clkin) {
    case CLOCK_MPC574XX_FMPLL_CLKIN_FIRC:
        clocks.fmpll_clkin = (clock_freq_t)CLOCK_MPC574XX_FIRC_FREQ;
        MC_CGM->AC5_SC &= ~AC5_SC_SELCTL;
        return 0;

    case CLOCK_MPC574XX_FMPLL_CLKIN_FXOSC:
        clocks.fmpll_clkin = (clock_freq_t)fxosc;
        MC_CGM->AC5_SC |= AC5_SC_SELCTL;
        return 0;

    default:
        break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EINVAL;
}

static int setup_fmpll_phi0(unsigned long freq)
{
#define MFD_MIN       10ul
#define MFD_MAX       150ul
#define PREDIV_MIN    1ul
#define PREDIV_MAX    6ul
#define RFDPHI_P2_MIN 0ul   /* powers of 2 */
#define RFDPHI_P2_MAX 4ul   /* powers of 2 */

    if (!picoRTOS_assert(freq > 0)) return -EINVAL;

    unsigned long mfd;
    unsigned long prediv;
    unsigned long rfdphi_p2;

    /* brute-force computation */
    for (prediv = PREDIV_MIN; prediv <= PREDIV_MAX; prediv++)
        for (mfd = MFD_MIN; mfd <= MFD_MAX; mfd++)
            for (rfdphi_p2 = RFDPHI_P2_MIN; rfdphi_p2 <= RFDPHI_P2_MAX; rfdphi_p2++) {
                unsigned long rfdphi = 2ul << rfdphi_p2;
                unsigned long f = (((unsigned long)clocks.fmpll_clkin / prediv) * mfd) / rfdphi;
                if (freq == f) {
                    PLLDIG->PLLDV = (uint32_t)PLLDV_PREDIV(prediv);
                    PLLDIG->PLLDV |= (uint32_t)PLLDV_MFD(mfd);
                    PLLDIG->PLLDV |= (uint32_t)PLLDV_RFDPHI(rfdphi_p2);
                    PLLDIG->PLLCAL3 = (uint32_t)PLLCAL3_MFDEN(0x270f);  /* FIXME */
                    PLLDIG->PLLFD = (uint32_t)PLLFD_SMDEN;              /* sigma-delta enabled */
                    return 0;
                }
            }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int switch_to_fmpll_phi0(struct clock_settings *settings)
{
    int res;

    /* fmpll setup */
    if ((res = setup_fmpll_clkin(settings->fmpll_clkin, settings->fxosc)) < 0 ||
        (res = setup_fmpll_phi0(settings->phi0)) < 0)
        return res;

    /* drun mode only (no low power) */
    MC_ME->RUN_PC[0] = (uint32_t)RUN_PC_DRUN;

    /* switch to phi0 */
    MC_ME->DRUN_MC = (uint32_t)(DRUN_MC_MVRON |         /* main Voltage regulator on */
                                DRUN_MC_FLAON(3) |      /* flash in run mode */
                                DRUN_MC_PLLON |         /* pll on */
                                DRUN_MC_FIRCON |        /* 16mhz firc on */
                                DRUN_MC_FXOSCON |       /* fxosc on */
                                DRUN_MC_SYSCLK(2));     /* sysclk is pll phi0 */

    /* activate drun */
    MC_ME->MCTL = (uint32_t)(MCTL_TARGET_MODE(3) | MCTL_KEY(0x5af0));
    MC_ME->MCTL = (uint32_t)(MCTL_TARGET_MODE(3) | MCTL_KEY(0xa50f));

    /* wait for pll stabilization */
    if ((res = stable_pll_busywait()) < 0)
        return res;

    /* check for drun mode */
    return drun_mode_busywait();
}

static int switch_to_fxosc(unsigned long fxosc)
{
    if (!picoRTOS_assert(fxosc >= 8000000ul)) return -EINVAL;
    if (!picoRTOS_assert(fxosc <= 40000000ul)) return -EINVAL;

    /* drun mode only (no low power) */
    MC_ME->RUN_PC[0] = (uint32_t)RUN_PC_DRUN;

    /* switch to fxosc */
    MC_ME->DRUN_MC = (uint32_t)(DRUN_MC_MVRON |         /* main Voltage regulator on */
                                DRUN_MC_FLAON(3) |      /* flash in run mode */
                                DRUN_MC_FXOSCON |       /* fxosc on */
                                DRUN_MC_SYSCLK(1));     /* sysclk is fxosc */

    /* activate drun */
    MC_ME->MCTL = (uint32_t)(MCTL_TARGET_MODE(3) | MCTL_KEY(0x5af0));
    MC_ME->MCTL = (uint32_t)(MCTL_TARGET_MODE(3) | MCTL_KEY(0xa50f));

    clocks.sysclk = (clock_freq_t)fxosc;
    return drun_mode_busywait();
}

static int set_scalable_dividers(struct clock_settings *settings)
{
    if (!picoRTOS_assert(settings->s160_div <= 8ul)) return -EINVAL;
    if (!picoRTOS_assert(settings->s80_div <= 8ul)) return -EINVAL;
    if (!picoRTOS_assert(settings->fs80_div <= 8ul)) return -EINVAL;
    if (!picoRTOS_assert(settings->s40_div <= 16ul)) return -EINVAL;

    if (settings->s160_div != 0)
        MC_CGM->SC_DC0 = (uint32_t)(SC_DCn_DIV(settings->s160_div - 1ul) | SC_DCn_DE);

    if (settings->s80_div != 0)
        MC_CGM->SC_DC1 = (uint32_t)(SC_DCn_DIV(settings->s80_div - 1ul) | SC_DCn_DE);

    if (settings->s40_div != 0)
        MC_CGM->SC_DC2 = (uint32_t)(SC_DCn_DIV(settings->s40_div - 1ul) | SC_DCn_DE);

    if (settings->fs80_div != 0)
        MC_CGM->SC_DC5 = (uint32_t)(SC_DCn_DIV(settings->fs80_div - 1ul) | SC_DCn_DE);

    return 0;
}

int clock_mpc574xx_init(struct clock_settings *settings)
{
    int res;

    /* dividers */
    if ((res = set_scalable_dividers(settings)) < 0)
        return res;

    switch (settings->sysclk) {
    case CLOCK_MPC574XX_SYSCLK_FIRC:
        clocks.sysclk = (clock_freq_t)CLOCK_MPC574XX_FIRC_FREQ;
        break;

    case CLOCK_MPC574XX_SYSCLK_FXOSC:
        res = switch_to_fxosc(settings->fxosc);
        break;

    case CLOCK_MPC574XX_SYSCLK_PHI0:
        res = switch_to_fmpll_phi0(settings);
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    if (res < 0)
        return res;

    /* set clock values */
    clocks.s160 = clocks.sysclk / (clock_freq_t)settings->s160_div;
    clocks.s80 = clocks.sysclk / (clock_freq_t)settings->s80_div;
    clocks.fs80 = clocks.sysclk / (clock_freq_t)settings->fs80_div;
    clocks.s40 = clocks.sysclk / (clock_freq_t)settings->s40_div;
    /* non-scalable clocks */
    clocks.f40 = clocks.sysclk / (clock_freq_t)4;
    clocks.f80 = clocks.sysclk / (clock_freq_t)2;
    clocks.f20 = clocks.sysclk / (clock_freq_t)8;

    /* TODO: assert clocks limits */

    return 0;
}

/* hooks */

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    if (!picoRTOS_assert(clkid < (clock_id_t)CLOCK_MPC574XX_COUNT))
        return (clock_freq_t)-EINVAL;

    switch (clkid) {
    case CLOCK_MPC574XX_F160: return clocks.sysclk;
    case CLOCK_MPC574XX_S160: return clocks.s160;
    case CLOCK_MPC574XX_S80: return clocks.s80;
    case CLOCK_MPC574XX_FS80: return clocks.fs80;
    case CLOCK_MPC574XX_S40: return clocks.s40;
    case CLOCK_MPC574XX_F40: return clocks.f40;
    case CLOCK_MPC574XX_F80: return clocks.f80;
    case CLOCK_MPC574XX_F20: return clocks.f20;
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return (clock_freq_t)-EIO;
}