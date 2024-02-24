#ifndef LCD_H
#define LCD_H

#define LCD_WIDTH  160l
#define LCD_HEIGHT 80l
#define LCD_PIXELS (LCD_WIDTH * LCD_HEIGHT)
#define LCD_BYTES  (LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t))

#ifndef LCD_FB_DIVIDER
# define LCD_FB_DIVIDER 1
#endif

#define LCD_FB_PIXELS (LCD_PIXELS / LCD_FB_DIVIDER)

/* Some SPIs simply can't send a 16bit word correctly
 * example: SAMD51 / ATMEL SERCOM + DMA */
#if !defined(LCD_FB_SWAP16) || defined(S_SPLINT_S)
# define LCD_PX16(x) (x)
#else
# define LCD_PX16(x) __builtin_bswap16(x)
#endif

#include "spi.h"
#include "gpio.h"
#include <stdint.h>

#include "picoRTOS_types.h"

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
    /* frame buffer, LCD Color: RGB565 (MSB rrrrrggggggbbbbb LSB) */
    uint16_t fb[LCD_FB_PIXELS] __ATTRIBUTE_ALIGNED__(ARCH_L1_DCACHE_LINESIZE);
};

int lcd_init(/*@out@*/ struct lcd *ctx, struct lcd_phys *phys);
void lcd_zero(struct lcd *ctx);
int lcd_refresh(struct lcd *ctx);

#endif
