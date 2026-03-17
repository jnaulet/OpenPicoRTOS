/*
 * ATSAME70Q21 memory mapping for ROM
 */

MEMORY
{
    flash(rx)    : ORIGIN = 0x00400000, LENGTH = 2048k
    ram(rwx)     : ORIGIN = 0x20400000, LENGTH = 384k
    sram(rwx)    : ORIGIN = 0x40074000, LENGTH = 1k
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
