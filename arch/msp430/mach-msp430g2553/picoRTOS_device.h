#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_IE1  0x00
#define ADDR_IE2  0x01
#define ADDR_IFG1 0x02
#define ADDR_IFG2 0x03

#define ADDR_DIGITAL_IO        0x10
#define ADDR_CLOCK_MODULE_PLUS 0x53
#define ADDR_WDTPLUS           0x120

#define ADDR_CALBC1_16MHZ 0x10f8
#define ADDR_CALDCO_16MHZ 0x10f9
#define ADDR_CALBC1_12MHZ 0x10fa
#define ADDR_CALDCO_12MHZ 0x10fb
#define ADDR_CALBC1_8MHZ  0x10fc
#define ADDR_CALDCO_8MHZ  0x10fd
#define ADDR_CALBC1_1MHZ  0x10fe
#define ADDR_CALDCO_1MHZ  0x10ff

/* IRQs */

#define IRQ_TRAPINT     17
#define IRQ_PORT1       19
#define IRQ_PORT2       20
#define IRQ_ADC10       21
#define IRQ_USCIAB0TX   22
#define IRQ_USCIAB0RX   23
#define IRQ_TIMER0_A1   24
#define IRQ_TIMER0_A0   25
#define IRQ_WDT         26
#define IRQ_COMPARATORA 27
#define IRQ_TIMER1_A1   28
#define IRQ_TIMER1_A0   29
#define IRQ_NMI         30

/* DEVICE */

#define DEVICE_INTERRUPT_VECTOR_COUNT 32

/* TICK_HZ is device-specific */

#define CONFIG_TICK_HZ (CONFIG_ACLK_FREQ / 64)

#endif
