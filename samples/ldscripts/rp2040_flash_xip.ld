/*
 * Memory map for RP2040 (FLASH, eXecute In Place)
 */
MEMORY
{
    flash(rx)      : ORIGIN = 0x10000000, LENGTH = 2048k
    ram(rwx)       : ORIGIN = 0x20000000, LENGTH = 256k
    scratch_x(rwx) : ORIGIN = 0x20040000, LENGTH = 4k
    scratch_y(rwx) : ORIGIN = 0x20041000, LENGTH = 4k
}

PROVIDE(__relocate_text = 0x1);

ENTRY(Reset_Handler)
REGION_ALIAS("FLASH", flash);
REGION_ALIAS("RAM", ram);

/* custom sections first */
SECTIONS
{
    .boot2 :
    {
        __boot2_start__ = .;
        KEEP (*(.boot2))
        __boot2_end__ = .;
    } > FLASH

    .stack1 (COPY):
    {
        *(.stack1*)
    } > scratch_x
    
    .stack0 (COPY):
    {
        *(.stack*)
    } > scratch_y

    __StackTop = ORIGIN(scratch_y) + LENGTH(scratch_y);
    __StackBottom = __StackTop - SIZEOF(.stack0);
    __Stack1Top = ORIGIN(scratch_x) + LENGTH(scratch_x);
    __Stack1Bottom = __Stack1Top - SIZEOF(.stack1);
}

/* common sections for cortex-m */
INCLUDE armvx-m_xip_sections.ld
