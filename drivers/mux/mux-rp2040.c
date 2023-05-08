#include "mux-rp2040.h"
#include "picoRTOS.h"

#include <stdint.h>

struct MUX_RP2040_IO {
    struct MUX_RP2040_GPIO {
        volatile uint32_t GPIOn_STATUS;
        volatile uint32_t GPIOn_CTRL;
    } GPIO[MUX_RP2040_PIN_COUNT];
    volatile uint32_t INTRn[4];
    struct {
        volatile uint32_t INTEn[4];
        volatile uint32_t INTFn[4];
        volatile uint32_t INTSn[4];
    } PROC[2];
    volatile uint32_t DORMANT_WAKE_INTEn[4];
    volatile uint32_t DORMANT_WAKE_INTFn[4];
    volatile uint32_t DORMANT_WAKE_INTSn[4];
};

#define GPIOn_STATUS_IRQTOPROC   (1 << 26)
#define GPIOn_STATUS_IRQFROMPAD  (1 << 24)
#define GPIOn_STATUS_INTOPERI    (1 << 19)
#define GPIOn_STATUS_INFROMPAD   (1 << 17)
#define GPIOn_STATUS_OETOPAD     (1 << 13)
#define GPIOn_STATUS_OEFROMPERI  (1 << 12)
#define GPIOn_STATUS_OUTTOPAD    (1 << 9)
#define GPIOn_STATUS_OUTFROMPERI (1 << 8)

#define GPIOn_CTRL_IRQOVER_M  0x3u
#define GPIOn_CTRL_IRQOVER(x) (((x) & GPIOn_CTRL_IRQOVER_M) << 28)
#define GPIOn_CTRL_INOVER_M   0x3u
#define GPIOn_CTRL_INOVER(x)  (((x) & GPIOn_CTRL_INOVER_M) << 16)
#define GPIOn_CTRL_OEOVER_M   0x3u
#define GPIOn_CTRL_OEOVER(x)  (((x) & GPIOn_CTRL_OEOVER_M) << 12)
#define GPIOn_CTRL_OUTOVER_M  0x3u
#define GPIOn_CTRL_OUTOVER(x) (((x) & GPIOn_CTRL_OUTOVER_M) << 8)
#define GPIOn_CTRL_FUNCSEL_M  0x1fu
#define GPIOn_CTRL_FUNCSEL(x) ((x) & GPIOn_CTRL_FUNCSEL_M)

struct MUX_RP2040_PADS {
    volatile uint32_t VOLTAGE_SELECT;
    volatile uint32_t GPIOn[MUX_RP2040_PIN_COUNT];
    volatile uint32_t SWCLK;
    volatile uint32_t SWD;
};

#define GPIOn_OD       (1 << 7)
#define GPIOn_IE       (1 << 6)
#define GPIOn_DRIVE_M  0x3u
#define GPIOn_DRIVE(x) (((x) & GPIOn_DRIVE_M) << 4)
#define GPIOn_PUE      (1 << 3)
#define GPIOn_PDE      (1 << 2)
#define GPIOn_SCHMITT  (1 << 1)
#define GPIOn_SLEWFAST (1 << 0)

/* Function: mux_rp2040_init
 * Initializes a port for muxing
 *
 * Parameters:
 *  ctx - The mux object to init
 *  io_base - The IO port base address
 *  pads_base - The address of the pads associated with the IO port
 *
 * Returns:
 * Always 0
 */
int mux_rp2040_init(struct mux *ctx, int io_base, int pads_base)
{
    ctx->io = (struct MUX_RP2040_IO*)io_base;
    ctx->pads = (struct MUX_RP2040_PADS*)pads_base;
    return 0;
}

/* Function: mux_rp2040_io
 * Sets a pin as input AND output
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to setup on that particular port
 *  funcsel - The function to set this pin to
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_rp2040_io(struct mux *ctx, size_t pin, mux_rp2040_t funcsel)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_RP2040_PIN_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(funcsel < MUX_RP2040_COUNT)) return -EINVAL;

    struct MUX_RP2040_GPIO *gpio = &ctx->io->GPIO[pin];

    /* IO */
    gpio->GPIOn_CTRL = (uint32_t)GPIOn_CTRL_FUNCSEL(funcsel);

    /* PADS */
    ctx->pads->GPIOn[pin] = (uint32_t)(GPIOn_IE |
                                       GPIOn_DRIVE(MUX_RP2040_DRIVE_4mA) |
                                       GPIOn_SCHMITT);

    return 0;
}

