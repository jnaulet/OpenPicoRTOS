#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_PORTA 0x20
#define ADDR_PORTB 0x23
#define ADDR_PORTC 0x26
#define ADDR_PORTD 0x29
#define ADDR_PORTE 0x2c
#define ADDR_PORTF 0x2f
#define ADDR_PORTG 0x32
#define ADDR_PORTH 0x100
#define ADDR_PORTJ 0x103
#define ADDR_PORTK 0x106
#define ADDR_PORTL 0x109

#define ADDR_TIFR0 0x35
#define ADDR_TIFR1 0x36
#define ADDR_TIFR2 0x37
#define ADDR_TIFR3 0x38
#define ADDR_TIFR4 0x39
#define ADDR_TIFR5 0x3a

#define ADDR_PCIFR 0x3b
#define ADDR_EIFR  0x3c
#define ADDR_EIMSK 0x3d

#define ADDR_GPIOR0 0x3e

#define ADDR_EEPROM 0x3f

#define ADDR_GTCCR 0x43

#define ADDR_TC0 0x44
#define ADDR_TC1 0x80
#define ADDR_TC2 0xb0
#define ADDR_TC3 0x90
#define ADDR_TC4 0xa0
#define ADDR_TC5 0x120

#define ADDR_SPI 0x4c

#define ADDR_SPMCSR 0x57

#define ADDR_WDT 0x60

#define ADDR_TIMSK0 0x6e
#define ADDR_TIMSK1 0x6f
#define ADDR_TIMSK2 0x70
#define ADDR_TIMSK3 0x71
#define ADDR_TIMSK4 0x72
#define ADDR_TIMSK5 0x73

#define ADDR_ADC 0x78

#define ADDR_TWI 0xb8

#define ADDR_USART0 0xc0
#define ADDR_USART1 0xc8
#define ADDR_USART2 0xd0

/* IRQs */

#define IRQ_INT0         1
#define IRQ_INT1         2
#define IRQ_INT2         3
#define IRQ_INT3         4
#define IRQ_INT4         5
#define IRQ_INT5         6
#define IRQ_INT6         7
#define IRQ_INT7         8
#define IRQ_PCINT0       9
#define IRQ_PCINT1       10
#define IRQ_PCINT2       11
#define IRQ_WDT          12
#define IRQ_TIMER2_COMPA 13
#define IRQ_TIMER2_COMPB 14
#define IRQ_TIMER2_OVF   15
#define IRQ_TIMER1_CAPT  16
#define IRQ_TIMER1_COMPA 17
#define IRQ_TIMER1_COMPB 18
#define IRQ_TIMER1_COMPC 19
#define IRQ_TINER1_OVF   20
#define IRQ_TIMER0_COMPA 21
#define IRQ_TIMER0_COMPB 22
#define IRQ_TIMER0_OVF   23
#define IRQ_SPI_STC      24
#define IRQ_USART0_RX    25
#define IRQ_USART0_UDRE  26
#define IRQ_USART0_TX    27
#define IRQ_ANALOG_COMP  28
#define IRQ_ADC          29
#define IRQ_EE_READY     30
#define IRQ_TIMER3_CAPT  31
#define IRQ_TIMER3_COMPA 32
#define IRQ_TIMER3_COMPB 33
#define IRQ_TIMER3_COMPC 34
#define IRQ_TIMR3_OVF    35
#define IRQ_USART1_RX    36
#define IRQ_USART1_UDE   37
#define IRQ_USART1_TX    38
#define IRQ_TWI          39
#define IRQ_SPM_READY    40
#define IRQ_TIMER4_CAPT  41
#define IRQ_TIMER4_COMPA 42
#define IRQ_TIMER4_COMPB 43
#define IRQ_TIMER4_COMPC 44
#define IRQ_TIMER4_OVF   45
#define IRQ_TIMER5_CAPT  46
#define IRQ_TIMER5_COMPA 47
#define IRQ_TIMER5_COMPB 48
#define IRQ_TIMER5_COMPC 49
#define IRQ_TIMER5_OVF   50
#define IRQ_USART2_RX    51
#define IRQ_USART2_UDRE  52
#define IRQ_USART2_TX    53
#define IRQ_USART3_RX    54
#define IRQ_USART3_UDRE  55
#define IRQ_USART3_TX    56

/* DEVICE INFO */

#define DEVICE_INTERRUPT_VECTOR_COUNT 56

#endif
