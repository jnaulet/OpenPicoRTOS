	.file "picoRTOS_portasm_u.S"
	.section .text

	.align 4
	.type arch_syscall, %function
	.global arch_syscall
arch_syscall:
	se_sc
	se_blr

	.align 4
	.type arch_compare_and_swap, %function
	.global arch_compare_and_swap
arch_compare_and_swap:
	mbar
	lharx r7, 0, r3
	se_cmp r7, r4
	se_bne .arch_compare_and_swap_fail_cmp
	sthcx. r5, 0, r3
	se_bne .arch_compare_and_swap_fail_sthcx
	mbar
	se_mr r3, r4		/* success */
	se_blr
.arch_compare_and_swap_fail_cmp:
	sthcx. r7, 0, r3	/* turn reservation bit off */
	mbar
.arch_compare_and_swap_fail_sthcx:
	se_mr r3, r5
	se_blr
