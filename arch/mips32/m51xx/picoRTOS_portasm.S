	/* Declarations */
	.set noat
	.set noreorder

	//.global arch_enable_vfp
	.global arch_start_first_task
	.global arch_syscall

	.global arch_CORE_TIMER
	.global arch_SYSCALL
	.global arch_EIC_handler

	.global arch_compare_and_swap
	.global arch_counter

	.extern picoRTOS_tick
	.extern picoRTOS_syscall

	#include "picoRTOSConfig.h"
	#include "picoRTOS_device.h"

	/* xc32-gcc ABI definitions */
	.equ zero, $0
	.equ at, $1
	.equ v0, $2
	.equ v1, $3
	.equ a0, $4
	.equ a1, $5
	.equ a2, $6
	.equ a3, $7
	.equ t0, $8
	.equ t1, $9
	.equ t2, $10
	.equ t3, $11
	.equ t4, $12
	.equ t5, $13
	.equ t6, $14
	.equ t7, $15
	.equ s0, $16
	.equ s1, $17
	.equ s2, $18
	.equ s3, $19
	.equ s4, $20
	.equ s5, $21
	.equ s6, $22
	.equ s7, $23
	.equ t8, $24
	.equ t9, $25
	.equ k0, $26
	.equ k1, $27
	.equ sp, $29
	.equ fp, $30
	.equ ra, $31

	/* counter increments every other clock */
	.equ TIMER_CORE_PERIOD, (CONFIG_SYSCLK_HZ / CONFIG_TICK_HZ) / 2
	/* interrupt control */
	.equ INTC_INTSTAT, ADDR_INTC + 0x20
	.equ INTC_IFS0, ADDR_INTC + 0x40
	/* Coprocessor 0 registers */
	.equ CP0_COUNT, $9
	.equ CP0_COMPARE, $11
	.equ CP0_CAUSE, $13
	.equ CP0_EPC, $14

	.macro SWITCH_TO_MAIN_STACK rD
	la at, stack
	or \rD, sp, zero
	lw sp, (at)
	.endm

	.macro SWITCH_TO_TASK_STACK rS
	la at, stack
	sw sp, (at)
	or sp, \rS, zero
	.endm

	.macro SAVE_CONTEXT rD
	addiu sp, sp, -116
	sw v0, 0(sp)
	sw v1, 4(sp)
	sw a0, 8(sp)
	sw a1, 12(sp)
	sw a2, 16(sp)
	sw a3, 20(sp)
	sw t0, 24(sp)
	sw t1, 28(sp)
	sw t2, 32(sp)
	sw t3, 36(sp)
	sw t4, 40(sp)
	sw t5, 44(sp)
	sw t6, 48(sp)
	sw t7, 52(sp)
	sw s0, 56(sp)
	sw s1, 60(sp)
	sw s2, 64(sp)
	sw s3, 68(sp)
	sw s4, 72(sp)
	sw s5, 76(sp)
	sw s7, 80(sp)
	sw s7, 84(sp)
	sw t8, 88(sp)
	sw t9, 92(sp)
	sw fp, 96(sp)
	sw ra, 100(sp)
	/* special registers */
	mflo at
	sw at, 104(sp)
	mfhi at
	sw at, 108(sp)
	mfc0 at, CP0_EPC
	sw at, 112(sp)
	SWITCH_TO_MAIN_STACK \rD
	.endm

	.macro RESTORE_CONTEXT rD
	SWITCH_TO_TASK_STACK \rD
	lw v0, 0(sp)
	lw v1, 4(sp)
	lw a0, 8(sp)
	lw a1, 12(sp)
	lw a2, 16(sp)
	lw a3, 20(sp)
	lw t0, 24(sp)
	lw t1, 28(sp)
	lw t2, 32(sp)
	lw t3, 36(sp)
	lw t4, 40(sp)
	lw t5, 44(sp)
	lw t6, 48(sp)
	lw t7, 52(sp)
	lw s0, 56(sp)
	lw s1, 60(sp)
	lw s2, 64(sp)
	lw s3, 68(sp)
	lw s4, 72(sp)
	lw s5, 76(sp)
	lw s7, 80(sp)
	lw s7, 84(sp)
	lw t8, 88(sp)
	lw t9, 92(sp)
	lw fp, 96(sp)
	lw ra, 100(sp)
	/* special registers */
	lw at, 104(sp)
	mtlo at
	lw at, 108(sp)
	mthi at
	lw at, 112(sp)
	mtc0 at, CP0_EPC
	addiu sp, sp, 116
	.endm

	.text

	.align 2
