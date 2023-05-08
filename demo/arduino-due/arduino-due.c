#include "arduino-due.h"
#include "picoRTOS.h"

#include "adc-sam3x.h"
#include "can-sam3x.h"
#include "clock-sam3x_pmc.h"
#include "gpio-sam3x_pio.h"
#include "mux-sam3x_pio.h"
#include "pwm-sam3x_tc.h"
#include "spi-sam3x.h"
#include "twi-sam3x.h"
#include "uart-sam3x.h"
#include "wd-sam3x.h"

/* TODO: unvoid all of this */

static void arduino_due_init_clock(void)
{
    struct clock_settings CLOCK_settings = {
        12000000ul,     /* moscx, 12Mhz */
        168000000ul,    /* plla */
        2ul,            /* mclk_div */
        1ul,            /* usb_div */
    };

    (void)clock_sam3x_pmc_init(&CLOCK_settings);
}

static void arduino_due_init_mux(void)
{
    struct mux_sam3x_pio MUXA;
    struct mux_sam3x_pio MUXB;
    struct mux_sam3x_pio MUXC;
    struct mux_sam3x_pio MUXD;

    (void)mux_sam3x_pio_init(&MUXA, ADDR_PIOA);
    (void)mux_sam3x_pio_init(&MUXB, ADDR_PIOB);
    (void)mux_sam3x_pio_init(&MUXC, ADDR_PIOC);
    (void)mux_sam3x_pio_init(&MUXD, ADDR_PIOD);

    /* ANALOG IN */
    /* Nothing to do in theory */

    /* DAC */
    /* ? */

    /* CAN */
    (void)mux_sam3x_pio_output(&MUXA, (size_t)0, MUX_SAM3X_PIO_A);  /* CANTX */
    (void)mux_sam3x_pio_input(&MUXA, (size_t)1, MUX_SAM3X_PIO_A);   /* CANRX */

    /* DIGITAL */
    (void)mux_sam3x_pio_output(&MUXB, (size_t)26, MUX_SAM3X_PIO_GPIO);  /* PIN22 */
    (void)mux_sam3x_pio_input(&MUXA, (size_t)14, MUX_SAM3X_PIO_GPIO);   /* PIN23 */
    (void)mux_sam3x_pio_output(&MUXA, (size_t)15, MUX_SAM3X_PIO_GPIO);  /* PIN24 */
    (void)mux_sam3x_pio_input(&MUXD, (size_t)0, MUX_SAM3X_PIO_GPIO);    /* PIN25 */
    (void)mux_sam3x_pio_output(&MUXD, (size_t)1, MUX_SAM3X_PIO_GPIO);   /* PIN26 */
    (void)mux_sam3x_pio_input(&MUXD, (size_t)2, MUX_SAM3X_PIO_GPIO);    /* PIN27 */
    (void)mux_sam3x_pio_output(&MUXD, (size_t)3, MUX_SAM3X_PIO_GPIO);   /* PIN28 */
    (void)mux_sam3x_pio_input(&MUXD, (size_t)6, MUX_SAM3X_PIO_GPIO);    /* PIN29 */
    (void)mux_sam3x_pio_output(&MUXD, (size_t)9, MUX_SAM3X_PIO_GPIO);   /* PIN30 */
    (void)mux_sam3x_pio_input(&MUXA, (size_t)7, MUX_SAM3X_PIO_GPIO);    /* PIN31 */
    (void)mux_sam3x_pio_output(&MUXD, (size_t)10, MUX_SAM3X_PIO_GPIO);  /* PIN32 */
    (void)mux_sam3x_pio_input(&MUXC, (size_t)1, MUX_SAM3X_PIO_GPIO);    /* PIN33 */
    (void)mux_sam3x_pio_output(&MUXC, (size_t)2, MUX_SAM3X_PIO_GPIO);   /* PIN34 */
    (void)mux_sam3x_pio_input(&MUXC, (size_t)3, MUX_SAM3X_PIO_GPIO);    /* PIN35 */
    (void)mux_sam3x_pio_output(&MUXC, (size_t)4, MUX_SAM3X_PIO_GPIO);   /* PIN36 */
    (void)mux_sam3x_pio_input(&MUXC, (size_t)5, MUX_SAM3X_PIO_GPIO);    /* PIN37 */
    (void)mux_sam3x_pio_output(&MUXC, (size_t)6, MUX_SAM3X_PIO_GPIO);   /* PIN38 */
    (void)mux_sam3x_pio_input(&MUXC, (size_t)7, MUX_SAM3X_PIO_GPIO);    /* PIN39 */
    (void)mux_sam3x_pio_output(&MUXC, (size_t)8, MUX_SAM3X_PIO_GPIO);   /* PIN40 */
    (void)mux_sam3x_pio_input(&MUXC, (size_t)9, MUX_SAM3X_PIO_GPIO);    /* PIN41 */
    (void)mux_sam3x_pio_output(&MUXA, (size_t)19, MUX_SAM3X_PIO_GPIO);  /* PIN42 */
    (void)mux_sam3x_pio_input(&MUXA, (size_t)20, MUX_SAM3X_PIO_GPIO);   /* PIN43 */
    (void)mux_sam3x_pio_output(&MUXC, (size_t)19, MUX_SAM3X_PIO_GPIO);  /* PIN44 */
    (void)mux_sam3x_pio_input(&MUXC, (size_t)18, MUX_SAM3X_PIO_GPIO);   /* PIN45 */
    (void)mux_sam3x_pio_output(&MUXC, (size_t)17, MUX_SAM3X_PIO_GPIO);  /* PIN46 */
    (void)mux_sam3x_pio_input(&MUXC, (size_t)16, MUX_SAM3X_PIO_GPIO);   /* PIN47 */
    (void)mux_sam3x_pio_output(&MUXC, (size_t)15, MUX_SAM3X_PIO_GPIO);  /* PIN48 */
    (void)mux_sam3x_pio_input(&MUXC, (size_t)14, MUX_SAM3X_PIO_GPIO);   /* PIN49 */
    (void)mux_sam3x_pio_output(&MUXC, (size_t)13, MUX_SAM3X_PIO_GPIO);  /* PIN50 */
    (void)mux_sam3x_pio_input(&MUXC, (size_t)12, MUX_SAM3X_PIO_GPIO);   /* PIN51 */
    (void)mux_sam3x_pio_output(&MUXB, (size_t)21, MUX_SAM3X_PIO_GPIO);  /* AD14(RXD3) */
    (void)mux_sam3x_pio_input(&MUXB, (size_t)14, MUX_SAM3X_PIO_GPIO);   /* CANTX1/IO */

    /* COMMUNICATON */
    (void)mux_sam3x_pio_input(&MUXA, (size_t)8, MUX_SAM3X_PIO_A);   /* RX */
    (void)mux_sam3x_pio_output(&MUXA, (size_t)9, MUX_SAM3X_PIO_A);  /* TX */
    (void)mux_sam3x_pio_input(&MUXD, (size_t)4, MUX_SAM3X_PIO_A);   /* TX3 */
    (void)mux_sam3x_pio_output(&MUXD, (size_t)5, MUX_SAM3X_PIO_A);  /* RX3 */
    (void)mux_sam3x_pio_output(&MUXA, (size_t)10, MUX_SAM3X_PIO_A); /* RX3 */
    (void)mux_sam3x_pio_input(&MUXA, (size_t)11, MUX_SAM3X_PIO_A);  /* TX3 */
    (void)mux_sam3x_pio_output(&MUXA, (size_t)12, MUX_SAM3X_PIO_A); /* RX2 */
    (void)mux_sam3x_pio_input(&MUXA, (size_t)13, MUX_SAM3X_PIO_A);  /* TX2 */
    (void)mux_sam3x_pio_output(&MUXB, (size_t)12, MUX_SAM3X_PIO_A); /* SDA */
    (void)mux_sam3x_pio_pull_up(&MUXB, (size_t)12);
    (void)mux_sam3x_pio_output(&MUXB, (size_t)13, MUX_SAM3X_PIO_A); /* SCL */
    (void)mux_sam3x_pio_pull_up(&MUXB, (size_t)12);

    /* PWM */
    (void)mux_sam3x_pio_output(&MUXB, (size_t)25, MUX_SAM3X_PIO_B); /* PWM2 */
    (void)mux_sam3x_pio_input(&MUXC, (size_t)28, MUX_SAM3X_PIO_B);  /* PWM3 */
    (void)mux_sam3x_pio_output(&MUXC, (size_t)26, MUX_SAM3X_PIO_B); /* PWM4 */
    (void)mux_sam3x_pio_output(&MUXC, (size_t)25, MUX_SAM3X_PIO_B); /* PWM5 */
    (void)mux_sam3x_pio_output(&MUXD, (size_t)7, MUX_SAM3X_PIO_B);  /* PWM11 */
    (void)mux_sam3x_pio_output(&MUXD, (size_t)8, MUX_SAM3X_PIO_B);  /* PWM12 */
    (void)mux_sam3x_pio_output(&MUXB, (size_t)27, MUX_SAM3X_PIO_B); /* PWM13 */

    /* MISC */
    (void)mux_sam3x_pio_input(&MUXA, (size_t)25, MUX_SAM3X_PIO_A);      /* MISO */
    (void)mux_sam3x_pio_output(&MUXA, (size_t)26, MUX_SAM3X_PIO_A);     /* MOSI */
    (void)mux_sam3x_pio_output(&MUXA, (size_t)27, MUX_SAM3X_PIO_A);     /* SPCK */
    (void)mux_sam3x_pio_output(&MUXA, (size_t)28, MUX_SAM3X_PIO_A);     /* SS0 */

    (void)mux_sam3x_pio_output(&MUXA, (size_t)17, MUX_SAM3X_PIO_A);     /* SDA1 */
    (void)mux_sam3x_pio_pull_up(&MUXB, (size_t)17);
    (void)mux_sam3x_pio_output(&MUXA, (size_t)18, MUX_SAM3X_PIO_A);     /* SCL1 */
    (void)mux_sam3x_pio_pull_up(&MUXB, (size_t)18);
}

