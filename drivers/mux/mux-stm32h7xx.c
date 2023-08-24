#include "mux-stm32h7xx.h"
#include "picoRTOS.h"

#include <stdint.h>

struct MUX_STM32H7XX {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
};

#define MODER_MODER0_M  0x3u
#define MODER_MODER0(x) ((x) & MODER_MODER0_M)

#define OSPEEDR_OSPEEDR0_M  0x3u
#define OSPEEDR_OSPEEDR0(x) ((x) & OSPEEDR_OSPEEDR0_M)

#define PUPDR_PUPDR0_M  0x3u
#define PUPDR_PUPDR0(x) ((x) & PUPDR_PUPDR0_M)

#define AFRL_AFR0_M  0xfu
#define AFRL_AFR0(x) ((x) & AFRL_AFR0_M)

#define AFRH_AFR8_M  0xfu
#define AFRH_AFR8(x) ((x) & AFRH_AFR8_M)

/* Function: mux_stm32h7xx_init
 * Initializes a port for muxing
 *
 * Parameters:
 *  ctx - The mux object to init
 *  base - The GPIO port base address
 *
 * Returns:
 * Always 0
 */
int mux_stm32h7xx_init(struct mux *ctx, int base)
{
    ctx->base = (struct MUX_STM32H7XX*)base;
    return 0;
}

/* Function: mux_stm32h7xx_analog
 * Sets a pin as analog input
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to setup on that particular port
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_stm32h7xx_analog(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_STM32H7XX_PIN_COUNT, return -EINVAL);

    size_t lshift = pin * (size_t)2;

    ctx->base->MODER |= MODER_MODER0(MODER_MODER0_M) << lshift;
    return 0;
}

/* Function: mux_stm32h7xx_output
 * Sets a pin as discrete output
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to setup on that particular port
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_stm32h7xx_output(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_STM32H7XX_PIN_COUNT, return -EINVAL);

    size_t lshift = pin * (size_t)2;

    ctx->base->MODER &= ~(MODER_MODER0(MODER_MODER0_M) << lshift);
    ctx->base->MODER |= MODER_MODER0(0x1) << lshift;
    return 0;
}

/* Function: mux_stm32h7xx_input
 * Sets a pin as discrete input
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to setup on that particular port
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_stm32h7xx_input(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_STM32H7XX_PIN_COUNT, return -EINVAL);

    size_t lshift = pin * (size_t)2;

    ctx->base->MODER &= ~(MODER_MODER0(MODER_MODER0_M) << lshift);
    return 0;
}

/* Function: mux_stm32h7xx_alt
 * Sets a pin as alternate function pin
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to setup on that particular port
 *  alt - The alternate function (0-15, see datasheet)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_stm32h7xx_alt(struct mux *ctx, size_t pin, size_t alt)
{
    picoRTOS_assert(pin < (size_t)MUX_STM32H7XX_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(alt < (size_t)MUX_STM32H7XX_ALT_COUNT, return -EINVAL);

    /* mode */
    size_t mode_lshift = pin * (size_t)2;
    size_t alt_lshift = (size_t)0x1fu & (pin * (size_t)4);

    ctx->base->MODER &= ~(MODER_MODER0(MODER_MODER0_M) << mode_lshift);
    ctx->base->MODER |= MODER_MODER0(0x2) << mode_lshift;

    /* alt */
    if (pin < (size_t)8) {
        ctx->base->AFRL &= ~(AFRL_AFR0(AFRL_AFR0_M) << alt_lshift);
        ctx->base->AFRL |= AFRL_AFR0(alt) << alt_lshift;
    }else{
        ctx->base->AFRH &= ~(AFRH_AFR8(AFRH_AFR8_M) << alt_lshift);
        ctx->base->AFRH |= AFRH_AFR8(alt) << alt_lshift;
    }

    return 0;
}

/* Function: mux_stm32h7xx_pull_up
 * Pulls up a specific mux pin
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to pull up
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_stm32h7xx_pull_up(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_STM32H7XX_PIN_COUNT, return -EINVAL);

    size_t lshift = pin * (size_t)2;

    ctx->base->PUPDR &= ~(PUPDR_PUPDR0(PUPDR_PUPDR0_M) << lshift);
    ctx->base->PUPDR |= PUPDR_PUPDR0(0x1) << lshift;
    return 0;
}

/* Function: mux_stm32h7xx_pull_down
 * Pulls down a specific mux pin
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to pull down
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_stm32h7xx_pull_down(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_STM32H7XX_PIN_COUNT, return -EINVAL);

    size_t lshift = pin * (size_t)2;

    ctx->base->PUPDR &= ~(PUPDR_PUPDR0(PUPDR_PUPDR0_M) << lshift);
    ctx->base->PUPDR |= PUPDR_PUPDR0(0x2) << lshift;
    return 0;
}
