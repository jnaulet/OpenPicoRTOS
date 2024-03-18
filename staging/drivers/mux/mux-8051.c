#include "mux-8051.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#define MUX_8051_PORT_COUNT 4 /* default */

#ifdef ADDR_P4
# undef MUX_8051_PORT_COUNT
# define MUX_8051_PORT_COUNT 5
#endif
#ifdef ADDR_P5
# undef MUX_8051_PORT_COUNT
# define MUX_8051_PORT_COUNT 6
#endif

#ifndef S_SPLINT_S
__sfr __at(ADDR_P0M1) P0M1;
__sfr __at(ADDR_P0M0) P0M0;
__sfr __at(ADDR_P1M1) P1M1;
__sfr __at(ADDR_P1M0) P1M0;
__sfr __at(ADDR_P3M1) P3M1;
__sfr __at(ADDR_P3M0) P3M0;
# if MUX_8051_PORT_COUNT > 4
__sfr __at(ADDR_P4M1) P4M1;
__sfr __at(ADDR_P4M0) P4M0;
# if MUX_8051_PORT_COUNT > 5
__sfr __at(ADDR_P5M1) P5M1;
__sfr __at(ADDR_P5M0) P5M0;
# endif
# endif
#else
static uint8_t P0M1;
static uint8_t P0M0;
static uint8_t P1M1;
static uint8_t P1M0;
static uint8_t P3M1;
static uint8_t P3M0;
# if MUX_8051_PORT_COUNT > 4
static uint8_t P4M1;
static uint8_t P4M0;
# if MUX_8051_PORT_COUNT > 5
static uint8_t P5M1;
static uint8_t P5M0;
# endif
# endif
#endif

int mux_8051_init(struct mux *ctx, size_t port)
{
    picoRTOS_assert(port < (size_t)MUX_8051_PORT_COUNT, return -EINVAL);

    ctx->port = port;
    return 0;
}

int mux_8051_mode(struct mux *ctx, size_t pin, mux_8051_mode_t mode)
{
    picoRTOS_assert(pin < (size_t)MUX_8051_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(mode < MUX_8051_MODE_COUNT, return -EINVAL);

    uint8_t mask = (uint8_t)(1 << pin);
    uint8_t PxM0 = (((uint8_t)mode & 0x1) != 0 ? mask : 0);
    uint8_t PxM1 = (((uint8_t)mode & 0x2) != 0 ? mask : 0);

    switch (ctx->port) {
    case 0:
        P0M1 = PxM1;
        P0M0 = PxM0;
        break;

    case 1:
        P1M1 = PxM1;
        P1M0 = PxM0;
        break;

    case 2:
        break;

    case 3:
        P3M1 = PxM1;
        P3M0 = PxM0;
        break;
#ifdef ADDR_P4
    case 4:
        P4M1 = PxM1;
        P4M0 = PxM0;
        break;
#endif
#ifdef ADDR_P5
    case 5:
        P5M1 = PxM1;
        P5M0 = PxM0;
        break;
#endif

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}
