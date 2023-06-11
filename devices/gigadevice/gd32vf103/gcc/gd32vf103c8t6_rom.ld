/*
 * GD32VF103C8T6 (lite) memory mapping for ROM
 */

MEMORY
{
    boot(rx)    : ORIGIN = 0x1fffb000, LENGTH = 18k
    rom(rx)     : ORIGIN = 0x08000000, LENGTH = 64k
    ram(rwx)    : ORIGIN = 0x20000000, LENGTH = 20k
}

PROVIDE(__relocate_text = 0x0);
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : DEFINED(__stack_size__) ? __stack_size__ : 0x100;

ENTRY(Reset_Handler)

/* Section Definitions */
SECTIONS
{
    .text :
    {
        __text_start__ = .;
        . = ALIGN(4);
        KEEP(*(.init))

        . = ALIGN(512);
        __vectors_start__ = .;
        KEEP(*(.vectors .vectors.*))
        __vectors_end__ = .;

        *(.text .text.* .gnu.linkonce.t.*)
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
        __text_end__ = .;
    } > rom

    . = ALIGN(4);
    __etext = .;
    __sdata = .;

    .data : AT (__sdata)
    {
        . = ALIGN(4);
        __data_start__ = .;
        *(.data .data.*);
        *(.sdata .sdata.* .gnu.linkonce.s.*);
        
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

    .bss (NOLOAD):
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
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
