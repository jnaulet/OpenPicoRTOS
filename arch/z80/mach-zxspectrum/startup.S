	.module startup

	.globl SysTick_Handler
	.globl _main_no_sdcc_init
	
	.include "macros.S"
	.include "generated/autoconf.S"

	.globl s__DATA
	.globl l__DATA
	.globl s__INITIALIZED
	.globl s__INITIALIZER
	.globl l__INITIALIZER

	.area _DATA

	.globl __stack_top
	.ds 64
__stack_top:
	
	.area _CODE
	
	.globl Reset_Handler
Reset_Handler:
	di
	ZERO_RAM s__DATA, l__DATA
	COPY_TO_RAM s__INITIALIZER, s__INITIALIZED, l__INITIALIZER
	;; init SP
	ld hl, #__stack_top
	ld sp, hl
	;; init interrupt mode 2
	ld a, #>(__im_2)
	ld i, a
	im 2
	;; main()
	jp _main_no_sdcc_init

	.globl _exit
_exit:
	jr _exit

	.bndry 256
	.globl __im_2
__im_2:
	.rept 129
	.word #__zx_spectrum_int
	.endm

	.area _CABS (ABS)
	.org 0x8080
	.globl __zx_spectrum_int
__zx_spectrum_int:
	jp SysTick_Handler
