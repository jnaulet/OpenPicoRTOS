#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_PWM 0x40050000

#define ADDR_SIO 0xd0000000

/* IRQs */

#define IRQ_TIMER_0   0
#define IRQ_TIMER_1   1
#define IRQ_TIMER_2   2
#define IRQ_TIMER_3   3
#define IRQ_PWM_WRAP  4
#define IRQ_USBCTRL   5
#define IRQ_XIP       6
#define IRQ_PIO0_0    7
#define IRQ_PIO0_1    8
#define IRQ_PIO1_0    9
#define IRQ_PIO1_1    10
#define IRQ_DMA_0     11
#define IRQ_DMA_1     12
#define IRQ_IO_BANK0  13
#define IRQ_IO_QSPI   14
#define IRQ_SIO_PROC0 15
#define IRQ_SIO_PROC1 16
#define IRQ_CLOCKS    17
#define IRQ_SPI0      18
#define IRQ_SPI1      19
#define IRQ_UART0     20
#define IRQ_UART1     21
#define IRQ_ADC_FIFO  22
#define IRQ_I2C0      23
#define IRQ_I2C1      24
#define IRQ_RTC       25

/* DEVICE */

#define DEVICE_INTERRUPT_VECTOR_COUNT 26

#endif
