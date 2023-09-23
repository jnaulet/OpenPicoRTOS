#include "clock-renesas_ra4.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct SYSTEM {
    uint8_t RESERVED0[28];
    volatile uint32_t MSTPCRA;  /* 0x4001e01c */
    volatile uint32_t SCKDIVCR; /* 0x4001e020 */
    uint8_t RESERVED1[2];
    volatile uint8_t SCKSCR;    /* 0x4001e026 */
    uint8_t RESERVED2[3];
    volatile uint8_t PLLCR;     /* 0x4001e02a */
    volatile uint8_t PLLCCR2;   /* 0x4001e02b */
    uint8_t RESERVED3[5];
    volatile uint8_t MEMWAIT;   /* 0x4001e031 */
    volatile uint8_t MOSCCR;    /* 0x4001e032 */
    uint8_t RESERVED4[3];
    volatile uint8_t HOCOCR;    /* 0x4001e036 */
    uint8_t RESERVED5;
    volatile uint8_t MOCOCR;    /* 0x4001e038 */
    uint8_t RESERVED6[3];
    volatile uint8_t OSCSF;     /* 0x4001e03c */
    uint8_t RESERVED7;
    volatile uint8_t CKOCR;     /* 0x4001e03e */
    volatile uint8_t TRCKCR;    /* 0x4001e03f */
    volatile uint8_t OSTDCR;    /* 0x4001e040 */
    volatile uint8_t OSTDSR;    /* 0x4001e041 */
    uint8_t RESERVED8[14];
    volatile uint8_t SLCDSCKCR; /* 0x4001e050 */
    uint8_t RESERVED9[16];
    volatile uint8_t MOCOUTCR;  /* 0x4001e061 */
    volatile uint8_t HOCOUTCR;  /* 0x4001e062 */
    uint8_t RESERVED10[63];
    volatile uint8_t MOSCWTCR;  /* 0x4001e0a2 */
    uint8_t RESERVED11[2];
    volatile uint8_t HOCOWTCR;  /* 0x4001e0a5 */
    uint8_t RESERVED12[42];
    volatile uint8_t USBCKCR;   /* 0x4001e0d0 */
    uint8_t RESERVED13[813];
    volatile uint16_t PRCR;     /* 0x4001e3fe */
    uint8_t RESERVED14[19];
    volatile uint8_t MOMCR;     /* 0x4001e413 */
    uint8_t RESERVED15[108];
    volatile uint8_t SOSCCR;    /* 0x4001e480 */
    volatile uint8_t SOMCR;     /* 0x4001e481 */
    uint8_t RESERVED16[14];
    volatile uint8_t LOCOCR;    /* 0x4001e490 */
    uint8_t RESERVED17;
    volatile uint8_t LOCOUTCR;  /* 0x4001e492 */
};

#define SCKDIVCR_FCK_M   0x7u
#define SCKDIVCR_FCK(x)  (((x) & SCKDIVCR_FCK_M) << 28)
#define SCKDIVCR_ICK_M   0x7u
#define SCKDIVCR_ICK(x)  (((x) & SCKDIVCR_ICK_M) << 24)
#define SCKDIVCR_PCKA_M  0x7u
#define SCKDIVCR_PCKA(x) (((x) & SCKDIVCR_PCKA_M) << 12)
#define SCKDIVCR_PCKB_M  0x7u
#define SCKDIVCR_PCKB(x) (((x) & SCKDIVCR_PCKB_M) << 8)
#define SCKDIVCR_PCKC_M  0x7u
#define SCKDIVCR_PCKC(x) (((x) & SCKDIVCR_PCKC_M) << 4)
#define SCKDIVCR_PCKD_M  0x7u
#define SCKDIVCR_PCKD(x) ((x) & SCKDIVCR_PCKD_M)

#define SCKSCR_CKSEL_M  0x7u
#define SCKSCR_CKSEL(x) ((x) & SCKSCR_CKSEL_M)

#define PLLCCR2_PLODIV_M  0x3u
#define PLLCCR2_PLODIV(x) (((x) & PLLCCR2_PLODIV_M) << 6)
#define PLLCCR2_PLLMUL_M  0x1fu
#define PLLCCR2_PLLMUL(x) ((x) & PLLCCR2_PLLMUL_M)

