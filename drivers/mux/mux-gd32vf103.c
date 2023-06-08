#include "mux-gd32vf103.h"
#include "picoRTOS.h"

struct MUX_GD32VF103_GPIO {
    volatile uint32_t GPIOx_CTLn[2];
    volatile uint32_t GPIOx_ISTAT;
    volatile uint32_t GPIOx_OCTL;
    volatile uint32_t GPIOx_BOP;
    volatile uint32_t GPIOx_BC;
    volatile uint32_t GPIOx_LOCK;
};

#define GPIOx_CTL0_CTL0_M  0x3u
#define GPIOx_CTL0_CTL0(x) (((x) & GPIOx_CTL0_CTL0_M) << 2)
#define GPIOx_CTL0_MD0_M   0x3u
#define GPIOx_CTL0_MD0(x)  ((x) & GPIOx_CTL0_MD0_M)

#include "picoRTOS_device.h"

struct AFIO {
    volatile uint32_t AFIO_EC;
    volatile uint32_t AFIO_PCF0;
    volatile uint32_t AFIO_EXTISSn[4];
    uint32_t RESERVED0;
    volatile uint32_t AFIO_PCF1;
};

static struct AFIO *AFIO = (struct AFIO*)ADDR_AFIO;

/* Function: mux_gd32vf103_init
 * Initializes a gpio port for muxing
 *
 * Parameters:
 *  ctx - The mux object to init
 *  base - The gpio port base address
 *
 * Returns:
 * Always 0
 */
int mux_gd32vf103_init(struct mux *ctx, int base)
{
    ctx->base = (struct MUX_GD32VF103_GPIO*)base;
    return 0;
}

/* Function: mux_gd32vf103_analog
 * Sets a pin as analog input
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to set to analog
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_gd32vf103_analog(struct mux *ctx, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_GD32VF103_PIN_COUNT)) return -EINVAL;

    size_t index = pin >> 3;
    size_t shift = (size_t)((0x7u & pin) * 4u);
    uint32_t reg = ctx->base->GPIOx_CTLn[index];

    reg &= ~((GPIOx_CTL0_CTL0(GPIOx_CTL0_CTL0_M) |
              GPIOx_CTL0_MD0(GPIOx_CTL0_MD0_M)) << shift);

    ctx->base->GPIOx_CTLn[index] = reg;
    return 0;
}

/* Function: mux_gd32vf103_input
 * Sets a pin as input
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to setup
 *  mode - The input pull mode
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_gd32vf103_input(struct mux *ctx, size_t pin, mux_gd32vf103_input_t mode)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_GD32VF103_PIN_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(mode < MUX_GD32VF103_INPUT_COUNT)) return -EINVAL;

    size_t index = pin >> 3;
    size_t shift = (size_t)((0x7u & pin) * 4u);
    uint32_t reg = ctx->base->GPIOx_CTLn[index];

    /* zero */
    reg &= ~((GPIOx_CTL0_CTL0(GPIOx_CTL0_CTL0_M) |
              GPIOx_CTL0_MD0(GPIOx_CTL0_MD0_M)) << shift);

    /* input */
    switch (mode) {
    case MUX_GD32VF103_INPUT_FLOATING:
        reg |= (GPIOx_CTL0_CTL0(1) << shift);
        break;

    case MUX_GD32VF103_INPUT_PULL_DOWN:
        reg |= (GPIOx_CTL0_CTL0(2) << shift);
        ctx->base->GPIOx_BC = (uint32_t)(1 << pin);
        break;

    case MUX_GD32VF103_INPUT_PULL_UP:
        reg |= (GPIOx_CTL0_CTL0(2) << shift);
        ctx->base->GPIOx_BOP = (uint32_t)(1 << pin);
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    ctx->base->GPIOx_CTLn[index] = reg;
    return 0;
}

/* Function: mux_gd32vf103_output
 * Sets a pin as output
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to setup
 *  mode - The output mode (OD/PP)
 *  speed - The port max supported speed
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_gd32vf103_output(struct mux *ctx, size_t pin,
                         mux_gd32vf103_output_t mode,
                         mux_gd32vf103_speed_t speed)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_GD32VF103_PIN_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(mode < MUX_GD32VF103_OUTPUT_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(speed < MUX_GD32VF103_SPEED_COUNT)) return -EINVAL;

    size_t index = pin >> 3;
    size_t shift = (size_t)((0x7u & pin) * 4u);
    uint32_t reg = ctx->base->GPIOx_CTLn[index];

    /* zero */
    reg &= ~((GPIOx_CTL0_CTL0(GPIOx_CTL0_CTL0_M) |
              GPIOx_CTL0_MD0(GPIOx_CTL0_MD0_M)) << shift);

    /* output */
    reg |= (GPIOx_CTL0_CTL0(mode) << shift);

    /* speed */
    switch (speed) {
    case MUX_GD32VF103_SPEED_10MHZ: reg |= (GPIOx_CTL0_MD0(1) << shift); break;
    case MUX_GD32VF103_SPEED_2MHZ: reg |= (GPIOx_CTL0_MD0(2) << shift); break;
    case MUX_GD32VF103_SPEED_50MHZ: reg |= (GPIOx_CTL0_MD0(3) << shift); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    ctx->base->GPIOx_CTLn[index] = reg;
    return 0;
}

