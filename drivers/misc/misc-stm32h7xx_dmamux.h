#ifndef MISC_STM32H7XX_DMAMUX_H
#define MISC_STM32H7XX_DMAMUX_H

#include <errno.h>
#include <stddef.h>

#define DMAMUX_STM32H7XX_CHANNEL_MAX 16

struct DMAMUX_STM32H7XX;

struct dmamux {
    /*@temp@*/ struct DMAMUX_STM32H7XX *base;
    size_t channel;
};

int dmamux_stm32h7xx_init(/*@out@*/ struct dmamux *ctx, int base, size_t channel);
int dmamux_stm32h7xx_set_dmareq_id(struct dmamux *ctx, unsigned long id);

#endif
