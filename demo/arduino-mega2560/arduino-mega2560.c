#include "arduino-mega2560.h"

static void arduino_mega2560_init_mux(void)
{
    struct mux PORTA;
    struct mux PORTB;
    struct mux PORTC;
    struct mux PORTD;
    struct mux PORTE;
    struct mux PORTF;
    struct mux PORTG;
    struct mux PORTH;
    struct mux PORTK;
    struct mux PORTL;

    (void)mux_avr_init(&PORTA, ADDR_PORTA);
    (void)mux_avr_init(&PORTB, ADDR_PORTB);
    (void)mux_avr_init(&PORTC, ADDR_PORTC);
    (void)mux_avr_init(&PORTD, ADDR_PORTD);
    (void)mux_avr_init(&PORTE, ADDR_PORTE);
    (void)mux_avr_init(&PORTF, ADDR_PORTF);
    (void)mux_avr_init(&PORTG, ADDR_PORTG);
    (void)mux_avr_init(&PORTH, ADDR_PORTH);
    (void)mux_avr_init(&PORTK, ADDR_PORTK);
    (void)mux_avr_init(&PORTL, ADDR_PORTL);

    /* ANALOG IN */
    (void)mux_avr_input(&PORTF, (size_t)0);         /* A0 */
    (void)mux_avr_input(&PORTF, (size_t)1);         /* A1 */
    (void)mux_avr_input(&PORTF, (size_t)2);         /* A2 */
    (void)mux_avr_input(&PORTF, (size_t)3);         /* A3 */
    /* (void)mux_avr_input(&PORTF, (size_t)4); */   /* A4 */
    /* (void)mux_avr_input(&PORTF, (size_t)5); */   /* A5 */
    /* (void)mux_avr_input(&PORTF, (size_t)6); */   /* A6 */
    /* (void)mux_avr_input(&PORTF, (size_t)7); */   /* A7 */
    (void)mux_avr_input(&PORTK, (size_t)0);         /* A8 */
    (void)mux_avr_input(&PORTK, (size_t)1);         /* A9 */
    (void)mux_avr_input(&PORTK, (size_t)2);         /* A10 */
    (void)mux_avr_input(&PORTK, (size_t)3);         /* A11 */
    (void)mux_avr_input(&PORTK, (size_t)4);         /* A12 */
    (void)mux_avr_input(&PORTK, (size_t)5);         /* A13 */
    (void)mux_avr_input(&PORTK, (size_t)6);         /* A14 */
    (void)mux_avr_input(&PORTK, (size_t)7);         /* A15 */

    /* DIGITAL */
    (void)mux_avr_input(&PORTA, (size_t)0);
    (void)mux_avr_output(&PORTA, (size_t)1, false);
    (void)mux_avr_input(&PORTA, (size_t)2);
    (void)mux_avr_output(&PORTA, (size_t)3, false);
    (void)mux_avr_input(&PORTA, (size_t)4);
    (void)mux_avr_output(&PORTA, (size_t)5, false);
    (void)mux_avr_input(&PORTA, (size_t)6);
    (void)mux_avr_output(&PORTA, (size_t)7, false);
    (void)mux_avr_input(&PORTC, (size_t)7);
    (void)mux_avr_output(&PORTC, (size_t)6, false);
    (void)mux_avr_input(&PORTC, (size_t)5);
    (void)mux_avr_output(&PORTC, (size_t)4, false);
    (void)mux_avr_input(&PORTC, (size_t)3);
    (void)mux_avr_output(&PORTC, (size_t)2, false);
    (void)mux_avr_input(&PORTC, (size_t)1);
    (void)mux_avr_output(&PORTC, (size_t)0, false);
    (void)mux_avr_input(&PORTD, (size_t)7);
    (void)mux_avr_output(&PORTG, (size_t)2, false);
    (void)mux_avr_input(&PORTG, (size_t)1);
    (void)mux_avr_output(&PORTG, (size_t)0, false);
    (void)mux_avr_input(&PORTL, (size_t)7);
    (void)mux_avr_output(&PORTL, (size_t)6, false);
    (void)mux_avr_input(&PORTL, (size_t)5);
    (void)mux_avr_output(&PORTL, (size_t)4, false);
    (void)mux_avr_input(&PORTL, (size_t)3);
    (void)mux_avr_output(&PORTL, (size_t)2, false);
    (void)mux_avr_input(&PORTL, (size_t)1);
    (void)mux_avr_output(&PORTL, (size_t)0, false);

    /* COMMUNICATION */
    (void)mux_avr_output(&PORTE, (size_t)1, true);  /* UART_TX */
    (void)mux_avr_input(&PORTE, (size_t)0);         /* UART_RX */
    (void)mux_avr_output(&PORTD, (size_t)0, true);  /* SDA */
    (void)mux_avr_output(&PORTD, (size_t)1, true);  /* SCL */

    /* PWM */
    (void)mux_avr_output(&PORTE, (size_t)4, false);
    (void)mux_avr_output(&PORTE, (size_t)5, false);
    (void)mux_avr_output(&PORTG, (size_t)5, false);
    (void)mux_avr_output(&PORTE, (size_t)3, false);
    (void)mux_avr_output(&PORTH, (size_t)3, false);
    (void)mux_avr_output(&PORTH, (size_t)4, false);
    (void)mux_avr_output(&PORTH, (size_t)5, false);
    (void)mux_avr_output(&PORTH, (size_t)6, false);
    (void)mux_avr_output(&PORTB, (size_t)4, false);
    (void)mux_avr_output(&PORTB, (size_t)5, false);
    (void)mux_avr_output(&PORTB, (size_t)6, false);
    (void)mux_avr_output(&PORTB, (size_t)7, false);

    /* ICSP */
    (void)mux_avr_input(&PORTB, (size_t)3);             /* MISO */
    (void)mux_avr_output(&PORTB, (size_t)2, false);     /* MOSI */
    (void)mux_avr_output(&PORTB, (size_t)1, false);     /* SCK */
    (void)mux_avr_output(&PORTB, (size_t)0, true);      /* _SS */
}

