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
