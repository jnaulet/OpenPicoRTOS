#ifndef TIMER_PIC32MX_H
#define TIMER_PIC32MX_H

/* TODO: might benefit from a timer HAL */

#include <stdbool.h>

struct TIMER_PIC32MX;

typedef long timer_period_us_t;

struct timer_pic32mx {
    /*@temp@*/ struct TIMER_PIC32MX *base;
    timer_period_us_t period;
    bool is_32bit; /* TODO: us that really useful ? */
};

struct timer_pic32mx_settings {
    timer_period_us_t period;
    bool t32_mode;
};

int timer_pic32mx_init(/*@out@*/ struct timer *ctx, struct TIMER_PIC32MX *base, clock_id_t clkid);
int timer_pic32mx_setup(struct timer *ctx, struct timer_pic32mx_settings *settings);

void timer_pic32mx_start(struct timer *ctx);
void timer_pic32mx_stop(struct timer *ctx);

#endif