static void arduino_mega2560_init_analog_in(/*@partial@*/ struct arduino_mega2560 *ctx)
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
    /* (void)adc_avr_adc_init(&ctx->A4, &ADC, (size_t)4); */
    /* (void)adc_avr_adc_init(&ctx->A5, &ADC, (size_t)5); */
    /* (void)adc_avr_adc_init(&ctx->A6, &ADC, (size_t)6); */
    /* (void)adc_avr_adc_init(&ctx->A7, &ADC, (size_t)7); */
    (void)adc_avr_adc_init(&ctx->A8, &ADC, (size_t)8);
    (void)adc_avr_adc_init(&ctx->A9, &ADC, (size_t)9);
    (void)adc_avr_adc_init(&ctx->A10, &ADC, (size_t)10);
    (void)adc_avr_adc_init(&ctx->A11, &ADC, (size_t)11);
    (void)adc_avr_adc_init(&ctx->A12, &ADC, (size_t)12);
    (void)adc_avr_adc_init(&ctx->A13, &ADC, (size_t)13);
    (void)adc_avr_adc_init(&ctx->A14, &ADC, (size_t)14);
    (void)adc_avr_adc_init(&ctx->A15, &ADC, (size_t)15);
}

static void arduino_mega2560_init_digital(/*@partial@*/ struct arduino_mega2560 *ctx)
{
    (void)gpio_avr_init(&ctx->DIGITAL22, ADDR_PORTA, (size_t)0);
    (void)gpio_avr_init(&ctx->DIGITAL23, ADDR_PORTA, (size_t)1);
    (void)gpio_avr_init(&ctx->DIGITAL24, ADDR_PORTA, (size_t)2);
    (void)gpio_avr_init(&ctx->DIGITAL25, ADDR_PORTA, (size_t)3);
    (void)gpio_avr_init(&ctx->DIGITAL26, ADDR_PORTA, (size_t)4);
    (void)gpio_avr_init(&ctx->DIGITAL27, ADDR_PORTA, (size_t)5);
    (void)gpio_avr_init(&ctx->DIGITAL28, ADDR_PORTA, (size_t)6);
    (void)gpio_avr_init(&ctx->DIGITAL29, ADDR_PORTA, (size_t)7);
    (void)gpio_avr_init(&ctx->DIGITAL30, ADDR_PORTC, (size_t)7);
    (void)gpio_avr_init(&ctx->DIGITAL31, ADDR_PORTC, (size_t)6);
    (void)gpio_avr_init(&ctx->DIGITAL32, ADDR_PORTC, (size_t)5);
    (void)gpio_avr_init(&ctx->DIGITAL33, ADDR_PORTC, (size_t)4);
    (void)gpio_avr_init(&ctx->DIGITAL34, ADDR_PORTC, (size_t)3);
    (void)gpio_avr_init(&ctx->DIGITAL35, ADDR_PORTC, (size_t)2);
    (void)gpio_avr_init(&ctx->DIGITAL36, ADDR_PORTC, (size_t)1);
    (void)gpio_avr_init(&ctx->DIGITAL37, ADDR_PORTC, (size_t)0);
    (void)gpio_avr_init(&ctx->DIGITAL38, ADDR_PORTD, (size_t)7);
    (void)gpio_avr_init(&ctx->DIGITAL39, ADDR_PORTG, (size_t)2);
    (void)gpio_avr_init(&ctx->DIGITAL40, ADDR_PORTG, (size_t)1);
    (void)gpio_avr_init(&ctx->DIGITAL41, ADDR_PORTG, (size_t)0);
    (void)gpio_avr_init(&ctx->DIGITAL42, ADDR_PORTL, (size_t)7);
    (void)gpio_avr_init(&ctx->DIGITAL43, ADDR_PORTL, (size_t)6);
    (void)gpio_avr_init(&ctx->DIGITAL44, ADDR_PORTL, (size_t)5);
    (void)gpio_avr_init(&ctx->DIGITAL45, ADDR_PORTL, (size_t)4);
    (void)gpio_avr_init(&ctx->DIGITAL46, ADDR_PORTL, (size_t)3);
    (void)gpio_avr_init(&ctx->DIGITAL47, ADDR_PORTL, (size_t)2);
    (void)gpio_avr_init(&ctx->DIGITAL48, ADDR_PORTL, (size_t)1);
    (void)gpio_avr_init(&ctx->DIGITAL49, ADDR_PORTL, (size_t)0);
}

