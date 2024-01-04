#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

#define ADDR_IOA             0x80
#define ADDR_SP              0x81
#define ADDR_DPL0            0x82
#define ADDR_DPH0            0x83
#define ADDR_DPL1            0x84
#define ADDR_DPH1            0x85
#define ADDR_DPS             0x86
#define ADDR_PCON            0x87
#define ADDR_TCON            0x88
#define ADDR_TMOD            0x89
#define ADDR_TL0             0x8a
#define ADDR_TL1             0x8b
#define ADDR_TH0             0x8c
#define ADDR_TH1             0x8d
#define ADDR_CKCON           0x8e
#define ADDR_IOB             0x90
#define ADDR_EXIF            0x91
#define ADDR_MPAGE           0x92
#define ADDR_SCON0           0x98
#define ADDR_SBUF0           0x99
#define ADDR_AUTOPTRH1       0x9a
#define ADDR_AUOTPTRL1       0x9b
#define ADDR_AUTOPTRH2       0x9d
#define ADDR_AUOTPTRL2       0x9e
#define ADDR_IOC             0xa0
#define ADDR_INT2CLR         0xa1
#define ADDR_INT4CLR         0xa2
#define ADDR_IE              0xa8
#define ADDR_EP2468STAT      0xaa
#define ADDR_EP24FIFOFLGS    0xab
#define ADDR_EP68FIFOFLGS    0xac
#define ADDR_AUTOPTR_SETUP   0xaf
#define ADDR_IOD             0xb0
#define ADDR_IOE             0xb1
#define ADDR_OEA             0xb2
#define ADDR_OEB             0xb3
#define ADDR_OEC             0xb4
#define ADDR_OED             0xb5
#define ADDR_OEE             0xb6
#define ADDR_IP              0xb8
#define ADDR_EP01STAT        0xba
#define ADDR_GPIFTRIG        0xbb
#define ADDR_GPIFSGL_DATH    0xbd
#define ADDR_GPIFSGL_DATLX   0xbe
#define ADDR_GPIFSGL_DATLNOX 0xbf
#define ADDR_SCON1           0xc0
#define ADDR_SBUF1           0xc1
#define ADDR_T2CON           0xc8
#define ADDR_RCAP2L          0xca
#define ADDR_RCAP2H          0xcb
#define ADDR_TL2             0xcc
#define ADDR_TH2             0xcd
#define ADDR_PSW             0xd0
#define ADDR_EICON           0xd8
#define ADDR_ACC             0xe0
#define ADDR_EIE             0xe8
#define ADDR_B               0xf0
#define ADDR_EIP             0xf8

/* IRQs */

#define IRQ_IE0     0   /* External Interrupt 0 */
#define IRQ_TF0     1   /* Timer 0 overflow */
#define IRQ_IE1     2   /* External Interrupt 1 */
#define IRQ_TF1     3   /* Timer 1 overflow */
#define IRQ_RI_TI_0 4   /* Serial port 0 interrupt */
#define IRQ_TF2     5   /* Timer 2 event */
#define IRQ_RESUME  6   /* WAKEUP / WU2 Pin or USB Resume */
#define IRQ_RI_TI_1 7   /* Serial port 1 interrupt */
#define IRQ_USB     8   /* USB */
#define IRQ_I2C     9   /* I2C */
#define IRQ_IE4     10  /* GPIF / FIFOs / INT4 Pin */
#define IRQ_IE5     11  /* INT5# Pin */
#define IRQ_IE6     12  /* INT6 Pin */

#define DEVICE_INTERRUPT_VECTOR_COUNT 13
#define DEVICE_DEFAULT_SYSCLK_HZ      12000000ul

#endif
