#include "picoRTOS.h"
#include "arduino-due.h"

#include "picoRTOS_mutex.h"
#include "picoRTOS_cond.h"


#define BLINK_PERIOD PICORTOS_DELAY_SEC(1)
#define BLINK_DELAY  PICORTOS_DELAY_MSEC(60)

struct blink {
    /* board */
    struct arduino_due due;
    /* ipcs */
    struct picoRTOS_mutex mutex;
    struct picoRTOS_cond cond;
};

/*
 * Blink test object init. Inits the board, starts the watchdog and configures IPCs
 */
static void blink_init(struct blink *ctx)
{
    /* board */
    (void)arduino_due_init(&ctx->due);

    /* watchdog */
    (void)wd_start(ctx->due.WDT);

    /* ipcs */
    picoRTOS_mutex_init(&ctx->mutex);
    picoRTOS_cond_init(&ctx->cond);
}

/*
 * tick_main bit bangs a GPIO to show the scheduler's activity and allows for measurements
 * of its tick period/drift
 */
static void tick_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    /* TICK is a gpio passed as a parameter from main
     * You can change it over there */
    struct gpio *TICK = (struct gpio*)priv;

    for (;;) {
        /* toggle pin */
        gpio_toggle(TICK);
        /* schedule to next tick / move to next task */
        picoRTOS_schedule();
    }
}

/*
 * blink_main ramps up PWM13 to allow a nice fading in of the builtin LED
 * This version uses IPCs (mutex and condition) to drive a second thread that fades
 * the LED out
 */
static void blink_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    picoRTOS_tick_t ref = picoRTOS_get_tick();
    struct blink *blink = (struct blink*)priv;

    (void)pwm_set_period(blink->due.PWM13, (pwm_period_us_t)100);
    pwm_start(blink->due.PWM13);

    for (;;) {

        size_t n = (size_t)100;
        pwm_duty_cycle_t duty_cycle = 0;

        picoRTOS_mutex_lock(&blink->mutex);

        /* fade in */
        while (n-- != 0) {
            (void)pwm_set_duty_cycle(blink->due.PWM13, duty_cycle);
            duty_cycle += PWM_DUTY_CYCLE_PCENT(1);
            picoRTOS_sleep(PICORTOS_DELAY_MSEC(5));
        }

        /* ipc */
        picoRTOS_cond_signal(&blink->cond);
        picoRTOS_mutex_unlock(&blink->mutex);

        /* schedule at 1s from ref */
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

/*
 * blink_again main waits for a condition sent by blink_main and fades out the
 * builtin LED using PWM13. Don't do this at home ! This is an IPC test, for normal conditions,
 * a state machine using a boolean is much more efficient (see demo/arduino-mega2560 for
 * real practical example)
 */
static void blink_again_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct blink *blink = (struct blink*)priv;

    for (;;) {

        size_t n = (size_t)99;
        pwm_duty_cycle_t duty_cycle = PWM_DUTY_CYCLE_PCENT(100);

        picoRTOS_mutex_lock(&blink->mutex);
        picoRTOS_cond_wait(&blink->cond, &blink->mutex);

        /* fade away */
        while (n-- != 0) {
            (void)pwm_set_duty_cycle(blink->due.PWM13, duty_cycle);
            duty_cycle -= PWM_DUTY_CYCLE_PCENT(1);
            picoRTOS_sleep(PICORTOS_DELAY_MSEC(5));
        }

        picoRTOS_mutex_unlock(&blink->mutex);
    }
}

/*
 * console_main is a thread that simply echoes characters
 * On arduino Due, it can be checked by using the USB serial port
 */
static void console_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct uart *UART = (struct uart*)priv;

    for (;;) {

        char c = (char)0;

        /* just echo */
        if (uart_read(UART, &c, sizeof(c)) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        (void)uart_write(UART, &c, sizeof(c));
    }
}

/*
 * spi_main is a thread that sends data to the SPI and expects to get it back.
 * Internal loopback is present on the Arduino Due, but we do it with a wire to allow for
 * measurements. Pins 1 & 4 of the SPI connector should be wired together
 */