static void arduino_due_init_analog_in(/*@partial@*/ struct arduino_due *ctx)
{
    /* clocks */
    (void)clock_sam3x_pmc_enable(CLOCK_SAM3X_PMC_PERIPH(PID_ADC), CLOCK_SAM3X_PMC_PERIPH_DIV_1);

    /* main ADC */
    static struct adc_sam3x ADC;

    (void)adc_sam3x_init(&ADC, ADDR_ADC);

    /* channels */
    static struct adc A0;
    static struct adc A1;
    static struct adc A2;
    static struct adc A3;
    static struct adc A4;
    static struct adc A5;
    static struct adc A6;
    static struct adc A7;
    static struct adc A8;
    static struct adc A9;
    static struct adc A10;
    static struct adc A11;

    struct adc_settings ADC_settings_mV = {
        4,  /* mult */
        5,  /* div */
        0   /* offset */
    };

    (void)adc_sam3x_adc_init(&A0, &ADC, (size_t)7);
    (void)adc_setup(&A0, &ADC_settings_mV);
    (void)adc_sam3x_adc_init(&A1, &ADC, (size_t)6);
    (void)adc_setup(&A1, &ADC_settings_mV);
    (void)adc_sam3x_adc_init(&A2, &ADC, (size_t)5);
    (void)adc_setup(&A2, &ADC_settings_mV);
    (void)adc_sam3x_adc_init(&A3, &ADC, (size_t)4);
    (void)adc_setup(&A3, &ADC_settings_mV);
    (void)adc_sam3x_adc_init(&A4, &ADC, (size_t)3);
    (void)adc_setup(&A4, &ADC_settings_mV);
    (void)adc_sam3x_adc_init(&A5, &ADC, (size_t)2);
    (void)adc_setup(&A5, &ADC_settings_mV);
    (void)adc_sam3x_adc_init(&A6, &ADC, (size_t)1);
    (void)adc_setup(&A6, &ADC_settings_mV);
    (void)adc_sam3x_adc_init(&A7, &ADC, (size_t)0);
    (void)adc_setup(&A7, &ADC_settings_mV);
    (void)adc_sam3x_adc_init(&A8, &ADC, (size_t)10);
    (void)adc_setup(&A8, &ADC_settings_mV);
    (void)adc_sam3x_adc_init(&A9, &ADC, (size_t)11);
    (void)adc_setup(&A9, &ADC_settings_mV);
    (void)adc_sam3x_adc_init(&A10, &ADC, (size_t)12);
    (void)adc_setup(&A10, &ADC_settings_mV);
    (void)adc_sam3x_adc_init(&A11, &ADC, (size_t)13);
    (void)adc_setup(&A11, &ADC_settings_mV);

    /* HAL */
    ctx->A0 = &A0;
    ctx->A1 = &A1;
    ctx->A2 = &A2;
    ctx->A3 = &A3;
    ctx->A4 = &A4;
    ctx->A5 = &A5;
    ctx->A6 = &A6;
    ctx->A7 = &A7;
    ctx->A8 = &A8;
    ctx->A9 = &A9;
    ctx->A10 = &A10;
    ctx->A11 = &A11;
}

