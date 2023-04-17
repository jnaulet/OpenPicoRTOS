	/* Generic ASM macros for all PPC E200s
	 *
	 * Provides:
	 *
	 * COPY_TO_RAM
	 * INIT_STACK
	 * ZERO_RAM
	 * SET_MMU_TLB
	 * def_irq_handler
	 */

	.macro E_LIW rS, value
	e_lis \rS, \value@ha
	e_or2i \rS, \value@l
	.endm

	/* COPY_TO_RAM */
	.macro COPY_TO_RAM base, start, end
	E_LIW r3, \base
	E_LIW r4, \start
	E_LIW r5, \end
	e_b 2f
1:
	e_lwz r0, 0 (r3)
	e_stw r0, 0 (r4)
	e_addi r3, r3, 4
	e_addi r4, r4, 4
2:
	se_cmp r4, r5
	se_bne 1b
	.endm

	/* ZERO_RAM */
	.macro ZERO_RAM start, end
	e_li r0, 0
	E_LIW r3, \start
	E_LIW r4, \end
	e_b 2f
1:
	e_stw r0, 0 (r3)
	e_addi r3, r3, 4
2:
	se_cmp r3, r4
	se_bne 1b
	.endm

	/* SET_MMU_TLB */
	.macro SET_MMU_TLB mas0, mas1, mas2, mas3
	E_LIW r3, \mas0
	mtmas0 r3
	E_LIW r3, \mas1
	mtmas1 r3
	E_LIW r3, \mas2
	mtmas2 r3
	E_LIW r3, \mas3
	mtmas3 r3
	tlbwe
	.endm

	/* ENABLE_BTB */
	.macro ENABLE_BTB
	e_li r3, 0x201
	mtbucsr r3
	se_isync
	.endm

	/* INVALIDATE_AND_ENABLE_CACHE */
	.equ DCACHE, 1010
	.equ ICACHE, 1011

	.macro INVALIDATE_AND_ENABLE_CACHE cache
	e_li r3, (1 << 1)	/* ICINV */
	e_li r4, (1 << 2)	/* ICABT */
	E_LIW r5, ~(1 << 2)	/* ~ICABT */
1:
	/* invalidate */
	mtspr \cache, r3
2:
	/* check abort */
	mfspr r6, \cache
	and. r7, r4, r6
	e_beq 3f
	/* clear ICABT */
	and. r6, r5, r6
	mtspr \cache, r6
	e_b 1b
3:
	/* check ICINV done */
	and. r7, r3, r6
	e_bne 2b
	/* enable */
	e_or2i r6, 0x1
	se_isync
	msync
	mtspr \cache, r6
	.endm

	/* INIT_IVOR */
	.macro INIT_IVOR spr, addr
	E_LIW r0, \addr
	/* real vector addr is IVPR(0-15) | IVORx(16-27) | 4b0000 */
	e_rlwinm r0, r0, 0, 16, 27
	mtspr \spr, r0
	.endm

	/* CLEAR_RESERVATIONS_ON_EXT_INTERRUPT */
	.macro CLEAR_RESERVATIONS_ON_EXT_INTERRUPT
	e_lis r0, 0x2
	mthid0 r0
	.endm

	/* ENABLE_MSR_ME */
	.macro ENABLE_MSR_ME
	mfmsr r0
	e_or2i r0, (1 << 12)	/* ME */
	mtmsr r0
	.endm

	/* TURN_WATCHDOG_OFF */
	.macro TURN_WATCHDOG_OFF
	e_li r0, 0
	mttcr r0
	.endm

	/* def_irq_handler */
	.macro def_irq_handler handler_name
	.weak \handler_name
\handler_name:
	se_illegal
	se_nop
	b \handler_name
	.endm
