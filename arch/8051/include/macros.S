	;; common macros for 8051s
	;;
	;; Provides:
	;;
	;; COPY_TO_RAM
	;; ZERO_RAM
	;; VECTOR
	;;

	.macro COPY_TO_RAM base, start, len, ?loop, ?next, ?end
	mov r2, #base
	mov r3, #(base >> 8)
	mov r4, #start
	mov r5, #(start >> 8)
	mov r6, #len
	mov r7, #(len >> 8)
loop:
	cjne r6, #0, next
	cjne r7, #0, next
	sjmp end
next:
	;; a = *base++
	mov dpl, r2
	mov dph, r3
        mov a, #0
	movc a, @a+dptr
	inc dptr
	mov r2, dpl
	mov r3, dph
	;; *start++ = a
	mov dpl, r4
	mov dph, r5
	movx @dptr, a
	inc dptr
	mov r4, dpl
	mov r5, dph
	;; len--
	clr c
	mov a, r6
	subb a, #1
	mov r6, a
	mov a, r7
	subb a, #0
	mov r7, a
	;;
	sjmp loop
end:
	.endm

	.macro ZERO_RAM start, len, ?loop, ?next, ?end
	mov dptr, #start
	mov r6, #len
	mov r7, #(len >> 8)
loop:
	cjne r6, #0, next
	cjne r7, #0, next
	sjmp end
next:
	mov a, #0
	movx @dptr, a
	inc dptr
	;; len--
	clr c
	mov a, r6
	subb a, #1
	mov r6, a
	mov a, r7
	subb a, #0
	mov r7, a
	;;
	sjmp loop
end:
	.endm

	.macro VECTOR num, handler
	mov _vn, num
	ljmp handler
	.ds 2
	.endm
