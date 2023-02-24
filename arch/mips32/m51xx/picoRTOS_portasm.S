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
	.equ INTC_IFS0_CLR, ADDR_INTC + 0x44
	/* Coprocessor 0 registers */
	.equ CP0_COUNT, $9
	.equ CP0_COMPARE, $11
	.equ CP0_CAUSE, $13
	.equ CP0_EPC, $14

	.macro SWITCH_TO_MAIN_STACK rD
	la at, main_sp
	lw at, (at)
	or \rD, sp, zero
	or sp, at, zero
	.endm

	.macro SWITCH_TO_TASK_STACK rD
	la at, main_sp
	sw sp, (at)
	or sp, \rD, zero
	.endm

	.macro SAVE_CONTEXT rD
	addiu \rD, \rD, -108
	sw v0, 0(\rD)
	sw v1, 4(\rD)
	sw a0, 8(\rD)
	sw a1, 12(\rD)
	sw a2, 16(\rD)
	sw a3, 20(\rD)
	sw t0, 24(\rD)
	sw t1, 28(\rD)
	sw t2, 32(\rD)
	sw t3, 36(\rD)
	sw t4, 40(\rD)
	sw t5, 44(\rD)
	sw t6, 48(\rD)
	sw t7, 52(\rD)
	sw s0, 56(\rD)
	sw s1, 60(\rD)
	sw s2, 64(\rD)
	sw s3, 68(\rD)
	sw s4, 72(\rD)
	sw s5, 76(\rD)
	sw s7, 80(\rD)
	sw s7, 84(\rD)
	sw t8, 88(\rD)
	sw t9, 92(\rD)
	sw fp, 96(\rD)
	sw ra, 100(\rD)
	mfc0 at, CP0_EPC
	sw at, 104(\rD)
	.endm

	.macro RESTORE_CONTEXT rD
	lw v0, 0(\rD)
	lw v1, 4(\rD)
	lw a0, 8(\rD)
	lw a1, 12(\rD)
	lw a2, 16(\rD)
	lw a3, 20(\rD)
	lw t0, 24(\rD)
	lw t1, 28(\rD)
	lw t2, 32(\rD)
	lw t3, 36(\rD)
	lw t4, 40(\rD)
	lw t5, 44(\rD)
	lw t6, 48(\rD)
	lw t7, 52(\rD)
	lw s0, 56(\rD)
	lw s1, 60(\rD)
	lw s2, 64(\rD)
	lw s3, 68(\rD)
	lw s4, 72(\rD)
	lw s5, 76(\rD)
	lw s7, 80(\rD)
	lw s7, 84(\rD)
	lw t8, 88(\rD)
	lw t9, 92(\rD)
	lw fp, 96(\rD)
	lw ra, 100(\rD)
	lw at, 104(\rD)
	mtc0 at, CP0_EPC
	addiu \rD, \rD, 108
	.endm

	.text

	.align 2
arch_CORE_TIMER:
	SWITCH_TO_MAIN_STACK k0
	SAVE_CONTEXT k0
	/* tick */
	or a0, k0, zero
	jal picoRTOS_tick
	nop
	/* reset timer */
	mfc0 k0, CP0_COMPARE
	la k1, TIMER_CORE_PERIOD
	addu k0, k0, k1
	mtc0 k0, CP0_COMPARE
	/* ack irq */
	addiu k0, zero, 1
	la fp, INTC_IFS0_CLR
	sw k0, (fp)
	/* quick stack restore & swap */
	or sp, v0, zero
	RESTORE_CONTEXT sp
	eret

	/* Should be syscall (EBASE + 0x180) but due to complications related to the
	 * microchip BSP, this is Core Software Interrupt 0 */
	.align 2
arch_SYSCALL:
	SWITCH_TO_MAIN_STACK k0
	SAVE_CONTEXT k0
	or a0, k0, zero	/* get task stack from k0 */
	lw a1, 8(a0)	/* get syscall from task stack */
	lw a2, 12(a0)	/* get priv from task stack */
	jal picoRTOS_syscall
	nop
	/* ack */
	mfc0 k0, CP0_CAUSE
	xori k0, k0, 0x100	/* flip IP0 */
	mtc0 k0, CP0_CAUSE
	/* ack irq */
	addiu k0, zero, 2
	la fp, INTC_IFS0_CLR
	sw k0, (fp)
	/* quick stack restore & swap */
	or sp, v0, zero
	RESTORE_CONTEXT sp
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
	SWITCH_TO_TASK_STACK a0
	RESTORE_CONTEXT sp
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
	sc at, 0 (a0)	/* turn off llbit */
.arch_compare_and_swap_fail_sc:
	jr ra
	or v0, a2, zero

	.align 2
	.type arch_EIC_handler, %function
arch_EIC_handler:
	SWITCH_TO_MAIN_STACK k0
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
	la fp, INTC_IFS0_CLR
	addu fp, fp, t2
	sw t1, (fp)
	/* back to task */
	SWITCH_TO_TASK_STACK k0
	RESTORE_CONTEXT sp
	eret

	.bss

	.align 2
main_sp:
	.long 0

	.global ISR_TABLE

	.align 2
	.type ISR_TABLE, %object
ISR_TABLE:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.long 0
	.long 0
	.endr
