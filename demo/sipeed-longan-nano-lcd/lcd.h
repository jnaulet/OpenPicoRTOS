#ifndef LCD_H
#define LCD_H

#define LCD_WIDTH              160
#define LCD_HEIGHT             80
#define LCD_FRAMEBUFFER_PIXELS (LCD_WIDTH * LCD_HEIGHT)
#define LCD_FRAMEBUFFER_BYTES  (LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t))

#include "spi.h"
#include "gpio.h"
#include <stdint.h>

struct lcd_phys {
    /*@temp@*/ struct spi *spi;
    /*@temp@*/ struct gpio *cs;
    /*@temp@*/ struct gpio *dc;
    /*@temp@*/ struct gpio *rst;
};

struct lcd {
    /*@temp@*/ struct spi *spi;
    /*@temp@*/ struct gpio *cs;
    /*@temp@*/ struct gpio *dc;
    /*@temp@*/ struct gpio *rst;
    /* frame buffer */
    uint8_t fb[LCD_FRAMEBUFFER_BYTES]; /* LCD Color: RGB565 (MSB rrrrrggggggbbbbb LSB) */
};

int lcd_init(/*@out@*/ struct lcd *ctx, struct lcd_phys *phys);
int lcd_refresh(struct lcd *ctx);

#endif