/* Function: mux_rp2040_input
 * Sets a pin as input only
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to setup on that particular port
 *  funcsel - The function to set this pin to
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_rp2040_input(struct mux *ctx, size_t pin, mux_rp2040_t funcsel)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_RP2040_PIN_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(funcsel < MUX_RP2040_COUNT)) return -EINVAL;

    struct MUX_RP2040_GPIO *gpio = &ctx->io->GPIO[pin];

    /* IO */
    gpio->GPIOn_CTRL = (uint32_t)GPIOn_CTRL_FUNCSEL(funcsel);

    /* PADS */
    ctx->pads->GPIOn[pin] = (uint32_t)(GPIOn_OD | GPIOn_IE |
                                       GPIOn_DRIVE(MUX_RP2040_DRIVE_4mA) |
                                       GPIOn_SCHMITT);

    return 0;
}

/* Function: mux_rp2040_output
 * Sets a pin as output only
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to setup on that particular port
 *  funcsel - The function to set this pin to
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_rp2040_output(struct mux *ctx, size_t pin, mux_rp2040_t funcsel)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_RP2040_PIN_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(funcsel < MUX_RP2040_COUNT)) return -EINVAL;

    struct MUX_RP2040_GPIO *gpio = &ctx->io->GPIO[pin];

    /* IO */
    gpio->GPIOn_CTRL = (uint32_t)GPIOn_CTRL_FUNCSEL(funcsel);

    /* PADS */
    ctx->pads->GPIOn[pin] = (uint32_t)(GPIOn_DRIVE(MUX_RP2040_DRIVE_4mA) |
                                       GPIOn_PDE | GPIOn_SCHMITT);

    return 0;
}

/* Function: mux_rp2040_pull_up
 * Pulls up a specific mux pin
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to pull up
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_rp2040_pull_up(struct mux *ctx, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_RP2040_PIN_COUNT)) return -EINVAL;

    /* PADS */
    ctx->pads->GPIOn[pin] |= GPIOn_PUE;
    ctx->pads->GPIOn[pin] &= ~GPIOn_PDE;

    return 0;
}

/* Function: mux_rp2040_pull_down
 * Pulls down a specific mux pin
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to pull down
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_rp2040_pull_down(struct mux *ctx, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_RP2040_PIN_COUNT)) return -EINVAL;

    /* PADS */
    ctx->pads->GPIOn[pin] |= GPIOn_PDE;
    ctx->pads->GPIOn[pin] &= ~GPIOn_PUE;

    return 0;
}

/* Function: mux_rp2040_override
 * Overrides a specific pin value
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to override
 *  over - The type of override to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_rp2040_override(struct mux *ctx, size_t pin, mux_rp2040_over_t over)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_RP2040_PIN_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(over < MUX_RP2040_OVER_COUNT)) return -EINVAL;

    struct MUX_RP2040_GPIO *gpio = &ctx->io->GPIO[pin];

    gpio->GPIOn_CTRL &= ~GPIOn_CTRL_OUTOVER(GPIOn_CTRL_OUTOVER_M);
    gpio->GPIOn_CTRL |= GPIOn_CTRL_OUTOVER(over);

    return 0;
}

/* Function: mux_rp2040_set_drive
 * Sets the drive strength of a pin
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to override
 *  drive - The drive strength to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_rp2040_set_drive(struct mux *ctx, size_t pin, mux_rp2040_drive_t drive)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_RP2040_PIN_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(drive < MUX_RP2040_DRIVE_COUNT)) return -EINVAL;

    /* PADS */
    ctx->pads->GPIOn[pin] &= ~GPIOn_DRIVE(GPIOn_DRIVE_M);
    ctx->pads->GPIOn[pin] |= GPIOn_DRIVE(drive);

    return 0;
}
