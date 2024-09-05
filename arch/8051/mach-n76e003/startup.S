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
	.include "generated/autoconf.S"

	.area HOME (CODE)

	.globl Reset_Handler
Reset_Handler:
	ljmp __non_interruptible_startup

	;; vectors
	VECTOR #0, Default_Handler		; External Interrupt 0
	VECTOR #1, Default_Handler		; Timer 0 overflow
	VECTOR #2, Default_Handler		; External Interrupt 1
	VECTOR #3, Default_Handler		; Timer 1 overflow
	VECTOR #4, Default_Handler		; Serial port 0 interrupt
	.ifdef CONFIG_N76E003_TIMER_T2
	VECTOR #5, SysTick_Handler		; Timer 2 event
	.else
	VECTOR #5, Default_Handler		; Timer 2 event
	.endif
	VECTOR #6, Default_Handler		; I2C status/timer-out-interrupt
	VECTOR #7, Default_Handler		; Pin interrupt
	VECTOR #8, Default_Handler		; Brown-out detection interrupt
	VECTOR #9, Default_Handler		; SPI interrupt
	VECTOR #10, Default_Handler		; WDT interrupt
	VECTOR #11, Default_Handler		; ADC interrupt
	VECTOR #12, Default_Handler		; Input capture interrupt
	VECTOR #13, Default_Handler		; PWM interrupt
	VECTOR #14, Default_Handler		; Fault Brake interrupt
	VECTOR #15, Default_Handler		; Serial port 1 interrupt
	VECTOR #16, Default_Handler		; Timer 3 overflow
	.ifdef CONFIG_N76E003_TIMER_WKT
	VECTOR #17, SysTick_Handler		; Self Wake-up Timer interrupt
	.else
	VECTOR #17, Default_Handler		; Self Wake-up Timer interrupt
	.endif

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
