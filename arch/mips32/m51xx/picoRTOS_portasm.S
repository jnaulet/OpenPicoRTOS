	/* Declarations */
	.set noat
	.set noreorder

	.extern picoRTOS_tick
	.extern picoRTOS_syscall

	#include "picoRTOSConfig.h"
	#include "picoRTOS_device.h"
	/* MIPS specifics */
	#include "../../lib/mips/regs.h"

	/* counter increments every other clock */
	.equ TIMER_CORE_PERIOD, (CONFIG_SYSCLK_HZ / CONFIG_TICK_HZ) / 2
	/* interrupt control */
	.equ INTC_INTSTAT, ADDR_INTC + 0x20
	.equ INTC_IFS0, ADDR_INTC + 0x40

	.macro SWAP_STACK
	mfc0 at, CP0_KSCRATCH0
	mtc0 sp, CP0_KSCRATCH0
	or sp, at, zero
	.endm

	.macro SAVE_CONTEXT_FPU
	/* align on 64bits,
	 * the tedious way */
	addiu sp, sp, -4
	ext at, sp, 2, 1
	beq at, zero, 1f
	sw zero, 0 (sp)
	addiu sp, sp, -4
	sw at, 0 (sp)
1:
	/* fpu registers */
	addiu sp, sp, -256
	sdc1 $f0, 0 (sp)
	sdc1 $f1, 8 (sp)
	sdc1 $f2, 16 (sp)
	sdc1 $f3, 24 (sp)
	sdc1 $f4, 32 (sp)
	sdc1 $f5, 40 (sp)
	sdc1 $f6, 48 (sp)
	sdc1 $f7, 56 (sp)
	sdc1 $f8, 64 (sp)
	sdc1 $f9, 72 (sp)
	sdc1 $f10, 80 (sp)
	sdc1 $f11, 88 (sp)
	sdc1 $f12, 96 (sp)
	sdc1 $f13, 104 (sp)
	sdc1 $f14, 112 (sp)
	sdc1 $f15, 120 (sp)
	sdc1 $f16, 128 (sp)
	sdc1 $f17, 136 (sp)
	sdc1 $f18, 144 (sp)
	sdc1 $f19, 152 (sp)
	sdc1 $f20, 160 (sp)
	sdc1 $f21, 168 (sp)
	sdc1 $f22, 176 (sp)
	sdc1 $f23, 184 (sp)
	sdc1 $f24, 192 (sp)
	sdc1 $f25, 200 (sp)
	sdc1 $f26, 208 (sp)
	sdc1 $f27, 216 (sp)
	sdc1 $f28, 224 (sp)
	sdc1 $f29, 232 (sp)
	sdc1 $f30, 240 (sp)
	sdc1 $f31, 248 (sp)
	.endm

	.macro SAVE_CONTEXT_DSP
	addiu sp, sp, -24
	mflo at, $ac1
	sw at, 0 (sp)
	mfhi at, $ac1
	sw at, 4 (sp)
	mflo at, $ac2
	sw at, 8 (sp)
	mfhi at, $ac2
	sw at, 12 (sp)
	mflo at, $ac3
	sw at, 16 (sp)
	mfhi at, $ac3
	sw at, 20 (sp)
	.endm

	.macro SAVE_CONTEXT
	/* coprocessors */
#ifdef MIPS32_M51XX_HAS_FPU64
	SAVE_CONTEXT_FPU
#endif
#ifdef MIPS32_M51XX_HAS_DSP
	SAVE_CONTEXT_DSP
