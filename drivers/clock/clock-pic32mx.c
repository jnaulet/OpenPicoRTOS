#include "clock-pic32mx.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>
#include "misc-pic32mx.h"

#define CLOCK_PIC32MX_REFOx_COUNT 4
#define CLOCK_PIC32MX_PBCLK_COUNT 8

struct OSCILLATOR_CONTROL {
    struct PIC32MX_CLR_SET_INV OSCCON;
    struct PIC32MX_CLR_SET_INV OSCTUN;
    struct PIC32MX_CLR_SET_INV SPLLCON;
    uint32_t RESERVED0[20];
    struct {
        struct PIC32MX_CLR_SET_INV CON;
        struct PIC32MX_CLR_SET_INV TRIM;
    } REFOx[CLOCK_PIC32MX_REFOx_COUNT];
    struct PIC32MX_CLR_SET_INV PBxDIV[CLOCK_PIC32MX_PBCLK_COUNT];
    uint32_t RESERVED1[20];
    struct PIC32MX_CLR_SET_INV SLEWCON;
    struct PIC32MX_CLR_SET_INV CLKSTAT;
};

#define OSCCON_FRCDIV_M  0x7u
#define OSCCON_FRCDIV(x) (((x) & OSCCON_FRCDIV_M) << 24)
#define OSCCON_DRMEN     (1 << 23)
#define OSCCON_SLP2SPD   (1 << 21)
#define OSCCON_COSC_M    0x7u
#define OSCCON_COSC(x)   (((x) & OSCCON_COSC_M) << 12)
#define OSCCON_NOSC_M    0x7u
#define OSCCON_NOSC(x)   (((x) & OSCCON_COSC_M) << 8)
#define OSCCON_CLKLOCK   (1 << 7)
#define OSCCON_ULOCK     (1 << 6)
#define OSCCON_SLOCK     (1 << 5)
#define OSCCON_SLPEN     (1 << 4)
#define OSCCON_CF        (1 << 3)
#define OSCCON_SOSCEN    (1 << 1)
#define OSCCON_OSWEN     (1 << 0)

#define OSCTUN_TUN_M  0x3fu
#define OSCTUN_TUN(x) ((x) & OSCTUN_TUN_M)

#define SPLLCON_PLLODIV_M   0x7u
#define SPLLCON_PLLODIV(x)  (((x) & SPLLCON_PLLODIV_M) << 24)
#define SPLLCON_PLLMULT_M   0x7fu
#define SPLLCON_PLLMULT(x)  (((x) & SPLLCON_PLLMULT_M) << 16)
#define SPLLCON_PLLIDIV_M   0x7u
#define SPLLCON_PLLIDIV(x)  (((x) & SPLLCON_PLLIDIV_M) << 8)
#define SPLLCON_PLLICLK     (1 << 7)
#define SPLLCON_PLLRANGE_M  0x7u
#define SPLLCON_PLLRANGE(x) ((x) & SPLLCON_PLLRANGE_M)

#define REFOxCON_RODIV_M  0x7fffu
#define REFOxCON_RODIV(x) (((x) & REFOxCON_RODIV_M) << 16)
#define REFOxCON_ON       (1 << 15)
#define REFOxCON_SIDL     (1 << 13)
#define REFOxCON_OE       (1 << 12)
#define REFOxCON_RSLP     (1 << 11)
#define REFOxCON_DIVSWEN  (1 << 9)
#define REFOxCON_ACTIVE   (1 << 8)
#define REFOxCON_ROSEL_M  0xfu
#define REFOxCON_ROSEL(x) ((x) & REFOxCON_ROSEL_M)

#define REFOxTRIM_ROTRIM_M  0x1ffu
#define REFOxTRIM_ROTRIM(x) (((x) & REFOxTRIM_ROTRIM_M) << 23)

#define PBxDIV_ON       (1 << 15)
#define PBxDIV_PBIVRDY  (1 << 11)
#define PBxDIV_PBDIV_M  0x7fu
#define PBxDIV_PBDIV(x) ((x) & PBxDIV_PBDIV_M)

#define SLEWCON_SYSDIV_M  0xfu
#define SLEWCON_SYSDIV(x) (((x) & SLEWCON_SYSDIV_M) << 16)
#define SLEWCON_SLWDIV_M  0x7u
#define SLEWCON_SLWDIV(x) (((x) & SLEWCON_SLWDIV_M) << 8)
#define SLEWCON_UPEN      (1 << 2)
#define SLEWCON_DNEN      (1 << 1)
#define SLEWCON_BUSY      (1 << 0)

#define CLKSTAT_LPRCDRY    (1 << 5)
#define CLKSTAT_SOSCRDY    (1 << 4)
#define CLKSTAT_POSCRDY    (1 << 2)
#define CLKSTAT_DIVSPLLRDY (1 << 1)
#define CLKSTAT_FRCRDY     (1 << 0)

static struct OSCILLATOR_CONTROL *OSCILLATOR =
    (struct OSCILLATOR_CONTROL*)ADDR_OSCILLATOR;

