#include "picoRTOS-SMP.h"
#include "mpc5775e-evb.h"

#include "tasks.h"

int main(void)
{
    static struct mpc5775e_evb evb;

    if (mpc5775e_evb_init(&evb) < 0) {
        picoRTOS_assert_void(false);
        return -1;
    }

    picoRTOS_init();

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack4[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack5[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack6[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_task_init(&task, tick_main, &evb.TICK, stack0, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_task_init(&task, led0_main, &evb.LED0, stack1, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_SMP_add_task(&task, picoRTOS_get_next_available_priority(), (picoRTOS_mask_t)0x1);
    picoRTOS_task_init(&task, led1_main, &evb.LED1, stack2, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_SMP_add_task(&task, picoRTOS_get_next_available_priority(), (picoRTOS_mask_t)0x2);

    /* adc */
    picoRTOS_task_init(&task, adc_main, &evb.ADC, stack3, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* pwm */
    picoRTOS_task_init(&task, pwm_main, &evb.PWM, stack4, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* spi */
    picoRTOS_task_init(&task, spi_main, &evb.SPI, stack5, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* can */
    picoRTOS_task_init(&task, can_main, &evb.CAN, stack6, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
