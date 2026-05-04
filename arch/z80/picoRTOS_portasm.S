	.module picoRTOS_portasm
	.optsdcc -mz80 sdcccall(1)

	.include "generated/picoRTOS_device.S"

	.globl _picoRTOS_tick
	.globl _picoRTOS_syscall

	.macro SWAP_STACK
	ld iy, (#stack)
	ld hl, #0
	add hl, sp
	ld (#stack), hl
	ld sp, iy
	.endm

	.macro SAVE_CONTEXT_NOSWAP
	push af
	push bc
	push de
	push hl
	push ix
	push iy
	.endm

	.macro SAVE_CONTEXT
	SAVE_CONTEXT_NOSWAP
	SWAP_STACK
	.endm

	.macro RESTORE_CONTEXT
	SWAP_STACK
	pop iy
	pop ix
	pop hl
	pop de
	pop bc
	pop af
	.endm

	.area _CODE

	.globl SysTick_Handler
SysTick_Handler:
	SAVE_CONTEXT
	call _picoRTOS_tick
	ld (#stack), de
	RESTORE_CONTEXT
	ei
	reti
	
	.globl Default_Handler
Default_Handler:
	SAVE_CONTEXT
	;; TBD
	RESTORE_CONTEXT
	ei
	reti

	.globl _arch_save_first_context
_arch_save_first_context:
	ld (#stack), hl
	SWAP_STACK
	ld iy, (#stack)
	ld l, 2 (iy)		; (iy + 2) is priv
	ld h, 3 (iy)
	push de			; fn ret
	push de			; _arch_start_first_task ret
	ld de, #0
	push de
	push de
	push de
	push hl			; priv
	push de
	push de
	SWAP_STACK		; hl is #stack
	ex de, hl
	pop hl
	pop af
	jp (hl)

	.globl _arch_start_first_task
_arch_start_first_task:
	ld (#stack), hl
	RESTORE_CONTEXT
	ei			; enable tick
	ret

	.globl _arch_syscall
_arch_syscall:
	di			; simulate interrupt entrance
	SAVE_CONTEXT		; hl: stack
	push de			; de: priv
	push af			; a: syscall
	inc sp
	call _picoRTOS_syscall
	inc sp
	pop af
	ld (#stack), de
	RESTORE_CONTEXT
	ei
	ret

	.globl _arch_compare_and_swap
_arch_compare_and_swap:
	di
	ld a, (hl)
	ld b, e			; b: old
	xor b
	jr NZ,_arch_compare_and_swap_fail
	ld a, d			; a: new
	ld (hl), a
	ld d, #0
	jp _arch_compare_and_swap_exit
_arch_compare_and_swap_fail:
	ld d, e
	ld e, #0
_arch_compare_and_swap_exit:
	pop hl
	pop af
	ei
	jp (hl)

	.area _DATA
stack:
	.ds 2
	
	.globl _ISR_TABLE
_ISR_TABLE:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.ds 2 ; fn
	.ds 2 ; priv
	.endm
