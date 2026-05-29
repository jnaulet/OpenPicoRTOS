	/* This file contains unprivileged accessible asm functions */
	.syntax unified
	.arch armv7e-m

	.text
	
	.thumb_func
	.type arch_syscall, %function
	.global arch_syscall
arch_syscall:
	push {lr}
	svc #0
	pop {pc}
	
	.thumb_func
	.type arch_compare_and_swap, %function
	.global arch_compare_and_swap
arch_compare_and_swap:
	dmb ish
	ldrex r3, [r0]
	cmp r3, r1
	bne .arch_compare_and_swap_fail_cmp
	strex r3, r2, [r0]
	cmp r3, #0
	bne .arch_compare_and_swap_fail_strex
	mov r0, r1	/* success */
	dmb ish
	bx lr
.arch_compare_and_swap_fail_cmp:
	clrex
.arch_compare_and_swap_fail_strex:
	mov r0, r2
	dmb ish
	bx lr
