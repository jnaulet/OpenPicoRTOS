	/* Specific ASM macros for all PPC E200z7s
	 *
	 * Provides:
	 *
	 * SET_MMU_TLB
	 * INIT_IVOR
	 * TURN_WATCHDOG_OFF
	 */

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

	/* INIT_IVOR */
	.macro INIT_IVOR spr, addr
	E_LIW r0, \addr
	/* real vector addr is IVPR(0-15) | IVORx(16-27) | 4b0000 */
	e_rlwinm r0, r0, 0, 16, 27
	mtspr \spr, r0
	.endm

	/* TURN_WATCHDOG_OFF */
	.macro TURN_WATCHDOG_OFF
	e_li r0, 0
	mttcr r0
	.endm

