	.syntax unified
	.arch armv7e-m

	.extern picoRTOS_tick
	.extern picoRTOS_syscall

	#include <generated/autoconf.h>

	.macro SAVE_CONTEXT rS
	mrs \rS, psp		/* use rS as psp stack */
	stmdb \rS!, {r4-r11}	/* save registers r4-r11 */
#ifdef CONFIG_FPU
	vstmdb \rS!, {s16-s31}	/* save fpu high registers */
#endif
	.endm

	.macro RESTORE_CONTEXT rS
#ifdef CONFIG_FPU
	vldmia \rS!, {s16-s31}	/* restore fpu high registers */
#endif
	ldmia \rS!, {r4-r11}	/* restore registers r4-11 */
	msr psp, \rS		/* reclaim stack space */
	.endm

	.text

	.thumb_func
	.type arch_save_first_context, %function
	.global arch_save_first_context
arch_save_first_context:
	push {r4-r11, lr}
	/* auto-save */
#ifdef CONFIG_FPU
	subs r0, r0, #8	/* blank + fpscr */
	vstmdb r0!, {s0-s15}	/* fpu low registers */
#endif
	mov r4, r2		/* r0 (priv) */
	mov r5, #0		/* r1 */
	mov r6, #0		/* r2 */
	mov r7, #0		/* r3 */
	mov r8, r12		/* r12 */
	mov r9, lr		/* lr (r14) */
	mov r10, r1		/* return address (fn) */
	ldr r11, =0x1000000	/* xspr */
	stmdb r0!, {r4-r11}
	/* user context */
	msr psp, r0
	SAVE_CONTEXT r0
	pop {r4-r11, pc}

	.equ SYSTICK_CSR, 0xe000e010

	.thumb_func
	.type arch_start_first_task, %function
	.global arch_start_first_task
arch_start_first_task:
	RESTORE_CONTEXT r0
	/* SPSEL, bit[1] Use PSP as current stack */
	/* FPCA, bit[2]  FP extension is active */
	mov r0, #6
	msr CONTROL, r0
	isb
	/* general purpose */
	pop {r0-r3, r12, lr}	/* pop auto-save */
	pop {r4-r5}		/* return addr and xspr */
	/* fpu */
#ifdef CONFIG_FPU
	mrs r0, psp
	vldmia r0!, {s0-s15}	/* pop auto-save */
	add r0, r0, #8		/* fpscr + blank */
	msr psp, r0
#endif
	/* interrupts */
	cpsie if

	/* nPRIV, bit[0] Thread mode has unprivileged access */
	mov r0, #7
	msr CONTROL, r0

	/* to idle */
	bx r4

	.thumb_func
	.global SysTick_Handler
SysTick_Handler:
	push {lr}	/* store lr for interrupt return */
	SAVE_CONTEXT r0
	bl picoRTOS_tick
	RESTORE_CONTEXT r0
	pop {pc}	/* return */

	.thumb_func
	.global SVC_Handler
SVC_Handler:
	push {lr}
	SAVE_CONTEXT r0
	ldr r1, [r0, #96]	/* get syscall from psp (auto-stacked r0) */
	ldr r2, [r0, #100]	/* get priv from psp (auto-stacked r1) */
	bl picoRTOS_syscall
	RESTORE_CONTEXT r0
	pop {pc}

	.equ SCB_CFSR,  0xe000ed28
	.equ MPU_MMFAR, 0xe000ed34
	.equ SCB_BFAR,  0xe000ed38

	.thumb_func
BusFault_Handler:
	push {lr}
	SAVE_CONTEXT r0
	ldr r1, =SCB_CFSR	/* clear fault (FIXME) */
	mov r2, #(0xffu << 8)
	str r2, [r1]
	mov r1, #8		/* SEGFAULT (FIXME) */
	ldr r2, =SCB_BFAR	/* provide fault addr reg */
	bl picoRTOS_syscall
	RESTORE_CONTEXT r0
	pop {pc}

	.thumb_func
MemManage_Handler:
	push {lr}
	SAVE_CONTEXT r0
	ldr r1, =SCB_CFSR	/* clear fault (FIXME) */
	mov r2, #(0xffu << 0)
	str r2, [r1]
	mov r1, #8		/* SEGFAULT (FIXME) */
	ldr r2, =MPU_MMFAR	/* provide fault addr reg */
	bl picoRTOS_syscall
	RESTORE_CONTEXT r0
	pop {pc}

	.thumb_func
Default_Handler:
	push {lr}
	SAVE_CONTEXT r0
	push {r0}
	/* get active exception in r0 */
	mrs r0, IPSR
	subs r0, #16
	ldr r3, =ISR_TABLE
	lsls r0, r0, #3	/* x8 to get the right entry */
	add r3, r3, r0
	ldr r1, [r3]
	ldr r0, [r3, #4]
	/* check if handled */
	cmp r1, #0
	beq isr_fn_is_null
	/* call fn(priv) */
	blx r1
	pop {r0}
	RESTORE_CONTEXT r0
	pop {pc}

isr_fn_is_null:
	/* isr call is null, use picoRTOS_register_interrupt() */
	bkpt #0
	b isr_fn_is_null

	.bss

	#include "picoRTOS_device.h"

	.align 2
	.type ISR_TABLE, %object
	.global ISR_TABLE
ISR_TABLE:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.long 0
	.long 0
	.endr

	.section .vectors, "ax"

	.word __StackTop
	.word Reset_Handler
	.word NMI_Handler
	.word HardFault_Handler
	.word MemManage_Handler
	.word BusFault_Handler
	.word UsageFault_Handler
	.word 0 /* reserved */
	.word 0 /* reserved */
	.word 0 /* reserved */
	.word 0 /* reserved */
	.word SVC_Handler
	.word DebugMon_Handler
	.word 0 /* reserved */
	.word PendSV_Handler
	.word SysTick_Handler
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.word Default_Handler
	.endr

	#include "../include/macros.S"

	/* Weak handlers */
	def_irq_handler NMI_Handler
	def_irq_handler HardFault_Handler
	def_irq_handler UsageFault_Handler
	def_irq_handler DebugMon_Handler
	def_irq_handler PendSV_Handler
