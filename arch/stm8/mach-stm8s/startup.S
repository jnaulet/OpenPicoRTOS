	.module startup
	.optsdcc -mstm8

	.globl Default_Handler
	.globl Trap_Handler
	.globl Awu_Handler
	.globl _main_no_sdcc_init

	.globl s_DATA
	.globl l_DATA
	.globl s_INITIALIZER
	.globl s_INITIALIZED
	.globl l_INITIALIZER

	.include "macros.S"

	.area HOME

	;; interrupt vectors
	.globl __vectors
__vectors:
	VECTOR __stm8s_startup	; reset
	VECTOR Trap_Handler	; syscall
	VECTOR Vector0_Handler
	VECTOR Awu_Handler	; systick
	VECTOR Vector2_Handler
	VECTOR Vector3_Handler
	VECTOR Vector4_Handler
	VECTOR Vector5_Handler
	VECTOR Vector6_Handler
	VECTOR Vector7_Handler
	.ds 4	; reserved
	.ds 4	; reserved
	VECTOR Vector10_Handler
	VECTOR Vector11_Handler
	VECTOR Vector12_Handler
	VECTOR Vector13_Handler
	VECTOR Vector14_Handler
	.ds 4	; reserved
	.ds 4	; reserved
	VECTOR Vector17_Handler
	VECTOR Vector18_Handler
	VECTOR Vector19_Handler
	.ds 4	; reserved
	.ds 4	; reserved
	VECTOR Vector22_Handler
	VECTOR Vector23_Handler
	VECTOR Vector24_Handler
	.ds 20	; reserved

	.area GSINIT

	.globl __stm8s_startup
__stm8s_startup:
	INIT_STACK #0x3ff
	ZERO_RAM s_DATA, #l_DATA
	COPY_TO_RAM s_INITIALIZER, s_INITIALIZED, #l_INITIALIZER
	;; jump to main
	jp _main_no_sdcc_init

	.area GSFINAL

	.globl _exit
_exit:
	break
	jra _exit

	.area CONST
	.area INITIALIZER
	.area CODE

	;; vector handlers
	.globl __irq_handlers
__irq_handlers:
	IRQ_HANDLER Vector0_Handler, #0
	IRQ_HANDLER Vector2_Handler, #2
	IRQ_HANDLER Vector3_Handler, #3
	IRQ_HANDLER Vector4_Handler, #4
	IRQ_HANDLER Vector5_Handler, #5
	IRQ_HANDLER Vector6_Handler, #6
	IRQ_HANDLER Vector7_Handler, #7
	IRQ_HANDLER Vector10_Handler, #10
	IRQ_HANDLER Vector11_Handler, #11
	IRQ_HANDLER Vector12_Handler, #12
	IRQ_HANDLER Vector13_Handler, #13
	IRQ_HANDLER Vector14_Handler, #14
	IRQ_HANDLER Vector17_Handler, #17
	IRQ_HANDLER Vector18_Handler, #18
	IRQ_HANDLER Vector19_Handler, #19
	IRQ_HANDLER Vector22_Handler, #22
	IRQ_HANDLER Vector23_Handler, #23
	IRQ_HANDLER Vector24_Handler, #24
