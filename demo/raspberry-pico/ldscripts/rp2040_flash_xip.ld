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

ENTRY(Reset_Handler)
REGION_ALIAS("REGION_TEXT", flash);

SECTIONS
{
    .boot2 :
    {
        __boot2_start__ = .;
        KEEP (*(.boot2))
        __boot2_end__ = .;
    }
}
