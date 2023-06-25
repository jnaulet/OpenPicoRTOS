#ifndef BOARD_H
#define BOARD_H

#include "gpio.h"
#include "lcd.h"
#include "spi.h"
#include "wd.h"

struct board {
    /*@shared@*/ struct gpio *LED;
    /*@shared@*/ struct wd *WD;
    struct lcd_phys lcd_phys;
};

int board_init(/*@out@*/ struct board *ctx);

#endif
