/*
 * SAM3X8E memory mapping for ROM
 */

MEMORY
{
    flash(rx)    : ORIGIN = 0x00000000, LENGTH = 256k
    ram(rwx)     : ORIGIN = 0x20000000, LENGTH = 32k
}

PROVIDE(__relocate_text = 0x0);
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : DEFINED(__stack_size__) ? __stack_size__ : 0x100;

ENTRY(Reset_Handler)
REGION_ALIAS("FLASH", flash);
REGION_ALIAS("RAM", ram);

/* common sections definition */
INCLUDE armvx-m_xip_sections.ld
/* user-defined sections definitions */
SECTIONS
{
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