/* default values */
#define USBCLK_DEFAULT 48000000
#define FRC_DEFAULT    8000000
#define BFRC_DEFAULT   8000000
#define LPRC_DEFAULT   32768
#define SOSC_DEFAULT   32768

static struct {
    /* internals */
    clock_freq_t posc;
    clock_freq_t plliclk;
    clock_freq_t pllclk;
    /* exported */
    clock_freq_t sysclk;
    clock_freq_t pbclk[CLOCK_PIC32MX_PBCLK_COUNT];
} clocks;

static int osccon_busywait(uint32_t mask)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while ((OSCILLATOR->OSCCON.REG & mask) == 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

static int configure_pll_input(clock_pic32mx_plliclk_t iclk)
{
    picoRTOS_assert(iclk < CLOCK_PIC32MX_PLLICLK_COUNT, return -EINVAL);

    int range;

    /* default: POSC */
    OSCILLATOR->SPLLCON.CLR = (uint32_t)SPLLCON_PLLICLK;
    OSCILLATOR->SPLLCON.CLR = (uint32_t)SPLLCON_PLLRANGE(SPLLCON_PLLRANGE_M);

    if (iclk == CLOCK_PIC32MX_PLLICLK_FRC) {
        OSCILLATOR->SPLLCON.SET = (uint32_t)SPLLCON_PLLICLK;
        OSCILLATOR->SPLLCON.SET = (uint32_t)SPLLCON_PLLRANGE(1);
        clocks.plliclk = (clock_freq_t)FRC_DEFAULT;
        return 0;
    }

    /* default: range has to be computed */
    if (clocks.posc < (clock_freq_t)10000000) range = 1;
    else if (clocks.posc < (clock_freq_t)16000000) range = 2;
    else if (clocks.posc < (clock_freq_t)26000000) range = 3;
    else if (clocks.posc < (clock_freq_t)42000000) range = 4;
    else if (clocks.posc < (clock_freq_t)64000000) range = 5;
    else{
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    OSCILLATOR->SPLLCON.SET = (uint32_t)SPLLCON_PLLRANGE(range);
    clocks.plliclk = clocks.posc;

    return 0;
}

static int configure_pll(clock_pic32mx_plliclk_t iclk,
                         unsigned long freq)
{
#define PLLODIV_P2_MAX  5ul
#define PLLMULT_MAX     128ul
#define PLLIDIV_MAX     8ul

    picoRTOS_assert(iclk < CLOCK_PIC32MX_PLLICLK_COUNT, return -EINVAL);

    int res;
    unsigned long pllodiv_p2;
    unsigned long pllmult;
    unsigned long pllidiv;

    if ((res = configure_pll_input(iclk)) < 0)
        return res;

    /* FIXME: far too many nested stuff here */
    for (pllodiv_p2 = 1ul; pllodiv_p2 <= PLLODIV_P2_MAX; pllodiv_p2++)
        for (pllmult = 1ul; pllmult <= PLLMULT_MAX; pllmult++)
            for (pllidiv = 1ul; pllidiv <= PLLIDIV_MAX; pllidiv++) {
                /* brute-force freq computation */
                unsigned long f;
                f = (((unsigned long)clocks.plliclk / pllidiv) * pllmult) / (1ul << pllodiv_p2);
                if (freq == f) {
                    /* clear pll */
                    OSCILLATOR->SPLLCON.CLR = (uint32_t)(SPLLCON_PLLODIV(SPLLCON_PLLODIV_M) |
                                                         SPLLCON_PLLMULT(SPLLCON_PLLMULT_M) |
                                                         SPLLCON_PLLIDIV(SPLLCON_PLLIDIV_M));
                    /* set pll */
                    OSCILLATOR->SPLLCON.SET = (uint32_t)(SPLLCON_PLLODIV(pllodiv_p2) |
                                                         SPLLCON_PLLMULT(pllmult - 1ul) |
                                                         SPLLCON_PLLIDIV(pllidiv - 1ul));

                    clocks.pllclk = (clock_freq_t)freq;
                    return 0;
                }
            }

    picoRTOS_break();
    /*@notreached@*/ return -EINVAL;
}

static int switch_osc(clock_pic32mx_osc_t osc)
{
    picoRTOS_assert(osc < CLOCK_PIC32MX_OSC_COUNT, return -EINVAL);
    picoRTOS_assert(osc != CLOCK_PIC32MX_OSC_RESERVED, return -EINVAL);

    OSCILLATOR->OSCCON.CLR = (uint32_t)OSCCON_NOSC(OSCCON_NOSC_M);
    OSCILLATOR->OSCCON.SET = (uint32_t)OSCCON_NOSC(osc);
    OSCILLATOR->OSCCON.SET = (uint32_t)OSCCON_OSWEN;

    switch (osc) {
    case CLOCK_PIC32MX_OSC_FRC: clocks.sysclk = (clock_freq_t)FRC_DEFAULT; break;
    case CLOCK_PIC32MX_OSC_SPLL: clocks.sysclk = clocks.pllclk; break;
    case CLOCK_PIC32MX_OSC_POSC: clocks.sysclk = clocks.posc; break;
    case CLOCK_PIC32MX_OSC_SOSC: clocks.sysclk = (clock_freq_t)SOSC_DEFAULT; break;
    case CLOCK_PIC32MX_OSC_LPRC: clocks.sysclk = (clock_freq_t)LPRC_DEFAULT; break;
    case CLOCK_PIC32MX_OSC_BFRC: clocks.sysclk = (clock_freq_t)BFRC_DEFAULT; break;
    /* Beware: this might change in the future */
    case CLOCK_PIC32MX_OSC_FRCDIV: clocks.sysclk = (clock_freq_t)FRC_DEFAULT; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    arch_set_clock_frequency((unsigned long)clocks.sysclk);
    return osccon_busywait((uint32_t)OSCCON_OSWEN);
}

/* Function: clock_pic32mx_init
 * Initializes the PIC32MX clock system
 *
 * Parameters:
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_pic32mx_init(struct clock_settings *settings)
{
    int res;

    /* unlock system */
    pic32mx_unlock();

    /* defaults */
    OSCILLATOR->OSCCON.SET = (uint32_t)OSCCON_SLP2SPD;
    OSCILLATOR->OSCCON.SET = (uint32_t)OSCCON_SOSCEN;       /* ? */
    OSCILLATOR->OSCCON.CLR = (uint32_t)OSCCON_SLPEN;        /* no sleep */

    /* 1st, remember POSC */
    clocks.posc = (clock_freq_t)settings->posc;

    /* 2nd: pll */
    if ((res = configure_pll(settings->plliclk, settings->pllo)) < 0) {
        pic32mx_lock();
        return res;
    }

    /* 3rd: final sysclk selection */
    res = switch_osc(settings->osc);
    pic32mx_lock();

    return res;
}

/* Function: clock_pic32mx_pbclk_enable
 * Enables a particular PB clock
 *
 * Parameters:
 *  clkid - The PB clock identifier (1-8)
 *  pbdiv - The clock divider to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_pic32mx_pbclk_enable(clock_id_t clkid, unsigned long pbdiv)
{
    picoRTOS_assert(clkid >= CLOCK_PIC32MX_PBCLK1, return -EINVAL);
    picoRTOS_assert(clkid <= CLOCK_PIC32MX_PBCLK8, return -EINVAL);
    picoRTOS_assert(pbdiv < CLOCK_PIC32MX_PBDIV_MAX, return -EINVAL);

    size_t index = (size_t)clkid - (size_t)CLOCK_PIC32MX_PBCLK1;

    pic32mx_unlock();
    OSCILLATOR->PBxDIV[index].REG = (uint32_t)(PBxDIV_ON | PBxDIV_PBDIV(pbdiv - 1ul));
    pic32mx_lock();

    clocks.pbclk[index] = clocks.sysclk / (clock_freq_t)pbdiv;
    return 0;
}

/* Function: clock_pic32mx_pbclk_disable
 * Disable a particular PB clock
 *
 * Parameters:
 *  clkid - The PB clock id (1-8)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int clock_pic32mx_pbclk_disable(clock_id_t clkid)
{
    picoRTOS_assert(clkid >= CLOCK_PIC32MX_PBCLK1, return -EINVAL);
    picoRTOS_assert(clkid <= CLOCK_PIC32MX_PBCLK8, return -EINVAL);

    size_t index = (size_t)clkid - (size_t)CLOCK_PIC32MX_PBCLK1;

    /* turn off */
    pic32mx_unlock();
    OSCILLATOR->PBxDIV[index].CLR = (uint32_t)PBxDIV_ON;
    pic32mx_lock();

    return 0;
}

/* hooks */

clock_freq_t clock_get_freq(clock_id_t clkid)
{
    picoRTOS_assert(clkid < (clock_id_t)CLOCK_PIC32MX_COUNT,
                    return (clock_freq_t)-EINVAL);

    switch (clkid) {
    case CLOCK_PIC32MX_SYSCLK: return clocks.sysclk;
    case CLOCK_PIC32MX_USBCLK: return (clock_freq_t)USBCLK_DEFAULT;
    case CLOCK_PIC32MX_LPRC: return (clock_freq_t)LPRC_DEFAULT;
    case CLOCK_PIC32MX_SOSC: return (clock_freq_t)SOSC_DEFAULT;
    case CLOCK_PIC32MX_PBCLK1: return clocks.pbclk[0];
    case CLOCK_PIC32MX_PBCLK2: return clocks.pbclk[1];
    case CLOCK_PIC32MX_PBCLK3: return clocks.pbclk[2];
    case CLOCK_PIC32MX_PBCLK4: return clocks.pbclk[3];
    case CLOCK_PIC32MX_PBCLK5: return clocks.pbclk[4];
    case CLOCK_PIC32MX_PBCLK6: return clocks.pbclk[5];
    case CLOCK_PIC32MX_PBCLK7: return clocks.pbclk[6];
    case CLOCK_PIC32MX_PBCLK8: return clocks.pbclk[7];
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return (clock_freq_t)-EINVAL;
}
