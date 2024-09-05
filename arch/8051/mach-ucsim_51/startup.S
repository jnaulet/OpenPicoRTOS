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
	VECTOR #0, Default_Handler		; INT0# Pin
	VECTOR #1, Default_Handler		; Timer 0 Overflow
	VECTOR #2, Default_Handler		; INT1# Pin
	VECTOR #3, Default_Handler		; Timer 1 Overflow
	VECTOR #4, Default_Handler		; USART0 Rx & Tx
	VECTOR #5, SysTick_Handler		; Timer 2 Overflow

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
