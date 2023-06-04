#include "sipeed-longan-nano.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "clock-gd32vf103.h"
#include "mux-gd32vf103.h"

static void clock_init(void)
{
    struct clock_settings CLOCK_settings = {
        0ul, /* disable hxtal */
        CLOCK_GD32VF103_CK_SYS_PLL,
        CLOCK_GD32VF103_PLLSEL_IRC8M,
        108000000ul,    /* pll */
        1ul,            /* ahb_div */
        2ul,            /* apb1_div (54Mhz max) */
        1ul,            /* apb2_div */
        8ul,            /* adc_div */
    };

    (void)clock_gd32vf103_init(&CLOCK_settings);
}

static void mux_init(void)
{
    struct mux MUXA;
    struct mux MUXB;
    struct mux MUXC;

    /* enable clocks */
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_PA);
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_PB);
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_PC);
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_AF);

    (void)mux_gd32vf103_init(&MUXA, ADDR_GPIOA);
    (void)mux_gd32vf103_init(&MUXB, ADDR_GPIOB);
    (void)mux_gd32vf103_init(&MUXC, ADDR_GPIOC);

    (void)mux_gd32vf103_output(&MUXC, (size_t)15,    /* TICK */
                               MUX_GD32VF103_OUTPUT_PUSH_PULL,
                               MUX_GD32VF103_SPEED_2MHZ);
    /* LEDs */
    (void)mux_gd32vf103_output(&MUXA, (size_t)1,     /* LED_G */
                               MUX_GD32VF103_OUTPUT_PUSH_PULL,
                               MUX_GD32VF103_SPEED_2MHZ);
    (void)mux_gd32vf103_output(&MUXC, (size_t)13,    /* LED_R */
                               MUX_GD32VF103_OUTPUT_PUSH_PULL,
                               MUX_GD32VF103_SPEED_2MHZ);
    /* LEDs PWM */
    (void)mux_gd32vf103_afio(&MUXA, (size_t)2,     /* LED_B */
                             MUX_GD32VF103_AFIO_PUSH_PULL,
                             MUX_GD32VF103_SPEED_2MHZ);
    /* USART0 */
    (void)mux_gd32vf103_afio(&MUXA, (size_t)9,       /* TX0 */
                             MUX_GD32VF103_AFIO_PUSH_PULL,
                             MUX_GD32VF103_SPEED_2MHZ);
    (void)mux_gd32vf103_input(&MUXA, (size_t)10,    /* RX0 */
                              MUX_GD32VF103_INPUT_FLOATING);
    /* SPI0 */
    (void)mux_gd32vf103_afio(&MUXA, (size_t)4,         /* NSS0 */
                             MUX_GD32VF103_AFIO_PUSH_PULL,
                             MUX_GD32VF103_SPEED_50MHZ);
    (void)mux_gd32vf103_afio(&MUXA, (size_t)5,        /* SCK0 */
                             MUX_GD32VF103_AFIO_PUSH_PULL,
                             MUX_GD32VF103_SPEED_50MHZ);
    (void)mux_gd32vf103_input(&MUXA, (size_t)6,     /* MISO0 */
                              MUX_GD32VF103_INPUT_FLOATING);
    (void)mux_gd32vf103_afio(&MUXA, (size_t)7,      /* MOSI0 */
                             MUX_GD32VF103_AFIO_PUSH_PULL,
                             MUX_GD32VF103_SPEED_50MHZ);
    /* I2C0 */
    (void)mux_gd32vf103_afio(&MUXB, (size_t)6,      /* SCL0 */
                             MUX_GD32VF103_AFIO_PUSH_PULL,
                             MUX_GD32VF103_SPEED_50MHZ);
    (void)mux_gd32vf103_afio(&MUXB, (size_t)7,      /* SDA0 */
                             MUX_GD32VF103_AFIO_PUSH_PULL,
                             MUX_GD32VF103_SPEED_50MHZ);
    /* I2C1 */
    (void)mux_gd32vf103_afio(&MUXB, (size_t)10,      /* SCL1 */
                             MUX_GD32VF103_AFIO_PUSH_PULL,
                             MUX_GD32VF103_SPEED_50MHZ);
    (void)mux_gd32vf103_afio(&MUXB, (size_t)11,      /* SDA1 */
                             MUX_GD32VF103_AFIO_PUSH_PULL,
                             MUX_GD32VF103_SPEED_50MHZ);
}

static int gpio_init(/*@partial@*/ struct sipeed_longan_nano *ctx)
{
    (void)gpio_gd32vf103_init(&ctx->TICK, ADDR_GPIOC, (size_t)15);
    (void)gpio_gd32vf103_init(&ctx->LED_R, ADDR_GPIOC, (size_t)13);
    (void)gpio_gd32vf103_init(&ctx->LED_G, ADDR_GPIOA, (size_t)1);

    return 0;
}

