#include "picoRTOS.h"
#include "sipeed-longan-nano.h"

#include "picoRTOS_mutex.h"
#include "picoRTOS_cond.h"

static void tick_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct gpio *TICK = (struct gpio*)priv;

    for (;;) {
        gpio_toggle(TICK);
        picoRTOS_schedule();
    }
}

/*
 * Watchdog refreshing thread
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
 * These threads blink the onboard RGB LEDs
 */
static struct picoRTOS_mutex mutex = PICORTOS_MUTEX_INITIALIZER;
static struct picoRTOS_cond cond = PICORTOS_COND_INITIALIZER;

#define BLINK_PERIOD PICORTOS_DELAY_SEC(1)
#define BLINK_DELAY  PICORTOS_DELAY_MSEC(30)

static void ledr_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct gpio *LED_R = (struct gpio*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        picoRTOS_mutex_lock(&mutex);

        /* blink */
        gpio_write(LED_R, false);
        picoRTOS_sleep(BLINK_DELAY << 1);
        gpio_write(LED_R, true);
        picoRTOS_sleep(BLINK_DELAY);
        gpio_write(LED_R, false);
        picoRTOS_sleep(BLINK_DELAY);
        gpio_write(LED_R, true);

        /* ipc */
        picoRTOS_cond_signal(&cond);
        picoRTOS_mutex_unlock(&mutex);

        picoRTOS_sleep_until(&ref, BLINK_PERIOD);
    }
}

static void ledg_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct gpio *LED_G = (struct gpio*)priv;

    for (;;) {
        picoRTOS_mutex_lock(&mutex);
        picoRTOS_cond_wait(&cond, &mutex);

        /* blink */
        gpio_write(LED_G, false);
        picoRTOS_sleep(BLINK_DELAY << 1);
        gpio_write(LED_G, true);
        picoRTOS_sleep(BLINK_DELAY);
        gpio_write(LED_G, false);
        picoRTOS_sleep(BLINK_DELAY);
        gpio_write(LED_G, true);

        picoRTOS_mutex_unlock(&mutex);
    }
}

/*
 * This thread uses a PWM to fade in & out the selected LED
 */
static void pwm_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct pwm *PWM = (struct pwm*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    (void)pwm_set_period(PWM, (pwm_period_us_t)200);
    pwm_start(PWM);

    for (;;) {
        size_t n;

        /* fade in */
        for (n = 0; n < (size_t)100; n++) {
            (void)pwm_set_duty_cycle(PWM, PWM_DUTY_CYCLE_PCENT(n));
            picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(12));
        }

        /* fade out */
        for (n = (size_t)100; n > 0; n--) {
            (void)pwm_set_duty_cycle(PWM, PWM_DUTY_CYCLE_PCENT(n));
            picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(12));
        }
    }
}

/*
 * uart_main is a thread that sends data to the UART and expects to get it back.
 * No loopback is present on the nano, so you must setup a wire between T0 & R0
 */
static void uart_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

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
 * spi_main is a thread that sends data to the SPI and expects to get it back.
 * Internal loopback is present on the Arduino Due, but we do it with a wire to allow for
 * measurements. Pins A6 & A7 of the SPI connector should be wired together
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
 * Reads some ADC value & do nothing
 */
