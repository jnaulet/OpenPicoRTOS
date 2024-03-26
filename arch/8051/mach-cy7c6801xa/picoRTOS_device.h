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
#define ADDR_GPIFSGLDATH     0xbd
#define ADDR_GPIFSGLDATLX    0xbe
#define ADDR_GPIFSGLDATLNOX  0xbf
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

/* Registers & buffers */

/* GPIF Waveform Memories */
#define ADDR_WAVEDATA          0xe400
/* GENERAL CONFIGURATION */
#define ADDR_CPUCS             0xe600
#define ADDR_IFCONFIG          0xe601
#define ADDR_PINFLAGSAB        0xe602
#define ADDR_PINFLAGSCD        0xe603
#define ADDR_FIFORESET         0xe604
#define ADDR_BREAKPT           0xe605
#define ADDR_BPADDRH           0xe606
#define ADDR_BPADDRL           0xe607
#define ADDR_UART230           0xe608
#define ADDR_FIFOPINPOLAR      0xe609
#define ADDR_REVID             0xe60a
#define ADDR_REVCTL            0xe60b
#define ADDR_GPIFHOLDAMOUNT    0xe60c
/* ENDPOINT CONFIGURATION */
#define ADDR_EP1OUTCFG         0xe610
#define ADDR_EP1INCFG          0xe611
#define ADDR_EP2CFG            0xe612
#define ADDR_EP4CFG            0xe613
#define ADDR_EP6CFG            0xe614
#define ADDR_EP8CFG            0xe615
#define ADDR_EP2FIFOCFG        0xe618
#define ADDR_EP4FIFOCFG        0xe619
#define ADDR_EP6FIFOCFG        0xe61a
#define ADDR_EP8FIFOCFG        0xe61b
#define ADDR_EP2AUTOINLENH     0xe620
#define ADDR_EP2AUTOINLENL     0xe621
#define ADDR_EP4AUTOINLENH     0xe622
#define ADDR_EP4AUTOINLENL     0xe623
#define ADDR_EP6AUTOINLENH     0xe624
#define ADDR_EP6AUTOINLENL     0xe625
#define ADDR_EP8AUTOINLENH     0xe626
#define ADDR_EP8AUTOINLENL     0xe627
#define ADDR_EP2FIFOPFH        0xe630
#define ADDR_EP2FIFOPFL        0xe631
#define ADDR_EP4FIFOPFH        0xe632
#define ADDR_EP4FIFOPFL        0xe633
#define ADDR_EP6FIFOPFH        0xe634
#define ADDR_EP6FIFOPFL        0xe635
#define ADDR_EP8FIFOPFH        0xe636
#define ADDR_EP8FIFOPFL        0xe637
#define ADDR_EP2ISOINPKTS      0xe640
#define ADDR_EP4ISOINPKTS      0xe641
#define ADDR_EP6ISOINPKTS      0xe642
#define ADDR_EP8ISOINPKTS      0xe643
#define ADDR_INPKTEND          0xe648
#define ADDR_OUTPKTEND         0xe649
/* INTERRUPTS */
#define ADDR_EP2FIFOIE         0xe650
#define ADDR_EP2FIFOIRQ        0xe651
#define ADDR_EP4FIFOIE         0xe652
#define ADDR_EP4FIFOIRQ        0xe653
#define ADDR_EP6FIFOIE         0xe654
#define ADDR_EP6FIFOIRQ        0xe655
#define ADDR_EP8FIFOIE         0xe656
#define ADDR_EP8FIFOIRQ        0xe657
#define ADDR_IBNIE             0xe658
#define ADDR_IBNIRQ            0xe659
#define ADDR_NAKIE             0xe65a
#define ADDR_NAKIRQ            0xe65b
#define ADDR_USBIE             0xe65c
#define ADDR_USBIRQ            0xe65d
#define ADDR_EPIE              0xe65e
#define ADDR_EPIRQ             0xe65f
#define ADDR_GPIFIE            0xe660
#define ADDR_GPIFIRQ           0xe661
#define ADDR_USBERRIE          0xe662
#define ADDR_USBERRIRQ         0xe663
#define ADDR_ERRCNTLIM         0xe664
#define ADDR_CLRERRCNT         0xe665
#define ADDR_INT2IVEC          0xe666
#define ADDR_INT4IVEC          0xe667
#define ADDR_INTSETUP          0xe668
/* INPUT / OUTPUT */
#define ADDR_PORTACFG          0xe670
#define ADDR_PORTCCFG          0xe671
#define ADDR_PORTECFG          0xe672
#define ADDR_I2CS              0xe678
#define ADDR_I2DAT             0xe679
#define ADDR_I2CTL             0xe67a
#define ADDR_XAUTODAT1         0xe67b
#define ADDR_XAUTODAT2         0xe67c
/* UDMA CRC */
#define ADDR_UDMACRCH          0xe67d
#define ADDR_UDMACRCL          0xe67e
#define ADDR_UDMACRC_QUALIFIER 0xe67f
/* USB CONTROL */
#define ADDR_USBCS             0xe680
#define ADDR_SUSPEND           0xe681
#define ADDR_WAKEUPCS          0xe682
#define ADDR_TOGCTL            0xe683
#define ADDR_USBFRAMEH         0xe684
#define ADDR_USBFRAMEL         0xe685
#define ADDR_MICROFRAME        0xe686
#define ADDR_FNADDR            0xe687
/* ENDPOINTS */
#define ADDR_EP0BCH            0xe68a
#define ADDR_EP0BCL            0xe68b
#define ADDR_EP1OUTBC          0xe68d
#define ADDR_EP1INBC           0xe68f
#define ADDR_EP2BCH            0xe690
#define ADDR_EP2BCL            0xe691
#define ADDR_EP4BCH            0xe694
#define ADDR_EP4BCL            0xe695
#define ADDR_EP6BCH            0xe698
#define ADDR_EP6BCL            0xe699
#define ADDR_EP8BCH            0xe69a
#define ADDR_EP8BCL            0xe69d
#define ADDR_EP0CS             0xe6a0
#define ADDR_EP1OUTCS          0xe6a1
#define ADDR_EP1INCS           0xe6a2
#define ADDR_EP2CS             0xe6a3
#define ADDR_EP4CS             0xe6a4
#define ADDR_EP6CS             0xe6a5
#define ADDR_EP8CS             0xe6a6
#define ADDR_EP2FIFOFLGS       0xe6a7
#define ADDR_EP4FIFOFLGS       0xe6a8
#define ADDR_EP6FIFOFLGS       0xe6a9
#define ADDR_EP8FIFOFLGS       0xe6aa
#define ADDR_EP2FIFOBCH        0xe6ab
#define ADDR_EP2FIFOBCL        0xe6ac
#define ADDR_EP4FIFOBCH        0xe6ad
#define ADDR_EP4FIFOBCL        0xe6ae
#define ADDR_EP6FIFOBCH        0xe6af
#define ADDR_EP6FIFOBCL        0xe6b0
#define ADDR_EP8FIFOBCH        0xe6b1
#define ADDR_EP8FIFOBCL        0xe6b2
#define ADDR_SUDPTRH           0xe6b3
#define ADDR_SUDPTRL           0xe6b4
#define ADDR_SUDPTRCTL         0xe6b5
#define ADDR_SETUPBUF          0xe6b8
/* GPIF */
#define ADDR_GPIFWFSELECT      0xe6c0
#define ADDR_GPIFIDLECS        0xe6c1
#define ADDR_GPIFIDLECTL       0xe6c2
#define ADDR_GPIFCTLCFG        0xe6c3
#define ADDR_GPIFADRH          0xe6c4
#define ADDR_GPIFADRL          0xe6c5
/* FLOWSTATE */
#define ADDR_FLOWSTATE         0xe6c6
#define ADDR_FLOWLOGIC         0xe6c7
#define ADDR_FLOWEQ0CTL        0xe6c8
#define ADDR_FLOWEQ1CTL        0xe6c9
#define ADDR_FLOWHOLDOFF       0xe6ca
#define ADDR_FLOWSTB           0xe6cb
#define ADDR_FLOWSTBEDGE       0xe6cc
#define ADDR_FLOWSTBPERIOD     0xe6cd
#define ADDR_GPIFTCB3          0xe6ce
#define ADDR_GPIFTCB2          0xe6cf
#define ADDR_GPIFTCB1          0xe6d0
#define ADDR_GPIFTCB0          0xe6d1
#define ADDR_EP2GPIFFLGSEL     0xe6d2
#define ADDR_EP2GPIFPFSTOP     0xe6d3
#define ADDR_EP2GPIFTRIG       0xe6d4
#define ADDR_EP4GPIFFLGSEL     0xe6da
#define ADDR_EP4GPIFPFSTOP     0xe6db
#define ADDR_EP4GPIFTRIG       0xe6dc
#define ADDR_EP6GPIFFLGSEL     0xe6e2
#define ADDR_EP6GPIFPFSTOP     0xe6e3
#define ADDR_EP6GPIFTRIG       0xe6e4
#define ADDR_EP8GPIFFLGSEL     0xe6ea
#define ADDR_EP8GPIFPFSTOP     0xe6eb
#define ADDR_EP8GPIFTRIG       0xe6ec
#define ADDR_XGPIFSGLDATH      0xe6f0
#define ADDR_XGPIFSGLDATLX     0xe6f1
#define ADDR_XGPIFSGLDATL_NOX  0xe6f2
#define ADDR_GPIFREADYCFG      0xe6f3
#define ADDR_GPIFREADYSTAT     0xe6f4
#define ADDR_GPIFABORT         0xe6f5
/* ENDPOINT BUFFERS */
#define ADDR_EP0BUF            0xe740
#define ADDR_EP10UTBUF         0xe780
#define ADDR_EP1INBUF          0xe7c0
#define ADDR_EP2FIFOBUF        0xf000
#define ADDR_EP4FIFOBUF        0xf400
#define ADDR_EP6FIFOBUF        0xf800
#define ADDR_EP8FIFOBUF        0xfc00

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
