#include "mux-n76e003.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#ifndef S_SPLINT_S
__sfr __at(ADDR_P0M1) P0M1;
__sfr __at(ADDR_P0M2) P0M2;
__sfr __at(ADDR_P1M1) P1M1;
__sfr __at(ADDR_P1M2) P1M2;
__sfr __at(ADDR_P3M1) P3M1;
__sfr __at(ADDR_P3M2) P3M2;
#else
static uint8_t P0M1;
static uint8_t P0M2;
static uint8_t P1M1;
static uint8_t P1M2;
static uint8_t P3M1;
static uint8_t P3M2;
#endif

int mux_n76e003_init(/*@out@*/ struct mux *ctx, size_t port)
{
    picoRTOS_assert(port < (size_t)MUX_N76E003_PORT_COUNT, return -EINVAL);

    ctx->port = port;
    return 0;
}

int mux_n76e003_mode(struct mux *ctx, size_t pin, mux_n76e003_mode_t mode)
{
    picoRTOS_assert(pin < (size_t)MUX_N76E003_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(mode < MUX_N76E003_MODE_COUNT, return -EINVAL);

    uint8_t mask = (uint8_t)(1 << pin);
    uint8_t PxM2 = (((uint8_t)mode & 0x1) != 0 ? mask : 0);
    uint8_t PxM1 = (((uint8_t)mode & 0x2) != 0 ? mask : 0);

    switch (ctx->port) {
    case 0:
        P0M1 = PxM1;
        P0M2 = PxM2;
        break;

    case 1:
        P1M1 = PxM1;
        P1M2 = PxM2;
        break;

    case 2:
        break;

    case 3:
        P3M1 = PxM1;
        P3M2 = PxM2;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}
