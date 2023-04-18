#include "picoRTOS.h"
#include "arduino-mega2560.h"

#include "adc.h"
#include "eeprom.h"
#include "gpio.h"
#include "mux.h"
#include "pwm.h"
#include "spi.h"
#include "twi.h"
#include "uart.h"
#include "wd.h"

struct blink {
    struct arduino_mega2560 mega2560;
};

static void eeprom_update_info(struct blink *ctx)
{
    uint8_t nreset = 0;
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* eeprom */
    while (eeprom_read(&ctx->mega2560.C99_EEPROM, (size_t)0,
                       &nreset, sizeof(nreset)) == -EAGAIN &&
           deadlock-- != 0) {
    }

    picoRTOS_assert_void(deadlock != 1);

    /* increment */
    nreset++;

    /* store */
    while (eeprom_write(&ctx->mega2560.C99_EEPROM, (size_t)0,
                        &nreset, sizeof(nreset)) == -EAGAIN &&
           deadlock-- != 0) {
    }

    picoRTOS_assert_void(deadlock != 1);
}

static void blink_init(struct blink *ctx)
{
    /* board */
    (void)arduino_mega2560_init(&ctx->mega2560);

    /* watchdog */
    (void)wd_start(&ctx->mega2560.WDT);

    /* eeprom */
    eeprom_update_info(ctx);
}

/*
 * Threads
 */

/*
 * tick_main is a thread that toggles the TICK gpio so you can
 * make measurements on a logical anaylser or oscilloscope and check
 * the scheduling frequency is right
 */
static void tick_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct gpio *TICK = (struct gpio*)priv;

    for (;;) {
        /* toggle pin */
        gpio_toggle(TICK);
        picoRTOS_schedule();
    }
}

/*
 * blink_main is a thread that uses the PWM to fade in and out the builtin led
 */
static void blink_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct pwm *PWM = (struct pwm*)priv;

    int duty_cycle = 0;
    bool fade_in = true;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    (void)pwm_set_period(PWM, (pwm_period_us_t)50);
    pwm_start(PWM);

    for (;;) {

        if (fade_in) {
            duty_cycle += 2;
            if (duty_cycle == 100) fade_in = false;
        }else{
            duty_cycle -= 2;
            if (duty_cycle == 0) fade_in = true;
        }

        (void)pwm_set_duty_cycle(PWM, PWM_DUTY_CYCLE_PCENT(duty_cycle));
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(10)); /* 100Hz */
    }
}

/*
 * console_main is a thread that simply echoes characters
 */
static void console_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

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
 * No loopback is present on the arduino, so you must setup a wire between pins 50 & 51.
 * If loopback fails, the thread quits and set the spi status to false
 */
static void spi_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

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
 * adc_main samples ADC every 40ms and checks the result
 * ADC must be wired to the 3.3v line
 */
static void adc_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct adc *ADC = (struct adc*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {

        int sample_mv = 0;

        if (adc_read(ADC, &sample_mv) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        /* Analog measurement from 3.3v */
        picoRTOS_assert_void(sample_mv > 3200);
        picoRTOS_assert_void(sample_mv < 3400);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(40l));
    }
}

/*
 * This thread tests the i2c slave, it reads a byte (0xa5) and send 0x5a back
 */
static void twi_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct twi *TWI = (struct twi*)priv;

    for (;;) {
        char c = (char)0;
        int timeout = (int)PICORTOS_DELAY_SEC(2);

        /* no timeout on rx */
        while (twi_read(TWI, &c, sizeof(c)) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);
        picoRTOS_assert_void(c == (char)0xa5);

        c = (char)0x5a;
        while (twi_write(TWI, &c, sizeof(c)) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);
    }
}

/*
 * wd_main is a thread that refreshes the internal watchdog.
 * This thread should be the last one by order of priority. If it fails, the system resets
 */
static void wd_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct wd *WDT = (struct wd*)priv;

    for (;;) {
        wd_refresh(WDT);
        picoRTOS_schedule();
    }
}

int main( void )
{
    static struct blink blink;

    blink_init(&blink);
    picoRTOS_init();

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack4[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack5[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack6[CONFIG_DEFAULT_STACK_COUNT];

    /* tick */
    picoRTOS_task_init(&task, tick_main, &blink.mega2560.DIGITAL23,
                       stack0, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, (picoRTOS_priority_t)TASK_TICK_PRIO);
    /* led */
    picoRTOS_task_init(&task, blink_main, &blink.mega2560.PWM13,
                       stack1, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* serial console */
    picoRTOS_task_init(&task, console_main, &blink.mega2560.SERIAL0,
                       stack2, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* spi loopback */
    picoRTOS_task_init(&task, spi_main, &blink.mega2560.SPI,
                       stack3, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* adc */
    picoRTOS_task_init(&task, adc_main, &blink.mega2560.A0,
                       stack4, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* i2c */
    picoRTOS_task_init(&task, twi_main, &blink.mega2560.I2C,
                       stack5, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* watchdog refresh */
    picoRTOS_task_init(&task, wd_main, &blink.mega2560.WDT,
                       stack6, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, (picoRTOS_priority_t)TASK_WD_PRIO);

    /* Start the scheduler. */
    picoRTOS_start();

    /* we're not supposed to end here */
    picoRTOS_assert_void(false);
    return -1;
}
