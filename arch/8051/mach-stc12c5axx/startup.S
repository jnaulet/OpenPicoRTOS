	.module startup
	
	.globl Default_Handler
	.globl SysTick_Handler
	
	.globl _vn
	.globl __start__stack

	.globl _main_no_sdcc_init
	
	.globl s_XSEG
	.globl l_XSEG
	.globl s_XINIT
	.globl s_XISEG
	.globl l_XISEG
	
	.include "macros.S"

	.area HOME (CODE)

	.globl Reset_Handler
Reset_Handler:
	ljmp __non_interruptible_startup

	;; vectors
	VECTOR #0, Default_Handler ; External Interrupt 0
	VECTOR #1, SysTick_Handler ; Timer 0
	VECTOR #2, Default_Handler ; External Interrupt 1
	VECTOR #3, Default_Handler ; Timer 1
	VECTOR #4, Default_Handler ; Serial port
	VECTOR #5, Default_Handler ; ADC
	VECTOR #6, Default_Handler ; LVD
	VECTOR #7, Default_Handler ; PCA
	VECTOR #8, Default_Handler ; UART2(S2)
	VECTOR #9, Default_Handler ; SPI

	.globl __non_interruptible_startup
__non_interruptible_startup:
	clr ea
	mov sp, #__start__stack - 1
	ZERO_RAM s_XSEG, l_XSEG
	COPY_TO_RAM s_XINIT, s_XISEG, l_XISEG
	;; jump to main
	ljmp _main_no_sdcc_init

	.globl _exit
_exit:
	sjmp _exit
