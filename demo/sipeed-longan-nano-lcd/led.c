#include "led.h"
#include "gd32vf103.h"

// ------------------------------------------------------------------------

void led_init()
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1 | GPIO_PIN_2);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    GPIO_BOP(GPIOA) = GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_BOP(GPIOC) = GPIO_PIN_13;
}

void led_set(int c)                             // c[0:2] = RGB
{
    if (c & 1) GPIO_BC(GPIOC) = GPIO_PIN_13;    // red
    else GPIO_BOP(GPIOC) = GPIO_PIN_13;
    if (c & 2) GPIO_BC(GPIOA) = GPIO_PIN_1;     // green
    else GPIO_BOP(GPIOA) = GPIO_PIN_1;
    if (c & 4) GPIO_BC(GPIOA) = GPIO_PIN_2;     // blue
    else GPIO_BOP(GPIOA) = GPIO_PIN_2;
}

// ------------------------------------------------------------------------