static void spi_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    size_t xfered = 0;
    struct spi *SPI = (struct spi*)priv;
    char rx[6] = { (char)0, (char)0, (char)0, (char)0, (char)0, (char)0 };
    char tx[6] = { (char)0xa5, (char)0x55, (char)0x5a, (char)0x55, (char)0x4d, (char)0x4e };

    for (;;) {
        int res;

        if ((res = spi_xfer(SPI, &rx[xfered], &tx[xfered], sizeof(tx) - xfered)) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        picoRTOS_assert_void(res > 0);

        /* ack xfer */
        xfered += (size_t)res;

        if (xfered == sizeof(tx)) {
            picoRTOS_assert_void(rx[0] == (char)0xa5);
            picoRTOS_assert_void(rx[4] == (char)0x4d);
            /* start again */
            xfered = 0;
        }
    }
}

/*
 * Watchdog refreshing thread, set at the shortest value possible on SAM3X
 * Hint: a watchdog refreshing task must be set at the lowest priority to ensure
 * it is executed last
 */
static void wd_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct wd *WD = (struct wd*)priv;

    for (;;) {
        wd_refresh(WD);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(4));
    }
}

/*
 * This thread ramps up a PWM duty cycle from 0 to 100% by 0.1% increments
 * Period is set at 100us
 */
static void pwm_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    pwm_duty_cycle_t duty_cycle = 0;
    struct pwm *PWM = (struct pwm*)priv;

    /* init */
    (void)pwm_set_period(PWM, (pwm_period_us_t)100);
    (void)pwm_set_duty_cycle(PWM, 0);

    /* start */
    pwm_start(PWM);

    for (;;) {
        (void)pwm_set_duty_cycle(PWM, duty_cycle);
        duty_cycle += PWM_DUTY_CYCLE_PMIL(1);

        picoRTOS_schedule();
    }
}

/*
 * This thread measures the output period and duty_cycle of a PWM,
 * Period should be exactly 100ms, duty cycle varies (check in debugger)
 */
static void ipwm_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct ipwm *IPWM = (struct ipwm*)priv;

    /* wait for hw to init properly */
    picoRTOS_sleep(PICORTOS_DELAY_MSEC(10));

    for (;;) {
        pwm_period_us_t period = 0;
        pwm_duty_cycle_t duty_cycle = 0;
        int deadlock = CONFIG_DEADLOCK_COUNT;

        while (ipwm_get_period(IPWM, &period) == -EAGAIN && deadlock-- != 0)
            picoRTOS_schedule();

        /* check */
        picoRTOS_assert_void(deadlock != -1);
        picoRTOS_assert_void(period == (pwm_period_us_t)100);

        deadlock = CONFIG_DEADLOCK_COUNT;
        while (ipwm_get_duty_cycle(IPWM, &duty_cycle) == -EAGAIN && deadlock-- != 0)
            picoRTOS_schedule();

        /* final check */
        picoRTOS_assert_void(deadlock != -1);
    }
}

#define CAN_ID_M2S 0x6
#define CAN_ID_S2M 0x7

/*
 * This thread write data on ID 6 and reads it back (must be 7)
 */
static void can_master_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct can *CAN = (struct can*)priv;

    (void)can_accept(CAN, (can_id_t)CAN_ID_S2M, 0);

    for (;;) {
        int res;
        can_id_t id = 0;
        char msg[CAN_DATA_COUNT] = { (char)0, (char)1, (char)2, (char)3,
                                     (char)4, (char)5, (char)6, (char)7 };

        if ((res = can_write(CAN, (can_id_t)CAN_ID_M2S, msg, sizeof(msg))) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        while (can_read(CAN, &id, msg, (size_t)res) == -EAGAIN)
            picoRTOS_schedule();

        picoRTOS_assert_void(id == (can_id_t)CAN_ID_S2M);
    }
}

/*
 * This thread listens for CAN data on ID 6 and echoes it back on ID 7
 */
