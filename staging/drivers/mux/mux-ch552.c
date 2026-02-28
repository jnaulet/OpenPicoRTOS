#include "mux-ch552.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#ifndef S_SPLINT_S
__sfr __at(ADDR_P1_MOD_OC) P1_MOD_OC;
__sfr __at(ADDR_P1_DIR_PU) P1_DIR_PU;
__sfr __at(ADDR_P3_MOD_OC) P3_MOD_OC;
__sfr __at(ADDR_P3_DIR_PU) P3_DIR_PU;
#else
static unsigned char P1_MOD_OC;
static unsigned char P1_DIR_PU;
static unsigned char P3_MOD_OC;
static unsigned char P3_DIR_PU;
#endif

int mux_ch552_init(struct mux *ctx, mux_ch552_port_t port)
{
    picoRTOS_assert(port < MUX_CH552_PORT_COUNT, return -EINVAL);

    ctx->port = port;
    return 0;
}

/* cppcheck-suppress [constParameterPointer] */
int mux_ch552_mode(struct mux *ctx, size_t pin, mux_ch552_mode_t mode)
{
    picoRTOS_assert(pin < (size_t)MUX_CH552_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(mode < MUX_CH552_MODE_COUNT, return -EINVAL);

    uint8_t mask = (uint8_t)(1 << pin);
    uint8_t Px_DIR_PU = (((uint8_t)mode & 0x1) != 0 ? mask : 0);
    uint8_t Px_MOD_OC = (((uint8_t)mode & 0x2) != 0 ? mask : 0);

    switch (ctx->port) {
    case MUX_CH552_PORT_P1:
        P1_MOD_OC |= Px_MOD_OC;
        P1_DIR_PU |= Px_DIR_PU;
        P1_MOD_OC &= ~(Px_MOD_OC ^ mask);
        P1_DIR_PU &= ~(Px_DIR_PU ^ mask);
        break;

    case MUX_CH552_PORT_P2:
        break;

    case MUX_CH552_PORT_P3:
        P3_MOD_OC |= Px_MOD_OC;
        P3_DIR_PU |= Px_DIR_PU;
        P3_MOD_OC &= ~(Px_MOD_OC ^ mask);
        P3_DIR_PU &= ~(Px_DIR_PU ^ mask);
        break;

    default:
        picoRTOS_assert_void(false);
        /*@notreached@*/ return -EIO;
    }

    return 0;
}