#define PLLCR_PLLSTP (1 << 0)

#define MEMWAIT_MEMWAIT (1 << 0)

#define MOSCCR_MOSTP (1 << 0)

#define SOSCCR_SOSTP (1 << 0)

#define LOCOCR_LCSTP (1 << 0)

#define HOCOCR_HCSTP (1 << 0)

#define MOCOCR_MCSTP (1 << 0)

#define OSCSF_PLLSF  (1 << 5)
#define OSCSF_MOSCSF (1 << 3)
#define OSCSF_HOCOSF (1 << 0)

#define OSTDCR_OSTDE  (1 << 7)
#define OSTDCR_OSTDIE (1 << 0)

#define OSTDSR_OSTDF (1 << 0)

#define MOSCWTCR_MSTS_M  0xfu
#define MOSCWTCR_MSTS(x) ((x) & MOSCWTCR_MSTS_M)

#define HOCOWTCR_HSTS_M  0x7u
#define HOCOWTCR_HSTS(x) ((x) & HOCOWTCR_HSTS_M)

#define PRCR_PRKEY_M  0xffu
#define PRCR_PRKEY(x) (((x) & PRCR_PRKEY_M) << 8)
#define PRCR_PRC3     (1 << 3)
#define PRCR_PRC1     (1 << 1)
#define PRCR_PRC0     (1 << 0)

#define MOMCR_MOSEL  (1 << 6)
#define MOMCR_MODRV1 (1 << 3)

#define SOMCR_SODRV_M 0x3u
#define SOMCR_SODRV(x) ((x) & SOMCR_SODRV_M)

#define SLCDSCKCR_LCDSCKEN     (1 << 7)
#define SLCDSCKCR_LCDSCKSEL_M  0x7u
#define SLCDSCKCR_LCDSCKSEL(x) ((x) & SLCDSCKCR_LCDSCKSEL_M)

#define CKOCR_CKOEN     (1 << 7)
#define CKOCR_CKODIV_M  0x7u
#define CKOCR_CKODIV(x) (((x) & CKOCR_CKODIV_M) << 4)
#define CKOCR_CKOSEL_M  0x7u
#define CKOCR_CKOSEL(x) ((x) & CKOCR_CKOSEL_M)

#define LOCOUTCR_LOCOUTRM_M  0xffu
#define LOCOUTCR_LOCOUTRM(x) ((x) & LOCOUTCR_LOCOUTRM_M)

#define MOCOUTCR_MOCOUTRM_M  0xffu
#define MOCOUTCR_MOCOUTRM(x) ((x) & MOCOUTCR_MOCOUTRM_M)

#define HOCOUTCR_HOCOUTRM_M  0xffu
#define HOCOUTCR_HOCOUTRM(x) ((x) & HOCOUTCR_HOCOUTRM_M)

#define TRCKCR_TRCKEN  (1 << 7)
#define TRCKCR_TRCK_M  0xfu
#define TRCKCR_TRCK(x) ((x) & TRCKCR_TRCK_M)

#define USBCKCR_USBCLKSEL (1 << 0)

struct MSTP {
    volatile uint32_t MSTPCRB;
    volatile uint32_t MSTPCRC;
    volatile uint32_t MSTPCRD;
};

/* OFS1 */
#define OFS1_HOCOFRQ1_M  0x7u
#define OFS1_HOCOFRQ1(x) (((x) & OFS1_HOCOFRQ1_M) << 12)
#define OFS1_HOCOEN      (1 << 8)
#define OFS1_VDSEL1_M    0x7u
#define OFS1_VDSEL1(x)   (((x) & OFS1_VDSEL1_M) << 3)
#define OFS1_LVDAS       (1 << 2)

/* instances */
static struct SYSTEM *SYSTEM = (struct SYSTEM*)ADDR_SYSTEM;
static struct MSTP *MSTP = (struct MSTP*)ADDR_MSTP;

static struct {
    unsigned long mosc;
    unsigned long hoco;
    unsigned long pll;
    unsigned long main;
    /* export */
    clock_freq_t fck;
    clock_freq_t ick;
    clock_freq_t pcka;
    clock_freq_t pckb;
    clock_freq_t pckc;
    clock_freq_t pckd;
} clocks;

