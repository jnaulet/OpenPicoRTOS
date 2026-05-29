/*
 * SECTION definitions for all ARM platforms
 * Alignment is 128 by default to match the ARMv6-M minimal MPU region size
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

		. = ALIGN(128);
		__init_end__ = .;
	}

	PROVIDE(einit = __vectors + (__init_end__ - __init_start__));
	PROVIDE(_einit = einit);
	PROVIDE(__einit = einit);

	.text : AT(__einit)
	{
		__text_start__ = .;
		__ptext_start__ = .;
		*picoRTOS.o (.text)
		*picoRTOS_port.o (.text)
		*picoRTOS_portasm.o (.text)
		*mpu_*.o (.text)
		*cache_*.o (.text)

		. = ALIGN(128);
		__runtime_vectors_start__ = .;
		/* vectors in ram (if needed) */
		. = . + (__vectors_end__ - __vectors_start__);

		. = ALIGN(128);
		__runtime_vectors_end__ = .;
		__ptext_end__ = .;
		__utext_start__ = .;
		*(.text*)

		. = ALIGN(128);
		__utext_end__ = .;
		__text_end__ = .;
	} > REGION_TEXT

	PROVIDE(__text_load__ = LOADADDR(.text));
	PROVIDE(etext = __einit + (__text_end__ - __text_start__));
	PROVIDE(_etext = etext);
	PROVIDE(__etext = etext);
	
	.data : AT(__etext)
	{
                __data_start__ = .;
		__pdata_start__ = .;
		*picoRTOS.o (.data)
		*picoRTOS_port.o (.data)
		*picoRTOS_portasm.o (.data)
		*mpu_*.o (.data)
		*cache_*.o (.data)

		. = ALIGN(128);
		/* join pdata & pbss to save 1 mpu entry
		 * FIXME: very inefficient at startup */
		__pbss_start__ = .;
		__pstack_start__ = .;
		*picoRTOS.o (.stack)

		. = ALIGN(128);
		__pstack_end__ = .;
		*picoRTOS.o (.bss)
		*picoRTOS_port.o (.bss)
		*picoRTOS_portasm.o (.bss)
		*mpu_*.o (.bss)
		*cache_*.o (.bss)
		
		. = ALIGN(128);
		__pbss_end__ = .;
		__pdata_end__ = .;
		KEEP(*(.init_array))
		KEEP(*(.fini_array))
                KEEP(*(.data*))

                . = ALIGN(128);
		__udata_start__ = .;
		KEEP(*(.rodata*)) /* FIXME */
		KEEP(*(.udata*))
		
		. = ALIGN(128);
		__udata_end__ = .;
                __data_end__ = .;
	}

	PROVIDE(edata = __etext + (__data_end__ - __data_start__));
	PROVIDE(_edata = edata);
	PROVIDE(__edata = edata);

	.bss :
	{
		__bss_start__ = .;
                *(.bss*)
                *(COMMON)

		. = ALIGN(128);
		__bss_end__ = .;
	}

	/DISCARD/ : { *(.dynsym) }
	/DISCARD/ : { *(.dynstr*) }
	/DISCARD/ : { *(.dynamic*) }
	/DISCARD/ : { *(.plt*) }
	/DISCARD/ : { *(.interp*) }
	/DISCARD/ : { *(.gnu*) }
	/DISCARD/ : { *(.ARM*) }

	/* for picoRTOS.c */
	PROVIDE(__pdata_len__ = __pdata_end__ - __pdata_start__);
	PROVIDE(__udata_len__ = __udata_end__ - __udata_start__);
	PROVIDE(__ptext_len__ = __ptext_end__ - __ptext_start__);
	PROVIDE(__utext_len__ = __utext_end__ - __utext_start__);
	
	/* for startup.S */
	PROVIDE(__StackTop = __pstack_end__);
	PROVIDE(__StackBottom = __pstack_start__);
}
