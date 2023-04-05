#ifndef MISC_RP2040_RESET_H
#define MISC_RP2040_RESET_H

#include <errno.h>

struct RP2040_RESET;

struct rp2040_reset {
    /*@temp@*/ struct RP2040_RESET *base;
};

typedef enum {
    RP2040_RESET_ADC        = 0,
    RP2040_RESET_BUSCTRL    = 1,
    RP2040_RESET_DMA        = 2,
    RP2040_RESET_I2C0       = 3,
    RP2040_RESET_I2C1       = 4,
    RP2040_RESET_IO_BANK0   = 5,
    RP2040_RESET_IO_QSPI    = 6,
    RP2040_RESET_JTAG       = 7,
    RP2040_RESET_PADS_BANK0 = 8,
    RP2040_RESET_PADS_QSPI  = 9,
    RP2040_RESET_PIO0       = 10,
    RP2040_RESET_PIO1       = 11,
    RP2040_RESET_PLL_SYS    = 12,
    RP2040_RESET_PLL_USB    = 13,
    RP2040_RESET_PWM        = 14,
    RP2040_RESET_RTC        = 15,
    RP2040_RESET_SPI0       = 16,
    RP2040_RESET_SPI1       = 17,
    RP2040_RESET_SYSCFG     = 18,
    RP2040_RESET_SYSINFO    = 19,
    RP2040_RESET_TBMAN      = 20,
    RP2040_RESET_TIMER      = 21,
    RP2040_RESET_UART0      = 22,
    RP2040_RESET_UART1      = 23,
    RP2040_RESET_USBCTRL    = 24,
    RP2040_RESET_COUNT
} rp2040_reset_t;

int rp2040_reset_init(struct rp2040_reset *ctx, struct RP2040_RESET *base);

int rp2040_reset(struct rp2040_reset *ctx, rp2040_reset_t ss);
int rp2040_unreset(struct rp2040_reset *ctx, rp2040_reset_t ss);

#endif