static int wd_init(/*@partial@*/ struct sipeed_longan_nano *ctx)
{
    struct wd_gd32vf103_fwdgt_settings WD_settings = {
        4000ul, /* timeout_us */
    };

    (void)wd_gd32vf103_fwdgt_init(&ctx->WDT, ADDR_FWDGT, CLOCK_GD32VF103_CK_FWDGT);
    (void)wd_gd32vf103_fwdgt_setup(&ctx->WDT, &WD_settings);

    return wd_start(&ctx->WDT);
}

static int uart_init(/*@partial@*/ struct sipeed_longan_nano *ctx)
{
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_USART0);

    struct uart_settings UART_settings = {
        9600ul,     /* baudrate */
        (size_t)8,  /* character size */
        false,      /* parenb */
        false,      /* parodd */
        false       /* cstopb */
    };

    (void)uart_gd32vf103_init(&ctx->USART0, ADDR_USART0, CLOCK_GD32VF103_APB2);
    (void)uart_setup(&ctx->USART0, &UART_settings);

    return 0;
}

static int spi_init(/*@partial@*/ struct sipeed_longan_nano *ctx)
{
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_SPI0);

    struct spi_settings SPI_settings = {
        843750ul, /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)8,  /* frame size */
        SPI_CS_POL_ACTIVE_LOW,
        (size_t)0   /* cs */
    };

    (void)spi_gd32vf103_init(&ctx->SPI0, ADDR_SPI0, CLOCK_GD32VF103_CK_SYS);
    (void)spi_setup(&ctx->SPI0, &SPI_settings);

    return 0;
}

static int pwm_init(/*@partial@*/ struct sipeed_longan_nano *ctx)
{
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_TIMER1);

    static struct pwm_gd32vf103 PWM;
    struct pwm_gd32vf103_settings PWM_settings = {
        1000000ul, /* frequency */
        PWM_GD32VF103_WAVEFORM_EAPWM_UP,
    };

    (void)pwm_gd32vf103_init(&PWM, ADDR_TIMER1, CLOCK_GD32VF103_APB1);
    (void)pwm_gd32vf103_setup(&PWM, &PWM_settings);

    struct pwm_gd32vf103_pwm_settings LED_B_settings = {
        PWM_GD32VF103_PWM_MODE_1,               /* mode */
    };

    (void)pwm_gd32vf103_pwm_init(&ctx->LED_B, &PWM, (size_t)2);
    (void)pwm_gd32vf103_pwm_setup(&ctx->LED_B, &LED_B_settings);
    return 0;
}

static int flash_init(/*@partial@*/ struct sipeed_longan_nano *ctx)
{
    (void)flash_gd32vf103_init(&ctx->FLASH, ADDR_FMC, (size_t)128);
    return 0;
}

static int adc_init(/*@partial@*/ struct sipeed_longan_nano *ctx)
{
    static struct adc_gd32vf103 ADC0;
    struct adc_gd32vf103_settings ADC0_settings = {
        ADC_GD32VF103_MODE_SINGLE,  /* mode */
        NULL,                       /* dma */
        true,                       /* tvsen */
    };

    (void)adc_gd32vf103_init(&ADC0, ADDR_ADC0);
    (void)adc_gd32vf103_setup(&ADC0, &ADC0_settings);

    (void)adc_gd32vf103_adc_init(&ctx->TEMP, &ADC0, (size_t)16);
    return 0;
}

static int can_init(/*@partial@*/ struct sipeed_longan_nano *ctx)
{
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_CAN0);
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_CAN1);

    struct can_settings CAN0_settings = {
        500000ul,   /* bitrate */
        (size_t)3,  /* tx_mailbox_count (hard-wired anyway) */
        true,       /* tx_auto_abort */
        true,       /* rx_overwrite */
        true,       /* loopback */
    };

    (void)can_gd32vf103_init(&ctx->CAN0, ADDR_CAN1, CLOCK_GD32VF103_APB1);
    (void)can_setup(&ctx->CAN0, &CAN0_settings);

    return 0;
}

static int twi_init(/*@partial@*/ struct sipeed_longan_nano *ctx)
{
    struct twi_settings TWI_settings = {
        TWI_BITRATE_STANDARD,   /* bitrate */
        TWI_MODE_MASTER,        /* mode */
        (twi_addr_t)0x55        /* slave address */
    };

    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_I2C0);
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_I2C1);

    (void)twi_gd32vf103_init(&ctx->I2C0, ADDR_I2C0, CLOCK_GD32VF103_APB1);
    (void)twi_setup(&ctx->I2C0, &TWI_settings);

    TWI_settings.mode = TWI_MODE_SLAVE;
    (void)twi_gd32vf103_init(&ctx->I2C1, ADDR_I2C1, CLOCK_GD32VF103_APB1);
    (void)twi_setup(&ctx->I2C1, &TWI_settings);

    return 0;
}

int sipeed_longan_nano_init(struct sipeed_longan_nano *ctx)
{
    clock_init();
    mux_init();

    (void)gpio_init(ctx);
    (void)wd_init(ctx);
    (void)uart_init(ctx);
    (void)spi_init(ctx);
    (void)pwm_init(ctx);
    (void)flash_init(ctx);
    (void)adc_init(ctx);
    (void)can_init(ctx);
    (void)twi_init(ctx);

    return 0;
}
