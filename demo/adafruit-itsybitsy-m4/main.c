#include "picoRTOS.h"
#include "adafruit-itsybitsy-m4.h"

/*
 * led_main double-blinks the user LED
 */
static void led_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    picoRTOS_tick_t ref = picoRTOS_get_tick();
    struct gpio *RED = (struct gpio*)priv;

    for (;;) {

        gpio_write(RED, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(50u));
        gpio_write(RED, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(50u));
        gpio_write(RED, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(100u));
        gpio_write(RED, false);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

/*
 * spi_main is a thread that sends data to the SPI and expects to get it back.
 * No loopback is present on the adafruit, so you must setup a wire between pins PA0(MO) & PA1 (MI)
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
 * uart_main is a thread that sends data to the UART and expects to get it back.
 * No loopback is present on the adafruit, so you must setup a wire between pins PA16(RX) & PA17 (TX)
 */
static void uart_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    char tx = (char)0;
    struct uart *UART = (struct uart*)priv;

    for (;;) {

        char rx = (char)0;
        int timeout = CONFIG_DEADLOCK_COUNT;

        if (uart_write(UART, &tx, sizeof(tx)) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        while (uart_read(UART, &rx, sizeof(rx)) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);
        picoRTOS_assert_void(tx == rx);

        /* increment data */
        tx = (char)((int)tx + 1);
    }
}

/*
 * pwm_main put a ramp on a PWM
 */
static void pwm_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct pwm *PWM = (struct pwm*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    pwm_duty_cycle_t duty_cycle = 0;

    (void)pwm_set_period(PWM, (pwm_period_us_t)100);
    pwm_start(PWM);

    for (;;) {
        (void)pwm_set_duty_cycle(PWM, duty_cycle);
        duty_cycle += PWM_DUTY_CYCLE_PCENT(1);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(10)); /* 100Hz */
    }
}

/*
 * adc_main samples SCALEDCOREVCC every 40ms and stores the result in sample
 */
static void adc_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct adc *ADC = (struct adc*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {

        int sample_mV = 0;

        if (adc_read(ADC, &sample_mV) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        /* Analog measurement from 3.3v */
        picoRTOS_assert_void(sample_mV > 900);
        picoRTOS_assert_void(sample_mV < 1200);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(40l));
    }
}

/*
 * wd_main is a thread that refreshed the watchdog
 */
static void wd_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct wd *WDT = (struct wd*)priv;

    for (;;) {
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(32));
        wd_refresh(WDT);
    }
}

/*
 * flash_main is a thread that tests some flash features
 */
static void flash_main(void *priv)
{
#define BUF_COUNT   132 /* 1x512b page + qword */

    picoRTOS_assert_void(priv != NULL);

    int res;
    size_t n;
    int nwritten = 0;
    static uint32_t buf[BUF_COUNT];

    int deadlock = CONFIG_DEADLOCK_COUNT;
    struct flash *FLASH = (struct flash*)priv;

    /* test on last block of flash */
    size_t block = (size_t)flash_get_nblocks(FLASH) - 1;
    size_t addr = (size_t)flash_get_block_addr(FLASH, block);
    uint32_t *mem = (uint32_t*)addr;

    /* init buffer */
    for (n = 0; n < (size_t)BUF_COUNT; n++)
        buf[n] = (uint32_t)n;

    /* erase sector */
    while ((res = flash_erase(FLASH, block)) == -EAGAIN && deadlock-- != 0)
        picoRTOS_schedule();

    picoRTOS_assert_void_fatal(deadlock != -1);
    picoRTOS_assert_void_fatal(res == 0);

    /* blankcheck */
    if (flash_blankcheck(FLASH, block) < 0)
        picoRTOS_break();

    /* at last, write */
    n = sizeof(buf);

    while (n != 0) {
        uint8_t *buf8 = (uint8_t*)buf;
        deadlock = CONFIG_DEADLOCK_COUNT;

        while ((res = flash_write(FLASH, addr + (size_t)nwritten, &buf8[nwritten], n)) == -EAGAIN &&
               deadlock-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void_fatal(res > 0);
        picoRTOS_assert_void_fatal(deadlock != -1);

        nwritten += res;
        n -= (size_t)res;
    }

    /* page */
    picoRTOS_assert_void(mem[0] == (uint32_t)0);
    picoRTOS_assert_void(mem[127] == (uint32_t)127);
    /* qword */
    picoRTOS_assert_void(mem[128] == (uint32_t)128);
    picoRTOS_assert_void(mem[131] == (uint32_t)131);

    /* suicide */
    picoRTOS_kill();
}

int main(void)
{
    static struct adafruit_itsybitsy_m4 itsybitsy;

    (void)adafruit_itsybitsy_m4_init(&itsybitsy);
    picoRTOS_init();

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack4[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack5[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack6[CONFIG_DEFAULT_STACK_COUNT];

    /* TICK */
    picoRTOS_task_init(&task, led_main, &itsybitsy.RED, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* SPI */
    picoRTOS_task_init(&task, spi_main, &itsybitsy.SPI, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* UART */
    picoRTOS_task_init(&task, uart_main, &itsybitsy.UART, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* PWM */
    picoRTOS_task_init(&task, pwm_main, &itsybitsy.D5, stack3, PICORTOS_STACK_COUNT(stack3));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* ADC */
    picoRTOS_task_init(&task, adc_main, &itsybitsy.ADC, stack4, PICORTOS_STACK_COUNT(stack4));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* FLASH */
    picoRTOS_task_init(&task, flash_main, &itsybitsy.FLASH, stack5, PICORTOS_STACK_COUNT(stack5));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* WDT */
    picoRTOS_task_init(&task, wd_main, &itsybitsy.WDT, stack6, PICORTOS_STACK_COUNT(stack6));
    picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
