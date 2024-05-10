	.cs08
	.module startup

	.globl Default_Handler
	.globl Vswi_Handler
	.globl _main_no_sdcc_init

	.globl s_DSEG
	.globl l_DSEG
	.globl s_XSEG
	.globl l_XSEG
	.globl s_XINIT
	.globl l_XINIT
	.globl s_XISEG

	.include "macros.S"

	.area GSINIT (CODE)
	.globl __mc9s08pt60_startup
__mc9s08pt60_startup:
	cli
	INIT_STACK #0x103f
	ZERO_RAM s_XSEG, #l_XSEG
	COPY_TO_RAM s_XINIT, s_XISEG, #l_XINIT
	;; jump to main
	jsr _main_no_sdcc_init

	.area GSFINAL (CODE)
	.globl _exit
_exit:
	bgnd
	nop
	bra _exit

	IRQ_HANDLER Vnvm_Handler, #0
	IRQ_HANDLER Vkeyboard1_Handler, #1
	IRQ_HANDLER Vkeyboard0_Handler, #2
	IRQ_HANDLER Vtsi_Handler, #3
	IRQ_HANDLER Vrtc_Handler, #4
	IRQ_HANDLER Viic_Handler, #5
	IRQ_HANDLER Vspi1_Handler, #6
	IRQ_HANDLER Vspi0_Handler, #7
	IRQ_HANDLER Vsci2tx_Handler, #8
	IRQ_HANDLER Vsci2rx_Handler, #9
	IRQ_HANDLER Vsci2err_Handler, #10
	IRQ_HANDLER Vsci1tx_Handler, #11
	IRQ_HANDLER Vsci1rx_Handler, #12
	IRQ_HANDLER Vsci1err_Handler, #13
	IRQ_HANDLER Vsci0tx_Handler, #14
	IRQ_HANDLER Vsci0rx_Handler, #15
	IRQ_HANDLER Vsci0err_Handler, #16
	IRQ_HANDLER Vadc_Handler, #17
	IRQ_HANDLER Vacmp_Handler, #18
	IRQ_HANDLER Vmtim1_Handler, #19
	IRQ_HANDLER Vmtim0_Handler, #20
	IRQ_HANDLER Vftm0ovf_Handler, #21
	IRQ_HANDLER Vftm0ch1_Handler, #22
	IRQ_HANDLER Vftm0ch0_Handler, #23
	IRQ_HANDLER Vftm1ovf_Handler, #24
	IRQ_HANDLER Vftm1ch1_Handler, #25
	IRQ_HANDLER Vftm1ch0_Handler, #26
	IRQ_HANDLER Vftm2ovf_Handler, #27
	IRQ_HANDLER Vftm2ch5_Handler, #28
	IRQ_HANDLER Vftm2ch4_Handler, #29
	IRQ_HANDLER Vftm2ch3_Handler, #30
	IRQ_HANDLER Vftm2ch2_Handler, #31
	IRQ_HANDLER Vftm2ch1_Handler, #32
	IRQ_HANDLER Vftm2ch0_Handler, #33
	IRQ_HANDLER Vftm2fault_Handler, #34
	IRQ_HANDLER Vclk_Handler, #35
	IRQ_HANDLER Vlvw_Handler, #36
	IRQ_HANDLER Vwdog_Handler, #37

	.area XINIT (CODE)
	.area CONST (CODE)
	.area CSEG (CODE)

	.area CODEIVT (ABS)
	.org 0xffb0
	.globl __vectors
__vectors:
	.dw Vnvm_Handler
	.dw Vkeyboard1_Handler
	.dw Vkeyboard0_Handler
	.dw Vtsi_Handler
	.dw Vrtc_Handler
	.dw Viic_Handler
	.dw Vspi1_Handler
	.dw Vspi0_Handler
	.dw Vsci2tx_Handler
	.dw Vsci2rx_Handler
	.dw Vsci2err_Handler
	.dw Vsci1tx_Handler
	.dw Vsci1rx_Handler
	.dw Vsci1err_Handler
	.dw Vsci0tx_Handler
	.dw Vsci0rx_Handler
	.dw Vsci0err_Handler
	.dw Vadc_Handler
	.dw Vacmp_Handler
	.dw Vmtim1_Handler
	.dw Vmtim0_Handler
	.dw Vftm0ovf_Handler
	.dw Vftm0ch1_Handler
	.dw Vftm0ch0_Handler
	.dw Vftm1ovf_Handler
	.dw Vftm1ch1_Handler
	.dw Vftm1ch0_Handler
	.dw Vftm2ovf_Handler
	.dw Vftm2ch5_Handler
	.dw Vftm2ch4_Handler
	.dw Vftm2ch3_Handler
	.dw Vftm2ch2_Handler
	.dw Vftm2ch1_Handler
	.dw Vftm2ch0_Handler
	.dw Vftm2fault_Handler
	.dw Vclk_Handler
	.dw Vlvw_Handler
	.dw Vwdog_Handler
	.dw Vswi_Handler
	.dw __mc9s08pt60_startup