static void adc_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct adc *ADC = (struct adc*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {

        int value = 0;
        int timeout = CONFIG_DEADLOCK_COUNT;

        while (adc_read(ADC, &value) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

/*
 * flash_main is a thread that tests some flash features
 */
static void flash_main(void *priv)
{
#define BUF_COUNT 256 /* 1Kb block */

    picoRTOS_assert_fatal(priv != NULL, return );

    int res;
    size_t n;
    int nwritten = 0;
    static uint32_t buf[BUF_COUNT];

    int deadlock = CONFIG_DEADLOCK_COUNT;
    struct flash *FLASH = (struct flash*)priv;

    /* test on last block of flash */
    size_t block = (size_t)flash_get_nblocks(FLASH) - 1;
    size_t addr = (size_t)flash_get_block_addr(FLASH, block);
    const uint32_t *mem = (uint32_t*)addr;

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

    /* block */
    picoRTOS_assert_void_fatal(mem[0] == (uint32_t)0);
    picoRTOS_assert_void_fatal(mem[127] == (uint32_t)127);
    picoRTOS_assert_void_fatal(mem[128] == (uint32_t)128);
    picoRTOS_assert_void_fatal(mem[255] == (uint32_t)255);

    /* suicide */
    picoRTOS_kill();
}

/*
 * This thread uses the CAN loopback mode to send PINGPONG and check the data has been
 * received correctly
 */
static void can_main(void *priv)
{
#define CAN_TEST_ID 0x6

    picoRTOS_assert_fatal(priv != NULL, return );

    struct can *CAN = (struct can*)priv;

    (void)can_accept(CAN, (can_id_t)CAN_TEST_ID, 0);

    for (;;) {

        int res;
        can_id_t id = 0;
        int timeout = (int)PICORTOS_DELAY_MSEC(500);

        static const char tx[] = { "PINGPONG" };
        char rx[CAN_DATA_COUNT] = { (char)0, (char)0, (char)0, (char)0,
                                    (char)0, (char)0, (char)0, (char)0 };

        /* ping */
        if ((res = can_write(CAN, (can_id_t)CAN_TEST_ID, tx, (size_t)CAN_DATA_COUNT)) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        /* pong */
        while (((res = can_read(CAN, &id, rx, sizeof(rx)))) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(res == (int)sizeof(rx));
        picoRTOS_assert_void(timeout != -1);
        picoRTOS_assert_void(id == (can_id_t)CAN_TEST_ID);
        picoRTOS_assert_void(rx[7] == 'G');
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
    static struct sipeed_longan_nano nano;

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

    (void)sipeed_longan_nano_init(&nano);
    picoRTOS_init();

    /* TICK */
    picoRTOS_task_init(&task, tick_main, &nano.TICK, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* Watchdog */
    picoRTOS_task_init(&task, wd_main, &nano.WDT, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());
    /* LEDs */
    picoRTOS_task_init(&task, ledr_main, &nano.LED_R, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, ledg_main, &nano.LED_G, stack3, PICORTOS_STACK_COUNT(stack3));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* PWM LED */
    picoRTOS_task_init(&task, pwm_main, &nano.LED_B, stack4, PICORTOS_STACK_COUNT(stack4));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* console */
    picoRTOS_task_init(&task, uart_main, &nano.USART0, stack5, PICORTOS_STACK_COUNT(stack5));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* SPI */
    picoRTOS_task_init(&task, spi_main, &nano.SPI0, stack6, PICORTOS_STACK_COUNT(stack6));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* ADC */
    picoRTOS_task_init(&task, adc_main, &nano.TEMP, stack7, PICORTOS_STACK_COUNT(stack7));
    //picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* FLASH */
    picoRTOS_task_init(&task, flash_main, &nano.FLASH, stack8, PICORTOS_STACK_COUNT(stack8));
    // picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* CAN */
    picoRTOS_task_init(&task, can_main, &nano.CAN0, stack9, PICORTOS_STACK_COUNT(stack9));
    // picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* I2C */
    // picoRTOS_priority_t prio = picoRTOS_get_next_available_priority();
    picoRTOS_task_init(&task, twi_master_main, &nano.I2C0, stack10, PICORTOS_STACK_COUNT(stack10));
    // picoRTOS_add_task(&task, prio);
    picoRTOS_task_init(&task, twi_slave_main, &nano.I2C1, stack11, PICORTOS_STACK_COUNT(stack11));
    // picoRTOS_add_task(&task, prio);

    /* start scheduling */
    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
