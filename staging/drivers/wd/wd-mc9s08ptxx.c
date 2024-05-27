#include "wd-mc9s08ptxx.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#define KEY_1ST 0xa602
#define KEY_2ND 0xb480

struct WD_MC9S08PTXX {
    uint8_t CS1;
    uint8_t CS2;
    uint8_t CNTH;
    uint8_t CNTL;
    uint8_t TOVALH;
    uint8_t TOVALL;
    uint8_t WINH;
    uint8_t WINL;
};

#define CS1_EN     (1 << 7)
#define CS1_INT    (1 << 6)
#define CS1_UPDATE (1 << 5)
#define CS1_TST_M  0x3u
#define CS1_TST(x) (((x) & CS1_TST_M) << 3)
#define CS1_DBG    (1 << 2)
#define CS1_WAIT   (1 << 1)
#define CS1_STOP   (1 << 0)

#define CS2_WIN    (1 << 7)
#define CS2_FLG    (1 << 6)
#define CS2_PRES   (1 << 4)
#define CS2_CLK_M  0x3u
#define CS2_CLK(x) ((x) & CS2_CLK_M)

int wd_mc9s08ptxx_wdt_init(struct wd *ctx, clock_id_t clkid)
{
    ctx->base = (struct WD_MC9S08PTXX*)ADDR_WDOG;
    ctx->clkid = clkid;

    return 0;
}

int wd_start(struct wd *ctx)
{
    ctx->base->CS1 |= CS1_EN;
    return 0;
}

int wd_stop(struct wd *ctx)
{
    ctx->base->CS1 &= ~CS1_EN;
    return 0;
}

void wd_refresh(/*@unused@*/ struct wd *ctx)
{
    /*@i@*/ (void)ctx;

    ASM(".include 'generated/picoRTOS_device.S'");

    ASM("pshh");
    ASM("pshx");

    ASM("ldhx #0xa602");
    ASM("sthx (ADDR_WDOG + 2)");
    ASM("ldhx #0xb480");
    ASM("sthx (ADDR_WDOG + 2)");

    ASM("pulx");
    ASM("pulh");
}
