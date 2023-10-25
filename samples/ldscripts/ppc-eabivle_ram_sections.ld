SECTIONS
{
	.init :
	{
		__init_start__ = .;
		KEEP(*(.init))
		*(.rodata .rodata* .gnu.linkonce.r.*)

		. = ALIGN(4);
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*crtend.o(.ctors))

		. = ALIGN(4);
		KEEP(*(.fini))

		. = ALIGN(4);
		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*crtend.o(.dtors))

		. = ALIGN(4);
		*(.eh_frame*)

		. = ALIGN(4);
		__init_end__ = .;
	} > FLASH

	.sdata2 :
	{
		__sdata2_start__ = .;
		*(.sdata2)
		*(.sdata2.*)

		. = ALIGN(4);
		__sdata2_end__ = .;
	} > FLASH

	__etext = ALIGN(4);

	.data :	 AT (__etext)
	{
		__data_start__ = .;
		*(.data .data.*);

		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP (*(.preinit_array))
		__preinit_array_end = .;

		. = ALIGN(4);
		__init_array_start = .;
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		__init_array_end = .;

		. = ALIGN(4);
		__fini_array_start = .;
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		__fini_array_end = .;

		. = ALIGN(4);
		__data_end__ = .;
	} > RAM

	__edata = __etext + (__data_end__ - __data_start__);

	.sdata : AT (__edata)
	{
		__sdata_start__ = .;
		*(.sdata)
		*(.sdata.*)

		. = ALIGN(4);
		__sdata_end__ = .;
	} > RAM

	__esdata = __edata + (__sdata_end__ - __sdata_start__);

	.text : AT (__esdata)
	{
		__text_start__ = .;
		*(.text .text.* .gnu.linkonce.t.*)

		. = ALIGN(4096);
		__vectors_start__ = .;
		KEEP(*(.vectors))

		. = ALIGN(4);
		__text_end__ = .;
	} > RAM

	.sbss :
	{
		__sbss_start__ = .;
		*(.sbss)
		*(.sbss.*)

		. = ALIGN(4);
		__sbss_end__ = .;
	} > RAM

	.bss :
	{
		__bss_start__ = .;
		*(.bss)
		*(.bss.*)
		*(COMMON)

		. = ALIGN(4096);
		__intc_vectors_start__ = .;
		. = . + (__interrupt_vector_count * 4);
		__intc_vectors_end__ = .;

		. = ALIGN(4);
		__bss_end__ = .;
	} > RAM

	.stack :
	{
		. = ALIGN(8);
		__stack_start__ = .;
		. = . + STACK_SIZE;
		. = ALIGN(8);
		__stack_end__ = .;
	} > RAM

	__StackTop = __stack_end__;
	__StackBottom = __stack_start__;
}