static void arduino_due_init_can(/*@partial@*/ struct arduino_due *ctx)
{
    /* clocks */
    (void)clock_sam3x_pmc_enable(CLOCK_SAM3X_PMC_PERIPH(PID_CAN0), CLOCK_SAM3X_PMC_PERIPH_DIV_1);

    /* CAN */
    static struct can CAN;

    struct can_settings CAN_settings = {
        125000ul,   /* bitrate */
        (size_t)1,  /* tx_mailbox_count */
        true,       /* tx_auto_abort */
        true,       /* rx_overwrite */
        false       /* loopback */
    };

    (void)can_sam3x_init(&CAN, ADDR_CAN0, CLOCK_SAM3X_PMC_PERIPH(PID_CAN0));
    (void)can_setup(&CAN, &CAN_settings);

    /* HAL */
    ctx->CAN = &CAN;
}

static void arduino_due_init_digital(/*@partial@*/ struct arduino_due *ctx)
{
    /* clocks */
    (void)clock_sam3x_pmc_enable(CLOCK_SAM3X_PMC_PERIPH(PID_PIOA), CLOCK_SAM3X_PMC_PERIPH_DIV_1);
    (void)clock_sam3x_pmc_enable(CLOCK_SAM3X_PMC_PERIPH(PID_PIOB), CLOCK_SAM3X_PMC_PERIPH_DIV_1);
    (void)clock_sam3x_pmc_enable(CLOCK_SAM3X_PMC_PERIPH(PID_PIOC), CLOCK_SAM3X_PMC_PERIPH_DIV_1);
    (void)clock_sam3x_pmc_enable(CLOCK_SAM3X_PMC_PERIPH(PID_PIOD), CLOCK_SAM3X_PMC_PERIPH_DIV_1);

    /* gpio */
    static struct gpio DIGITAL22;
    static struct gpio DIGITAL23;
    static struct gpio DIGITAL24;
    static struct gpio DIGITAL25;
    static struct gpio DIGITAL26;
    static struct gpio DIGITAL27;
    static struct gpio DIGITAL28;
    static struct gpio DIGITAL29;
    static struct gpio DIGITAL30;
    static struct gpio DIGITAL31;
    static struct gpio DIGITAL32;
    static struct gpio DIGITAL33;
    static struct gpio DIGITAL34;
    static struct gpio DIGITAL35;
    static struct gpio DIGITAL36;
    static struct gpio DIGITAL37;
    static struct gpio DIGITAL38;
    static struct gpio DIGITAL39;
    static struct gpio DIGITAL40;
    static struct gpio DIGITAL41;
    static struct gpio DIGITAL42;
    static struct gpio DIGITAL43;
    static struct gpio DIGITAL44;
    static struct gpio DIGITAL45;
    static struct gpio DIGITAL46;
    static struct gpio DIGITAL47;
    static struct gpio DIGITAL48;
    static struct gpio DIGITAL49;
    static struct gpio DIGITAL50;
    static struct gpio DIGITAL51;
    static struct gpio DIGITAL52;
    static struct gpio DIGITAL53;

    (void)gpio_sam3x_pio_init(&DIGITAL22, ADDR_PIOB, (size_t)26);
    (void)gpio_sam3x_pio_init(&DIGITAL23, ADDR_PIOA, (size_t)14);
    (void)gpio_sam3x_pio_init(&DIGITAL24, ADDR_PIOA, (size_t)15);
    (void)gpio_sam3x_pio_init(&DIGITAL25, ADDR_PIOD, (size_t)0);
    (void)gpio_sam3x_pio_init(&DIGITAL26, ADDR_PIOD, (size_t)1);
    (void)gpio_sam3x_pio_init(&DIGITAL27, ADDR_PIOD, (size_t)2);
    (void)gpio_sam3x_pio_init(&DIGITAL28, ADDR_PIOD, (size_t)3);
    (void)gpio_sam3x_pio_init(&DIGITAL29, ADDR_PIOD, (size_t)6);
    (void)gpio_sam3x_pio_init(&DIGITAL30, ADDR_PIOD, (size_t)9);
    (void)gpio_sam3x_pio_init(&DIGITAL31, ADDR_PIOA, (size_t)7);
    (void)gpio_sam3x_pio_init(&DIGITAL32, ADDR_PIOD, (size_t)10);
    (void)gpio_sam3x_pio_init(&DIGITAL33, ADDR_PIOC, (size_t)1);
    (void)gpio_sam3x_pio_init(&DIGITAL34, ADDR_PIOC, (size_t)2);
    (void)gpio_sam3x_pio_init(&DIGITAL35, ADDR_PIOC, (size_t)3);
    (void)gpio_sam3x_pio_init(&DIGITAL36, ADDR_PIOC, (size_t)4);
    (void)gpio_sam3x_pio_init(&DIGITAL37, ADDR_PIOC, (size_t)5);
    (void)gpio_sam3x_pio_init(&DIGITAL38, ADDR_PIOC, (size_t)6);
    (void)gpio_sam3x_pio_init(&DIGITAL39, ADDR_PIOC, (size_t)7);
    (void)gpio_sam3x_pio_init(&DIGITAL40, ADDR_PIOC, (size_t)8);
    (void)gpio_sam3x_pio_init(&DIGITAL41, ADDR_PIOC, (size_t)9);
    (void)gpio_sam3x_pio_init(&DIGITAL42, ADDR_PIOA, (size_t)19);
    (void)gpio_sam3x_pio_init(&DIGITAL43, ADDR_PIOA, (size_t)20);
    (void)gpio_sam3x_pio_init(&DIGITAL44, ADDR_PIOC, (size_t)19);
    (void)gpio_sam3x_pio_init(&DIGITAL45, ADDR_PIOC, (size_t)18);
    (void)gpio_sam3x_pio_init(&DIGITAL46, ADDR_PIOC, (size_t)17);
    (void)gpio_sam3x_pio_init(&DIGITAL47, ADDR_PIOC, (size_t)16);
    (void)gpio_sam3x_pio_init(&DIGITAL48, ADDR_PIOC, (size_t)15);
    (void)gpio_sam3x_pio_init(&DIGITAL49, ADDR_PIOC, (size_t)14);
    (void)gpio_sam3x_pio_init(&DIGITAL50, ADDR_PIOC, (size_t)13);
    (void)gpio_sam3x_pio_init(&DIGITAL51, ADDR_PIOC, (size_t)12);
    (void)gpio_sam3x_pio_init(&DIGITAL52, ADDR_PIOB, (size_t)21);
    (void)gpio_sam3x_pio_init(&DIGITAL53, ADDR_PIOB, (size_t)14);

    /* HAL */
    ctx->DIGITAL22 = &DIGITAL22;
    ctx->DIGITAL23 = &DIGITAL23;
    ctx->DIGITAL24 = &DIGITAL24;
    ctx->DIGITAL25 = &DIGITAL25;
    ctx->DIGITAL26 = &DIGITAL26;
    ctx->DIGITAL27 = &DIGITAL27;
    ctx->DIGITAL28 = &DIGITAL28;
    ctx->DIGITAL29 = &DIGITAL29;
    ctx->DIGITAL30 = &DIGITAL30;
    ctx->DIGITAL31 = &DIGITAL31;
    ctx->DIGITAL32 = &DIGITAL32;
    ctx->DIGITAL33 = &DIGITAL33;
    ctx->DIGITAL34 = &DIGITAL34;
    ctx->DIGITAL35 = &DIGITAL35;
    ctx->DIGITAL36 = &DIGITAL36;
    ctx->DIGITAL37 = &DIGITAL37;
    ctx->DIGITAL38 = &DIGITAL38;
    ctx->DIGITAL39 = &DIGITAL39;
    ctx->DIGITAL40 = &DIGITAL40;
    ctx->DIGITAL41 = &DIGITAL41;
    ctx->DIGITAL42 = &DIGITAL42;
    ctx->DIGITAL43 = &DIGITAL43;
    ctx->DIGITAL44 = &DIGITAL44;
    ctx->DIGITAL45 = &DIGITAL45;
    ctx->DIGITAL46 = &DIGITAL46;
    ctx->DIGITAL47 = &DIGITAL47;
    ctx->DIGITAL48 = &DIGITAL48;
    ctx->DIGITAL49 = &DIGITAL49;
    ctx->DIGITAL50 = &DIGITAL50;
    ctx->DIGITAL51 = &DIGITAL51;
    ctx->DIGITAL52 = &DIGITAL52;
    ctx->DIGITAL53 = &DIGITAL53;
}

