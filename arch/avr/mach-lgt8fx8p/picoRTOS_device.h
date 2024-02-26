#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_PORTB  0x23
#define ADDR_PORTC  0x26
#define ADDR_PORTD  0x29
#define ADDR_TIFR0  0x35
#define ADDR_TIFR1  0x36
#define ADDR_TIFR2  0x37
#define ADDR_PCIFR  0x3b
#define ADDR_EIFR   0x3c
#define ADDR_EIMSK  0x3d
#define ADDR_GPIOR0 0x3e
#define ADDR_GPIOR1 0x4a
#define ADDR_GPIOR2 0x4b
#define ADDR_EEPROM 0x3f
#define ADDR_GTCCR  0x43
#define ADDR_TC0    0x44
#define ADDR_TC1    0x80
#define ADDR_TC2    0xb0
#define ADDR_SPI    0x4c
#define ADDR_SPMCSR 0x57
#define ADDR_WDT    0x60
#define ADDR_CLKPR  0x61
#define ADDR_TIMSK0 0x6e
#define ADDR_TIMSK1 0x6f
#define ADDR_TIMSK2 0x70
#define ADDR_ADC    0x78
#define ADDR_TWI    0xb8
#define ADDR_USART  0xc0
#define ADDR_PMCR   0xf2

/* IRQs */

#define IRQ_INT0         1
#define IRQ_INT1         2
#define IRQ_PCINT0       3
#define IRQ_PCINT1       4
#define IRQ_PCINT2       5
#define IRQ_WDT          6
#define IRQ_TIMER2_COMPA 7
#define IRQ_TIMER2_COMPB 8
#define IRQ_TIMER2_OVF   9
#define IRQ_TIMER1_CAPT  10
#define IRQ_TIMER1_COMPA 11
#define IRQ_TIMER1_COMPB 12
#define IRQ_TINER1_OVF   13
#define IRQ_TIMER0_COMPA 14
#define IRQ_TIMER0_COMPB 15
#define IRQ_TIMER0_OVF   16
#define IRQ_SPI_STC      17
#define IRQ_USART_RX     18
#define IRQ_USART_UDRE   19
#define IRQ_USART_TX     20
#define IRQ_ADC          21
#define IRQ_EE_READY     22
#define IRQ_ANALOG_COMP  23
#define IRQ_TWI          24
#define IRQ_SPM_READY    25

/* DEVICE INFO */

#define DEVICE_INTERRUPT_VECTOR_COUNT 25
#define DEVICE_DEFAULT_SYSCLK_HZ      32000000ul

#endif
