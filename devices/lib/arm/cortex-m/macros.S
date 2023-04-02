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
	orr r1, r1, #0xf00000
	str r1, [r0]
	/* extended FPU frame stacking (no lazy frame) */
	ldr.w r0, =FPCCR
	movs r1, #0x80000000
	str r1, [r0]
	.endm

	/* def_irq_handler */
	.macro def_irq_handler handler_name
	.thumb_func
	.weak \handler_name
\handler_name:
	bkpt #0
	b \handler_name
	.endm