#define MOCO_HZ 8000000ul
#define SOSC_HZ 32768ul
#define LOCO_HZ 32768ul
#define IWDT_HZ 15000000ul

static void unlock(void)
{
    SYSTEM->PRCR = (uint16_t)(PRCR_PRKEY(0xa5) | PRCR_PRC0);
}

static void lock(void)
{
    SYSTEM->PRCR = (uint16_t)PRCR_PRKEY(0xa5);
}

static int hoco_get_freq(/*@out@*/ unsigned long *freq)
{
    /* FIXME */
    uint32_t OFS1 = *(uint32_t*)0x404;
    uint8_t HOCOWTCR = (uint8_t)HOCOWTCR_HSTS(5);

    switch ((OFS1 >> 12) & OFS1_HOCOFRQ1_M) {
    case 0: *freq = 24000000ul; break;
    case 2: *freq = 32000000ul; break;
    case 4: *freq = 48000000ul; break;
    case 5:
        *freq = 64000000ul;
        HOCOWTCR = (uint8_t)HOCOWTCR_HSTS(6);
        break;

    default:
        *freq = 0;
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    SYSTEM->HOCOWTCR = HOCOWTCR;
    return 0;
}

static int setup_main_oscillator(struct clock_settings *settings)
{
    picoRTOS_assert(settings->mosel < CLOCK_RENESAS_RA4_MOSEL_COUNT, return -EINVAL);
    picoRTOS_assert(settings->mosc <= 20000000ul, return -EINVAL);

    if (settings->mosc > 10000000ul) SYSTEM->MOMCR &= ~MOMCR_MODRV1;
    else SYSTEM->MOMCR |= MOMCR_MODRV1;

    if (settings->mosel == CLOCK_RENESAS_RA4_MOSEL_RESONATOR)
        SYSTEM->MOMCR &= ~MOMCR_MOSEL;
    else
        SYSTEM->MOMCR |= MOMCR_MOSEL;

    clocks.mosc = settings->mosc;
    return 0;
}

static int setup_pll(unsigned long hz)
{
    picoRTOS_assert(hz != 0, return -EINVAL);
    return -ENOSYS; /* No MOSC on Arduino UNO R4 */
}

static int setup_hoco(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while (deadlock-- != 0)
        if ((SYSTEM->OSCSF & OSCSF_HOCOSF) != 0)
            break;

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    if (clocks.hoco > 32000000ul)
        SYSTEM->MEMWAIT = (uint8_t)MEMWAIT_MEMWAIT;

    clocks.main = clocks.hoco;
    return 0;
}

static int setup_clock_source(struct clock_settings *settings)
{
    picoRTOS_assert(settings->cksel < CLOCK_RENESAS_RA4_CKSEL_COUNT, return -EINVAL);

    int res = 0;

    switch (settings->cksel) {
    case CLOCK_RENESAS_RA4_CKSEL_HOCO: res = setup_hoco(); break;
    case CLOCK_RENESAS_RA4_CKSEL_MOCO: clocks.main = MOCO_HZ; break;
    case CLOCK_RENESAS_RA4_CKSEL_LOCO: clocks.main = LOCO_HZ; break;
    case CLOCK_RENESAS_RA4_CKSEL_MOSC: clocks.main = clocks.mosc; break;
    case CLOCK_RENESAS_RA4_CKSEL_SOSC: clocks.main = SOSC_HZ; break;
    case CLOCK_RENESAS_RA4_CKSEL_PLL: res = setup_pll(settings->pll); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    SYSTEM->SCKSCR = (uint8_t)SCKSCR_CKSEL(settings->cksel);
    return res;
}

/* Function: clock_renesas_ra4_init
 * Intializes the Renesas RA4 clock system
 *
 * Parameters:
 *  settings - The clocks settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_renesas_ra4_init(struct clock_settings *settings)
{
    picoRTOS_assert(settings->fck < CLOCK_RENESAS_RA4_SCKDIV_COUNT, return -EINVAL);
    picoRTOS_assert(settings->ick < CLOCK_RENESAS_RA4_SCKDIV_COUNT, return -EINVAL);
    picoRTOS_assert(settings->pcka < CLOCK_RENESAS_RA4_SCKDIV_COUNT, return -EINVAL);
    picoRTOS_assert(settings->pckb < CLOCK_RENESAS_RA4_SCKDIV_COUNT, return -EINVAL);
    picoRTOS_assert(settings->pckc < CLOCK_RENESAS_RA4_SCKDIV_COUNT, return -EINVAL);
    picoRTOS_assert(settings->pckd < CLOCK_RENESAS_RA4_SCKDIV_COUNT, return -EINVAL);

    int res;

    unlock();

    /* dividers first */
    SYSTEM->SCKDIVCR = (uint32_t)(SCKDIVCR_FCK(settings->fck) |
                                  SCKDIVCR_ICK(settings->ick) |
                                  SCKDIVCR_PCKA(settings->pcka) |
                                  SCKDIVCR_PCKB(settings->pckb) |
                                  SCKDIVCR_PCKC(settings->pckc) |
                                  SCKDIVCR_PCKD(settings->pckd));

    if ((res = hoco_get_freq(&clocks.hoco)) < 0 ||
        (res = setup_main_oscillator(settings)) < 0 ||
        (res = setup_clock_source(settings)) < 0) {
        /* don't forget to re-lock */
        lock();
        return res;
    }

    /* save dividers */
    clocks.fck = (clock_freq_t)(clocks.main / (1ul << settings->fck));
    clocks.ick = (clock_freq_t)(clocks.main / (1ul << settings->ick));
    clocks.pcka = (clock_freq_t)(clocks.main / (1ul << settings->pcka));
    clocks.pckb = (clock_freq_t)(clocks.main / (1ul << settings->pckb));
    clocks.pckc = (clock_freq_t)(clocks.main / (1ul << settings->pckc));
    clocks.pckd = (clock_freq_t)(clocks.main / (1ul << settings->pckd));

    lock();
    return 0;
}

/* Function: clock_renesas_ra4_module_start
 * Sends power/clock to the selected module
 *
 * Parameters:
 *  module - The module identifier (ex: MODULE_A(17), see doc)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_renesas_ra4_module_start(size_t module)
{
    picoRTOS_assert(module <= (size_t)MODULE_D(31), return -EINVAL);

    size_t shift = module & (size_t)0x1f;

    switch (module >> 5) {
    case 0: SYSTEM->MSTPCRA &= ~(uint32_t)(1 << shift); break;      /* A */
    case 1: MSTP->MSTPCRB &= ~(uint32_t)(1 << shift); break;        /* B */
    case 2: MSTP->MSTPCRC &= ~(uint32_t)(1 << shift); break;        /* C */
    case 3: MSTP->MSTPCRD &= ~(uint32_t)(1 << shift); break;        /* D */
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    return 0;
}

