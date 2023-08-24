#include "mux-pic32mx.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>
#include "misc-pic32mx.h"

struct MUX_PIC32MX {
    struct PIC32MX_CLR_SET_INV ANSEL;
    struct PIC32MX_CLR_SET_INV TRIS;
    struct PIC32MX_CLR_SET_INV PORT;
    struct PIC32MX_CLR_SET_INV LAT;
    struct PIC32MX_CLR_SET_INV ODC;
    struct PIC32MX_CLR_SET_INV CNPU;
    struct PIC32MX_CLR_SET_INV CNPD;
    struct PIC32MX_CLR_SET_INV CNCON;
    struct PIC32MX_CLR_SET_INV CNEN;
    struct PIC32MX_CLR_SET_INV CNSTAT;
    struct PIC32MX_CLR_SET_INV CNNE;
    struct PIC32MX_CLR_SET_INV CNF;
    /* opt on A & B */
    struct PIC32MX_CLR_SET_INV SRCON0;
    struct PIC32MX_CLR_SET_INV SRCON1;
};

#define PPSI_REG ((volatile uint32_t*)(ADDR_CONFIGURATION + 0x1404))
#define PPSO_REG ((volatile uint32_t*)(ADDR_CONFIGURATION + 0x1538))

#define PPSI_INPUT_M  0xfu
#define PPSI_INPUT(x) ((x) & PPSI_INPUT_M)

#define PPSO_OUTPUT_M  0xfu
#define PPSO_OUTPUT(x) ((x) & PPSO_OUTPUT_M)

/* Function: mux_pic32mx_init
 * Initialises a mux PORT
 *
 * Parameters:
 *  ctx - The mux context to init
 *  base - The mux port base address
 *
 * Returns:
 * Always 0
 */
int mux_pic32mx_init(struct mux_pic32mx *ctx, int base)
{
    ctx->base = (struct MUX_PIC32MX*)base;
    return 0;
}

/* Function: mux_pic32mx_analog
 * Configures a pin for analog input
 *
 * Parameters:
 *  ctx - A previously initialized mux context
 *  pin - The pin to configure
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_pic32mx_analog(struct mux_pic32mx *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_PIC32MX_PIN_COUNT, return -EINVAL);

    ctx->base->ANSEL.SET = (uint32_t)(1 << pin);
    ctx->base->TRIS.SET = (uint32_t)(1 << pin);
    return 0;
}

/* Function: mux_pic32mx_input
 * Configures a pin for digital input
 *
 * Parameters:
 *  ctx - A previously initialized mux context
 *  pin - The pin to configure
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_pic32mx_input(struct mux_pic32mx *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_PIC32MX_PIN_COUNT, return -EINVAL);

    ctx->base->ANSEL.CLR = (uint32_t)(1 << pin);
    ctx->base->TRIS.SET = (uint32_t)(1 << pin);
    return 0;
}

/* Function: mux_pic32mx_output
 * Configures a pin for digital output
 *
 * Parameters:
 *  ctx - A previously initialized mux context
 *  pin - The pin to configure
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_pic32mx_output(struct mux_pic32mx *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_PIC32MX_PIN_COUNT, return -EINVAL);

    ctx->base->ANSEL.CLR = (uint32_t)(1 << pin);
    ctx->base->TRIS.CLR = (uint32_t)(1 << pin);
    return 0;
}

/* Function: mux_pic32mx_peripheral_pin_select_input
 * Configures a PPS input
 *
 * Parameters:
 *  ppsi - The input PPS to configure
 *  opt - The PPS register value (see RM)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_pic32mx_peripheral_pin_select_input(mux_pic32mx_ppsi_t ppsi, size_t opt)
{
    picoRTOS_assert(ppsi < MUX_PIC32MX_PPSI_COUNT, return -EINVAL);
    picoRTOS_assert(opt < (size_t)MUX_PIC32MX_PPSx_OPT_COUNT, return -EINVAL);

    PPSI_REG[(size_t)ppsi] = (uint32_t)PPSI_INPUT(opt);
    return 0;
}

/* Function: mux_pic32mx_peripheral_pin_select_output
 * Configures a PPS output
 *
 * Parameters:
 *  ppso - The output PPS to configure
 *  opt - The PPS register value (see RM)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_pic32mx_peripheral_pin_select_output(mux_pic32mx_ppso_t ppso, size_t opt)
{
    picoRTOS_assert(ppso < MUX_PIC32MX_PPSO_COUNT, return -EINVAL);
    picoRTOS_assert(opt < (size_t)MUX_PIC32MX_PPSx_OPT_COUNT, return -EINVAL);

    PPSO_REG[(size_t)ppso] = (uint32_t)PPSO_OUTPUT(opt);
    return 0;
}

/* Function: mux_pic32mx_pull_up
 * Adds a pull up to the selected pin
 *
 * Parameters:
 *  ctx - The mux context
 *  pin - The pin to pull up
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_pic32mx_pull_up(struct mux_pic32mx *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_PIC32MX_PIN_COUNT, return -EINVAL);

    ctx->base->CNPU.SET = (uint32_t)(1 << pin);
    ctx->base->CNPD.CLR = (uint32_t)(1 << pin);
    return 0;
}

/* Function: mux_pic32mx_pull_down
 * Adds a pull down to the selected pin
 *
 * Parameters:
 *  ctx - The mux context
 *  pin - The pin to pull down
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_pic32mx_pull_down(struct mux_pic32mx *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_PIC32MX_PIN_COUNT, return -EINVAL);

    ctx->base->CNPD.SET = (uint32_t)(1 << pin);
    ctx->base->CNPU.CLR = (uint32_t)(1 << pin);
    return 0;
}
