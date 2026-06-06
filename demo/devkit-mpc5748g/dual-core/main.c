#include "picoRTOS-SMP.h"
#include "devkit-mpc5748g.h"
#include "tasks.h"

int main(void)
{
    static struct devkit_mpc5748g board;

    if (devkit_mpc5748g_init(&board) < 0) {
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

    /* shared task */
    picoRTOS_task_init(&task, tick_main, &board.TICK, stack0, PICORTOS_STACK_COUNT(stack0));
    /*
     * BUG: tweaking these 2 following line might create a deadlock on the leds mutex
     * Not idea what's going on, this needs to be fixed
     */
    picoRTOS_SMP_add_task(&task, picoRTOS_get_next_available_priority(), (picoRTOS_mask_t)0x1);
    // picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* per core tasks */
    picoRTOS_task_init(&task, led0_main, &board.VU, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_SMP_add_task(&task, picoRTOS_get_next_available_priority(), (picoRTOS_mask_t)0x1);
    picoRTOS_task_init(&task, led1_main, &board.VU, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_SMP_add_task(&task, picoRTOS_get_next_available_priority(), (picoRTOS_mask_t)0x2);

    /* adc + pwm */
    picoRTOS_task_init(&task, adc_main, &board.ADC_PWM, stack3, PICORTOS_STACK_COUNT(stack3));
    // picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* spi */
    picoRTOS_task_init(&task, spi_main, &board.SPI, stack4, PICORTOS_STACK_COUNT(stack4));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* can */
    picoRTOS_task_init(&task, can_main, &board.CAN0, stack5, PICORTOS_STACK_COUNT(stack5));
    // picoRTOS_SMP_add_task(&task, picoRTOS_get_next_available_priority(), (picoRTOS_mask_t)0x1);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* uart */
    picoRTOS_task_init(&task, console_main, &board.UART, stack6, PICORTOS_STACK_COUNT(stack6));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
