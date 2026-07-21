/*
 * SECTION definitions for all RISCV platforms
 */
SECTIONS
{
	.init :
	{
		PROVIDE(vectors = .);
		PROVIDE(_vectors = vectors);
		PROVIDE(__vectors = vectors);

		__init_start__ = .;
		__vectors_start__ = .;
                KEEP(*(.vectors))
                __vectors_end__ = .;

		. = ALIGN(4);
		KEEP(*(.init))
                KEEP(*(.fini))
                KEEP(*(.eh_frame*))

		. = ALIGN(256);
		__init_end__ = .;
	}

	PROVIDE(einit = __vectors + (__init_end__ - __init_start__));
	PROVIDE(_einit = einit);
	PROVIDE(__einit = einit);

	.text : AT(__einit)
	{
		__text_start__ = .;
		ptext_start = .;
		*picoRTOS.o (.text)
		*picoRTOS_port.o (.text)
		*picoRTOS_portasm.o (.text)
		*mpu_*.o (.text)
		*cache_*.o (.text)

		*picoRTOS-SMP.o (.text)
		*picoRTOS-SMP_port.o (.text)
		*picoRTOS-SMP_portasm.o (.text)

		*(.ptext)

		. = ALIGN(128);
		__runtime_vectors_start__ = .;
		/* vectors in ram (if needed) */
		. = . + (__vectors_end__ - __vectors_start__);

		. = ALIGN(4);
		__runtime_vectors_end__ = .;
		ptext_end = .;
		utext_start = .;
		*(.text*)

		. = ALIGN(4);
		KEEP(*(.rodata*))

		. = ALIGN(4);
		utext_end = .;
		__text_end__ = .;
	} > REGION_TEXT

	PROVIDE(etext = __einit + (__text_end__ - __text_start__));
	PROVIDE(_etext = etext);
	PROVIDE(__etext = etext);
	
	.data : AT(__etext)
	{
                __data_start__ = .;
		pdata_start = .;
		*picoRTOS.o (.data)
		*picoRTOS_port.o (.data)
		*picoRTOS_portasm.o (.data)
		*mpu_*.o (.data)
		*cache_*.o (.data)

		*picoRTOS-SMP.o (.data)
		*picoRTOS-SMP_port.o (.data)
		*picoRTOS-SMP_portasm.o (.data)

		*(.pdata)

		. = ALIGN(4);
		/* join pdata & pbss to save 1 mpu entry
		 * FIXME: very inefficient at startup */
		__pbss_start__ = .;
		__pstack_start__ = .;
		*(.pstack)

		. = ALIGN(4);
		__pstack_end__ = .;
		*picoRTOS.o (.bss)
		*picoRTOS_port.o (.bss)
		*picoRTOS_portasm.o (.bss)
		*mpu_*.o (.bss)
		*cache_*.o (.bss)

		*picoRTOS-SMP.o (.bss)
		*picoRTOS-SMP_port.o (.bss)
		*picoRTOS-SMP_portasm.o (.bss)

		*(.pbss)

		. = ALIGN(4);
		__pbss_end__ = .;
		pdata_end = .;
		KEEP(*(.init_array))
		KEEP(*(.fini_array))
                KEEP(*(.data*))

                . = ALIGN(8);
                PROVIDE(__gp = . + 0x800);
                *(.sdata .sdata.* .gnu.linkonce.s.*);

                . = ALIGN(8);
                PROVIDE(__tp = .);
                *(.tdata .tdata.* .gnu.linkonce.td.*)

                . = ALIGN(4);
		udata_start = .;
		KEEP(*(.udata*))
		
		. = ALIGN(4);
		udata_end = .;
                __data_end__ = .;
	}

	PROVIDE(edata = __etext + (__data_end__ - __data_start__));
	PROVIDE(_edata = edata);
	PROVIDE(__edata = edata);

	.bss :
	{
		__bss_start__ = .;
		. = ALIGN(1024);
		*(.ustack)
		
                *(.bss*)
                *(COMMON)

		. = ALIGN(4);
		__bss_end__ = .;
	}

	/DISCARD/ : { *(.dynsym) }
	/DISCARD/ : { *(.dynstr*) }
	/DISCARD/ : { *(.dynamic*) }
	/DISCARD/ : { *(.plt*) }
	/DISCARD/ : { *(.interp*) }
	/DISCARD/ : { *(.gnu*) }
	/DISCARD/ : { *(.riscv*) }

	/* for picoRTOS.c */
	PROVIDE(pdata_len = pdata_end - pdata_start);
	PROVIDE(udata_len = udata_end - udata_start);
	PROVIDE(ptext_len = ptext_end - ptext_start);
	PROVIDE(utext_len = utext_end - utext_start);
}