static void arduino_mega2560_init_communication(/*@partial@*/ struct arduino_mega2560 *ctx)
{
    /* SERIAL */
    struct uart_settings UART_settings = {
        9600ul,
        (size_t)8,
        UART_PAR_NONE,
        UART_CSTOPB_1BIT
    };

    (void)uart_avr_init(&ctx->SERIAL0, ADDR_USART0, CLOCK_ATMEGA2560_CLKIO);
    (void)uart_setup(&ctx->SERIAL0, &UART_settings);

    /* I2C */
    struct twi_settings TWI_settings = {
        TWI_BITRATE_STANDARD,
        TWI_MODE_SLAVE,
        (twi_addr_t)0x69
    };

    (void)twi_avr_init(&ctx->I2C, ADDR_TWI, CLOCK_ATMEGA2560_CLKIO);
    (void)twi_setup(&ctx->I2C, &TWI_settings);
}

static void arduino_mega2560_init_pwm(/*@partial@*/ struct arduino_mega2560 *ctx)
{
    static struct pwm_avr TC1;
    static struct pwm_avr TC2;
    static struct pwm_avr TC3;
    static struct pwm_avr TC4;
    static struct pwm_avr TC5;

    /* Fixed period (see manual) */
    struct pwm_avr_settings TCn_settings = {
        PWM_AVR_CS_CLKIO_1,
        PWM_AVR_WAVEFORM_FAST_PWM_10BIT
    };

    /* Variable period set by ICRn */
    struct pwm_avr_settings TCn_settings_ICRn = {
        PWM_AVR_CS_CLKIO_1,
        PWM_AVR_WAVEFORM_FAST_PWM_ICRn
    };

    /* parent */
    (void)pwm_avr_init(&TC1, ADDR_TC1, CLOCK_ATMEGA2560_CLKIO);
    (void)pwm_avr_setup(&TC1, &TCn_settings_ICRn);
    (void)pwm_avr_init(&TC2, ADDR_TC2, CLOCK_ATMEGA2560_CLKIO);
    (void)pwm_avr_setup(&TC2, &TCn_settings);
    (void)pwm_avr_init(&TC3, ADDR_TC3, CLOCK_ATMEGA2560_CLKIO);
    (void)pwm_avr_setup(&TC3, &TCn_settings_ICRn);
    (void)pwm_avr_init(&TC4, ADDR_TC4, CLOCK_ATMEGA2560_CLKIO);
    (void)pwm_avr_setup(&TC4, &TCn_settings);
    (void)pwm_avr_init(&TC5, ADDR_TC5, CLOCK_ATMEGA2560_CLKIO);
    (void)pwm_avr_setup(&TC5, &TCn_settings);

    /* pwm */
    struct pwm_settings PWM_settings = {
        PWM_AVR_MODE_CLEAR
    };

    (void)pwm_avr_pwm_init(&ctx->PWM2, &TC3, PWM_AVR_OCnB);
    (void)pwm_avr_pwm_setup(&ctx->PWM2, &PWM_settings);
    (void)pwm_avr_pwm_init(&ctx->PWM3, &TC3, PWM_AVR_OCnC);
    (void)pwm_avr_pwm_setup(&ctx->PWM3, &PWM_settings);
    /* (void)pwm_avr_pwm_init(&ctx->PWM4, &TC0, PWM_AVR_OCnC); */
    /* (void)pwm_avr_pwm_setup(&ctx->PWM4, &PWM_settings); */
    (void)pwm_avr_pwm_init(&ctx->PWM5, &TC3, PWM_AVR_OCnA);
    (void)pwm_avr_pwm_setup(&ctx->PWM5, &PWM_settings);
    (void)pwm_avr_pwm_init(&ctx->PWM6, &TC4, PWM_AVR_OCnA);
    (void)pwm_avr_pwm_setup(&ctx->PWM6, &PWM_settings);
    (void)pwm_avr_pwm_init(&ctx->PWM7, &TC4, PWM_AVR_OCnB);
    (void)pwm_avr_pwm_setup(&ctx->PWM7, &PWM_settings);
    (void)pwm_avr_pwm_init(&ctx->PWM8, &TC4, PWM_AVR_OCnC);
    (void)pwm_avr_pwm_setup(&ctx->PWM8, &PWM_settings);
    (void)pwm_avr_pwm_init(&ctx->PWM9, &TC2, PWM_AVR_OCnB);
    (void)pwm_avr_pwm_setup(&ctx->PWM9, &PWM_settings);
    (void)pwm_avr_pwm_init(&ctx->PWM10, &TC2, PWM_AVR_OCnA);
    (void)pwm_avr_pwm_setup(&ctx->PWM10, &PWM_settings);
    (void)pwm_avr_pwm_init(&ctx->PWM11, &TC1, PWM_AVR_OCnA);
    (void)pwm_avr_pwm_setup(&ctx->PWM11, &PWM_settings);
    (void)pwm_avr_pwm_init(&ctx->PWM12, &TC1, PWM_AVR_OCnB);
    (void)pwm_avr_pwm_setup(&ctx->PWM12, &PWM_settings);
    (void)pwm_avr_pwm_init(&ctx->PWM13, &TC1, PWM_AVR_OCnC);
    (void)pwm_avr_pwm_setup(&ctx->PWM13, &PWM_settings);
}

