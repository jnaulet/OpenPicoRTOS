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

#define IRQ_INT0         1
#define IRQ_INT1         2
#define IRQ_INT2         3
#define IRQ_INT3         4
#define IRQ_INT6         7
#define IRQ_PCINT0       9
#define IRQ_USB_GENERAL  10
#define IRQ_USB_ENDPOINT 11
#define IRQ_WDT          12
#define IRQ_TIMER1_CAPT  16
#define IRQ_TIMER1_COMPA 17
#define IRQ_TIMER1_COMPB 18
#define IRQ_TIMER1_COMPC 19
#define IRQ_TINER1_OVF   20
#define IRQ_TIMER0_COMPA 21
#define IRQ_TIMER0_COMPB 22
#define IRQ_TIMER0_OVF   23
#define IRQ_SPI_STC      24
#define IRQ_USART1_RX    25
#define IRQ_USART1_UDRE  26
#define IRQ_USART1_TX    27
#define IRQ_ANALOG_COMP  28
#define IRQ_ADC          29
#define IRQ_EE_READY     30
#define IRQ_TIMER3_CAPT  31
#define IRQ_TIMER3_COMPA 32
#define IRQ_TIMER3_COMPB 33
#define IRQ_TIMER3_COMPC 34
#define IRQ_TINER3_OVF   35
#define IRQ_TWI          36
#define IRQ_SPM_READY    37
#define IRQ_TIMER4_COMPA 38
#define IRQ_TIMER4_COMPB 39
#define IRQ_TIMER4_COMPD 40
#define IRQ_TIMER4_OVF   41
#define IRQ_TIMER4_FPF   42

/* DEVICE INFO */

#define DEVICE_INTERRUPT_VECTOR_COUNT 42

#endif
