#include "arduino-uno.h"

static void arduino_uno_init_mux(void)
{
    struct mux PORTB;
    struct mux PORTC;
    struct mux PORTD;

    (void)mux_avr_init(&PORTB, ADDR_PORTB);
    (void)mux_avr_init(&PORTC, ADDR_PORTC);
    (void)mux_avr_init(&PORTD, ADDR_PORTD);

    /* DIGITAL: UART */
    (void)mux_avr_output(&PORTD, (size_t)1, true);  /* UART_TX */
    (void)mux_avr_input(&PORTD, (size_t)0);         /* UART_RX */

    /* DIGITAL: GPIO */
    (void)mux_avr_input(&PORTD, (size_t)2);
    (void)mux_avr_output(&PORTD, (size_t)4, false);
    (void)mux_avr_input(&PORTD, (size_t)6);
    (void)mux_avr_output(&PORTD, (size_t)7, false);
    (void)mux_avr_input(&PORTB, (size_t)0);

    /* DIGITAL: PWM */
    (void)mux_avr_output(&PORTD, (size_t)3, false);
    (void)mux_avr_output(&PORTB, (size_t)1, false);

    /* DIGITAL: SPI */
    (void)mux_avr_input(&PORTB, (size_t)4);             /* MISO */
    (void)mux_avr_output(&PORTB, (size_t)2, false);     /* MOSI */
    (void)mux_avr_output(&PORTB, (size_t)3, false);     /* SCK */
    (void)mux_avr_output(&PORTB, (size_t)2, true);      /* _SS */

    /* MISC: I2C */
    (void)mux_avr_output(&PORTC, (size_t)4, true);  /* SDA */
    (void)mux_avr_output(&PORTC, (size_t)5, true);  /* SCL */
}

static void arduino_uno_init_analog_in(/*@partial@*/ struct arduino_uno *ctx)
{
    static struct adc_avr ADC;
    struct adc_avr_settings ADC_settings = {
        ADC_AVR_PRESCALER_2,
        ADC_AVR_REFS_AVCC,
        ADC_AVR_TRIG_MANUAL,
        false /* !left_adjust_result */
    };

    (void)adc_avr_init(&ADC, ADDR_ADC);
    (void)adc_avr_setup(&ADC, &ADC_settings);

    /* channels */
    struct adc_settings ADC_settings_mV = {
        13, /* mult */
        3,  /* div */
        0   /* offset */
    };

    (void)adc_avr_adc_init(&ctx->A0, &ADC, (size_t)0);
    (void)adc_setup(&ctx->A0, &ADC_settings_mV);
    (void)adc_avr_adc_init(&ctx->A1, &ADC, (size_t)1);
    (void)adc_setup(&ctx->A1, &ADC_settings_mV);
    (void)adc_avr_adc_init(&ctx->A2, &ADC, (size_t)2);
    (void)adc_setup(&ctx->A2, &ADC_settings_mV);
    (void)adc_avr_adc_init(&ctx->A3, &ADC, (size_t)3);
    (void)adc_setup(&ctx->A3, &ADC_settings_mV);
}

static void arduino_uno_init_uart(/*@partial@*/ struct arduino_uno *ctx)
{
    /* SERIAL */
    struct uart_settings UART_settings = {
        9600ul,
        (size_t)8,
        UART_PAR_NONE,
        UART_CSTOPB_1BIT
    };

    (void)uart_avr_init(&ctx->SERIAL, ADDR_USART, CLOCK_ATMEGA328P_CLKIO);
    (void)uart_setup(&ctx->SERIAL, &UART_settings);
}

static void arduino_uno_init_digital(/*@partial@*/ struct arduino_uno *ctx)
{
    (void)gpio_avr_init(&ctx->DIGITAL2, ADDR_PORTD, (size_t)2);
    (void)gpio_avr_init(&ctx->DIGITAL4, ADDR_PORTD, (size_t)4);
    (void)gpio_avr_init(&ctx->DIGITAL6, ADDR_PORTD, (size_t)6);
    (void)gpio_avr_init(&ctx->DIGITAL7, ADDR_PORTD, (size_t)7);
    (void)gpio_avr_init(&ctx->DIGITAL8, ADDR_PORTB, (size_t)0);
}

static void arduino_uno_init_spi(/*@partial@*/ struct arduino_uno *ctx)
{
    struct spi_settings SPI_settings = {
        10000000ul, /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)8,  /* frame size */
        SPI_CS_POL_IGNORE,
        (size_t)0   /* CS */
    };

    (void)spi_avr_init(&ctx->SPI, ADDR_SPI, CLOCK_ATMEGA328P_CLKIO);
    (void)spi_setup(&ctx->SPI, &SPI_settings);
}

static void arduino_uno_init_pwm(/*@partial@*/ struct arduino_uno *ctx)
{
    static struct pwm_avr TC1;
    static struct pwm_avr TC2;

    /* Variable period set by ICRn */
    struct pwm_avr_settings TCn_settings_ICRn = {
        PWM_AVR_CS_CLKIO_1,
        PWM_AVR_WAVEFORM_FAST_PWM_ICRn
    };

    (void)pwm_avr_init(&TC1, ADDR_TC1, CLOCK_ATMEGA328P_CLKIO);
    (void)pwm_avr_setup(&TC1, &TCn_settings_ICRn);
    (void)pwm_avr_init(&TC2, ADDR_TC2, CLOCK_ATMEGA328P_CLKIO);
    (void)pwm_avr_setup(&TC2, &TCn_settings_ICRn);

    /* pwm */
    struct pwm_settings PWM_settings = {
        PWM_AVR_MODE_CLEAR
    };

    (void)pwm_avr_pwm_init(&ctx->PWM3, &TC2, PWM_AVR_OCnB);
    (void)pwm_avr_pwm_setup(&ctx->PWM3, &PWM_settings);
    (void)pwm_avr_pwm_init(&ctx->PWM9, &TC1, PWM_AVR_OCnA);
    (void)pwm_avr_pwm_setup(&ctx->PWM9, &PWM_settings);
}

static void arduino_uno_init_twi(/*@partial@*/ struct arduino_uno *ctx)
{
    /* I2C */
    struct twi_settings TWI_settings = {
        TWI_BITRATE_STANDARD,
        TWI_MODE_SLAVE,
        (twi_addr_t)0x69
    };

    (void)twi_avr_init(&ctx->I2C, ADDR_TWI, CLOCK_ATMEGA328P_CLKIO);
    (void)twi_setup(&ctx->I2C, &TWI_settings);
}

static void arduino_uno_init_eeprom(/*@partial@*/ struct arduino_uno *ctx)
{
    (void)eeprom_avr_init(&ctx->C99_EEPROM, ADDR_EEPROM, ADDR_SPMCSR);
}

static void arduino_uno_init_wdt(/*@partial@*/ struct arduino_uno *ctx)
{
    struct wd_avr_settings WD_settings = {
        WD_AVR_CYCLES_2K
    };

    (void)wd_avr_init(&ctx->WDT, ADDR_WDT);
    (void)wd_avr_setup(&ctx->WDT, &WD_settings);
}

int arduino_uno_init(struct arduino_uno *ctx)
{
    arduino_uno_init_mux();

    arduino_uno_init_analog_in(ctx);
    arduino_uno_init_uart(ctx);
    arduino_uno_init_digital(ctx);
    arduino_uno_init_spi(ctx);
    arduino_uno_init_pwm(ctx);
    arduino_uno_init_twi(ctx);
    arduino_uno_init_eeprom(ctx);
    arduino_uno_init_wdt(ctx);

    return 0;
}
