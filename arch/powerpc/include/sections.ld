/*
 * SECTION definitions for all PPC platforms
 */
SECTIONS
{
	.init :
	{
		__init_start__ = .;
		KEEP(*(.init))
                KEEP(*(.fini))
                KEEP(*(.eh_frame*))

		. = ALIGN(16);
		__init_end__ = .;
	}

	/* vector need 64k alignment & to be in ptext
	* FIXME: find a better solution */
	PROVIDE(einit = ALIGN(0x10000));
	PROVIDE(_einit = einit);
	PROVIDE(__einit = einit);

	.text : AT(__einit)
	{
		__text_start__ = .;
		__ptext_start__ = .;
		*(.vectors)

		*picoRTOS.o (.text)
		*picoRTOS_port.o (.text)
		*picoRTOS_portasm.o (.text)
		*mpu-*.o (.text)
		*cache-*.o (.text)
		*intc-*.o (.text)
		*timer-*.o (.text)

		*picoRTOS-SMP.o (.text)
		*picoRTOS-SMP_port.o (.text)
		*picoRTOS-SMP_portasm.o (.text)
		*core-*.o (.text)
		*spinlock-*.o (.text)

		*(.ptext)

		. = ALIGN(16);
		__ptext_end__ = .;
		__utext_start__ = .;
		*(.text*)
		KEEP(*(.rodata*))

		. = ALIGN(16);
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
		*mpu-*.o (.data)
		*cache-*.o (.data)
		*intc-*.o (.data)
		*timer-*.o (.data)

		*picoRTOS-SMP.o (.data)
		*picoRTOS-SMP_port.o (.data)
		*picoRTOS-SMP_portasm.o (.data)
		*core-*.o (.data)
		*spinlock-*.o (.data)

		*(.pdata)

		. = ALIGN(128);
		/* join pdata & pbss to save 1 mpu entry
		 * FIXME: very inefficient at startup */
		__pbss_start__ = .;
		__pstack_start__ = .;
		*(.pstack)

		. = ALIGN(128);
		__pstack_end__ = .;
		*picoRTOS.o (.bss)
		*picoRTOS_port.o (.bss)
		*picoRTOS_portasm.o (.bss)
		*mpu-*.o (.bss)
		*cache-*.o (.bss)
		*intc-*.o (.bss)
		*timer-*.o (.bss)

		*picoRTOS-SMP.o (.bss)
		*picoRTOS-SMP_port.o (.bss)
		*picoRTOS-SMP_portasm.o (.bss)
		*core-*.o (.data)
		*spinlock-*.o (.bss)

		*(.pbss)

		. = ALIGN(16);
		__pbss_end__ = .;
		__pdata_end__ = .;
		KEEP(*(.init_array))
		KEEP(*(.fini_array))
                KEEP(*(.data*))
                KEEP(*(.sdata*))

                . = ALIGN(16);
		__udata_start__ = .;
		KEEP(*(.udata*))
		
		. = ALIGN(16);
		__udata_end__ = .;
                __data_end__ = .;
	}

	PROVIDE(edata = __etext + (__data_end__ - __data_start__));
	PROVIDE(_edata = edata);
	PROVIDE(__edata = edata);

	.ustack (NOLOAD) :
	{
		. = ALIGN(128);
		__ustack_start__ = .;
		*(.ustack)

		. = ALIGN(16);
		__ustack_end__ = .;
	}

	.bss (NOLOAD) :
	{		
		__bss_start__ = .;
                *(.bss*)
                *(COMMON)

		. = ALIGN(16);
		__bss_end__ = .;
	}

	/DISCARD/ : { *(.dynsym) }
	/DISCARD/ : { *(.dynstr*) }
	/DISCARD/ : { *(.dynamic*) }
	/DISCARD/ : { *(.plt*) }
	/DISCARD/ : { *(.interp*) }
	/DISCARD/ : { *(.gnu*) }
	/DISCARD/ : { *(.iplt*) }
	/DISCARD/ : { *(.rela*) }

	/* for picoRTOS.c */
	PROVIDE(__pdata_len__ = __pdata_end__ - __pdata_start__);
	PROVIDE(__udata_len__ = __udata_end__ - __udata_start__);
	PROVIDE(__ptext_len__ = __ptext_end__ - __ptext_start__);
	PROVIDE(__utext_len__ = __utext_end__ - __utext_start__);
}
