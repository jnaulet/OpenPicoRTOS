	.module picoRTOS_portasm
	.optsdcc -mstm8

	.include "generated/picoRTOS_device.S"

	.globl _picoRTOS_tick
	.globl _picoRTOS_syscall
	.globl _picoRTOS_irq

	.macro SWAP_STACK
	ldw x, sp
	ldw y, _bsp
	ldw sp, y
	ldw _bsp, x
	.endm

	.macro SAVE_CONTEXT
	;; auto-stack frame:
	;; PCL
	;; PCH
	;; PCE
	;; YL:YH
	;; XL:XH
	;; A
	;; CC
	SWAP_STACK
	rcf ; ensure no carry is propagated
	.endm

	.macro RESTORE_CONTEXT
	SWAP_STACK
	.endm

	.area CODE

	.globl Trap_Handler
Trap_Handler:
	SAVE_CONTEXT
	;; x is already set (task sp)
	;; a is already set (syscall)
	;; get priv from sp
	ldw y, x
	addw y, #12
	pushw y
	;; syscall
	call _picoRTOS_syscall
	ldw _bsp, x
	RESTORE_CONTEXT
	iret

	.globl Awu_Handler
Awu_Handler:
	SAVE_CONTEXT
	;; ack AWU
	ldw y, ADDR_AWU
	ld a, (y)
	;;
	call _picoRTOS_tick
	ldw _bsp, x
	RESTORE_CONTEXT
	iret

	.globl Default_Handler
Default_Handler:
	SAVE_CONTEXT
	;; x is already set (task sp)
	;; a is already set (irq)
	call _picoRTOS_irq
	RESTORE_CONTEXT
	iret

	.globl _arch_save_first_context
_arch_save_first_context:
	subw x, #9
	;; cc:a
	clr (1, x)
	clr (2, x)
	;; xh: >priv
	ld a, (5, sp)
	ld (3, x), a
	;; xl: <priv
	ld a, (6, sp)
	ld (4, x), a
	;; yh:yl
	clr (5, x)
	clr (6, x)
	;; pce
	clr (7, x)
	;; pch: >fn
	ld a, (3, sp)
	ld (8, x), a
	;; pcl: <fn
	ld a, (4, sp)
	ld (9, x), a
	ret

	.globl _arch_start_first_task
_arch_start_first_task:
	;; prepare stack sp
	ldw _bsp, x
	RESTORE_CONTEXT
	iret

	.globl _arch_syscall
_arch_syscall:
	trap
	ret

	.area DATA
	;; leave room for ___SDCC_hc08_retn

	.globl _bsp
_bsp:	.ds 2

	.area INITIALIZED
	;; sdcc 4.0.0 requires this
	.area SSEG
