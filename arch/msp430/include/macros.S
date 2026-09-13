	/* Generic ASM macros for all MSP430s
	 *
	 * Provides:
	 *
	 * COPY_TO_RAM
	 * ZERO_RAM
	 */

	/* COPY_TO_RAM */
	.macro COPY_TO_RAM base, start, end
	mov #\base, R4
	mov #\start, R5
	mov #\end, R6
	jmp 2f
1:
	mov @R4+, 0(R5)
	incd R5
2:
	cmp R5, R6
	jnz 1b
	.endm

	/* ZERO_RAM */
	.macro ZERO_RAM start, end
	mov #\start, R4
	mov #\end, R5
	jmp 2f
1:
	mov #0, 0(R4)
	incd R4
2:
	cmp R4, R5
	jnz 1b
	.endm