static void arduino_due_init_communication(/*@partial@*/ struct arduino_due *ctx)
{
    /* clocks */
    (void)clock_sam3x_pmc_enable(CLOCK_SAM3X_PMC_PERIPH(PID_UART), CLOCK_SAM3X_PMC_PERIPH_DIV_1);
    (void)clock_sam3x_pmc_enable(CLOCK_SAM3X_PMC_PERIPH(PID_TWI1), CLOCK_SAM3X_PMC_PERIPH_DIV_1);

    /* SERIAL0 */
    static struct uart SERIAL0;

    struct uart_settings UART_settings = {
        9600ul,     /* baudrate */
        (size_t)8,  /* character size */
        false,      /* parenb */
        false,      /* parodd */
        false       /* cstopb */
    };

    (void)uart_sam3x_init(&SERIAL0, ADDR_UART, CLOCK_SAM3X_PMC_PERIPH(PID_UART));
    (void)uart_setup(&SERIAL0, &UART_settings);

    /* I2C */
    static struct twi I2C;

    struct twi_settings TWI_settings = {
        TWI_BITRATE_STANDARD,
        TWI_MODE_MASTER,
        (twi_addr_t)0x69
    };

    (void)twi_sam3x_init(&I2C, ADDR_TWI1, CLOCK_SAM3X_PMC_PERIPH(PID_TWI1));
    (void)twi_setup(&I2C, &TWI_settings);

    /* HAL */
    ctx->SERIAL0 = &SERIAL0;
    ctx->I2C = &I2C;
}

