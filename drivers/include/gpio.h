#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <errno.h>
#include <stdbool.h>

struct gpio;

struct gpio_settings {
    bool invert;
};

/* Function: gpio_setup
 * Configures a gpio
 *
 * Parameters:
 *  gpio - A gpio instance
 *  settings - The settings to apply
 *
 * Returns:
 * Always 0
 */
int gpio_setup(struct gpio *ctx, struct gpio_settings *settings);

/* Function: gpio_write
 * Sets a gpio to a value
 *
 * Parameters:
 *  gpio - A gpio instance
 *  value - The value to set
 */
void gpio_write(struct gpio *ctx, bool value);

/* Function: gpio_read
 * Read value from a gpio
 *
 * Parameters:
 *  gpio - A gpio instance
 *
 * Returns:
 * true if the gpio is high, false otherwise
 */
bool gpio_read(struct gpio *ctx);

/* Function: gpio_toggle
 * Toggles a gpio value
 *
 * Parameters:
 *  gpio - A gpio instance
 */
void gpio_toggle(struct gpio *ctx);

#endif