#endif
	/* cpu */
	addiu sp, sp, -116
	sw v0, 0 (sp)
	sw v1, 4 (sp)
	sw a0, 8 (sp)
	sw a1, 12 (sp)
	sw a2, 16 (sp)
	sw a3, 20 (sp)
	sw t0, 24 (sp)
	sw t1, 28 (sp)
	sw t2, 32 (sp)
	sw t3, 36 (sp)
	sw t4, 40 (sp)
	sw t5, 44 (sp)
	sw t6, 48 (sp)
	sw t7, 52 (sp)
	sw s0, 56 (sp)
	sw s1, 60 (sp)
	sw s2, 64 (sp)
	sw s3, 68 (sp)
	sw s4, 72 (sp)
	sw s5, 76 (sp)
	sw s7, 80 (sp)
	sw s7, 84 (sp)
	sw t8, 88 (sp)
	sw t9, 92 (sp)
	sw fp, 96 (sp)
	sw ra, 100 (sp)
	/* special registers */
	mflo at, $ac0
	sw at, 104 (sp)
	mfhi at, $ac0
	sw at, 108 (sp)
	mfc0 at, CP0_EPC
	sw at, 112 (sp)
	SWAP_STACK
	.endm

	.macro RESTORE_CONTEXT_FPU
	ldc1 $f0, 0 (sp)
	ldc1 $f1, 8 (sp)
	ldc1 $f2, 16 (sp)
	ldc1 $f3, 24 (sp)
	ldc1 $f4, 32 (sp)
	ldc1 $f5, 40 (sp)
	ldc1 $f6, 48 (sp)
	ldc1 $f7, 56 (sp)
	ldc1 $f8, 64 (sp)
	ldc1 $f9, 72 (sp)
	ldc1 $f10, 80 (sp)
	ldc1 $f11, 88 (sp)
	ldc1 $f12, 96 (sp)
	ldc1 $f13, 104 (sp)
	ldc1 $f14, 112 (sp)
	ldc1 $f15, 120 (sp)
	ldc1 $f16, 128 (sp)
	ldc1 $f17, 136 (sp)
	ldc1 $f18, 144 (sp)
	ldc1 $f19, 152 (sp)
	ldc1 $f20, 160 (sp)
	ldc1 $f21, 168 (sp)
	ldc1 $f22, 176 (sp)
	ldc1 $f23, 184 (sp)
	ldc1 $f24, 192 (sp)
	ldc1 $f25, 200 (sp)
	ldc1 $f26, 208 (sp)
	ldc1 $f27, 216 (sp)
	ldc1 $f28, 224 (sp)
	ldc1 $f29, 232 (sp)
	ldc1 $f30, 240 (sp)
	ldc1 $f31, 248 (sp)
	addiu sp, sp, 256
	/* realign if needed */
	lw at, 0 (sp)
	beq at, zero, 1f
	addiu sp, sp, 4
	addiu sp, sp, 4
1:
	.endm

	.macro RESTORE_CONTEXT_DSP
	lw at, 0 (sp)
	mtlo at, $ac1
	lw at, 4 (sp)
	mthi at, $ac1
	lw at, 8 (sp)
	mtlo at, $ac2
	lw at, 12 (sp)
	mthi at, $ac2
	lw at, 16 (sp)
	mtlo at, $ac3
	lw at, 20 (sp)
	mthi at, $ac3
	addiu sp, sp, 24
	.endm

	.macro RESTORE_CONTEXT
	SWAP_STACK
	/* cpu */
	lw v0, 0 (sp)
	lw v1, 4 (sp)
	lw a0, 8 (sp)
	lw a1, 12 (sp)
	lw a2, 16 (sp)
	lw a3, 20 (sp)
	lw t0, 24 (sp)
	lw t1, 28 (sp)
	lw t2, 32 (sp)
	lw t3, 36 (sp)
	lw t4, 40 (sp)
	lw t5, 44 (sp)
	lw t6, 48 (sp)
	lw t7, 52 (sp)
	lw s0, 56 (sp)
	lw s1, 60 (sp)
	lw s2, 64 (sp)
	lw s3, 68 (sp)
	lw s4, 72 (sp)
	lw s5, 76 (sp)
	lw s7, 80 (sp)
	lw s7, 84 (sp)
	lw t8, 88 (sp)
	lw t9, 92 (sp)
	lw fp, 96 (sp)
	lw ra, 100 (sp)
	/* special registers */
	lw at, 104 (sp)
	mtlo at, $ac0
	lw at, 108 (sp)
	mthi at, $ac0
	lw at, 112 (sp)
	mtc0 at, CP0_EPC
	addiu sp, sp, 116
	/* coprocessors */