static void arduino_due_init_pwm(/*@partial@*/ struct arduino_due *ctx)
{
    /* clocks */
    (void)clock_sam3x_pmc_enable(CLOCK_SAM3X_PMC_PERIPH(PID_TC0), CLOCK_SAM3X_PMC_PERIPH_DIV_1);
    (void)clock_sam3x_pmc_enable(CLOCK_SAM3X_PMC_PERIPH(PID_TC6), CLOCK_SAM3X_PMC_PERIPH_DIV_1);
    (void)clock_sam3x_pmc_enable(CLOCK_SAM3X_PMC_PERIPH(PID_TC7), CLOCK_SAM3X_PMC_PERIPH_DIV_1);
    (void)clock_sam3x_pmc_enable(CLOCK_SAM3X_PMC_PERIPH(PID_TC8), CLOCK_SAM3X_PMC_PERIPH_DIV_1);

    /* PWM */
    static struct pwm PWM2;
    static struct pwm PWM4;
    static struct pwm PWM5;
    static struct pwm PWM11;
    static struct pwm PWM12;
    static struct pwm PWM13;

    struct pwm_sam3x_tc_settings PWM_settings_A = {
        PWM_SAM3X_TC_TIOA,
        PWM_SAM3X_TC_TCCLKS_TIMER_CLOCK1,
        PWM_SAM3X_TC_WAVSEL_UP_RC,
        PWM_SAM3X_TC_CP_CLEAR,      /* nCPn */
        PWM_SAM3X_TC_CP_SET         /* nCPC */
    };

    /* invert signal on B outputs */
    struct pwm_sam3x_tc_settings PWM_settings_B = {
        PWM_SAM3X_TC_TIOB,
        PWM_SAM3X_TC_TCCLKS_TIMER_CLOCK1,
        PWM_SAM3X_TC_WAVSEL_UP_RC,
        PWM_SAM3X_TC_CP_SET,        /* nCPn */
        PWM_SAM3X_TC_CP_CLEAR       /* nCPC */
    };

    (void)pwm_sam3x_tc_init(&PWM2, ADDR_TC0, CLOCK_SAM3X_PMC_PERIPH(PID_TC0));
    (void)pwm_sam3x_tc_setup(&PWM2, &PWM_settings_A);
    (void)pwm_sam3x_tc_init(&PWM4, ADDR_TC6, CLOCK_SAM3X_PMC_PERIPH(PID_TC6));
    (void)pwm_sam3x_tc_setup(&PWM4, &PWM_settings_B);
    (void)pwm_sam3x_tc_init(&PWM5, ADDR_TC6, CLOCK_SAM3X_PMC_PERIPH(PID_TC6));
    (void)pwm_sam3x_tc_setup(&PWM5, &PWM_settings_A);
    (void)pwm_sam3x_tc_init(&PWM11, ADDR_TC8, CLOCK_SAM3X_PMC_PERIPH(PID_TC8));
    (void)pwm_sam3x_tc_setup(&PWM11, &PWM_settings_A);
    (void)pwm_sam3x_tc_init(&PWM12, ADDR_TC8, CLOCK_SAM3X_PMC_PERIPH(PID_TC8));
    (void)pwm_sam3x_tc_setup(&PWM12, &PWM_settings_B);
    (void)pwm_sam3x_tc_init(&PWM13, ADDR_TC0, CLOCK_SAM3X_PMC_PERIPH(PID_TC0));
    (void)pwm_sam3x_tc_setup(&PWM13, &PWM_settings_B);

    /* IPWM */
    static struct ipwm PWM3;

    struct ipwm_sam3x_tc_settings IPWM_settings = {
        PWM_SAM3X_TC_TCCLKS_TIMER_CLOCK1,
        IPWM_SAM3X_TC_LDR_RISING
    };

    (void)ipwm_sam3x_tc_init(&PWM3, ADDR_TC7, CLOCK_SAM3X_PMC_PERIPH(PID_TC7));
    (void)ipwm_sam3x_tc_setup(&PWM3, &IPWM_settings);

    /* HAL */
    ctx->PWM2 = &PWM2;
    ctx->PWM3 = &PWM3;
    ctx->PWM4 = &PWM4;
    ctx->PWM5 = &PWM5;
    ctx->PWM11 = &PWM11;
    ctx->PWM12 = &PWM12;
    ctx->PWM13 = &PWM13;
}

