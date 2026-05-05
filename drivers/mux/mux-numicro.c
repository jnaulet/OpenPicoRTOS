#include "mux-numicro.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#ifndef S_SPLINT_S
__sfr __at(ADDR_P0M1) P0M1;
__sfr __at(ADDR_P0M2) P0M2;
__sfr __at(ADDR_P1M1) P1M1;
__sfr __at(ADDR_P1M2) P1M2;
__sfr __at(ADDR_P2M1) P2M1;
__sfr __at(ADDR_P2M2) P2M2;
__sfr __at(ADDR_P3M1) P3M1;
__sfr __at(ADDR_P3M2) P3M2;
#else
static uint8_t P0M1;
static uint8_t P0M2;
static uint8_t P1M1;
static uint8_t P1M2;
static uint8_t P2M1;
static uint8_t P2M2;
static uint8_t P3M1;
static uint8_t P3M2;
#endif

/* Function: mux_numicro_init
 * Initializes a port for muxing
 *
 * Parameters:
 *  ctx - The mux object to init
 *  port - The IO port index [0-3]
 *
 * Returns:
 * Always 0
 */
int mux_numicro_init(struct mux *ctx, size_t port)
{
    picoRTOS_assert(port < (size_t)MUX_NUMICRO_PORT_COUNT, return -EINVAL);

    ctx->port = port;
    return 0;
}

/* Function: mux_numicro_mode
 * Sets pin mode
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to setup
 *  mode - The pin mode
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
/* cppcheck-suppress [constParameterPointer] */
int mux_numicro_mode(struct mux *ctx, size_t pin, mux_numicro_mode_t mode)
{
    picoRTOS_assert(pin < (size_t)MUX_NUMICRO_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(mode < MUX_NUMICRO_MODE_COUNT, return -EINVAL);

    uint8_t mask = (uint8_t)(1 << pin);
    uint8_t PxM2 = (((uint8_t)mode & 0x1) != 0 ? mask : 0);
    uint8_t PxM1 = (((uint8_t)mode & 0x2) != 0 ? mask : 0);

    switch (ctx->port) {
    case 0:
        P0M1 |= PxM1;
        P0M2 |= PxM2;
        P0M1 &= ~(PxM1 ^ mask);
        P0M2 &= ~(PxM2 ^ mask);
        break;

    case 1:
        P1M1 |= PxM1;
        P1M2 |= PxM2;
        P1M1 &= ~(PxM1 ^ mask);
        P1M2 &= ~(PxM2 ^ mask);
        break;

    case 2:
        P2M1 |= PxM1;
        P2M2 |= PxM2;
        P2M1 &= ~(PxM1 ^ mask);
        P2M2 &= ~(PxM2 ^ mask);
        break;

    case 3:
        P3M1 |= PxM1;
        P3M2 |= PxM2;
        P3M1 &= ~(PxM1 ^ mask);
        P3M2 &= ~(PxM2 ^ mask);
        break;

    default:
        picoRTOS_assert_void(false);
        /*@notreached@*/ return -EIO;
    }

    return 0;
}
