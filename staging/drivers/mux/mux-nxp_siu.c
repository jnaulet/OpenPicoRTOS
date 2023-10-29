#include "mux-nxp_siu.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct MUX_NXP_SIU {
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

#define PCR_PA_M   0x7u
#define PCR_PA(x)  (((x) & PCR_PA_M) << 10)
#define PCR_OBE    (1 << 9)
#define PCR_IBE    (1 << 8)
#define PCR_DSC_M  0x3u
#define PCR_DSC(x) (((x) & PCR_DSC_M) << 6)
#define PCR_SRC_M  0x3u
#define PCR_SRC(x) (((x) & PCR_SRC_M) << 2)
#define PCR_WPE    (1 << 1)
#define PCR_WPS    (1 << 0)

int mux_nxp_siu_init(struct mux *ctx, int base)
{
    ctx->base = (struct MUX_NXP_SIU*)base;
    return 0;
}

int mux_nxp_siu_output(struct mux *ctx, size_t pin, mux_nxp_siu_t mux)
{
    picoRTOS_assert(pin < (size_t)MUX_NXP_SIU_PCR_COUNT, return -EINVAL);
    picoRTOS_assert(mux < MUX_NXP_SIU_COUNT, return -EINVAL);

    /* pa */
    ctx->base->PCR[pin] &= ~PCR_PA(PCR_PA_M);
    ctx->base->PCR[pin] |= PCR_PA(mux);
    /* obe */
    ctx->base->PCR[pin] |= PCR_OBE;
    ctx->base->PCR[pin] &= ~PCR_IBE;
    /* max dsc & src */
    ctx->base->PCR[pin] |= PCR_DSC(PCR_DSC_M);
    ctx->base->PCR[pin] |= PCR_SRC(PCR_SRC_M);

    return 0;
}

int mux_nxp_siu_input(struct mux *ctx, size_t pin, mux_nxp_siu_t mux)
{
    picoRTOS_assert(pin < (size_t)MUX_NXP_SIU_PCR_COUNT, return -EINVAL);
    picoRTOS_assert(mux < MUX_NXP_SIU_COUNT, return -EINVAL);

    /* pa */
    ctx->base->PCR[pin] &= ~PCR_PA(PCR_PA_M);
    ctx->base->PCR[pin] |= PCR_PA(mux);
    /* ibe */
    ctx->base->PCR[pin] |= PCR_IBE;
    ctx->base->PCR[pin] &= ~PCR_OBE;

    return 0;
}

int mux_nxp_siu_pull_up(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_NXP_SIU_PCR_COUNT, return -EINVAL);

    ctx->base->PCR[pin] |= (PCR_WPE | PCR_WPS);
    return 0;
}

int mux_nxp_siu_pull_down(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_NXP_SIU_PCR_COUNT, return -EINVAL);

    ctx->base->PCR[pin] |= PCR_WPE;
    ctx->base->PCR[pin] &= ~PCR_WPS;
    return 0;
}