arch_CORE_TIMER:
	/* tick */
	SAVE_CONTEXT a0
	jal picoRTOS_tick
	nop
	/* reset timer */
	mfc0 k0, CP0_COMPARE
	la k1, TIMER_CORE_PERIOD
	addu k0, k0, k1
	mtc0 k0, CP0_COMPARE
	/* ack irq */
	la fp, INTC_IFS0
	lw k0, (fp)
	ins k0, zero, 0, 1
	sw k0, (fp)
	/* epilogue */
	RESTORE_CONTEXT v0
	eret

	/* Should be syscall (EBASE + 0x180) but due to complications related to the
	 * microchip BSP, this is Core Software Interrupt 0 */
	.align 2
arch_SYSCALL:
	SAVE_CONTEXT a0
	lw a1, 8(a0)	/* get syscall from task stack */
	lw a2, 12(a0)	/* get priv from task stack */
	jal picoRTOS_syscall
	nop
	/* ack */
	mfc0 k0, CP0_CAUSE
	xori k0, k0, 0x100	/* flip IP0 */
	mtc0 k0, CP0_CAUSE
	/* ack irq */
	la fp, INTC_IFS0
	lw k0, (fp)
	ins k0, zero, 1, 1
	sw k0, (fp)
	/* epilogue */
	RESTORE_CONTEXT v0
	eret

	.align 2
	.type arch_start_first_task, %function
arch_start_first_task:
	/* set first tick to now + period */
	mfc0 t0, CP0_COUNT
	la t1, TIMER_CORE_PERIOD
	addu t0, t0, t1
	mtc0 t0, CP0_COMPARE
	ehb
	/* restore first context */
	RESTORE_CONTEXT a0
	/* emulate return from exception */
	mfc0 ra, CP0_EPC
	jr ra
	ei

	.align 2
	.type arch_syscall, %function
arch_syscall:
	mfc0 at, CP0_CAUSE
	ori at, at, 0x100 /* IP0 */
	mtc0 at, CP0_CAUSE
	wait	/* workaround: interrupt might not happen immediately */
	jr ra
	nop

	.align 2
	.type arch_compare_and_swap, %function
arch_compare_and_swap:
	ll at, (a0)
	bne at, a1, .arch_compare_and_swap_fail_cmp
	nop
	sc a2, (a0)
	beq a2, 0, .arch_compare_and_swap_fail_sc
	nop
	jr ra
	or v0, a1, zero
.arch_compare_and_swap_fail_cmp:
	sc at, (a0)	/* turn off llbit */
.arch_compare_and_swap_fail_sc:
	jr ra
	or v0, a2, zero

	.align 2
	.type arch_EIC_handler, %function
arch_EIC_handler:
	SAVE_CONTEXT k0
	/* extract irq parameters */
	la fp, INTC_INTSTAT
	lw k1, (fp)
	andi k1, k1, 0xff   /* k1: irq */
	sll t0, k1, 3	    /* t0: ISR_TABLE index */
	andi t1, k1, 0x1f   /* t1: bit mask */
	addiu at, zero, 1
	sllv t1, at, t1
	sra t2, k1, 5	    /* t2: IFSx offset */
	sll t2, t2, 4
	/* find isr table entry */
	la fp, ISR_TABLE
	addu fp, fp, t0
	lw k1, 0(fp)	/* fn to k1 */
	lw a0, 4(fp)	/* priv to a0 */
	/* finally branch to isr */
	jr k1
	nop
	/* ack IFSx */
	la fp, INTC_IFS0
	addu fp, fp, t2
	lw k1, (fp)
	xor k1, k1, t1	/* flip IFSx bit */
	sw k1, (fp)
	/* back to task */
	RESTORE_CONTEXT k0
	eret

	.align 2
	.type arch_counter, %function
arch_counter:
	mfc0 v0, CP0_COUNT
	mfc0 v1, CP0_COMPARE
	subu v0, v0, v1
	jr ra
	sll v0, v0, 1

	.bss

	.align 2
stack:
	.long 0

	.global ISR_TABLE

	.align 2
	.type ISR_TABLE, %object
ISR_TABLE:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.long 0
	.long 0
	.endr
