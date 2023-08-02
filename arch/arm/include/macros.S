	/* Generic ASM macros for all ARM Cortex-Ms
	 *
	 * Provides:
	 *
	 * COPY_TO_RAM
	 * INIT_VTOR
	 * INIT_STACK
	 * ZERO_RAM
	 * def_irq_handler
	 */

	/* COPY_TO_RAM */
	.macro COPY_TO_RAM base, start, end
	ldr r1, =\base
	ldr r2, =\start
	ldr r3, =\end
	b 2f
1:
	ldm r1!, {r0}
	stm r2!, {r0}
2:
	cmp r2, r3
	bne 1b
	.endm

	/* INIT_VTOR */
	.macro INIT_VTOR base
	ldr r0, =\base
	ldr r1, =0xe000ed08
	str r0, [r1]
	.endm

	/* INIT_STACK */
	.macro INIT_STACK base
	ldr r0, =\base
	msr msp, r0
	.endm

	/* ZERO_RAM */
	.macro ZERO_RAM start, end
	ldr r1, =\start
	ldr r2, =\end
	movs r0, #0
	b 2f
1:
	stm r1!, {r0}
2:
	cmp r1, r2
	bne 1b
	.endm

	.equ CPACR, 0xe000ed88
	.equ FPCCR, 0xe000ef34

	.macro ENABLE_VFP
	/* enable CP10 & CP11 */
	ldr.w r0, =CPACR
	ldr r1, [r0]
	orr r1, r1, #(0xf << 20)
	str r1, [r0]
	/* extended FPU frame stacking (no lazy frame) */
	ldr.w r0, =FPCCR
	movs r1, #(0x1 << 31)
	str r1, [r0]
	/* mbar */
	dsb
	isb
	.endm

	.equ CSSELR, 0xe000ed84

	.macro SELECT_L1_DCACHE
	mov r0, #0
	ldr r11, =CSSELR
	str r0, [r11]
	dsb
	.endm

	.equ CCSIDR, 0xe000ed80

	.macro CCSIDR_NumSets rD, rS
	asrs \rD, \rS, #13
	ubfx \rD, \rD, #0, #15
	.endm

	.macro CCSIDR_Associativity rD, rS
	asrs \rD, \rS, #3
	ubfx \rD, \rD, #0, #10
	.endm

	.equ DCISW, 0xe000ef60

	.macro INVALIDATE_DCACHE
	SELECT_L1_DCACHE
	/* extract sets/ways */
	ldr r11, =CCSIDR
	ldr r0, [r11]
	CCSIDR_NumSets r1, r0
	CCSIDR_Associativity r3, r0
	/* invalidate all */
	ldr r11, =DCISW
1:
	mov r2, r3
2:
	lsl r0, r2, #25 /* ways */
	orr r0, r0, r1  /* sets */
	lsl r0, r0, #5
	str r0, [r11]
	/* ways-- */
	subs r2, r2, #1
	bge 2b
	/* sets-- */
	subs r1, r1, #1
	bge 1b
	/* mbar */
	dsb
	.endm

	.equ ICIALLU, 0xe000ef50

	/* INVALIDATE_ICACHE
	 * Borrowed from ARM TRM
	 */
	.macro INVALIDATE_ICACHE
	mov r0, #0
	ldr r11, =ICIALLU
	str r0, [r11]
	/* mbar */
	dsb
	isb
	.endm

	.equ CCR, 0xe000ed14

	/* ENABLE_I_CACHE */
	.macro ENABLE_I_CACHE
	ldr r11, =CCR
	ldr r0, [r11]
	orr r0, r0, #(0x1 << 17)
	str r0, [r11]
	/* mbar */
	dsb
	isb
	.endm

	/* ENABLE_D_CACHE */
	.macro ENABLE_D_CACHE
	ldr r11, =CCR
	ldr r0, [r11]
	orr r0, r0, #(0x1 << 16)
	str r0, [r11]
	/* mbar */
	dsb
	isb
	.endm

	/* def_irq_handler */
	.macro def_irq_handler handler_name
	.thumb_func
	.weak \handler_name
\handler_name:
	bkpt #0
	b \handler_name
	.endm