#ifdef MIPS32_M51XX_HAS_DSP
	RESTORE_CONTEXT_DSP
#endif
#ifdef MIPS32_M51XX_HAS_FPU64
	RESTORE_CONTEXT_FPU
#endif
	ehb
	.endm

	.text

	.align 2
	.type arch_CORE_TIMER, %function
	.global arch_CORE_TIMER
arch_CORE_TIMER:
	SAVE_CONTEXT
	/* tick */
	mfc0 a0, CP0_KSCRATCH0
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
	mtc0 v0, CP0_KSCRATCH0
	RESTORE_CONTEXT
	eret

	/* Should be syscall (EBASE + 0x180) but due to complications related to the
	 * microchip BSP, this is Core Software Interrupt 0 */
	.align 2
	.type arch_SYSCALL, %function
	.global arch_SYSCALL
arch_SYSCALL:
	SAVE_CONTEXT
	mfc0 a0, CP0_KSCRATCH0	/* get task sp */
	lw a1, 8(a0)		/* get syscall from task stack */
	lw a2, 12(a0)		/* get priv from task stack */

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
	mtc0 v0, CP0_KSCRATCH0
	RESTORE_CONTEXT
	eret

	.align 2
	.type arch_save_first_context, %function
	.global arch_save_first_context
arch_save_first_context:
	/* switch to task stack */
	mtc0 a0, CP0_KSCRATCH0
	SWAP_STACK
	/* populate registers */
	or a0, a2, zero
	mtc0 a1, CP0_EPC
	and a2, a2, zero
	SAVE_CONTEXT
	jr ra
	mfc0 v0, CP0_KSCRATCH0

	.align 2
	.type arch_start_first_task, %function
	.global arch_start_first_task
arch_start_first_task:
	/* set first tick to now + period */
	mfc0 t0, CP0_COUNT
	la t1, TIMER_CORE_PERIOD
	addu t0, t0, t1
	mtc0 t0, CP0_COMPARE
	ehb

	/* restore first context */
	mtc0 a0, CP0_KSCRATCH0
	RESTORE_CONTEXT
	/* emulate return from exception */
	mfc0 ra, CP0_EPC
	jr ra
	ei

	.align 2
	.type arch_syscall, %function
	.global arch_syscall
arch_syscall:
	mfc0 at, CP0_CAUSE
	ori at, at, 0x100 /* IP0 */
	mtc0 at, CP0_CAUSE
	wait	/* workaround: interrupt might not happen immediately */
	jr ra
	nop

	.align 2
	.type arch_compare_and_swap, %function
	.global arch_compare_and_swap
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
	.global arch_EIC_handler
arch_EIC_handler:
	SAVE_CONTEXT
	/* extract irq parameters */
	la fp, INTC_INTSTAT
	lw k0, (fp)
	andi k0, k0, 0xff   /* k0: irq */
	sll t0, k0, 3	    /* t0: ISR_TABLE index */
	andi t1, k0, 0x1f   /* t1: bit mask */
	addiu at, zero, 1
	sllv t1, at, t1
	sra t2, k0, 5	    /* t2: IFSx offset */
	sll t2, t2, 4

	/* find isr table entry */
	la fp, ISR_TABLE
	addu fp, fp, t0
	lw k0, 0(fp)	/* fn to k0 */
	lw a0, 4(fp)	/* priv to a0 */

	/* finally branch to isr */
	jr k0
	nop

	/* ack IFSx */
	la fp, INTC_IFS0
	addu fp, fp, t2
	lw k0, (fp)
	xor k0, k0, t1	/* flip IFSx bit */
	sw k0, (fp)

	/* epilogue */
	RESTORE_CONTEXT
	eret

	.align 2
	.type arch_counter, %function
	.global arch_counter
arch_counter:
	/* FIXME: not very accurate */
	mfc0 v0, CP0_COUNT
	mfc0 v1, CP0_COMPARE
	subu v0, v0, v1
	jr ra
	sll v0, v0, 1

	.bss

	.align 2
	.type ISR_TABLE, %object
	.global ISR_TABLE
ISR_TABLE:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.long 0
	.long 0
	.endr
