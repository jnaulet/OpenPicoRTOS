#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_P0         0x80
#define ADDR_SP         0x81
#define ADDR_DPL        0x82
#define ADDR_DPH        0x83
#define ADDR_PCON       0x87
#define ADDR_TCON       0x88
#define ADDR_TMOD       0x89
#define ADDR_TL0        0x8a
#define ADDR_TL1        0x8b
#define ADDR_TH0        0x8c
#define ADDR_TH1        0x8d
#define ADDR_AUXR       0x8e
#define ADDR_WAKE_CLKO  0x8f
#define ADDR_P1         0x90
#define ADDR_P1M1       0x91
#define ADDR_P1M0       0x92
#define ADDR_P0M1       0x93
#define ADDR_P0M0       0x94
#define ADDR_P2M1       0x95
#define ADDR_P2M0       0x96
#define ADDR_CLK_DIV    0x97
#define ADDR_SCON       0x98
#define ADDR_SBUF       0x99
#define ADDR_S2CON      0x9a
#define ADDR_S2BUF      0x9b
#define ADDR_BRT        0x9c
#define ADDR_P1ASF      0x9d
#define ADDR_P2         0xa0
#define ADDR_BUS_SPEED  0xa1
#define ADDR_AUXR1      0xa2
#define ADDR_IE         0xa8
#define ADDR_SADDR      0xa9
#define ADDR_IE2        0xaf
#define ADDR_P3         0xb0
#define ADDR_P3M1       0xb1
#define ADDR_P3M0       0xb2
#define ADDR_P4M1       0xb3
#define ADDR_P4M0       0xb4
#define ADDR_IP2        0xb5
#define ADDR_IP2H       0xb6
#define ADDR_IPH        0xb7
#define ADDR_IP         0xb8
#define ADDR_SADEN      0xb9
#define ADDR_P4SW       0xbb
#define ADDR_ADC_CONTR  0xbc
#define ADDR_ADC_RES    0xbd
#define ADDR_ADC_RESL   0xbe
#define ADDR_P4         0xc0
#define ADDR_WDT_CONTR  0xc1
#define ADDR_IAP_DATA   0xc2
#define ADDR_IAP_ADDRH  0xc3
#define ADDR_IAP_ADDRL  0xc4
#define ADDR_IAP_CMD    0xc5
#define ADDR_IAP_TRIG   0xc6
#define ADDR_IAP_CONTR  0xc7
#define ADDR_P5         0xc8
#define ADDR_P5M1       0xc9
#define ADDR_P5M0       0xca
#define ADDR_SPSTAT     0xcd
#define ADDR_SPCTL      0xce
#define ADDR_PSW        0xd0
#define ADDR_CCON       0xd8
#define ADDR_CMOD       0xd9
#define ADDR_CCAPM0     0xda
#define ADDR_CCAPM1     0xdb
#define ADDR_ACC        0xe0
#define ADDR_CL         0xe9
#define ADDR_CCAP0L     0xea
#define ADDR_CCAP1L     0xeb
#define ADDR_B          0xf0
#define ADDR_PCA_PWM0   0xf2
#define ADDR_PCA_PWM1   0xf3
#define ADDR_CH         0xf9
#define ADDR_CCAP0H     0xfa
#define ADDR_CCAP1H     0xfb

/* IRQs */

#define IRQ_IE0          0  /* External Interrupt 0 */
#define IRQ_TF0          1  /* Timer 0 overflow */
#define IRQ_IE1          2  /* External Interrupt 1 */
#define IRQ_TF1          3  /* Timer 1 overflow */
#define IRQ_RI_TI        4  /* Serial port interrupt */
#define IRQ_ADC          5  /* ADC flag */
#define IRQ_LVD          6  /* Low voltage detection */
#define IRQ_CF_CCF0_CCF1 7  /* PWM */
#define IRQ_S2TI_S2RI    8  /* UART2 (S2) */
#define IRQ_SPIF         9  /* SPI */

/* DEVICE */

#define DEVICE_INTERRUPT_VECTOR_COUNT 10
#define DEVICE_DEFAULT_SYSCLK_HZ      22100000ul

/* timer t0 non-auto-reload */
extern void arch_reload_timer(void);

#endif
