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
	VECTOR #4, Default_Handler		; UART0 interrupt
	VECTOR #5, SysTick_Handler		; Timer 2 interrupt
	VECTOR #6, Default_Handler		; SPI0 interrupt
	VECTOR #7, Default_Handler		; Touch key timer interrupt
	VECTOR #8, Default_Handler		; USB interrupt
	VECTOR #9, Default_Handler		; ADC interrupt
	VECTOR #10, Default_Handler		; UART1 interrupt
	VECTOR #11, Default_Handler		; PWM1/PWM2 interrupt
	VECTOR #12, Default_Handler		; GPIO interrupt
	VECTOR #13, Default_Handler		; Watchdog timer interrupt

	.globl __non_interruptible_startup
__non_interruptible_startup:
	clr ea
	mov sp, #__start__stack - 1
	ZERO_RAM s_XSEG, l_XSEG
	COPY_TO_RAM s_XINIT, s_XISEG, l_XISEG
	;; static variables init
	ljmp __gsinit

	.globl _exit
_exit:
	sjmp _exit

	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.globl __gsinit
__gsinit:
	.area GSINIT (CODE)
	.area GSFINAL (CODE)
	;; jump to main
	ljmp _main_no_sdcc_init
