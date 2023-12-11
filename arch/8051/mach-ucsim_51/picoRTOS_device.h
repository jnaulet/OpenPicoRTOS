#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

#define ADDR_P0     0x80
#define ADDR_SP     0x81
#define ADDR_DPL    0x82
#define ADDR_DPH    0x83
#define ADDR_PCON   0x87
#define ADDR_TCON   0x88
#define ADDR_TMOD   0x89
#define ADDR_TL0    0x8a
#define ADDR_TL1    0x8b
#define ADDR_TH0    0x8c
#define ADDR_TH1    0x8d
#define ADDR_CKCON  0x8e
#define ADDR_P1     0x90
#define ADDR_SCON   0x98
#define ADDR_SBUF   0x99
#define ADDR_P2     0xa0
#define ADDR_IE     0xa8
#define ADDR_P3M1   0xac
#define ADDR_P3M2   0xad
#define ADDR_P3     0xb0
#define ADDR_P0M1   0xb1
#define ADDR_P0M2   0xb2
#define ADDR_P1M1   0xb3
#define ADDR_P1M2   0xb4
#define ADDR_P2S    0xb5
#define ADDR_IP     0xb8
#define ADDR_T2CON  0xc8
#define ADDR_RCAP2L 0xca
#define ADDR_RCAP2H 0xcb
#define ADDR_TL2    0xcc
#define ADDR_TH2    0xcd
#define ADDR_PSW    0xd0
#define ADDR_EIE    0xe8
#define ADDR_B      0xf0
#define ADDR_EIP    0xf8

/* IRQs */

#define IRQ_IE_0    0   /* External Interrupt 0 */
#define IRQ_TF_0    1   /* Timer 0 overflow */
#define IRQ_IE_1    2   /* External Interrupt 1 */
#define IRQ_TF_1    3   /* Timer 1 overflow */
#define IRQ_RI_TI_0 4   /* Serial port 0 interrupt */
#define IRQ_TF_2    5   /* Timer 2 event */
#define IRQ_PIF     6   /* Pin interrupt */
#define IRQ_RI_TI_1 7   /* Serial port 1 interrupt */
#define IRQ_USB     8   /* USB interrupt */
#define IRQ_I2C     9   /* I2C interrupt */
#define IRQ_IE_4    10  /* GPIF / FIFOs / INT4 Pin */
#define IRQ_IE_5    11  /* INT5 Pin */
#define IRQ_IE_6    12  /* INT6 Pin */

#define DEVICE_INTERRUPT_VECTOR_COUNT 12
#define DEVICE_DEFAULT_SYSCLK_HZ      12000000ul

/* TIMER2 REGs */

#define ADDR_xIE_wET2 ADDR_IE
#define xIE_ET2       (1 << 5)

#endif
