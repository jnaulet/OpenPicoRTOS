#ifndef MUX_GD32VF103_H
#define MUX_GD32VF103_H

#include "mux.h"
#include <stddef.h>
#include <stdint.h>

#define MUX_GD32VF103_PIN_COUNT 16

struct MUX_GD32VF103_GPIO;

struct mux {
    /*@temp@*/ struct MUX_GD32VF103_GPIO *base;
};

int mux_gd32vf103_init(/*@out@*/ struct mux *ctx, int base);

typedef enum {
    MUX_GD32VF103_INPUT_FLOATING,
    MUX_GD32VF103_INPUT_PULL_DOWN,
    MUX_GD32VF103_INPUT_PULL_UP,
    MUX_GD32VF103_INPUT_COUNT
} mux_gd32vf103_input_t;

typedef enum {
    MUX_GD32VF103_OUTPUT_PUSH_PULL,
    MUX_GD32VF103_OUTPUT_OPEN_DRAIN,
    MUX_GD32VF103_OUTPUT_COUNT
} mux_gd32vf103_output_t;

typedef enum {
    MUX_GD32VF103_AFIO_PUSH_PULL,
    MUX_GD32VF103_AFIO_OPEN_DRAIN,
    MUX_GD32VF103_AFIO_COUNT
} mux_gd32vf103_afio_t;

typedef enum {
    MUX_GD32VF103_SPEED_10MHZ,
    MUX_GD32VF103_SPEED_2MHZ,
    MUX_GD32VF103_SPEED_50MHZ,
    MUX_GD32VF103_SPEED_COUNT,
} mux_gd32vf103_speed_t;

int mux_gd32vf103_analog(struct mux *ctx, size_t pin);
int mux_gd32vf103_input(struct mux *ctx, size_t pin, mux_gd32vf103_input_t mode);
int mux_gd32vf103_output(struct mux *ctx, size_t pin, mux_gd32vf103_output_t mode, mux_gd32vf103_speed_t speed);
int mux_gd32vf103_afio(struct mux *ctx, size_t pin, mux_gd32vf103_afio_t mode, mux_gd32vf103_speed_t speed);

/* remap */

typedef enum {
    MUX_GD32VF103_AFIO_REMAP_TIMER1IT1,
    MUX_GD32VF103_AFIO_REMAP_SPI2,
    MUX_GD32VF103_AFIO_REMAP_SWJ_CFG,
    MUX_GD32VF103_AFIO_REMAP_CAN1,
    MUX_GD32VF103_AFIO_REMAP_TIMER4CH3,
    MUX_GD32VF103_AFIO_REMAP_PD01,
    MUX_GD32VF103_AFIO_REMAP_CAN0,
    MUX_GD32VF103_AFIO_REMAP_TIMER3,
    MUX_GD32VF103_AFIO_REMAP_TIMER2,
    MUX_GD32VF103_AFIO_REMAP_TIMER1,
    MUX_GD32VF103_AFIO_REMAP_TIMER0,
    MUX_GD32VF103_AFIO_REMAP_USART2,
    MUX_GD32VF103_AFIO_REMAP_USART1,
    MUX_GD32VF103_AFIO_REMAP_USART0,
    MUX_GD32VF103_AFIO_REMAP_I2C0,
    MUX_GD32VF103_AFIO_REMAP_SPI0,
    MUX_GD32VF103_AFIO_REMAP_COUNT
} mux_gd32vf103_afio_remap_t;

int mux_gd32vf103_afio_remap(mux_gd32vf103_afio_remap_t remap, uint32_t value);

#endif
