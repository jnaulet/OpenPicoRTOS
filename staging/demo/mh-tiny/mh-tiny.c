#include "mh-tiny.h"
#include "clock-attiny88.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

static void init_mux(void)
{
    struct mux PORTB;
    struct mux PORTD;

    (void)mux_avr_init(&PORTB, ADDR_PORTB);
    (void)mux_avr_init(&PORTD, ADDR_PORTD);

    (void)mux_avr_output(&PORTB, (size_t)5, false); /* SCK */
    (void)mux_avr_input(&PORTB, (size_t)4);         /* MISO */
    (void)mux_avr_output(&PORTB, (size_t)3, false); /* MOSI */
    (void)mux_avr_output(&PORTB, (size_t)2, true);  /* _SS */

    (void)mux_avr_output(&PORTD, (size_t)0, false); /* LED */
    (void)mux_avr_output(&PORTD, (size_t)1, true); /* TICK */
}

static void init_spi(/*@partial@*/ struct mh_tiny *ctx)
{
    struct spi_settings SPI_settings = {
        1000000ul,                           /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)16,                           /* frame size */
        SPI_CS_POL_IGNORE,
        (size_t)0,
    };

    static uint8_t rx_buf[8];
    static uint8_t tx_buf[8];
    struct spi_avr_irqdriven_settings IRQ_settings = {
      (picoRTOS_irq_t)IRQ_SPI_STC,
      rx_buf,
      tx_buf,
      (uint8_t)sizeof(rx_buf) - 1,
    };
    
    (void)spi_avr_init(&ctx->SPI, ADDR_SPI, CLOCK_ATTINY88_CLKIO);
    (void)spi_setup(&ctx->SPI, &SPI_settings);
    /* IRQ mode */
    (void)spi_avr_setup_irqdriven(&ctx->SPI, &IRQ_settings);
}

static void init_gpio(/*@partial@*/ struct mh_tiny *ctx)
{
    /* LED + TICK */
    (void)gpio_avr_init(&ctx->LED, ADDR_PORTD, (size_t)0);
    (void)gpio_avr_init(&ctx->TICK, ADDR_PORTD, (size_t)1);
}

static void init_wd(/*@partial@*/ struct mh_tiny *ctx)
{
    struct wd_avr_settings WD_settings = {
        WD_AVR_CYCLES_2K,
    };

    (void)wd_avr_init(&ctx->WD, ADDR_WDT);
    (void)wd_avr_setup(&ctx->WD, &WD_settings);

    (void)wd_start(&ctx->WD);
}

int mh_tiny_init(struct mh_tiny *ctx)
{
    init_mux();
    init_spi(ctx);
    init_gpio(ctx);
    init_wd(ctx);

    return 0;
}
