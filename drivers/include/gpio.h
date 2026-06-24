#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <errno.h>
#include <stdbool.h>

struct gpio;

/* Enum: GPIO inversion
 *
 * GPIO_INVERT_IGNORE - Ignore GPIO inversion req
 * GPIO_INVERT_DISABLE - Disable inversion
 * GPIO_INVERT_ENABLE - Enable inversion
 */
typedef enum {
    GPIO_INVERT_IGNORE,
    GPIO_INVERT_DISABLE,
    GPIO_INVERT_ENABLE,
    GPIO_INVERT_COUNT
} gpio_invert_t;

struct gpio_settings {
    gpio_invert_t invert;
};

/**
 * int **gpio_setup**(**struct gpio** \*<ins>ctx</ins>,
 * const **struct gpio_settings** \*<ins>settings</ins>);
 * > Configures a gpio
 *
 * ### RETURN
 * Returns always 0
 */
int gpio_setup(struct gpio *ctx, const struct gpio_settings *settings);

/**
 * void **gpio_write**(**struct gpio** \*<ins>ctx/<ins>, **bool** <ins>value</ins>);
 * > Sets gpio value
 */
void gpio_write(struct gpio *ctx, bool value);

/**
 * bool **gpio_read**(**struct gpio** \*<ins>ctx</ins>);
 * > Read gpio value
 *
 * ### RETURN
 * > Returns true if the gpio is high, false otherwise
 */
bool gpio_read(struct gpio *ctx);

/**
 * void **gpio_toggle**(**struct gpio** \*<ins>ctx</ins>);
 * > Toggles a gpio value
 */
void gpio_toggle(struct gpio *ctx);

#endif
