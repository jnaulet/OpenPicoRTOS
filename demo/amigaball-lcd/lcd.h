#ifndef LCD_H
#define LCD_H

#define LCD_WIDTH              160l
#define LCD_HEIGHT             80l
#define LCD_FRAMEBUFFER_PIXELS (LCD_WIDTH * LCD_HEIGHT)
#define LCD_FRAMEBUFFER_BYTES  (LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t))

/* Some SPIs simply can't send a 16bit word correctly
 * exeample: SAMD51 / ATMEL SERCOM + DMA */
#ifndef LCD_FB_SWAP16
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
    uint16_t fb[LCD_FRAMEBUFFER_PIXELS]
    __attribute__ ((aligned(ARCH_L1_DCACHE_LINESIZE)));
};

int lcd_init(/*@out@*/ struct lcd *ctx, struct lcd_phys *phys);
int lcd_refresh(struct lcd *ctx);

#endif
