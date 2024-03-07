#include "mux-stm32f10xxx.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct MUX_STM32F10XXX {
    volatile uint32_t GPIOx_CRL;
    volatile uint32_t GPIOx_CRH;
};

#define GPIOx_CRL_CNF0_M   0x3u
#define GPIOx_CRL_CNF0(x)  (((x) & GPIOx_CRL_CNF0_M) << 2)
#define GPIOx_CRL_MODE0_M  0x3u
#define GPIOx_CRL_MODE0(x) ((x) & GPIOx_CRL_MODE0_M)

#define GPIOx_CRH_CNF8_M   0x3u
#define GPIOx_CRH_CNF8(x)  (((x) & GPIOx_CRH_MODE8_M) << 2)
#define GPIOx_CRH_MODE8_M  0x3u
#define GPIOx_CRH_MODE8(x) ((x) & GPIOx_CRH_MODE8_M)

struct MUX_STM32F10XXX_AFIO {
    volatile uint32_t AFIO_EVCR;
    volatile uint32_t AFIO_MAPR; /* ! */
    volatile uint32_t AFIO_EXTICR1;
    volatile uint32_t AFIO_EXTICR2;
    volatile uint32_t AFIO_EXTICR3;
    volatile uint32_t AFIO_EXTICR4;
    volatile uint32_t RESERVED0;
    volatile uint32_t AFIO_MAPR2;
};

#define AFIO_EVCR_EVOE    (1 << 7)
#define AFIO_EVCR_PORT_M  0x7u
#define AFIO_EVCR_PORT(x) (((x) & AFIO_EVCR_PORT_M) << 4)
#define AFIO_EVCR_PIN_M   0xfu
#define AFIO_EVCR_PIN(x)  ((x) & AFIO_EVCR_PIN_M)

/* low-, medium-, high- and XL- density devices */
#define AFIO_MAPR_SWJ_CFG_M          0x7u
#define AFIO_MAPR_SWJ_CFG(x)         (((x) & AFIO_MAPR_SWJ_CFG_M) << 24)
#define AFIO_MAPR_ADC2_ETRGREG_REMAP (1 << 20)
#define AFIO_MAPR_ADC2_ETRGINJ_REMAP (1 << 19)
#define AFIO_MAPR_ADC1_ETRGREG_REMAP (1 << 18)
#define AFIO_MAPR_ADC1_ETRGINJ_REMAP (1 << 17)
#define AFIO_MAPR_TIM5CH4_REMAP      (1 << 16)
#define AFIO_MAPR_PD01_REMAP         (1 << 15)
#define AFIO_MAPR_CAM_REMAP_M        0x3u
#define AFIO_MAPR_CAN_REMAP(x)       (((x) & AFIO_MAPR_CAM_REMAP_M) << 13)
#define AFIO_MAPR_TIM4_REMAP         (1 <<  12)
#define AFIO_MAPR_TIM3_REMAP_M       0x3u
#define AFIO_MAPR_TIM3_REMAP(x)      (((x) & AFIO_MAPR_TIM3_REMAP_M) << 10)
#define AFIO_MAPR_TIM2_REMAP_M       0x3u
#define AFIO_MAPR_TIM2_REMAP(x)      (((x) & AFIO_MAPR_TIM2_REMAP_M) << 8)
#define AFIO_MAPR_TIM1_REMAP_M       0x3u
#define AFIO_MAPR_TIM1_REMAP(x)      (((x) & AFIO_MAPR_TIM1_REMAP_M) << 6)
#define AFIO_MAPR_USART3_REMAP_M     0x3u
#define AFIO_MAPR_USART3_REMAP(x)    (((x) & AFIO_MAPR_USART3_REMAP_M) << 4)
#define AFIO_MAPR_USART2_REMAP       (1 << 3)
#define AFIO_MAPR_USART1_REMAP       (1 << 2)
#define AFIO_MAPR_I2C1_REMAP         (1 << 1)
#define AFIO_MAPR_SPI1_REMAP         (1 << 0)

#define AFIO_EXTICR1_EXTI0_M  0xfu
#define AFIO_EXTICR1_EXTI0(x) ((x) & AFIO_EXTICR1_EXTI0_M)

#define AFIO_EXTICR2_EXTI4_M  0xfu
#define AFIO_EXTICR2_EXTI4(x) ((x) & AFIO_EXTICR2_EXTI4_M)

#define AFIO_EXTICR3_EXTI8_M  0xfu
#define AFIO_EXTICR3_EXTI8(x) ((x) & AFIO_EXTICR3_EXTI8_M)

