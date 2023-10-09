#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_XIP     0x10000000
#define ADDR_XIP_SSI 0x18000000

#define ADDR_SYSINFO             0x40000000
#define ADDR_SYSCFG              0x40004000
#define ADDR_CLOCKS              0x40008000
#define ADDR_RESET               0x4000c000
#define ADDR_PSM                 0x40010000
#define ADDR_IO_BANK0            0x40014000
#define ADDR_IO_QSPI             0x40018000
#define ADDR_PADS_BANK0          0x4001c000
#define ADDR_PADS_QSPI           0x40020000
#define ADDR_XOSC                0x40024000
#define ADDR_PLL_SYS             0x40028000
#define ADDR_PLL_USB             0x4002c000
#define ADDR_BUSCTRL             0x40030000
#define ADDR_UART0               0x40034000
#define ADDR_UART1               0x40038000
#define ADDR_SPI0                0x4003c000
#define ADDR_SPI1                0x40040000
#define ADDR_I2C0                0x40044000
#define ADDR_I2C1                0x40048000
#define ADDR_ADC                 0x4004c000
#define ADDR_PWM                 0x40050000
#define ADDR_TIMER               0x40054000
#define ADDR_WATCHDOG            0x40058000
#define ADDR_RTC                 0x4005c000
#define ADDR_ROSC                0x40060000
#define ADDR_VREG_AND_CHIP_RESET 0x40064000
#define ADDR_TBMAN               0x4006c000

#define ADDR_DMA 0x50000000

#define ADDR_USBCTRL       0x50100000
#define ADDR_USBCTRL_DPRAM 0x50100000
#define ADDR_USBCTRL_REGS  0x50110000

#define ADDR_PIO0    0x50200000
#define ADDR_PIO1    0x50300000
#define ADDR_XIP_AUX 0x50400000

#define ADDR_SIO      0xd0000000
#define ADDR_SIO_GPIO 0xd0000004

#define ADDR_PPB 0xe0000000

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

/* The startup frequency is typically 6MHz but varies with PVT */
#define DEVICE_DEFAULT_SYSCLK_HZ 6000000

#endif
