#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_PREFETCH 0xbf8e0000
#define ADDR_EBI      0xbf8e1000
#define ADDR_SQI1     0xbf8e2000
#define ADDR_USB      0xbf8e3000
#define ADDR_CRYPTO   0xbf8e5000
#define ADDR_RNG      0xbf8e6000

#define ADDR_CAN1_AND_CAN2 0xbf880000
#define ADDR_CAN1          0xbf880000
#define ADDR_CAN2          0xbf881000

#define ADDR_ETHERNET      0xbf882000
#define ADDR_USBCR         0xbf884000

#define ADDR_PORTA_PORTK 0xbf860000
#define ADDR_PORTA       0xbf860000
#define ADDR_PORTB       0xbf860100
#define ADDR_PORTC       0xbf860200
#define ADDR_PORTD       0xbf860300
#define ADDR_PORTE       0xbf860400
#define ADDR_PORTF       0xbf860500
#define ADDR_PORTG       0xbf860600
#define ADDR_PORTH       0xbf860700
#define ADDR_PORTJ       0xbf860800
#define ADDR_PORTK       0xbf860900

#define ADDR_TIMER1_TIMER9  0xbf840000
#define ADDR_TIMER1         0xbf840000
#define ADDR_TIMER2         0xbf840200
#define ADDR_TIMER3         0xbf840400
#define ADDR_TIMER4         0xbf840600
#define ADDR_TIMER5         0xbf840800
#define ADDR_TIMER6         0xbf840a00
#define ADDR_TIMER7         0xbf840c00
#define ADDR_TIMER8         0xbf840e00
#define ADDR_TIMER9         0xbf841000

#define ADDR_IC1_IC9        0xbf842000
#define ADDR_IC1            0xbf842000
#define ADDR_IC2            0xbf842200
#define ADDR_IC3            0xbf842400
#define ADDR_IC4            0xbf842600
#define ADDR_IC5            0xbf842800
#define ADDR_IC6            0xbf842a00
#define ADDR_IC7            0xbf842c00
#define ADDR_IC8            0xbf842e00
#define ADDR_IC9            0xbf843000

#define ADDR_OC1_OC9        0xbf844000
#define ADDR_OC1            0xbf844000
#define ADDR_OC2            0xbf844200
#define ADDR_OC3            0xbf844400
#define ADDR_OC4            0xbf844600
#define ADDR_OC5            0xbf844800
#define ADDR_OC6            0xbf844a00
#define ADDR_OC7            0xbf844c00
#define ADDR_OC8            0xbf844e00
#define ADDR_OC9            0xbf845000

#define ADDR_ADC            0xbf84b000

#define ADDR_COMPARATOR_1_2 0xbf84c000
#define ADDR_COMPARATOR_1   0xbf84c000
#define ADDR_COMPARATOR_2   0xbf84c010

#define ADDR_I2C1_I2C5   0xbf820000
#define ADDR_I2C1        0xbf820000
#define ADDR_I2C2        0xbf820200
#define ADDR_I2C3        0xbf820400
#define ADDR_I2C4        0xbf820600
#define ADDR_I2C5        0xbf820800

#define ADDR_SPI1_SPI6   0xbf821000
#define ADDR_SPI1        0xbf821000
#define ADDR_SPI2        0xbf821200
#define ADDR_SPI3        0xbf821400
#define ADDR_SPI4        0xbf821600
#define ADDR_SPI5        0xbf821800
#define ADDR_SPI6        0xbf821a00

#define ADDR_UART1_UART6 0xbf822000     /* ! */
#define ADDR_UART1       0xbf822000
#define ADDR_UART2       0xbf822200
#define ADDR_UART3       0xbf822400
#define ADDR_UART4       0xbf822600
#define ADDR_UART5       0xbf822800
#define ADDR_UART6       0xbf822a00

#define ADDR_PMP         0xbf82e000

#define ADDR_INTC 0xbf810000
#define ADDR_DMA  0xbf811000

#define ADDR_CONFIGURATION    0xbf800000
#define ADDR_FLASH_CONTROLLER 0xbf800600
#define ADDR_WATCHDOG_TIMER   0xbf800800
#define ADDR_DEADMAN_TIMER    0xbf800a00
#define ADDR_RTCC             0xbf800c00
#define ADDR_CVREF            0xbf800e00
#define ADDR_OSCILLATOR       0xbf801200
#define ADDR_PPS              0xbf801400

/* Factory programmed flash */

#define ADDR_DEVADCx 0xbfc54000
#define ADDR_DEVSNx  0xbfc54020

/* DEVICE */

#define DEVICE_INTERRUPT_VECTOR_COUNT   213
#define DEVICE_ADC_DEDICATED_SAR_COUNT  5
#define DEVICE_ADC_SHARED_CL2_SAR_COUNT 7
#define DEVICE_ADC_SHARED_CL3_SAR_COUNT 33

#endif