/* Function: clock_renesas_ra4_module_stop
 * Stops power/clock to the selected module
 *
 * Parameters:
 *  module - The module identifier (ex: MODULE_B(2), see doc)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_renesas_ra4_module_stop(size_t module)
{
    picoRTOS_assert(module <= (size_t)MODULE_D(31), return -EINVAL);

    size_t shift = module & (size_t)0x1f;

    switch (module >> 5) {
    case 0: SYSTEM->MSTPCRA |= (uint32_t)(1 << shift); break;       /* A */
    case 1: MSTP->MSTPCRB |= (uint32_t)(1 << shift); break;         /* B */
    case 2: MSTP->MSTPCRC |= (uint32_t)(1 << shift); break;         /* C */
    case 3: MSTP->MSTPCRD |= (uint32_t)(1 << shift); break;         /* D */
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    return 0;
}

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    picoRTOS_assert(clkid < (clock_id_t)CLOCK_RENESAS_RA4_COUNT,
                    return (clock_freq_t)-EINVAL);

    switch (clkid) {
    case CLOCK_RENESAS_RA4_FCLK: return clocks.fck;
    case CLOCK_RENESAS_RA4_ICLK: return clocks.ick;
    case CLOCK_RENESAS_RA4_PCLKA: return clocks.pcka;
    case CLOCK_RENESAS_RA4_PCLKB: return clocks.pckb;
    case CLOCK_RENESAS_RA4_PCLKC: return clocks.pckb;
    case CLOCK_RENESAS_RA4_PCLKD: return clocks.pckc;
    case CLOCK_RENESAS_RA4_IWDTCLK: return (clock_freq_t)IWDT_HZ;
    /* TODO: add missing */
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return (clock_freq_t)-EIO;
}
