#include "picoRTOS.h"
#include "raspberry-pico.h"
#include "tasks.h"

int main(void)
{
    static struct raspberry_pico pico;

    (void)raspberry_pico_init(&pico);
    picoRTOS_init();

    picoRTOS_priority_t prio;
    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack4[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack5[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack6[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack7[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack8[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack9[CONFIG_DEFAULT_STACK_COUNT];

    /* TICK */
    picoRTOS_task_init(&task, tick_main, pico.GPIO19, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, (picoRTOS_priority_t)0);

    /* LEDs, strict deadlines, no round-robin */
    picoRTOS_task_init(&task, led0_main, pico.PWM4B, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, led1_main, pico.PWM4B, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* SPI & ADC (round-robin) */
    prio = picoRTOS_get_next_available_priority();
    picoRTOS_task_init(&task, spi_main, pico.SPI, stack3, PICORTOS_STACK_COUNT(stack3));
    (void)picoRTOS_add_task(&task, prio);
    picoRTOS_task_init(&task, adc_main, pico.ADC0, stack4, PICORTOS_STACK_COUNT(stack4));
    (void)picoRTOS_add_task(&task, prio);

    /* I2C (round-robin) */
    prio = picoRTOS_get_next_available_priority();
    picoRTOS_task_init(&task, twi_master_main, pico.I2C0, stack5, PICORTOS_STACK_COUNT(stack5));
    (void)picoRTOS_add_task(&task, prio);
    picoRTOS_task_init(&task, twi_slave_main, pico.I2C1, stack6, PICORTOS_STACK_COUNT(stack6));
    (void)picoRTOS_add_task(&task, prio);

    /* PWM */
    picoRTOS_task_init(&task, pwm_main, pico.PWM5A, stack7, PICORTOS_STACK_COUNT(stack7));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* IPWM */
    picoRTOS_task_init(&task, ipwm_main, pico.PWM2B, stack8, PICORTOS_STACK_COUNT(stack8));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* WD, keep last */
    picoRTOS_task_init(&task, wd_main, pico.WDT, stack9, PICORTOS_STACK_COUNT(stack9));
    (void)picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