static void arduino_mega2560_init_icsp(/*@partial@*/ struct arduino_mega2560 *ctx)
{
    struct spi_settings SPI_settings = {
        10000000ul, /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)8,  /* frame size */
        SPI_CS_POL_IGNORE,
        (size_t)0   /* CS */
    };

    (void)spi_avr_init(&ctx->SPI, ADDR_SPI, CLOCK_ATMEGA2560_CLKIO);
    (void)spi_setup(&ctx->SPI, &SPI_settings);
}

static void arduino_mega2560_init_eeprom(/*@partial@*/ struct arduino_mega2560 *ctx)
{
    (void)eeprom_avr_init(&ctx->C99_EEPROM, ADDR_EEPROM, ADDR_SPMCSR);
}

static void arduino_mega2560_init_wdt(/*@partial@*/ struct arduino_mega2560 *ctx)
{
    struct wd_avr_settings WD_settings = {
        WD_AVR_CYCLES_4K
    };

    (void)wd_avr_init(&ctx->WDT, ADDR_WDT);
    (void)wd_avr_setup(&ctx->WDT, &WD_settings);
}

int arduino_mega2560_init(struct arduino_mega2560 *ctx)
{
    arduino_mega2560_init_mux();
    arduino_mega2560_init_analog_in(ctx);
    arduino_mega2560_init_digital(ctx);
    arduino_mega2560_init_communication(ctx);
    arduino_mega2560_init_pwm(ctx);
    arduino_mega2560_init_icsp(ctx);
    arduino_mega2560_init_eeprom(ctx);
    arduino_mega2560_init_wdt(ctx);

    return 0;
}
