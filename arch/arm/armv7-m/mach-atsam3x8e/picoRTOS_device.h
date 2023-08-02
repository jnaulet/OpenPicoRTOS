#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_SPI0    0x40008000
#define ADDR_SPI1    0x4000c000

#define ADDR_TC0     0x40080000
#define ADDR_TC1     0x40080040
#define ADDR_TC2     0x40080080
#define ADDR_TC3     0x40084000
#define ADDR_TC4     0x40084040
#define ADDR_TC5     0x40084080
#define ADDR_TC6     0x40088000
#define ADDR_TC7     0x40088040
#define ADDR_TC8     0x40088080

#define ADDR_TWI0    0x4008c000
#define ADDR_TWI1    0x40090000

#define ADDR_PWM     0x40094000

#define ADDR_USART0  0x40098000
#define ADDR_USART1  0x4009c000
#define ADDR_USART2  0x400a0000
#define ADDR_USART3  0x400a4000

#define ADDR_CAN0    0x400b4000
#define ADDR_CAN1    0x400b8000

#define ADDR_ADC     0x400c0000

#define ADDR_DMAC    0x400c4000

#define ADDR_DACC    0x400c8000

#define ADDR_PMC     0x400e0600

#define ADDR_UART    0x400e0800

#define ADDR_EEFC0   0x400e0a00
#define ADDR_EEFC1   0x400e0c00

#define ADDR_PIOA    0x400e0e00
#define ADDR_PIOB    0x400e1000
#define ADDR_PIOC    0x400e1200
#define ADDR_PIOD    0x400e1400
#define ADDR_PIOE    0x400e1600
#define ADDR_PIOF    0x400e1800

#define ADDR_WDT     0x400e1a50

#define ADDR_RTC     0x400e1a60

/* IRQs */

#define IRQ(pid) (pid)

/* PERIPHERAL IDs */

#define DEVICE_PID_COUNT  45

#define PID_SUPC   0u
#define PID_RSTC   1u
#define PID_RTC    2u
#define PID_WDT    4u
#define PID_PMC    5u
#define PID_UART   8u
#define PID_PIOA   11u
#define PID_PIOB   12u
#define PID_PIOC   13u
#define PID_PIOD   14u
#define PID_PIOE   15u
#define PID_PIOF   16u
#define PID_USART0 17u
#define PID_USART1 18u
#define PID_USART2 19u
#define PID_USART3 20u
#define PID_TWI0   22u
#define PID_TWI1   23u
#define PID_SPI0   24u
#define PID_SPI1   25u
#define PID_TC0    27u
#define PID_TC1    28u
#define PID_TC2    29u
#define PID_TC3    30u
#define PID_TC4    31u
#define PID_TC5    32u
#define PID_TC6    33u
#define PID_TC7    34u
#define PID_TC8    35u
#define PID_PWM    36u
#define PID_ADC    37u
#define PID_DACC   38u
#define PID_DMAC   39u
#define PID_TRNG   41u
#define PID_EMAC   42u
#define PID_CAN0   43u
#define PID_CAN1   44u

/* DEVICE */

#define DEVICE_INTERRUPT_VECTOR_COUNT DEVICE_PID_COUNT

#endif
