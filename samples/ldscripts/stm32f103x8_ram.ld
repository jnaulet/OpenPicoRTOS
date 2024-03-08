/*
 * STM32F103xx memory mapping for RAM
 */

MEMORY
{
    flash(rx)    : ORIGIN = 0x08000000, LENGTH = 64k
    ram(rwx)     : ORIGIN = 0x20000000, LENGTH = 20k
}

PROVIDE(__relocate_text = 0x1);
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : DEFINED(__stack_size__) ? __stack_size__ : 0x100;

ENTRY(Reset_Handler)
REGION_ALIAS("FLASH", flash);
REGION_ALIAS("RAM", ram);

/* common sections definition */
INCLUDE armvx-m_ram_sections.ld
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