static void arduino_due_init_misc(/*@partial@*/ struct arduino_due *ctx)
{
    /* clocks */
    (void)clock_sam3x_pmc_enable(CLOCK_SAM3X_PMC_PERIPH(PID_SPI0), CLOCK_SAM3X_PMC_PERIPH_DIV_1);
    (void)clock_sam3x_pmc_enable(CLOCK_SAM3X_PMC_PERIPH(PID_TWI0), CLOCK_SAM3X_PMC_PERIPH_DIV_1);

    /* SPI */
    static struct spi SPI;

    struct spi_settings SPI_settings = {
        1000000ul, /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)8,  /* frame size */
        SPI_CS_POL_ACTIVE_LOW,
        (size_t)0   /* cs */
    };

    (void)spi_sam3x_init(&SPI, ADDR_SPI0, CLOCK_SAM3X_PMC_PERIPH(PID_SPI0));
    (void)spi_setup(&SPI, &SPI_settings);

    /* I2C1 */
    static struct twi I2C1;

    struct twi_settings TWI_settings = {
        TWI_BITRATE_IGNORE,
        TWI_MODE_SLAVE,
        (twi_addr_t)0x69
    };

    (void)twi_sam3x_init(&I2C1, ADDR_TWI0, CLOCK_SAM3X_PMC_PERIPH(PID_TWI0));
    (void)twi_setup(&I2C1, &TWI_settings);

    /* HAL */
    ctx->SPI = &SPI;
    ctx->I2C1 = &I2C1;
}

static void arduino_due_init_wd(/*@partial@*/ struct arduino_due *ctx)
{
    /* WDT */
    static struct wd WDT;

    struct wd_sam3x_settings WD_settings = {
        4ul,                 /* timeout ms */
    };

    (void)wd_sam3x_init(&WDT, ADDR_WDT, CLOCK_SAM3X_PMC_SCLK);
    (void)wd_sam3x_setup(&WDT, &WD_settings);

    /* HAL */
    ctx->WDT = &WDT;
}

int arduino_due_init(struct arduino_due *ctx)
{
    arduino_due_init_clock();
    arduino_due_init_mux();

    arduino_due_init_analog_in(ctx);
    arduino_due_init_can(ctx);
    arduino_due_init_digital(ctx);
    arduino_due_init_communication(ctx);
    arduino_due_init_pwm(ctx);
    arduino_due_init_misc(ctx);
    arduino_due_init_wd(ctx);

    return 0;
}
