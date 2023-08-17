#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_PORTB 0x23
#define ADDR_PORTC 0x26
#define ADDR_PORTD 0x29
#define ADDR_PORTE 0x2c
#define ADDR_PORTF 0x2f

#define ADDR_TIFR0 0x35
#define ADDR_TIFR1 0x36
#define ADDR_TIFR3 0x38
#define ADDR_TIFR4 0x39

#define ADDR_PCIFR 0x3b
#define ADDR_EIFR  0x3c
#define ADDR_EIMSK 0x3d

#define ADDR_GPIOR0 0x3e

#define ADDR_EEPROM 0x3f

#define ADDR_GTCCR 0x43

#define ADDR_TC0 0x44
#define ADDR_TC1 0x80
#define ADDR_TC3 0x90

#define ADDR_SPI 0x4c

#define ADDR_SPMCSR 0x57

#define ADDR_WDT 0x60

#define ADDR_TIMSK0 0x6e
#define ADDR_TIMSK1 0x6f
#define ADDR_TIMSK3 0x71
#define ADDR_TIMSK4 0x72

#define ADDR_ADC 0x78

#define ADDR_TWI 0xb8

#define ADDR_USART1 0xc8

/* IRQs */

#define IRQ_INT0         2
#define IRQ_INT1         3
#define IRQ_INT2         4
#define IRQ_INT3         5
#define IRQ_INT6         8
#define IRQ_PCINT0       10
#define IRQ_USB_GENERAL  11
#define IRQ_USB_ENDPOINT 12
#define IRQ_WDT          13
#define IRQ_TIMER1_CAPT  17
#define IRQ_TIMER1_COMPA 18
#define IRQ_TIMER1_COMPB 19
#define IRQ_TIMER1_COMPC 20
#define IRQ_TINER1_OVF   21
#define IRQ_TIMER0_COMPA 22
#define IRQ_TIMER0_COMPB 23
#define IRQ_TIMER0_OVF   24
#define IRQ_SPI_STC      25
#define IRQ_USART1_RX    26
#define IRQ_USART1_UDRE  27
#define IRQ_USART1_TX    28
#define IRQ_ANALOG_COMP  29
#define IRQ_ADC          30
#define IRQ_EE_READY     31
#define IRQ_TIMER3_CAPT  32
#define IRQ_TIMER3_COMPA 33
#define IRQ_TIMER3_COMPB 34
#define IRQ_TIMER3_COMPC 35
#define IRQ_TINER3_OVF   36
#define IRQ_TWI          37
#define IRQ_SPM_READY    38
#define IRQ_TIMER4_COMPA 39
#define IRQ_TIMER4_COMPB 40
#define IRQ_TIMER4_COMPD 41
#define IRQ_TIMER4_OVF   42
#define IRQ_TIMER4_FPF   43

/* DEVICE INFO */

#define DEVICE_INTERRUPT_VECTOR_COUNT 42

#endif
