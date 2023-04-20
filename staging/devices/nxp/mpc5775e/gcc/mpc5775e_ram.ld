/*
 * MPC5775E memory mapping for RAM
 */

MEMORY
{
    rom(rx)   : ORIGIN = 0x01000000, LENGTH = 4096k
    ram(rwx)  : ORIGIN = 0x40000000, LENGTH = 512k
}

PROVIDE(__relocate_text = 0x1);
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : DEFINED(__stack_size__) ? __stack_size__ : 0x100;

ENTRY(Reset_Handler)

SECTIONS
{
    .init :
    {
        __init_start__ = .;
        KEEP(*(.init))

        . = ALIGN(4);
        KEEP(*(.fini));

        . = ALIGN(4096);
        KEEP(*(.vectors .vectors.*))
        *(.rodata .rodata* .gnu.linkonce.r.*)

      . = ALIGN(4);
        *(.eh_frame*)

        . = ALIGN(4);
        __init_end__ = .;
    } > rom

    __etext = .;
    
    .text : AT (__etext)
    {
        __text_start__ = .;
        *(.text .text.* .gnu.linkonce.t.*)

        . = ALIGN(4);
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))

        . = ALIGN(4);
        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))

        . = ALIGN(4096);
        __vectors_start__ = .;
        . = . + 76;
        . = ALIGN(4);
        __vectors_end__ = .;

        . = ALIGN(4);
        __text_end__ = .;
    } > ram

    __sdata = __etext + (__text_end__ - __text_start__);

    .data : AT (__sdata)
    {
        . = ALIGN(4);
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
    } > ram

    .sdata (NOLOAD) :
    {
        . = ALIGN(4);
        __sdata_start__ = .;
        *(.sdata .sdata.*);
        . = ALIGN(4);
        __sdata_end__ = .;
    } > ram

    .sdata2 (NOLOAD) :
    {
        . = ALIGN(4);
        __sdata2_start__ = .;
        *(.sdata2 .sdata2.*);
        . = ALIGN(4);
        __sdata2_end__ = .;
    } > ram

    .bss (NOLOAD):
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss .bss.*)
        *(COMMON)

        . = ALIGN(4096);
        __intc_vectors_start__ = .;
        . = . + 2048;
        __intc_vectors_end__ = .;

        . = ALIGN(4);
        __bss_end__ = .;
    } > ram

    .sbss (NOLOAD):
    {
        . = ALIGN(4);
        __sbss_start__ = .;
        *(.sbss .sbss.*)
        . = ALIGN(4);
        __sbss_end__ = .;
    } > ram

    .stack (NOLOAD):
    {
        . = ALIGN(8);
        __stack_start__ = .;
        . = . + STACK_SIZE;
        . = ALIGN(8);
        __stack_end__ = .;
    } > ram

    __StackTop = __stack_end__;
    __StackBottom = __stack_start__;
}
