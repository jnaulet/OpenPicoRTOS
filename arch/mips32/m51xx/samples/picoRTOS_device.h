#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

#ifndef S_SPLINT_S
# include <xc.h>
# include <sys/asm.h>
#endif

/* MEMORY MAPPING */

#define ADDR_PREFETCH 0xbf8e0000
#define ADDR_EBI      0xbf8e1000
#define ADDR_SQI1     0xbf8e2000
#define ADDR_USB      0xbf8e3000
#define ADDR_CRYPTO   0xbf8e5000
#define ADDR_RNG      0xbf8e6000

#define ADDR_CAN1_AND_CAN2 0xbf880000 /* FIXME ? */
#define ADDR_ETHERNET      0xbf882000
#define ADDR_USBCR         0xbf884000

#define ADDR_PORTA_PORTK 0xbf860000     /* FIXME */

#define ADDR_TIMER1_TIMER9  0xbf840000  /* FIXME */
#define ADDR_IC1_IC9        0xbf842000  /* FIXME */
#define ADDR_OC1_OC9        0xbf844000  /* FIXME */
#define ADDR_ADC            0xbf84b000
#define ADDR_COMPARATOR_1_2 0xbf84c000  /* FIXME */

#define ADDR_I2C1_I2C5   0xbf820000     /* ! */
#define ADDR_SPI1_SPI6   0xbf821000     /* ! */
#define ADDR_UART1_UART6 0xbf822000     /* ! */
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

/* DEVICE */

#define DEVICE_INTERRUPT_VECTOR_COUNT 213

#endif