#define AFIO_EXTICR4_EXTI12_M  0xfu
#define AFIO_EXTICR4_EXTI12(x) ((x) & AFIO_EXTICR4_EXTI12_M)

#define AFIO_MAPR2_FSMC_NADV   (1 << 10)
#define AFIO_MAPR2_TIM14_REMAP (1 << 9)
#define AFIO_MAPR2_TIM13_REMAP (1 << 8)
#define AFIO_MAPR2_TIM11_REMAP (1 << 7)
#define AFIO_MAPR2_TIM10_REMAP (1 << 6)
#define AFIO_MAPR2_TIM9_REMAP  (1 << 5)

/* always the same */
// static struct MUX_STM32F10XXX_AFIO *AFIO = (struct MUX_STM32F10XXX_AFIO*)ADDR_AFIO;

int mux_stm32f10xxx_init(struct mux *ctx, int base)
{
    ctx->base = (struct MUX_STM32F10XXX*)base;
    return 0;
}

static int set_cnf_mode(struct mux *ctx, size_t pin, uint32_t cnf, uint32_t mode)
{
    picoRTOS_assert(pin < (size_t)MUX_STM32F10XXX_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(cnf <= (uint32_t)GPIOx_CRL_CNF0_M, return -EINVAL);
    picoRTOS_assert(mode <= (uint32_t)GPIOx_CRL_MODE0_M, return -EINVAL);

    size_t shift;

    if (pin < (size_t)8) {
        shift = pin << 2;
        ctx->base->GPIOx_CRL &= ~(GPIOx_CRL_CNF0(GPIOx_CRL_CNF0_M) << shift);
        ctx->base->GPIOx_CRL |= GPIOx_CRL_CNF0(cnf) << shift;
        ctx->base->GPIOx_CRL &= ~(GPIOx_CRL_MODE0(GPIOx_CRL_MODE0_M) << shift);
        ctx->base->GPIOx_CRL |= GPIOx_CRL_MODE0(mode) << shift;
    }else{
        shift = (pin - (size_t)8) << 2;
        ctx->base->GPIOx_CRH &= ~(GPIOx_CRH_CNF8(GPIOx_CRH_CNF8_M) << shift);
        ctx->base->GPIOx_CRH |= GPIOx_CRH_CNF8(cnf) << shift;
        ctx->base->GPIOx_CRH &= ~(GPIOx_CRH_MODE8(GPIOx_CRH_CNF8_M) << shift);
        ctx->base->GPIOx_CRH |= GPIOx_CRH_MODE8(mode) << shift;
    }

    return 0;
}

int mux_stm32f10xxx_analog(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_STM32F10XXX_PIN_COUNT, return -EINVAL);
    return set_cnf_mode(ctx, pin, 0, 0);
}

int mux_stm32f10xxx_output(struct mux *ctx, size_t pin, mux_stm32f10xxx_output_t type)
{
    picoRTOS_assert(pin < (size_t)MUX_STM32F10XXX_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(type < MUX_STM32F10XXX_OUTPUT_COUNT, return -EINVAL);

    uint32_t cnf = 0;

    if (type == MUX_STM32F10XXX_OUTPUT_PUSH_PULL) cnf = (uint32_t)0;
    else cnf = (uint32_t)1;

    return set_cnf_mode(ctx, pin, cnf, (uint32_t)1); /* FIXME */
}

int mux_stm32f10xxx_input(struct mux *ctx, size_t pin, mux_stm32f10xxx_input_t type)
{
    picoRTOS_assert(pin < (size_t)MUX_STM32F10XXX_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(type < MUX_STM32F10XXX_INPUT_COUNT, return -EINVAL);

    uint32_t cnf = 0;

    if (type == MUX_STM32F10XXX_INPUT_FLOATING) cnf = (uint32_t)1;
    else cnf = (uint32_t)2;

    return set_cnf_mode(ctx, pin, cnf, 0);
}

int mux_stm32f10xxx_alt(struct mux *ctx, size_t pin, mux_stm32f10xxx_alt_t type)
{
    picoRTOS_assert(pin < (size_t)MUX_STM32F10XXX_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(type < MUX_STM32F10XXX_ALT_COUNT, return -EINVAL);

    uint32_t cnf = 0;

    if (type == MUX_STM32F10XXX_ALT_PUSH_PULL) cnf = (uint32_t)2;
    else cnf = (uint32_t)3;

    return set_cnf_mode(ctx, pin, cnf, (uint32_t)1); /* ! */
}
