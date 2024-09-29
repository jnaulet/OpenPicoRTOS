#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* ATTINY414 Device */

/* MEMORY MAPPING */

#define ADDR_VPORTA  0x0000
#define ADDR_VPORTB  0x0004
#define ADDR_GPIO    0x001c
#define ADDR_CPU     0x0030
#define ADDR_RSTCTRL 0x0040
#define ADDR_SLPCTRL 0x0050
#define ADDR_CLKCTRL 0x0060
#define ADDR_BOD     0x0080
#define ADDR_VREF    0x00a0
#define ADDR_WDT     0x0100
#define ADDR_CPUINT  0x0110
#define ADDR_CRCSCAN 0x0120
#define ADDR_RTC     0x0140
#define ADDR_EVSYS   0x0180
#define ADDR_CCL     0x01c0
#define ADDR_PORTMUX 0x0200
#define ADDR_PORTA   0x0400
#define ADDR_PORTB   0x0420
#define ADDR_ADC0    0x0600
#define ADDR_AC0     0x0670
#define ADDR_DAC0    0x0680
#define ADDR_USART0  0x0800
#define ADDR_TWI0    0x0810
#define ADDR_SPI0    0x0820
#define ADDR_TCA0    0x0a00
#define ADDR_TCB0    0x0a40
#define ADDR_TCD0    0x0a80
#define ADDR_SYSCFG  0x0f00
#define ADDR_NVMCTRL 0x1000
#define ADDR_SIGROW  0x1100
#define ADDR_FUSES   0x1280
#define ADDR_USERROW 0x1300

/* IRQs */

#define IRQ_CRCSCAN_NMI 1
#define IRQ_BOD_VLM     2
#define IRQ_PORTA_PORT  3
#define IRQ_PORTB_PORT  4
#define IRQ_RTC_CNT     6
#define IRQ_RTC_PIT     7
#define IRQ_TCA0_OVF    8
#define IRQ_TCA0_LUNF   8
#define IRQ_TCA0_HUNF   9
#define IRQ_TCA0_CMP0   10
#define IRQ_TCA0_LCMP0  10
#define IRQ_TCA0_CMP1   11
#define IRQ_TCA0_LCMP1  11
#define IRQ_TCA0_CMP2   12
#define IRQ_TCA0_LCMP2  12
#define IRQ_TCB0_INT    13
#define IRQ_TCD0_OVF    14
#define IRQ_TCD0_TRIG   15
#define IRQ_AC0_AC      16
#define IRQ_AC0_RESRDY  17
#define IRQ_AC0_WCOMP   18
#define IRQ_TWI0_TWIS   19
#define IRQ_TWI0_TWIM   20
#define IRQ_SPI0_INT    21
#define IRQ_USART0_RXC  22
#define IRQ_USART0_DRE  23
#define IRQ_USART0_TXC  24
#define IRQ_NVMCTRL_EE  25

/* DEVICE INFO */

#define DEVICE_INTERRUPT_VECTOR_COUNT 25

#endif