/* Function: mux_gd32vf103_afio
 * Sets a pin as atlernate / AFIO
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to setup
 *  mode - The afio mode (OD/PP)
 *  speed - The port max supported speed
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_gd32vf103_afio(struct mux *ctx, size_t pin,
                       mux_gd32vf103_afio_t mode,
                       mux_gd32vf103_speed_t speed)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_GD32VF103_PIN_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(mode < MUX_GD32VF103_AFIO_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(speed < MUX_GD32VF103_SPEED_COUNT)) return -EINVAL;

    size_t index = pin >> 3;
    size_t shift = (size_t)((0x7u & pin) * 4u);
    uint32_t reg = ctx->base->GPIOx_CTLn[index];

    /* zero */
    reg &= ~((GPIOx_CTL0_CTL0(GPIOx_CTL0_CTL0_M) |
              GPIOx_CTL0_MD0(GPIOx_CTL0_MD0_M)) << shift);

    /* afio */
    if (mode == MUX_GD32VF103_AFIO_PUSH_PULL) reg |= (GPIOx_CTL0_CTL0(2) << shift);
    else reg |= (GPIOx_CTL0_CTL0(3) << shift);

    /* speed */
    switch (speed) {
    case MUX_GD32VF103_SPEED_10MHZ: reg |= (GPIOx_CTL0_MD0(1) << shift); break;
    case MUX_GD32VF103_SPEED_2MHZ: reg |= (GPIOx_CTL0_MD0(2) << shift); break;
    case MUX_GD32VF103_SPEED_50MHZ: reg |= (GPIOx_CTL0_MD0(3) << shift); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EINVAL;
    }

    ctx->base->GPIOx_CTLn[index] = reg;
    return 0;
}

static int set_afio_pcf0(size_t shift, uint32_t mask, uint32_t value)
{
    if (!picoRTOS_assert(shift < (size_t)32)) return -EINVAL;
    if (!picoRTOS_assert(value <= mask)) return -EINVAL;

    uint32_t reg = AFIO->AFIO_PCF0;

    reg &= ~(mask << shift);
    reg |= (value & mask) << shift;

    AFIO->AFIO_PCF0 = reg;
    return 0;
}

/* Function: mux_gd32vf103_afio_remap
 * Remaps an AFIO (see doc)
 *
 * Parameters:
 *  remap - The function to remap
 *  value - The remap value (see RM)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_gd32vf103_afio_remap(mux_gd32vf103_afio_remap_t remap, uint32_t value)
{
    if (!picoRTOS_assert(remap < MUX_GD32VF103_AFIO_REMAP_COUNT)) return -EINVAL;

    switch (remap) {
    case MUX_GD32VF103_AFIO_REMAP_TIMER1IT1:
        return set_afio_pcf0((size_t)29, (uint32_t)0x1, value);
    case MUX_GD32VF103_AFIO_REMAP_SPI2:
        return set_afio_pcf0((size_t)28, (uint32_t)0x1, value);
    case MUX_GD32VF103_AFIO_REMAP_SWJ_CFG:
        return set_afio_pcf0((size_t)24, (uint32_t)0x7, value);
    case MUX_GD32VF103_AFIO_REMAP_CAN1:
        return set_afio_pcf0((size_t)22, (uint32_t)0x1, value);
    case MUX_GD32VF103_AFIO_REMAP_TIMER4CH3:
        return set_afio_pcf0((size_t)16, (uint32_t)0x1, value);
    case MUX_GD32VF103_AFIO_REMAP_PD01:
        return set_afio_pcf0((size_t)15, (uint32_t)0x1, value);
    case MUX_GD32VF103_AFIO_REMAP_CAN0:
        return set_afio_pcf0((size_t)13, (uint32_t)0x3, value);
    case MUX_GD32VF103_AFIO_REMAP_TIMER3:
        return set_afio_pcf0((size_t)12, (uint32_t)0x1, value);
    case MUX_GD32VF103_AFIO_REMAP_TIMER2:
        return set_afio_pcf0((size_t)10, (uint32_t)0x3, value);
    case MUX_GD32VF103_AFIO_REMAP_TIMER1:
        return set_afio_pcf0((size_t)8, (uint32_t)0x3, value);
    case MUX_GD32VF103_AFIO_REMAP_TIMER0:
        return set_afio_pcf0((size_t)6, (uint32_t)0x3, value);
    case MUX_GD32VF103_AFIO_REMAP_USART2:
        return set_afio_pcf0((size_t)4, (uint32_t)0x3, value);
    case MUX_GD32VF103_AFIO_REMAP_USART1:
        return set_afio_pcf0((size_t)3, (uint32_t)0x1, value);
    case MUX_GD32VF103_AFIO_REMAP_USART0:
        return set_afio_pcf0((size_t)2, (uint32_t)0x1, value);
    case MUX_GD32VF103_AFIO_REMAP_I2C0:
        return set_afio_pcf0((size_t)1, (uint32_t)0x1, value);
    case MUX_GD32VF103_AFIO_REMAP_SPI0:
        return set_afio_pcf0((size_t)0, (uint32_t)0x1, value);

    default:
        break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}