static void can_slave_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct can *CAN = (struct can*)priv;

    (void)can_accept(CAN, (can_id_t)CAN_ID_M2S, 0);

    for (;;) {
        int res;
        can_id_t id = 0;
        char msg[CAN_DATA_COUNT] = { (char)0, (char)0, (char)0, (char)0,
                                     (char)0, (char)0, (char)0, (char)0 };

        if ((res = can_read(CAN, &id, msg, sizeof(msg))) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        picoRTOS_assert_void(id == (can_id_t)CAN_ID_M2S);

        (void)can_write(CAN, (can_id_t)CAN_ID_S2M, msg, (size_t)res);
    }
}

/*
 * adc_main samples an ADC every 40ms and compares it to 3300mv,
 * ADC must be wired to the 3.3v line
 */
static void adc_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct adc *ADC = (struct adc*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {

        int sample_mv = 0;

        if (adc_read(ADC, &sample_mv) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        /* Analog measurement from 3.3v */
        picoRTOS_assert_void(sample_mv > 3100);
        picoRTOS_assert_void(sample_mv < 3500);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(40));
    }
}

/*
 * This thread tests the i2c master by sending a byte (0xa5) to 0x69 and expecting a
 * specific answer from a slave (0x5a)
 */
static void twi_master_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct twi *TWI = (struct twi*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        char c = (char)0xa5;
        int timeout = (int)PICORTOS_DELAY_MSEC(500);

        while (twi_write(TWI, &c, sizeof(c)) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);

        while (twi_read(TWI, &c, sizeof(c)) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);
        picoRTOS_assert_void(c == (char)0x5a);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

/*
 * This thread tests the i2c slave, it reads a byte (0xa5) and send 0x5a back
 */
static void twi_slave_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct twi *TWI = (struct twi*)priv;

    for (;;) {
        int res;
        int timeout = (int)PICORTOS_DELAY_SEC(2);

        if ((res = twi_poll(TWI)) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        if (res == TWI_WRITE) {
            char c = (char)0;
            while (twi_read(TWI, &c, sizeof(c)) == -EAGAIN && timeout-- != 0)
                picoRTOS_schedule();

            picoRTOS_assert_void(timeout != -1);
            picoRTOS_assert_void(c == (char)0xa5);
        }

        if (res == TWI_READ) {
            char c = (char)0x5a;
            while (twi_write(TWI, &c, sizeof(c)) == -EAGAIN && timeout-- != 0)
                picoRTOS_schedule();

            picoRTOS_assert_void(timeout != -1);
        }
    }
}

int main(void)
{
    static struct blink blink;

    /* Init the system */
    picoRTOS_init();
    blink_init(&blink);

    /* Each thread needs its own stack, but "task" is used as a way to communicate with
     * the kernel, so we only need 1 */
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
    static picoRTOS_stack_t stack10[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack11[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack12[CONFIG_DEFAULT_STACK_COUNT];

    /* Init fixed priorities first */
    picoRTOS_task_init(&task, tick_main, blink.due.DIGITAL23, stack0, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, (picoRTOS_priority_t)0);

    /* watchdog, last */
    picoRTOS_task_init(&task, wd_main, blink.due.WDT, stack1, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());

    /* blink threads */
    picoRTOS_task_init(&task, blink_main, &blink, stack2, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, blink_again_main, &blink, stack3, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* console thread */
    picoRTOS_task_init(&task, console_main, blink.due.SERIAL0, stack4, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* spi thread */
    picoRTOS_task_init(&task, spi_main, blink.due.SPI, stack5, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* pwm thread */
    picoRTOS_task_init(&task, pwm_main, blink.due.PWM2, stack6, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* ipwm thread */
    picoRTOS_task_init(&task, ipwm_main, blink.due.PWM3, stack7, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* can thread */
    picoRTOS_task_init(&task, can_master_main, blink.due.CAN0, stack8, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, can_slave_main, blink.due.CAN1, stack9, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* adc thread */
    picoRTOS_task_init(&task, adc_main, blink.due.A0, stack10, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* i2c threads */
    picoRTOS_task_init(&task, twi_master_main, blink.due.I2C, stack11, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, twi_slave_main, blink.due.I2C1, stack12, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* start the scheduler */
    picoRTOS_start();

    /* we're not supposed to end here */
    picoRTOS_assert_void(false);
    return -1;
}
